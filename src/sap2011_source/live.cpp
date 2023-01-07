//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "live.h"
#include "CZT_FeatureImage.h"                                                                                                         
#include "options3.h"
#include "records.h"
#include "start.h"
#include "animals.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SDL_NumLab"
#pragma link "SDL_rchart"
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


Tsal *sal;
//---------------------------------------------------------------------------
__fastcall Tsal::Tsal(TComponent* Owner)
        : TForm(Owner)
{
   skip_channel1=false;
   option->squiz=2;
   option->bitmapHeight=256;
   option->brain_mode=false;
   active1=true; active2=true; active3=true; active4=true;
   option->keep_file_name=false;
   option->save_all=false;
   DerivThresh1->Brush->Color=clYellow;
   entropyThresh1->Brush->Color=clRed;
   Brightness1->Brush->Color=(TColor)RGB(0,0,0);
   option->detect_low_noise=false;
   option->saveRecFiles=false; 
   Sonogram1=new Graphics::TBitmap();
   option->chan=0;
   option->select_table=1;
   option->save_image_mem=true;
   option->move_sound=true;
   option->delete_trash_file=true;
   option->keep_original_rawfile_name=false;
   option->format->ItemIndex=2; // get serial number and time from file age...
   option->create_feature_arrays=true;//false;
   option->refine_segmentation=false;
  

   for(int i=0;i<6;i++)for(int j=0;j<100;j++)
   { Syll_types1[i][j]=0; Syll_types2[i][j]=0; Syll_types3[i][j]=0; Syll_types4[i][j]=0; }

   // we now upload the settings for each bird

}
//---------------------------------------------------------------------------



void Tsal::update_file_lists()
{
   if(DirectoryExists(chan0_in->Text))FileListBox0->Directory=chan0_in->Text;
   if(DirectoryExists(chan1_in->Text))FileListBox1->Directory=chan1_in->Text;
   if(DirectoryExists(chan2_in->Text))FileListBox2->Directory=chan2_in->Text;
   if(DirectoryExists(chan3_in->Text))FileListBox3->Directory=chan3_in->Text;
   if(DirectoryExists(chan4_in->Text))FileListBox4->Directory=chan4_in->Text;
   if(DirectoryExists(chan5_in->Text))FileListBox5->Directory=chan5_in->Text;
   if(DirectoryExists(chan6_in->Text))FileListBox6->Directory=chan6_in->Text;
   if(DirectoryExists(chan7_in->Text))FileListBox7->Directory=chan7_in->Text;
   if(DirectoryExists(chan8_in->Text))FileListBox8->Directory=chan8_in->Text;
   if(DirectoryExists(chan9_in->Text))FileListBox9->Directory=chan9_in->Text;
   if(DirectoryExists(chan10_in->Text))FileListBox10->Directory=chan10_in->Text;
}

void Tsal::force_file_lists()
{
   if(DirectoryExists(chan0_in->Text))if(!ForceDirectories(chan0_in->Text))Application->MessageBox(L"cannot create input folder for chan 0", NULL, MB_OK); else FileListBox0->Directory=chan0_in->Text;
   if(DirectoryExists(chan1_in->Text))if(!ForceDirectories(chan1_in->Text))Application->MessageBox(L"cannot create input folder for chan 1", NULL, MB_OK); else FileListBox1->Directory=chan1_in->Text;
   if(DirectoryExists(chan2_in->Text))if(!ForceDirectories(chan2_in->Text))Application->MessageBox(L"cannot create input folder for chan 2", NULL, MB_OK); else FileListBox2->Directory=chan2_in->Text;
   if(DirectoryExists(chan3_in->Text))if(!ForceDirectories(chan3_in->Text))Application->MessageBox(L"cannot create input folder for chan 3", NULL, MB_OK); else FileListBox3->Directory=chan3_in->Text;
   if(DirectoryExists(chan4_in->Text))if(!ForceDirectories(chan4_in->Text))Application->MessageBox(L"cannot create input folder for chan 4", NULL, MB_OK); else FileListBox4->Directory=chan4_in->Text;
   if(DirectoryExists(chan5_in->Text))if(!ForceDirectories(chan5_in->Text))Application->MessageBox(L"cannot create input folder for chan 5", NULL, MB_OK); else FileListBox5->Directory=chan5_in->Text;
   if(DirectoryExists(chan6_in->Text))if(!ForceDirectories(chan6_in->Text))Application->MessageBox(L"cannot create input folder for chan 6", NULL, MB_OK); else FileListBox6->Directory=chan6_in->Text;
   if(DirectoryExists(chan7_in->Text))if(!ForceDirectories(chan7_in->Text))Application->MessageBox(L"cannot create input folder for chan 7", NULL, MB_OK); else FileListBox7->Directory=chan7_in->Text;
   if(DirectoryExists(chan8_in->Text))if(!ForceDirectories(chan8_in->Text))Application->MessageBox(L"cannot create input folder for chan 8", NULL, MB_OK); else FileListBox8->Directory=chan8_in->Text;
   if(DirectoryExists(chan9_in->Text))if(!ForceDirectories(chan9_in->Text))Application->MessageBox(L"cannot create input folder for chan 9", NULL, MB_OK); else FileListBox9->Directory=chan9_in->Text;
   if(DirectoryExists(chan10_in->Text))if(!ForceDirectories(chan10_in->Text))Application->MessageBox(L"cannot create input folder for chan 10", NULL, MB_OK); else FileListBox10->Directory=chan10_in->Text;

   if(DirectoryExists(chan0_out->Text))if(!ForceDirectories(chan0_out->Text))Application->MessageBox(L"cannot create output folder for chan 0", NULL, MB_OK);
   if(DirectoryExists(chan1_out->Text))if(!ForceDirectories(chan1_out->Text))Application->MessageBox(L"cannot create output folder for chan 1", NULL, MB_OK);
   if(DirectoryExists(chan2_out->Text))if(!ForceDirectories(chan2_out->Text))Application->MessageBox(L"cannot create output folder for chan 2", NULL, MB_OK);
   if(DirectoryExists(chan3_out->Text))if(!ForceDirectories(chan3_out->Text))Application->MessageBox(L"cannot create output folder for chan 3", NULL, MB_OK);
   if(DirectoryExists(chan4_out->Text))if(!ForceDirectories(chan4_out->Text))Application->MessageBox(L"cannot create output folder for chan 4", NULL, MB_OK);
   if(DirectoryExists(chan5_out->Text))if(!ForceDirectories(chan5_out->Text))Application->MessageBox(L"cannot create output folder for chan 5", NULL, MB_OK);
   if(DirectoryExists(chan6_out->Text))if(!ForceDirectories(chan6_out->Text))Application->MessageBox(L"cannot create output folder for chan 6", NULL, MB_OK);
   if(DirectoryExists(chan7_out->Text))if(!ForceDirectories(chan7_out->Text))Application->MessageBox(L"cannot create output folder for chan 7", NULL, MB_OK);
   if(DirectoryExists(chan8_out->Text))if(!ForceDirectories(chan8_out->Text))Application->MessageBox(L"cannot create output folder for chan 8", NULL, MB_OK);
   if(DirectoryExists(chan9_out->Text))if(!ForceDirectories(chan9_out->Text))Application->MessageBox(L"cannot create output folder for chan 9", NULL, MB_OK);
   if(DirectoryExists(chan10_out->Text))if(!ForceDirectories(chan10_out->Text))Application->MessageBox(L"cannot create output folder for chan 10", NULL, MB_OK);

}






void Tsal::Reset_wavefiles(int num_slaves)
{
  wave_files[0].ResetContents();
  if(num_slaves)for(int i=1;i<=num_slaves;i++) wave_files[i].ResetContents();
}



void Tsal::Delete_wavefiles(int num_slaves)
{
    DeleteFile(FileListBox0->FileName);
    if(num_slaves>0) DeleteFile(FileListBox1->FileName);
    if(num_slaves>1) DeleteFile(FileListBox2->FileName);
    if(num_slaves>2) DeleteFile(FileListBox3->FileName);
    if(num_slaves>3) DeleteFile(FileListBox4->FileName);
    if(num_slaves>4) DeleteFile(FileListBox5->FileName);
    if(num_slaves>5) DeleteFile(FileListBox6->FileName);
    if(num_slaves>6) DeleteFile(FileListBox7->FileName);
    if(num_slaves>7) DeleteFile(FileListBox8->FileName);
    if(num_slaves>8) DeleteFile(FileListBox9->FileName);
    if(num_slaves>9) DeleteFile(FileListBox10->FileName);
}




//void Tsal::Calc(AnsiString fileName, TStaticText *rec)
bool Tsal::Calc(int num_slaves)
{
     static AnsiString oldName="empty";
     if(oldName==FileListBox0->FileName)
     { // this should be replaced by a 'reset and delete all' function, for all wave_Files.
       // here should come a for loop for num_slaves+1...
       Reset_wavefiles(num_slaves);
       Delete_wavefiles(num_slaves);

       //DeleteFile(fileName); return;}  // get rid of bad files
       // note, the same should be done for the rest of the file lists....
       return(false);
     }

     oldName=FileListBox0->FileName;
     if(DrawIt)m_Calculator.render_image=true;
     else m_Calculator.render_image=false;
     m_Calculator.RenderImage(DrawIt);// yes render it
     CZT_PcmEq equalizer;// doesnt do anything (yet)
     //if(wave.OpenFile(fileName.c_str()) != pcmpos_error)
     //{
          option->wave_names[0]=FileListBox0->FileName;
          if(equalizeIt) wave_files[0].UseFilter(equalizer);
          if(m_Calculator.Calculate(wave_files, num_slaves)) // return true if the sound contain meaningful data
          {
			 m_Calculator.save_to_database(0);  // this will take care of renaming and moving the data-containing files
			 rec1->Color=(TColor)RGB(0,0,0);
             rec1->Caption="Passed";
             passed=true;
            file_table_recs0->Value++;
             if(save1->ItemIndex==0 || save1->ItemIndex==2)
                        ms_recs0->Value+=option->ms_recordes_added; // ms table saved
             if(save1->ItemIndex==0 || save1->ItemIndex==1)
                        recnum1->Value+=option->syllCounter;

          }
          else //  the files are empty of meaningful data and should be deleted
          {
             passed=false; rec1->Color=clGreen; rec1->Caption="Rejected";
             Delete_wavefiles(num_slaves);
            /*if(num_slaves)
             {
                for(int i=0; i<m_Calculator.slices; i++) delete[] m_Calculator.slave_chans[i];
                delete[] m_Calculator.slave_chans;
             } */ 
          }
          return(true);
    // }
    // else DeleteFile(fileName);
 }


