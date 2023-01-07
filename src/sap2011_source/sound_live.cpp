//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "sound_live.h"
#include "CZT_FeatureImage.h"
#include "options3.h"
#include "records.h"
#include "start.h"
#include "animals.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SDL_NumLab"
#pragma link "TeeProcs"
#pragma link "TeeProcs"
#pragma link "segmentation"
#pragma link "segmentation"
#pragma resource "*.dfm"
#include <time.h>
#include <limits.h>
#include <math.h>
#include "CZT_PcmEq.h"
#include <fstream> // for ifstream, ofstream, ios_base
#include <iostream>// for cin, cout, endl
#define on      1
#define off     0

using std::ios_base;
using std::cout;
using std::cin;
using std::endl;

Tsound_live *sound_live;

//---------------------------------------------------------------------------
__fastcall Tsound_live::Tsound_live(TComponent* Owner)
        : TForm(Owner)
{
    app_starting=true;
    DrawIt=true;
	option->module=1; // sound_live module
	option->frequency= 44100 / 4;
	option->cepstrum_pitch=true;
	//update_vars();
	//option->sono_display=true;
    //option->calculate_continuity=false;
}
//---------------------------------------------------------------------------


void Tsound_live::update_file_lists()
{
   for(int i=0;i<8;i++) if(DirectoryExists(chan_in[i]->Text))FileListBox[i]->Directory=chan_in[i]->Text;
}



void Tsound_live::force_file_lists()
{
   for(int i=0;i<8;i++)
   {
      if(!DirectoryExists(chan_in[i]->Text))if(!ForceDirectories(chan_in[i]->Text))
      {
			str="cannot create input folder for chan " + i;
			Application->MessageBox((wchar_t*)str.c_str(), NULL, MB_OK);
      }
      else FileListBox[i]->Directory=chan_in[i]->Text;

	  if(!DirectoryExists(chan_out[i]->Text))if(!ForceDirectories(chan_out[i]->Text))
      {
        str="cannot create output folder for chan 0"+i;
		Application->MessageBox((wchar_t*)str.c_str(), NULL, MB_OK);
      }
   }
}


//void Tsal::Calc(AnsiString fileName, TStaticText *rec)
void Tsound_live::Calc(int chan)
{
    static AnsiString oldName="empty";
    if(oldName==FileListBox[chan]->FileName)
    { // this should be replaced by a 'reset and delete all' function, for all wave_Files.
       // here should come a for loop for num_slaves+1...
       wave_files[0].ResetContents();
       DeleteFile(FileListBox[chan]->FileName);
       return;
    }

    oldName=FileListBox[chan]->FileName;
    if(DrawIt)
    {
        m_Calculator.render_image=true;
        m_Calculator.GetSonogram().SetBrightness(contrast[chan]);
    }
    else m_Calculator.render_image=false;
    m_Calculator.RenderImage(DrawIt);// yes render it
    CZT_PcmEq equalizer;// doesnt do anything (yet)
     //if(wave.OpenFile(fileName.c_str()) != pcmpos_error)
     //{
    option->wave_names[0]=FileListBox[chan]->FileName;
    option->extract_file_attributes(option->wave_names[0]); // extracting the age of the file into option.hour, min...
    if(equalizeIt) wave_files[0].UseFilter(equalizer);

    if(m_Calculator.Calculate(wave_files, 0)) // return true if the sound contain meaningful data and should be saved
    {
      if(sampling[chan]->Checked && random(100)>10) // if random sampling is on, do not save this file
      {
       rec[chan]->Font->Color=clGray;
       passed=false; rec[chan]->Caption="Dropped";
       DeleteFile(FileListBox[chan]->FileName);
      }
      else
      {
       option->chan=chan;
       save_data(chan);
       rec[chan]->Font->Color=clRed;
       rec[chan]->Caption="Passed";
       passed=true;
      }
    }

    else //  the files are empty of meaningful data and should be deleted
    {
       rec[chan]->Font->Color=clGreen;
       passed=false; rec[chan]->Caption="Rejected";
       DeleteFile(FileListBox[chan]->FileName);
    }
 }


//---------------------------------------------------------------------------

 void Tsound_live::save_data(int chan)
 {
     AnsiString str1;
    // option->wave_names[0]=FileListBox[chan]->FileName; // this is the file we are going to analyze
    // option->extract_file_attributes();   // we now have the time stamp of the file for the Records

     if(folder_choice->ItemIndex)// change a folder every day, accordign to the age of each bird...
     {
       set_output_folder(chan); // calc new ages and set new output folders -- only new ms_table names are set here (if there is data!)
	  // str=syllable_table[chan]->Caption + "_" + Age[chan];
       if(Age[chan]<0)Age[chan]=0;
	   str= "raw_" +ID_[chan]->Text + "_" + Age[chan];
       if(str!=ms_table[chan]->Caption)DataForm->new_raw_table(str, 0); // create a new raw table
       ms_table[chan]->Caption=str;
      // recnum[chan]->Value=0;
      // ms_records[chan]->Value=0;
     }

     // the other setting is changing folder when its size is large enough (for DVD size...)
     if( (folder_choice->ItemIndex==0) && folder_size[chan]->Value>folder_size_limit->Position)// size comparison in Mb, change folder name;
     {   // first, we set the new folder name based on the first file included in it
        AnsiString subdir_name=ID_[chan]->Text;
        subdir_name+="_"; subdir_name+=option->month; subdir_name+="_"; subdir_name+=option->day;
        subdir_name+="_"; subdir_name+=option->hour; subdir_name+="_"; subdir_name+=option->minute;
        AnsiString current_folder_name=chan_out[chan]->Text;
        int i=current_folder_name.Length()-1;
        do{i--;}while(current_folder_name[i]!='\\');
        current_folder_name.SetLength(i);
        // that's it, now check that folder can be created...
        str=current_folder_name+subdir_name+"\\";
        if (!DirectoryExists(str)) if(!ForceDirectories(str))
        {
		   Application->MessageBox(L"Folder size quata has been reached, but folder could not be changed: folder size monitoring has been reset", NULL, MB_OK);
           folder_size[chan]->Value=0;
        }
        else { chan_out[chan]->Text=str; folder_size[chan]->Value=0; }

     }
	 DataForm->file_table_name=file_table[chan]->Caption;
	 if(age[chan]->Value<0)age[chan]->Value=0;
	 option->bird_age=age[chan]->Value;
	 option->syllCounter=0;// reset the syllables counter
	 FileListBox[chan]->ItemIndex=0;
     option->select_table=chan+1;

     switch(save[chan]->ItemIndex)
     {
       case 1:  option->saveRawFile=false; option->saveRecFiles=false;   break;
       case 2:  option->saveRawFile=false; option->saveRecFiles=true;  break;
       case 3:  option->saveRawFile=true; option->saveRecFiles=false;  break;
       case 4:  option->saveRawFile=true; option->saveRecFiles=true; break;
     }
     m_Calculator.save_to_database(chan+1);  // this will take care of renaming and moving the data-containing files
     file_table_recs[chan]->Value++;
     if(save[chan]->ItemIndex==3 || save[chan]->ItemIndex==4)
                ms_records[chan]->Value+=option->ms_recordes_added; // ms table saved
     if(save[chan]->ItemIndex==2 || save[chan]->ItemIndex==4)
                recnum[chan]->Value+=option->syllCounter; // syllable table saved

  }


//---------------------------------------------------------------------------


void __fastcall Tsound_live::Brightness0Change(TObject *Sender)
{
  for(int i=0;i<8;i++)contrast[i]=exp((float)Brightness[i]->Position/700.0);
}
//---------------------------------------------------------------------------


void __fastcall Tsound_live::entropyThresh0Change(TObject *Sender)
{
  int bird;
  TShape *thresh;
  TTrackBar *entropyThresh;
  TLabel *entr_ind;
	   if(Sender==entropyThresh0) {bird=3; thresh=thresh0;  entropyThresh=entropyThresh0;  entr_ind=ent_ind0;}
  else if(Sender==entropyThresh1) {bird=4; thresh=thresh1;  entropyThresh=entropyThresh1;  entr_ind=ent_ind1;}
  else if(Sender==entropyThresh2) {bird=5; thresh=thresh2;  entropyThresh=entropyThresh2;  entr_ind=ent_ind2;}
  else if(Sender==entropyThresh3) {bird=6; thresh=thresh3;  entropyThresh=entropyThresh3;  entr_ind=ent_ind3;}
  else if(Sender==entropyThresh4) {bird=7; thresh=thresh4;  entropyThresh=entropyThresh4;  entr_ind=ent_ind4;}
  else if(Sender==entropyThresh5) {bird=8; thresh=thresh5;  entropyThresh=entropyThresh5;  entr_ind=ent_ind5;}
  else if(Sender==entropyThresh6) {bird=9; thresh=thresh6;  entropyThresh=entropyThresh6;  entr_ind=ent_ind6;}
  else if(Sender==entropyThresh7) {bird=10; thresh=thresh7;  entropyThresh=entropyThresh7;  entr_ind=ent_ind7;}
  else return;

  thresh->Pen->Color=clAqua;//clRed;
 // EntropyB->Down=true;  DerivPowerB->Down=true;
  //option->entropy_thresh=(float)(-entropyThresh->Position)/100.0;
  //entr_ind->Caption=-(float)entropyThresh->Position/100;//entropyThresh->Position;
  option->sec_thresh_raw=entropyThresh->Position;
  secondaryThresholdTB->Value=option->adjustThreshold(secondaryFeatureDD->ItemIndex, entropyThresh->Position, option->frequency, 1);
  //int pos=entropyThresh->Position/3.6+30;
  int pos = entropyThresh->Position * 88 / 125;
  thresh->Top=pos;
  DataForm->Settings(bird, "entropy_thresh", entropyThresh->Position);
  update_vars();
}
//---------------------------------------------------------------------------



