//---------------------------------------------------------------------------
#include <fstream.h>
#include <vcl.h>
#pragma hdrstop
#include "CZT_BirdFramework.h"
#include "CWavIterator.h"
#include "david.h"
#include "CWavIterator.h"
#include "table_dialog.h"
const SELDIRHELP = 1000;    // needed for SelectDirectory call
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ZTBIRDCONTROLPACKLib_OCX"
#pragma link "ZTBIRDCONTROLPACKLib_OCX"
#pragma link "NumLab"
#pragma link "MyAccess"
#pragma link "ZTBIRDCONTROLPACKLib_OCX"
#pragma link "CSPIN"
#pragma link "CSPIN"
#pragma link "DBAccess"
#pragma link "MemDS"
#pragma link "ZTBIRDCONTROLPACKLib_OCX"
#pragma link "xlcClasses"
#pragma link "xlEngine"
#pragma link "xlReport"
#pragma resource "*.dfm"
#define morningSession 0
#define afternoonSession 1
TForm1 *Form1;
using namespace ztBirdEngine;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
  Timer_freq=10; // the timer tick 10 times a second. 
}
//---------------------------------------------------------------------------
inline void TForm1::SetBirdString(TztBird* pBird,int nID,char* string)
{
     BSTR myBstr = AnsiToOLESTR(string);
     pBird->SetStringValue(nID,myBstr);
     SysFreeString(myBstr);
}

inline void TForm1::SetEngineString(int nID,char* string)
{
     BSTR myBstr = AnsiToOLESTR(string);
     ztBirdEngine1->SetStringValue(nID,myBstr);
     SysFreeString(myBstr);
}

void TForm1::HandleMonitorDisplay(TComboBox* pBox,int nBird)
{
   m_Birds[nBird]->SetLongValue(vMonitorDisplay,pBox->ItemIndex);
}

void TForm1::InitBirds()
{
     int nState;
     long lThreshold;

     // reading the state of the channel and setting the display accordingly
     nState = ztBird1->GetLongValue(vBirdState);
     if(nState & ztbs_Active) state1->ItemIndex = 1;
     else state1->ItemIndex = 2;

     nState = ztBird2->GetLongValue(vBirdState);
     if(nState & ztbs_Active) state2->ItemIndex = 1;
     else state2->ItemIndex = 2;

     nState = ztBird3->GetLongValue(vBirdState);
     if(nState & ztbs_Active) state3->ItemIndex = 1;
     else state3->ItemIndex = 2;

     nState = ztBird4->GetLongValue(vBirdState);
     if(nState & ztbs_Active) state4->ItemIndex = 1;
     else state4->ItemIndex = 2;

     // reading the threshold values and setting display accordingly
     lThreshold = ztBird1->GetLongValue(vPeakThreshold);
     trckThreshold1->Position = -lThreshold/320; thresh1->Value=lThreshold;

     lThreshold = ztBird2->GetLongValue(vPeakThreshold);
     trckThreshold2->Position = -lThreshold/320; thresh2->Value=lThreshold;

     lThreshold = ztBird3->GetLongValue(vPeakThreshold);
     trckThreshold3->Position = -lThreshold/320; thresh3->Value=lThreshold;

     lThreshold = ztBird4->GetLongValue(vPeakThreshold);
     trckThreshold4->Position = -lThreshold/320; thresh4->Value=lThreshold;

     minimum_recording_duration->Position = ztBird1->GetLongValue(vPeakRecordDuration)/1000;
     long soundDur = ztBird1->GetLongValue(vPeakComparator);
     SoundDurTb->Position =  soundDur;

     bool st;
     if(current->Active)
     {
     current->First();
     current->Next(); st=current->FieldValues["training"]; if(st)train1->Down=true; else train1->Down=false; train1Click(this);
     current->Next(); st=current->FieldValues["training"]; if(st)train2->Down=true; else train2->Down=false; train2Click(this);
     current->Next(); st=current->FieldValues["training"]; if(st)train3->Down=true; else train3->Down=false; train3Click(this);
     current->Next(); st=current->FieldValues["training"]; if(st)train4->Down=true; else train4->Down=false; train4Click(this);
     }
     SoundDurTbChange(this); // setting the number of peaks for accepting a file
     minimum_recording_durationChange(this); // setting the minimum duration of recording session after threshold is met

     // display the song to by played
     song1->Caption=ExtractFileName(ztBird1->GetStringValue(vBirdWaveFile));
     song2->Caption=ExtractFileName(ztBird2->GetStringValue(vBirdWaveFile));
     song3->Caption=ExtractFileName(ztBird3->GetStringValue(vBirdWaveFile));
     song4->Caption=ExtractFileName(ztBird4->GetStringValue(vBirdWaveFile));

     songList1->Items->Add(ztBird1->GetStringValue(vBirdWaveFile)); songList1->ItemIndex=0;
     songList2->Items->Add(ztBird2->GetStringValue(vBirdWaveFile)); songList2->ItemIndex=0;
     songList3->Items->Add(ztBird3->GetStringValue(vBirdWaveFile)); songList3->ItemIndex=0;
     songList4->Items->Add(ztBird4->GetStringValue(vBirdWaveFile)); songList4->ItemIndex=0;
}


void TForm1::Disable_folder_changing()
{

   SaveDefault->Enabled=false;  loadConfig->Enabled=false;  btnStartEngine->Enabled=false;
   btnCfgBird1->Enabled=false;  btnCfgBird2->Enabled=false; btnCfgBird3->Enabled=false; btnCfgBird4->Enabled=false;
   btnStopEngine->Enabled=true;
  // btnLoadSnd1->Enabled=false;  btnLoadSnd2->Enabled=false; btnLoadSnd3->Enabled=false; btnLoadSnd4->Enabled=false;
 //  btnAudTarg1->Enabled=false;  btnAudTarg2->Enabled=false; btnAudTarg3->Enabled=false; btnAudTarg4->Enabled=false;
   options1->Enabled=false;  options2->Enabled=false; options3->Enabled=false; options4->Enabled=false;
}

void TForm1::Enable_folder_changing()
{

  SaveDefault->Enabled=true;  loadConfig->Enabled=true;  btnStartEngine->Enabled=true;
   btnCfgBird1->Enabled=true;  btnCfgBird2->Enabled=true; btnCfgBird3->Enabled=true; btnCfgBird4->Enabled=true;
   btnLoadSnd1->Enabled=true;  btnLoadSnd2->Enabled=true; btnLoadSnd3->Enabled=true; btnLoadSnd4->Enabled=true;
 //  btnAudTarg1->Enabled=true;  btnAudTarg2->Enabled=true; btnAudTarg3->Enabled=true; btnAudTarg4->Enabled=true;
   options1->Enabled=true;  options2->Enabled=true;    options3->Enabled=true; options4->Enabled=true;
   btnStopEngine->Enabled=false;
}
void __fastcall TForm1::btnSaveCfgClick(TObject *Sender)
{
   
   ztBirdEngine1->InvokeSaveDlg();

}


void TForm1::EnableTriggers(bool bEnable)
{

   btnTrigger1->Enabled = bEnable;
   btnTrigger2->Enabled = bEnable;
   btnTrigger3->Enabled = bEnable;
   btnTrigger4->Enabled = bEnable;
}