//---------------------------------------------------------------------------


void __fastcall Tsal::Brightness1Change(TObject *Sender)
{
     TTrackBar *Brightness;
     if(Sender==Brightness1){Brightness=Brightness1;}
    
       // Numbers come in flavor of 1 to 20000 : 10000 = middle: use magic voodoo number to
     // set brightness: valid brightness values are somehwhere in the range of  1.0f-600.0f (256.0f = normal)
     // So there are actually better ways to make a UI-Brightness slider than what I am doing
     float bright=exp((float)Brightness->Position/700);
     m_Calculator.GetImage().SetBrightness(bright);//float(Brightness->Position/39.0625));
    // m_Calculator.GetImage().DrawAllSlices(image);

}
//---------------------------------------------------------------------------


void __fastcall Tsal::entropyThresh1Change(TObject *Sender)
{
  int bird;
  TTrackBar *entropyThresh, *DerivThresh; TImage *image; Graphics::TBitmap *Sonogram; TNumLab *ent_ind;
  if(Sender==entropyThresh1) {bird=2; image=Image1; entropyThresh=entropyThresh1; DerivThresh=DerivThresh1; Sonogram=Sonogram1; ent_ind=ent_ind1;}
  
  EntropyB->Down=true;  DerivPowerB->Down=true;

  int pos=entropyThresh->Position/3.6;
  option->entropy_thresh=(float)(-entropyThresh->Position)/100.0;
  //  entropy_val->Value=(float)-entropyTb->Position/100;


 // int pos=entropyThresh->Position;
 // option->entropy_thresh=(75-entropyThresh->Position)/5;
  ent_ind->Value=-(float)entropyThresh->Position/100;//entropyThresh->Position;
  image->Picture->Bitmap=Sonogram;
  image->Canvas->Pen->Color=clRed;
  image->Canvas->MoveTo(0,pos);
  image->Canvas->LineTo(image->Width,pos);
  image->Canvas->Pen->Color=clYellow;
  pos=DerivThresh->Position;
  image->Canvas->MoveTo(0,pos);
  image->Canvas->LineTo(image->Width,pos);
  //DataForm->Change_Database("SAP");
  DataForm->Settings(bird, "entropy_thresh", entropyThresh->Position);
}
//---------------------------------------------------------------------------


void __fastcall Tsal::image_stretch1Click(TObject *Sender)
{
 if(image_stretch1->ItemIndex)
       {Image1->AutoSize=true;Image1->Stretch=false;}//Image1->Top=10; Image1->Height=200; }
  else {Image1->AutoSize=false; Image1->Stretch=true; Image1->Width=663; }
}
//---------------------------------------------------------------------------

void Tsal::save_settings()
{
    DataForm->Settings(2, "input_folder", chan0_in->Text);
    DataForm->Settings(3, "input_folder", chan1_in->Text);
    DataForm->Settings(4, "input_folder", chan2_in->Text);
    DataForm->Settings(5, "input_folder", chan3_in->Text);
    DataForm->Settings(6, "input_folder", chan4_in->Text);
    DataForm->Settings(7, "input_folder", chan5_in->Text);
    DataForm->Settings(8, "input_folder", chan6_in->Text);
    DataForm->Settings(9, "input_folder", chan7_in->Text);
    DataForm->Settings(10, "input_folder", chan8_in->Text);
    DataForm->Settings(11, "input_folder", chan9_in->Text);
    DataForm->Settings(12, "input_folder", chan10_in->Text);

    DataForm->Settings(2, "output_folder_sound", chan0_out->Text);
    DataForm->Settings(3, "output_folder_sound", chan1_out->Text);
    DataForm->Settings(4, "output_folder_sound", chan2_out->Text);
    DataForm->Settings(5, "output_folder_sound", chan3_out->Text);
    DataForm->Settings(6, "output_folder_sound", chan4_out->Text);
    DataForm->Settings(7, "output_folder_sound", chan5_out->Text);
    DataForm->Settings(8, "output_folder_sound", chan6_out->Text);
    DataForm->Settings(9, "output_folder_sound", chan7_out->Text);
    DataForm->Settings(10, "output_folder_sound", chan8_out->Text);
    DataForm->Settings(11, "output_folder_sound", chan9_out->Text);
    DataForm->Settings(12, "output_folder_sound", chan10_out->Text);

    if(slave1_active->Checked)str="1"; else str="0"; DataForm->Settings(3, "active_or_slave", str);
    if(slave2_active->Checked)str="1"; else str="0"; DataForm->Settings(4, "active_or_slave", str);
    if(slave3_active->Checked)str="1"; else str="0"; DataForm->Settings(5, "active_or_slave", str);
    if(slave4_active->Checked)str="1"; else str="0"; DataForm->Settings(6, "active_or_slave", str);
    if(slave5_active->Checked)str="1"; else str="0"; DataForm->Settings(7, "active_or_slave", str);
    if(slave6_active->Checked)str="1"; else str="0"; DataForm->Settings(8, "active_or_slave", str);
    if(slave7_active->Checked)str="1"; else str="0"; DataForm->Settings(9, "active_or_slave", str);
    if(slave8_active->Checked)str="1"; else str="0"; DataForm->Settings(10, "active_or_slave", str);
    if(slave9_active->Checked)str="1"; else str="0"; DataForm->Settings(11, "active_or_slave", str);
    if(slave10_active->Checked)str="1"; else str="0"; DataForm->Settings(12, "active_or_slave", str);

    DataForm->Settings(3, "slave_output", slave_state1->ItemIndex);
    DataForm->Settings(4, "slave_output", slave_state2->ItemIndex);
    DataForm->Settings(5, "slave_output", slave_state3->ItemIndex);
    DataForm->Settings(6, "slave_output", slave_state4->ItemIndex);
    DataForm->Settings(7, "slave_output", slave_state5->ItemIndex);
    DataForm->Settings(8, "slave_output", slave_state6->ItemIndex);
    DataForm->Settings(9, "slave_output", slave_state7->ItemIndex);
    DataForm->Settings(10, "slave_output", slave_state8->ItemIndex);
    DataForm->Settings(11, "slave_output", slave_state9->ItemIndex);
    DataForm->Settings(12, "slave_output", slave_state10->ItemIndex);
}