void Tsound_live::save_settings()
{

	for(int i=0;i<8;i++)
	{
	   DataForm->Settings(i+3, "input_folder", chan_in[i]->Text);
	   DataForm->Settings(i+3, "output_folder_sound", chan_out[i]->Text);
	   DataForm->Settings(i+3, "bird_ID", ID_[i]->Text);
	   DataForm->Settings(i+3,"save", save[i]->ItemIndex);
	}
	update_vars();
}

//---------------------------------------------------------------------------


void __fastcall Tsound_live::FormClose(TObject *Sender, TCloseAction &Action)
{
  try
  {
		//if (MessageDlg("Save settings?", mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
		save_settings();
  }
  catch(...){StartForm->Visible=true;}
  StartForm->Visible=true;
}
//---------------------------------------------------------------------------





void __fastcall Tsound_live::Timer1Timer(TObject *Sender)
{
      // first we handle the 'round the clock' statistics
    /* */
    static int min15=0, oldTime=0; // in ms = 900000
      min15+=Timer1->Interval;
      if(min15>9000)
      {
        // reset all counts at 23:00
        if(option->hour==23)for(int i=0; i<5; i++)for(int j=0; j<6; j++) option->types[i][j]=0;
        min15=0;
        
        int time=option->hour*4+option->minute/15; // set a time scale from 0-100 steps of 15 minutes around the clock
        if(time!=oldTime) for(int syll_type=0;syll_type<6;syll_type++)
        {
            Syll_types0[syll_type][time]=option->types[1][syll_type];
            option->types[1][syll_type]=0;
            Chart0->Series[syll_type]->Clear();
            for(int j=0;j<100;j++)Chart0->Series[syll_type]->AddXY( j/4,Syll_types0[syll_type][j], j/4, clBlue );

            Syll_types1[syll_type][time]=option->types[2][syll_type];
            option->types[2][syll_type]=0;
            Chart1->Series[syll_type]->Clear();
            for(int j=0;j<100;j++)Chart1->Series[syll_type]->AddXY( j/4,Syll_types1[syll_type][j], j/4, clBlue );

            Syll_types2[syll_type][time]=option->types[3][syll_type];
            option->types[3][syll_type]=0;
            Chart2->Series[syll_type]->Clear();
            for(int j=0;j<100;j++)Chart2->Series[syll_type]->AddXY( j/4,Syll_types2[syll_type][j], j/4, clBlue );

            Syll_types3[syll_type][time]=option->types[4][syll_type];
            option->types[4][syll_type]=0;
            Chart3->Series[syll_type]->Clear();
            for(int j=0;j<100;j++)Chart3->Series[syll_type]->AddXY( j/4,Syll_types3[syll_type][j], j/4, clBlue );

            Syll_types4[syll_type][time]=option->types[5][syll_type];
            option->types[5][syll_type]=0;
            Chart4->Series[syll_type]->Clear();
            for(int j=0;j<100;j++)Chart4->Series[syll_type]->AddXY( j/4,Syll_types4[syll_type][j], j/4, clBlue );

            Syll_types5[syll_type][time]=option->types[6][syll_type];
            option->types[6][syll_type]=0;
            Chart5->Series[syll_type]->Clear();
            for(int j=0;j<100;j++)Chart5->Series[syll_type]->AddXY( j/4,Syll_types5[syll_type][j], j/4, clBlue );

            Syll_types6[syll_type][time]=option->types[7][syll_type];
            option->types[7][syll_type]=0;
            Chart6->Series[syll_type]->Clear();
            for(int j=0;j<100;j++)Chart6->Series[syll_type]->AddXY( j/4,Syll_types6[syll_type][j], j/4, clBlue );

            Syll_types7[syll_type][time]=option->types[8][syll_type];
            option->types[8][syll_type]=0;
            Chart7->Series[syll_type]->Clear();
            for(int j=0;j<100;j++)Chart7->Series[syll_type]->AddXY( j/4,Syll_types7[syll_type][j], j/4, clBlue );

        }

        oldTime=time; 
     }

     // in case of problems...
     // Label3->Caption=option->problem_file;
      if(option->DeleteFile_failed)
      {
		 IsBusy=false;
         if(DeleteFile(option->File_to_delete)) {option->DeleteFile_failed=false; skip_channel1=false;}
         else skip_channel1=true;
         if(!FileExists(option->File_to_delete)) {option->DeleteFile_failed=false; skip_channel1=false;}
      }
    
      // okay, do the management job...
	  if(!IsBusy){busy->Brush->Color=clGreen; Lookfor_Files();}
      else
      {
        busy->Brush->Color=clRed;
        if(option->problem_file.ToIntDef(0))
        {
          str="exception had happened, error number: ";
          str+=option->problem_file;
          startup_state->Caption=str;
        }
		return;
      }

	  problem_filename->Caption=option->problem_file;

      if(option->halt)
      {
         stopClick(this);
		 Application->MessageBox((wchar_t*)option->Message.c_str(), NULL, MB_OK);

      }

}

 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
 void Tsound_live::set_output_folder(int bird)
{
		  //TDate hatched;
		  //hatched=FormatDateTime(DataForm->Retrieve(bird+3, "hatching_date"),1);
		  Age[bird]= (int)option->file_age - (int)hatched[bird];
		  if(Age[bird]<0)Age[bird]=0;
		  age[bird]->Value=Age[bird].ToInt();

         // folder_size[bird]->Value=0;
		  if(folder_choice->ItemIndex)
			 option->moveTo_sound_folders[bird+1]=chan_out[bird]->Text+Age[bird]+"\\";
		  else option->moveTo_sound_folders[bird+1]=chan_out[bird]->Text;
		  if (!DirectoryExists(option->moveTo_sound_folders[bird+1]))
				if(!ForceDirectories(option->moveTo_sound_folders[bird+1]))
					  Application->MessageBox(L"Folder could not be changed!", NULL, MB_OK);

}



 void Tsound_live::set_output_folders()
{
  try{
       for(int i=0;i<8;i++)
       {
          folder_size[i]->Value=0;
		  Age[i]=DataForm->find_age(i+3); // this gives us the real!!! age of the bird in refference to current time (not file age!)
		  age[i]->Value=Age[i].ToInt();
          if(folder_choice->ItemIndex)
                option->moveTo_sound_folders[i+1]=chan_out[i]->Text+Age[i]+"\\";
          else
                option->moveTo_sound_folders[i+1]=chan_out[i]->Text;

          if (!DirectoryExists(option->moveTo_sound_folders[i+1]))
                if(!ForceDirectories(option->moveTo_sound_folders[i+1]))
                {
                    str="Folder "; str+=option->moveTo_sound_folders[i+1]; str+=" could not be created";
                    str+=". Please check if the drive is accessible, or if the folders contains too many files -- move them.";
					Application->MessageBox((wchar_t*)str.c_str(), NULL, MB_OK);
                }

       }
     }
     catch(...)
     {Application->MessageBox(L"Folders for output (wave files) could not be changed!", NULL, MB_OK);}
}



void Tsound_live:: Lookfor_Files()
{
   if(save0->ItemIndex) {FileListBox[0]->Update(); if(FileListBox[0]->Items->Count && save0->ItemIndex){IsBusy=true; Analyze_Files(0); IsBusy=false; return; }}
   if(save1->ItemIndex) {FileListBox[1]->Update(); if(FileListBox[1]->Items->Count && save1->ItemIndex){IsBusy=true; Analyze_Files(1); IsBusy=false; return; }}
   if(save2->ItemIndex) {FileListBox[2]->Update(); if(FileListBox[2]->Items->Count && save2->ItemIndex){IsBusy=true; Analyze_Files(2); IsBusy=false; return; }}
   if(save3->ItemIndex) {FileListBox[3]->Update(); if(FileListBox[3]->Items->Count && save3->ItemIndex){IsBusy=true; Analyze_Files(3); IsBusy=false; return; }}
   if(save4->ItemIndex) {FileListBox[4]->Update(); if(FileListBox[4]->Items->Count && save4->ItemIndex){IsBusy=true; Analyze_Files(4); IsBusy=false; return; }}
   if(save5->ItemIndex) {FileListBox[5]->Update(); if(FileListBox[5]->Items->Count && save5->ItemIndex){IsBusy=true; Analyze_Files(5); IsBusy=false; return; }}
   if(save6->ItemIndex) {FileListBox[6]->Update(); if(FileListBox[6]->Items->Count && save6->ItemIndex){IsBusy=true; Analyze_Files(6); IsBusy=false; return; }}
   if(save7->ItemIndex) {FileListBox[7]->Update(); if(FileListBox[7]->Items->Count && save7->ItemIndex){IsBusy=true; Analyze_Files(7); IsBusy=false; return; }}
}


//---------------------------------------------------------------------------

void Tsound_live::update_vars()
{
  for(int chan=0;chan<8;chan++)
  {
		pri_segmented_feature[chan] = DataForm->Retrieve(chan+3, "pri_segmented_feature").ToInt();
		pri_thresh_raw[chan] = DataForm->Retrieve(chan+3, "deriv_thresh").ToInt();
		sec_segmented_feature[chan] = DataForm->Retrieve(chan+3, "sec_segmented_feature").ToInt();
		sec_thresh_raw[chan] = DataForm->Retrieve(chan+3, "entropy_thresh").ToInt();
		reverse_segmentation[chan] = DataForm->Retrieve(chan+3, "pri_lt").ToInt();
		sec_lt[chan] = DataForm->Retrieve(chan+3, "sec_lt").ToInt();
		smooth_feature[chan] = DataForm->Retrieve(chan+3, "smooth_feature").ToInt();
		adapt_threshold[chan] = DataForm->Retrieve(chan+3, "adapt_thresh").ToInt();
		sec_segmentation[chan] = DataForm->Retrieve(chan+3, "sec_segmentation").ToInt();
		sec_logic[chan]  = DataForm->Retrieve(chan+3, "sec_logic").ToInt();
		HP_small[chan] = 100* DataForm->Retrieve(chan+3, "hp_small").ToInt();
		float x =(float)DataForm->Retrieve(chan+3, "hp_big").ToInt()/30.0;
		HP_big[chan] = exp(x);
		ShortDateFormat = "m/d/yyyy";
		hatched[chan]=FormatDateTime(DataForm->Retrieve(chan+3, "hatching_date"),1);

  }
}