void TForm1::InitDataArrays()
{
  m_Birds[0] = ztBird1;  m_Birds[1] = ztBird2; m_Birds[2] = ztBird3;  m_Birds[3] = ztBird4;
  m_BirdTrackBars[0]=trckThreshold1; m_BirdTrackBars[1]=trckThreshold2;
  m_BirdTrackBars[2]=trckThreshold3; m_BirdTrackBars[3]=trckThreshold4;
  for(int i=0;i<10;i++){ effector_delay[i]=0; effector_timer[i]=0; }
  updateIO(true);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::trckThreshold1Change(TObject *Sender)
{
  TztBird* Bird; TNumLab *thresh; TTrackBar *threshTb;
  if(Sender==trckThreshold1) {threshTb=trckThreshold1; thresh=thresh1; Bird=m_Birds[0];}
  else if(Sender==trckThreshold2) {threshTb=trckThreshold2; thresh=thresh2; Bird=m_Birds[1];}
  else if(Sender==trckThreshold3) {threshTb=trckThreshold3; thresh=thresh3; Bird=m_Birds[2];}
  else if(Sender==trckThreshold4) {threshTb=trckThreshold4; thresh=thresh4; Bird=m_Birds[3];}

  thresh->Value=threshTb->Position*320;
  int nNewThreshold = thresh->Value;
  Bird->SetLongValue(vPeakThreshold,nNewThreshold);
}
//---------------------------------------------------------------------------




void __fastcall TForm1::btnCfgBird1Click(TObject *Sender)
{
  ztBird1->ConfigureAudioSource();
 sound_source1->Caption=ztBird1->GetStringValue(vAudioSrcDescription);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnCfgBird2Click(TObject *Sender)
{
 ztBird2->ConfigureAudioSource();
 sound_source2->Caption=ztBird2->GetStringValue(vAudioSrcDescription);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnCfgBird3Click(TObject *Sender)
{
 ztBird3->ConfigureAudioSource();
 sound_source3->Caption=ztBird3->GetStringValue(vAudioSrcDescription);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnCfgBird4Click(TObject *Sender)
{
 ztBird4->ConfigureAudioSource();
 sound_source4->Caption=ztBird4->GetStringValue(vAudioSrcDescription);
}

//---------------------------------------------------------------------------

void __fastcall TForm1::OnEngineNotification(TObject *Sender,
      long nBufferID)
{
 	using namespace ztBirdEngine;

	switch(nBufferID)
	{
		case ztee_Error_NoValidBirds:
		case ztee_Error_NoValidAudioDrivers:
		case ztee_Warning_BirdNotUsed:
		case ztee_Status_EngineStarted:
		case ztee_Status_EngineStopped:
		case ztee_Error_EngineNotStarted:
		case ztee_Error_NoTempFilePathSet:
              //  str=zt_EngineEvent_Description[nBufferID];
              //  Memo1->Lines->Add(str);
               // moEngineNotes->Text += zt_EngineEvent_Description[nBufferID];
               // moEngineNotes->Text += "\r\n";
                break;
	}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnStartEngineClick(TObject *Sender)
{
    sound_source1->Caption=ztBird1->GetStringValue(vAudioSrcDescription);
    sound_source2->Caption=ztBird2->GetStringValue(vAudioSrcDescription);
    sound_source3->Caption=ztBird3->GetStringValue(vAudioSrcDescription);
    sound_source4->Caption=ztBird4->GetStringValue(vAudioSrcDescription);

    song1->Caption=ExtractFileName(ztBird1->GetStringValue(vBirdWaveFile));
    song2->Caption=ExtractFileName(ztBird2->GetStringValue(vBirdWaveFile));
    song3->Caption=ExtractFileName(ztBird3->GetStringValue(vBirdWaveFile));
    song4->Caption=ExtractFileName(ztBird4->GetStringValue(vBirdWaveFile));

    Disable_folder_changing();
    EnableTriggers(true);
    ztBirdEngine1->StartEngine();
    recording=true;
    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnStopEngineClick(TObject *Sender)
{
    ztBirdEngine1->StopEngine();
    EnableTriggers(false);
    recording=false;
    Enable_folder_changing();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::btnAudTarg2Click(TObject *Sender)
{
m_Birds[1]->ConfigureAudioTarget();        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnAudTarg4Click(TObject *Sender)
{
m_Birds[3]->ConfigureAudioTarget();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnAudTarg3Click(TObject *Sender)
{
m_Birds[2]->ConfigureAudioTarget();        
}
//---------------------------------------------------------------------------


void __fastcall TForm1::btnTrigger2Click(TObject *Sender)
{
m_Birds[1]->TriggerSound();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnTrigger4Click(TObject *Sender)
{
m_Birds[3]->TriggerSound();          
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnTrigger3Click(TObject *Sender)
{
m_Birds[2]->TriggerSound();
}
//---------------------------------------------------------------------------


void TForm1::HandleLoadWav(int nBird)
{
  OpenDialog1->Filter = "Wav files (*.wav)|*.WAV";
  if(OpenDialog1->Execute())
  {
    switch(nBird)
    {
     case 0: if (FileExists(OpenDialog1->FileName.c_str()))
             {
               SetBirdString(ztBird1,vBirdWaveFile,OpenDialog1->FileName.c_str());
               song1->Caption=OpenDialog1->FileName;
             }
             else {Application->MessageBox("file cannot be found, please try again", NULL, MB_OK);}
             break;

     case 1: if (FileExists(OpenDialog1->FileName.c_str()))
             {
               SetBirdString(ztBird2,vBirdWaveFile,OpenDialog1->FileName.c_str());
               song2->Caption=OpenDialog1->FileName;
             }
             else {Application->MessageBox("file cannot be found, please try again", NULL, MB_OK);}
             break;

     case 2: if (FileExists(OpenDialog1->FileName.c_str()))
             {
               SetBirdString(ztBird3,vBirdWaveFile,OpenDialog1->FileName.c_str());
               song3->Caption=OpenDialog1->FileName;
             }
             else {Application->MessageBox("file cannot be found, please try again", NULL, MB_OK);}
             break;

     case 3: if (FileExists(OpenDialog1->FileName.c_str()))
             {
               SetBirdString(ztBird4,vBirdWaveFile,OpenDialog1->FileName.c_str());
               song4->Caption=OpenDialog1->FileName;
             }
             else {Application->MessageBox("file cannot be found, please try again", NULL, MB_OK);}
             break;
  }

 }
}
                                                                                                               


void __fastcall TForm1::btnLoadSnd2Click(TObject *Sender)
{
 HandleLoadWav(1);
 song2->Caption=ExtractFileName(ztBird2->GetStringValue(vBirdWaveFile));
 songList2->Items->Add(ztBird2->GetStringValue(vBirdWaveFile));
  songList2->ItemIndex=0;
}

//---------------------------------------------------------------------------

void __fastcall TForm1::btnLoadSnd4Click(TObject *Sender)
{
 HandleLoadWav(3);
 song4->Caption=ExtractFileName(ztBird4->GetStringValue(vBirdWaveFile));
 songList4->Items->Add(ztBird4->GetStringValue(vBirdWaveFile));
 songList4->ItemIndex=0;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnLoadSnd3Click(TObject *Sender)
{
HandleLoadWav(2);
song3->Caption=ExtractFileName(ztBird3->GetStringValue(vBirdWaveFile));
songList3->Items->Add(ztBird3->GetStringValue(vBirdWaveFile));
 songList3->ItemIndex=0;
}
//---------------------------------------------------------------------------void __fastcall TForm1::buttonClick(TObject *Sender)



void __fastcall TForm1::btnAudTarg1Click(TObject *Sender)
{
        m_Birds[0]->ConfigureAudioTarget();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnLoadSnd1Click(TObject *Sender)
{
        HandleLoadWav(0);
        song1->Caption=ExtractFileName(ztBird1->GetStringValue(vBirdWaveFile));
        songList1->Items->Add(ztBird1->GetStringValue(vBirdWaveFile));
        songList1->ItemIndex=0;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btnTrigger1Click(TObject *Sender)
{
m_Birds[0]->TriggerSound();
}
//---------------------------------------------------------------------------



void __fastcall TForm1::ExitClick(TObject *Sender)
{
    ztBirdEngine1->StopEngine();
    EnableTriggers(false);
    recording=false;
    Enable_folder_changing();
    SaveDefaultClick(this);
    if(current->Active)
     {
      current->RecNo=2; current->Edit();
      current->FieldValues["pecks"]=Playbacks1->Value; current->Next(); current->Edit();
      current->FieldValues["pecks"]=Playbacks2->Value; current->Next(); current->Edit();
      current->FieldValues["pecks"]=Playbacks3->Value; current->Next(); current->Edit();
      current->FieldValues["pecks"]=Playbacks4->Value;
      current->Post();
   }
    Application->Terminate();

}
//---------------------------------------------------------------------------

void TForm1::training_schedules()
{
 static AnsiString ref_date=Date();
 if(ref_date!=date) // a new day!
 {
    ref_date=date;
    Word Year1, Month1, Day1, Year2, Month2, Day2;
    TDateTime date = Now();
    DecodeDate(date, Year1, Month1, Day1);

    // onsets of trainings
    DecodeDate(start1->Date, Year2, Month2, Day2);
    if(Month1==Month2 && Day1==Day2){train1->Down=true; train1Click(this);}
    DecodeDate(start2->Date, Year2, Month2, Day2);
    if(Month1==Month2 && Day1==Day2){train2->Down=true; train2Click(this);}
    DecodeDate(start3->Date, Year2, Month2, Day2);
    if(Month1==Month2 && Day1==Day2){train3->Down=true; train3Click(this);}
    DecodeDate(start4->Date, Year2, Month2, Day2);
    if(Month1==Month2 && Day1==Day2){train4->Down=true; train4Click(this);}

    // stopping trainings
    DecodeDate(end1->Date, Year2, Month2, Day2);
    if(Month1==Month2 && Day1==Day2)train1->Down=false;
    DecodeDate(end2->Date, Year2, Month2, Day2);
    if(Month1==Month2 && Day1==Day2)train2->Down=false;
    DecodeDate(end3->Date, Year2, Month2, Day2);
    if(Month1==Month2 && Day1==Day2)train3->Down=false;
    DecodeDate(end4->Date, Year2, Month2, Day2);
    if(Month1==Month2 && Day1==Day2)train4->Down=false; 

 }
}


void __fastcall TForm1::Timer1Timer(TObject *Sender)   // 100ms
{
  // bool passivePlay;
   static int i=0;
   i++;
   if(i>10)
   {
    // date=Date();
     date=FormatDateTime("yyyy'-'mm'-'dd", Date());
     tm=Time(); i=0; Label1->Caption = tm; gettime(&tStr);
     training_schedules(); // check if this is a time to start of stop training
     hour=tStr.ti_hour;
     minute=tStr.ti_min;     
     if(hour==lightsOff->Value && !sleepTime)
     {

     //  MyConnection1->Connected=false; sql_on=false;
       sessionLb->Caption="Sleep time";
       Form1->Color=clBlue;
       if(nightStop->Checked){ztBirdEngine1->StopEngine(); EnableTriggers(false);}
       sleepTime=true;

     }
   
     if(hour==lightsOn->Value && sleepTime)
     {

       sessionLb->Caption="Good Morning!";
       Form1->Color=clBtnFace;
       CheckDates();   // new day -- should training start?
       if(nightStop->Checked){EnableTriggers(true); ztBirdEngine1->StartEngine();}
       sleepTime=false;
     //  MyConnection1->Connected=true;
     //  if(MyConnection1->Connected) {sql_on=true; keypeckTable->Active=true;}

     }


     if(hour>=AfternoonSession->Value && session==morningSession)
     {
     //   MyConnection1->Connected=false; sql_on=false;
        sessionLb->Caption="Afternoon session";
        session=afternoonSession;
        Playbacks1->Value=0;
        Playbacks2->Value=0;
        Playbacks3->Value=0;
        Playbacks4->Value=0;

     }
   //  else {MyConnection1->Connected=true; sql_on=true; keypeck_Table->Active=true;}

     if(hour>=MorningSession->Value && hour<AfternoonSession->Value && session==afternoonSession)
     {
     //   MyConnection1->Connected=false;
        sessionLb->Caption="Morning session";
        session=morningSession;
        Playbacks1->Value=0;
        Playbacks2->Value=0;
        Playbacks3->Value=0;
        Playbacks4->Value=0;
     //   MyConnection1->Connected=true;
     }
   //  else {MyConnection1->Connected=true; sql_on=true; keypeck_Table->Active=true;}
   }   //    if( && greenLight)select();
 
 if(recording && ( (hour<lightsOff->Value && hour>=lightsOn->Value) || !dont_train_at_night->Checked))// && MyConnection1->Connected && keypeck_Table->Active)
 {
   if(train1->Down) Handle_training(0);
   if(train2->Down) Handle_training(1);
   if(train3->Down) Handle_training(2);
   if(train4->Down) Handle_training(3);
 }
 
}
//---------------------------------------------------------------------------


void __fastcall TForm1::state1Click(TObject *Sender)
{

  using namespace ztBirdEngine;
  switch(state1->ItemIndex)
  {
    case 0: ztBird1->SetLongValue(vBirdState,0); ztBird1->SetLongValue(vRecordingDisplay,0); break;
    case 1: ztBird1->SetLongValue(vBirdState,1); ztBird1->SetLongValue(vRecordingDisplay,2); break;
    case 2: ztBird1->SetLongValue(vBirdState,2); ztBird1->SetLongValue(vMonitorDisplay,2);
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::display1Click(TObject *Sender)
{
  using namespace ztBirdEngine;
  switch(display1->ItemIndex)
  {
     case 0: ztBird1->SetLongValue(vRecordingDisplay,0); ztBird1->SetLongValue(vMonitorDisplay,0); break;
     case 1: ztBird1->SetLongValue(vRecordingDisplay,1); ztBird1->SetLongValue(vMonitorDisplay,1); break;
     case 2: ztBird1->SetLongValue(vRecordingDisplay,2); ztBird1->SetLongValue(vMonitorDisplay,2); break;
     case 3: ztBird1->SetLongValue(vRecordingDisplay,3); ztBird1->SetLongValue(vMonitorDisplay,3); break;
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::state2Click(TObject *Sender)
{
  using namespace ztBirdEngine;
  switch(state2->ItemIndex)
  {
    case 0: ztBird2->SetLongValue(vBirdState,0); ztBird2->SetLongValue(vRecordingDisplay,0); break;
    case 1: ztBird2->SetLongValue(vBirdState,1); ztBird2->SetLongValue(vRecordingDisplay,2); break;
    case 2: ztBird2->SetLongValue(vBirdState,2); ztBird2->SetLongValue(vMonitorDisplay,2);
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::state3Click(TObject *Sender)
{
 using namespace ztBirdEngine;
  switch(state3->ItemIndex)
  {
    case 0: ztBird3->SetLongValue(vBirdState,0); ztBird3->SetLongValue(vRecordingDisplay,0); break;
    case 1: ztBird3->SetLongValue(vBirdState,1); ztBird3->SetLongValue(vRecordingDisplay,2); break;
    case 2: ztBird3->SetLongValue(vBirdState,2); ztBird3->SetLongValue(vMonitorDisplay,2);
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::state4Click(TObject *Sender)
{
 using namespace ztBirdEngine;
  switch(state1->ItemIndex)
  {
    case 0: ztBird4->SetLongValue(vBirdState,0); ztBird4->SetLongValue(vRecordingDisplay,0); break;
    case 1: ztBird4->SetLongValue(vBirdState,1); ztBird4->SetLongValue(vRecordingDisplay,2); break;
    case 2: ztBird4->SetLongValue(vBirdState,2); ztBird4->SetLongValue(vMonitorDisplay,2);
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::display2Click(TObject *Sender)
{
using namespace ztBirdEngine;
  switch(display2->ItemIndex)
  {
     case 0: ztBird2->SetLongValue(vRecordingDisplay,0); ztBird2->SetLongValue(vMonitorDisplay,0); break;
     case 1: ztBird2->SetLongValue(vRecordingDisplay,1); ztBird2->SetLongValue(vMonitorDisplay,1); break;
     case 2: ztBird2->SetLongValue(vRecordingDisplay,2); ztBird2->SetLongValue(vMonitorDisplay,2); break;
     case 3: ztBird2->SetLongValue(vRecordingDisplay,3); ztBird2->SetLongValue(vMonitorDisplay,3); break;
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::display3Click(TObject *Sender)
{
using namespace ztBirdEngine;
  switch(display3->ItemIndex)
  {
     case 0: ztBird3->SetLongValue(vRecordingDisplay,0); ztBird3->SetLongValue(vMonitorDisplay,0); break;
     case 1: ztBird3->SetLongValue(vRecordingDisplay,1); ztBird3->SetLongValue(vMonitorDisplay,1); break;
     case 2: ztBird3->SetLongValue(vRecordingDisplay,2); ztBird3->SetLongValue(vMonitorDisplay,2); break;
     case 3: ztBird3->SetLongValue(vRecordingDisplay,3); ztBird3->SetLongValue(vMonitorDisplay,3); break;
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::display4Click(TObject *Sender)
{
using namespace ztBirdEngine;
  switch(display4->ItemIndex)
  {
     case 0: ztBird4->SetLongValue(vRecordingDisplay,0); ztBird4->SetLongValue(vMonitorDisplay,0); break;
     case 1: ztBird4->SetLongValue(vRecordingDisplay,1); ztBird4->SetLongValue(vMonitorDisplay,1); break;
     case 2: ztBird4->SetLongValue(vRecordingDisplay,2); ztBird4->SetLongValue(vMonitorDisplay,2); break;
     case 3: ztBird4->SetLongValue(vRecordingDisplay,3); ztBird4->SetLongValue(vMonitorDisplay,3); break;
  }
}
//---------------------------------------------------------------------------




void __fastcall TForm1::train1Click(TObject *Sender)
{
 if(train1->Down){key1->Visible=true; key2->Visible=true;
                  if(current->Active)
                  {
                  current->RecNo=2;
                  current->Edit();
                  current->FieldValues["training"]=true;
                  current->Post();
                  }}
 else {key1->Visible=false; key2->Visible=false;
                if(current->Active) {current->RecNo=2;current->Edit(); current->FieldValues["training"]=false; current->Post();}}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::train2Click(TObject *Sender)
{
 if(train2->Down){ key3->Visible=true; key4->Visible=true;
                if(current->Active){current->RecNo=3; current->Edit(); current->FieldValues["training"]=true; current->Post();}}
 else {key3->Visible=false; key4->Visible=false;
                if(current->Active){current->RecNo=3; current->Edit(); current->FieldValues["training"]=false; current->Post();}}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::train3Click(TObject *Sender)
{
 if(train3->Down){ key5->Visible=true; key6->Visible=true;
                if(current->Active){current->RecNo=4; current->Edit(); current->FieldValues["training"]=true; current->Post();}}
 else { key5->Visible=false; key6->Visible=false;
                if(current->Active){current->RecNo=4; current->Edit(); current->FieldValues["training"]=false; current->Post();}}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::train4Click(TObject *Sender)
{
 if(train4->Down){ key7->Visible=true; key8->Visible=true;
                if(current->Active){current->RecNo=5; current->Edit(); current->FieldValues["training"]=true; current->Post();}}
 else { key7->Visible=false; key8->Visible=false;
                if(current->Active){current->RecNo=5; current->Edit(); current->FieldValues["training"]=false; current->Post();}}
}
//---------------------------------------------------------------------------




void __fastcall TForm1::MaxDurChange(TObject *Sender)
{
  MaxDurN->Value=MaxDur->Position*1000;
  for(int i=0;i<4;i++) m_Birds[i]->SetLongValue(vRecordingCutoffMS, MaxDurN->Value);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Reset1Click(TObject *Sender)
{
 Playbacks1->Value=0;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn2Click(TObject *Sender)
{
  Playbacks2->Value=0;     
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn4Click(TObject *Sender)
{
Playbacks3->Value=0;             
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn5Click(TObject *Sender)
{
Playbacks4->Value=0;             
}
//---------------------------------------------------------------------------

void __fastcall TForm1::loadConfigClick(TObject *Sender)
{
   ztBirdEngine1->InvokeLoadDlg();
                                                                           
}
//---------------------------------------------------------------------------

void __fastcall TForm1::minimum_recording_durationChange(TObject *Sender)
{
  duration->Value=minimum_recording_duration->Position*1000;
  for(int i=0;i<4;i++) m_Birds[i]->SetLongValue(vPeakRecordDuration,duration->Value);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SoundDurTbChange(TObject *Sender)
{
  peaksN->Value=SoundDurTb->Position;
  long peaks = peaksN->Value;
  for(int i=0;i<4;i++) m_Birds[i]->SetLongValue(vPeakComparator,peaks);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::FormShow(TObject *Sender)
{
     // variable setting:

     if(!database_exists("sap")) create_database("sap");
     MyConnection1->Database="sap";
     MyConnection1->Connected=true;
     if(!table_exists("birds"))create_birds();
     if(!table_exists("key_pecks"))create_keypecks();
     if(!table_exists("nidaq")) create_nidaq();
     session=morningSession;
     sleepTime=false;
     IsBusy=false;
     recording=false;
     NIdeviceNum=1;
     for(int i=0;i<9;i++)accumPecks[i]=0;
     for(int i=0;i<5;i++) {Latent[i]=50;  recorded[i]=0; training_latency[i]=20;}
     Duration=200; PeakFrLow=5;

     // the IDs should not be changed by user, only birdname is changed and called ID sometimes
     SetBirdString(ztBird1,vBirdID,"a");
     SetBirdString(ztBird2,vBirdID,"b");
     SetBirdString(ztBird3,vBirdID,"c");
     SetBirdString(ztBird4,vBirdID,"d");

     try{NIDAQ->Active=true;} // check if table is okay:
     catch(...){Application->MessageBox("could not find NIDAQ table, keys will be assigned with default values", NULL, MB_OK);}
     try{InitDataArrays(); }  // some additional variable settings and data card state settings
     catch(...){Application->MessageBox("data array error, the NIDAQ table might be corrupted", NULL, MB_OK);}

     if(!LoadDefault()) try{set_default();}  // setting or loading default state
     catch(...){Application->MessageBox("set default error", NULL, MB_OK);}
     // check if tables are okay:
     
     try{current->Active=true;}
     catch(...){Application->MessageBox("could not find setting table, recorder will start using default values", NULL, MB_OK);}
     try{NIDAQ->Active=true;}
     catch(...){Application->MessageBox("could not find NIDAQ table. Key pecks wont register", NULL, MB_OK); }
     try{keypeckTable->Active=true;}
     catch(...){Application->MessageBox("could not find key-pecks table. Training records wont register", NULL, MB_OK); }

     TrainingClick(this);  // configure data card
     
     sound_source1->Caption=ztBird1->GetStringValue(vAudioSrcDescription);
     sound_source2->Caption=ztBird2->GetStringValue(vAudioSrcDescription);
     sound_source3->Caption=ztBird3->GetStringValue(vAudioSrcDescription);
     sound_source4->Caption=ztBird4->GetStringValue(vAudioSrcDescription);

     song1->Caption=ExtractFileName(ztBird1->GetStringValue(vBirdWaveFile));
     song2->Caption=ExtractFileName(ztBird2->GetStringValue(vBirdWaveFile));
     song3->Caption=ExtractFileName(ztBird3->GetStringValue(vBirdWaveFile));
     song4->Caption=ExtractFileName(ztBird4->GetStringValue(vBirdWaveFile));


  if(current->Active)
  {
       current->RecNo=2;
       str=current->FieldValues["bird_ID"];
       SetBirdString(ztBird1,vBirdName,str.c_str());
       hatch1->Date=current->FieldValues["hatching_date"];
       start1->Date=current->FieldValues["start_training"];
       end1->Date=current->FieldValues["stop_training"];

       current->RecNo=3;
       str=current->FieldValues["bird_ID"];
       SetBirdString(ztBird2,vBirdName,str.c_str());
       hatch2->Date=current->FieldValues["hatching_date"];
       start2->Date=current->FieldValues["start_training"];
       end2->Date=current->FieldValues["stop_training"];

       current->RecNo=4;
       str=current->FieldValues["bird_ID"];
       SetBirdString(ztBird3,vBirdName,str.c_str());
       hatch3->Date=current->FieldValues["hatching_date"];
       start3->Date=current->FieldValues["start_training"];
       end3->Date=current->FieldValues["stop_training"];

       current->RecNo=5;
       str=current->FieldValues["bird_ID"];
       SetBirdString(ztBird4,vBirdName,str.c_str());
       hatch4->Date=current->FieldValues["hatching_date"];
       start4->Date=current->FieldValues["start_training"];
       end4->Date=current->FieldValues["stop_training"];
     }

     str=ztBird1->GetStringValue(vBirdName); name[0]=str.ToInt();
     str=ztBird2->GetStringValue(vBirdName); name[1]=str.ToInt();
     str=ztBird3->GetStringValue(vBirdName); name[2]=str.ToInt();
     str=ztBird4->GetStringValue(vBirdName); name[3]=str.ToInt();

     CheckDates();
     InitBirds();
     tmpSound1->Caption=ztBird1->GetStringValue(vBirdTempFilePath);
     save1->Caption=ztBird1->GetStringValue(vBirdWaveFilePath);
     tmpSound2->Caption=ztBird2->GetStringValue(vBirdTempFilePath);
     save2->Caption=ztBird2->GetStringValue(vBirdWaveFilePath);
     tmpSound3->Caption=ztBird3->GetStringValue(vBirdTempFilePath);
     save3->Caption=ztBird3->GetStringValue(vBirdWaveFilePath);
     tmpSound4->Caption=ztBird4->GetStringValue(vBirdTempFilePath);
     save4->Caption=ztBird4->GetStringValue(vBirdWaveFilePath);

     trckThreshold1->Position=ztBird1->GetLongValue(vPeakThreshold)/320;
     trckThreshold2->Position=ztBird2->GetLongValue(vPeakThreshold)/320;
     trckThreshold3->Position=ztBird3->GetLongValue(vPeakThreshold)/320;
     trckThreshold4->Position=ztBird4->GetLongValue(vPeakThreshold)/320;

     gain1->Position=ztBird1->GetFloatValue(vPeakGain)*100; gainN1->Value=(float)gain1->Position/100.0;
     gain2->Position=ztBird2->GetFloatValue(vPeakGain)*100; gainN2->Value=(float)gain2->Position/100.0;
     gain3->Position=ztBird3->GetFloatValue(vPeakGain)*100; gainN3->Value=(float)gain3->Position/100.0;
     gain4->Position=ztBird4->GetFloatValue(vPeakGain)*100; gainN4->Value=(float)gain4->Position/100.0;

     problems1=0; problems2=0; problems3=0; problems4=0;
}
//---------------------------------------------------------------------------

void TForm1::set_default()
{
     SetBirdString(ztBird1,vBirdName,"1");
     SetBirdString(ztBird2,vBirdName,"2");
     SetBirdString(ztBird3,vBirdName,"3");
     SetBirdString(ztBird4,vBirdName,"4");

     // force all the channels to recording state
     ztBird1->SetLongValue(vBirdState,ztbs_Active); // state 1
     ztBird2->SetLongValue(vBirdState,ztbs_Active);
     ztBird3->SetLongValue(vBirdState,ztbs_Active);
     ztBird4->SetLongValue(vBirdState,ztbs_Active);

     ztBird1->SetLongValue(vRecordingDisplay,3);
     ztBird2->SetLongValue(vRecordingDisplay,3);
     ztBird3->SetLongValue(vRecordingDisplay,3);
     ztBird4->SetLongValue(vRecordingDisplay,3);

     for(int i=0;i<4;i++) m_Birds[i]->SetLongValue(vRecordingCutoffMS,MaxDur->Position*1000);

     AnsiString folder="c:\\SAP\\tmpSound"; if(!DirectoryExists(folder)) ForceDirectories(folder);
     for(int i=0;i<4;i++)SetBirdString(m_Birds[i], vBirdTempFilePath, folder.c_str());
     folder="c:\\SAP\\in1"; if(!DirectoryExists(folder)) ForceDirectories(folder);
     SetBirdString(m_Birds[0],vBirdWaveFilePath, folder.c_str());
     folder1->Caption=ztBird1->GetStringValue(vBirdWaveFilePath);
     folder="c:\\SAP\\in2"; if(!DirectoryExists(folder)) ForceDirectories(folder);
     SetBirdString(m_Birds[1],vBirdWaveFilePath, folder.c_str());
     folder="c:\\SAP\\in3"; if(!DirectoryExists(folder)) ForceDirectories(folder);
     SetBirdString(m_Birds[2],vBirdWaveFilePath, folder.c_str());
     folder="c:\\SAP\\in4"; if(!DirectoryExists(folder)) ForceDirectories(folder);
     SetBirdString(m_Birds[3],vBirdWaveFilePath, folder.c_str());
}




void __fastcall TForm1::CSpinButton1DownClick(TObject *Sender)
{
  lightsOn->Value--;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CSpinButton1UpClick(TObject *Sender)
{
   lightsOn->Value++; 
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CSpinButton2DownClick(TObject *Sender)
{
  lightsOff->Value--;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CSpinButton2UpClick(TObject *Sender)
{
   lightsOff->Value++;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CSpinButton3DownClick(TObject *Sender)
{
  MorningSession->Value--;        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CSpinButton3UpClick(TObject *Sender)
{
  MorningSession->Value++;            
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CSpinButton4DownClick(TObject *Sender)
{
  AfternoonSession->Value--;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CSpinButton4UpClick(TObject *Sender)
{
  AfternoonSession->Value++;       
}
//---------------------------------------------------------------------------


void __fastcall TForm1::set1Click(TObject *Sender)
{
  str=InputBox("Bird ID", "please type bird's name", "Default string");
  SetBirdString(ztBird1,vBirdName,str.c_str());
  recorded[0]=0;
  str=ztBird1->GetStringValue(vBirdName); name[0]=str.ToInt();
  if(current->Active)
  {
      current->RecNo=2; current->Edit();
      current->FieldValues["bird_ID"]=str;
      current->Post();
  }
  
}
//---------------------------------------------------------------------------

void __fastcall TForm1::set2Click(TObject *Sender)
{
  str=InputBox("Bird ID", "please type bird's name", "Default string");
  SetBirdString(ztBird2,vBirdName,str.c_str());
  recorded[1]=0;
  str=ztBird2->GetStringValue(vBirdName); name[1]=str.ToInt();
  if(current->Active)
  {
      current->RecNo=3; current->Edit();
      current->FieldValues["bird_ID"]=str;
      current->Post();
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::set3Click(TObject *Sender)
{
  str=InputBox("Bird ID", "please type bird's name", "Default string");
  SetBirdString(ztBird3,vBirdName,str.c_str());
  recorded[2]=0;
  str=ztBird3->GetStringValue(vBirdName); name[2]=str.ToInt();
  if(current->Active)
  {
      current->RecNo=4; current->Edit();
      current->FieldValues["bird_ID"]=str;
      current->Post();
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::set4Click(TObject *Sender)
{
  str=InputBox("Bird ID", "please type bird's name", "Default string");
  SetBirdString(ztBird4,vBirdName,str.c_str());
  recorded[3]=0;
  str=ztBird4->GetStringValue(vBirdName); name[3]=str.ToInt();
  if(current->Active)
  {
      current->RecNo=5; current->Edit();
      current->FieldValues["bird_ID"]=str;
      current->Post();
  }
}
//---------------------------------------------------------------------------






void __fastcall TForm1::TrainingClick(TObject *Sender)
{
  if(Training->Checked)
  {
    try{configDataCard();}
    catch(...){Application->MessageBox("configDataCard error", NULL, MB_OK);}
    training=true;
    t1->Enabled=true; t2->Enabled=true; t3->Enabled=true; t4->Enabled=true;  // enabled the key port-line settup pannels
  }
  else
  {
    training=false;
    t1->Enabled=false; t2->Enabled=false; t3->Enabled=false; t4->Enabled=false;
    for(int i=0;i<9;i++)peck[i]=false;
  }
}
//---------------------------------------------------------------------------

void TForm1::configDataCard()
{
    zt_DIG_Prt_Config(NIdeviceNum, 0, 0, 0);  // input
    zt_DIG_Prt_Config(NIdeviceNum, 1, 0, 0);  // input
    zt_DIG_Prt_Config(NIdeviceNum, 2, 0, 1);  // output
}



void TForm1::updateIO(bool retrive)
{
   if(retrive && NIDAQ->Active)// get channels from table
   {
      NIdeviceNum=NIDAQ->FieldValues["device_ID"];
      NIDAQ->RecNo=1; p1->Value=NIDAQ->FieldValues["port"];  l1->Value=NIDAQ->FieldValues["line"];
      NIDAQ->RecNo=2; p2->Value=NIDAQ->FieldValues["port"];  l2->Value=NIDAQ->FieldValues["line"];
      NIDAQ->RecNo=3; p3->Value=NIDAQ->FieldValues["port"];  l3->Value=NIDAQ->FieldValues["line"];
      NIDAQ->RecNo=4; p4->Value=NIDAQ->FieldValues["port"];  l4->Value=NIDAQ->FieldValues["line"];
      NIDAQ->RecNo=5; p5->Value=NIDAQ->FieldValues["port"];  l5->Value=NIDAQ->FieldValues["line"];
      NIDAQ->RecNo=6; p6->Value=NIDAQ->FieldValues["port"];  l6->Value=NIDAQ->FieldValues["line"];
      NIDAQ->RecNo=7; p7->Value=NIDAQ->FieldValues["port"];  l7->Value=NIDAQ->FieldValues["line"];
      NIDAQ->RecNo=8; p8->Value=NIDAQ->FieldValues["port"];  l8->Value=NIDAQ->FieldValues["line"];
      NIDAQ->RecNo=9; p9->Value=NIDAQ->FieldValues["port"];  l9->Value=NIDAQ->FieldValues["line"];
      NIDAQ->RecNo=10; p10->Value=NIDAQ->FieldValues["port"];l10->Value=NIDAQ->FieldValues["line"];
      NIDAQ->RecNo=11; p11->Value=NIDAQ->FieldValues["port"];l11->Value=NIDAQ->FieldValues["line"];
      NIDAQ->RecNo=12; p12->Value=NIDAQ->FieldValues["port"];l12->Value=NIDAQ->FieldValues["line"];
      NIDAQ->RecNo=13; p13->Value=NIDAQ->FieldValues["port"];l13->Value=NIDAQ->FieldValues["line"];
      NIDAQ->RecNo=14; p14->Value=NIDAQ->FieldValues["port"];l14->Value=NIDAQ->FieldValues["line"];
      NIDAQ->RecNo=15; p15->Value=NIDAQ->FieldValues["port"];l15->Value=NIDAQ->FieldValues["line"];
      NIDAQ->RecNo=16; p16->Value=NIDAQ->FieldValues["port"];l16->Value=NIDAQ->FieldValues["line"];

   }
   else if(NIDAQ->Active) // save
   {
     NIDAQ->RecNo=1; NIDAQ->Edit();
     NIDAQ->FieldValues["device_ID"]=NIdeviceNum;
     NIDAQ->FieldValues["port"]=p1->Value;
     NIDAQ->FieldValues["line"]=l1->Value;

     NIDAQ->RecNo=2; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p2->Value;
     NIDAQ->FieldValues["line"]=l2->Value;

     NIDAQ->RecNo=3; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p3->Value;
     NIDAQ->FieldValues["line"]=l3->Value;

     NIDAQ->RecNo=4; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p4->Value;
     NIDAQ->FieldValues["line"]=l4->Value;

     NIDAQ->RecNo=5; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p5->Value;
     NIDAQ->FieldValues["line"]=l5->Value;

     NIDAQ->RecNo=6; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p6->Value;
     NIDAQ->FieldValues["line"]=l6->Value;

     NIDAQ->RecNo=7; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p7->Value;
     NIDAQ->FieldValues["line"]=l7->Value;

     NIDAQ->RecNo=8; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p8->Value;
     NIDAQ->FieldValues["line"]=l8->Value;

     NIDAQ->RecNo=9; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p9->Value;
     NIDAQ->FieldValues["line"]=l9->Value;

     NIDAQ->RecNo=10; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p10->Value;
     NIDAQ->FieldValues["line"]=l10->Value;

     NIDAQ->RecNo=11; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p11->Value;
     NIDAQ->FieldValues["line"]=l11->Value;

     NIDAQ->RecNo=12; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p12->Value;
     NIDAQ->FieldValues["line"]=l12->Value;

     NIDAQ->RecNo=13; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p13->Value;
     NIDAQ->FieldValues["line"]=l13->Value;

     NIDAQ->RecNo=14; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p14->Value;
     NIDAQ->FieldValues["line"]=l14->Value;

     NIDAQ->RecNo=15; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p15->Value;
     NIDAQ->FieldValues["line"]=l15->Value;

     NIDAQ->RecNo=16; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p16->Value;
     NIDAQ->FieldValues["line"]=l16->Value;

     NIDAQ->Post();
   }

}


void __fastcall TForm1::p1sDownClick(TObject *Sender)
{
  if(Sender==p1s)p1->Value--;
  if(Sender==p2s)p2->Value--;
  if(Sender==p3s)p3->Value--;
  if(Sender==p4s)p4->Value--;
  if(Sender==p5s)p5->Value--;
  if(Sender==p6s)p6->Value--;
  if(Sender==p7s)p7->Value--;
  if(Sender==p8s)p8->Value--;
  if(Sender==p9s)p9->Value--;
  if(Sender==p10s)p10->Value--;
  if(Sender==p11s)p11->Value--;
  if(Sender==p12s)p12->Value--;
  if(Sender==p13s)p13->Value--;
  if(Sender==p14s)p14->Value--;
  if(Sender==p15s)p15->Value--;
  if(Sender==p16s)p16->Value--;
  updateIO(false);// save the data
}
//---------------------------------------------------------------------------

void __fastcall TForm1::p1sUpClick(TObject *Sender)
{
  if(Sender==p1s)p1->Value++;
  if(Sender==p2s)p2->Value++;
  if(Sender==p3s)p3->Value++;
  if(Sender==p4s)p4->Value++;
  if(Sender==p5s)p5->Value++;
  if(Sender==p6s)p6->Value++;
  if(Sender==p7s)p7->Value++;
  if(Sender==p8s)p8->Value++;
  if(Sender==p9s)p9->Value++;
  if(Sender==p10s)p10->Value++;
  if(Sender==p11s)p11->Value++;
  if(Sender==p12s)p12->Value++;
  if(Sender==p13s)p13->Value++;
  if(Sender==p14s)p14->Value++;
  if(Sender==p15s)p15->Value++;
  if(Sender==p16s)p16->Value++;
  updateIO(false);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::l1sDownClick(TObject *Sender)
{
  if(Sender==l1s)l1->Value--;
  if(Sender==l2s)l2->Value--;
  if(Sender==l3s)l3->Value--;
  if(Sender==l4s)l4->Value--;
  if(Sender==l5s)l5->Value--;
  if(Sender==l6s)l6->Value--;
  if(Sender==l7s)l7->Value--;
  if(Sender==l8s)l8->Value--;
  if(Sender==l9s)l9->Value--;
  if(Sender==l10s)l10->Value--;
  if(Sender==l11s)l11->Value--;
  if(Sender==l12s)l12->Value--;
  if(Sender==l13s)l13->Value--;
  if(Sender==l14s)l14->Value--;
  if(Sender==l15s)l15->Value--;
  if(Sender==l16s)l16->Value--;
  updateIO(false);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::l1sUpClick(TObject *Sender)
{
  if(Sender==l1s)l1->Value++;
  if(Sender==l2s)l2->Value++;
  if(Sender==l3s)l3->Value++;
  if(Sender==l4s)l4->Value++;
  if(Sender==l5s)l5->Value++;
  if(Sender==l6s)l6->Value++;
  if(Sender==l7s)l7->Value++;
  if(Sender==l8s)l8->Value++;
  if(Sender==l9s)l9->Value++;
  if(Sender==l10s)l10->Value++;
  if(Sender==l11s)l11->Value++;
  if(Sender==l12s)l12->Value++;
  if(Sender==l13s)l13->Value++;
  if(Sender==l14s)l14->Value++;
  if(Sender==l15s)l15->Value++;
  if(Sender==l16s)l16->Value++;
  updateIO(false);
}
//---------------------------------------------------------------------------



void __fastcall TForm1::end1Change(TObject *Sender)
{
   TDateTimePicker *end_date;
   int bird;
   if(Sender==end1){bird=2; end_date=end1;}
   else if(Sender==end2){bird=3; end_date=end2;}
   else if(Sender==end3){bird=4; end_date=end3;}
   else {bird=5; end_date=end4;}

   if(current->Active)
   {
    current->RecNo=bird;
    current->Edit();
    current->FieldValues["stop_training"]=end_date->Date;
    current->Post();
   }
   CheckDates();
}
//---------------------------------------------------------------------------

void TForm1::CheckDates()
{
 Word Year1, Month1, Day1, Year2, Month2, Day2;
 //TDateTimePicker *endDate;

 TDateTime date = Now();
 DecodeDate(date, Year1, Month1, Day1);
 // bird 1
 DecodeDate(start1->Date, Year2, Month2, Day2);
 if(toTrain1->Checked && Month1==Month2 && Day1==Day2 && train1->Down==false)
 {
   train1->Down=true;
   train1Click(this);
 }
 DecodeDate(end1->Date, Year2, Month2, Day2);
 if(toStop1->Checked && Month1==Month2 && Day1==Day2)
 {
   train1->Down=false;
   train1Click(this);
 }
 // bird 2
 DecodeDate(start2->Date, Year2, Month2, Day2);
 if(toTrain2->Checked && Month1==Month2 && Day1==Day2 && train2->Down==false)
 {
   train2->Down=true;
   train2Click(this);
 }
 DecodeDate(end2->Date, Year2, Month2, Day2);
 if(toStop2->Checked && Month1==Month2 && Day1==Day2)
 {
   train2->Down=false;
   train2Click(this);
 }
  // bird 3
 DecodeDate(start3->Date, Year2, Month2, Day2);
 if(toTrain3->Checked && Month1==Month2 && Day1==Day2 && train3->Down==false)
 {
   train3->Down=true;
   train3Click(this);
 }
 DecodeDate(end3->Date, Year2, Month2, Day2);
 if(toStop3->Checked && Month1==Month2 && Day1==Day2)
 {
   train3->Down=false;
   train3Click(this);
 }
 // bird 4
 DecodeDate(start4->Date, Year2, Month2, Day2);
 if(toTrain4->Checked && Month1==Month2 && Day1==Day2 && train4->Down==false)
 {
   train4->Down=true;
   train4Click(this);
 }
 DecodeDate(end4->Date, Year2, Month2, Day2);
 if(toStop4->Checked && Month1==Month2 && Day1==Day2)
 {
   train4->Down=false;
   train4Click(this);
 }
 ID1->Caption=ztBird1->GetStringValue(vBirdName);
 ID2->Caption=ztBird2->GetStringValue(vBirdName);
 ID3->Caption=ztBird3->GetStringValue(vBirdName);
 ID4->Caption=ztBird4->GetStringValue(vBirdName);

 Age1->Caption=(int)Date()-(int)hatch1->Date;
 Age2->Caption=(int)Date()-(int)hatch2->Date;
 Age3->Caption=(int)Date()-(int)hatch3->Date;
 Age4->Caption=(int)Date()-(int)hatch4->Date;
}

void __fastcall TForm1::SaveDefaultClick(TObject *Sender)
{

   BSTR myBstr = AnsiToOLESTR("c:\\SAP\\Sound Analysis.ini");
   ztBirdEngine1->SaveConfiguration(myBstr);
   SysFreeString(myBstr);
}
//---------------------------------------------------------------------------



bool TForm1::LoadDefault()
{
   if(!FileExists("c:\\SAP\\Sound Analysis.ini")) return(0);
   str="c:\\SAP\\Sound Analysis.ini";
   BSTR myBstr = AnsiToOLESTR(str.c_str());
   ztBirdEngine1->LoadConfiguration(myBstr);
   SysFreeString(myBstr);

     sound_source1->Caption=ztBird1->GetStringValue(vAudioSrcDescription);
     sound_source2->Caption=ztBird2->GetStringValue(vAudioSrcDescription);
     sound_source3->Caption=ztBird3->GetStringValue(vAudioSrcDescription);
     sound_source4->Caption=ztBird4->GetStringValue(vAudioSrcDescription);
  return(1);
}


void __fastcall TForm1::hatch1Change(TObject *Sender)
{
   TDateTimePicker *hatch_date;
   int bird;
   if(Sender==hatch1){bird=2; hatch_date=hatch1;}
   else if(Sender==hatch2){bird=3; hatch_date=hatch2;}
   else if(Sender==hatch3){bird=4; hatch_date=hatch3;}
   else {bird=5; hatch_date=hatch4;}
   
   if(current->Active)
   {
    current->RecNo=bird;
    current->Edit();
    current->FieldValues["hatching_date"]=hatch_date->Date;
    current->Post();
   }
   CheckDates();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::BitBtn6Click(TObject *Sender)
{
  keypeckTable->Active=true;
  keypeckTable->Refresh();
  if(playbackDisplay->Visible==true)playbackDisplay->Visible=false;
  else playbackDisplay->Visible=true;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::resetLightsClick(TObject *Sender)
{
  problems1=0; problems2=0; problems3=0; problems4=0;
  warning1->Visible=false;
  warning2->Visible=false;
  warning3->Visible=false;
  warning4->Visible=false;
  messages->Clear(); 
}
//---------------------------------------------------------------------------


void __fastcall TForm1::options1Click(TObject *Sender)
{
 str5=ztBird1->GetStringValue(vBirdName);
 ztBird1->InvokeConfigureDlg();
 str=ztBird1->GetStringValue(vBirdName);
 if(str!=str5)
 {
  recorded[0]=0;
  current->RecNo=2; current->Edit();
  current->FieldValues["bird_ID"]=str;
  current->Post();
 }
 str=ztBird1->GetLongValue(vPeakThreshold);
 thresh1->Value=str.ToInt();
 sound_source1->Caption=ztBird1->GetStringValue(vAudioSrcDescription);
 str=ztBird1->GetStringValue(vBirdName); name[0]=str.ToInt();
 //wchar_t* str1=L"c:\\sap\\tmpSound";
 //ztBird1->SetStringValue(vBirdTempFilePath,str1);
 str=ztBird1->GetStringValue(vBirdTempFilePath);
 if(!DirectoryExists(str))if(!ForceDirectories(str))
                Application->MessageBox("could not create temporary folder -- this is a fatal problem, you must reset it.", NULL, MB_OK);;
 //str1=L"c:\\sap\\in1";
 //ztBird1->SetStringValue(vBirdWaveFilePath,str1);
 //str=ztBird1->GetStringValue(vBirdWaveFilePath);
 if(!DirectoryExists(str))if(!ForceDirectories(str))
                Application->MessageBox("could not create folder to pass sounds too -- this is a fatal problem, you must reset it.", NULL, MB_OK);;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::options2Click(TObject *Sender)
{
   str5=ztBird2->GetStringValue(vBirdName);
   ztBird2->InvokeConfigureDlg();
   str=ztBird2->GetStringValue(vBirdName);
 if(str!=str5)
 {
  recorded[1]=0;
  current->RecNo=3; current->Edit();
  current->FieldValues["bird_ID"]=str;
  current->Post();
 }
   str=ztBird2->GetLongValue(vPeakThreshold);
   thresh2->Value=str.ToInt();
   sound_source2->Caption=ztBird2->GetStringValue(vAudioSrcDescription);
   str=ztBird2->GetStringValue(vBirdName); name[1]=str.ToInt();
   str=ztBird2->GetStringValue(vBirdTempFilePath);

 if(!DirectoryExists(str))if(!ForceDirectories(str))
                Application->MessageBox("could not create temporary folder -- this is a fatal problem, you must reset it.", NULL, MB_OK);;

 str=ztBird2->GetStringValue(vBirdWaveFilePath);
 if(!DirectoryExists(str))if(!ForceDirectories(str))
                Application->MessageBox("could not create folder to pass sounds too -- this is a fatal problem, you must reset it.", NULL, MB_OK);;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::options3Click(TObject *Sender)
{
   str5=ztBird3->GetStringValue(vBirdName);
   ztBird3->InvokeConfigureDlg();
   str=ztBird3->GetStringValue(vBirdName);
 if(str!=str5)
 {
  recorded[2]=0;
  current->RecNo=4; current->Edit();
  current->FieldValues["bird_ID"]=str;
  current->Post();
 }
   str=ztBird3->GetLongValue(vPeakThreshold);
   thresh3->Value=str.ToInt();
   sound_source3->Caption=ztBird3->GetStringValue(vAudioSrcDescription);
   str=ztBird3->GetStringValue(vBirdName); name[2]=str.ToInt();
   str=ztBird3->GetStringValue(vBirdTempFilePath);
 if(!DirectoryExists(str))if(!ForceDirectories(str))
                Application->MessageBox("could not create temporary folder -- this is a fatal problem, you must reset it.", NULL, MB_OK);;

 str=ztBird3->GetStringValue(vBirdWaveFilePath);
 if(!DirectoryExists(str))if(!ForceDirectories(str))
                Application->MessageBox("could not create folder to pass sounds too -- this is a fatal problem, you must reset it.", NULL, MB_OK);;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::options4Click(TObject *Sender)
{
  str5=ztBird4->GetStringValue(vBirdName);
  ztBird4->InvokeConfigureDlg();
  str=ztBird4->GetStringValue(vBirdName);
 if(str!=str5)
 {
  recorded[3]=0;
  current->RecNo=5; current->Edit();
  current->FieldValues["bird_ID"]=str;
  current->Post();
 }
  str=ztBird4->GetLongValue(vPeakThreshold);
  thresh4->Value=str.ToInt();
  sound_source4->Caption=ztBird4->GetStringValue(vAudioSrcDescription);
  str=ztBird4->GetStringValue(vBirdName); name[3]=str.ToInt();
  str=ztBird4->GetStringValue(vBirdTempFilePath);
 if(!DirectoryExists(str))if(!ForceDirectories(str))
                Application->MessageBox("could not create temporary folder -- this is a fatal problem, you must reset it.", NULL, MB_OK);;

 str=ztBird4->GetStringValue(vBirdWaveFilePath);
 if(!DirectoryExists(str))if(!ForceDirectories(str))
                Application->MessageBox("could not create folder to pass sounds too -- this is a fatal problem, you must reset it.", NULL, MB_OK);;

}
//---------------------------------------------------------------------------




void __fastcall TForm1::UpDown1Changing(TObject *Sender, bool &AllowChange)
{
 Quota1->Value=UpDown1->Position;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::UpDown2Changing(TObject *Sender, bool &AllowChange)
{
   Quota2->Value=UpDown2->Position;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::UpDown3Changing(TObject *Sender, bool &AllowChange)
{
  Quota3->Value=UpDown3->Position;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::UpDown4Changing(TObject *Sender, bool &AllowChange)
{
  Quota4->Value=UpDown4->Position;
}
//---------------------------------------------------------------------------
 

void __fastcall TForm1::gain1Change(TObject *Sender)
{
  TztBird* Bird; TNumLab *gainN; TTrackBar *gainTb;
  if(Sender==gain1)      {gainTb=gain1; gainN=gainN1; Bird=m_Birds[0];}
  else if(Sender==gain2) {gainTb=gain2; gainN=gainN2; Bird=m_Birds[1];}
  else if(Sender==gain3) {gainTb=gain3; gainN=gainN3; Bird=m_Birds[2];}
  else if(Sender==gain4) {gainTb=gain4; gainN=gainN4; Bird=m_Birds[3];}
  gainN->Value=gainTb->Position/100.0;
  Bird->SetFloatValue(vPeakGain,gainN->Value);

}
//---------------------------------------------------------------------------



void __fastcall TForm1::ztBird1BirdNotification(TObject *Sender,
      long nEventID)
{
        using namespace ztBirdEngine;
        switch(nEventID)
        {
                case ztbe_PeakCounterTrigger:
                ztBird1->TriggerSound();
                break;
                case ztbe_PlaySoundCompleted:
                // un-mute
                ztBird1->SetLongValue(vMuteBirdData,0);
        }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ztBird2BirdNotification(TObject *Sender,
      long nEventID)
{
        using namespace ztBirdEngine;
        switch(nEventID)
        {
                case ztbe_PeakCounterTrigger:
                ztBird2->TriggerSound();
                break;
                case ztbe_PlaySoundCompleted:
                // un-mute
                ztBird2->SetLongValue(vMuteBirdData,0);
        }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ztBird3BirdNotification(TObject *Sender,
      long nEventID)
{
       using namespace ztBirdEngine;
        switch(nEventID)
        {
                case ztbe_PeakCounterTrigger:
                ztBird3->TriggerSound();
                break;
                case ztbe_PlaySoundCompleted:
                // un-mute
                ztBird3->SetLongValue(vMuteBirdData,0);
        }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::ztBird4BirdNotification(TObject *Sender,
      long nEventID)
{
       using namespace ztBirdEngine;
        switch(nEventID)
        {
                case ztbe_PeakCounterTrigger:
                ztBird4->TriggerSound();
                break;
                case ztbe_PlaySoundCompleted:
                // un-mute
                ztBird4->SetLongValue(vMuteBirdData,0);
        }

}
//---------------------------------------------------------------------------


void __fastcall TForm1::feedback_delay1Change(TObject *Sender)
{
    delay1->Value=feedback_delay1->Position;
    ztBird1->SetLongValue(vPeakTrigger, delay1->Value);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::NI_device_numChange(TObject *Sender)
{
  NInum->Value=NI_device_num->Position;
  NIdeviceNum=NI_device_num->Position;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::mute1Click(TObject *Sender)
{
  TSpeedButton *mt;
  int bird;
  if(Sender==mute1) {mt=mute1; bird=0; }
  else if(Sender==mute2) {mt=mute2; bird=1; }
  else if(Sender==mute3) {mt=mute3; bird=2; }
  else {mt=mute4; bird=3; }

  if(mt->Down){m_Birds[bird]->SetLongValue(vMuteBirdData,1); m_Birds[bird]->SetLongValue(vRecordingDisplay,3); }
  else {m_Birds[bird]->SetLongValue(vMuteBirdData,0); m_Birds[bird]->SetLongValue(vRecordingDisplay,3); }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::refractory1Change(TObject *Sender)
{
  TTrackBar *refractoryTB;
  TNumLab *refractoryNum;
  int bird;
  if(Sender==refractory1){refractoryTB=refractory1; refractoryNum=refractoryNum1; bird=0;}
  else if(Sender==refractory2){refractoryTB=refractory2; refractoryNum=refractoryNum2; bird=1;}
  else if(Sender==refractory3){refractoryTB=refractory3; refractoryNum=refractoryNum3; bird=2;}
  else {refractoryTB=refractory4; refractoryNum=refractoryNum4; bird=3;}

  training_latency[bird]=refractoryTB->Position;
  refractoryNum->Value=(float)refractoryTB->Position/10.0;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::start1Change(TObject *Sender)
{
   TDateTimePicker *start_date;
   int bird;
   if(Sender==start1){bird=2; start_date=start1;}
   else if(Sender==start2){bird=3; start_date=start2;}
   else if(Sender==start3){bird=4; start_date=start3;}
   else {bird=5; start_date=start4;}

   if(current->Active)
   {
    current->RecNo=bird;
    current->Edit();
    current->FieldValues["start_training"]=start_date->Date;
    current->Post();
   }
   CheckDates();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::trigger1Click(TObject *Sender)
{
  TSpeedButton *trigger;
  TLabel *tl;
  TTrackBar *feedback_delay;
  TNumLab *delayN;
  if(Sender==trigger1) {trigger=trigger1; tl=tl1; feedback_delay=feedback_delay1; delayN=delay1; }
  else if(Sender==trigger2) {trigger=trigger2;tl=tl2; feedback_delay=feedback_delay2; delayN=delay2; }
  else if(Sender==trigger3) {trigger=trigger3;tl=tl3; feedback_delay=feedback_delay3; delayN=delay3; }
  else {trigger=trigger4;tl=tl4; feedback_delay=feedback_delay4; delayN=delay4; }
  if(trigger->Down)
  {trigger->Caption="On"; tl->Visible=true;feedback_delay->Visible=true;delayN->Visible=true;}
  else
  {trigger->Caption="Off"; tl->Visible=false;feedback_delay->Visible=false;delayN->Visible=false;}
}
//---------------------------------------------------------------------------


void __fastcall TForm1::songList1Click(TObject *Sender)
{
   if(songList1->ItemIndex>-1){
   SetBirdString(ztBird1,vBirdWaveFile,songList1->Items->Strings[songList1->ItemIndex].c_str());
   song1->Caption=ExtractFileName(songList1->Items->Strings[songList1->ItemIndex]); }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::songList1KeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
if(Key==46 || Key==110)  songList1->Items->Delete(songList1->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::songList2KeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
if(Key==46 || Key==110)  songList2->Items->Delete(songList2->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::songList3KeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
if(Key==46 || Key==110)  songList3->Items->Delete(songList3->ItemIndex);        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::songList4KeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
if(Key==46 || Key==110)  songList4->Items->Delete(songList4->ItemIndex);        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::songList4Click(TObject *Sender)
{
   if(songList4->ItemIndex>-1){
   SetBirdString(ztBird4,vBirdWaveFile,songList4->Items->Strings[songList4->ItemIndex].c_str());
   song4->Caption=ExtractFileName(songList4->Items->Strings[songList4->ItemIndex]);   }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::songList3Click(TObject *Sender)
{
   if(songList3->ItemIndex>-1){
   SetBirdString(ztBird3,vBirdWaveFile,songList3->Items->Strings[songList3->ItemIndex].c_str());
   song3->Caption=ExtractFileName(songList3->Items->Strings[songList3->ItemIndex]);  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::songList2Click(TObject *Sender)
{
   if(songList2->ItemIndex>-1){
   SetBirdString(ztBird2,vBirdWaveFile,songList2->Items->Strings[songList2->ItemIndex].c_str());
   song2->Caption=ExtractFileName(songList2->Items->Strings[songList2->ItemIndex]); }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::chan1_oddsChange(TObject *Sender)
{
chan1_odds_val->Value=1.0/(float)chan1_odds->Position;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::chan2_oddsChange(TObject *Sender)
{
chan2_odds_val->Value=1.0/(float)chan2_odds->Position;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::chan3_oddsChange(TObject *Sender)
{
chan3_odds_val->Value=1.0/(float)chan3_odds->Position;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::chan4_oddsChange(TObject *Sender)
{
chan4_odds_val->Value=1.0/(float)chan4_odds->Position;

}
//---------------------------------------------------------------------------

void TForm1::SaveKeyPecks(int animal, int peckNo, AnsiString theDate, AnsiString theTime)
{

 str="INSERT INTO key_pecks";
 str+="(bird_ID, key_pecked, date, time) VALUES (";
 str+= animal; str+=",";
 str+= peckNo; str+=", '";
 str+= theDate; str+="','";
 str+= theTime;
 str+="')";
 MyCommand1->SQL->Clear();
 MyCommand1->SQL->Add(str);
 MyCommand1->Execute(1);
 //MyConnection1->Commit();
 MyCommand1->UnPrepare();
}


void __fastcall TForm1::empty_tableClick(TObject *Sender)
{
  if (MessageDlg("Are you sure? All records will be deleted", mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
  {
  keypeckTable->Active=false;
  keypeckTable->EmptyTable();
  keypeckTable->Active=true;
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TabSheet7Enter(TObject *Sender)
{
    keypeckTable->Active=true;
    keypeckTable->Refresh();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::ToExelClick(TObject *Sender)
{
  keypeckTable->Active=true;
  xlReport1->Report();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::songList1aClick(TObject *Sender)
{
   if(songList1a->ItemIndex>-1){
   SetBirdString(ztBird1,vBirdWaveFile,songList1a->Items->Strings[songList1a->ItemIndex].c_str());
   song1->Caption=ExtractFileName(songList1a->Items->Strings[songList1a->ItemIndex]); }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::songList1aKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
if(Key==46 || Key==110)  songList1a->Items->Delete(songList1a->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::load1aClick(TObject *Sender)
{
        HandleLoadWav(0);
        song1->Caption=ExtractFileName(ztBird1->GetStringValue(vBirdWaveFile));
        songList1a->Items->Add(ztBird1->GetStringValue(vBirdWaveFile));
        songList1a->ItemIndex=0;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::effect_duration1Change(TObject *Sender)
{
  effect_durationNum1->Value=effect_duration1->Position;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::effect_duration2Change(TObject *Sender)
{
effect_durationNum2->Value=effect_duration2->Position;        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::effect_duration3Change(TObject *Sender)
{
effect_durationNum3->Value=effect_duration3->Position;        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::effect_duration4Change(TObject *Sender)
{
effect_durationNum4->Value=effect_duration4->Position;        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::effect_duration5Change(TObject *Sender)
{
effect_durationNum5->Value=effect_duration5->Position;        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::effect_duration6Change(TObject *Sender)
{
effect_durationNum6->Value=effect_duration6->Position;        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::effect_duration7Change(TObject *Sender)
{
effect_durationNum7->Value=effect_duration7->Position;        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::effect_duration8Change(TObject *Sender)
{
effect_durationNum8->Value=effect_duration8->Position;        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::load2aClick(TObject *Sender)
{
        HandleLoadWav(1);
        song2->Caption=ExtractFileName(ztBird2->GetStringValue(vBirdWaveFile));
        songList2a->Items->Add(ztBird2->GetStringValue(vBirdWaveFile));
        songList2a->ItemIndex=0;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::load3aClick(TObject *Sender)
{
        HandleLoadWav(2);
        song3->Caption=ExtractFileName(ztBird3->GetStringValue(vBirdWaveFile));
        songList3a->Items->Add(ztBird3->GetStringValue(vBirdWaveFile));
        songList3a->ItemIndex=0;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::load4aClick(TObject *Sender)
{
        HandleLoadWav(3);
        song4->Caption=ExtractFileName(ztBird4->GetStringValue(vBirdWaveFile));
        songList4a->Items->Add(ztBird4->GetStringValue(vBirdWaveFile));
        songList4a->ItemIndex=0;
}
//---------------------------------------------------------------------------





void __fastcall TForm1::effect_delay1Change(TObject *Sender)
{
  effect_delayNum1->Value=effect_delay1->Position;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::effect_delay2Change(TObject *Sender)
{
effect_delayNum2->Value=effect_delay2->Position;        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::effect_delay3Change(TObject *Sender)
{
effect_delayNum3->Value=effect_delay3->Position;        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::effect_delay4Change(TObject *Sender)
{
effect_delayNum4->Value=effect_delay4->Position;        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::effect_delay5Change(TObject *Sender)
{
effect_delayNum5->Value=effect_delay5->Position;        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::effect_delay6Change(TObject *Sender)
{
effect_delayNum6->Value=effect_delay6->Position;        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::effect_delay7Change(TObject *Sender)
{
effect_delayNum7->Value=effect_delay7->Position;        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::effect_delay8Change(TObject *Sender)
{
effect_delayNum8->Value=effect_delay8->Position;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::songList2aClick(TObject *Sender)
{
if(songList2a->ItemIndex>-1){
   SetBirdString(ztBird2,vBirdWaveFile,songList2a->Items->Strings[songList2a->ItemIndex].c_str());
   song2->Caption=ExtractFileName(songList2a->Items->Strings[songList2a->ItemIndex]); }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::songList3aClick(TObject *Sender)
{
if(songList3a->ItemIndex>-1){
   SetBirdString(ztBird3,vBirdWaveFile,songList3a->Items->Strings[songList3a->ItemIndex].c_str());
   song3->Caption=ExtractFileName(songList3a->Items->Strings[songList3a->ItemIndex]); }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::songList4aClick(TObject *Sender)
{
if(songList4a->ItemIndex>-1){
   SetBirdString(ztBird4,vBirdWaveFile,songList4a->Items->Strings[songList4a->ItemIndex].c_str());
   song4->Caption=ExtractFileName(songList4a->Items->Strings[songList4a->ItemIndex]); }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::songList2aKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
if(Key==46 || Key==110)  songList2a->Items->Delete(songList2a->ItemIndex);        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::songList3aKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
if(Key==46 || Key==110)  songList3a->Items->Delete(songList3a->ItemIndex);        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::songList4aKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
if(Key==46 || Key==110)  songList4a->Items->Delete(songList4a->ItemIndex);        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCloseQuery(TObject *Sender, bool &CanClose)
{
   ExitClick(this);
}
//---------------------------------------------------------------------------

void TForm1::Handle_training(int bird) // 4 bird = 0 to 3
{
    // we first declare pointers to GUI objects that set parameters for each bird.

                // These GUI checkboxes can control the following on/off states for each bird:
     TCheckBox  *chan_passive,    // in passive playbacks mode, sounds are played in a non-operant regimen
                *enable_quota,    // only a limited amount of sounds are played per session
                *random_switch,   // one of several sound is chosen randomly and played, either passively or operantly.
                *key_effect_a,    // this means that activating detector (key) a, results in an effector (e.g., light-off) activation
                *key_effect_b,    // same, for the second detector
                *playback_mute_rec, // mute recorder during playback (prevent recording of playback sound
                *passive_quota_check;

     TRadioGroup *detector_a_list, *detector_b_list, *passive_list;
                      // trackbars and numlabs are used to set parameter values for each bird:
     TTrackBar  *chan_odds,        // in the chan_passive mode, this parameter set the probability of playbacks
                *effect_a_duration,  // in the discrimination mode, this parameter set the duration of effector activation
                *effect_a_delay,     // in the discrimination mode, this parameter set the delay between key-peck and effector activation
                *effect_b_duration,  // in the discrimination mode, this parameter set the duration of effector activation
                *effect_b_delay;     // in the discrimination mode, this parameter set the delay between key-peck and effector activation

     TNumLab    *affector_port,   // the National Instrument port of the effector
                *affector_line,   // the National Instrument line of the effector
                *detector1_port,  // the National Instrument port of the first detector
                *detector1_line,  // the National Instrument line of the first detector
                *detector2_port,  // the National Instrument port of the second detector
                *detector2_line,  // the National Instrument line of the second detector
                *playbacks_played,// the number of playbacks already played to the bird during a session
                *playbacks_quota, // the quota of playbacks during a session
                *passive_played,  // the quota for passive song playbacks
                *passive_quota;      // the actual number of passive song playbacks

     TShape
                *box,             // the rectangle representing the box will apear blue during an effector activation
                *detector_1,      // the circle display representing each detector will appear red during detector activation
                *detector_2;      // circle display of the second detector

     TListBox *songList;         // there are two listboxes per bird with sound playbacks to pick from

     int detector1=bird*2;  // we have two detectors per bird, hense the funny indexing method that relates a detector to a bird
     int detector2=bird*2+1;
     
     switch(bird)  // this switch link between the GUI controls of each bird to the
     {             // current bird. Note that you should not change these values unless you change GUI


       case 0: detector_a_list=detect1_list; detector_b_list=detect2_list; passive_list=passive1_list;
               affector_port=p4; affector_line=l4; box=Box1;
               detector1_port=p1; detector1_line=l1; detector2_port=p2; detector2_line=l2;
               detector_1=key1; detector_2=key2; chan_passive=chan1_passive; chan_odds=chan1_odds;
               playbacks_played=Playbacks1; playbacks_quota=Quota1; enable_quota=enable_quota1;
               random_switch=random_switch1; key_effect_a=key_effect1; key_effect_b=key_effect2;
               effect_a_duration=effect_duration1; effect_a_delay=effect_delay1;
               effect_b_duration=effect_duration2; effect_b_delay=effect_delay2;
               playback_mute_rec=playback_mute_rec1;
               passive_played=passive_played1; passive_quota=passive_quota1; passive_quota_check=passive_quota_check1;
               break;
       case 1: detector_a_list=detect3_list; detector_b_list=detect4_list; passive_list=passive2_list;
               affector_port=p8; affector_line=l8; box=Box2;
               detector1_port=p5; detector1_line=l5; detector2_port=p6; detector2_line=l6;
               detector_1=key3; detector_2=key4; chan_passive=chan2_passive; chan_odds=chan2_odds;
               playbacks_played=Playbacks2; playbacks_quota=Quota2; enable_quota=enable_quota2;
               random_switch=random_switch2; key_effect_a=key_effect3; key_effect_b=key_effect4;
               effect_a_duration=effect_duration3; effect_a_delay=effect_delay3;
               effect_b_duration=effect_duration4; effect_b_delay=effect_delay4;
               playback_mute_rec=playback_mute_rec2;
               passive_played=passive_played2; passive_quota=passive_quota2; passive_quota_check=passive_quota_check2;
               break;
       case 2: detector_a_list=detect5_list; detector_b_list=detect6_list; passive_list=passive3_list;
               affector_port=p12; affector_line=l12; box=Box3;
               detector1_port=p9; detector1_line=l9; detector2_port=p10; detector2_line=l10;
               detector_1=key5; detector_2=key6; chan_passive=chan3_passive; chan_odds=chan3_odds;
               playbacks_played=Playbacks3; playbacks_quota=Quota3; enable_quota=enable_quota3;
               random_switch=random_switch3; key_effect_a=key_effect5; key_effect_b=key_effect6;
               effect_a_duration=effect_duration5; effect_a_delay=effect_delay5;
               effect_b_duration=effect_duration6; effect_b_delay=effect_delay6;
               playback_mute_rec=playback_mute_rec3;
               passive_played=passive_played3; passive_quota=passive_quota3; passive_quota_check=passive_quota_check3;
               break;
       case 3: detector_a_list=detect7_list; detector_b_list=detect8_list; passive_list=passive4_list;
               affector_port=p16; affector_line=l16; box=Box4;
               detector1_port=p13; detector1_line=l13; detector2_port=p14; detector2_line=l14;
               detector_1=key7; detector_2=key8; chan_passive=chan4_passive; chan_odds=chan4_odds;
               playbacks_played=Playbacks4; playbacks_quota=Quota4; enable_quota=enable_quota4;
               random_switch=random_switch4; key_effect_a=key_effect7; key_effect_b=key_effect8;
               effect_a_duration=effect_duration7; effect_a_delay=effect_delay7;
               effect_b_duration=effect_duration8; effect_b_delay=effect_delay8;
               playback_mute_rec=playback_mute_rec4;
               passive_played=passive_played4; passive_quota=passive_quota4; passive_quota_check=passive_quota_check4;
               break;
     }

      // lets go...
      Latent[bird]++; // latent period is the minimum time gap between playbacks, when Latent[] is too small, no playbacks will occur
      if(key_effect_a->Checked || key_effect_b->Checked || passive_effect->Checked)// in this mode, activating detectors can cause different playbacks and different post-playbacks effects
      {                          // use this mode for song discrimination task and for go-no-go tasks.
                                 // activate an effector if:
         if(  !effector_delay[bird]  // effector delay time has ellapsed
              && !effector_on[bird]  // and effector state is still off
              && effector_timer[bird]  // and effector timer is not zero (effect has been triggered)
           )  
         {
             zt_DIG_Out_Line(NIdeviceNum,affector_port->Value,affector_line->Value,1); // activate effector
             box->Brush->Color=clBlue; // GUI display of the effect
             effector_on[bird]=true;   // indicates that the effector is now on
         }
         if(effector_on[bird] && !effector_timer[bird]) // if effector is on and its timer is zero -- stop the effect
         {
           zt_DIG_Out_Line(NIdeviceNum,affector_port->Value,affector_line->Value,0); // turn off effector
           box->Brush->Color=clTeal;
           effector_on[bird]=false;
         }
         if(effector_delay[bird]) effector_delay[bird]--;  // count down the delay of the effect
         else if(effector_timer[bird]) effector_timer[bird]--;  // count down the duration of the effect
      }
      if(training)  // if ' enable operant training' is checked, see in the 'training method' page.
      {

       // first check the first detectors (keys) position
        //if(bird==2)InState=fake1->Down;   // only for testing it...
        zt_DIG_In_Line(NIdeviceNum, detector1_port->Value, detector1_line->Value, &InState);
        if(InState){ detector_1->Brush->Color=clYellow; peck[detector1]=false;} // peck[] indicate the detector position
        else {peck[detector1]=true; detector_1->Brush->Color=clRed; }
        // check the second detector
      //  if(bird==2)InState=fake2->Down;  // only for testing it...
      zt_DIG_In_Line(NIdeviceNum,detector2_port->Value, detector2_line->Value,&InState);

        if(InState){ detector_2->Brush->Color=clYellow; peck[detector2]=false;}
        else {peck[detector2]=true; detector_2->Brush->Color=clRed; }
      }
      // we now decide if passive playback should occur
      bool passivePlay=false;
      if(chan_passive->Checked && hour>=random_start->Value && hour<=random_stop->Value && ( !passive_quota_check->Checked || passive_played->Value<=passive_quota->Value) )
        if(random(chan_odds->Position)==0 && hour>=random_start->Value && hour<=random_stop->Value)
           passivePlay=true;
      // we now decide if playback should occur
      if(Latent[bird]>training_latency[bird])
       if( passivePlay || (   (peck[detector1] || peck[detector2]) && (playbacks_played->Value<playbacks_quota->Value || !enable_quota->Checked) ) )
      {
           if(passivePlay) {        songList=set_playback(bird,    passive_list->ItemIndex, random_switch->Checked); }
           else if(peck[detector1]) songList=set_playback(bird, detector_a_list->ItemIndex, random_switch->Checked);   // link the playbacks of detector 1 to the channel
           else if(peck[detector2]) songList=set_playback(bird, detector_b_list->ItemIndex, random_switch->Checked); // link the playbacks of detector 2 to the channel

           // if first detector is linked with effector and that first detector is now activated
           if(key_effect_a->Checked && peck[detector1]                // the bird pecked a 'loaded' key
               && !effector_timer[bird]                               // effect is not on already
               && songList->ItemIndex>=gonogo_thresh->Position) // go no go
                 // The bird has activated a detector that should (eventually) trigger an effect
           {
              effector_delay[bird]=effect_a_delay->Position*Timer_freq; // activate the effector delay timer, how long before the effect start
              effector_timer[bird]=effect_a_duration->Position*Timer_freq;  // activate the effector activation timer, how long should the effect continue
           }

           // same for the second detector
           if(key_effect_b->Checked && peck[detector2]  // the bird pecked a 'loaded' key
               && !effector_timer[bird] // same for the second detector
               && songList->ItemIndex>=gonogo_thresh->Position) // go no go
           {
              effector_delay[bird]=effect_b_delay->Position*Timer_freq;
              effector_timer[bird]=effect_b_duration->Position*Timer_freq;
           }

           if(passive_effect->Checked && passivePlay  // the bird pecked a 'loaded' key
               && !effector_timer[bird] // same for the second detector
               && songList->ItemIndex>=gonogo_thresh->Position) // go no go
           {
              effector_delay[bird]=effect_b_delay->Position*Timer_freq;
              effector_timer[bird]=effect_b_duration->Position*Timer_freq;
           }

           if(playback_mute_rec->Checked) m_Birds[bird]->SetLongValue(vMuteBirdData,1); // avoid recording playbacks
           m_Birds[bird]->TriggerSound();  // play the sound
           Latent[bird]=0;  // reset the refractory period until the next playback
           if(passivePlay) passive_played->Value++;
           else playbacks_played->Value++;
           int pki;
           if(peck[detector1]==0 && peck[detector2]==0)pki=0;
           else if(peck[detector1]==1 && peck[detector2]==0)pki=1;
           else if(peck[detector1]==0 && peck[detector2]==1)pki=2;
           else pki=-1;
           SaveKeyPecks(name[bird], pki , date, tm);  // save events to the table (need more work here)
           //SaveKeyPecks(name[bird], (int)peck[bird] , date, tm);
      }
}



TListBox* TForm1::set_playback(int bird, bool song_list, bool random_switch)
{
   TLabel *now_playing;
   TListBox *songList;

   switch(bird)
   {
     case 0: now_playing=song1; if(song_list) songList=songList1a; else songList=songList1; break;
     case 1: now_playing=song2; if(song_list) songList=songList2a; else songList=songList2; break;
     case 2: now_playing=song3; if(song_list) songList=songList3a; else songList=songList3; break;
     case 3: now_playing=song4; if(song_list) songList=songList4a; else songList=songList4; break;
   }

   if(random_switch)  // chose a song from the list if so indicated
   {
     int sw=random(songList->Items->Count);
     songList->ItemIndex=sw;
   }

   if(songList->ItemIndex==-1)return(songList);  // no can do...
   SetBirdString(m_Birds[bird],vBirdWaveFile,songList->Items->Strings[songList->ItemIndex].c_str());
   now_playing->Caption  =   ExtractFileName(songList->Items->Strings[songList->ItemIndex]);
   return(songList);
}




void __fastcall TForm1::gonogo_threshChange(TObject *Sender)
{
  gonogo_num->Value=gonogo_thresh->Position+1;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::passive_durationChange(TObject *Sender)
{
  passive_durationN->Value=passive_duration->Position;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::passive_delayChange(TObject *Sender)
{
passive_delayN->Value=passive_delay->Position;        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::change_databaseClick(TObject *Sender)
{
    showDatabases();
    Application->CreateForm(__classid(TtableDialog), &tableDialog);

    if (tableDialog->ShowModal() == mrOk)
        Change_Database(tableDialog->ListBox1->Items->Strings[tableDialog->ListBox1->ItemIndex]);
    database->Caption= tableDialog->ListBox1->Items->Strings[tableDialog->ListBox1->ItemIndex];
}
//---------------------------------------------------------------------------

void TForm1::showDatabases()
 {
    MyQuery1->SQL->Clear();
    AnsiString str="SHOW DATABASES;";
    MyQuery1->SQL->Add(str);
    MyQuery1->Execute();
    MyQuery1->First();
 }

 void TForm1::Change_Database(AnsiString name)
{
  MyConnection1->Database=name;
}

void TForm1::create_database(AnsiString name)
 {
     str="CREATE DATABASE ";
     str+=name;
     MyCommand1->SQL->Clear();
     MyCommand1->SQL->Add(str);
     MyCommand1->Execute(1);
 }

bool TForm1::database_exists(AnsiString database_name)
{
  showDatabases();
  MyQuery1->First();
  for (int i=0; i < MyQuery1->RecordCount; i++)
  {
     if(database_name==MyQuery1->Fields->Fields[0]->AsString)return(true);
     MyQuery1->Next();
  }
  return(false);
}


void TForm1::create_birds()
{
  current->Active=false;
  DeleteFile("c:\\sap\\birds.txt");
  str="CREATE TABLE `birds` ( `num` tinyint(4) NOT NULL auto_increment,";
  str+="`bird_ID` smallint(6) NOT NULL default '0', `hatching_date` date NOT NULL default '0000-00-00',";
  str+="`start_training` date NOT NULL default '0000-00-00',  `stop_training` date NOT NULL default '0000-00-00',";
  str+="`training` tinyint(1) NOT NULL default '0',   `pecks` smallint(6) NOT NULL default '0',";
  str+="`song_quota` smallint(6) NOT NULL default '0', `input_folder` varchar(50) NOT NULL default '',";
  str+="`sound_folder_size` smallint(6) NOT NULL default '0', `output_folder_sound` varchar(50) NOT NULL default '',";
  str+="`output_folder_features` varchar(50) NOT NULL default '', `syllable_table` varchar(50) NOT NULL default '',";
  str+="`entropy_thresh` smallint(6) NOT NULL default '0', `deriv_thresh` smallint(6) NOT NULL default '0',";
  str+="`syllable_duration_reject` smallint(6) NOT NULL default '0', `bout_duration_reject` smallint(6) NOT NULL default '0',";
  str+="`number_of_syllables_reject` smallint(3) unsigned NOT NULL default '0', `display_contrast` smallint(3) unsigned NOT NULL default '0',";
  str+="`update` tinyint(1) NOT NULL default '0', `active_or_slave` tinyint(1) NOT NULL default '0', PRIMARY KEY  (`num`))";
  MyCommand1->SQL->Clear();
  MyCommand1->SQL->Add(str);
  MyCommand1->Execute(1);

  fstream inout("c:\\sap\\birds.txt");
  str="1 \t 10 \t 2005-01-20 \t 2005-05-16 \t 2005-06-16 \t 0  \t 0 \t 10 \tc:\\\\SAP\\\\in0\\\\ \t 0 \tc:\\\\SAP\\\\out0\\\\ \trecs0_features \trecs0 \t 282 \t 116 \t 300 \t 300 \t 0 \t 255 \t 0 \t 0";
  inout << str.c_str() << endl;
  str="2 \t 11 \t 2005-01-20 \t 2005-05-16 \t 2005-06-16 \t 0  \t 0 \t 10 \tc:\\\\SAP\\\\in1\\\\ \t 0 \tc:\\\\SAP\\\\out1\\\\ \trecs1_features \trecs1 \t 282 \t 116 \t 300 \t 300 \t 0 \t 255 \t 0 \t 0";
  inout << str.c_str() << endl;
  str="3 \t 12 \t 2005-01-20 \t 2005-05-16 \t 2005-06-16 \t 0  \t 0 \t 10 \tc:\\\\SAP\\\\in2\\\\ \t 0 \tc:\\\\SAP\\\\out2\\\\ \trecs2_features \trecs2 \t 282 \t 116 \t 300 \t 300 \t 0 \t 255 \t 0 \t 0";
  inout << str.c_str() << endl;
  str="4 \t 13 \t 2005-01-20 \t 2005-05-16 \t 2005-06-16 \t 0  \t 0 \t 10 \tc:\\\\SAP\\\\in3\\\\ \t 0 \tc:\\\\SAP\\\\out3\\\\ \trecs3_features \trecs3 \t 282 \t 116 \t 300 \t 300 \t 0 \t 255 \t 0 \t 0";
  inout << str.c_str() << endl;
  str="5 \t 14 \t 2005-01-20 \t 2005-05-16 \t 2005-06-16 \t 0  \t 0 \t 10 \tc:\\\\SAP\\\\in4\\\\ \t 0 \tc:\\\\SAP\\\\out4\\\\ \trecs4_features \trecs4 \t 282 \t 116 \t 300 \t 300 \t 0 \t 255 \t 0 \t 0";
  inout << str.c_str() << endl;

  str="LOAD DATA INFILE 'c:/sap/birds.txt' into table birds";
  MyCommand1->SQL->Clear();
  MyCommand1->SQL->Add(str);
  MyCommand1->Execute(1);
  current->Active=true;
  current->First();
  DeleteFile("c:\\sap\\birds.txt");
}


bool TForm1::table_exists(AnsiString table_name)
{
  showTables();
  MyQuery1->First();
  for (int i=0; i < MyQuery1->RecordCount; i++)
  {
     if(table_name==MyQuery1->Fields->Fields[0]->AsString)return(true);
     MyQuery1->Next();
  }
  return(false);
}


 void TForm1::showTables()
 {
    MyQuery1->SQL->Clear();
    AnsiString str="SHOW TABLES;";
    MyQuery1->SQL->Add(str);
    MyQuery1->Execute();
    MyQuery1->First();
 }


 void TForm1::create_keypecks()
 {
  keypeckTable->Active=false;
  str="CREATE TABLE `key_pecks` (";
  str+="`bird_ID` smallint(6) default '0',";
  str+="`key_pecked` tinyint(4) default '0',";
  str+="`date` date NOT NULL default '0000-00-00',";
  str+="`time` time NOT NULL default '00:00:00')";
  MyCommand1->SQL->Clear();
  MyCommand1->SQL->Add(str);
  MyCommand1->Execute(1);
  keypeckTable->Active=true;
 }


 void TForm1::create_nidaq()
 {
  NIDAQ->Active=false;
  DeleteFile("c:\\sap\\birds.txt");
  str="CREATE TABLE `nidaq`(";
  str+="`recnum` int(11) NOT NULL auto_increment,";
  str+="`device_ID` tinyint(4) default '0',";
  str+="`port` tinyint(4) default '0',";
  str+="`line` tinyint(4) default '0',";
  str+="PRIMARY KEY  (`recnum`))";
  MyCommand1->SQL->Clear();
  MyCommand1->SQL->Add(str);
  MyCommand1->Execute(1);

  fstream inout("c:\\sap\\birds.txt");
  str="1 \t 1 \t 0 \t 0";
  inout << str.c_str() << endl;
  str="2 \t 1 \t 0 \t 1";
  inout << str.c_str() << endl;
  str="3 \t 1 \t 0 \t 2";
  inout << str.c_str() << endl;
  str="4 \t 1 \t 2 \t 4";
  inout << str.c_str() << endl;
  str="5 \t 1 \t 0 \t 4";
  inout << str.c_str() << endl;
  str="6 \t 1 \t 0 \t 5";
  inout << str.c_str() << endl;
  str="7 \t 1 \t 0 \t 6";
  inout << str.c_str() << endl;
  str="8 \t 1 \t 2 \t 7";
  inout << str.c_str() << endl;
  str="9 \t 1 \t 1 \t 0";
  inout << str.c_str() << endl;
  str="10 \t 1 \t 1 \t 1";
  inout << str.c_str() << endl;
  str="11 \t 1 \t 1 \t 2";
  inout << str.c_str() << endl;
  str="12 \t 1 \t 1 \t 3";
  inout << str.c_str() << endl;
  str="13 \t 1 \t 1 \t 4";
  inout << str.c_str() << endl;
  str="14 \t 1 \t 1 \t 5";
  inout << str.c_str() << endl;
  str="15 \t 1 \t 1 \t 6";
  inout << str.c_str() << endl;
  str="16 \t 1 \t 1 \t 7";
  inout << str.c_str() << endl;
  str="LOAD DATA INFILE 'c:/sap/birds.txt' into table nidaq";
  MyCommand1->SQL->Clear();
  MyCommand1->SQL->Add(str);
  MyCommand1->Execute(1);
  NIDAQ->Active=true;
  DeleteFile("c:\\sap\\birds.txt");
}
void __fastcall TForm1::rand_start_btnDownClick(TObject *Sender)
{
  random_start->Value--;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::rand_start_btnUpClick(TObject *Sender)
{
   random_start->Value++;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::rand_stop_btnDownClick(TObject *Sender)
{
random_stop->Value--;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::rand_stop_btnUpClick(TObject *Sender)
{
 random_stop->Value++;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::UpDown5Changing(TObject *Sender, bool &AllowChange)
{
  passive_quota1->Value=UpDown5->Position;        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::UpDown6Changing(TObject *Sender, bool &AllowChange)
{
   passive_quota2->Value=UpDown6->Position;               
}
//---------------------------------------------------------------------------

void __fastcall TForm1::UpDown7Changing(TObject *Sender, bool &AllowChange)
{
   passive_quota3->Value=UpDown7->Position;               
}
//---------------------------------------------------------------------------

void __fastcall TForm1::UpDown8Changing(TObject *Sender, bool &AllowChange)
{
    passive_quota4->Value=UpDown8->Position;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CheckBox1Click(TObject *Sender)
{
  enable_quota1->Checked=CheckBox1->Checked;
  if(enable_quota1->Checked) Quota1->Visible=true; else Quota1->Visible=false;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::CheckBox2Click(TObject *Sender)
{
    enable_quota2->Checked=CheckBox2->Checked;
    if(enable_quota2->Checked) Quota2->Visible=true; else Quota2->Visible=false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CheckBox3Click(TObject *Sender)
{
   enable_quota3->Checked=CheckBox3->Checked;
   if(enable_quota3->Checked) Quota3->Visible=true; else Quota3->Visible=false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CheckBox4Click(TObject *Sender)
{
    enable_quota4->Checked=CheckBox4->Checked;
    if(enable_quota4->Checked) Quota4->Visible=true; else Quota4->Visible=false;
}
//---------------------------------------------------------------------------

