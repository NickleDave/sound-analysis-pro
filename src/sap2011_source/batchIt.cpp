//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "batchIt.h"
#include "CZT_FeatureCalc.h"
#include "CZT_FeatureImage.h"
#include "CZT_PcmEq.h"
#include "options3.h"
#include "records.h"
#include "start.h"
#include "animals.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SDL_NumLab"
#pragma link "SDL_rchart"
#pragma link "SDL_plot3d"
#pragma resource "*.dfm"
#define kList1  0
#define kList2  1
#define Xs              1
#define Ys              2
#define Xe              3
#define Ye              4
#define Val             5
#define ValidComp       6

Tbatch *batch;

//---------------------------------------------------------------------------
__fastcall Tbatch::Tbatch(TComponent* Owner)
        : TForm(Owner)
{
  option->module=2;
 /* if (segment == NULL) {
      segment = new TFrame1(NULL);
  } */
  parameters->TabVisible=false;
  sound_files->TabVisible=false;
 // raw_tables->TabVisible=false;
  output->TabVisible=false;
  segmentation->TabVisible=false;
  raw2syll->TabVisible=false;
  rhythm->TabVisible=true;
  review->TabVisible=false;
  options->TabVisible=false;
  progress->TabVisible=false;
  output->TabVisible=false;
  rhythm_settings->TabVisible=false;
  rhythm->TabVisible=false;
  save_criteria->TabVisible=false;
  do_rawsyll_batch->TabVisible=false;

  doing_rhythm_batch=false;

  setslaves->Enabled=false;
  derivTB->Brush->Color=clYellow;
  entropyTb->Brush->Color=clRed;
  sldrBrightness->Brush->Color=(TColor)RGB(0,0,0);//clBlack;
  counting=false;
  option->cepstrum_pitch=true;
  option->save_all=false;
  option->save_image_mem=false;
  option->custom_parse=false;
  option->brain_mode=false;
  option->raw_feature_folder=raw_features1->Text;
  option->frequency=10000;
  Sonogram=new Graphics::TBitmap();
  //RecordFile1->Caption=DataForm->Records->TableName;
  DataForm->Records->Active=true;
  //option->syllable_table_name=DataForm->Records->TableName;
  //RecordsNum->Caption=DataForm->count_records(option->syllable_table_name);
  //DataForm->CountRecsClick(this);
  //RecordsNum->Caption=DataForm->recNum;
  option->bird_ID="none";  // make sure no overwrite happens
  option->chan=0;
  option->move_sound=false;
  option->delete_trash_file=false;
  option->create_feature_arrays=true;//false;
  option->refine_segmentation=false;
  option->keep_file_name=true;
  option->keep_original_rawfile_name=true;
  option->select_table=0;
  option->format->ItemIndex=format->ItemIndex;
  DataForm->MyConnection1->Database="SAP";
 // DataForm->settings->TableName="settings";
  DataForm->settings->Active=true;
  table_features->TabVisible=false;
  select_tables->TabVisible=false;
  slave_chans=0;
  got_data=false;
  //DataForm->new_raw_table("raw_features_default");
  option->format->ItemIndex=2;//get serial number and time from file name...
  option->bitmapHeight=256;
  option->squiz=2;
  m_Calculator.m_Image.m_nMaxSlices=100000;
  ActiveList=kList1;
  DataForm->file_table_name="file_table"; // default
  if (!DirectoryExists("c:\\SAP\\tmpSound\\"))ForceDirectories("c:\\SAP\\tmpSound\\");
  if (!DirectoryExists("c:\\SAP\\raw_features\\"))ForceDirectories("c:\\SAP\\raw_features\\");
  DirectoryListBox1->Directory = "c:\\sap\\sounds";
  try{

	 //derivTB->Position=DataForm->Retrieve(1, "deriv_thresh").ToInt();
	 //option->amplitude_thresh=(125-derivTB->Position);
	 deriv_val->Value=125-derivTB->Position;
	 entropyTb->Position=DataForm->Retrieve(1, "entropy_thresh").ToInt();
	 entropy_val->Value=(float)-entropyTb->Position/50;
	 option->entropy_thresh=(float)(-entropyTb->Position)/50.0;
	 sldrBrightness->Position=DataForm->Retrieve(1, "display_contrast").ToInt();


	 data_window->Position = DataForm->Retrieve(1, "FFT_window").ToInt();
	 advance->Position = DataForm->Retrieve(1, "advance_window").ToInt();
	 frequency_range->Position = DataForm->Retrieve(1, "frequency_range").ToInt();
     option->data_window->Position=DataForm->Retrieve(1, "FFT_window").ToInt();
	 option->data_windowChange(this);
	 option->advance->Position=DataForm->Retrieve(1, "advance_window").ToInt();
	 option->advanceChange(this);
	 option->frequency_range->Position=DataForm->Retrieve(1, "frequency_range").ToInt();
	 option->frequency_rangeChange(this);
	 pitch_method->ItemIndex=option->pitch_method->ItemIndex;

		derivTB->Position = DataForm->Retrieve(1, "deriv_thresh").ToInt();
		int pri_feature = DataForm->Retrieve(1, "pri_segmented_feature").ToInt();
        deriv_val->Value = option->adjustThreshold(pri_feature, derivTB->Position, option->frequency, 1);
        entropyTb->Position = DataForm->Retrieve(1, "entropy_thresh").ToInt();
        int sec_feature = DataForm->Retrieve(1, "sec_segmented_feature").ToInt();
		entropy_val->Value = option->adjustThreshold(sec_feature, entropyTb->Position, option->frequency, 0);
		pri_feature = DataForm->Retrieve(2, "pri_segmented_feature").ToInt();
		sec_feature = DataForm->Retrieve(2, "sec_segmented_feature").ToInt();

		DataForm->Retrieve_segmentation_to_options(1);
		segment_by->ItemIndex = option->pri_segmented_feature1;
		secondaryFeatureDD->ItemIndex = option->sec_segmented_feature1;
		segmentation_flip->ItemIndex = option->reverse_segmentation1;
		secondaryComparisonRB->ItemIndex = option->sec_lt1;
		smoothGraphCB->Checked = option->smooth_feature1;
		adaptThreshCB->Checked = option->adapt_threshold1;
		secondaryFeatureCB->Checked = option->sec_segmentation1;
		secondaryLogicRB->ItemIndex = option->sec_logic1;
		hp_small->Position=option->hp_small1/100;
		hp_big->Position=30*log(option->hp_big1);

		option->pri_segmented_feature = option->pri_segmented_feature1;
		option->pri_thresh_raw = option->pri_thresh_raw1;
		option->reverse_segmentation =  option->reverse_segmentation1;
		option->smooth_feature = option->smooth_feature1;
		option->adapt_threshold = option->adapt_threshold1;
		option->sec_segmentation = option->sec_segmentation1;
		option->sec_segmented_feature = option->sec_segmented_feature1;
		option->sec_thresh_raw =  option->sec_thresh_raw1;
		option->sec_lt = option->sec_lt1;
		option->sec_logic =  option->sec_logic1;
		option->hp_small = option->hp_small1;
		option->hp_big = option->hp_big1;

	 //DataForm->Retrieve_frame_params(segment, 1);

	 segment_by->ItemIndex=DataForm->Retrieve(1, "fine_segmentation").ToInt();
	 //segment_byChange(this);
	 hp_big->Position=DataForm->Retrieve(1, "hp_big").ToInt();
	 hp_bigChange(this);
	 hp_small->Position=DataForm->Retrieve(1, "hp_small").ToInt();
	 hp_smallChange(this);


   }
   catch(Exception& e)
  {
	Application->MessageBox(L"could not open the settings table, default settings will be applied", NULL, MB_OK);

  }
  analysis_summary->Cells[0][0]="Name";
  analysis_summary->Cells[1][0]="age";
  analysis_summary->Cells[2][0]="exp.day";
  analysis_summary->Cells[3][0]="model";
  analysis_summary->Cells[4][0]="# of syll";
  analysis_summary->Cells[5][0]="max syll dur";
  analysis_summary->Cells[6][0]="max bout dur";
  analysis_summary->Cells[7][0]="details";


  time_stamp->Cells[0][0]="File name";
  time_stamp->Cells[1][0]="month";
  time_stamp->Cells[2][0]="day";
  time_stamp->Cells[3][0]="year";
  time_stamp->Cells[4][0]="hour";
  time_stamp->Cells[5][0]="minute";
  time_stamp->Cells[6][0]="second";
  time_stamp->Cells[7][0]="ms";
  time_stamp->Cells[8][0]="time stamp";



  data_typeClick(this);
  update_segmentation_params();

  //batch->WindowState=wsMaximized;
}

//---------------------------------------------------------------------------
void Tbatch::Calc(AnsiString fileName)
{
	 if(option->halt){doit=false; return;}
	 CZT_PcmEq equalizer;
	 if(wave_files[0].OpenFile(fileName.c_str()) != pcmpos_error)
	 {
		  SoundFile=fileName;
		  if(equalize->Checked) wave_files[0].UseFilter(equalizer);

		  if( m_Calculator.Calculate(wave_files,slave_chans-deduct))// || option->save_all)// ||  deduct a missing slave if needed...
		  {
			 m_Calculator.save_to_database(0); // this is the main function...
			 rec1->Color=(TColor)RGB(0,0,0);//clBlack;
			 rec1->Caption="Accepted";
			 files_accepted++;
			 files_acceptedN->Caption=files_accepted;
			 msCounter+=option->ms_recordes_added;
			 msCounterN->Caption=msCounter;
			 file_table_index->Value++;
			 if(do_rhythm->Checked)calc_rhythmClick(this);
		  }
		  else
		  {
				rec1->Color=clGreen;
				rec1->Caption="Rejected";
		  }

		  analysis_summary->Cells[5][1]=option->advanceTb->Value*m_Calculator.maxSyllableDur;
		  analysis_summary->Cells[6][1]=option->advanceTb->Value*m_Calculator.maxBoutDur;
		  analysis_summary->Cells[4][1]=(AnsiString)m_Calculator.syllable_number;
		  if(Draw1->Down)
		  {
			m_Calculator.render_image=true;
			m_Calculator.GetImage().DrawAllSlices(Image1);
		   //	Sonogram->Assign(Image1->Picture->Bitmap);
		  }
		  else m_Calculator.render_image=false;
	 }
 }



//---------------------------------------------------------------------------



void __fastcall Tbatch::SingleFileClick(TObject *Sender)
{
   slave_chans=0;
   Screen->Cursor = crHourGlass;
   int ind=save1->ItemIndex;
   save1->ItemIndex=3;
   DataForm->Records->Active=true;
   option->halt=false;
   Draw1->Down=true;
   Image1->Visible=true;
   m_Calculator.render_image=true;
   //m_Calculator.RenderImage(1);
  // Draw1->Down=true;
   //OpenDialog1->DefaultExt="*.wav";
  // OpenDialog1->Filter="sound files|*.wav";
  // show_sonogram->Checked=true;
  // m_Calculator.render_image=true;
	//m_Calculator.RenderImage(1);
   if(soundList->Items->Count)
   {
	option->moveTo_sound_folders[0]=target_sound_file_folder->Text;
	 entropyTbChange(this);
	 derivTBChange(this);
	 option->wave_names[0]=soundList->FileName;
	 option->extract_file_attributes(soundList->FileName);
	 Calc(soundList->FileName);
	 RecordsNum->Caption=DataForm->count_records(option->syllable_table_name);
	 total_count+=option->syllCounter;
	 counter->Caption=total_count;
	 calc_rhythm->Enabled=true;
   }
   else  MessageDlg("could not open sound file -- please click on a file!", mtInformation, TMsgDlgButtons() << mbOK, 0);
   save1->ItemIndex=ind; // back to normal mode
   Screen->Cursor = crDefault;
}


// This method is activated when the application is idle, and at that time, the batch can make a step.
// batch is activated by the 'doit' global variable.
// for a limited number of operations (do next x files), batch is also conditionsed by the counting global variable.