//---------------------------------------------------------------------------

void Tsound_live::Retrieve_segmentation_to_options(chan)
{
		option->pri_segmented_feature = pri_segmented_feature[chan];
		option->pri_thresh_raw = pri_thresh_raw[chan];
		option->sec_segmented_feature = sec_segmented_feature[chan];
		option->sec_thresh_raw = sec_thresh_raw[chan];
		option->reverse_segmentation = reverse_segmentation[chan];
		option->sec_lt = sec_lt[chan];
		option->smooth_feature = smooth_feature[chan];
		option->adapt_threshold = adapt_threshold[chan];
		option->sec_segmentation = sec_segmentation[chan];
		option->sec_logic  = sec_logic[chan];
		option->hp_small = HP_small[chan];
		option->hp_big = HP_big[chan];

}


//---------------------------------------------------------------------------


bool Tsound_live::Analyze_Files(int chan)
{
	 // look at the top (sound) listbox for files
	 // if file exists, find for each of active slave channels a similar file name (zero on the list)
	 // all those file names should be stored in an array
	 // call calculate(wave) where waveiterators will be formed for all waves, and then send to feature_calc

	 // remove all this part -- after decideing to save a file -- then calculate age (by file age!) and change folder if necessary...

	   FileListBox[chan]->ItemIndex=0;
	  // option->wave_names[0]=FileListBox[chan]->FileName;
	  // option->extract_file_attributes();
	   //DataForm->Retrieve_segmentation_to_options(chan+3);      // got rid of the many calls...
	   Retrieve_segmentation_to_options(chan);
	  // option->amplitude_thresh= (125-DerivThresh[chan]->Position);
	   option->entropy_thresh=(float)(-entropyThresh[chan]->Position)/100.0;//(20-entropyThresh->Position)/10;
	   option->bird_ID=ID_[chan]->Text;//.ToIntDef(0);  // consider overiding from the extract file attributes
	   option->syllable_table_name=syllable_table[chan]->Caption;
	   option->features_table=ms_table[chan]->Caption;
	  // DrawIt=true;
	   // now we collect the appropriate file names from the slave channels, perhaps giving the system time and more chances
	   // to find the files just created. Name check for correct time (should be the same time stamp).

	   AnsiString sound_file=FileListBox[chan]->FileName; // this is the sound file name, that we will use as a refference...
	   bool wave_error=true;
	   if( wave_files[0].OpenFile(sound_file.c_str()) != pcmpos_error ) wave_error=false;
	   Calc(chan);  // compute all features and decide_file fate:

	   syllable_dur[chan]->Value=m_Calculator.maxSyllableDur;
	   bout_dur[chan]->Value=m_Calculator.maxBoutDur;
	   syllable_num[chan]->Value=m_Calculator.syllable_number;
	   if(DrawIt) plot_sonogram(chan);

	   if(passed)
	   {
		 if(create_bout_table->Checked)DataForm->insert_bout(ID_[chan]->Text, bout_dur[chan]->Value, m_Calculator.newFile);
		 file_ch[chan]->Caption=m_Calculator.newFile;
		 int iFileHandle = FileOpen(file_ch[chan]->Caption, fmOpenRead);
		 int size = FileSeek(iFileHandle,0,2)/1000; // size of file in Kb
		 FileClose(iFileHandle);
		 folder_size[chan]->Value+=(float)size/1000;
		// if(folder_size[chan]->Value<32768) DataForm->Settings(chan+1, "sound_folder_size", folder_size[chan]->Value);
	   }
		return(1);
}

//---------------------------------------------------------------------------

void Tsound_live::plot_sonogram(int chan)
{
	option->doNotSegment = false;
	//hide all features
	m_Calculator.GetSonogram().GetPitch().bRender = false;
	m_Calculator.GetSonogram().GetEntropy().bRender = false;
	m_Calculator.GetSonogram().GetFM().bRender = false;
	m_Calculator.GetSonogram().GetAM().bRender = false;
	m_Calculator.GetSonogram().GetDerivPow().bRender = false;
	m_Calculator.GetSonogram().Getmfa().bRender = false;
	m_Calculator.GetSonogram().GetPitchGoodness().bRender = false;
	m_Calculator.GetSonogram().GetPeakFreq().bRender = false;
	m_Calculator.GetSonogram().Getcepst0().bRender = false;
	m_Calculator.GetSonogram().Getcontinuity_t().bRender = false;
	m_Calculator.GetSonogram().Getcontinuity_f().bRender = false;
	m_Calculator.GetSonogram().Getprincipal_frequency().bRender = false;
	m_Calculator.GetSonogram().Getslope().bRender = false;
	m_Calculator.GetSonogram().Gethp_small().bRender = false;
	m_Calculator.GetSonogram().Gethp_big().bRender = false;
	m_Calculator.GetSonogram().Gethp_diff().bRender = false;

	// set brightness level
	m_Calculator.GetSonogram().SetBrightness(float(Brightness[chan]->Position/39.0625));

	// get threshold position of appropriate channel
	int pos = DerivThresh[chan]->Position * 88 / 125;
	int feature = segment_by->ItemIndex;
	switch(feature) {
		case 0: m_Calculator.GetSonogram().GetDerivPow().bRender = true; break;
		case 1: m_Calculator.GetSonogram().GetPitch().bRender = true; break;
		case 2: m_Calculator.GetSonogram().GetPeakFreq().bRender = true; break;
		case 3: m_Calculator.GetSonogram().GetPitchGoodness().bRender = true; break;
		case 4: m_Calculator.GetSonogram().GetFM().bRender = true; break;
		case 5: m_Calculator.GetSonogram().GetAM().bRender = true; break;
		case 6: m_Calculator.GetSonogram().GetEntropy().bRender = true; break;
		case 7: m_Calculator.GetSonogram().Getcontinuity_t().bRender = true; break;
		case 8:m_Calculator.GetSonogram().Getcontinuity_f().bRender = true; break;
		default: m_Calculator.GetSonogram().GetDerivPow().bRender = true;
	}

	if (option->sec_segmentation) {
		int featureSec = secondaryFeatureDD->ItemIndex;
		switch(featureSec) {
			case 0: m_Calculator.GetSonogram().GetDerivPow().bRender = true; break;
			case 1: m_Calculator.GetSonogram().GetPitch().bRender = true; break;
			case 2: m_Calculator.GetSonogram().GetPeakFreq().bRender = true; break;
			case 3: m_Calculator.GetSonogram().GetPitchGoodness().bRender = true; break;
			case 4: m_Calculator.GetSonogram().GetFM().bRender = true; break;
			case 5: m_Calculator.GetSonogram().GetAM().bRender = true; break;
			case 6: m_Calculator.GetSonogram().GetEntropy().bRender = true; break;
			case 7: m_Calculator.GetSonogram().Getcontinuity_t().bRender = true; break;
			case 8: m_Calculator.GetSonogram().Getcontinuity_f().bRender = true; break;
			default: m_Calculator.GetSonogram().GetDerivPow().bRender = true;
		}
	}

	if (option->smooth_feature || option->adapt_threshold) {
		m_Calculator.do_hpfilt();
		m_Calculator.Render_sonogram_images();
		if (option->adapt_threshold) m_Calculator.GetSonogram().Gethp_big().bRender = true;
		if (option->smooth_feature)  m_Calculator.GetSonogram().Gethp_small().bRender = true;
	}

	// segment the sound acording to the criteria
	int stop_duration = 0;
	for (int i = 0; i < m_Calculator.total_frames; i++) {
		if (m_Calculator.sound_detect_feature(i)) {
			m_Calculator.signal[i] = true;
			stop_duration = 0;
		}
		else {
			if (stop_duration <= option->min_stop_duration) m_Calculator.signal[i] = true;
			else m_Calculator.signal[i] = false;
			stop_duration++;
		}
	}

	// plot the sonogram
	m_Calculator.GetSonogram().DrawAllSlices(image[chan]);
}




void __fastcall Tsound_live::ProcTreshTBChange(TObject *Sender)
{

  Timer1->Interval=ProcTreshTB->Position*10;
  ProcTreshN->Value=ProcTreshTB->Position*10;
}
//---------------------------------------------------------------------------




void __fastcall Tsound_live::DerivThresh0Change(TObject *Sender)
{
  int bird;
  TShape *thresh;
  TTrackBar *drvThresh;
  TLabel *deriv_ind;
	   if(Sender==DerivThresh0) {bird=3; thresh=thresh0;  drvThresh=DerivThresh0;  deriv_ind=deriv_ind0;}
  else if(Sender==DerivThresh1) {bird=4; thresh=thresh1;  drvThresh=DerivThresh1;  deriv_ind=deriv_ind1;}
  else if(Sender==DerivThresh2) {bird=5; thresh=thresh2;  drvThresh=DerivThresh2;  deriv_ind=deriv_ind2;}
  else if(Sender==DerivThresh3) {bird=6; thresh=thresh3;  drvThresh=DerivThresh3;  deriv_ind=deriv_ind3;}
  else if(Sender==DerivThresh4) {bird=7; thresh=thresh4;  drvThresh=DerivThresh4;  deriv_ind=deriv_ind4;}
  else if(Sender==DerivThresh5) {bird=8; thresh=thresh5;  drvThresh=DerivThresh5;  deriv_ind=deriv_ind5;}
  else if(Sender==DerivThresh6) {bird=9; thresh=thresh6;  drvThresh=DerivThresh6;  deriv_ind=deriv_ind6;}
  else if(Sender==DerivThresh7) {bird=10; thresh=thresh7;  drvThresh=DerivThresh7;  deriv_ind=deriv_ind7;}
  else return;

  thresh->Pen->Color=clYellow;//clRed;
  DerivPowerB->Down=true;
  //int pos=(drvThresh->Position)/2;
  //option->amplitude_thresh=125-drvThresh->Position; //m_Calculator.GetSonogram().DrawAllSlices(image);
  //segmentation_panel->NumLab3->Value=option->amplitude_thresh;
  //deriv_ind->Caption=125-drvThresh->Position;
  option->pri_thresh_raw=drvThresh->Position;
  NumLab3->Value=option->adjustThreshold(segment_by->ItemIndex, option->pri_thresh_raw, option->frequency, 1);
  deriv_ind->Caption=(int)NumLab3->Value;
  //int pos=drvThresh->Position/3.6+30;
  int pos = drvThresh->Position * 88 / 125;
  thresh->Top=pos+10;
  DataForm->Settings(bird, "deriv_thresh", drvThresh->Position);
  update_vars();
}
//---------------------------------------------------------------------------