void __fastcall Tsal::FormClose(TObject *Sender, TCloseAction &Action)
{
  if (MessageDlg("Save settings?", mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes) save_settings();
  StartForm->Visible=true;
  delete Sonogram1; 
  //delete sal;
}
//---------------------------------------------------------------------------


void __fastcall Tsal::FormShow(TObject *Sender)
{
       m_Calculator.GetImage().GetPitch().bRender = false;
       m_Calculator.GetImage().GetEntropy().bRender = true;
       m_Calculator.GetImage().GetFM().bRender = false;
       m_Calculator.GetImage().GetAM().bRender = false;
       m_Calculator.GetImage().GetDerivPow().bRender = true;
       m_Calculator.GetImage().Getmfa().bRender = false;
       m_Calculator.GetImage().GetPitchGoodness().bRender = false;
       m_Calculator.GetImage().GetPeakFreq().bRender = false;
       m_Calculator.GetImage().Getcepst0().bRender = false;
       m_Calculator.GetImage().Getcontinuity_t().bRender = false;
       m_Calculator.GetImage().Getcontinuity_f().bRender = false;
       m_Calculator.GetImage().Getprincipal_frequency().bRender = false;
       m_Calculator.GetImage().Getslope().bRender = false;

       option->Draw_sonogram=false;
}
//---------------------------------------------------------------------------



void __fastcall Tsal::Timer1Timer(TObject *Sender)
{
      // first we handle the 'round the clock' statistics
      static int min15=0, oldTime=0; // in ms = 900000
      min15+=Timer1->Interval;
      if(min15==900000)
      {
        // reset all counts at 23:00
        if(option->hour==23)for(int i=0; i<5; i++)for(int j=0; j<6; j++) option->types[i][j]=0;
        min15=0;
        int time=option->hour*4+option->minute/15; // set a time scale from 0-100 steps of 15 minutes around the clock
        if(time!=oldTime) for(int syll_type=0;syll_type<6;syll_type++)
        {
            Syll_types1[syll_type][time]=option->types[1][syll_type];
            option->types[1][syll_type]=0;
            Chart1->Series[syll_type]->Clear();
            for(int j=0;j<100;j++)
                Chart1->Series[syll_type]->AddXY( j/4,Syll_types1[syll_type][j], j/4, clBlue );

            
        }
        oldTime=time;
     }

     // in case of problems...
      if(option->DeleteFile_failed)
      {
         if(DeleteFile(option->File_to_delete)) {option->DeleteFile_failed=false; skip_channel1=false;}
         else skip_channel1=true;
         if(!FileExists(option->File_to_delete)) {option->DeleteFile_failed=false; skip_channel1=false;}
      }

      // okay, do the management job...
      if(!IsBusy)Analyze_Files();
      problem_filename->Caption=option->problem_file;

      if(option->halt)
      {
         stopClick(this);
		 Application->MessageBox((wchar_t*)option->Message.c_str(), NULL, MB_OK);

      }

}





void Tsal::set_output_folders()
{
       option->bird_age=DataForm->find_age(recorder_chan->Value+1);
       Age1=option->bird_age;
       bird_age->Text=Age1;
       if(folder_choice->ItemIndex)
       {
         option->moveTo_sound_folders[0]=chan0_out->Text+Age1+"\\";
         option->moveTo_sound_folders[1]=chan1_out->Text+Age1+"\\";
         option->moveTo_sound_folders[2]=chan2_out->Text+Age1+"\\";
         option->moveTo_sound_folders[3]=chan3_out->Text+Age1+"\\";
         option->moveTo_sound_folders[4]=chan4_out->Text+Age1+"\\";
         option->moveTo_sound_folders[5]=chan5_out->Text+Age1+"\\";
         option->moveTo_sound_folders[6]=chan6_out->Text+Age1+"\\";
         option->moveTo_sound_folders[7]=chan7_out->Text+Age1+"\\";
         option->moveTo_sound_folders[8]=chan8_out->Text+Age1+"\\";
         option->moveTo_sound_folders[9]=chan9_out->Text+Age1+"\\";
         option->moveTo_sound_folders[10]=chan10_out->Text+Age1+"\\";

       }
       else
       {
         option->moveTo_sound_folders[0]=chan0_out->Text+"\\"; // this is the destination folder
         option->moveTo_sound_folders[1]=chan1_out->Text+"\\";
         option->moveTo_sound_folders[2]=chan2_out->Text+"\\";
         option->moveTo_sound_folders[3]=chan3_out->Text+"\\";
         option->moveTo_sound_folders[4]=chan4_out->Text+"\\";
         option->moveTo_sound_folders[5]=chan5_out->Text+"\\";
         option->moveTo_sound_folders[6]=chan6_out->Text+"\\";
         option->moveTo_sound_folders[7]=chan7_out->Text+"\\";
         option->moveTo_sound_folders[8]=chan8_out->Text+"\\";
         option->moveTo_sound_folders[9]=chan9_out->Text+"\\";
         option->moveTo_sound_folders[10]=chan10_out->Text+"\\";
       }
}







//---------------------------------------------------------------------------

bool Tsal::Analyze_Files()
{

     // look at the top (sound) listbox for files
     // if file exists, find for each of active slave channels a similar file name (zero on the list)
     // all those file names should be stored in an array
     // call calculate(wave) where waveiterators will be formed for all waves, and then send to feature_calc
     IsBusy=true;
     static int oldDay=option->day;
     AnsiString Slave_name;

     TFileListBox *FileListBox[11];
     FileListBox[0]=FileListBox0;  FileListBox[1]=FileListBox1; FileListBox[2]=FileListBox2; FileListBox[3]=FileListBox3;
     FileListBox[4]=FileListBox4; FileListBox[5]=FileListBox5; FileListBox[6]=FileListBox6; FileListBox[7]=FileListBox7;
     FileListBox[8]=FileListBox8; FileListBox[9]=FileListBox9; FileListBox[10]=FileListBox10;

     TCheckBox *slave_active[11];
     slave_active[1]=slave1_active; slave_active[2]=slave2_active;slave_active[3]=slave3_active;slave_active[4]=slave4_active;
     slave_active[5]=slave5_active;slave_active[6]=slave6_active;slave_active[7]=slave7_active;slave_active[8]=slave8_active;
     slave_active[9]=slave9_active;slave_active[10]=slave10_active;

     TRChart *chanGraph[11];
     chanGraph[1]=RChart1;  chanGraph[2]=RChart2; chanGraph[3]=RChart3; chanGraph[4]=RChart4; chanGraph[5]=RChart5;
     chanGraph[6]=RChart6; chanGraph[7]=RChart7; chanGraph[8]=RChart8; chanGraph[9]=RChart9; chanGraph[10]=RChart10;

     TLabel *file_ch[11];
     file_ch[0]=file_ch0; file_ch[1]=file_ch1;file_ch[2]=file_ch2;file_ch[3]=file_ch3;file_ch[4]=file_ch4;file_ch[5]=file_ch5;
     file_ch[6]=file_ch6;file_ch[7]=file_ch7;file_ch[8]=file_ch8;file_ch[9]=file_ch9;file_ch[10]=file_ch10;

     TStaticText *saved_file[9];
     saved_file[0]=saved_file0; saved_file[1]=saved_file1;saved_file[2]=saved_file2;saved_file[3]=saved_file3;saved_file[4]=saved_file4;saved_file[5]=saved_file5;
     saved_file[6]=saved_file6;saved_file[7]=saved_file7;saved_file[8]=saved_file8;

     int num_chan=0; // including the sound channel
     AnsiString str1;

     FileListBox[0]->Update();
     for(int i=1;i<=num_slaves->Value;i++)
     {
        if(slave_active[i]->Checked)FileListBox[i]->Update();
     }
     if(!FileListBox[0]->Items->Count)
     {
        IsBusy=false; return(0);
     } // nothing to do...

     //  we decide if to generate new folders based on day to day folder scheme:
   /* */

    FileListBox[0]->ItemIndex=0;
    option->wave_names[0]=FileListBox[0]->FileName; // this is the file we are going to analyze
    option->extract_file_attributes(FileListBox[0]->FileName);   // we now have the time stamp of the file for the Records


     if(folder_choice->ItemIndex && oldDay!=option->day)// change a folder every day, accordign to the age of each bird...
     {
       startup_state->Caption="Changing folder & updating age";
       folder_size1->Value=0;
       Age1=DataForm->find_age(recorder_chan->Value+1);
       str="raw_"+ ID_1->Text +"_" + Age1;
      // DataForm->Change_Database("chan1");
       DataForm->new_raw_table(str, Slaves); // create a new raw table
       option->features_table=str;
       slaves_in_table->Value=DataForm->chans_exist(str, 1);
       ms_table1->Caption=str;
       Age1+="\\";
       str=chan0_out->Text+ Age1;
       if (!DirectoryExists(str)) if(!ForceDirectories(str))
				 Application->MessageBox(L"Folder 1 could not be changed!", NULL, MB_OK);
       oldDay=option->day;
       set_output_folders();
       startup_state->Caption="Done Changing folder";
      // DataForm->Change_Database("SAP");
     }


     // the other setting is changing folder when its size is large enough (for DVD size...)
     if( (folder_choice->ItemIndex==0) && folder_size1->Value>folder_size_limit->Position)// size comparison in Mb, change folder name;
     {   // first, we set the new folder name based on the first file included in it
        AnsiString subdir_name=ID_1->Text;
        subdir_name+="_"; subdir_name+=option->month; subdir_name+="_"; subdir_name+=option->day;
        subdir_name+="_"; subdir_name+=option->hour; subdir_name+="_"; subdir_name+=option->minute;
        AnsiString current_folder_name=chan0_out->Text;
        int i=current_folder_name.Length()-1;
        do{i--;}while(current_folder_name[i]!='\\');
        current_folder_name.SetLength(i);
        // that's it, now check that folder can be created...
        str=current_folder_name+subdir_name+"\\";
        if (!DirectoryExists(str)) if(!ForceDirectories(str))
        {
		   Application->MessageBox(L"Folder size quata has been reached, but folder could not be changed: folder size monitoring has been reset", NULL, MB_OK);
           folder_size1->Value=0;
        }
        else { chan0_out->Text=str; folder_size1->Value=0;
        }
     }

     
   // then we process the sound

       //if(equalize->Checked)
       equalizeIt=true;// else equalizeIt=false;

      // option->raw_feature_folder=raw_features->Text;
       option->syllCounter=0;// reset the syllables counter
     //  FileListBox[0]->ItemIndex=0;
     //  option->wave_names[0]=FileListBox[0]->FileName; // this is the file we are going to analyze
     //  option->extract_file_attributes();   // we now have the time stamp of the file for the Records
       switch(save1->ItemIndex)
       {
         case 0:  option->saveRawFile=true; option->saveRecFiles=true;   break;
         case 1:  option->saveRawFile=false; option->saveRecFiles=true;  break;
         case 2:  option->saveRawFile=true; option->saveRecFiles=false;  break;
         case 3:  option->saveRawFile=false; option->saveRecFiles=false; break;
       }
   //    option->Records_File=RecordFile->Caption; // this is the main output file for this bird, with syllable features
   //    option->amplitude_thresh= (125-DerivThresh1->Position);
       option->entropy_thresh=(float)(-entropyThresh1->Position)/100.0;//(20-entropyThresh->Position)/10;
       option->bird_ID=ID_1->Text;//.ToIntDef(0);  // consider overiding from the extract file attributes
       //option->serial_number=fileNum;
       DrawIt=true;
       // now we collect the appropriate file names from the slave channels, perhaps giving the system time and more chances
       // to find the files just created. Name check for correct time (should be the same time stamp).

       AnsiString sound_file=FileListBox0->FileName; // this is the sound file name, that we will use as a refference...
       bool wave_error=true;

   if( wave_files[0].OpenFile(sound_file.c_str()) != pcmpos_error ) wave_error=false;


       // see if the first active slave channel has a matching wave file...
       file_ch[0]->Caption=ExtractFileName(FileListBox[0]->FileName);
       for(int i=1;i<11;i++)
       {
            if(slave_active[i]->Checked)
           {
             int diff, files_waiting=FileListBox[i]->Items->Count;
             num_chan++;
             FileListBox[i]->Update();
             if(!FileListBox[i]->Items->Count){IsBusy=false; return(0); }// nothing to do until the file shows up...
             FileListBox[i]->ItemIndex=0;
             bool found_slave_match=false;
             for(int j=0;j<files_waiting;j++)
             {
                str= ExtractFileName(sound_file);
                str=str.SubString(str.Length()-20, 17);
                str1=ExtractFileName(FileListBox[i]->FileName);
                str1=str1.SubString(str1.Length()-20, 17);
                diff=str.AnsiCompare(str1);
                if(!diff){found_slave_match=true; Slave_name= FileListBox[i]->FileName; break;}
                FileListBox[i]->ItemIndex++;
             }
             if(found_slave_match)
             {
                    option->wave_names[i]=Slave_name;
                    if(wave_files[i].OpenFile(Slave_name.c_str())== pcmpos_error ) wave_error=true;
                    else file_ch[i]->Caption=ExtractFileName(FileListBox[i]->FileName);

             }
             else {IsBusy=false; return(0);} // try again later...
           }
       }


         // one should also take into account the posiblity of different file name due to minor time gaps
         // or due to some difference in names... To account for this, an alternative mechanism should be set
         // so that if after a few round, no match was found, the file age can be used for detecting the appropriate
         // match. The file age difference should have a threshold of difference in tolerance.
         // it could be that the file age is the best way to go anyway -- look int this



       // do the same for all the slave channels...

       // Calc should be modified to allow transfer of and array of file names and num_slaves.
       // Calc will create WaveIterator arrays and then call the generic featureCalc function.

       if(!Calc(num_chan))
       {
        IsBusy=false;
        return(false);
       }  // compute all features and decide_file fate:

       saved_file0->Caption=m_Calculator.file_names[0];
       for(int i=1;i<=num_slaves->Value;i++)
       {
        if(slave_active[i]->Checked)saved_file[i]->Caption=m_Calculator.file_names[i];
       }

       max_syll_dur1->Value=m_Calculator.maxSyllableDur;
       max_bout_dur1->Value=m_Calculator.maxBoutDur;
       syllable_number1->Value=m_Calculator.syllable_number;

       // if(passed) // file contain data and must be saved in an appropriate location
    //   {          // saving the new file is handled by  CZT_FeatureCalc::save_to_database()
        // file_ch0->Caption=m_Calculator.newFile;
         // same should be done to the other files -- require changes in featurecalc
         int iFileHandle = FileOpen(file_ch0->Caption, fmOpenRead);
         int size = FileSeek(iFileHandle,0,2)/1000; // size of file in Kb
         FileClose(iFileHandle);
         folder_size1->Value+=(float)size/1000;
        // DataForm->Change_Database("SAP");
         DataForm->Settings(option->select_table+1, "sound_folder_size", folder_size1->Value);
         float x1;
         for(int ch=1;ch<=num_chan;ch++)
         {
           chanGraph[ch]->ClearGraf();
           for(int i=0;i<1000;i++)
           {
              x1=m_Calculator.amp_chans[ch-1][i];
              chanGraph[ch]->MarkAt(i,x1,1);   //i was
           }
           chanGraph[ch]->ShowGraf();
         }

         for(int i=0;i<10;i++)for(int j=0;j<1000;j++) m_Calculator.amp_chans[i][j]=0;

         m_Calculator.GetImage().SetBrightness(float(Brightness1->Position/39.0625));
         m_Calculator.GetImage().DrawAllSlices(Image1);
         Sonogram1->Assign(Image1->Picture->Bitmap);
         ScrollBox1->HorzScrollBar->Position=(ScrollBox1->HorzScrollBar->Range
                                                  *m_Calculator.bout_end / max(m_Calculator.total_frames,1))
                                                  -m_Calculator.maxBoutDur;

    IsBusy=false;
    return(true);
}


 ////////////////////////////////////////////////////////////////////////////////////////////



void __fastcall Tsal::ProcTreshTBChange(TObject *Sender)
{

  Timer1->Interval=ProcTreshTB->Position*10;
  ProcTreshN->Value=ProcTreshTB->Position*10;
}
//---------------------------------------------------------------------------

void __fastcall Tsal::DerivThresh1Change(TObject *Sender)
{
  int bird;
  TTrackBar *DerivThresh; TImage *image; Graphics::TBitmap *Sonogram; TNumLab *deriv_ind;
  if(Sender==DerivThresh1) {bird=2; image=Image1; DerivThresh=DerivThresh1; Sonogram=Sonogram1; deriv_ind=deriv_ind1; }
  EntropyB->Down=true;  DerivPowerB->Down=true;
  int pos=DerivThresh->Position;
  //option->amplitude_thresh=125-DerivThresh->Position; //m_Calculator.GetImage().DrawAllSlices(image);
  deriv_ind->Value=125-DerivThresh->Position;
  image->Picture->Bitmap=Sonogram;
  image->Canvas->Pen->Color=clYellow;
  image->Canvas->MoveTo(0,pos);
  image->Canvas->LineTo(image->Width,pos);
  DataForm->Settings(bird, "deriv_thresh", DerivThresh->Position);
}
//---------------------------------------------------------------------------


void __fastcall Tsal::save1Click(TObject *Sender)
{
  TRadioGroup *save;
  TEdit *sound_input_folder, *soundDrive, *ID;
  TStaticText *raw_features;
  bool *active;

  if(Sender==save1)
        {save=save1; soundDrive=chan0_out; raw_features=ms_table1;
        sound_input_folder=chan0_in;  active=&active1; ID=ID_1;}
 

  if(save->ItemIndex==2) MessageDlg("Warning: No data will be saved!", mtWarning, TMsgDlgButtons() << mbOK, 0);
  else if(save->ItemIndex==3) // pause state
  {
        *active=false;
        sound_input_folder->Color=clWhite; sound_input_folder->ReadOnly=false; sound_input_folder->ShowHint=false;
        soundDrive->Color=clWhite; soundDrive->ReadOnly=false; soundDrive->ShowHint=false;
        //raw_features->Color=clWhite; raw_features->ReadOnly=false; raw_features->ShowHint=false;
        ID->Color=clWhite; ID->ReadOnly=false; ID->ShowHint=false;

  }
  else
  {
    *active=true;
    sound_input_folder->Color=clBtnFace; sound_input_folder->ReadOnly=true; sound_input_folder->ShowHint=true;
    soundDrive->Color=clBtnFace; soundDrive->ReadOnly=true; soundDrive->ShowHint=true;
  //  raw_features->Color=clBtnFace; raw_features->ReadOnly=true;  raw_features->ShowHint=true;
    ID->Color=clBtnFace; ID->ReadOnly=true; ID->ShowHint=true;

  }
}
//---------------------------------------------------------------------------


void __fastcall Tsal::PitchBClick(TObject *Sender)
{
   m_Calculator.GetImage().GetPitch().bRender = PitchB->Down;
}
//---------------------------------------------------------------------------

void __fastcall Tsal::EntropyBClick(TObject *Sender)
{
 m_Calculator.GetImage().GetEntropy().bRender = EntropyB->Down;
}
//---------------------------------------------------------------------------

void __fastcall Tsal::FMBClick(TObject *Sender)
{
 m_Calculator.GetImage().GetFM().bRender = FMB->Down;
}
//---------------------------------------------------------------------------

void __fastcall Tsal::PitchGoodnessBClick(TObject *Sender)
{
m_Calculator.GetImage().GetPitchGoodness().bRender = PitchGoodnessB->Down;
}
//---------------------------------------------------------------------------

void __fastcall Tsal::DerivPowerBClick(TObject *Sender)
{
m_Calculator.GetImage().GetDerivPow().bRender = DerivPowerB->Down;
}
//---------------------------------------------------------------------------

void __fastcall Tsal::PeakFrBClick(TObject *Sender)
{
m_Calculator.GetImage().GetPeakFreq().bRender = PeakFrB->Down;
}
//---------------------------------------------------------------------------






 
void __fastcall Tsal::input0Click(TObject *Sender)
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
    else if(Sender==input8) { fileList=FileListBox8; input_folder=chan8_in;}
    else if(Sender==input9) { fileList=FileListBox9; input_folder=chan9_in;}
    else if(Sender==input10) { fileList=FileListBox10; input_folder=chan10_in;}
    else return;

   OpenDialog1->FileName="not relevant";
   if(OpenDialog1->Execute())
   {
      fileList->Directory=ExtractFileDir(OpenDialog1->FileName);
      input_folder->Text=fileList->Directory+"\\";
   }   /* */
}