//---------------------------------------------------------------------------
 /*
void __fastcall Tbatch::MyIdleHandler(TObject *Sender, bool &Done)
{
  try{


        if(counting){back_count--; if(back_count<1){counting=false; doit=false;}} // this will set a limit to number of operations in do next x files mode
        if(table_batch && doit) do_table_batch();
        else if(soundList->ItemIndex<soundList->Items->Count-1 && doit)  processFile(); // make a step, processing next file
        else if(doit) // All the files in the folder where processed
        {
            if(All_subfolders->Checked)// continue across all subfolders
            {
              if(FileListBox1->ItemIndex<FileListBox1->Items->Count-1)
              {
                 FileListBox1->ItemIndex++; // change subfolder
                 FileListBox1Click(this);   // get the files exstracted to 'soundlist'
                 FileListBox1Change(this);
                 Sleep(100);
                 FileListBox1->Refresh();
                 Sleep(100);
                 FileListBox1Change(this);
                // set_slave_directory(1, slave_list1, folder_name1);
                // option->slave_mode[0]=slave_state1->ItemIndex;
  

              }
              else
              {
                doit=false;  // finish the batch
                stop = GetTickCount();
                int dur=stop-start;
                AnsiString sMessage;
                sMessage.printf("done within %d seconds",dur/1000);
                MessageDlg(sMessage.c_str(), mtInformation, TMsgDlgButtons() << mbOK, 0);
                RecordsNum->Caption=DataForm->count_records(option->syllable_table_name);
              }
            }

            else
            {
              st->Brush->Color=clGreen;
              doit=false;
              stop = GetTickCount();
              int dur=stop-start;
              AnsiString sMessage;
              sMessage.printf("done within %d seconds",dur/1000);
              MessageDlg(sMessage.c_str(), mtInformation, TMsgDlgButtons() << mbOK, 0);
              RecordsNum->Caption=DataForm->count_records(option->syllable_table_name);
            }
        }
     }catch(...)
       {
           doit=false;
           MessageDlg("an exception has occured, could not finish the batch",mtWarning, TMsgDlgButtons() << mbOK, 0);
       }
}

*/
void Tbatch::processFile()
{
        if(!first_time) soundList->ItemIndex++;
        files_processedN->Caption=files_processed;
        SoundFile=soundList->FileName;
        option->wave_names[0]=SoundFile;
        num_slaves=0;//lookfor_slave_files();
        /*
        if(wait_for_slaves->ItemIndex && count_slaves() && !num_slaves)
        {
          if(!first_time)soundList->ItemIndex--;
          st->Brush->Color=clRed;
          status->Caption="executing batch";
          return;
        }
        */
        if(first_time) first_time=false;
        // else soundList->ItemIndex++;
        st->Brush->Color=clRed;
        status->Caption="executing batch";
     // AnsiString soundFile=soundList->FileName;

        int old_day=option->day;
        int old_month=option->month;
        int old_year=option->year;
        option->extract_file_attributes(soundList->FileName);
        if(daily_raw_tables->ItemIndex)// change table name every day
        {
           if(old_day!=option->day || old_month!=option->month || old_year!=option->year)
           {
              // find if we already have a date in the table name by counting _
              int i, counter=0;
              str=daily_raw_name->Text;
              for(i=1;i<str.Length();i++)
              {
                 if(str[i]=='_')counter++;
                 if(counter==2)break;
              }
              if(counter==2)str=str.SubString(1,i-1);
              daily_raw_name->Text=str;
              //daily_raw_name->Text=raw_features1->Text+"_"+option->month+"_"+option->day+"_"+option->year;
              daily_raw_name->Text=daily_raw_name->Text+"_"+option->month+"_"+option->day+"_"+option->year;
              str=daily_raw_name->Text;
              option->features_table=daily_raw_name->Text;
              if((save1->ItemIndex==0 || save1->ItemIndex==2) && !DataForm->table_exists(str))
              {
                m_Calculator.last_ms_record=0;
                str=DataForm->new_raw_table(str,slave_chans);
               // daily_raw_name->Text=str;
                //raw_features1->Text=str;
                ms_table_name->Caption=str;
                option->features_table=str;
                m_Calculator.last_ms_record=0;
              }
              else // table exists
              {   // but the slave_chans do not exist!
                 if((save1->ItemIndex==0 || save1->ItemIndex==2) && slave_chans && !DataForm->chans_exist(str, slave_chans))
                 {
                   m_Calculator.last_ms_record=0;
                   str=DataForm->new_raw_table(str,slave_chans); // create a new raw table
                 }
              }
              raw_features1->Text=str;
              ms_table_name->Caption=str;
              option->features_table=str;
           }
        }
        files_processed++;
        Calc(SoundFile); // this will also do:  m_Calculator.save_to_database();
        total_count+=option->syllCounter;
        counter->Caption=total_count;
        counter->Repaint();
        batch->Refresh();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


void __fastcall Tbatch::FormClose(TObject *Sender, TCloseAction &Action)
{
        option->show_start=true;
        StartForm->Visible=true;
        delete Sonogram;
       // delete batch;
}



void __fastcall Tbatch::FormCreate(TObject *Sender)
{
   doit=false;
   //Application->OnIdle = MyIdleHandler;
   PitchBClick(this);
   EntropyBClick(this);
   FMBClick(this);
   AMClick(this);
   PeakFrBClick(this);
   PitchGoodnessBClick(this);
   sldrBrightnessChange(this);
   option->Draw_sonogram=false;
}


// this method only activates the batch by setting the global variable 'doit' to true.
// to see the activation of batch routines, go to the MyIdleHandler method. This indirection
// is an alternative to setting a seperate thread, allowing user to stop the batch.void __fastcall Tbatch::derivTBChange(TObject *Sender)

void __fastcall Tbatch::derivTBChange(TObject *Sender)
{
	plot_sonogram();
	/*
	//EntropyB->Down=true;
	//DerivPowerB->Down=true;
	int pos=derivTB->Position;
	//option->amplitude_thresh=125-derivTB->Position;
	m_Calculator.GetImage().DrawAllSlices(Image1);
	Image1->Canvas->Pen->Color=clYellow;
	Image1->Canvas->MoveTo(0,pos);
	Image1->Canvas->LineTo(Image1->Width,pos);
	deriv_val->Value=125-derivTB->Position;
	segment->NumLab3->Value=deriv_val->Value;
	segment->save->OnClick(this);

	int feature = segment->segment_by->ItemIndex;
	float factor = option->frequency_range_val->Value/125;

   prototype:
   option->sec_thresh_raw = ent_TB->Position;
ent_vl->Value = option->adjustThreshold(frame->secondaryFeatureDD->ItemIndex, ent_TB->Position, frequency_val->Value, 0);
frame->secondaryThresholdTB->Value = ent_vl->Value;

	switch(feature) {
	case 0:
		option->Amplitude_thresh->Position = derivTB->Position; // we have the same slider in options screen and we make it the same
		//option->amplitude_thresh = (125 - derivTB->Position) / 1.25;
		deriv_val->Value = (125 - derivTB->Position) / 1.25;//option->amplitude_thresh;
		DerivPowerB->Down = true;
		if (option->sec_segmentation) m_Calculator.GetImage().GetDerivPow().bRender = true;
		DerivPowerBClick(this);
		break;

	case 1:
		deriv_val->Value = (125 - derivTB->Position) * factor;
		if (option->sec_segmentation)m_Calculator.GetImage().GetPitch().bRender = true;
		PitchBClick(this);
		break;

	case 2:
		deriv_val->Value = (125 - derivTB->Position) * factor;
		DerivPowerB->Down = true;
		if (option->sec_segmentation)m_Calculator.GetImage().GetPeakFreq().bRender = true;
		DerivPowerBClick(this);
		break;

	case 3:
		deriv_val->Value = (125 - derivTB->Position) * 36;
		PitchGoodnessB->Down = true;
		if (option->sec_segmentation)m_Calculator.GetImage().GetPitchGoodness().bRender = true;
		PitchGoodnessBClick(this);
		break;

	case 4:
		deriv_val->Value = (125 - derivTB->Position) * 0.72;
		FMB->Down = true;
		if (option->sec_segmentation)m_Calculator.GetImage().GetFM().bRender = true;
		FMBClick(this);
		break;

	case 5:
		deriv_val->Value = (62.5 - (float)derivTB->Position) / 62.5;
		AM->Down = true;
		if (option->sec_segmentation)m_Calculator.GetImage().GetAM().bRender = true;
		AMClick(this);
		break;

	case 6:
		deriv_val->Value = (-derivTB->Position) * 0.064;
		EntropyB->Down = true;
		if (option->sec_segmentation)m_Calculator.GetImage().GetEntropy().bRender = true;
		EntropyBClick(this);
		break;

	case 7:
		deriv_val->Value = (125 - derivTB->Position) * 2.4;
		//continulity_tB->Down = true;
		if (option->sec_segmentation)m_Calculator.GetImage().Getcontinuity_t().bRender = true;
		//continulity_tBClick(this);
		break;

	case 8:
		deriv_val->Value = (125 - derivTB->Position) * 2.4;
		//continuity_fB->Down = true;
		if (option->sec_segmentation)m_Calculator.GetImage().Getcontinuity_f().bRender = true;
		//continuity_fBClick(this);
		break;

	default:
		option->Amplitude_thresh->Position = derivTB->Position; // we have the same slider in options screen and we make it the same
		//option->amplitude_thresh = (125 - derivTB->Position) * 1.25;
		deriv_val->Value = (125 - derivTB->Position) * 1.25;//option->amplitude_thresh; // (125-deriv_TB->Position);//deriv_TB->Position;
		DerivPowerB->Down = true;
		if (option->sec_segmentation)m_Calculator.GetImage().GetDerivPow().bRender = true;
		DerivPowerBClick(this);
	}


	if (option->sec_segmentation) {
		int featureSec = segment->secondaryFeatureDD->ItemIndex;
		switch(featureSec) {
		case 0:
			DerivPowerB->Down = true;
			m_Calculator.GetImage().GetDerivPow().bRender = true;
			break;
		case 1:
			PitchB->Down = true;
			m_Calculator.GetImage().GetPitch().bRender = true;
			break;
		case 2:
			DerivPowerB->Down = true;
			m_Calculator.GetImage().GetPeakFreq().bRender = true;
			break;
		case 3:
			PitchGoodnessB->Down = true;
			m_Calculator.GetImage().GetPitchGoodness().bRender = true;
			break;
		case 4:
			FMB->Down = true;
			m_Calculator.GetImage().GetFM().bRender = true;
			break;
		case 5:
			AM->Down = true;
			m_Calculator.GetImage().GetAM().bRender = true;
			break;
		case 6:
			EntropyB->Down = true;
			m_Calculator.GetImage().GetEntropy().bRender = true;
			break;
		case 7:
		   //	continulity_tB->Down = true;
			m_Calculator.GetImage().Getcontinuity_t().bRender = true;
			break;
		case 8:
			//continuity_fB->Down = true;
			m_Calculator.GetImage().Getcontinuity_f().bRender = true;
			break;
		default:
			DerivPowerB->Down = true;
			m_Calculator.GetImage().GetDerivPow().bRender = true;
		}
	}
	option->pri_segmented_feature = segment->segment_by->ItemIndex;
	option->pri_thresh_raw = deriv_val->Value;
	option->reverse_segmentation = segment->segmentation_flip->ItemIndex;
	option->doNotSegment = false;
	segment->NumLab3->Value = deriv_val->Value;

		if (option->smooth_feature) {
			m_Calculator.do_hpfilt();
			m_Calculator.Render_sonogram_images();
			if (!(option->paginationOn)) {
				 m_Calculator.GetImage().DrawAllSlices(Image1);
			} else {
				m_Calculator.GetImage().DrawSlices(0, 1000-1, Image1);
			}
		}

		if (option->adapt_threshold ) {
			m_Calculator.do_hpfilt(); // recalc_light(0);
			m_Calculator.Render_sonogram_images();
			m_Calculator.GetImage().Gethp_big().bRender = option->adapt_threshold;
			if (!(option->paginationOn)) {
				m_Calculator.GetImage().DrawAllSlices(Image1);
			} else m_Calculator.GetImage().DrawSlices(0,  1000-1, Image1);
		}


		for (int i = 0; i < recnum; i++) m_Calculator.sound_detect_feature(i);
		m_Calculator.GetImage().DrawAllSlices(Image1);
		if (!option->adapt_threshold ) {
			Image1->Canvas->Pen->Color = clYellow;
			Image1->Canvas->MoveTo(0, pos);
			Image1->Canvas->LineTo(Image1->Width, pos);
			//show_outlines(image, onset, offset);
		}
		if (option->sec_segmentation) {
			Image1->Canvas->Pen->Color = clAqua; // (TColor)RGB(0, 0, 0);
			int posSec = 20 + entropyTb->Position * 9 / 5;
			Image1->Canvas->MoveTo(0, posSec);
			Image1->Canvas->LineTo(Image1->Width, posSec);
			//show_outlines(image, onset, offset);
		}
   //	updateSegmentation = false;
   //	updateSegmentation2=false;
    */
}
//---------------------------------------------------------------------------


void __fastcall Tbatch::stretchClick(TObject *Sender)
{
if(stretch->ItemIndex)
	   {Image1->AutoSize=true;Image1->Stretch=false;}
  else {Image1->AutoSize=false; Image1->Stretch=true; Image1->Width=ScrollBox1->Width-10;  }
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::sldrBrightnessChange(TObject *Sender)
{
 float bright=exp((float)sldrBrightness->Position/700);
 m_Calculator.GetImage().SetBrightness(bright);//float(sldrBrightness->Position/39.0625));
     m_Calculator.GetImage().DrawAllSlices(Image1);
}
//---------------------------------------------------------------------------

// batch only a certain number of sound files
void __fastcall Tbatch::batch_countClick(TObject *Sender)
{
  /*
  breakIt->Enabled=true;
  files_processed=0;
  files_processedN->Caption=0;
  files_acceptedN->Caption=0;
  msCounter=0;
  batch_controls->ActivePage=progress;
  Draw1Click(this);
  //batch_controls->Visible=false;

  if(save_file_table->ItemIndex)//(auto_file_table->Checked)
  {
    //str="file_table_"; str+=analysis_summary->Cells[0][1];//syllable_table_name->Caption;
    DataForm->file_table_name=file_table_name1->Text;//str;
    DataForm->Create_File_Table();
    file_table_name->Caption=file_table_name1->Text;//str;
  }

  slave_chans=count_slaves();
  slave_numL->Caption=slave_chans;
  if(save1->ItemIndex==3)MessageDlg("Warning: Are you sure? The save option is off and no data will be saved...", mtWarning, TMsgDlgButtons() << mbOK, 0);
  first_time=true;
  option->halt=false;
  DataForm->Records->Active=true;
   {
     str = InputBox("Sound Analysis Pro", "Animal name:", "Please type animal name (e.g., R109");
     option->bird_ID=str;//.ToIntDef(0);
     if(option->bird_ID=="")return;
   }
   //Draw1->Down=false;
   entropyTbChange(this);
   derivTBChange(this);
   start=GetTickCount();
   m_Calculator.render_image=false;
   back_count=back_counter->Position;
   counting=true;
   DataForm->update_file_table_index(0);
   doit=true;
   */
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::entropyTbChange(TObject *Sender)
{
 plot_sonogram();
 /*
  //if(live_update->Down)
  {
	EntropyB->Down=true;
	DerivPowerB->Down=true;
	int pos=entropyTb->Position/3.6;
	option->entropy_thresh=(float)(-entropyTb->Position)/50.0;
	entropy_val->Value=(float)-entropyTb->Position/50;
	m_Calculator.GetImage().DrawAllSlices(Image1);
	Image1->Canvas->Pen->Color=clRed;
	Image1->Canvas->MoveTo(0,pos);
	Image1->Canvas->LineTo(Image1->Width,pos);
	Image1->Canvas->Pen->Color=clYellow;
	pos=derivTB->Position;
	Image1->Canvas->MoveTo(0,pos);
	Image1->Canvas->LineTo(Image1->Width,pos);

  }
  else
  {
    Image1->Picture->Bitmap=Sonogram;
    int pos=entropyTb->Position/3.6;
    option->entropy_thresh=(float)(-entropyTb->Position)/50.0;
    Image1->Canvas->Pen->Color=clRed;
    Image1->Canvas->MoveTo(0,pos);
    Image1->Canvas->LineTo(Image1->Width,pos);
    Image1->Canvas->Pen->Color=clYellow;
    pos=derivTB->Position;
    Image1->Canvas->MoveTo(0,pos);
    Image1->Canvas->LineTo(Image1->Width,pos);
    entropy_val->Value=(float)-entropyTb->Position/100.0;
  }    */
  //DataForm->Settings(1, "entropy_thresh", entropyTb->Position);
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::save1Click(TObject *Sender)
{
        switch(save1->ItemIndex)
       {
         case 0:option->saveRawFile=true;
                option->saveRecFiles=true;
                m_Calculator.last_ms_record=0;
                //DataForm->new_raw_table(daily_raw_name->Text, slave_chans);
                //ms_table_name->Caption=raw_features1->Text;//str;
                batch_saving->Caption="syllable and raw features tables";
                break;
         case 1:option->saveRawFile=false; option->saveRecFiles=true;
                batch_saving->Caption="syllable table";
                break;
         case 2:option->saveRawFile=true; option->saveRecFiles=false;
                m_Calculator.last_ms_record=0;
                //DataForm->new_raw_table(daily_raw_name->Text, slave_chans);
                //ms_table_name->Caption=raw_features1->Text;//str;
                batch_saving->Caption="raw features tables";
                break;
         case 3:option->saveRawFile=false; option->saveRecFiles=false;
                batch_saving->Caption="nothing";
       }
}
//---------------------------------------------------------------------------


void __fastcall Tbatch::PitchBClick(TObject *Sender)
{

	 m_Calculator.GetImage().GetPitch().bRender = PitchB->Down;
	 m_Calculator.GetImage().DrawAllSlices(Image1);

}
//---------------------------------------------------------------------------

void __fastcall Tbatch::EntropyBClick(TObject *Sender)
{
     m_Calculator.GetImage().GetEntropy().bRender = EntropyB->Down;
     m_Calculator.GetImage().DrawAllSlices(Image1);
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::FMBClick(TObject *Sender)
{
     m_Calculator.GetImage().GetFM().bRender = FMB->Down;
     m_Calculator.GetImage().DrawAllSlices(Image1);
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::PeakFrBClick(TObject *Sender)
{
     m_Calculator.GetImage().GetPeakFreq().bRender = PeakFrB->Down;
     m_Calculator.GetImage().DrawAllSlices(Image1);
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::PitchGoodnessBClick(TObject *Sender)
{
     m_Calculator.GetImage().GetPitchGoodness().bRender = PitchGoodnessB->Down;
     m_Calculator.GetImage().DrawAllSlices(Image1);
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::DerivPowerBClick(TObject *Sender)
{
m_Calculator.GetImage().GetDerivPow().bRender = DerivPowerB->Down;
     m_Calculator.GetImage().DrawAllSlices(Image1);
}
//---------------------------------------------------------------------------



void __fastcall Tbatch::breakItClick(TObject *Sender)
{
  //batch_controls->Visible=true;
  breakIt->Enabled=false;
  doit=false;
  option->custom_parse=false;
  st->Brush->Color=clGreen;
  status->Caption="The batch is now complete";
  RecordsNum->Caption=DataForm->count_records(option->syllable_table_name);
}
//---------------------------------------------------------------------------



void __fastcall Tbatch::back_counterChange(TObject *Sender)
{
  files_to_do->Value=back_counter->Position;
}
//---------------------------------------------------------------------------


void __fastcall Tbatch::save_optionsClick(TObject *Sender)
{
  //if(save_options->ItemIndex)option->saveRecFiles=true;
 // else option->saveRecFiles=false;
}
//---------------------------------------------------------------------------


void __fastcall Tbatch::raw_features1Change(TObject *Sender)
{
  option->raw_feature_folder=raw_features1->Text;
}
//---------------------------------------------------------------------------


void __fastcall Tbatch::options1Click(TObject *Sender)
{
  option->Visible=true;
  option->BringToFront();
  option->show_start=false;
}
//---------------------------------------------------------------------------


void __fastcall Tbatch::Delete_recsClick(TObject *Sender)
{
  if (MessageDlg("Are you sure? All records will be deleted!", mtWarning, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
  {
    DataForm->Records->Active=false;
    DataForm->Records->EmptyTable();
    DataForm->Records->Active=true;
    RecordFile1->Caption=option->syllable_table_name;
    RecordsNum->Caption=DataForm->count_records(option->syllable_table_name);
  }
}
//---------------------------------------------------------------------------




void __fastcall Tbatch::test_folderClick(TObject *Sender)
{
if (!DirectoryExists(option->moveTo_sound_folders[0]))
		{if(!ForceDirectories(option->moveTo_sound_folders[0])) Application->MessageBox(L"Sorry, cannot create folder. Please check if disk is accessible", NULL, MB_OK);
		  else Application->MessageBox(L"Folder created, Okay!", NULL, MB_OK);
        }
   else Application->MessageBox(L"Folder in place, Okay!", NULL, MB_OK);
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::target_sound_file_folderChange(TObject *Sender)
{
  option->moveTo_sound_folders[0]=target_sound_file_folder->Text;
}
//---------------------------------------------------------------------------






void __fastcall Tbatch::data_typeClick(TObject *Sender)
{
     if(data_type->ItemIndex)  // two pannels
     {
         FileListBox1->Visible=true;
         //label4->Visible=true;
         All_subfolders->Visible=true;
         All_subfolders->Checked=true;
         soundList->Top=171;
         soundList->Height=214;
         DirectoryListBox1->FileList=FileListBox1;
     }
     else                     // one pannel
     {
        FileListBox1->Visible=false;
        //label4->Visible=false;
        All_subfolders->Checked=false;
        All_subfolders->Visible=false;
        soundList->Top=39;
        soundList->Height=330;
        DirectoryListBox1->FileList=soundList;
     }
//   if(data_type->ItemIndex){soundList->Mask="*.dat";option->format->ItemIndex=0;}
 //  else {soundList->Mask="*.wav";option->format->ItemIndex=1;}
}
//---------------------------------------------------------------------------




void __fastcall Tbatch::Draw1Click(TObject *Sender)
{
  if(Draw1->Down)
  {
	   m_Calculator.render_image=true; m_Calculator.RenderImage(1);

  }
  else {m_Calculator.render_image=false;  m_Calculator.RenderImage(0);}
  Draw2->Down=Draw1->Down;
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::rawfile_nameClick(TObject *Sender)
{
  if(rawfile_name->ItemIndex)option->keep_original_rawfile_name=false;
  else option->keep_original_rawfile_name=true;
}
//---------------------------------------------------------------------------

    

void __fastcall Tbatch::BitBtn3Click(TObject *Sender)
{
   soundList->Directory=Edit1->Text;
}
//---------------------------------------------------------------------------


void __fastcall Tbatch::Edit1Click(TObject *Sender)
{
   Edit1->Clear();
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::new_tableClick(TObject *Sender)
{
  // need work...
  batch->Enabled=true;
  batch_count->Enabled=true;
  database=DataForm->MyConnection1->Database;
  //DataForm->new_tableClick(this);
  str = InputBox("Bird name", "Insert Bird Name:", "bird");
  analysis_summary->Cells[0][1]=str;//DataForm->TheTable->TableName;
  option->saveRawFile=false;
  //update_table_names();

}
//---------------------------------------------------------------------------

void __fastcall Tbatch::toExcelClick(TObject *Sender)
{
  DataForm->ToExelClick(this);
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::toMatlabClick(TObject *Sender)
{
  DataForm->Visible=true;
  DataForm->BringToFront();
  DataForm->show_start=false;
  DataForm->datapages->ActivePageIndex=1;
}
//---------------------------------------------------------------------------



void __fastcall Tbatch::sound_output1Click(TObject *Sender)
{
   OpenDialog1->FileName="not relevant";
   if(OpenDialog1->Execute()) target_sound_file_folder->Text=ExtractFileDir(OpenDialog1->FileName)+"\\";

}
//---------------------------------------------------------------------------

void __fastcall Tbatch::features_output1Click(TObject *Sender)
{

     str = InputBox("Sound Analysis Pro", "raw features table:", "Please type raw-feature table name, starting with a letter");
     m_Calculator.last_ms_record=0;
     DataForm->new_raw_table(str, count_slaves());
     raw_features1->Text=str;
     ms_table_name->Caption=str;
     option->features_table=str;

}
//---------------------------------------------------------------------------

void __fastcall Tbatch::test2Click(TObject *Sender)
{
    if (!DirectoryExists(option->moveTo_sound_folders[0]))
    {
      if(!ForceDirectories(option->moveTo_sound_folders[0])) Application->MessageBox(L"Sorry, cannot create sound data folder. Please check if disk is accessible", NULL, MB_OK);
      else MessageDlg("Folder created for sound data, Okay!", mtInformation, TMsgDlgButtons() << mbOK, 0);

    }
   //else MessageDlg("Folder for sound data is in place, Okay!", mtInformation, TMsgDlgButtons() << mbOK, 0);
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::BitBtn10Click(TObject *Sender)
{
  str="Once you analyzed sound data and saved the raw features in a binary file \r";
  str+="you may use that file to re-analyze your data very quickly. \r \r";
  str+="For example, you may want to re-segment your sound to syllables using a variety \r";
  str+="of parameter setting (e.i., using differet values of 'derivative power threshold \r";
  str+="Perfoming this analysis on the binary files (as opposed to the sound files) can save \r";
  str+="about 90% of the computation time.";
  MessageDlg(str, mtInformation, TMsgDlgButtons() << mbOK, 0);

}
//---------------------------------------------------------------------------

void __fastcall Tbatch::set_slavesClick(TObject *Sender)
{
  batch_controls->ActivePage=setslaves;
 /*
  str="Sound Analysis use MySQL tables to store syllable features. The default table for storing the data \r";
  str+="is 'recs0'. Obviously, it is better to set a different table for each experiment. \r \r";
  str+=" * If you already created such a table, you can open it by clicking 'change table'. \r \r";
  str+=" * You can create a new table by clicking 'new table' based on a tamplete table \r \r";
  str+="   It is esential to select the appropriate template, we strongly recomend you use either \r";
  str+="      -- 'syll_tamplete' to create a new empty table \r";
  str+="      -- a table name you already used, in such a case, the new table will contain both \r";
  str+="          the format (fields) and the data of the existing table. \r";
  MessageDlg(str, mtInformation, TMsgDlgButtons() << mbOK, 0);
  */
}
//---------------------------------------------------------------------------


void __fastcall Tbatch::BitBtn4Click(TObject *Sender)
{
  str="Sound Analysis can save both raw and segmented features:";
  str+="1. The feaures of each segmented syllable are saved to a table";
  str+="2. The raw features of each FFT time window can be save to a binary file.";
  str+="     Those files can be then used to re-segment syllables, as if the  sound";
  str+="     has been re-analyzed, but much faster.";
  MessageDlg(str, mtInformation, TMsgDlgButtons() << mbOK, 0);
}
//---------------------------------------------------------------------------


void __fastcall Tbatch::BitBtn6Click(TObject *Sender)
{
  str="When this option is selected, all the files that where 'accepted' as relevant are moved";
  str+="to the folder you select. Note that if your sounds cannot be moved, they will be copied.";
  MessageDlg(str, mtInformation, TMsgDlgButtons() << mbOK, 0);
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::BitBtn9Click(TObject *Sender)
{
  str="Plesae select sampling rate. This version of Sound Analysis only support sounds sampled at these rates";
  MessageDlg(str, mtInformation, TMsgDlgButtons() << mbOK, 0);
}
//---------------------------------------------------------------------------


void __fastcall Tbatch::help10Click(TObject *Sender)
{
  str="These two sliders control the segmentation of the sound to syllable and bout units. \r ";
  str+="Syllable units are underlined by a light blue color, and bouts are underlined by a red color.\r";
  str+="Note that additional constraints on segmentation are set in the option pages. In particular, \r";
  str+="if 'minimum peak frequency' in 'detection of animal sound' is lower than a threshold, sound \r";
  str+="wont be considered as a syllable regardless of these threshold settings. \r \r";
  str+="Note that derivative power and Wiener entropy thresholds can be related by 'or/end'.  \r";
  str+="That is, segmentation can be done by satisfying either one or both thresholds.";
  MessageDlgPos(str, mtInformation, TMsgDlgButtons() << mbOK, 0, 140,162);
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::AMClick(TObject *Sender)
{
     m_Calculator.GetImage().GetAM().bRender = AM->Down;
	 m_Calculator.GetImage().DrawAllSlices(Image1);
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::start_formBClick(TObject *Sender)
{
StartForm->Visible=true;
   StartForm->BringToFront();
}
//---------------------------------------------------------------------------


void __fastcall Tbatch::BitBtn5Click(TObject *Sender)
{
str="This option will overide the determination to 'accept of reject' a sound file, all sounds will be treated as 'accepted'";
str+=". This means that records will be added to the database and raw feature files will be saved, and so forth, regardless of the" ;
str+="content of the files. For example, even a file with one short syllable will be saved.";
  MessageDlg(str, mtInformation, TMsgDlgButtons() << mbOK, 0);
}
//---------------------------------------------------------------------------


void __fastcall Tbatch::DirectoryListBox1Change(TObject *Sender)
{
  if(!data_type->ItemIndex)
  {
        soundList->ItemIndex=0;
       // identify_bird();
        return;
  }
  if(FileListBox1->Items->Strings[0]=="[..]" || FileListBox1->Items->Strings[0]=="[.]")FileListBox1->Items->Delete(0);
  if(FileListBox1->Items->Strings[0]=="[..]" || FileListBox1->Items->Strings[0]=="[.]")FileListBox1->Items->Delete(0);
  if(FileListBox1->Items->Count)
  {
   FileListBox1->ItemIndex=0;
   FileListBox1Click(this);
   soundList->Directory=active_folder->Text;
  }
  else soundList->Clear();
  if(soundList->Items->Count)soundList->ItemIndex=0;

}
//---------------------------------------------------------------------------


void __fastcall Tbatch::FileListBox1Change(TObject *Sender)
{
  soundList->Directory=active_folder->Text;
  soundList->ItemIndex=0;
  /*
  if(slave_list1->ItemIndex)set_slave_directory(1, slave_list1, folder_name1);
  if(slave_list2->ItemIndex)set_slave_directory(2, slave_list2, folder_name2);
  if(slave_list3->ItemIndex)set_slave_directory(3, slave_list3, folder_name3);
  if(slave_list4->ItemIndex)set_slave_directory(4, slave_list4, folder_name4);
  if(slave_list5->ItemIndex)set_slave_directory(5, slave_list5, folder_name5);
  if(slave_list6->ItemIndex)set_slave_directory(6, slave_list6, folder_name6);
  if(slave_list7->ItemIndex)set_slave_directory(7, slave_list7, folder_name7);
  if(slave_list8->ItemIndex)set_slave_directory(8, slave_list8, folder_name8);
  // identify_bird();
  */
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::FileListBox1Click(TObject *Sender)
{
  AnsiString str=FileListBox1->FileName;
  int i=1; do{if(str[i]=='[' || str[i]==']') str.Delete(i,1); i++;}while(i<=str.Length());
  str+="\\";
  active_folder->Text=str;
  soundList->ItemIndex=0;
  soundList->Directory=active_folder->Text;
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::resegmentClick(TObject *Sender)
{
  breakIt->Enabled=true;
  total_count=0;
  save1->ItemIndex=1; // save syllable tables only
  option->create_feature_arrays=false;
  Get_raw_Table_attributes(); // retrieve simple attributes such as table name, number of records, etc.
  st->Brush->Color=clRed;
  status->Caption="executing batch";
  st->Repaint();
  batch->Repaint();
  batch->Refresh();
  AnsiString SyllTable=RecordFile1->Caption;
  option->create_feature_arrays=false;
  database=DataForm->MyConnection1->Database;
  change_the_table=false;
  batch_controls->ActivePage=progress;
  get_age();
  doit=true;
}


//---------------------------------------------------------------------------


void __fastcall Tbatch::open_raw_tableClick(TObject *Sender)
{
  DataForm->openTableClick(this);
  populate_raw_tables();

}
//---------------------------------------------------------------------------

void Tbatch::populate_raw_tables()
{
  DataForm->showTables();
  tables->Clear();
  DataForm->MyQuery1->First();
  for (int i=0; i < DataForm->MyQuery1->RecordCount; i++)
    {
     str=DataForm->MyQuery1->Fields->Fields[0]->AsString;
     if(str[1]=='r' && str[2]=='a' && str[3]=='w')tables->Items->Add(str);
     DataForm->MyQuery1->Next();
    }
    tables->ItemIndex=0;
   // Get_raw_Table_attributes();

}



void Tbatch::Get_raw_Table_attributes()
{
      option->move_sound=false;
      // find raw table name, database, and number of records in the table
      DataForm->TheTable->Active=false;
      DataForm->TheTable->TableName= table_batch_list->Items->Strings[table_batch_list->ItemIndex];
      Raw_Table=DataForm->TheTable->TableName;
      DataForm->TheTable->Active=true;
      //DataForm->CountRecsClick(this);
      recnum=DataForm->count_records(table_batch_list->Items->Strings[table_batch_list->ItemIndex]);//DataForm->recNum;
      raw_features_database=DataForm->MyConnection1->Database;
      DataForm->initiate_handler(raw_features_database, Raw_Table); // open the handler for the table, go to first record...
     if(option->syllable_table_name.Length())
     {
      DataForm->TheTable->Active=false;
      DataForm->TheTable->TableName=option->syllable_table_name;
      DataForm->TheTable->Active=true;
      }// go back to the syllable table
}



void Tbatch::Get_brain_table_attribute()
{
      /*
       option->move_sound=false;
      // find raw table name, database, and number of records in the table
      DataForm->TheTable->Active=false;
      DataForm->TheTable->TableName= multi_tables_list->Items->Strings[multi_tables_list->ItemIndex];
      Raw_Table=DataForm->TheTable->TableName;
      DataForm->TheTable->Active=true;
      //DataForm->CountRecsClick(this);
      recnum=DataForm->count_records(multi_tables_list->Items->Strings[multi_tables_list->ItemIndex]);//DataForm->recNum;
      raw_features_database=DataForm->MyConnection1->Database;
      DataForm->initiate_handler(raw_features_database, Raw_Table); // open the handler for the table, go to first record...

     if(option->syllable_table_name.Length())
     {
      DataForm->TheTable->Active=false;
      DataForm->TheTable->TableName=option->syllable_table_name;
      DataForm->TheTable->Active=true;
     }
     */
     //get_age();
}







/*
   This function take advantage of the CZT_featureCalc utility fucntions to generate syllable table the raw
   feature tables
   1. retrieve epochs of 50000 records from the raw table into a querry
   2. retrieve one record at a time from the querry into the m_ variables
   3. call parse_sound() in CZT_featureCalc, as we do usually with wave file analyzed -- that's it!
   The only other issue this function needs to follow is the stitching points from one wave file to the next
   in the raw features table
*/
void Tbatch::do_table_batch()
{
   st->Brush->Color=clRed;
   status->Caption="executing batch";
   int old_file_index=-1;
   static int current_rec=0, cumulative_rec=0, file_index=-999; // raw features are saved in raw feature tables, that contain 1ms features
   // data across many epochs of data. Each epoch is derived from one sound file. Those are the 'embeded file data'.
   // Let change_table, be an index of a record within an embeded file in the raw-features table
   static bool change_the_table=false;
   // we change table once a table analysis is complete, move to the next one in the listbox.
   m_Calculator.features.bird_ID=birdname->Text;
   option->bird_ID=birdname->Text;
   if(change_the_table)
   {
      if(table_batch_list->ItemIndex<table_batch_list->Items->Count-1)
      {
        get_age();
        //option->day++;
        table_batch_list->ItemIndex++;
        Get_raw_Table_attributes();
        cumulative_rec=0;
        current_rec=0; // this will be the first record of the new embeded sound file we discovered
        change_the_table=false;
      }
      else // no more tables to do...
      
      {
         doit=false; // finish the batch
         table_batch=false;
         st->Brush->Color=clGreen;
         status->Caption="The batch is now complete";
         option->create_feature_arrays=true;
      }
   }// end if(change_table)
   if(doit==false)return;

   // this is the main loop of calculations (within a daily raw table):
   int iterations; // these are iteration loops of 50000 iterations, that break the table to smaller units.
   short unsigned Year, Month, Day;
   // When the user clicks break, this loop will still finish its execution and then the batch stops.
   if(recnum-cumulative_rec>50000) iterations=50000; // normal case
   else {iterations=recnum-cumulative_rec; change_the_table=true;} // This is the end of the table.
   //next time we need to change the table
   DataForm->get_recs(Raw_Table, iterations); // place the next epoch of (usually 50,000) records in the myQuerry


  // first transfer the data from the table into the features arrays (not only to vars
  // in a second for() loop, compute the segmentation...


   for(int i=1;i<iterations;i++) // we now process those records:
   {
       file_index=m_Calculator.msTable_to_vars(i);// copy data from the raw table myQuerry[i] to the m_ variables
       //double tt=tm;
       if(file_index!=old_file_index)// we detected a new_embeded_file in the raw features table -- reset the file variables
       {
            counter->Caption=m_Calculator.save_and_reset();// this function judge if time is continuues or not in this record
            current_rec=0;  // did we reset everthinging?
            option->wave_names[0]=DataForm->GetFileName(DataForm->file_table_name, file_index);//(option->file_table, file_index);
            m_Calculator.features.serial_number=DataForm->GetFile_serial(DataForm->file_table_name, file_index);
            if(option->age_in_table)
            {
              m_Calculator.features.day=(int)m_Calculator.features.serial_number-bird_hatched;
              m_Calculator.features.month=-1;
            }
            else // day is day of month, month is month
            {
              DecodeDate(m_Calculator.features.serial_number, Year, Month, Day);
              m_Calculator.features.day=Day;
              m_Calculator.features.month=Month;
            }
       }
       else
       {
            m_Calculator.parse_sound(current_rec);
            cumulative_rec++; current_rec++;
       }
       old_file_index=file_index;
   //    else {cumulative_rec++; current_rec++;}
   }
}

      //  m_Calculator.raw_to_vars(Raw_Table, counter, recnum); // gives m_vars
 /*
 void Tbatch::initiate_handler()
 {
        // open databse
        str="use ";
        str+=raw_features_database;
        DataForm->MyCommand1->SQL->Clear();
        DataForm->MyCommand1->SQL->Add(str);
        DataForm->MyCommand1->Execute(1);

        // open the table
        str="HANDLER `";
        str+=Raw_Table;
        str+="` open;";
        DataForm->MyCommand1->SQL->Clear();
        DataForm->MyCommand1->SQL->Add(str);
        DataForm->MyCommand1->Execute(1);

        // go to the begining
        str="HANDLER `";
        str+=Raw_Table;
        str+="` read first limit 1;";
        DataForm->MyCommand1->SQL->Clear();
        DataForm->MyCommand1->SQL->Add(str);
        DataForm->MyCommand1->Execute(1);
}

    
 void Tbatch::get_raw_recs(int i)
 {
    str="HANDLER `";
    str+=Raw_Table;
    str+="` read next limit ";
    str+=i; str+=";";
    DataForm->MyQuery1->SQL->Clear();
    DataForm->MyQuery1->SQL->Add(str);
    DataForm->MyQuery1->Execute();
    DataForm->MyQuery1->First();
    DataForm->MyQuery1->RecordCount;
 }
    */



 bool Tbatch::new_embeded_file(TDateTime tm)
 {
    static TDateTime old_tm=0;
    unsigned short year1, month1, day1, Hour1, Min1, Sec1, Msec1;
    bool new_epoch=false;
    if((double)tm-(double)old_tm>0.00001)
    {
        new_epoch=true;
        tm.DecodeDate(&year1, &month1, &day1);
        tm.DecodeTime(&Hour1, &Min1, &Sec1, &Msec1);
        option->serial_number=tm;
        option->month=month1;
        option->day=day1;
        option->hour=Hour1;
        option->minute=Min1;
        option->second=Sec1;
    }
    old_tm=tm;
    return(new_epoch);
 }


  int Tbatch::count_slaves()
  {
    int cnt=0;
    if(slave_state1->ItemIndex)cnt++;
    if(slave_state2->ItemIndex)cnt++;
    if(slave_state3->ItemIndex)cnt++;
    if(slave_state4->ItemIndex)cnt++;
    if(slave_state5->ItemIndex)cnt++;
    if(slave_state6->ItemIndex)cnt++;
    if(slave_state7->ItemIndex)cnt++;
    if(slave_state8->ItemIndex)cnt++;
    return(cnt);
  }




void __fastcall Tbatch::hatchedChange(TObject *Sender)
{
  DataForm->Settings(1, "hatching_date", FormatDateTime(hatched->Date,1));
}
//---------------------------------------------------------------------------




int Tbatch::lookfor_slave_files()
{
  return(0);
  /*
  AnsiString Slave_name, slv_state, str1;
  TRadioGroup *slave_state[8];
  TFileListBox *slave_list[8];

  slv_state="";
  deduct=false;
  slave_state[0]=slave_state1;  slave_state[1]=slave_state2;  slave_state[2]=slave_state3;  slave_state[3]=slave_state4;
  slave_state[4]=slave_state5;  slave_state[5]=slave_state6;  slave_state[6]=slave_state7;  slave_state[7]=slave_state8;

  slave_list[0]=slave_list1;    slave_list[1]=slave_list2;    slave_list[2]=slave_list3;    slave_list[3]=slave_list4;
  slave_list[4]=slave_list5;    slave_list[5]=slave_list6;    slave_list[6]=slave_list7;    slave_list[7]=slave_list8;
  int num_chan=0;
  for(int i=0;i<8;i++)
	   {
		   if(slave_state[i]->ItemIndex)
		   {
			 int diff, files_waiting=slave_list[i]->Items->Count;
			 num_chan++;
			// slave_list[i]->Update();
			 if(!slave_list[i]->Items->Count)
			 {
				current_slave->Caption="no slave files available";
				return(false); // nothing to do until the file shows up...
			 }

			 // we are now looking for matching files:
			// slave_list[i]->ItemIndex=0;
			if(slave_list[i]->ItemIndex>files_waiting-2)// we are more or less at the end of the list
			{
			  slave_list[i]->ItemIndex=0;// we should search from the beginning of the list...
			}
			if(slave_list[i]->ItemIndex>0)slave_list[i]->ItemIndex--; // go one back, just in case...
			bool found_slave_match=false;
			for(int j=slave_list[i]->ItemIndex; j<files_waiting; j++)
			{
				str= ExtractFileName(SoundFile);
				str=str.SubString(str.Length()-char_comp->Value,char_comp->Value-3);//20, 17);
				//str=str.SubString(4, str.Length()-3);
				str1=ExtractFileName(slave_list[i]->FileName);
				str1=str1.SubString(str1.Length()-char_comp->Value,char_comp->Value-3);//20, 17);
				// str1=str1.SubString(4, str1.Length()-3);
				diff=str.AnsiCompare(str1);
				if(!diff){found_slave_match=true; Slave_name= slave_list[i]->FileName; break;}
				slave_list[i]->ItemIndex++;
			}
			if(found_slave_match)
			{
				slv_state+="slave ";
				slv_state+=i;
				slv_state+=" + ";
				current_slave->Caption=slv_state;
				option->wave_names[i+1]=Slave_name;
				if(wave_files[i+1].OpenFile(Slave_name.c_str())== pcmpos_error )return(false);
					// else file_ch[i]->Caption=ExtractFileName(FileListBox[i]->FileName);
			}
			else  //return(false); // try again later...
			{
				//if(i==0) return(false);
				slv_state+="slave ";
				slv_state+=i;
				slv_state+=" - ";
				current_slave->Caption=slv_state;
				if(i==1) {deduct=true; return(1);}// could not find match to flow channels deduct it...
				else return(false);

			}
		   }
	   }
	   return(num_chan);     */
}



 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void __fastcall Tbatch::slave_state1Click(TObject *Sender)
{
         if(Sender==slave_state1) { set_slave_directory(1, slave_list1, folder_name1);  option->slave_mode[0]=slave_state1->ItemIndex; }
    else if(Sender==slave_state2) { set_slave_directory(2, slave_list2, folder_name2);  option->slave_mode[1]=slave_state2->ItemIndex; }
    else if(Sender==slave_state3) { set_slave_directory(3, slave_list3, folder_name3);  option->slave_mode[2]=slave_state3->ItemIndex; }
    else if(Sender==slave_state4) { set_slave_directory(4, slave_list4, folder_name4);  option->slave_mode[3]=slave_state4->ItemIndex; }
    else if(Sender==slave_state5) { set_slave_directory(5, slave_list5, folder_name5);  option->slave_mode[4]=slave_state5->ItemIndex; }
    else if(Sender==slave_state6) { set_slave_directory(6, slave_list6, folder_name6);  option->slave_mode[5]=slave_state6->ItemIndex; }
    else if(Sender==slave_state7) { set_slave_directory(7, slave_list7, folder_name7);  option->slave_mode[6]=slave_state7->ItemIndex; }
    else if(Sender==slave_state8) { set_slave_directory(8, slave_list8, folder_name8);  option->slave_mode[7]=slave_state8->ItemIndex; }


}
//---------------------------------------------------------------------------




bool Tbatch::set_slave_directory(int slave_num, TFileListBox* slave_list, TEdit* folder_name)
{
    AnsiString str1;
    if(automate_slave_find->ItemIndex) // slave folders are in subdirectories //slave1, slave2...
    {
    str=FileListBox1->Directory;
    char c=str[str.Length()];
    if(!(c=='\\'))
    {
        str+="\\";
    }
    str1=ExtractFileName(FileListBox1->FileName);
    str1=str1.SubString(2,str1.Length()-2);
    str+=str1;
    str+="\\slave";
    str+=slave_num;
    }
    else   // subdirectories are manually specified.
    {
       str=folder_name->Text;
    }

	if(DirectoryExists(str)) { slave_list->Directory=str; folder_name->Text=str; slave_list->Update(); slave_list->ItemIndex=0; return(true);}
    else return(false);

}


void __fastcall Tbatch::yearChange(TObject *Sender)
{
  option->year=year->Text.ToInt();
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::new_file_tableBClick(TObject *Sender)
{
  DataForm->new_file_tableBClick(this);
  file_table_name->Caption=DataForm->file_table_name;
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::BitBtn1Click(TObject *Sender)
{
        str="The slave channels will typically include wave files generated in conjunction with";
        str+="the sound file, such as presure date, electrophysiology recordings, etc. Each of the";
        str+="associated file must be placed in a subfolder of the sound files and named 'Slave1',";
        str+="'Slave2.,... Setting folders manually will not update when master folder has changed";
        MessageDlg(str, mtInformation, TMsgDlgButtons() << mbOK, 0);
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::automate_slave_findClick(TObject *Sender)
{
  if(!automate_slave_find->ItemIndex)
  {
        folder_name1->Enabled=true;  folder_name1->Color=clWhite;
        folder_name2->Enabled=true;  folder_name2->Color=clWhite;
        folder_name3->Enabled=true;  folder_name3->Color=clWhite;
        folder_name4->Enabled=true;  folder_name4->Color=clWhite;
        folder_name5->Enabled=true;  folder_name5->Color=clWhite;
        folder_name6->Enabled=true;  folder_name6->Color=clWhite;
        folder_name7->Enabled=true;  folder_name7->Color=clWhite;
        folder_name8->Enabled=true;  folder_name8->Color=clWhite;
  }
  else
  {
        folder_name1->Enabled=false;  folder_name1->Color=clBtnFace;
        folder_name2->Enabled=false;  folder_name2->Color=clBtnFace;
        folder_name3->Enabled=false;  folder_name3->Color=clBtnFace;
        folder_name4->Enabled=false;  folder_name4->Color=clBtnFace;
        folder_name5->Enabled=false;  folder_name5->Color=clBtnFace;
        folder_name6->Enabled=false;  folder_name6->Color=clBtnFace;
        folder_name7->Enabled=false;  folder_name7->Color=clBtnFace;
        folder_name8->Enabled=false;  folder_name8->Color=clBtnFace;
  }
}
//---------------------------------------------------------------------------

bool Tbatch::ready_to_go()
{
  /*
  if(first_time)return(true);
  if(save1->ItemIndex==1 || save1->ItemIndex==3)return(true);
  //if(Dont_wait->Checked)return(true);
  str="SELECT max(file_index) FROM ";
  str+=file_table_name->Caption;
  DataForm->MyQuery1->SQL->Clear();
  DataForm->MyQuery1->SQL->Add(str);
  DataForm->MyQuery1->Execute();
  int count=DataForm->MyQuery1->Fields->Fields[0]->AsInteger;

  str="SELECT COUNT(*) FROM "; str+=ms_table_name->Caption;
  str+=" where file_index="; str+=count;
  DataForm->MyQuery1->SQL->Clear();
  DataForm->MyQuery1->SQL->Add(str);
  DataForm->MyQuery1->Execute();
  count=DataForm->MyQuery1->Fields->Fields[0]->AsInteger;

  if(count)
  { wait_to_mySQL->Visible=false; return true;}
  else { wait_to_mySQL->Visible=true; return false; }
	*/
	return(1);
}


void __fastcall Tbatch::ApplicationEvents1Idle(TObject *Sender, bool &Done)
{
    batch->Update();
    try{


        if(counting){back_count--; if(back_count<1){counting=false; doit=false;}} // this will set a limit to number of operations in do next x files mode
        if(table_batch && doit) do_table_batch();
        else if(soundList->ItemIndex<soundList->Items->Count-1 && doit)
        {
                //if(ready_to_go())
                processFile(); // make a step, processing next file
        }
        else if(doit) // All the files in the folder where processed
        {
            if(All_subfolders->Checked)// continue across all subfolders
            {
              if(FileListBox1->ItemIndex<FileListBox1->Items->Count-1)
              {
                 FileListBox1->ItemIndex++; // change subfolder
                 FileListBox1Click(this);   // get the files exstracted to 'soundlist'
                 FileListBox1Change(this);
                 //Sleep(20);
                 FileListBox1->Refresh();
                 //Sleep(100);
                 FileListBox1Change(this);
                // option->extract_file_attributes();

                // set_slave_directory(1, slave_list1, folder_name1);
                // option->slave_mode[0]=slave_state1->ItemIndex;


              }
              else
              {
                doit=false;  // finish the batch
                status->Caption="The batch is now complete";
                //batch_controls->Visible=true;
                stop = GetTickCount();
                int dur=stop-start;
                AnsiString sMessage;
                sMessage.printf("done within %d seconds",dur/1000);
                MessageDlg(sMessage.c_str(), mtInformation, TMsgDlgButtons() << mbOK, 0);
                RecordsNum->Caption=DataForm->count_records(option->syllable_table_name);
              }
            }

            else
            {
              st->Brush->Color=clGreen;
              //batch_controls->Visible=true;
              status->Caption="The batch is now complete";
              doit=false;
              stop = GetTickCount();
              int dur=stop-start;
              AnsiString sMessage;
              sMessage.printf("done within %d seconds",dur/1000);
              MessageDlg(sMessage.c_str(), mtInformation, TMsgDlgButtons() << mbOK, 0);
              RecordsNum->Caption=DataForm->count_records(option->syllable_table_name);
            }
        }
     }catch(...)
       {
           doit=false;
           MessageDlg("an exception has occured, could not finish the batch",mtWarning, TMsgDlgButtons() << mbOK, 0);
       }
}
//---------------------------------------------------------------------------


void __fastcall Tbatch::addClick(TObject *Sender)
{
   TListBox *list;
   list=table_batch_list;
   for(int i=0; i<tables->Items->Count; i++)
       if(tables->Selected[i])list->Items->Add(tables->Items->Strings[i]);//soundList->FileName);
  /*
   list->ItemIndex=0;
   Get_raw_Table_attributes();
   get_age();
   */
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::table_batch_listMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    Get_raw_Table_attributes();
    get_age();
  //  age->Value=ageTb->Position+table_batch_list->ItemIndex;
        //table_batch_list->Items->Strings[table_batch_list->ItemIndex].ToIntDef(1);
}
//---------------------------------------------------------------------------


void __fastcall Tbatch::table_batch_listKeyPress(TObject *Sender,
      char &Key)
{
    if(Key==46 || Key==110 || Key=='d')  table_batch_list->Items->Delete(table_batch_list->ItemIndex);
}
//---------------------------------------------------------------------------


void __fastcall Tbatch::open_file_tableClick(TObject *Sender)
{
   DataForm->openTableClick(this);
   DataForm->file_table_name=DataForm->TheTable->TableName;
}
//---------------------------------------------------------------------------



void Tbatch::get_age()
{
        str=table_batch_list->Items->Strings[table_batch_list->ItemIndex];
        int i=1;
        do{i++;}while(str[i]!='_' && i<str.Length()); // takes us to the "_"
        i++; // path it to the age part of the name
        str=str.SubString(i,str.Length());
        option->day=str.ToIntDef(0);
}



void __fastcall Tbatch::openTableClick(TObject *Sender)
{
  // need work...
  batch->Enabled=true;
  batch_count->Enabled=true;
  database=DataForm->MyConnection1->Database;
  DataForm->TheTable=DataForm->Records;
  DataForm->openTableClick(this);
  analysis_summary->Cells[0][1]=DataForm->TheTable->TableName;
  //update_table_names();
}
//---------------------------------------------------------------------------


void __fastcall Tbatch::open_file_tblClick(TObject *Sender)
{
  DataForm->open_FILE_tableClick(this);
  file_table_name->Caption=DataForm->file_table_name;
}
//---------------------------------------------------------------------------





void Tbatch::identify_bird()
{
/*
     if(soundList->FileName=="")return;
     if(DataForm->retrieve_animal(ExtractFileName(soundList->FileName)))
     {
        bird_hatched=FormatDateTime(DataForm->MyQuery1->FieldValues["hatched"],1);
        //TDateTime file_age=file_age.FileDateToDateTime(FileAge(soundList->FileName));
       // int the_age=file_age-bird_hatched;
       int the_age=option->file_age-bird_hatched;
        if(the_age<0 || the_age>99999)
        {
          analysis_summary->Cells[0][1]="none";
          analysis_summary->Cells[1][1]="0";
          analysis_summary->Cells[4][1]="none";
          analysis_summary->Cells[3][1]="0";
          analysis_summary->Cells[7][1]="none";
        }
        else
        {
          analysis_summary->Cells[0][1]=DataForm->MyQuery1->FieldValues["bird_name"];
          analysis_summary->Cells[1][1]=the_age;
          analysis_summary->Cells[3][1]=DataForm->MyQuery1->FieldValues["model"];
          analysis_summary->Cells[2][1]=DataForm->MyQuery1->FieldValues["training"]-DataForm->MyQuery1->FieldValues["hatched"];
          analysis_summary->Cells[7][1]=DataForm->MyQuery1->FieldValues["details"];
          hatched->Date=bird_hatched;
          if(daily_raw_tables->ItemIndex) update_table_names();
        }
     }
     else
     {
          analysis_summary->Cells[1][1]="none";
          analysis_summary->Cells[2][1]="0";
          analysis_summary->Cells[4][1]="none";
          analysis_summary->Cells[3][1]="0";
          analysis_summary->Cells[7][1]="none";
     }
     */
}



void __fastcall Tbatch::age_or_dayClick(TObject *Sender)
{
  if(age_or_day->ItemIndex) option->age_in_table=false;
  else option->age_in_table=true; 
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::char_comp_tbChange(TObject *Sender)
{
  char_comp->Value=char_comp_tb->Position;
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::new_birdClick(TObject *Sender)
{
  batch->Enabled=true;
  batch_count->Enabled=true;
  //if(Sender==new_bird) animals_form->datapages->ActivePageIndex=1;
  //else
  animals_form->datapages->ActivePageIndex=0;
  if (animals_form->ShowModal() == mrOk)
  {
    analysis_summary->Cells[0][1]=animals_form->name->Text;
    analysis_summary->Cells[1][1]=animals_form->age->Value;
    analysis_summary->Cells[4][1]=animals_form->song->Text;
    analysis_summary->Cells[3][1]=animals_form->training->Text;
    analysis_summary->Cells[7][1]=animals_form->bird_comments->Text;
    //update_table_names();
  }
  if(animals_form->new_bird) animals_form->Insert_bird_record();
  batch_name->Text=animals_form->name->Text;
  hatched->Date=animals_form->hatched->Date;
  ref_time->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::change_raw_tableClick(TObject *Sender)
{
  str=DataForm->open_Raw_Table();
  raw_features1->Text=str;
  ms_table_name->Caption=str;
  option->features_table=str;

}
//---------------------------------------------------------------------------

/*

what this function should do (not doing yet) is create all the tables
base only on the labels appearing in the output tab
make sure that the tables are okay too if possible (e.g., file table)
-- only create the tables that are supposed to be created

*/

void Tbatch::update_table_names()
{
  if(save_file_table->ItemIndex)
  {
    DataForm->file_table_name=file_table_name1->Text;
    file_table_name->Caption=file_table_name1->Text;
    DataForm->Create_File_Table();
  }

  if(save1->ItemIndex<2)
  {
     DataForm->Create_table(syllable_table_name1->Text);
     DataForm->fix_syll_table_fields(syllable_table_name1->Text);
     option->syllable_table_name=syllable_table_name1->Text;
     DataForm->TheTable->TableName=syllable_table_name1->Text;
     syllable_table_name->Caption=syllable_table_name1->Text;
     RecordsNum->Caption=DataForm->count_records(option->syllable_table_name);
  }

  if(save1->ItemIndex==0 || save1->ItemIndex==2)
  {
     str=daily_raw_name->Text;
     DataForm->new_raw_table(str,slave_chans);
     raw_features1->Text=str;
     ms_table_name->Caption=str;
     option->features_table=str;
     m_Calculator.last_ms_record=0;
  }


  /*

  RecordFile1->Caption="syll_" + analysis_summary->Cells[0][1];
  DataForm->Create_table(RecordFile1->Caption);
  option->syllable_table_name=RecordFile1->Caption;
  DataForm->TheTable->TableName=RecordFile1->Caption;
  RecordsNum->Caption=DataForm->count_records(option->syllable_table_name);
  option->syllable_table_name=DataForm->TheTable->TableName;
  syllable_table_name->Caption=option->syllable_table_name;
  int bird_age= option->file_age - (int)hatched->Date;
  if(bird_age<0)bird_age=0;

  daily_raw_name->Text=raw_features1->Text+"_"+option->month+"_"+option->day+"_"+option->year;
  option->features_table=daily_raw_name->Text;
  str=daily_raw_name->Text;//"raw_" + analysis_summary->Cells[0][1] +"_" + bird_age;
  if((option->saveRawFile && (save1->ItemIndex==0 || save1->ItemIndex==2)) && !DataForm->table_exists(str))
  {
        m_Calculator.last_ms_record=0;
        str=DataForm->new_raw_table(str,slave_chans);
        raw_features1->Text=str;
        ms_table_name->Caption=str;
        option->features_table=str;
  }

  */
}




void __fastcall Tbatch::SyllMinDurTbChange(TObject *Sender)
{
     option->syllable_duration_thresh=1.4*SyllMinDurTb->Position;
     SyllDurLow->Value=option->syllable_duration_thresh;
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::BoutMinDurTBChange(TObject *Sender)
{
  option->bout_duration_thresh=1.4*BoutMinDurTB->Position;
  BoutDurLow->Value=option->bout_duration_thresh;
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::min_syllablesChange(TObject *Sender)
{
   option->minimum_syllables_per_files=min_syllables->Position;
   mean_syllable_num->Value=option->min_syllables->Position;
}
//---------------------------------------------------------------------------



void __fastcall Tbatch::BitBtn8Click(TObject *Sender)
{
  batch_controls->ActivePage=output;
}
//---------------------------------------------------------------------------





void __fastcall Tbatch::state_transition_TBChange(TObject *Sender)
{
        state_transition_Num->Value=(float)state_transition_TB->Position/10.0;
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::missmatchesTbChange(TObject *Sender)
{
        missmatchesNum->Value=missmatchesTb->Position;
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::BitBtn12Click(TObject *Sender)
{
  str="State duration data are saved into the table state_duration";
  str+="Note that each batch adds data to this table.";
  str+="You need to empty it via the MySQL control center";
  MessageDlg(str, mtInformation, TMsgDlgButtons() << mbOK, 0);
}
//---------------------------------------------------------------------------


void __fastcall Tbatch::add2Click(TObject *Sender)
{
   // TListBox *list;
   //list=multi_tables_list;
   for(int i=0; i<multi_tables->Items->Count; i++)
       if(multi_tables->Selected[i])multi_tables_list->Items->Add(multi_tables->Items->Strings[i]);//soundList->FileName);
  multi_tables_list->ItemIndex=0;
  Get_brain_table_attribute();
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::multi_tables_listKeyPress(TObject *Sender,
      char &Key)
{
if(Key==46 || Key==110 || Key=='d')  multi_tables_list->Items->Delete(multi_tables_list->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::multi_tables_listMouseDown(TObject *Sender,
	  TMouseButton Button, TShiftState Shift, int X, int Y)
{
      Get_brain_table_attribute();
}

//---------------------------------------------------------------------------


void __fastcall Tbatch::forwardClick(TObject *Sender)
{
  got_data=false;
  segment_sf(this);
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::resetClick(TObject *Sender)
{
   DataForm->initiate_handler("sap", Raw_Table);
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::multi_segmentClick(TObject *Sender)
{
  option->brain_mode=true;
  breakIt->Enabled=true;
  total_count=0;
  save1->ItemIndex=1; // save syllable tables only
  option->create_feature_arrays=false;
  Get_raw_Table_attributes(); // retrieve simple attributes such as table name, number of records, etc.
  st->Brush->Color=clRed;
  status->Caption="executing batch";
  st->Repaint();
  batch->Repaint();
  batch->Refresh();
  AnsiString SyllTable=RecordFile1->Caption;
  option->create_feature_arrays=false;
  database=DataForm->MyConnection1->Database;
  table_batch=true;
  option->custom_parse=true;
  change_the_table=false;
  option->segmentation_feature=segment_by->ItemIndex;
  option->custom_segmentation_thresh=custom_thresh->Text.ToDouble();
  batch_controls->ActivePage=progress;
  get_age();
  doit=true;
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::create_braintableClick(TObject *Sender)
{
  // need work
  batch->Enabled=true;
  batch_count->Enabled=true;
  database=DataForm->MyConnection1->Database;
  DataForm->new_brain_tableClick(this);
  analysis_summary->Cells[0][1]=DataForm->TheTable->TableName;
  //update_table_names();
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::BitBtn7Click(TObject *Sender)
{
  str=" The 'do it all' button is initially inactive (gray).";
  str+="Before starting the batch you need to identify the bird: click 'new bird'.";
  str+="or 'add bird', and make sure all options are set before starting the batch";
  MessageDlg(str, mtInformation, TMsgDlgButtons() << mbOK, 0);
}
//---------------------------------------------------------------------------


void __fastcall Tbatch::brain_tableClick(TObject *Sender)
{
 if(brain_table->ItemIndex)
 {
        // if needed, create brain tables
        option->brain_mode=true;
 }
 else option->brain_mode=false;//Add_Brain_Record();
}



//---------------------------------------------------------------------------




void __fastcall Tbatch::calc_rhythmClick(TObject *Sender)
{
  calculate_rhythm();
}

float Tbatch::calculate_rhythm()
{
int range, samp;
float Gridmax=0;
static int slice=1, counter=0;
static bool first_time=true;
static float RR[5000];

  switch(rhythm_range->ItemIndex)
  {
          case 0: range=200; samp=2; break;
          case 1: range=500; samp=5; break;
          case 2: range=5000; samp=50; break;
  }

  if(first_time) for(int i=0;i<range;i++) {RR[i]=0; first_time=false;}

  float* r=m_Calculator.Calculate_rhythm(rhythm_range->ItemIndex, rhythm_feature->ItemIndex, rhythm_filter->ItemIndex);

  rhythm_chart->ClearGraf();
  rhythm_chart->DataColor=clRed;
  rhythm_chart->MoveTo(0,0);
  float Xr, maxY=0;
  str="";
  rhythm_spect->GridMat->Resize(range,100);
  rhythm_spect->MeshVisible=false;


  for(int i=1;i<range;i++) // we now process those records:
  {
    Xr=(float)i*0.030517578/option->advanceTb->Value;
    rhythm_chart->DrawTo(Xr,r[i]);
    if(maxY<r[i])maxY=r[i];
    if(counter<files_per_rhythmcalc->Position)RR[i]+=r[i];
    else
    {
       rhythm_spect->GridMat->Elem[i][slice]=(int)(RR[i]);
       if(Gridmax<RR[i])Gridmax=RR[i];
    }
  }

  counter++;

  if(counter>files_per_rhythmcalc->Position)
  {
        counter=0;
        slice++;
        for(int i=0;i<range;i++)RR[i]=0;
        rhythm_spect->Refresh();
  }
  //if(print_rhythm->Checked)rhythm_data->Lines->Add(str);
  rhythm_chart->Scale1X->RangeHigh=Xr;
  rhythm_chart->Scale1Y->RangeHigh=maxY;
  rhythm_chart->Scale1X->RangeLow=0;
  rhythm_chart->Scale1Y->RangeLow=0;
  rhythm_chart->ShowGraf();
  return(Gridmax);
}
//---------------------------------------------------------------------------


void __fastcall Tbatch::colorsChange(TObject *Sender)
{
  rhythm_spect->ColorScaleHigh=colors->Position;
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::angleXChange(TObject *Sender)
{
  rhythm_spect->ViewAngleX=angleX->Position;
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::angleZChange(TObject *Sender)
{
  rhythm_spect->ViewAngleZ=angleZ->Position;
}
//---------------------------------------------------------------------------



void __fastcall Tbatch::test_time_stampClick(TObject *Sender)
{
  time_stamp_panel->Visible=true;
  test_time_stamp->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::show_filesClick(TObject *Sender)
{
  file_panel->ActivePage=select_sounds;
  file_panel->Visible=true;
  select_waves->Visible=true;
  test_time_stamp->Visible=true;
  show_files->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::features_calcClick(TObject *Sender)
{
  //sound_files raw_tables animals segmentation raw2syll rhythm progress
  save1Click(this);
  table_batch=false;

  str = DataForm->Retrieve(1, "input_folder");
		if (DirectoryExists(str)) DirectoryListBox1->Directory = str;

  sound_files->TabVisible=true;
  batch_controls->ActivePage=sound_files;
  switch(save1->ItemIndex)
  {
    case 0: raw_table_panel->Visible=true; syllable_table_panel->Visible=true; break;
    case 1: raw_table_panel->Visible=false; syllable_table_panel->Visible=true; break;
    case 2: raw_table_panel->Visible=true; syllable_table_panel->Visible=false;
            wav_selectionClick(this); break;
    case 3: raw_table_panel->Visible=false; syllable_table_panel->Visible=false;
            save_file_table->ItemIndex=0; save_file_tableClick(this); break;
  }
}
//---------------------------------------------------------------------------



void __fastcall Tbatch::soundListClick(TObject *Sender)
{
  option->extract_file_attributes(soundList->FileName);
  time_stamp->Cells[0][1]=ExtractFileName(soundList->FileName);
  time_stamp->Cells[1][1]=option->month;
  time_stamp->Cells[2][1]=option->day;
  time_stamp->Cells[3][1]=option->year;
  time_stamp->Cells[4][1]=option->hour;
  time_stamp->Cells[5][1]=option->minute;
  time_stamp->Cells[6][1]=option->second;
  time_stamp->Cells[7][1]=option->msec;
  time_stamp->Cells[8][1]=option->serial_number;
  to_segmentation->Enabled=true;
  if(daily_raw_tables->ItemIndex)daily_raw_name->Text=raw_features1->Text+"_"+option->month+"_"+option->day+"_"+option->year;
  else daily_raw_name->Text=raw_features1->Text;
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::formatClick(TObject *Sender)
{
   option->format->ItemIndex=format->ItemIndex;
}
//---------------------------------------------------------------------------




void __fastcall Tbatch::batch_modeClick(TObject *Sender)
{
   if(batch_mode->ItemIndex)batch_name_panel->Visible=true;
   else batch_name_panel->Visible=false;
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::batch_choicesClick(TObject *Sender)
{
   if(batch_name->Text!="")
   {
     str="file_table_"; str+=batch_name->Text;
     file_table_name1->Text=str;
     str="syll_"; str+=batch_name->Text;
     syllable_table_name1->Text=str;
     str="raw_"; str+=batch_name->Text;
     raw_features1->Text=str;
     daily_raw_name->Text=str;
     batch_types->Visible=true;
   }
   else MessageDlg("Please name the batch or select animal with a name (do not use _ symbols", mtError, TMsgDlgButtons()<< mbOK, 0);
   batch_choices->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::batch_name1Change(TObject *Sender)
{
  ref_time->Visible=true;
  batch_name->Text=batch_name1->Text;
}
//---------------------------------------------------------------------------



void __fastcall Tbatch::batch_processClick(TObject *Sender)
{
        wav_selectionClick(this); // make sure features of all sounds are saved if so indicated
        update_table_names(); // create the needed tables.
        breakIt->Enabled=true;
        files_processed=0;
        files_processedN->Caption=0;
        files_accepted=0;
        msCounter=0;
        files_acceptedN->Caption=0;
        SoundFile=soundList->FileName;
        option->wave_names[0]=SoundFile;
      //  option->extract_file_attributes(SoundFile);
      // option->features_table=daily_raw_name->Text;
      // option->syllable_table_name=syllable_table_name1->Text;


        Draw1Click(this);  // for sonogram display rendering
       /*
        if(save_file_table->ItemIndex)//auto_file_table->Checked)
        {
          str="file_table_"; str+=analysis_summary->Cells[0][1];//syllable_table_name->Caption;
          DataForm->file_table_name=str;
          DataForm->Create_File_Table();
          file_table_name->Caption=str;
        }
      */
        int bird_age= option->file_age - (int)hatched->Date;
        if(bird_age<0)bird_age=0; //{Application->MessageBox("bird age can't be negative!", NULL, MB_OK);return;}

        batch_controls->ActivePage=progress;//->Visible=false;
      /*
        str=option->syllable_table_name;
        if(str[1]=='s' && str[2]=='y' && str[3]=='l' && str.Length()>6)
						str="raw_" + str.SubString(6,str.Length()-6);
        str+="_";
        str+=bird_age;
        raw_features1->Text=str;

		slave_chans=count_slaves();
		slave_numL->Caption=slave_chans;    */
        save1Click(this);  // make sure that table saving options are fine
       // if(save1->ItemIndex==3)MessageDlg("Warning: Are you sure? The save option is off and no data will be saved...", mtWarning, TMsgDlgButtons() << mbOK, 0);
        first_time=true;
        option->halt=false;
        DataForm->Records->Active=true;

        option->bird_ID = batch_name->Text;//analysis_summary->Cells[0][1];//;InputBox("Sound Analysis Pro", "Animal ID:", "Please type animal identification number");
        //option->bird_ID=str.ToIntDef(0);
        if(option->bird_ID=="")return;

         entropyTbChange(this);
         derivTBChange(this);
         //Draw1->Down=false;
         start=GetTickCount();
    //     show_sonogram->Checked=false;
		 m_Calculator.render_image=false;
         file_table_index->Value=DataForm->update_file_table_index(0);
         doit=true;
         RecordsNum->Caption=DataForm->count_records(option->syllable_table_name);
}
//---------------------------------------------------------------------------


void __fastcall Tbatch::to_segmentationClick(TObject *Sender)
{
	 str = DirectoryListBox1->Directory;
	 DataForm->Settings(1, "input_folder", str);
	 sonogram->Visible=true;
     if(doing_rhythm_batch)
     {
       rhythm_settings->TabVisible=true;
       batch_controls->ActivePage=rhythm_settings;
       return;
     }
     if(save1->ItemIndex<2)
     {
     //  segment_byChange(this);
       segmentation_memo->Visible=true;
       segmentation_done->Visible=true;
       //to_select_wav->Enabled=false;
       segmentation_done->Enabled=true;
       segment_label->Visible=true;
       segmentation->TabVisible=true;
       batch_controls->ActivePage=segmentation;
     }
     else to_outputClick(this);
     to_segmentation->Enabled=false;
}
//---------------------------------------------------------------------------




void __fastcall Tbatch::segmentation_doneClick(TObject *Sender)
{
  if(table_batch)
  {
   do_rawsyll_batch->TabVisible=true;
   batch_controls->ActivePage=do_rawsyll_batch;
   return;
  }

  save_criteria->TabVisible=true;
  wav_selection->Visible=true;
  to_output->Visible=true;
  segmentation_done->Enabled=false;
  batch_controls->ActivePage=save_criteria;
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::wav_selectionClick(TObject *Sender)
{
  if(wav_selection->ItemIndex)
  {
      option->save_all=false;
      saving_thresh->Visible=true;
      move_sounds->Visible=true;
  }
  else
  {
     option->save_all=true;
     saving_thresh->Visible=false;
     move_sounds->Visible=false;
     move_options->ItemIndex=0;
  }
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::move_optionsClick(TObject *Sender)
{
  if(move_options->ItemIndex==1)option->delete_trash_file=true;
  else option->delete_trash_file=false;

  if(move_options->ItemIndex==2)
  {
     option->moveTo_sound_folders[0]=target_sound_file_folder->Text;
     test_folderClick(this);
     option->move_sound=true;
     sound_output1->Enabled=true;
     target_sound_file_folder->Enabled=true;
     test2->Enabled=true;
     move_to->Visible=true;
  }
  else
  {
    option->move_sound=false;
    sound_output1->Enabled=false;
    target_sound_file_folder->Enabled=false;
    test2->Enabled=false;
    move_to->Visible=false;
  }




}
//---------------------------------------------------------------------------



void __fastcall Tbatch::to_outputClick(TObject *Sender)
{
   output->TabVisible=true;
   batch_controls->ActivePage=output;
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::save_file_tableClick(TObject *Sender)
{
  if(save_file_table->ItemIndex)file_table_options->Visible=true;
  else file_table_options->Visible=false;
}
//---------------------------------------------------------------------------



void __fastcall Tbatch::to_reviewClick(TObject *Sender)
{
  Draw1->Down=false;
  review->TabVisible=true;
  batch_controls->ActivePage=review;

  str="Batch name: "; str+=batch_name->Text;
  if(batch_mode->ItemIndex)str+=". This batch is for unidentified, or multiple subjects";
  else str+=".  This batch include a single subject/animal";
  batch_summary->Lines->Add(str);

  str="Refernece time: ";
  str+=DateToStr(hatched->Date);
  batch_summary->Lines->Add(str);

  str="Folders included:";
  if(data_type->ItemIndex)
  {
    str+="batch will process data across subfolders";
    if(!All_subfolders-> Checked)str+=", but stop at the end of each.";
  }
  else str+="All the batch data are located in a single folder";
  batch_summary->Lines->Add(str);

  str="Time stamp: ";
  switch(format->ItemIndex)
  {
    case 0: str+="No detection of data age (time=real batch time)"; break;
    case 1: str+="File age"; break;
    case 2: str+="File name used for accurate stamp (if possible, for SAP generated files)"; break;
  }
  batch_summary->Lines->Add(str);

  str="Batch output: ";
  switch(save1->ItemIndex)
  {
    case 0: str+="save raw and syllable features"; break;
    case 1: str+="save syllable table only"; break;
    case 2: str+="save raw features only"; break;
    case 3: str+="save nothing(dry-run)"; break;
  }
  batch_summary->Lines->Add(str);

   if(save1->ItemIndex>1)str="Segmentation: no segmentation to syllables";
   //else if(segmentation_method->ItemIndex)str="Segmentation: fine segmentation (dynamic threshold)";
   else str="Segmentation: segment by fixed amplitude threshold";
   batch_summary->Lines->Add(str);

   str="";
   batch_summary->Lines->Add(str);
   str="PROCESSING AND OUTPUT";
   batch_summary->Lines->Add(str);

   if(wav_selection->ItemIndex)str="Data filtering: save features from selected files";
   else str="Data filtering: save features from all processed files";
   batch_summary->Lines->Add(str);

   switch(move_options->ItemIndex)
   {
     case 0: str="wave files and folders will not be altered by this batch"; break;
     case 1: str="REJECTED FILES WILL BE DELETED BY THIS BATCH"; break;
     case 2: str="REJECTED WAVE FILES WILL BE MOVED TO OTHER FOLDERS"; break;
   }
   batch_summary->Lines->Add(str);

   str="";
   batch_summary->Lines->Add(str);
   str="MySQL table output";
   batch_summary->Lines->Add(str);

   str="File table: ";
   if(save_file_table->ItemIndex)str+=file_table_name1->Text;
   else str+="no file table will be saved";
   batch_summary->Lines->Add(str);

   str="Syllable table: ";
   if(save1->ItemIndex<2)str+=syllable_table_name1->Text;
   else str+="no syllable-table will be saved";
   batch_summary->Lines->Add(str);

   str="Raw features table: ";
   if(save1->ItemIndex==0 || save1->ItemIndex==2)str+=raw_features1->Text;
   if(daily_raw_tables->ItemIndex)str+=" + date, create daily tables";
   else str+=" (no daily changing tables)";
   batch_summary->Lines->Add(str);
}
//---------------------------------------------------------------------------

void Tbatch::hp_smallChange_sf(TObject *Sender)
{
	//	m_Calculator.do_hpfilt();
}

void Tbatch::secondaryFeatureCBClick_sf(TObject *Sender)
{
	/*entropyTb->Visible = true;
	entropy_val->Visible = true;
	segment->secondaryThresholdTB->Value = entropy_val->Value;   */
	plot_sonogram();
}


void __fastcall Tbatch::daily_raw_tablesClick(TObject *Sender)
{
   if(daily_raw_tables->ItemIndex)
   {
     //daily_raw_name->Visible=true;
     //daily_raw_lb->Visible=true;
     daily_raw_name->Text=raw_features1->Text+"_"+option->month+"_"+option->day+"_"+option->year;
     //MessageDlg("please click on some wave files and observe the daily table name", mtInformation, TMsgDlgButtons() << mbOK, 0);
   }
   else
   {
     //daily_raw_name->Visible=false;
     //daily_raw_lb->Visible=false;
     daily_raw_name->Text=raw_features1->Text;
   }
}
//---------------------------------------------------------------------------

void Tbatch::save_frame_sf(TObject *Sender)
{
   //DataForm->Save_frame_params(1);

   AnsiString str;
	str=segment_by->ItemIndex;
	DataForm->Settings(1, "pri_segmented_feature", str);
	str=secondaryFeatureDD->ItemIndex;
	DataForm->Settings(1, "sec_segmented_feature", str);
	str=segmentation_flip->ItemIndex;
	DataForm->Settings(1, "pri_lt", str);
	str=secondaryComparisonRB->ItemIndex;
	DataForm->Settings(1, "sec_lt", str);
	str= smoothGraphCB->Checked? 1:0;
	DataForm->Settings(1,"smooth_feature" , str);
	str= hp_small->Position;
	DataForm->Settings(1,"hp_small" , str);
	str= hp_big->Position;
	DataForm->Settings(1,"hp_big" , str);
	str= adaptThreshCB->Checked? 1:0;
	DataForm->Settings(1,"adapt_thresh" , str);
	str= secondaryFeatureCB->Checked? 1:0;
	DataForm->Settings(1, "sec_segmentation", str);
	str= secondaryLogicRB->ItemIndex;
	DataForm->Settings(1, "sec_logic", str);
	str= min_stop_durTb->Position;
	DataForm->Settings(1, "syllable_duration_reject", str);
	str= bout_stop->Position;
	DataForm->Settings(1, "bout_duration_reject", str);


    SingleFileClick(this);
}



void __fastcall Tbatch::raw_to_syllClick(TObject *Sender)
{
  table_batch=true;
  raw2syll->TabVisible=true;
  populate_raw_tables();
  batch_controls->ActivePage=raw2syll;
  file_panel->ActivePage=select_tables;
  file_panel->Visible=true;
}
//---------------------------------------------------------------------------


void __fastcall Tbatch::thresh_tableChange(TObject *Sender)
{
   //multi_chart2->Pen->Color=clRed;
   multi_chart2->LineWidth=4;
   //multi_chart2->PenColor=clRed;
   //multi_chart2->Line(50,50, thresh_table->Position, 100);
   //segmentation_threshold1->Text=thresh_table->Max-thresh_table->Position;
   //segmentation_threshold2->Text=thresh_table->Max-thresh_table->Position;

}
//---------------------------------------------------------------------------


void __fastcall Tbatch::tablesClick(TObject *Sender)
{
  str=tables->Items->Strings[tables->ItemIndex];
  int i=DataForm->count_records(str);
  raw_recs->Caption=i;
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::table_batch_listClick(TObject *Sender)
{
  str=table_batch_list->Items->Strings[table_batch_list->ItemIndex];
  //DataForm->TheTable->TableName=str;
  Get_raw_Table_attributes();
  int i=DataForm->count_records(str);
  raw_recs->Caption=i;
  got_data=false;
  segment_sf(this);
}
//---------------------------------------------------------------------------


void __fastcall Tbatch::similarity_batchClick(TObject *Sender)
{
  StartForm->similarity_batchClick(this);
 // batch->Close();

}
//---------------------------------------------------------------------------


void __fastcall Tbatch::BitBtn17Click(TObject *Sender)
{
  for(int i=0;i<100;i++)
  {
      SingleFileClick(this);
      soundList->ItemIndex++;
      calc_rhythmClick(this);
  }
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::files_per_rhythmcalcChange(TObject *Sender)
{
  files_per_rhythm_val->Value=files_per_rhythmcalc->Position;
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::rhythm_batchClick(TObject *Sender)
{
  sound_files->TabVisible=true;
  batch_controls->ActivePage=sound_files;
  doing_rhythm_batch=true;
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::start_rhythm_batchClick(TObject *Sender)
{
  bool rhythm_done=false;
  float val, maxval=0;
  angleZChange(this);
  angleXChange(this);
  colorsChange(this);
  rhythm->TabVisible=true;
  batch_controls->ActivePage=rhythm;

  // starting point
      switch(file_selection->ItemIndex)
      {
        case 0: soundList->ItemIndex=0; break;
        case 1: soundList->ItemIndex=soundList->Items->Count/2; break;
        case 2: soundList->ItemIndex=soundList->Items->Count; break;
        case 3: soundList->ItemIndex=Random(soundList->Items->Count); break;
      }


  // first case: single folder batch:
  if(!data_type->ItemIndex)
  {
      for(int i=0;i<(rhythms_per_folder->Position*files_per_rhythmcalc->Position);i++)
      {
          SingleFileClick(this);
          val=calculate_rhythm();
          if(maxval<val)maxval=val;
          // next sound:
          switch(file_selection->ItemIndex)
          {
            case 0: soundList->ItemIndex++; break;
            case 1: soundList->ItemIndex++; break;
            case 2: if(soundList->ItemIndex)soundList->ItemIndex--; break;
            case 3: soundList->ItemIndex=Random(soundList->Items->Count); break;
          }
      }
  }

  else // multipe folders
  do{
      for(int i=0;i<(rhythms_per_folder->Position*files_per_rhythmcalc->Position);i++)
      {
          SingleFileClick(this);
          val=calculate_rhythm();
          if(maxval<val)maxval=val;
           switch(file_selection->ItemIndex)
          {
            case 0: soundList->ItemIndex++; break;
            case 1: soundList->ItemIndex++; break;
            case 2: if(soundList->ItemIndex)soundList->ItemIndex--; break;
            case 3: soundList->ItemIndex=Random(soundList->Items->Count); break;
          }
      }
      FileListBox1->ItemIndex++; // change subfolder
      if(FileListBox1->ItemIndex==FileListBox1->Items->Count-1)rhythm_done=true;
      FileListBox1Click(this);   // get the files exstracted to 'soundlist'
      FileListBox1Change(this);
      FileListBox1->Refresh();
      FileListBox1Change(this);
      if(soundList->Items->Count<1)rhythm_done=true;
      switch(file_selection->ItemIndex)
      {
        case 0: soundList->ItemIndex=0;
        case 1: soundList->ItemIndex=soundList->Items->Count/2;
        case 2: soundList->ItemIndex=soundList->Items->Count;
        case 3: soundList->ItemIndex=Random(soundList->Items->Count);
      }
  }while(!rhythm_done);

  maxval/=1000;
  if(!maxval)maxval=1;
  rhythm_spect->SuppressPaint=true;
  for(int i=0; i<rhythm_spect->GridMat->NrOfColumns; i++)
     for(int j=0;j<rhythm_spect->GridMat->NrOfRows; j++)
         rhythm_spect->GridMat->Elem[i][j]/=maxval;
  rhythm_spect->SuppressPaint=false;
  MessageDlg("Rythm Batch done", mtInformation, TMsgDlgButtons()<< mbOK, 0);

}
//---------------------------------------------------------------------------

void __fastcall Tbatch::rhythms_per_folderChange(TObject *Sender)
{
  rhythms_per_folder_val->Value=rhythms_per_folder->Position;
}
//---------------------------------------------------------------------------

void Tbatch::segment_sf(TObject *Sender)
{

  if(!table_batch)return;
  // that's for the sound segmentation part, commented for now

  /*
  option->fine_segmentation->Down=segmentation_method->ItemIndex;
   DataForm->Settings(1, "fine_segmentation", (int)segmentation_method->ItemIndex);

   if(segmentation_method->ItemIndex)
   {
		segmentation_pannel->Visible=true;
		entropyTb->Visible=true;
		entropy_val->Visible=true;
		option->hp_amp=true;
   }
   else
   {
		segmentation_pannel->Visible=false;
		entropyTb->Visible=false;
		entropy_val->Visible=false;
		option->hp_amp=false;
   }

  */


// thats for the raw features table part
   TTrackBar* thresh_table;
   thresh_table=derivTB;
  //multi_chart->MouseAction=maZoomDrag;
  switch(segment_by->ItemIndex)
  {
		case 0: multi_chart2->Scale1Y->RangeLow=0; multi_chart2->Scale1Y->RangeHigh=100; thresh_table->Min=0; thresh_table->Max=100; break; // amplitude
		case 1: multi_chart2->Scale1Y->RangeLow=-10; multi_chart2->Scale1Y->RangeHigh=0; thresh_table->Min=-100; thresh_table->Max=0; break; // entropy
		case 2: multi_chart2->Scale1Y->RangeLow=0; multi_chart2->Scale1Y->RangeHigh=option->frequency_range_val->Value; thresh_table->Min=0; thresh_table->Max=option->frequency_range_val->Value; break; // pitch
		case 3: multi_chart2->Scale1Y->RangeLow=0; multi_chart2->Scale1Y->RangeHigh=100; thresh_table->Min=0; thresh_table->Max=100; break; // FM
		case 4: multi_chart2->Scale1Y->RangeLow=0; multi_chart2->Scale1Y->RangeHigh=1000; thresh_table->Min=0; thresh_table->Max=1000; break; // goodness
  }
  multi_chart2->ClearGraf();
  multi_chart2->DataColor=clBlue;

  if(!got_data)
  {
		DataForm->get_recs(Raw_Table, 5000); // place the next epoch of (usually 50,000) records in the myQuerry
		got_data=true;
  }

  for(int i=1;i<5000;i++) // we now process those records:
  {
	m_Calculator.msTable_to_vars(i);
	float y;
	switch(segment_by->ItemIndex)
	{
		case 0:y=m_Calculator.m_amplitude; break;
		case 1:y=m_Calculator.m_Entropy; break;
		case 2: y=m_Calculator.m_Pitch; break;
		case 3: y=m_Calculator.m_FM; break;
		case 4: y=m_Calculator.m_PitchGoodness; break;

	}

	multi_chart2->MarkAt(i,y,1);
  }
  multi_chart2->ShowGraf();
}





void __fastcall Tbatch::done_rawtosyllClick(TObject *Sender)
{
  resegment->Visible=true;
}
//---------------------------------------------------------------------------



void __fastcall Tbatch::show_rawtosylltablesClick(TObject *Sender)
{
table_features->TabVisible=true;
    sonogram_display->TabVisible=false;
    sonogram->Visible=true;
	//segment->Visible=true;
    table_batch_list->ItemIndex=0;
    if(table_batch_list->ItemIndex==0) table_batch_listClick(this);
    create_tables_panel->Visible=true;
    segmentation->TabVisible=true;
    batch_controls->ActivePage=segmentation;
}
//---------------------------------------------------------------------------





//---------------------------------------------------------------------------
void Tbatch::plot_sonogram()
{
	 //option->doNotSegment = false;

	  hide_all_features();
	  update_segmentation_params();
	  // set brightness level
	  m_Calculator.GetImage().SetBrightness(float(sldrBrightness->Position/700));
	  //float bright=exp((float)sldrBrightness->Position/700);
	  //m_Calculator.GetImage().SetBrightness(bright);
	 // get threshold position
	 int pos = derivTB->Position * 148 / 125; //20 + DerivThresh[chan]->Position * 9 / 5;
	 int feature = segment_by->ItemIndex;
	 deriv_val->Value = option->adjustThreshold(feature, derivTB->Position, option->frequency, 1);
	 NumLab3->Value = deriv_val->Value;
	 //option->amplitude_thresh=(float) (125-derivTB->Position);

	 //save->OnClick(this);

	for (int i = 0; i < recnum; i++) m_Calculator.sound_detect_feature(i);
	 switch(feature) {
	   case 0: m_Calculator.GetImage().GetDerivPow().bRender = true; break;
	   case 1: m_Calculator.GetImage().GetPitch().bRender = true; break;
	   case 2: m_Calculator.GetImage().GetPeakFreq().bRender = true; break;
	   case 3: m_Calculator.GetImage().GetPitchGoodness().bRender = true; break;
	   case 4: m_Calculator.GetImage().GetFM().bRender = true; break;
	   case 5: m_Calculator.GetImage().GetAM().bRender = true; break;
	   case 6: m_Calculator.GetImage().GetEntropy().bRender = true; break;
	   case 7: m_Calculator.GetImage().Getcontinuity_t().bRender = true; break;
	   case 8:m_Calculator.GetImage().Getcontinuity_f().bRender = true; break;
	   default: m_Calculator.GetImage().GetDerivPow().bRender = true;
	 }

	 if (option->sec_segmentation) {
		entropyTb->Visible=true;
		entropy_val->Visible=true;
		option->sec_segmented_feature = secondaryFeatureDD->ItemIndex;
		entropy_val->Value = option->adjustThreshold(option->sec_segmented_feature, entropyTb->Position, option->frequency, 0);
		secondaryThresholdTB->Value = entropy_val->Value;
		//option->entropy_thresh=(float)(-entropyTb->Position)/50.0;
		option->entropy_thresh=(float)(125-entropyTb->Position);
		int featureSec = secondaryFeatureDD->ItemIndex;
		switch(featureSec) {
			 case 0: m_Calculator.GetImage().GetDerivPow().bRender = true; break;
			 case 1: m_Calculator.GetImage().GetPitch().bRender = true; break;
			 case 2: m_Calculator.GetImage().GetPeakFreq().bRender = true; break;
			 case 3: m_Calculator.GetImage().GetPitchGoodness().bRender = true; break;
			 case 4: m_Calculator.GetImage().GetFM().bRender = true; break;
			 case 5: m_Calculator.GetImage().GetAM().bRender = true; break;
			 case 6: m_Calculator.GetImage().GetEntropy().bRender = true; break;
			 case 7: m_Calculator.GetImage().Getcontinuity_t().bRender = true; break;
			 case 8: m_Calculator.GetImage().Getcontinuity_f().bRender = true; break;
			 default: m_Calculator.GetImage().GetDerivPow().bRender = true;
								}
		} else {
		  entropyTb->Visible=false;
		  entropy_val->Visible=false;
		}

		if (option->smooth_feature || option->adapt_threshold) {
		   m_Calculator.do_hpfilt();
           	m_Calculator.Render_sonogram_images();
		   if (option->adapt_threshold)
              m_Calculator.GetImage().Gethp_big().bRender = true;
		   if (option->smooth_feature)
              m_Calculator.GetImage().Gethp_small().bRender = true;
		}
		// segment the sound acording to the criteria

		int stop_duration = 0;
		for (int i = 0; i < m_Calculator.total_frames; i++) {
		if (m_Calculator.sound_detect_feature(i)) {
		   m_Calculator.signal[i] = true;
		   stop_duration = 0;
		} else {
		  if (stop_duration <= option->min_stop_duration) m_Calculator.signal[i] = true;
		  else m_Calculator.signal[i] = false;
		  stop_duration++;
		}
	 }
	 for (int i = 0; i < m_Calculator.total_frames; i++) m_Calculator.sound_detect_feature(i);
	 // plot the sonogram
	 m_Calculator.GetImage().DrawAllSlices(Image1);


     if (!(option->adapt_threshold)) {
		Image1->Canvas->Pen->Color = clYellow;
		Image1->Canvas->MoveTo(0, pos);
		Image1->Canvas->LineTo(Image1->Width, pos);
		//show_outlines(image, onset, offset);
	}
	if (option->sec_segmentation) {
		Image1->Canvas->Pen->Color = clAqua; // (TColor)RGB(0, 0, 0);
		int posSec = entropyTb->Position * 148 / 125;
		Image1->Canvas->MoveTo(0, posSec);
		Image1->Canvas->LineTo(Image1->Width, posSec);
		//show_outlines(image, onset, offset);
	}

}


//---------------------------------------------------------------------------


void Tbatch::hide_all_features() {
	m_Calculator.GetImage().GetPitch().bRender = false;
	m_Calculator.GetImage().GetEntropy().bRender = false;
	m_Calculator.GetImage().GetFM().bRender = false;
	m_Calculator.GetImage().GetAM().bRender = false;
	m_Calculator.GetImage().GetDerivPow().bRender = false;
    m_Calculator.GetImage().Getmfa().bRender = false;
	m_Calculator.GetImage().GetPitchGoodness().bRender = false;
	m_Calculator.GetImage().GetPeakFreq().bRender = false;
	m_Calculator.GetImage().Getcepst0().bRender = false;
	m_Calculator.GetImage().Getcontinuity_t().bRender = false;
	m_Calculator.GetImage().Getcontinuity_f().bRender = false;
	m_Calculator.GetImage().Getprincipal_frequency().bRender = false;
	m_Calculator.GetImage().Getslope().bRender = false;
	m_Calculator.GetImage().Gethp_small().bRender = false;
	m_Calculator.GetImage().Gethp_big().bRender = false;
	m_Calculator.GetImage().Gethp_diff().bRender = false;
   }
//---------------------------------------------------------------------------



void Tbatch::update_segmentation_params()
{
		option->pri_segmented_feature = segment_by->ItemIndex;
		option->pri_thresh_raw = derivTB->Position;
		option->reverse_segmentation =  segmentation_flip->ItemIndex;
		option->smooth_feature = smoothGraphCB->Checked ;
		option->adapt_threshold = adaptThreshCB->Checked;
		option->sec_segmentation = secondaryFeatureCB->Checked;
		option->sec_segmented_feature = secondaryFeatureDD->ItemIndex;
		option->sec_thresh_raw = entropyTb->Position;
		option->sec_lt = secondaryComparisonRB->ItemIndex;
		option->sec_logic =  secondaryLogicRB->ItemIndex;
		option->hp_small = hp_small->Position*100;
		float x = hp_big->Position;
		x /= 30;
		option->hp_big = exp(x);
}

void __fastcall Tbatch::segment_byChange(TObject *Sender)
{
 option->pri_segmented_feature = segment_by->ItemIndex;
 plot_sonogram();
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::segmentation_flipClick(TObject *Sender)
{
 option->reverse_segmentation = segmentation_flip->ItemIndex;
 plot_sonogram();
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::smoothGraphCBClick(TObject *Sender)
{
if (smoothGraphCB->Checked) {
		option->smooth_feature = true;
		hp_small->Visible = true;
		hp_small_num->Visible = true;
		StaticText49->Visible = true;
	}
	else {
		option->smooth_feature = false;
		hp_small->Visible = false;
		hp_small_num->Visible = false;
		StaticText49->Visible = false;
	}
	 plot_sonogram();
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::hp_smallChange(TObject *Sender)
{
	option->HP_small->Position = hp_small->Position;
	hp_small_num->Value = hp_small->Position;
	option->hp_small = hp_small->Position * 100;
    plot_sonogram();
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::adaptThreshCBClick(TObject *Sender)
{
if (adaptThreshCB->Checked) {
		option->adapt_threshold = true;
		hp_big->Visible = true;
		hp_big_num->Visible = true;
		StaticText50->Visible = true;
	} else {
		option->adapt_threshold = false;
		hp_big->Visible = false;
		hp_big_num->Visible = false;
		StaticText50->Visible = false;
	}
    plot_sonogram();
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::hp_bigChange(TObject *Sender)
{
	 float x;
	x = hp_big->Position;
	x /= 30; // range 1.0 to 10.0
	//option->hp_big = exp(x);
	option->HP_big->Position = hp_big->Position;
	hp_big_num->Value = hp_big->Position;
	option->hp_big = exp(x);
	plot_sonogram();
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::secondaryFeatureCBClick(TObject *Sender)
{
	if (secondaryFeatureCB->Checked) {
		secondaryLogicRB->Visible = true;
		secondaryFeatureDD->Visible = true;
		secondaryComparisonRB->Visible = true;
		secondaryThresholdTB->Visible = true;
        option->sec_segmentation = true;
	}
	else {
		secondaryLogicRB->Visible = false;
		secondaryFeatureDD->Visible = false;
		secondaryComparisonRB->Visible = false;
		secondaryThresholdTB->Visible = false;
        option->sec_segmentation = false;
	}
    //option->sec_segmentation = true;
    plot_sonogram();
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::secondaryFeatureDDChange(TObject *Sender)
{
      option->sec_segmented_feature=secondaryFeatureDD->ItemIndex;
      plot_sonogram();
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::secondaryComparisonRBClick(TObject *Sender)
{
     option->sec_lt = secondaryComparisonRB->ItemIndex;
     plot_sonogram();
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::secondaryLogicRBClick(TObject *Sender)
{
     option->sec_logic1 = secondaryLogicRB->ItemIndex;
     plot_sonogram();

}
//---------------------------------------------------------------------------

void __fastcall Tbatch::min_stop_durTbChange(TObject *Sender)
{
     option->stop_duration_thresh = min_stop_durTb->Position;
	 min_stop_durN->Value=min_stop_durTb->Position;
	 /*
	 if (option->stop_duration_thresh > 0) {
		return;
	} */
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::bout_stopChange(TObject *Sender)
{
     option->bout_duration_thresh = bout_stop->Position;
	 bout_Stop_N->Value=bout_stop->Position;
     if (option->stop_duration_thresh > 0) {
        return;
    }
}
//---------------------------------------------------------------------------



void __fastcall Tbatch::frequency_rangeChange(TObject *Sender)
{
	option->frequency_range->Position = frequency_range->Position;
    option->frequency_rangeChange(this);
	int i = pow((double)2, 4 - frequency_range->Position);
	frequency_range_val->Value = option->sampling_rate / i;
	switch(frequency_range->Position) {
	case 1: // show quater of the range of FFT
		option->range_x2 = false;
		option->range_x05 = true;
		break;

	case 2: // show half of the range of FFT
		option->range_x2 = false;
		option->range_x05 = false;
		break;

	case 3: // show all the range of FFT (well, almost)
	    option->range_x2 = true;
        option->range_x05 = false;
		break;
	}
	 option->frequency = frequency_range_val->Value;
	 DataForm->Settings(1, "frequency_range", frequency_range->Position);
}
//---------------------------------------------------------------------------


void __fastcall Tbatch::data_windowChange(TObject *Sender)
{
	windowTB->Value = 1000.0 * (float)data_window->Position / (float)option->sampling_rate;
	option->data_window->Position = data_window->Position;
	option->data_windowChange(this);
	DataForm->Settings(1, "FFT_window", data_window->Position);
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::advanceChange(TObject *Sender)
{
	advanceTb->Value = 1000.0 * (float)advance->Position / (float)option->sampling_rate;
	option->advance->Position = advance->Position;
	option->advanceChange(this);
	DataForm->Settings(1, "advance_window", advance->Position);
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::Draw2Click(TObject *Sender)
{
   Draw1->Down=Draw2->Down;
   Draw1Click(this);
}
//---------------------------------------------------------------------------



void __fastcall Tbatch::saveClick(TObject *Sender)
{
	AnsiString str;
	int chan=1;
	str=segment_by->ItemIndex;
	DataForm->Settings(chan, "pri_segmented_feature", str);
	str=secondaryFeatureDD->ItemIndex;
	DataForm->Settings(chan, "sec_segmented_feature", str);
	str=segmentation_flip->ItemIndex;
	DataForm->Settings(chan, "pri_lt", str);
	str=secondaryComparisonRB->ItemIndex;
	DataForm->Settings(chan, "sec_lt", str);
	str= smoothGraphCB->Checked? 1:0;
	DataForm->Settings(chan,"smooth_feature" , str);
	str= hp_small->Position;
	DataForm->Settings(chan,"hp_small" , str);
	str= hp_big->Position;
	DataForm->Settings(chan,"hp_big" , str);
	str= adaptThreshCB->Checked? 1:0;
	DataForm->Settings(chan,"adapt_thresh" , str);
	str= secondaryFeatureCB->Checked? 1:0;
	DataForm->Settings(chan, "sec_segmentation", str);
	str= secondaryLogicRB->ItemIndex;
	DataForm->Settings(chan, "sec_logic", str);
	str= min_stop_durTb->Position;
	DataForm->Settings(chan, "syllable_duration_reject", str);
	str= bout_stop->Position;
	DataForm->Settings(chan, "bout_duration_reject", str);
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::goto_parametersClick(TObject *Sender)
{
	 parameters->TabVisible=true;
	 batch_controls->ActivePage=parameters;
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::pitch_methodClick(TObject *Sender)
{
  option->pitch_method->ItemIndex=pitch_method->ItemIndex;
  option->pitch_methodClick(this);
  if(pitch_method->ItemIndex==3)Panel12->Visible=true;
  else Panel12->Visible=false;
}
//---------------------------------------------------------------------------

void __fastcall Tbatch::minfr_tbChange(TObject *Sender)
{
  minfr->Value=minfr_tb->Position;
  option->minFreq=minfr_tb->Position;
}
//---------------------------------------------------------------------------


void __fastcall Tbatch::min_syllable_durTbChange(TObject *Sender)
{
   option->min_syllable_duration=min_syllable_durTb->Position;
   min_syllable_durN->Value=min_syllable_durTb->Position*advanceTb->Value;
}
//---------------------------------------------------------------------------