void __fastcall Tsound_live::save1Click(TObject *Sender)
{
 if(app_starting)return;
 int i;
        if(Sender==save0) i=0;
  else  if(Sender==save1) i=1;
  else  if(Sender==save2) i=2;
  else  if(Sender==save3) i=3;
  else  if(Sender==save4) i=4;
  else  if(Sender==save5) i=5;
  else  if(Sender==save6) i=6;
  else  if(Sender==save7) i=7;
  else return;

  if(save[i]->ItemIndex>1)check_tables();// tables need to be saved, make sure they exist
  if(!save[i]->ItemIndex) // pause state
  {
		rec[i]->Font->Color=(TColor)RGB(0,0,0);//clBlack;
		rec[i]->Font->Color=(TColor)RGB(0,0,0); rec[i]->Caption="Pause";
        chan_in[i]->Color=clWhite; chan_in[i]->ReadOnly=false; chan_in[i]->ShowHint=false;
        chan_out[i]->Color=clWhite; chan_out[i]->ReadOnly=false; chan_out[i]->ShowHint=false;
        ID_[i]->Color=clWhite; ID_[i]->ReadOnly=false; ID_[i]->ShowHint=false;

  }
  else
  {
    rec[i]->Font->Color=clBlue;
    rec[i]->Font->Color=clRed; rec[i]->Caption="Monitoring";
    chan_in[i]->Color=clBtnFace; chan_in[i]->ReadOnly=true; chan_in[i]->ShowHint=true;
    chan_out[i]->Color=clBtnFace; chan_out[i]->ReadOnly=true; chan_out[i]->ShowHint=true;
    ID_[i]->Color=clBtnFace; ID_[i]->ReadOnly=true; ID_[i]->ShowHint=true;
  }
}


//---------------------------------------------------------------------------


void __fastcall Tsound_live::PitchBClick(TObject *Sender)
{
   m_Calculator.GetSonogram().GetPitch().bRender = PitchB->Down;
}
//---------------------------------------------------------------------------

void __fastcall Tsound_live::EntropyBClick(TObject *Sender)
{
 m_Calculator.GetSonogram().GetEntropy().bRender = EntropyB->Down;
}
//---------------------------------------------------------------------------

void __fastcall Tsound_live::FMBClick(TObject *Sender)
{
 m_Calculator.GetSonogram().GetFM().bRender = FMB->Down;
}
//---------------------------------------------------------------------------

void __fastcall Tsound_live::PitchGoodnessBClick(TObject *Sender)
{
m_Calculator.GetSonogram().GetPitchGoodness().bRender = PitchGoodnessB->Down;
}
//---------------------------------------------------------------------------

void __fastcall Tsound_live::DerivPowerBClick(TObject *Sender)
{
m_Calculator.GetSonogram().GetDerivPow().bRender = DerivPowerB->Down;
}
//---------------------------------------------------------------------------

void __fastcall Tsound_live::PeakFrBClick(TObject *Sender)
{
m_Calculator.GetSonogram().GetPeakFreq().bRender = PeakFrB->Down;
}
//---------------------------------------------------------------------------

 
void __fastcall Tsound_live::input0Click(TObject *Sender)
{
    TFileListBox *fileList;  TEdit *input_folder;

    if(Sender==input0)      { fileList=FileListBox0; input_folder=chan0_in;}
    else if(Sender==input1) { fileList=FileListBox1; input_folder=chan1_in;}
    else if(Sender==input2) { fileList=FileListBox2; input_folder=chan2_in;}
    else if(Sender==input3) { fileList=FileListBox3; input_folder=chan3_in;}
    else if(Sender==input4) { fileList=FileListBox4; input_folder=chan4_in;}
    else if(Sender==input5) { fileList=FileListBox5; input_folder=chan5_in;}
    else if(Sender==input6) { fileList=FileListBox6; input_folder=chan6_in;}
    else if(Sender==input7) { fileList=FileListBox7; input_folder=chan7_in;}
    else return;

   OpenDialog1->FileName="not relevant";
   if(OpenDialog1->Execute())
   {
      fileList->Directory=ExtractFileDir(OpenDialog1->FileName);
      input_folder->Text=fileList->Directory+"\\";
   }   /* */
}


void __fastcall Tsound_live::startClick(TObject *Sender)
{
  bool all_paused=true;
  for(int i=0;i<8;i++)
  {
        rec[i]->Font->Color=clRed; rec[i]->Caption="monitoring...";
        if(save[i]->ItemIndex)all_paused=false;
		else {rec[i]->Font->Color=(TColor)RGB(0,0,0); rec[i]->Caption="Pause";}
  }
  if(all_paused)
  {
	Application->MessageBox(L"No channel is currently active. Please activate at least one channel (red label, right side of the channel display)", NULL, MB_OK);
	return;
  }

  startup_state->Caption="starting...";
  static bool first_time=true;
  IsBusy=false;
  TBorderIcons tempBI = BorderIcons;
  tempBI >> biSystemMenu;
  BorderIcons = tempBI;
  force_file_lists(); // make sure that input and output channels directories are okay
  if(first_time){first_time=false; check_tables(); }
  option->halt=false;
  option->move_sound=true;
  option->delete_trash_file=true;
  m_Calculator.m_Image.m_nMaxSlices=10000;
  set_output_folders();  // also find the age
  edit(on);   // dissallow editing

   m_Calculator.GetSonogram().GetPitch().bRender = false;
   m_Calculator.GetSonogram().GetEntropy().bRender = false;
   m_Calculator.GetSonogram().GetFM().bRender = false;
   m_Calculator.GetSonogram().GetAM().bRender = false;
   m_Calculator.GetSonogram().GetDerivPow().bRender = true;
   m_Calculator.GetSonogram().Getmfa().bRender = false;
   m_Calculator.GetSonogram().GetPitchGoodness().bRender = false;
   m_Calculator.GetSonogram().GetPeakFreq().bRender = false;
   m_Calculator.GetSonogram().Getcepst0().bRender = false;
   m_Calculator.GetSonogram().Getcontinuity_t().bRender = false;
   m_Calculator.GetSonogram().Getcontinuity_f().bRender = false;
   m_Calculator.GetSonogram().Getprincipal_frequency().bRender = false;
   m_Calculator.GetSonogram().Getslope().bRender = false;
   m_Calculator.GetSonogram().Gethp_small().bRender = false;
   m_Calculator.GetSonogram().Gethp_big().bRender = false;
   m_Calculator.GetSonogram().Gethp_diff().bRender = false;


   option->Draw_sonogram=false;

  start->Enabled=false;
  stop->Enabled=true;
  update_vars();
  Timer1->Enabled=true;
  startup_state->Caption="Monitoring channels";
}
//---------------------------------------------------------------------------


void Tsound_live::check_tables()
{

   for(int i=0;i<8;i++)
   {
     if(save[i]->ItemIndex==2 || save[i]->ItemIndex==4)DataForm->Create_table(syllable_table[i]->Caption); // option: save to syllable and/or ms table
     if(save[i]->ItemIndex==3 || save[i]->ItemIndex==4) DataForm->Create_table(ms_table[i]->Caption);  // option: save to ms table
   }
}


void __fastcall Tsound_live::output0Click(TObject *Sender)
{
    TEdit *output_folder;
    if(Sender==output0)      { output_folder=chan0_out;}
    else if(Sender==output1) { output_folder=chan1_out;}
    else if(Sender==output2) { output_folder=chan2_out;}
    else if(Sender==output3) { output_folder=chan3_out;}
    else if(Sender==output4) { output_folder=chan4_out;}
    else if(Sender==output5) { output_folder=chan5_out;}
    else if(Sender==output6) { output_folder=chan6_out;}
    else if(Sender==output7) { output_folder=chan7_out;}
    else return;

   OpenDialog1->FileName="not relevant";
   if(OpenDialog1->Execute()) output_folder->Text=ExtractFileDir(OpenDialog1->FileName)+"\\";
}
//---------------------------------------------------------------------------



void __fastcall Tsound_live::change_table0Click(TObject *Sender)
{

   TStaticText *RecordFile;
   //TNumLab *Rec_num;

   int bird;
		if(Sender==change_table0){bird=0; }
   else if(Sender==change_table1){bird=1; }
   else if(Sender==change_table2){bird=2; }
   else if(Sender==change_table3){bird=3; }
   else if(Sender==change_table4){bird=4;}
   else if(Sender==change_table5){bird=5; }
   else if(Sender==change_table6){bird=6; }
   else if(Sender==change_table7){bird=7; }
   else return;
   //DataForm->TheTable=DataForm->Records1;
   //RecordFile=RecordFile1;
   //Rec_num=recnum1;
   DataForm->openTableClick(this);
   folder_size[bird]->Value=folder_size_limit->Position+1;// this will start a new folder!
   syllable_table[bird]->Caption=DataForm->TheTable->TableName;
   recnum[bird]->Value=DataForm->recNum;
   ms_records[bird]->Value=0;
   DataForm->Settings(bird+3, "syllable_table", DataForm->TheTable->TableName);
   //str=DataForm->TheTable->TableName;
   str="raw_" + ID_[bird]->Text;
   Age[bird]=DataForm->find_age(bird+3);
   str+="_"; str+=Age[bird];
   DataForm->new_raw_table(str,0);//new_raw_brain_table(str);
   ms_table[bird]->Caption=str;

   str="file_table_";
   //str+=DataForm->TheTable->TableName;
   str+=ID_[bird]->Text;
   file_table[bird]->Caption=str;
   DataForm->file_table_name=file_table[bird]->Caption;
   if(!DataForm->table_exists(str))DataForm->Create_File_Table();
   DataForm->update_file_table_index(bird);
}
//---------------------------------------------------------------------------