void __fastcall Tsal::startClick(TObject *Sender)
{
  startup_state->Caption="starting...";
  TBorderIcons tempBI = BorderIcons;
  tempBI >> biSystemMenu;
  BorderIcons = tempBI;
  rec1->Caption="monitoring...";
 // rec1->Visible=false;
  force_file_lists(); // make sure that input and output channels directories are okay
  ID_1->Text=DataForm->Retrieve(2, "bird_ID");
 
  Slaves=count_slaves();

  // get the output folders into option -> sot that 
  DataForm->Records1->Active=true;
  option->syllable_table_name=DataForm->Records1->TableName;
  recnum1->Value=DataForm->count_records(option->syllable_table_name);

   try{
       Age1=DataForm->find_age(recorder_chan->Value+1);
       str="raw_" + ID_1->Text +"_" + Age1;
       //str=RecordFile1->Caption+"_" + Age1;
       DataForm->new_raw_table(str, Slaves);//new_raw_brain_table(str);
       slaves_in_table->Value=DataForm->chans_exist(str, 1);
       ms_table1->Caption=str;
       option->features_table=str;
       Age1+="\\";
    }
    catch(...)
    {
	  Application->MessageBox(L"failed to create raw_feature tables", NULL, MB_OK);
    }

  if(slaves_in_table->Value<num_slaves->Value)
  {
      if(MessageDlg("SAP2 will need to delete the syllable table and recreate it with appropriate number of channels, okay?", mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
      {
          DataForm->DeleteTable(option->features_table);
          DataForm->new_raw_table(option->features_table, Slaves);
      }
      else
      {
         stopClick(this);
         return;
      }

  }
  IDL1->Caption=ID_1->Text;
  // Timer1->Enabled=true;
  option->halt=false;
  option->move_sound=true;
  option->delete_trash_file=true;
  m_Calculator.m_Image.m_nMaxSlices=10000;
  set_output_folders();
  edit(on);
  Timer1->Enabled=true;
  start->Enabled=false;
  stop->Enabled=true; 
  switch(save1->ItemIndex)
  {
        case 0: str=" saving raw features && syllable table"; break;
        case 1: str=" saving syllable table only"; break;
        case 2: str=" saving raw features only"; break;
        case 3: str=" save nothing but sound files"; break;
        case 4: str=" paused"; break;
  }
  startup_state->Caption="Monitoring channels"+str;
}


//---------------------------------------------------------------------------


void __fastcall Tsal::output0Click(TObject *Sender)
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
    else if(Sender==output8) { output_folder=chan8_out;}
    else if(Sender==output9) { output_folder=chan9_out;}
    else if(Sender==output10){ output_folder=chan10_out;}
    else return;

   OpenDialog1->FileName="not relevant";
   if(OpenDialog1->Execute()) output_folder->Text=ExtractFileDir(OpenDialog1->FileName)+"\\";
}
//---------------------------------------------------------------------------


void __fastcall Tsal::features_output1Click(TObject *Sender)
{
    TStaticText *raw_features;
    if(Sender==features_output1)raw_features=ms_table1;

     raw_features->Caption = InputBox("New Table", "Table name must start with a letter, e.g., Bird_109:", "");
     raw_features->Caption=DataForm->new_raw_table(raw_features->Caption, count_slaves());

}
//---------------------------------------------------------------------------

void __fastcall Tsal::table1Click(TObject *Sender)
{

   TStaticText *RecordFile;
   TNumLab *Rec_num;
   TNumLab *folderSize;
   int bird=recorder_chan->Value+1;
   folderSize=folder_size1;
   DataForm->TheTable=DataForm->Records1;
   RecordFile=syllable_table1;
   Rec_num=recnum1;
   DataForm->openTableClick(this);
   //CheckTableType();
   //DataForm->openBrainTableClick(this);
   folderSize->Value=folder_size_limit->Position+1;// this will start a new folder!
   RecordFile->Caption=DataForm->TheTable->TableName;
   Rec_num->Value=DataForm->recNum;
   DataForm->Settings(bird, "syllable_table", DataForm->TheTable->TableName);
   str=DataForm->TheTable->TableName;
   int age=DataForm->find_age(bird);
   str+="_"; str+=age;
   DataForm->new_raw_table(str,count_slaves());//new_raw_brain_table(str);
   slaves_in_table->Value=DataForm->chans_exist(str, 1);
   option->features_table=str;
   option->syllable_table_name=DataForm->Records1->TableName;
   ms_table1->Caption=str;

   str="file_table_"; str+=DataForm->TheTable->TableName;
   file_table->Caption=str;
   DataForm->file_table_name=file_table->Caption;
   DataForm->Create_File_Table();
}
//---------------------------------------------------------------------------

void __fastcall Tsal::stopClick(TObject *Sender)
{
  TBorderIcons tempBI = BorderIcons;
  tempBI << biSystemMenu;
  BorderIcons = tempBI;
  Timer1->Enabled=false;
  rec1->Caption="engine off";
  //rec1->Color=clBtnFace;  rec1->Font->Color=clRed;
  //rec1->Font->Size=60;  rec1->Caption="engine off";
  start->Enabled=true;
  stop->Enabled=false;
  edit(off);
  startup_state->Caption="Stoped";
}
//---------------------------------------------------------------------------

void __fastcall Tsal::BitBtn2Click(TObject *Sender)
{
  option->Visible=true;
  option->BringToFront();
  option->show_start=false;        
}
//---------------------------------------------------------------------------


void __fastcall Tsal::folder_size_limitChange(TObject *Sender)
{
        folder_sizeN->Value=folder_size_limit->Position;
}
//---------------------------------------------------------------------------


 void Tsal::edit(bool state)
{
        TColor color;
        if(state) color=clSilver;
        else color=clWhite;

        //chan0_in->Color=color;
        chan0_in->ReadOnly=state; chan0_in->ShowHint=state;
        //chan0_out->Color=color;
        chan0_out->ReadOnly=state; chan0_out->ShowHint=state;
      //  raw_features1->Color=color; raw_features1->ReadOnly=state; raw_features1->ShowHint=state;
        ID_1->Color=color; ID_1->ReadOnly=state; ID_1->ShowHint=state;
        slave1_active->Enabled=(!state); slave2_active->Enabled=(!state); slave3_active->Enabled=(!state);
        slave4_active->Enabled=(!state); slave5_active->Enabled=(!state); slave6_active->Enabled=(!state);
        slave7_active->Enabled=(!state); slave8_active->Enabled=(!state); slave9_active->Enabled=(!state);
        slave10_active->Enabled=(!state);
}


void __fastcall Tsal::ID_1Change(TObject *Sender)
{
  int bird; TEdit *id;
  bird=2;
  id=ID_1;
  //else Application->MessageBox("bug 003", NULL, MB_OK);
  DataForm->Settings(bird, "bird_ID", id->Text);
}
//---------------------------------------------------------------------------

void __fastcall Tsal::chan0_inChange(TObject *Sender)
{
 update_file_lists();
 /*
  int bird;
  TEdit *input;
  TFileListBox* file_list;
  if(Sender==chan0_in) {bird=2; file_list=FileListBox0; input=chan0_in;}
  else if(Sender==chan1_in) {bird=3; file_list=FileListBox1; input=chan1_in;}
  else if(Sender==chan2_in) {bird=4; file_list=FileListBox2; input=chan2_in;}
  else if(Sender==chan3_in) {bird=5; file_list=FileListBox3; input=chan3_in;}
  else if(Sender==chan4_in) {bird=6; file_list=FileListBox4; input=chan4_in;}
  else if(Sender==chan5_in) {bird=7; file_list=FileListBox5; input=chan5_in;}
  else if(Sender==chan6_in) {bird=8; file_list=FileListBox6; input=chan6_in;}
  else if(Sender==chan7_in) {bird=9; file_list=FileListBox7; input=chan7_in;}
  else if(Sender==chan8_in) {bird=10; file_list=FileListBox8; input=chan8_in;}
  else if(Sender==chan9_in) {bird=11; file_list=FileListBox9; input=chan9_in;}
  else Application->MessageBox("bug 004", NULL, MB_OK);
  file_list->Directory=input->Text;
  DataForm->Settings(bird, "input_folder", input->Text); // save the input folder change to the database
          */
}
//---------------------------------------------------------------------------


void __fastcall Tsal::chan0_outChange(TObject *Sender)
{

  int bird; TEdit *out;
  //if(Sender==chan0_out)
  { bird=2; out=chan0_out; DataForm->Settings(bird, "output_folder_sound", out->Text);}
 // else Application->MessageBox("bug 005", NULL, MB_OK);
  

}
//---------------------------------------------------------------------------

void __fastcall Tsal::raw_features1Change(TObject *Sender)
{
  int bird;
  TStaticText *out;
  if(Sender==ms_table1)      { bird=2; out=ms_table1; }
  DataForm->Settings(bird, "output_folder_features", out->Caption);
}
//---------------------------------------------------------------------------

void Tsal::retrieveFeatures()
{
  try{
   ID_1->Text=DataForm->Retrieve(2,"bird_ID");
   DerivThresh1->Position=DataForm->Retrieve(2, "deriv_thresh").ToInt();
   entropyThresh1->Position=DataForm->Retrieve(2, "entropy_thresh").ToInt();

   chan0_in->Text=DataForm->Retrieve(2,"input_folder");
   chan0_out->Text=DataForm->Retrieve(2,"output_folder_sound");
   ms_table1->Caption=DataForm->Retrieve(2,"output_folder_features");

   chan1_in->Text=DataForm->Retrieve(3,"input_folder");
   chan1_out->Text=DataForm->Retrieve(3,"output_folder_sound");
   slave1_active->Checked=DataForm->Retrieve(3, "active_or_slave").ToInt();

   chan2_in->Text=DataForm->Retrieve(4,"input_folder");
   chan2_out->Text=DataForm->Retrieve(4,"output_folder_sound");
   slave2_active->Checked=DataForm->Retrieve(4, "active_or_slave").ToInt();

   chan3_in->Text=DataForm->Retrieve(5,"input_folder");
   chan3_out->Text=DataForm->Retrieve(5,"output_folder_sound");
   slave3_active->Checked=DataForm->Retrieve(5, "active_or_slave").ToInt();

   chan4_in->Text=DataForm->Retrieve(6,"input_folder");
   chan4_out->Text=DataForm->Retrieve(6,"output_folder_sound");
   slave4_active->Checked=DataForm->Retrieve(6, "active_or_slave").ToInt();

   chan5_in->Text=DataForm->Retrieve(7,"input_folder");
   chan5_out->Text=DataForm->Retrieve(7,"output_folder_sound");
   slave5_active->Checked=DataForm->Retrieve(7, "active_or_slave").ToInt();

   chan6_in->Text=DataForm->Retrieve(8,"input_folder");
   chan6_out->Text=DataForm->Retrieve(8,"output_folder_sound");
   slave6_active->Checked=DataForm->Retrieve(8, "active_or_slave").ToInt();

   chan7_in->Text=DataForm->Retrieve(9,"input_folder");
   chan7_out->Text=DataForm->Retrieve(9,"output_folder_sound");
   slave7_active->Checked=DataForm->Retrieve(9, "active_or_slave").ToInt();

   chan8_in->Text=DataForm->Retrieve(10,"input_folder");
   chan8_out->Text=DataForm->Retrieve(10,"output_folder_sound");
   slave8_active->Checked=DataForm->Retrieve(10, "active_or_slave").ToInt();

   chan9_in->Text=DataForm->Retrieve(11,"input_folder");
   chan9_out->Text=DataForm->Retrieve(11,"output_folder_sound");
   slave9_active->Checked=DataForm->Retrieve(11, "active_or_slave").ToInt();

   chan10_in->Text=DataForm->Retrieve(12,"input_folder");
   chan10_out->Text=DataForm->Retrieve(12,"output_folder_sound");
   slave10_active->Checked=DataForm->Retrieve(12, "active_or_slave").ToInt();



   folder_size1->Value=DataForm->Retrieve(2, "sound_folder_size").ToInt();
   syllable_table1->Caption=DataForm->Retrieve(2,"syllable_table");
   DataForm->Create_table(syllable_table1->Caption);
   Age1=DataForm->find_age(recorder_chan->Value+1);
   bird_age->Text=Age1;
   ms_table1->Caption="raw_"+ID_1->Text +"_"+ Age1;
   DataForm->new_raw_table(ms_table1->Caption, num_slaves->Value);
   str="file_table_"; str+=ID_1->Text;//syllable_table[i]->Caption;
   DataForm->file_table_name=str;
   file_table->Caption=str;
   DataForm->Create_File_Table();
//   file_table_recs[i]->Value=DataForm->update_file_table_index(i);
//   save[i]->ItemIndex=DataForm->Retrieve(i+2, "save").ToInt();



  }
  catch(...)
  {
	Application->MessageBox(L"could not open the settings table, default settings will be applied", NULL, MB_OK);

  }
}     


void __fastcall Tsal::new_table1Click(TObject *Sender)
{
   TStaticText *RecordFile;
   TNumLab *Rec_num;
   TNumLab *folderSize;
   AnsiString database;
   int bird, age;
   TDate dt;

   bird=recorder_chan->Value+1; // this is the first bird of this moducle, bird 1 is reserved for other modules.
   folderSize=folder_size1;
   DataForm->TheTable=DataForm->Records1;
   RecordFile=syllable_table1;
   Rec_num=recnum1;
   //database="chan1";
   age=DataForm->find_age(bird);
   DataForm->new_brain_tableClick(this);
   folderSize->Value=folder_size_limit->Position+1;// this will start a new folder!
   RecordFile->Caption=DataForm->TheTable->TableName;
   Rec_num->Value=DataForm->recNum;
   DataForm->Settings(bird, "syllable_table", DataForm->TheTable->TableName);
   str=DataForm->TheTable->TableName;
   str+="_"; str+=age;
   DataForm->new_raw_table(str, count_slaves());//new_raw_brain_table(str);
   slaves_in_table->Value=DataForm->chans_exist(str, 1);
   option->syllable_table_name=DataForm->Records1->TableName;
   option->features_table=str;
   ms_table1->Caption=str;

   str="file_table_"; str+=DataForm->TheTable->TableName;
   file_table->Caption=str;
   DataForm->file_table_name=file_table->Caption;
   DataForm->Create_File_Table();

}
//---------------------------------------------------------------------------



void __fastcall Tsal::SyllMinDurTbChange(TObject *Sender)
{
     option->syllable_duration_thresh=1.4*SyllMinDurTb->Position;
     SyllDurLow->Value=option->syllable_duration_thresh;
}
//---------------------------------------------------------------------------

void __fastcall Tsal::BoutMinDurTBChange(TObject *Sender)
{
   option->bout_duration_thresh=1.4*BoutMinDurTB->Position;
   BoutDurLow->Value=option->bout_duration_thresh;
}
//---------------------------------------------------------------------------

void __fastcall Tsal::min_syllablesChange(TObject *Sender)
{
  option->minimum_syllables_per_files=min_syllables->Position;
  mean_syllable_num->Value=min_syllables->Position;
}
//---------------------------------------------------------------------------


void __fastcall Tsal::AMClick(TObject *Sender)
{
m_Calculator.GetImage().GetAM().bRender = AM->Down;
}
//---------------------------------------------------------------------------

void __fastcall Tsal::BitBtn1Click(TObject *Sender)
{
   StartForm->Visible=true;
   StartForm->BringToFront();          
}
//---------------------------------------------------------------------------


void __fastcall Tsal::slow_executionChange(TObject *Sender)
{
  option->slow_execution=slow_execution->Position;
}
//---------------------------------------------------------------------------

void __fastcall Tsal::help5Click(TObject *Sender)
{
  str=" Depending on your system, demands imposed by the Sound Processing Live module \r";
  str+=" might interfere with the SA Recorder module. \r";
  str+="In particular, the playbacks regimen might crush after the processor is  \r";
  str+=" 100% busy with analysis for too long. \r";
  str+="To overcome this problem, you should slow down the execution of the live analysis \r";
  str+="This is done by halting execution for very short durations during the spectral analysis. \r";
  str+="You want to decrease processor load to about 60-70%. To do so, start the Windows  \r";
  str+="Task Manager (control-alt-del) and click performance. Then Start the Live analysis while.";
  str+="the SA Recorder is running, and watch the CPU usage during the analysis (as sonograms appears ";
  str+="on the screen. Then increase the 'Slow-down execution' until you see that the peak CPU";
  str+=" usage is down to 70% or less";

  MessageDlg(str, mtInformation, TMsgDlgButtons() << mbOK, 0);
}


//---------------------------------------------------------------------------



void __fastcall Tsal::folder_choiceClick(TObject *Sender)
{
  if(folder_choice->ItemIndex)
  {sizeLb->Visible=false; sizeLb2->Visible=false; folder_size_limit->Visible=false;
   folder_sizeN->Visible=false; sizeH2->Visible=true;}
  else
   {sizeLb->Visible=true; sizeLb2->Visible=true; folder_size_limit->Visible=true;
   folder_sizeN->Visible=true; sizeH2->Visible=false;}
}
//---------------------------------------------------------------------------



void __fastcall Tsal::slave1_activeClick(TObject *Sender)
{
  TCheckBox *slave_active;
  TStaticText *ch_Lb;

  if(Sender==slave1_active)      {slave_active=slave1_active;   ch_Lb=ch1_Lb; option->slave_mode[0]=slave_state1->ItemIndex+1;}
  else if(Sender==slave2_active) {slave_active=slave2_active;   ch_Lb=ch2_Lb; option->slave_mode[1]=slave_state2->ItemIndex+1;}
  else if(Sender==slave3_active) {slave_active=slave3_active;   ch_Lb=ch3_Lb; option->slave_mode[2]=slave_state3->ItemIndex+1;}
  else if(Sender==slave4_active) {slave_active=slave4_active;   ch_Lb=ch4_Lb; option->slave_mode[3]=slave_state4->ItemIndex+1;}
  else if(Sender==slave5_active) {slave_active=slave5_active;   ch_Lb=ch5_Lb; option->slave_mode[4]=slave_state5->ItemIndex+1;}
  else if(Sender==slave6_active) {slave_active=slave6_active;   ch_Lb=ch6_Lb; option->slave_mode[5]=slave_state6->ItemIndex+1;}
  else if(Sender==slave7_active) {slave_active=slave7_active;   ch_Lb=ch7_Lb; option->slave_mode[6]=slave_state7->ItemIndex+1;}
  else if(Sender==slave8_active) {slave_active=slave8_active;   ch_Lb=ch8_Lb; option->slave_mode[7]=slave_state8->ItemIndex+1;}
  else if(Sender==slave9_active) {slave_active=slave9_active;   ch_Lb=ch9_Lb; option->slave_mode[8]=slave_state9->ItemIndex+1;}
  else if(Sender==slave10_active){slave_active=slave10_active;  ch_Lb=ch10_Lb;option->slave_mode[9]=slave_state10->ItemIndex+1;}

  if(!slave_active->Checked)ch_Lb->Visible=true;
  else ch_Lb->Visible=false;
  count_slaves();
}
//---------------------------------------------------------------------------


void __fastcall Tsal::gain1Change(TObject *Sender)
{
  TRChart *chart;
  TTrackBar *gain;
  if(Sender==gain1){chart=RChart1; gain=gain1;}
  else if(Sender==gain2){chart=RChart2;gain=gain2;}
  else if(Sender==gain3){chart=RChart3;gain=gain3;}
  else if(Sender==gain4){chart=RChart4;gain=gain4;}
  else if(Sender==gain5){chart=RChart5;gain=gain5;}
  else if(Sender==gain6){chart=RChart6;gain=gain6;}
  else if(Sender==gain7){chart=RChart7;gain=gain7;}
  else if(Sender==gain8){chart=RChart8;gain=gain8;}
  else if(Sender==gain9){chart=RChart9;gain=gain9;}
  else if(Sender==gain10){chart=RChart10;gain=gain10;}

  chart->Scale1Y->RangeHigh=200*gain->Position;

}
//---------------------------------------------------------------------------


void __fastcall Tsal::warning1Click(TObject *Sender)
{
  str="Note: each slave channel will create a field in the tables. If you create a table with, say 4 slaves, and then you record from 5 slaves";
  str+="an error will occur while SA+ records data to the tables. Having extra channes is fine, but will consume additional storage space.";
  str+="If you created a table with a certain number of channels, and then decides to add channels you have two choised: either manually add fields";
  str+="to the table (in mySQL), or simply create a new table. You will then have to deal with different tables types collected for the same animal.";
   MessageDlgPos(str, mtWarning, TMsgDlgButtons() << mbOK, 0, 10,250);

}
//---------------------------------------------------------------------------


int Tsal::count_slaves()
{
   int cnt=0;
   if(slave1_active->Checked)cnt++;
   if(slave2_active->Checked)cnt++;
   if(slave3_active->Checked)cnt++;
   if(slave4_active->Checked)cnt++;
   if(slave5_active->Checked)cnt++;
   if(slave6_active->Checked)cnt++;
   if(slave7_active->Checked)cnt++;
   if(slave8_active->Checked)cnt++;
   if(slave9_active->Checked)cnt++;
   if(slave10_active->Checked)cnt++;
   num_slaves->Value=cnt;
   return(cnt);

}



void __fastcall Tsal::slave_state1Click(TObject *Sender)
{
         if(Sender==slave_state1) { option->slave_mode[0]=slave_state1->ItemIndex+1; }
    else if(Sender==slave_state2) { option->slave_mode[1]=slave_state2->ItemIndex+1; }
    else if(Sender==slave_state3) { option->slave_mode[2]=slave_state3->ItemIndex+1; }
    else if(Sender==slave_state4) { option->slave_mode[3]=slave_state4->ItemIndex+1; }
    else if(Sender==slave_state5) { option->slave_mode[4]=slave_state5->ItemIndex+1; }
    else if(Sender==slave_state6) { option->slave_mode[5]=slave_state6->ItemIndex+1; }
    else if(Sender==slave_state7) { option->slave_mode[6]=slave_state7->ItemIndex+1; }
    else if(Sender==slave_state8) { option->slave_mode[7]=slave_state8->ItemIndex+1; }
    else if(Sender==slave_state9) { option->slave_mode[8]=slave_state9->ItemIndex+1; }
    else if(Sender==slave_state10){ option->slave_mode[9]=slave_state10->ItemIndex+1;}


}
//---------------------------------------------------------------------------


//chans_exist(AnsiString table_name, int num_slaves);


void __fastcall Tsal::start_timerTimer(TObject *Sender)
{
//
   start_timer->Enabled=false;
   startup_state->Caption="retrieving settings from tables";
   DataForm->MyConnection1->Database="SAP";
  // DataForm->settings->TableName="settings";
   DataForm->settings->Active=true;
   retrieveFeatures();
   // test the folders exsists, or create them!
   startup_state->Caption="openning input folders";
   if (!DirectoryExists("c:\\SAP\\tmpSound\\"))if(!ForceDirectories("c:\\SAP\\tmpSound\\"))
		Application->MessageBox(L"cannot create folder c:\\tmpSound. This is the default folder of SA recorder for storage of sounds. Go to the SA recorder and make sure that each chanel has a set, and accessable temporary folder, otherwise, you might experience no recording input", NULL, MB_OK);


   if (!DirectoryExists(chan0_in->Text)) if(!ForceDirectories(chan0_in->Text))
   {
     chan0_in->Text="c:\\sap\\in1\\";
	 if(!ForceDirectories(chan0_in->Text)) Application->MessageBox(L"SA+ failed to access or create input folder c:\\sap\\in0. Please type an alterative folder name for the input of chan0", NULL, MB_OK);
	 else Application->MessageBox(L"The input folder for chan0 is not accessible, SA+ has succeeded to revert to the defauld channel c:\\sap\\in0", NULL, MB_OK);
   }
   if(!DirectoryExists(chan1_in->Text)) if(!ForceDirectories(chan1_in->Text))
   {
     chan1_in->Text="c:\\sap\\in2\\";
	 if(!ForceDirectories(chan1_in->Text)) Application->MessageBox(L"SA+ failed to access or create input folder c:\\sap\\in1. Please type an alterative folder name for the input of chan1", NULL, MB_OK);
	 else Application->MessageBox(L"The input folder for chan1 is not accessible, SA+ has succeeded to revert to the defauld channel c:\\sap\\in0", NULL, MB_OK);
   }
   if (!DirectoryExists(chan2_in->Text)) if(!ForceDirectories(chan2_in->Text))
   {
     chan2_in->Text="c:\\sap\\in3\\";
	 if(!ForceDirectories(chan2_in->Text)) Application->MessageBox(L"SA+ failed to access or create input folder c:\\sap\\in2. Please type an alterative folder name for the input of chan2", NULL, MB_OK);
	 else Application->MessageBox(L"The input folder for chan2 is not accessible, SA+ has succeeded to revert to the defauld channel c:\\sap\\in2", NULL, MB_OK);
   }
   if (!DirectoryExists(chan3_in->Text)) if(!ForceDirectories(chan3_in->Text))
   {
     chan3_in->Text="c:\\sap\\in4\\";
	 if(!ForceDirectories(chan3_in->Text)) Application->MessageBox(L"SA+ failed to access or create input folder c:\\sap\\in3. Please type an alterative folder name for the input of chan3", NULL, MB_OK);
	 else Application->MessageBox(L"The input folder for chan3 is not accessible, SA+ has succeeded to revert to the defauld channel c:\\sap\\in3", NULL, MB_OK);
   }
   if (!DirectoryExists(chan4_in->Text)) if(!ForceDirectories(chan4_in->Text))
   {
     chan4_in->Text="c:\\sap\\in5\\";
	 if(!ForceDirectories(chan4_in->Text)) Application->MessageBox(L"SA+ failed to access or create input folder c:\\sap\\in4. Please type an alterative folder name for the input of chan4", NULL, MB_OK);
	 else Application->MessageBox(L"The input folder for chan4 is not accessible, SA+ has succeeded to revert to the defauld channel c:\\sap\\in4", NULL, MB_OK);
   }
   if (!DirectoryExists(chan5_in->Text)) if(!ForceDirectories(chan5_in->Text))
   {
     chan5_in->Text="c:\\sap\\in6\\";
	 if(!ForceDirectories(chan5_in->Text)) Application->MessageBox(L"SA+ failed to access or create input folder c:\\sap\\in5. Please type an alterative folder name for the input of chan5", NULL, MB_OK);
	 else Application->MessageBox(L"The input folder for chan5 is not accessible, SA+ has succeeded to revert to the defauld channel c:\\sap\\in5", NULL, MB_OK);
   }
   if (!DirectoryExists(chan6_in->Text)) if(!ForceDirectories(chan6_in->Text))
   {
     chan6_in->Text="c:\\sap\\in7\\";
	 if(!ForceDirectories(chan6_in->Text)) Application->MessageBox(L"SA+ failed to access or create input folder c:\\sap\\in6. Please type an alterative folder name for the input of chan6", NULL, MB_OK);
	 else Application->MessageBox(L"The input folder for chan6 is not accessible, SA+ has succeeded to revert to the defauld channel c:\\sap\\in6", NULL, MB_OK);
   }
   if (!DirectoryExists(chan7_in->Text)) if(!ForceDirectories(chan7_in->Text))
   {
     chan7_in->Text="c:\\sap\\in8\\";
     if(!ForceDirectories(chan7_in->Text)) Application->MessageBox(L"SA+ failed to access or create input folder c:\\sap\\in7. Please type an alterative folder name for the input of chan7", NULL, MB_OK);
	 else Application->MessageBox(L"The input folder for chan7 is not accessible, SA+ has succeeded to revert to the defauld channel c:\\sap\\in7", NULL, MB_OK);
   }
   if (!DirectoryExists(chan8_in->Text)) if(!ForceDirectories(chan8_in->Text))
   {
     chan8_in->Text="c:\\sap\\in9\\";
	 if(!ForceDirectories(chan8_in->Text)) Application->MessageBox(L"SA+ failed to access or create input folder c:\\sap\\in8. Please type an alterative folder name for the input of chan8", NULL, MB_OK);
	 else Application->MessageBox(L"The input folder for chan8 is not accessible, SA+ has succeeded to revert to the defauld channel c:\\sap\\in8", NULL, MB_OK);
   }
   if (!DirectoryExists(chan9_in->Text)) if(!ForceDirectories(chan9_in->Text))
   {
     chan9_in->Text="c:\\sap\\in10\\";
	 if(!ForceDirectories(chan9_in->Text)) Application->MessageBox(L"SA+ failed to access or create input folder c:\\sap\\in9. Please type an alterative folder name for the input of chan9", NULL, MB_OK);
	 else Application->MessageBox(L"The input folder for chan9 is not accessible, SA+ has succeeded to revert to the defauld channel c:\\sap\\in9", NULL, MB_OK);
   }
   if (!DirectoryExists(chan10_in->Text)) if(!ForceDirectories(chan10_in->Text))
   {
     chan10_in->Text="c:\\sap\\in11\\";
	 if(!ForceDirectories(chan10_in->Text)) Application->MessageBox(L"SA+ failed to access or create input folder c:\\sap\\in10. Please type an alterative folder name for the input of chan10", NULL, MB_OK);
	 else Application->MessageBox(L"The input folder for chan10 is not accessible, SA+ has succeeded to revert to the defauld channel c:\\sap\\in10", NULL, MB_OK);
   }

    startup_state->Caption="updating pending wave file lists";
   // now set the input sound folders
   update_file_lists();

    startup_state->Caption="openning output folders";
   if (!DirectoryExists(chan0_out->Text)) if(!ForceDirectories(chan0_out->Text))
   {
     chan0_out->Text="c:\\sap\\out1\\";
	 if(!ForceDirectories(chan0_out->Text)) Application->MessageBox(L"SA+ failed to access or create input folder c:\\sap\\out0. Please type an alterative folder name for the input of chan0", NULL, MB_OK);
	 else Application->MessageBox(L"The input folder for chan0 is not accessible, SA+ has succeeded to revert to the defauld channel c:\\sap\\out0", NULL, MB_OK);
   }
   if (!DirectoryExists(chan1_out->Text)) if(!ForceDirectories(chan1_out->Text))
   {
     chan1_out->Text="c:\\sap\\out2\\";
	 if(!ForceDirectories(chan1_out->Text)) Application->MessageBox(L"SA+ failed to access or create input folder c:\\sap\\out1. Please type an alterative folder name for the input of chan1", NULL, MB_OK);
	 else Application->MessageBox(L"The input folder for chan1 is not accessible, SA+ has succeeded to revert to the defauld channel c:\\sap\\out0", NULL, MB_OK);
   }
   if (!DirectoryExists(chan2_out->Text)) if(!ForceDirectories(chan2_out->Text))
   {
     chan2_out->Text="c:\\sap\\out3\\";
	 if(!ForceDirectories(chan2_out->Text)) Application->MessageBox(L"SA+ failed to access or create input folder c:\\sap\\out2. Please type an alterative folder name for the input of chan2", NULL, MB_OK);
	 else Application->MessageBox(L"The input folder for chan2 is not accessible, SA+ has succeeded to revert to the defauld channel c:\\sap\\out2", NULL, MB_OK);
   }
   if (!DirectoryExists(chan3_out->Text)) if(!ForceDirectories(chan3_out->Text))
   {
     chan3_out->Text="c:\\sap\\out4\\";
	 if(!ForceDirectories(chan3_out->Text)) Application->MessageBox(L"SA+ failed to access or create input folder c:\\sap\\out3. Please type an alterative folder name for the input of chan3", NULL, MB_OK);
	 else Application->MessageBox(L"The input folder for chan3 is not accessible, SA+ has succeeded to revert to the defauld channel c:\\sap\\out3", NULL, MB_OK);
   }
   if (!DirectoryExists(chan4_out->Text)) if(!ForceDirectories(chan4_out->Text))
   {
     chan4_out->Text="c:\\sap\\out5\\";
	 if(!ForceDirectories(chan4_out->Text)) Application->MessageBox(L"SA+ failed to access or create input folder c:\\sap\\out4. Please type an alterative folder name for the input of chan4", NULL, MB_OK);
	 else Application->MessageBox(L"The input folder for chan4 is not accessible, SA+ has succeeded to revert to the defauld channel c:\\sap\\out4", NULL, MB_OK);
   }
   if (!DirectoryExists(chan5_out->Text)) if(!ForceDirectories(chan5_out->Text))
   {
     chan5_out->Text="c:\\sap\\out6\\";
	 if(!ForceDirectories(chan5_out->Text)) Application->MessageBox(L"SA+ failed to access or create input folder c:\\sap\\out5. Please type an alterative folder name for the input of chan5", NULL, MB_OK);
	 else Application->MessageBox(L"The input folder for chan5 is not accessible, SA+ has succeeded to revert to the defauld channel c:\\sap\\out5", NULL, MB_OK);
   }
   if (!DirectoryExists(chan6_out->Text)) if(!ForceDirectories(chan6_out->Text))
   {
     chan6_out->Text="c:\\sap\\out7\\";
	 if(!ForceDirectories(chan6_out->Text)) Application->MessageBox(L"SA+ failed to access or create input folder c:\\sap\\out6. Please type an alterative folder name for the input of chan6", NULL, MB_OK);
	 else Application->MessageBox(L"The input folder for chan6 is not accessible, SA+ has succeeded to revert to the defauld channel c:\\sap\\out6", NULL, MB_OK);
   }
   if (!DirectoryExists(chan7_out->Text)) if(!ForceDirectories(chan7_out->Text))
   {
     chan7_out->Text="c:\\sap\\out8\\";
	 if(!ForceDirectories(chan7_out->Text)) Application->MessageBox(L"SA+ failed to access or create input folder c:\\sap\\out7. Please type an alterative folder name for the input of chan7", NULL, MB_OK);
	 else Application->MessageBox(L"The input folder for chan7 is not accessible, SA+ has succeeded to revert to the defauld channel c:\\sap\\out7", NULL, MB_OK);
   }
   if (!DirectoryExists(chan8_out->Text)) if(!ForceDirectories(chan8_out->Text))
   {
     chan8_out->Text="c:\\sap\\out9\\";
	 if(!ForceDirectories(chan8_out->Text)) Application->MessageBox(L"SA+ failed to access or create input folder c:\\sap\\out8. Please type an alterative folder name for the input of chan8", NULL, MB_OK);
	 else Application->MessageBox(L"The input folder for chan8 is not accessible, SA+ has succeeded to revert to the defauld channel c:\\sap\\out8", NULL, MB_OK);
   }
   if (!DirectoryExists(chan9_out->Text)) if(!ForceDirectories(chan9_out->Text))
   {
     chan9_out->Text="c:\\sap\\out10\\";
	 if(!ForceDirectories(chan9_out->Text)) Application->MessageBox(L"SA+ failed to access or create input folder c:\\sap\\out9. Please type an alterative folder name for the input of chan9", NULL, MB_OK);
	 else Application->MessageBox(L"The input folder for chan9 is not accessible, SA+ has succeeded to revert to the defauld channel c:\\sap\\out9", NULL, MB_OK);
   }
   if (!DirectoryExists(chan10_out->Text)) if(!ForceDirectories(chan10_out->Text))
   {
     chan10_out->Text="c:\\sap\\out11\\";
     if(!ForceDirectories(chan10_out->Text)) Application->MessageBox(L"SA+ failed to access or create input folder c:\\sap\\out10. Please type an alterative folder name for the input of chan10", NULL, MB_OK);
     else Application->MessageBox(L"The input folder for chan10 is not accessible, SA+ has succeeded to revert to the defauld channel c:\\sap\\out10", NULL, MB_OK);
   }

  // DataForm->Change_Database("SAP"); // we will stay there all along in the 'brain mode..."
    startup_state->Caption="finalizing tables activation";

   //DataForm->settings->TableName="settings";
   //DataForm->settings->Active=true;
   DataForm->settings->First(); DataForm->settings->Next();  // this brings us to the second raw of the settings table
   // where the first bird of the 'live' module data are stored. The firt bird in settings, belong to all other modules.
   DataForm->Records1->Active=false;
   try{
   DataForm->Records1->TableName = DataForm->settings->FieldValues["syllable_table"];
   folder_size1->Value=DataForm->settings->FieldValues["sound_folder_size"];
   DataForm->Records1->Active=true;
   DataForm->TheTable=DataForm->Records1;
   }
   catch(...)
   {
     DataForm->Create_table("recs_n1");
     DataForm->Records1->TableName = "recs_n1";
     DataForm->Settings(2, "syllable_table", "recs_n1");
   }

   slave_state1->ItemIndex=DataForm->Retrieve(3, "slave_output").ToInt();
   slave_state2->ItemIndex=DataForm->Retrieve(4, "slave_output").ToInt();
   slave_state3->ItemIndex=DataForm->Retrieve(5, "slave_output").ToInt();
   slave_state4->ItemIndex=DataForm->Retrieve(6, "slave_output").ToInt();
   slave_state5->ItemIndex=DataForm->Retrieve(7, "slave_output").ToInt();
   slave_state6->ItemIndex=DataForm->Retrieve(8, "slave_output").ToInt();
   slave_state7->ItemIndex=DataForm->Retrieve(9, "slave_output").ToInt();
   slave_state8->ItemIndex=DataForm->Retrieve(10, "slave_output").ToInt();
   slave_state9->ItemIndex=DataForm->Retrieve(11, "slave_output").ToInt();
   slave_state10->ItemIndex=DataForm->Retrieve(12, "slave_output").ToInt();

   syllable_table1->Caption=DataForm->Records1->TableName;
   startup_state->Caption="ready";
   start->Enabled=true;

}
//---------------------------------------------------------------------------



void __fastcall Tsal::chang_bird1Click(TObject *Sender)
{
  if(Sender==chang_bird1)  animals_form->datapages->ActivePageIndex=0;
  else animals_form->datapages->ActivePageIndex=1;
  if (animals_form->ShowModal() == mrOk)
  {
    animals_form->Animals_to_settings(2);
    retrieveFeatures();
  }
  if(animals_form->new_bird) animals_form->Insert_bird_record();
}
//---------------------------------------------------------------------------


void __fastcall Tsal::save_configClick(TObject *Sender)
{
   save_settings();
   Application->MessageBox(L"Configuration saved.", NULL, MB_OK);
}
//---------------------------------------------------------------------------

void __fastcall Tsal::recorder_chanChange(TObject *Sender)
{
       option->bird_age=DataForm->find_age(recorder_chan->Value+1);
       Age1=option->bird_age;
       bird_age->Text=Age1;
}


//---------------------------------------------------------------------------


void __fastcall Tsal::BitBtn37Click(TObject *Sender)
{
  Application->HelpContext(1430);
}



//---------------------------------------------------------------------------