void __fastcall Tsound_live::stopClick(TObject *Sender)
{
  TBorderIcons tempBI = BorderIcons;
  tempBI << biSystemMenu;
  BorderIcons = tempBI;
  Timer1->Enabled=false;
  for(int i=0; i<8;i++) {rec[i]->Font->Color=clBlue; rec[i]->Caption="engine off";}
  //rec1->Color=clBtnFace;  rec1->Font->Color=clRed;
  //rec1->Font->Size=60;  rec1->Caption="engine off";
  start->Enabled=true;
  stop->Enabled=false;
  edit(off);
  startup_state->Caption="Stopped";
}
//---------------------------------------------------------------------------

void __fastcall Tsound_live::BitBtn2Click(TObject *Sender)
{
  option->Visible=true;
  option->BringToFront();
  option->show_start=false;        
}
//---------------------------------------------------------------------------


void __fastcall Tsound_live::folder_size_limitChange(TObject *Sender)
{
        folder_sizeN->Value=folder_size_limit->Position;
}
//---------------------------------------------------------------------------


 void Tsound_live::edit(bool state)
{
     TColor color;
     if(state) color=clSilver;
     else color=clWhite;
     for(int i=0;i<8;i++)
     {
        chan_in[i]->Color=color; chan_in[i]->ReadOnly=state; chan_in[i]->ShowHint=state;
        chan_out[i]->Color=color; chan_out[i]->ReadOnly=state; chan_out[i]->ShowHint=state;
       // raw_features0->Color=color; raw_features0->ShowHint=state;
        ID_[i]->Color=color; ID_[i]->ReadOnly=state; ID_[i]->ShowHint=state;
     }
}



void __fastcall Tsound_live::chan0_inChange(TObject *Sender)
{
 update_file_lists();

}
//---------------------------------------------------------------------------


void __fastcall Tsound_live::chan0_outChange(TObject *Sender)
{

  int bird;
  TEdit *out;
  if(Sender==chan0_out) { bird=3; out=chan0_out;}
  else if(Sender==chan1_out) { bird=4; out=chan1_out;}
  else if(Sender==chan2_out) { bird=5; out=chan2_out;}
  else if(Sender==chan3_out) { bird=6; out=chan3_out;}
  else if(Sender==chan4_out) { bird=7; out=chan4_out;}
  else if(Sender==chan5_out) { bird=8; out=chan5_out;}
  else if(Sender==chan6_out) { bird=9; out=chan6_out;}
  else if(Sender==chan7_out) { bird=10; out=chan7_out;}

  DataForm->Settings(bird, "output_folder_sound", out->Text);

}

//---------------------------------------------------------------------------


void Tsound_live::retrieveFeatures()
{
  for(int i=0;i<8;i++)
  {
	try{
		 ID_[i]->Text=DataForm->Retrieve(i+3,"bird_ID");
		 entropyThresh[i]->Position=DataForm->Retrieve(i+3, "entropy_thresh").ToInt();
		 DerivThresh[i]->Position=DataForm->Retrieve(i+3, "deriv_thresh").ToInt();
		 chan_in[i]->Text=DataForm->Retrieve(i+3,"input_folder");
		 chan_out[i]->Text=DataForm->Retrieve(i+3,"output_folder_sound");
		 folder_size[i]->Value=DataForm->Retrieve(i+3, "sound_folder_size").ToInt();
		 syllable_table[i]->Caption=DataForm->Retrieve(i+3,"syllable_table");
		 Age[i]=DataForm->find_age(i+3);
		 ms_table[i]->Caption="raw_"+ID_[i]->Text +"_"+ Age[i];
		 //syllable_table[i]->Caption+"_"+ Age[i];
		 str="file_table_"; str+=ID_[i]->Text;//syllable_table[i]->Caption;
		 DataForm->file_table_name=str;
		 file_table[i]->Caption=str;
		 if(!DataForm->table_exists(str)) DataForm->Create_File_Table();
		 file_table_recs[i]->Value=DataForm->update_file_table_index(i);
		 save[i]->ItemIndex=DataForm->Retrieve_int(i+3, "save");
		  //  ms_table[i]->Caption=DataForm->Retrieve(i+3,"output_folder_features");

	}
	catch(...)
	{
		Application->MessageBox(L"could not open the settings table, default settings will be applied", NULL, MB_OK);
	}
  }
}


void __fastcall Tsound_live::new_table0Click(TObject *Sender)
{
   TStaticText *RecordFile;
   //TNumLab *folderSize, *Rec_num;
   AnsiString database;
   int bird;
   TDate dt;

		if(Sender==new_table0){bird=0; }
   else if(Sender==new_table1){bird=1; }
   else if(Sender==new_table2){bird=2; }
   else if(Sender==new_table3){bird=3; }
   else if(Sender==new_table4){bird=4; }
   else if(Sender==new_table5){bird=5; }
   else if(Sender==new_table6){bird=6; }
   else if(Sender==new_table7){bird=7; }
   else return;
	// this is the first bird of this moducle, bird 1 is reserved for other modules.
   //folderSize=folder_size1;
   //DataForm->TheTable=DataForm->Records1;
   //RecordFile=RecordFile1;

   //database="chan1";
   Age[bird]=DataForm->find_age(bird+3);
   DataForm->new_tableClick(this);
   folder_size[bird]->Value=folder_size_limit->Position+1;// this will start a new folder!
   syllable_table[bird]->Caption=DataForm->TheTable->TableName;
   recnum[bird]->Value=0;//DataForm->recNum;
   ms_records[bird]->Value=0;
   DataForm->Settings(bird+3, "syllable_table", DataForm->TheTable->TableName);
  // str=DataForm->TheTable->TableName;
   str="raw_"+ ID_[bird]->Text + "_" + Age[bird];
   DataForm->new_raw_table(str, 0);//new_raw_brain_table(str);
   ms_table[bird]->Caption=str;

   str="file_table_";
   //str+=DataForm->TheTable->TableName;
   str+=ID_[bird]->Text;
   file_table[bird]->Caption=str;
   DataForm->file_table_name=file_table[bird]->Caption;
   if(!DataForm->table_exists(str))DataForm->Create_File_Table();

}
//---------------------------------------------------------------------------



void __fastcall Tsound_live::SyllMinDurTbChange(TObject *Sender)
{
	 option->syllable_duration_thresh=1.4*SyllMinDurTb->Position;
	 SyllDurLow->Value=option->syllable_duration_thresh;
}
//---------------------------------------------------------------------------

void __fastcall Tsound_live::BoutMinDurTBChange(TObject *Sender)
{
   option->bout_duration_thresh=1.4*BoutMinDurTB->Position;
   BoutDurLow->Value=option->bout_duration_thresh;
}
//---------------------------------------------------------------------------

void __fastcall Tsound_live::min_syllablesChange(TObject *Sender)
{
  option->minimum_syllables_per_files=min_syllables->Position;
  mean_syllable_num->Value=min_syllables->Position;
}
//---------------------------------------------------------------------------


void __fastcall Tsound_live::AMClick(TObject *Sender)
{
m_Calculator.GetSonogram().GetAM().bRender = AM->Down;
}
//---------------------------------------------------------------------------

void __fastcall Tsound_live::BitBtn1Click(TObject *Sender)
{
   StartForm->Visible=true;
   StartForm->BringToFront();
}
//---------------------------------------------------------------------------


void __fastcall Tsound_live::slow_executionChange(TObject *Sender)
{
  option->slow_execution=slow_execution->Position;
}
//---------------------------------------------------------------------------


void __fastcall Tsound_live::folder_choiceClick(TObject *Sender)
{
  if(folder_choice->ItemIndex)
  {sizeLb->Visible=false; sizeLb2->Visible=false; folder_size_limit->Visible=false;
   folder_sizeN->Visible=false; sizeH2->Visible=true;}
  else
   {sizeLb->Visible=true; sizeLb2->Visible=true; folder_size_limit->Visible=true;
   folder_sizeN->Visible=true; sizeH2->Visible=false;}
}
//---------------------------------------------------------------------------

















void __fastcall Tsound_live::save_memClick(TObject *Sender)
{
   if(save_mem->ItemIndex)option->save_image_mem=false;
   else option->save_image_mem=true;
}
//---------------------------------------------------------------------------


void __fastcall Tsound_live::FormCreate(TObject *Sender)
{
   FileListBox[0]=FileListBox0;  FileListBox[1]=FileListBox1; FileListBox[2]=FileListBox2; FileListBox[3]=FileListBox3;
   FileListBox[4]=FileListBox4; FileListBox[5]=FileListBox5; FileListBox[6]=FileListBox6; FileListBox[7]=FileListBox7;
   // file_ch is a strange name for file name of channels 1, 2, ... n
   file_ch[0]=file_ch0; file_ch[1]=file_ch1;file_ch[2]=file_ch2;file_ch[3]=file_ch3;
   file_ch[4]=file_ch4; file_ch[5]=file_ch5; file_ch[6]=file_ch6;file_ch[7]=file_ch7;

   chan_in[0]=chan0_in; chan_in[1]=chan1_in; chan_in[2]=chan2_in; chan_in[3]=chan3_in;
   chan_in[4]=chan4_in; chan_in[5]=chan5_in; chan_in[6]=chan6_in; chan_in[7]=chan7_in;

   chan_out[0]=chan0_out; chan_out[1]=chan1_out; chan_out[2]=chan2_out; chan_out[3]=chan3_out;
   chan_out[4]=chan4_out; chan_out[5]=chan5_out; chan_out[6]=chan6_out; chan_out[7]=chan7_out;

   DerivThresh[0]=DerivThresh0; DerivThresh[1]=DerivThresh1; DerivThresh[2]=DerivThresh2; DerivThresh[3]=DerivThresh3;
   DerivThresh[4]=DerivThresh4; DerivThresh[5]=DerivThresh5; DerivThresh[6]=DerivThresh6; DerivThresh[7]=DerivThresh7;

   entropyThresh[0]=entropyThresh0; entropyThresh[1]=entropyThresh1; entropyThresh[2]=entropyThresh2; entropyThresh[3]=entropyThresh3;
   entropyThresh[4]=entropyThresh4; entropyThresh[5]=entropyThresh5; entropyThresh[6]=entropyThresh6; entropyThresh[7]=entropyThresh7;

   Brightness[0]=Brightness0; Brightness[1]=Brightness1; Brightness[2]=Brightness2; Brightness[3]=Brightness3;
   Brightness[4]=Brightness4; Brightness[5]=Brightness5; Brightness[6]=Brightness6; Brightness[7]=Brightness7;

   ID_[0]=ID_0; ID_[1]=ID_1; ID_[2]=ID_2; ID_[3]=ID_3;
   ID_[4]=ID_4; ID_[5]=ID_5; ID_[6]=ID_6; ID_[7]=ID_7;

   image[0]=Image1; image[1]=Image2;image[2]=Image3;image[3]=Image4;
   image[4]=Image5; image[5]=Image6;image[6]=Image7;image[7]=Image8;

   ms_table[0]=ms_table0; ms_table[1]=ms_table1; ms_table[2]=ms_table2; ms_table[3]=ms_table3;
   ms_table[4]=ms_table4; ms_table[5]=ms_table5; ms_table[6]=ms_table6; ms_table[7]=ms_table7;

   syllable_table[0]=syllable_table0; syllable_table[1]=syllable_table1;syllable_table[2]=syllable_table2;syllable_table[3]=syllable_table3;
   syllable_table[4]=syllable_table4; syllable_table[5]=syllable_table5;syllable_table[6]=syllable_table6;syllable_table[7]=syllable_table7;

   // save mode radio-group
   save[0]=save0; save[1]=save1;save[2]=save2;save[3]=save3;
   save[4]=save4; save[5]=save5;save[6]=save6;save[7]=save7;

   rec[0]=rec0; rec[1]=rec1; rec[2]=rec2; rec[3]=rec3;
   rec[4]=rec4; rec[5]=rec5; rec[6]=rec6; rec[7]=rec7;

   sampling[0]=sampling1; sampling[1]=sampling2; sampling[2]=sampling3; sampling[3]=sampling4;
   sampling[4]=sampling5; sampling[5]=sampling6; sampling[6]=sampling7; sampling[7]=sampling8;


   bout_dur[0]=bout_dur0; bout_dur[1]=bout_dur1;bout_dur[2]=bout_dur2;bout_dur[3]=bout_dur3;
   bout_dur[4]=bout_dur4; bout_dur[5]=bout_dur5;bout_dur[6]=bout_dur6;bout_dur[7]=bout_dur7;

   syllable_dur[0]=syll_dur0; syllable_dur[1]=syll_dur1;syllable_dur[2]=syll_dur2;syllable_dur[3]=syll_dur3;
   syllable_dur[4]=syll_dur4; syllable_dur[5]=syll_dur5;syllable_dur[6]=syll_dur6;syllable_dur[7]=syll_dur7;

   syllable_num[0]=syllable_num0; syllable_num[1]=syllable_num1;syllable_num[2]=syllable_num2;syllable_num[3]=syllable_num3;
   syllable_num[4]=syllable_num4; syllable_num[5]=syllable_num5;syllable_num[6]=syllable_num6;syllable_num[7]=syllable_num7;

   folder_size[0]=folder_size0; folder_size[1]=folder_size1;folder_size[2]=folder_size2;folder_size[3]=folder_size3;
   folder_size[4]=folder_size4; folder_size[5]=folder_size5;folder_size[6]=folder_size6;folder_size[7]=folder_size7;

   recnum[0]=recnum0;recnum[1]=recnum1;recnum[2]=recnum2;recnum[3]=recnum3;
   recnum[4]=recnum4;recnum[5]=recnum5;recnum[6]=recnum6;recnum[7]=recnum7;

   ms_records[0]=ms_recs0;ms_records[1]=ms_recs1;ms_records[2]=ms_recs2;ms_records[3]=ms_recs3;
   ms_records[4]=ms_recs4;ms_records[5]=ms_recs5;ms_records[6]=ms_recs6;ms_records[7]=ms_recs7;

   age[0]=age0;age[1]=age1;age[2]=age2;age[3]=age3;
   age[4]=age4;age[5]=age5;age[6]=age6;age[7]=age7;

   file_table[0]=file_table1; file_table[1]=file_table2; file_table[2]=file_table3; file_table[3]=file_table4;
   file_table[4]=file_table5; file_table[5]=file_table6; file_table[6]=file_table7; file_table[7]=file_table8;

   file_table_recs[0]=file_table_recs0;file_table_recs[1]=file_table_recs1;file_table_recs[2]=file_table_recs2;file_table_recs[3]=file_table_recs3;
   file_table_recs[4]=file_table_recs4;file_table_recs[5]=file_table_recs5;file_table_recs[6]=file_table_recs6;file_table_recs[7]=file_table_recs7;

   skip_channel1=false;
   option->squiz=2;
   option->bitmapHeight=256;
   option->brain_mode=false;
   option->keep_file_name=false;
   option->save_all=false;
   for(int i=0;i<8;i++)
   {
        DerivThresh[i]->Brush->Color=clRed;
        entropyThresh[i]->Brush->Color=clRed;
		Brightness[i]->Brush->Color=(TColor)RGB(0,0,0);
   }
   option->detect_low_noise=false;
   option->saveRecFiles=false;
  // Sonogram1=new Graphics::TBitmap();

   option->select_table=1;
   option->save_image_mem=true;//true;
   option->move_sound=true;
   option->delete_trash_file=true;
   option->keep_original_rawfile_name=false;
   option->format->ItemIndex=2; // get serial number and time from file name (recorder is the input)
   option->create_feature_arrays=true;//false;
   option->refine_segmentation=false;

   for(int i=0;i<6;i++)for(int j=0;j<100;j++)
   { Syll_types1[i][j]=0; Syll_types2[i][j]=0; Syll_types3[i][j]=0; Syll_types4[i][j]=0; }
}
//---------------------------------------------------------------------------

void __fastcall Tsound_live::on_start_timerTimer(TObject *Sender)
{
 //
  on_start_timer->Enabled=false;
   // we now upload the settings for each bird
   startup_state->Caption="retrieving state date from the settings table";
   DataForm->MyConnection1->Database="SAP";
   //DataForm->settings->TableName="settings";
   DataForm->settings->Active=true;
   retrieveFeatures();
   startup_state->Caption="Opening or creating input directories";
   // test the folders exsists, or create them!
   if (!DirectoryExists("c:\\SAP\\tmpSound\\"))if(!ForceDirectories("c:\\SAP\\tmpSound\\"))
		Application->MessageBox(L"cannot create folder c:\\tmpSound. This is the default folder of SA recorder for storage of sounds. Go to the SA recorder and make sure that each chanel has a set, and accessable temporary folder, otherwise, you might experience no recording input", NULL, MB_OK);


   if (!DirectoryExists(chan0_in->Text))
        if(!ForceDirectories(chan0_in->Text))
   {
     chan0_in->Text="c:\\sap\\in0\\";
	 if(!ForceDirectories(chan0_in->Text)) Application->MessageBox(L"SA+ failed to access or create input folder c:\\sap\\in0. Please type an alterative folder name for the input of chan0", NULL, MB_OK);
	 else Application->MessageBox(L"The input folder for chan0 is not accessible, SA+ has succeeded to revert to the defauld channel c:\\sap\\in0", NULL, MB_OK);
   }
   if(!DirectoryExists(chan1_in->Text)) if(!ForceDirectories(chan1_in->Text))
   {
     chan1_in->Text="c:\\sap\\in1\\";
	 if(!ForceDirectories(chan1_in->Text)) Application->MessageBox(L"SA+ failed to access or create input folder c:\\sap\\in1. Please type an alterative folder name for the input of chan1", NULL, MB_OK);
     else Application->MessageBox(L"The input folder for chan1 is not accessible, SA+ has succeeded to revert to the defauld channel c:\\sap\\in0", NULL, MB_OK);
   }
   if (!DirectoryExists(chan2_in->Text)) if(!ForceDirectories(chan2_in->Text))
   {
     chan2_in->Text="c:\\sap\\in2\\";
	 if(!ForceDirectories(chan2_in->Text)) Application->MessageBox(L"SA+ failed to access or create input folder c:\\sap\\in2. Please type an alterative folder name for the input of chan2", NULL, MB_OK);
	 else Application->MessageBox(L"The input folder for chan2 is not accessible, SA+ has succeeded to revert to the defauld channel c:\\sap\\in2", NULL, MB_OK);
   }
   if (!DirectoryExists(chan3_in->Text)) if(!ForceDirectories(chan3_in->Text))
   {
     chan3_in->Text="c:\\sap\\in3\\";
	 if(!ForceDirectories(chan3_in->Text)) Application->MessageBox(L"SA+ failed to access or create input folder c:\\sap\\in3. Please type an alterative folder name for the input of chan3", NULL, MB_OK);
	 else Application->MessageBox(L"The input folder for chan3 is not accessible, SA+ has succeeded to revert to the defauld channel c:\\sap\\in3", NULL, MB_OK);
   }
   if (!DirectoryExists(chan4_in->Text)) if(!ForceDirectories(chan4_in->Text))
   {
     chan4_in->Text="c:\\sap\\in4\\";
	 if(!ForceDirectories(chan4_in->Text)) Application->MessageBox(L"SA+ failed to access or create input folder c:\\sap\\in4. Please type an alterative folder name for the input of chan4", NULL, MB_OK);
	 else Application->MessageBox(L"The input folder for chan4 is not accessible, SA+ has succeeded to revert to the defauld channel c:\\sap\\in4", NULL, MB_OK);
   }
   if (!DirectoryExists(chan5_in->Text)) if(!ForceDirectories(chan5_in->Text))
   {
     chan5_in->Text="c:\\sap\\in5\\";
	 if(!ForceDirectories(chan5_in->Text)) Application->MessageBox(L"SA+ failed to access or create input folder c:\\sap\\in5. Please type an alterative folder name for the input of chan5", NULL, MB_OK);
	 else Application->MessageBox(L"The input folder for chan5 is not accessible, SA+ has succeeded to revert to the defauld channel c:\\sap\\in5", NULL, MB_OK);
   }
   if (!DirectoryExists(chan6_in->Text)) if(!ForceDirectories(chan6_in->Text))
   {
     chan6_in->Text="c:\\sap\\in6\\";
	 if(!ForceDirectories(chan6_in->Text)) Application->MessageBox(L"SA+ failed to access or create input folder c:\\sap\\in6. Please type an alterative folder name for the input of chan6", NULL, MB_OK);
	 else Application->MessageBox(L"The input folder for chan6 is not accessible, SA+ has succeeded to revert to the defauld channel c:\\sap\\in6", NULL, MB_OK);
   }
   if (!DirectoryExists(chan7_in->Text)) if(!ForceDirectories(chan7_in->Text))
   {
     chan7_in->Text="c:\\sap\\in7\\";
	 if(!ForceDirectories(chan7_in->Text)) Application->MessageBox(L"SA+ failed to access or create input folder c:\\sap\\in7. Please type an alterative folder name for the input of chan7", NULL, MB_OK);
	 else Application->MessageBox(L"The input folder for chan7 is not accessible, SA+ has succeeded to revert to the defauld channel c:\\sap\\in7", NULL, MB_OK);
   }
     // now set the input sound folders
   startup_state->Caption="Updating playback files list";
   update_file_lists();
   Brightness0Change(this);// updating brightnesses of the derivative displays

   startup_state->Caption="Opening or creating output directories";

   if (!DirectoryExists(chan0_out->Text)) if(!ForceDirectories(chan0_out->Text))
   {
     chan0_out->Text="c:\\sap\\out0\\";
	 if(!ForceDirectories(chan0_out->Text)) Application->MessageBox(L"SA+ failed to access or create input folder c:\\sap\\out0. Please type an alterative folder name for the input of chan0", NULL, MB_OK);
	 else Application->MessageBox(L"The input folder for chan0 is not accessible, SA+ has succeeded to revert to the defauld channel c:\\sap\\out0", NULL, MB_OK);
   }
   if (!DirectoryExists(chan1_out->Text)) if(!ForceDirectories(chan1_out->Text))
   {
     chan1_out->Text="c:\\sap\\out1\\";
	 if(!ForceDirectories(chan1_out->Text)) Application->MessageBox(L"SA+ failed to access or create input folder c:\\sap\\out1. Please type an alterative folder name for the input of chan1", NULL, MB_OK);
	 else Application->MessageBox(L"The input folder for chan1 is not accessible, SA+ has succeeded to revert to the defauld channel c:\\sap\\out0", NULL, MB_OK);
   }
   if (!DirectoryExists(chan2_out->Text)) if(!ForceDirectories(chan2_out->Text))
   {
     chan2_out->Text="c:\\sap\\out2\\";
	 if(!ForceDirectories(chan2_out->Text)) Application->MessageBox(L"SA+ failed to access or create input folder c:\\sap\\out2. Please type an alterative folder name for the input of chan2", NULL, MB_OK);
	 else Application->MessageBox(L"The input folder for chan2 is not accessible, SA+ has succeeded to revert to the defauld channel c:\\sap\\out2", NULL, MB_OK);
   }
   if (!DirectoryExists(chan3_out->Text)) if(!ForceDirectories(chan3_out->Text))
   {
     chan3_out->Text="c:\\sap\\out3\\";
	 if(!ForceDirectories(chan3_out->Text)) Application->MessageBox(L"SA+ failed to access or create input folder c:\\sap\\out3. Please type an alterative folder name for the input of chan3", NULL, MB_OK);
	 else Application->MessageBox(L"The input folder for chan3 is not accessible, SA+ has succeeded to revert to the defauld channel c:\\sap\\out3", NULL, MB_OK);
   }
   if (!DirectoryExists(chan4_out->Text)) if(!ForceDirectories(chan4_out->Text))
   {
     chan4_out->Text="c:\\sap\\out4\\";
	 if(!ForceDirectories(chan4_out->Text)) Application->MessageBox(L"SA+ failed to access or create input folder c:\\sap\\out4. Please type an alterative folder name for the input of chan4", NULL, MB_OK);
	 else Application->MessageBox(L"The input folder for chan4 is not accessible, SA+ has succeeded to revert to the defauld channel c:\\sap\\out4", NULL, MB_OK);
   }
   if (!DirectoryExists(chan5_out->Text)) if(!ForceDirectories(chan5_out->Text))
   {
     chan5_out->Text="c:\\sap\\out5\\";
	 if(!ForceDirectories(chan5_out->Text)) Application->MessageBox(L"SA+ failed to access or create input folder c:\\sap\\out5. Please type an alterative folder name for the input of chan5", NULL, MB_OK);
     else Application->MessageBox(L"The input folder for chan5 is not accessible, SA+ has succeeded to revert to the defauld channel c:\\sap\\out5", NULL, MB_OK);
   }
   if (!DirectoryExists(chan6_out->Text)) if(!ForceDirectories(chan6_out->Text))
   {
     chan6_out->Text="c:\\sap\\out6\\";
	 if(!ForceDirectories(chan6_out->Text)) Application->MessageBox(L"SA+ failed to access or create input folder c:\\sap\\out6. Please type an alterative folder name for the input of chan6", NULL, MB_OK);
	 else Application->MessageBox(L"The input folder for chan6 is not accessible, SA+ has succeeded to revert to the defauld channel c:\\sap\\out6", NULL, MB_OK);
   }
   if (!DirectoryExists(chan7_out->Text)) if(!ForceDirectories(chan7_out->Text))
   {
     chan7_out->Text="c:\\sap\\out7\\";
     if(!ForceDirectories(chan7_out->Text)) Application->MessageBox(L"SA+ failed to access or create input folder c:\\sap\\out7. Please type an alterative folder name for the input of chan7", NULL, MB_OK);
     else Application->MessageBox(L"The input folder for chan7 is not accessible, SA+ has succeeded to revert to the defauld channel c:\\sap\\out7", NULL, MB_OK);
   }
    startup_state->Caption="Calculating the current birds age";
   for(int i=0;i<8;i++) age[i]->Value=DataForm->find_age(i+3);
   app_starting=false;
   startup_state->Caption="Ready";
   start->Enabled=true;
  
}
//---------------------------------------------------------------------------



void __fastcall Tsound_live::update_tablesClick(TObject *Sender)
{
   retrieveFeatures();
}
//---------------------------------------------------------------------------

void __fastcall Tsound_live::change_bird1Click(TObject *Sender)
{
  int chan;
  if(Sender==change_bird1)chan=3;
  else if(Sender==change_bird2)chan=4;
  else if(Sender==change_bird3)chan=5;
  else if(Sender==change_bird4)chan=6;
  else if(Sender==change_bird5)chan=7;
  else if(Sender==change_bird6)chan=8;
  else if(Sender==change_bird7)chan=9;
  else if(Sender==change_bird8)chan=10;
  animals_form->datapages->ActivePageIndex=0;
  if (animals_form->ShowModal() == mrOk)
  {
	animals_form->Animals_to_settings(chan);
	retrieveFeatures();
	//update_tablesClick(this);
  }
  if(animals_form->new_bird) animals_form->Insert_bird_record();
}
//---------------------------------------------------------------------------







void __fastcall Tsound_live::advanceChange(TObject *Sender)
{
  advanceTb->Value=float(advance->Position)/44.1;
  option->advance->Position=advance->Position;
  option->advanceChange(this);
}
//---------------------------------------------------------------------------

void __fastcall Tsound_live::data_windowChange(TObject *Sender)
{
   windowTB->Value=(float)data_window->Position/44.1;
   option->data_window->Position=data_window->Position;
   option->data_windowChange(this);
}
//---------------------------------------------------------------------------

void __fastcall Tsound_live::create_bout_tableClick(TObject *Sender)
{
        if(create_bout_table->Checked==true)DataForm->Create_bouts();
}
//---------------------------------------------------------------------------


void __fastcall Tsound_live::BitBtn7Click(TObject *Sender)
{
  Application->HelpContext(970);
}
//---------------------------------------------------------------------------

void __fastcall Tsound_live::Save_configClick(TObject *Sender)
{
  save_settings();
  MessageDlg("Setting is now saved.", mtInformation, TMsgDlgButtons() << mbOK, 0);
}
//---------------------------------------------------------------------------




void __fastcall Tsound_live::show_segmentation_panel1Click(TObject *Sender)
{
  if(Sender==show_segmentation_panel1){option->chan=0; active_frame=1; segment_panels->Top=45;}
  else if(Sender==show_segmentation_panel2){option->chan=1; active_frame=2; segment_panels->Top=120; }
  else if(Sender==show_segmentation_panel3){option->chan=2; active_frame=3;segment_panels->Top=210; }
  else if(Sender==show_segmentation_panel4){option->chan=3; active_frame=4;segment_panels->Top=300; }
  else if(Sender==show_segmentation_panel5){option->chan=4; active_frame=5;segment_panels->Top=390;}
  else if(Sender==show_segmentation_panel6){option->chan=5; active_frame=6;segment_panels->Top=480;}
  else if(Sender==show_segmentation_panel7){option->chan=6; active_frame=7;segment_panels->Top=510;}
  else if(Sender==show_segmentation_panel8){option->chan=7; active_frame=8;segment_panels->Top=540;}

	DataForm->Retrieve_segmentation_to_options(active_frame+2);


    segment_by->ItemIndex = option->pri_segmented_feature;
	secondaryFeatureDD->ItemIndex = option->sec_segmented_feature;
	segmentation_flip->ItemIndex = option->reverse_segmentation;
	secondaryComparisonRB->ItemIndex = option->sec_lt;
	smoothGraphCB->Checked = option->smooth_feature;
	adaptThreshCB->Checked = option->adapt_threshold;
	secondaryFeatureCB->Checked = option->sec_segmentation;
	secondaryLogicRB->ItemIndex = option->sec_logic;
	hp_small->Position=option->hp_small/100;
	hp_big->Position=30*log(option->hp_big);
    // set threshold display for primary feature:
    NumLab3->Value = option->adjustThreshold(segment_by->ItemIndex, option->pri_thresh_raw, option->frequency, 1);
    secondaryThresholdTB->Value = option->adjustThreshold(secondaryFeatureDD->ItemIndex, option->sec_thresh_raw, option->frequency, 1);
	min_stop_durTb->Position=option->stop_duration_thresh;
	bout_stop->Position=option->bout_duration_thresh;
	smoothGraphCBClick(smoothGraphCB);
	secondaryFeatureCBClick(secondaryFeatureCB);
	adaptThreshCBClick(adaptThreshCB);


	if(segment_panels->Visible==false)segment_panels->Visible=true;
	else segment_panels->Visible=false;
}
//---------------------------------------------------------------------------

 void Tsound_live::save_frame_sf(TObject *Sender)
{

  //DataForm->Save_frame_params(active_frame+2);

    int chan = active_frame+2;
    AnsiString str;
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

  segment_panels->Visible=false;
  update_vars();


}






void __fastcall Tsound_live::recorderClick(TObject *Sender)
{
   if(FileExists("C:\\Program Files (x86)\\SAP2011\\Recorder2011.exe"))
	 ShellExecute(Application->Handle, "open", "C:\\Program Files (x86)\\SAP2011\\Recorder2011.exe", NULL, NULL, SW_SHOWNORMAL);
	 else if(FileExists("C:\\Program Files\\SAP2011\\Recorder2011.exe"))
		  ShellExecute(Application->Handle, "open", "C:\\Program Files\\SAP2011\\Recorder2011.exe", NULL, NULL, SW_SHOWNORMAL);
	 else ShellExecute(Application->Handle, "open", "Recorder2011.exe", NULL, NULL, SW_SHOWNORMAL);

}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

void __fastcall Tsound_live::segment_byChange(TObject *Sender)
{
    option->pri_segmented_feature = segment_by->ItemIndex;
	DataForm->Settings(active_frame+2, "pri_segmented_feature", String(segment_by->ItemIndex));
    update_vars();
	 //plot_sonogram(active_frame);
}
//---------------------------------------------------------------------------

void __fastcall Tsound_live::segmentation_flipClick(TObject *Sender)
{
     option->reverse_segmentation = segmentation_flip->ItemIndex;
	 DataForm->Settings(active_frame+2, "pri_lt", String(segmentation_flip->ItemIndex));
	 update_vars();
	 //plot_sonogram(active_frame);
}
//---------------------------------------------------------------------------

void __fastcall Tsound_live::smoothGraphCBClick(TObject *Sender)
{
     if (smoothGraphCB->Checked) {
		option->smooth_feature = true;
		hp_small->Visible = true;
		hp_small_num->Visible = true;
		StaticText49->Visible = true;
		DataForm->Settings(active_frame+2, "smooth_feature", "1");
	}
	else {
		option->smooth_feature = false;
		hp_small->Visible = false;
		hp_small_num->Visible = false;
		StaticText49->Visible = false;
        DataForm->Settings(active_frame+2, "smooth_feature", "0");
	}
	 //plot_sonogram(active_frame);
	 update_vars();
}
//---------------------------------------------------------------------------

void __fastcall Tsound_live::adaptThreshCBClick(TObject *Sender)
{
     if (adaptThreshCB->Checked) {
		option->adapt_threshold = true;
		hp_big->Visible = true;
		hp_big_num->Visible = true;
		StaticText50->Visible = true;
		DataForm->Settings(active_frame+2, "adapt_thresh", "1");
	} else {
		option->adapt_threshold = false;
		hp_big->Visible = false;
		hp_big_num->Visible = false;
		StaticText50->Visible = false;
        DataForm->Settings(active_frame+2, "adapt_thresh", "0");
	}
	//plot_sonogram(active_frame);
	update_vars();
}
//---------------------------------------------------------------------------

void __fastcall Tsound_live::hp_smallChange(TObject *Sender)
{
     option->HP_small->Position = hp_small->Position;
	hp_small_num->Value = hp_small->Position;
    option->hp_small = hp_small->Position * 100;
	DataForm->Settings(active_frame+2, "hp_small", String(hp_small->Position));
	//plot_sonogram(active_frame);
	update_vars();
}
//---------------------------------------------------------------------------

void __fastcall Tsound_live::hp_bigChange(TObject *Sender)
{
      float x;
	x = hp_big->Position;
	x /= 30; // range 1.0 to 10.0
	//option->hp_big = exp(x);
	option->HP_big->Position = hp_big->Position;
	hp_big_num->Value = hp_big->Position;
    option->hp_big = exp(x);
	DataForm->Settings(active_frame+2, "hp_big", String(hp_big->Position));
	//plot_sonogram(active_frame);
	update_vars();
}
//---------------------------------------------------------------------------

void __fastcall Tsound_live::secondaryFeatureCBClick(TObject *Sender)
{
	if (secondaryFeatureCB->Checked) {
		secondaryLogicRB->Visible = true;
		secondaryFeatureDD->Visible = true;
		secondaryComparisonRB->Visible = true;
		secondaryThresholdTB->Visible = true;
        option->sec_segmentation = true;

        switch(active_frame) {
           case 1: entropyThresh0->Visible=true; break;
           case 2: entropyThresh1->Visible=true; break;
           case 3: entropyThresh2->Visible=true; break;
           case 4: entropyThresh3->Visible=true; break;
           case 5: entropyThresh4->Visible=true; break;
           case 6: entropyThresh5->Visible=true; break;
           case 7: entropyThresh6->Visible=true; break;
           case 8: entropyThresh7->Visible=true; break;
           default: entropyThresh0->Visible=true;
        }
		DataForm->Settings(active_frame+2, "sec_segmentation", "1");
	}
	else {
		secondaryLogicRB->Visible = false;
		secondaryFeatureDD->Visible = false;
		secondaryComparisonRB->Visible = false;
		secondaryThresholdTB->Visible = false;
        option->sec_segmentation = false;
        switch(active_frame) {
           case 1: entropyThresh0->Visible=false; break;
           case 2: entropyThresh1->Visible=false; break;
           case 3: entropyThresh2->Visible=false; break;
           case 4: entropyThresh3->Visible=false; break;
           case 5: entropyThresh4->Visible=false; break;
           case 6: entropyThresh5->Visible=false; break;
           case 7: entropyThresh6->Visible=false; break;
           case 8: entropyThresh7->Visible=false; break;
           default: entropyThresh0->Visible=false;
        }
        DataForm->Settings(active_frame+2, "sec_segmentation", "0");
	}
	//plot_sonogram(active_frame);
	update_vars();
}
//---------------------------------------------------------------------------

void __fastcall Tsound_live::secondaryFeatureDDChange(TObject *Sender)
{
     option->sec_segmented_feature=secondaryFeatureDD->ItemIndex;
	 DataForm->Settings(active_frame+2, "sec_segmented_feature", String(secondaryFeatureDD->ItemIndex));
	 //plot_sonogram(active_frame);
	 update_vars();
}
//---------------------------------------------------------------------------

void __fastcall Tsound_live::secondaryComparisonRBClick(TObject *Sender)
{
     option->sec_lt = secondaryComparisonRB->ItemIndex;
     DataForm->Settings(active_frame+2, "sec_lt", String(secondaryComparisonRB->ItemIndex));
	 //plot_sonogram(active_frame);
	 update_vars();
}
//---------------------------------------------------------------------------

void __fastcall Tsound_live::secondaryLogicRBClick(TObject *Sender)
{
     option->sec_logic1 = secondaryLogicRB->ItemIndex;
	 DataForm->Settings(active_frame+2, "sec_logic", String(secondaryLogicRB->ItemIndex));
	 //plot_sonogram(active_frame);
	 update_vars();
}
//---------------------------------------------------------------------------

void __fastcall Tsound_live::min_stop_durTbChange(TObject *Sender)
{
     option->stop_duration_thresh = min_stop_durTb->Position;
	 min_stop_durN->Value=min_stop_durTb->Position;
     if (option->stop_duration_thresh > 0) {
        return;
    }
}
//---------------------------------------------------------------------------

void __fastcall Tsound_live::bout_stopChange(TObject *Sender)
{
    option->bout_duration_thresh = bout_stop->Position;
	 bout_Stop_N->Value=bout_stop->Position;
     if (option->stop_duration_thresh > 0) {
        return;
    }
}
//---------------------------------------------------------------------------

void __fastcall Tsound_live::saveClick(TObject *Sender)
{
	 save_frame_sf(Sender);
	 segment_panels->Visible=false;
}
//---------------------------------------------------------------------------



void __fastcall Tsound_live::LinkLabel1LinkClick(TObject *Sender, const UnicodeString Link,
          TSysLinkType LinkType)
{
  AnsiString link=Link;
  ShellExecute(0, "open", link.c_str(), 0, 0, 1);
}
//---------------------------------------------------------------------------

