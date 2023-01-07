//---------------------------------------------------------------------------
#define ZT_INCLUDE_BORLAND_HELPERS 1
#include <fstream.h>
#include "vcl.h"
#pragma hdrstop
#include "CWavIterator.h"
#include "david.h"
#include "table_dialog.h"
#include "CZT_BirdFramework.h"
#include "animals.h"

const SELDIRHELP = 1000;    // needed for SelectDirectory call
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ZTBIRDCONTROLPACKLib_OCX"
#pragma link "MyAccess"
#pragma link "CSPIN"
#pragma link "CSPIN"
#pragma link "DBAccess"
#pragma link "MemDS"
#pragma link "ZTBIRDCONTROLPACKLib_OCX"
#pragma link "SDL_NumLab"
#pragma link "MyDacVcl"
#pragma resource "*.dfm"
#define not_in_session -1




TForm1 *Form1;
using namespace ztBirdEngine;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{

  Config config((string)"c:/sap/db.properties");
   string schema = config.pString((string)"schema");
   string user = config.pString((string)"username");
   string pwd = config.pString((string)"password");
   const char* schemaStr = schema.c_str();
   const char* pwdStr = pwd.c_str();
   const char* userStr = user.c_str();
   wchar_t schemaUnicodeStr[80];
   wchar_t pwdUnicodeStr[80];
   wchar_t userUnicodeStr[80];
   MultiByteToWideChar(CP_ACP, 0, schemaStr, -1, schemaUnicodeStr, 1024);
   MultiByteToWideChar(CP_ACP, 0, pwdStr, -1, pwdUnicodeStr, 1024);
   MultiByteToWideChar(CP_ACP, 0, userStr, -1, userUnicodeStr, 1024);
   MyConnection1->Database = schemaUnicodeStr;
   MyConnection1->Username = userUnicodeStr;
   MyConnection1->Password=pwdUnicodeStr;
   /**/
  try {
	  MyConnection1->Connect();
  }catch(Exception & exception) {
	  MyConnection1->LoginPrompt=true;//Password= pwdUnicodeStr;
	 // MyConnection1->Connect();
  }
//  MyConnection1->Database= schemaUnicodeStr;



  //try{MyConnection1->Connect();  }
  //catch(...){MyConnection1->LoginPrompt=true; MyConnection1->Connect();}
  Timer_freq=10; // the timer tick 10 times a second.
  update_check=true; // allow the click channel checkbox to update
  io_folders_enable=false;
}
//---------------------------------------------------------------------------


inline void TForm1::SetBirdString(TztBird* pBird,int nID,char* string)
{
	 BSTR myBstr = AnsiToOLESTR(string);
     pBird->SetStringValue(nID,myBstr);
     SysFreeString(myBstr);
}

//---------------------------------------------------------------------------

inline void TForm1::SetEngineString(int nID,char* string)
{
    BSTR myBstr = AnsiToOLESTR(string);
    ztBirdEngine1->SetStringValue(nID,myBstr);
    SysFreeString(myBstr);
}

//---------------------------------------------------------------------------

void TForm1::HandleMonitorDisplay(TComboBox* pBox,int nBird)
{
   m_Birds[nBird]->SetLongValue(vMonitorDisplay,pBox->ItemIndex);
}

//---------------------------------------------------------------------------

void TForm1::InitBirds()
{
     int nState;
     long lThreshold;

     //minimum_recording_duration->Position = 1;
     SoundDurTbChange(this);
     bool st;
     if(current->Active)
     {
       current->First();
       for(int i=0;i<10;i++)
       {
        current->Next();
        st=current->FieldValues["training"];
        if(st)train[i]->Down=true;
        else train[i]->Down=false;
        Handle_training_state(i);}
     }
     SoundDurTbChange(this); // setting the number of peaks for accepting a file
     minimum_recording_durationChange(this); // setting the minimum duration of recording session after threshold is met

	 CZT_OCX_BirdProps props;
	 for(int i=0;i<10;i++)
	 {
		props.RetrieveAllValues(m_Birds[i]);
		props.m_lClipThreshold=32767;
		props.PutAllValues(m_Birds[i]);
   //     song_list[i*2]->Items->Add(m_Birds[i]->GetStringValue(vBirdWaveFile));
   //     song_list[i*2]->ItemIndex=0;
     }
}

//---------------------------------------------------------------------------


void TForm1::Disable_folder_changing()
{
   SaveDefault->Enabled=false;  loadConfig->Enabled=false;  btnStartEngine->Enabled=false;
   btnStopEngine->Enabled=true;
   /*options1->Visible=false; options2->Visible=false; options3->Visible=false; options4->Visible=false;
   options5->Visible=false; options6->Visible=false; options7->Visible=false; options8->Visible=false;
   options9->Visible=false; options10->Visible=false;  */
}

//---------------------------------------------------------------------------

void TForm1::Enable_folder_changing()
{
  SaveDefault->Enabled=true;  loadConfig->Enabled=true;  btnStartEngine->Enabled=true;
  EnableTriggers(0);
  btnStopEngine->Enabled=false;
}

//---------------------------------------------------------------------------


void TForm1::EnableTriggers(bool bEnable)
{

   if(!lock0->Down)play0->Enabled = bEnable;
   /*
   play2->Enabled = bEnable;
   play3->Enabled = bEnable;
   play4->Enabled = bEnable;
   play5->Enabled = bEnable;
   play6->Enabled = bEnable;
   play7->Enabled = bEnable;
   play8->Enabled = bEnable;
   play9->Enabled = bEnable;
   play10->Enabled = bEnable;  */
}

//---------------------------------------------------------------------------


void __fastcall TForm1::trckThreshold1Change(TObject *Sender)
{
  TztBird* Bird; TNumLab *thresh; TTrackBar *threshTb;
  if(Sender==trckThreshold1) {threshTb=trckThreshold1; thresh=thresh1; Bird=m_Birds[0];}
  else if(Sender==trckThreshold2) {threshTb=trckThreshold2; thresh=thresh2; Bird=m_Birds[1];}
  else if(Sender==trckThreshold3) {threshTb=trckThreshold3; thresh=thresh3; Bird=m_Birds[2];}
  else if(Sender==trckThreshold4) {threshTb=trckThreshold4; thresh=thresh4; Bird=m_Birds[3];}
  else if(Sender==trckThreshold5) {threshTb=trckThreshold5; thresh=thresh5; Bird=m_Birds[4];}
  else if(Sender==trckThreshold6) {threshTb=trckThreshold6; thresh=thresh6; Bird=m_Birds[5];}
  else if(Sender==trckThreshold7) {threshTb=trckThreshold7; thresh=thresh7; Bird=m_Birds[6];}
  else if(Sender==trckThreshold8) {threshTb=trckThreshold8; thresh=thresh8; Bird=m_Birds[7];}
  else if(Sender==trckThreshold9) {threshTb=trckThreshold9; thresh=thresh9; Bird=m_Birds[8];}
  else if(Sender==trckThreshold10) {threshTb=trckThreshold10; thresh=thresh10; Bird=m_Birds[9];}

  int nNewThreshold=threshTb->Position*320;
  thresh->Value=nNewThreshold/1000;
  Bird->SetLongValue(vPeakThreshold,nNewThreshold);
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
  //  if(!checkActive())return; // nothing to record since all channels are inactive
   // CZT_OCX_BirdProps props;
    pre_thresh_buffer->Enabled=false;
    folder_problems=false;
    for(int i=0;i<10;i++)
    {
        //bird_active[i]->Enabled=false;
        if(bird_active[i]->Checked) validate_folders(i);
       // recdata_accumulated[i]=0;
    }

    if(folder_problems)
    {
        MessageDlgPos("At list one of the folders in the recording destination is invalid. Please go to 'recording' and change temporary or stage 1 storage location", mtError, TMsgDlgButtons() << mbOK, 0, 10, 50);

        return;
    }

    if(!active1->Checked && !active2->Checked && !active3->Checked && !active4->Checked
    && !active5->Checked && !active6->Checked && !active7->Checked && !active8->Checked
    && !active9->Checked && !active10->Checked)
    {
       MessageDlgPos("There is nothing to record: Please configure && activate at least one of the channels first.", mtError, TMsgDlgButtons() << mbOK, 0, 10, 50);
       return;
    }
    Update_Labels();
    Disable_folder_changing();
    recording=true;
    display->Enabled=false;
    EnableTriggers(true);
    bool train_on=false;
    for(int i=0;i<10;i++)
    {
        Handle_training_state(i);
        if(train[i]->Down) train_on=true;
    }
    /*
    if(train_on)
    {
        try{configDataCard();}
		catch(...)
		{
			Application->MessageBox(L"NI Card configuration error, training might not work properly", NULL, MB_OK);
		}
    } */
    ztBirdEngine1->StartEngine();
}


//---------------------------------------------------------------------------



void __fastcall TForm1::btnStopEngineClick(TObject *Sender)
{
    //str=m_Birds[0]->GetLongValue(vPrebufferSamples);//vBirdState);
    pre_thresh_buffer->Enabled=true;
    ztBirdEngine1->StopEngine();
    EnableTriggers(false);
    recording=false;
    Enable_folder_changing();
    display->Enabled=true;
    for(int i=0;i<10;i++) bird_active[i]->Enabled=true;
}


//---------------------------------------------------------------------------




void __fastcall TForm1::play0Click(TObject *Sender)
{
   int i=0;
   if(Sender==play0)i=0;
   else if(Sender==play1)i=1;
   else if(Sender==play2)i=2;
   else if(Sender==play3)i=3;
   else if(Sender==play4)i=4;
   else if(Sender==play5)i=5;
   else if(Sender==play6)i=6;
   else if(Sender==play7)i=7;
   else if(Sender==play8)i=8;
   else if(Sender==play9)i=9;
   else if(Sender==play10)i=10;
   else if(Sender==play11)i=11;
   else if(Sender==play12)i=12;
   else if(Sender==play13)i=13;
   else if(Sender==play14)i=14;
   else if(Sender==play15)i=15;
   else if(Sender==play16)i=16;
   else if(Sender==play17)i=17;
   else if(Sender==play18)i=18;
   else if(Sender==play19)i=19;
   int bird=i/2;
   Play_counts[i]->Value++;
   if(playback_mute_rec[bird]->Checked) m_Birds[bird]->SetLongValue(vMuteBirdData,1); // avoid recording playbacks
   m_Birds[bird]->TriggerSound();  // play the sound
   float fr=1000/Timer1->Interval;
   Session[i].gap_timer=fr*Session[i].gap;
}


//---------------------------------------------------------------------------


void __fastcall TForm1::ExitClick(TObject *Sender)
{
    ztBirdEngine1->StopEngine();
    EnableTriggers(false);
    recording=false;
	try{
		Enable_folder_changing();
		save_all_configs();
		for(int i=0;i<10;i++)save_channel_attributes(m_Birds[i], i+1);
		for(int i=0;i<20;i++)save_session_attributes(i);
	  }
    catch(...){Application->Terminate();}
    Application->Terminate();
}


//---------------------------------------------------------------------------

void TForm1::training_schedules()
{
/*
 static AnsiString ref_date=Date();
 if(ref_date!=date) // a new day!
 {
	ref_date=date;
	Word Year1, Month1, Day1, Year2, Month2, Day2;
	TDateTime date = Now();
    DecodeDate(date, Year1, Month1, Day1);

    // onsets of trainings
    for(int i=0;i<10;i++)
    {
       recdata_accumulated[i]=0; // reset recording log in the morning
       DecodeDate(start_train[i]->Date, Year2, Month2, Day2);
       if(toTrain[i]->Checked && Month1==Month2 && Day1==Day2){train[i]->Down=true; Handle_training_state(i);}
       DecodeDate(end_train[i]->Date, Year2, Month2, Day2);
       if(toStop[i]->Checked && Month1==Month2 && Day1==Day2){ train[i]->Down=false; Handle_training_state(i);}
    }
 }
 */
}


void TForm1::update_session_state(int i)  // every 30s
{
  // figure out if session should be on or off based on the time alone
  bool session_on=false;
  if(!Session[i].enabled){Session[i].state=false; return;}
  int session_dur;
   if( (hour==Session[i].start_hour && minute>=Session[i].start_min) // just on the minute to start
     || (hour>Session[i].start_hour &&  hour<Session[i].end_hour) // in the middle of a session
     || (hour==Session[i].end_hour && minute<Session[i].end_min) ) // in last hour of session
     session_on=true;


   if(!session_on){Session[i].state=false; return; }

   // reset variables in the beginning of a new session
   else if(session_on && Session[i].state==false)
   {
        if(!onset_timing[i]->ItemIndex)Session[i].state=true; // start session on time
        Session[i].song_played=0;
        playbacks_played[i]->Value=0;
        if(!Session[i].operant)passive_played[i/2]->Value=0;
        else if(Session[i].operant==1) // key 1
        {
           if(i%2) playbacks_played[i-1]->Value=0; // odd
           else playbacks_played[i]->Value=0; // even
        }
        else if(Session[i].operant==2)  // key 2
        {
           if(i%2) playbacks_played[i]->Value=0; // odd
           else playbacks_played[i+1]->Value=0; // even
        }
        else // both keys
        {
           if(i%2) {playbacks_played[i-1]->Value=0; playbacks_played[i]->Value=0;}// odd
           else {playbacks_played[i]->Value=0; playbacks_played[i+1]->Value=0;}// odd // even
        }
   }

   else if(session_on && onset_timing[i]->ItemIndex && Session[i].state==false)//random start
   {
     // if only 15 min left start the session
     if(hour==Session[i].end_hour && Session[i].end_min-minute<15)Session[i].state=true;
     else // otherwise toss a coin
     {
        session_dur=120*(Session[i].end_hour-Session[i].start_hour);// number of 30s intervals in the session
        if(random(session_dur)==0) Session[i].state=true;
     }
   }

   else Session[i].state=true;
}


//---------------------------------------------------------------------------
/*
This time is activated every 100ms to test and update the status of different
sensors (keys) and manage tasks.
Logic:
For each bird: Is a sesson on, and is everyting okay with it.
Have session objects with the following features:
state: session on/off
type: operant/passive
onset: ontime or randomized.    */
void __fastcall TForm1::Timer1Timer(TObject *Sender)   // 100ms
{
   static int first_time=true,
            time_timer=0, // for displaying time
            second30=0,   // 30 seconds timer for handling tasks
            direct_recording_timer=9999999;//6;//direct_recording_duration->Position*10; // manage epochs of direct recording
   static bool direct_recording=false;

   if(first_time){ first_time=false; initiation();}



  // highest priority task:
  // 1. handle training
   if(NIcard_exists->Checked)
   {
      read_NIDAQ();
	  if(controls->ActivePage==devices)paint_test_keys();
   }

   /*
   for(int i=0;i<20;i++)
   {
           if(Session[i].state)
           {
             read_NIDAQ();
             break;
           }
   }
   */

   if(recording) for(int i=0;i<20;i++) Handle_training(i);

   // 2. handle direct recording
   if(recording)direct_recording_timer++;
   if(recording && !direct_recording && direct_recording_timer>2)// wait 0.5 seconds and then start direct recording
   {
    for(int i=0;i<10;i++)m_Birds[i]->SetLongValue(vDirectRecord,direct_record_start); //if(bird_active[i]->Color==clBlue)
    direct_recording=true;
   }
   if(recording && direct_recording && direct_recording_timer>direct_recording_duration->Position*10)// stop the direct recording for 1/2 second
   {
    for(int i=0;i<10;i++)m_Birds[i]->SetLongValue(vDirectRecord,direct_record_stop); //if(bird_active[i]->Color==clBlue)
    direct_recording_timer=0;
	direct_recording=false;
   }

   // lower priority tasks:

  // calc time of day once a sec
  /*  */
   time_timer++;
   if(time_timer>10)
   {
	 date=FormatDateTime("yyyy'-'mm'-'dd", Date());
	 tm=FormatDateTime("hh':'mm':'ss", Time());//tm=Time();
	 time_timer=0;
	 Label1->Caption = tm;
	 gettime(&tStr);
     hour=tStr.ti_hour;
	 minute=tStr.ti_min;
	 second=tStr.ti_sec;
   }

   // hanlde low priority tasks every 30 secs
   second30++;
   if(second30*Timer1->Interval>30000)
   {
    second30=0;
	training_schedules(); // start training or stop training for birds where training is automatically set.
    if(hour==23 && minute>50)
                for(int i=0;i<10;i++) recdata_accumulated[i]=0;
    // if night, all sessions state are off -- return;
    if(hour>=lightsOff->Value || hour<lightsOn->Value)
    {
        sessionLb->Color=clBlack;
        if(dont_train_at_night->Checked) sessionLb->Caption="Training off";
        else sessionLb->Caption="Lights off";
        //for(int i=0;i<10;i++) Box[i]->Brush->Color=clTeal;
        return;
    }
    else
    {
      sessionLb->Color=clTeal;
      sessionLb->Caption="Playbacks";
    }
    // update Session object parameters
	for(int i=0;i<10;i++) Handle_training_state(i);
    update_ages();
   } // end 'every 30 seconds' updates

}


//_____________________________________________________________________________


void __fastcall TForm1::train1Click(TObject *Sender)
{
 int i;
 if(Sender==train1) {train1a->Down=train1->Down; i=0; }
 else if(Sender==train2){train2a->Down=train2->Down; i=1; }
 else if(Sender==train3){train3a->Down=train3->Down; i=2; }
 else if(Sender==train4){train4a->Down=train4->Down; i=3; }
 else if(Sender==train5){train5a->Down=train5->Down; i=4; }
 else if(Sender==train6){train6a->Down=train6->Down; i=5; }
 else if(Sender==train7){train7a->Down=train7->Down; i=6; }
 else if(Sender==train8){train8a->Down=train8->Down; i=7; }
 else if(Sender==train9){train9a->Down=train9->Down; i=8; }
 else if(Sender==train10){train10a->Down=train10->Down; i=9;}
 // training buttons are duplicated in the sessions tab:
 else if(Sender==train1a){train1->Down=train1a->Down; i=0; }
 else if(Sender==train2a){train2->Down=train2a->Down; i=1;  }
 else if(Sender==train3a){train3->Down=train3a->Down; i=2;  }
 else if(Sender==train4a){train4->Down=train4a->Down; i=3;  }
 else if(Sender==train5a){train5->Down=train5a->Down; i=4;  }
 else if(Sender==train6a){train6->Down=train6a->Down; i=5; }
 else if(Sender==train7a){train7->Down=train7a->Down; i=6; }
 else if(Sender==train8a){train8->Down=train8a->Down; i=7;  }
 else if(Sender==train9a){train9->Down=train9a->Down; i=8;  }
 else if(Sender==train10a){train10->Down=train10a->Down; i=9;}
 else return;
 // channel is off but training is turned on
 Handle_training_state(i);
 if(!train[i]->Down)
 {
        Session[i*2].state=false;
		Session[i*2+1].state=false;
		playback_error[i]->Visible=false;
        return;
 }

 //is there any enabled (active) session?
if(!Session[i*2].enabled && !Session[i*2+1].enabled)
{
  str="Both operant training and passive playbacks are off -- training cannot be done. Try setting a training session below.";
  MessageDlgPos(str, mtWarning, TMsgDlgButtons() << mbOK, 0, 10, 80);
  train[i]->Down=false;
  train[i+10]->Down=false;// we have two coppies of thsi button, we make them behave the same...
  return;
}

 if(!bird_active[i]->Checked && train[i]->Down)
 {
    str="You activated a training session but the recording channel is turned off. Training will not start unless you set valid input and output channels. Click the Setting button (right side of this button)";
    MessageDlgPos(str, mtWarning, TMsgDlgButtons() << mbOK, 0, 10, 80);
 }

 if(playbacks_[i*2]->ItemIndex<0 && playbacks_[i*2+1]->ItemIndex<0)
 {
    str="You activated a training session but no sounds are selected for playbacks. Go to Playbacks->sounds and select a sound for playbacks in the appropriate channel)";
    MessageDlgPos(str, mtWarning, TMsgDlgButtons() << mbOK, 0, 10, 80);
 }
}

//---------------------------------------------------------------------------

void TForm1::Handle_training_state(int bird)
{
	  if(train[bird]->Down)
	  {
		  // is session time on?
		  update_session_state(bird*2);
		  update_session_state(bird*2+1);

		  // display session state as color of rectangle so if no session is on color is black
		  if(Session[bird*2].state || Session[bird*2+1].state){
		  playback_error[bird]->Visible=false;
		  Box[bird]->Brush->Color=clTeal;
		  }
		  else {
		  playback_error[bird]->Caption="Off Session";
		  playback_error[bird]->Visible=true;
		  Box[bird]->Brush->Color=clBlack;
		  }

		  // should we see key one?
		  if(Session[bird*2].enabled && (Session[bird*2].operant==1 || Session[bird*2].operant==3)
		  || Session[bird*2+1].enabled && (Session[bird*2+1].operant==1 || Session[bird*2+1].operant==3))
			 key[bird*2]->Visible=true;
			 else key[bird*2]->Visible=false;

		  // should we see key two?
		  if(Session[bird*2].enabled && (Session[bird*2].operant==2 || Session[bird*2].operant==3)
		  || Session[bird*2+1].enabled && (Session[bird*2+1].operant==2 || Session[bird*2+1].operant==3))
			 key[bird*2+1]->Visible=true;
			 else key[bird*2+1]->Visible=false;

		 // should we see passive?
		 if((Session[bird*2].enabled && !Session[bird*2].operant) || (Session[bird*2+1].enabled && !Session[bird*2+1].operant) )
			   playing[bird]->Visible=true;
		  else playing[bird]->Visible=false;

		  // remember that training is on:
		  current->RecNo=bird+3;
		  current->Edit();
		  current->FieldValues["training"]=true;
		  current->Post();
	  }

	  else // training is off
	  {
		  key[bird*2]->Visible=false;
		  key[bird*2+1]->Visible=false;
		  playing[bird]->Visible=false;
		  if(current->Active)
		  {
			current->RecNo=bird+3;
			current->Edit();
			current->FieldValues["training"]=false;
			current->Post();
		  }
	  }
}

//---------------------------------------------------------------------------

void __fastcall TForm1::MaxDurChange(TObject *Sender)
{
  MaxDurN->Value=MaxDur->Position*1000;
  for(int i=0;i<10;i++) m_Birds[i]->SetLongValue(vRecordingCutoffMS, MaxDurN->Value);
}

//---------------------------------------------------------------------------

void __fastcall TForm1::Reset1Click(TObject *Sender)
{
 int i;
 if(Sender==Reset1) i=0;
 else if(Sender==Reset2) i=1;
 else if(Sender==Reset3) i=2;
 else if(Sender==Reset4) i=3;
 else if(Sender==Reset5) i=4;
 else if(Sender==Reset6) i=5;
 else if(Sender==Reset7) i=6;
 else if(Sender==Reset8) i=7;
 else if(Sender==Reset8) i=8;
 else if(Sender==Reset10) i=9;

 recdata_accumulated[i]=0;
 recorded_bar[i]->Position=0;
 playbacks_played[i*2]->Value=0;
 playbacks_played[i*2+1]->Value=0;
 passive_played[i]->Value=0;
 Session[i*2].song_played=0;
 Session[i*2+1].song_played=0;
 recorded_bar[i]->Position=0;
 recdata_accumulated[i]=0;
}

//---------------------------------------------------------------------------


void __fastcall TForm1::minimum_recording_durationChange(TObject *Sender)
{
  static bool first_time=true;
  if(first_time) {first_time=false; return;}
  duration->Value=minimum_recording_duration->Position*1000;
  for(int i=0;i<10;i++) m_Birds[i]->SetLongValue(vPeakRecordDuration,duration->Value);
}


//---------------------------------------------------------------------------


void __fastcall TForm1::SoundDurTbChange(TObject *Sender)
{
  peaksN->Value=SoundDurTb->Position;
  long peaks = peaksN->Value;
  for(int i=0;i<10;i++) m_Birds[i]->SetLongValue(vPeakComparator,peaks);
}
//---------------------------------------------------------------------------


void TForm1::initiation()
{
	 date=FormatDateTime("yyyy'-'mm'-'dd", Date());
	 tm=FormatDateTime("hh'-'mm'-'ss", Time());
	 gettime(&tStr);
	 hour=tStr.ti_hour;
	 minute=tStr.ti_min;
	 second=tStr.ti_sec;
 // 1.the IDs should not be changed by user, only birdname is changed and called ID sometimes
	 SetBirdString(ztBird1,vBirdID,"a");
	 SetBirdString(ztBird1,vBirdName,"1");
	 SetBirdString(ztBird2,vBirdID,"b");
	 SetBirdString(ztBird2,vBirdName,"2");
	 SetBirdString(ztBird3,vBirdID,"c");
	 SetBirdString(ztBird3,vBirdName,"3");
	 SetBirdString(ztBird4,vBirdID,"d");
	 SetBirdString(ztBird4,vBirdName,"4");
	 SetBirdString(ztBird5,vBirdID,"e");
	 SetBirdString(ztBird5,vBirdName,"5");
	 SetBirdString(ztBird6,vBirdID,"f");
	 SetBirdString(ztBird6,vBirdName,"6");
	 SetBirdString(ztBird7,vBirdID,"g");
	 SetBirdString(ztBird7,vBirdName,"7");
	 SetBirdString(ztBird8,vBirdID,"h");
	 SetBirdString(ztBird8,vBirdName,"8");
	 SetBirdString(ztBird9,vBirdID,"i");
	 SetBirdString(ztBird9,vBirdName,"9");
	 SetBirdString(ztBird10,vBirdID,"j");
	 SetBirdString(ztBird10,vBirdName,"10");

  // 2. set arrays of objects

	bird_active[0]=active1;  bird_active[1]=active2; bird_active[2]=active3; bird_active[3]=active4; bird_active[4]=active5;
	bird_active[5]=active6;  bird_active[6]=active7; bird_active[7]=active8; bird_active[8]=active9; bird_active[9]=active10;

	ports[0]=p1; ports[1]=l1;
	ports[2]=p2; ports[3]=l2;
	ports[4]=p3; ports[5]=l3;
	ports[6]=p4; ports[7]=l4;
	ports[8]=p5; ports[9]=l5;
	ports[10]=p6; ports[11]=l6;
	ports[12]=p7; ports[13]=l7;
	ports[14]=p8; ports[15]=l8;
	ports[16]=p9; ports[17]=l9;
	ports[18]=p10; ports[19]=l10;

	ports[20]=p11; ports[21]=l11;
	ports[22]=p12; ports[23]=l12;
	ports[24]=p13; ports[25]=l13;
	ports[26]=p14; ports[27]=l14;
	ports[28]=p15; ports[29]=l15;
	ports[30]=p16; ports[31]=l16;
	ports[32]=p17; ports[33]=l17;
	ports[34]=p18; ports[35]=l18;
	ports[36]=p19; ports[37]=l19;
	ports[38]=p20; ports[39]=l20;

	ports[40]=p21; ports[41]=l21;
	ports[42]=p22; ports[43]=l22;
	ports[44]=p23; ports[45]=l23;
	ports[46]=p24; ports[47]=l24;
	ports[48]=p25; ports[49]=l25;
	ports[50]=p26; ports[51]=l26;
	ports[52]=p27; ports[53]=l27;
	ports[54]=p28; ports[55]=l28;
	ports[56]=p29; ports[57]=l29;
	ports[58]=p30; ports[59]=l30;

	ports[60]=p31; ports[61]=l31;
	ports[62]=p32; ports[63]=l32;
	ports[64]=p33; ports[65]=l33;
	ports[66]=p34; ports[67]=l34;
	ports[68]=p35; ports[69]=l35;
	ports[70]=p36; ports[71]=l36;
	ports[72]=p37; ports[73]=l37;
	ports[74]=p38; ports[75]=l38;
	ports[76]=p39; ports[77]=l39;
	ports[78]=p40; ports[79]=l40;

	m_Birds[0] = ztBird1;  m_Birds[1] = ztBird2; m_Birds[2] = ztBird3;  m_Birds[3] = ztBird4;
	m_Birds[4] = ztBird5;  m_Birds[5] = ztBird6;  m_Birds[6] = ztBird7; m_Birds[7] = ztBird8;
	m_Birds[8] = ztBird9; m_Birds[9] = ztBird10;

	song[0]=song0;   song[1]=song1; song[2]=song2; song[3]=song3; song[4]=song4;
	song[5]=song5;   song[6]=song6; song[7]=song7; song[8]=song8; song[9]=song9;
	song[10]=song10;   song[11]=song11; song[12]=song12; song[13]=song13; song[14]=song14;
	song[15]=song5;   song[16]=song16; song[17]=song17; song[18]=song18; song[19]=song19;

	key[0]=key1; key[1]=key2; key[2]=key3; key[3]=key4;
	key[4]=key5; key[5]=key6; key[6]=key7; key[7]=key8;
	key[8]=key9; key[9]=key10;
	key[10]=key11; key[11]=key12; key[12]=key13; key[13]=key14;
	key[14]=key15; key[15]=key16; key[16]=key17; key[17]=key18;
	key[18]=key19; key[19]=key20;

	Box[0]=Box1; Box[1]=Box2; Box[2]=Box3; Box[3]=Box4;
	Box[4]=Box5; Box[5]=Box6; Box[6]=Box7; Box[7]=Box8;
	Box[8]=Box9; Box[9]=Box10;

	vol[0]=vol1; vol[1]=vol2; vol[2]=vol3; vol[3]=vol4;
	vol[4]=vol5; vol[5]=vol6; vol[6]=vol7; vol[7]=vol8;
	vol[8]=vol9; vol[9]=vol10;

	toTrain[0]=toTrain1; toTrain[1]=toTrain2; toTrain[2]=toTrain3; toTrain[3]=toTrain4;
	toTrain[4]=toTrain5; toTrain[5]=toTrain6; toTrain[6]=toTrain7; toTrain[7]=toTrain8;
	toTrain[8]=toTrain9; toTrain[9]=toTrain10;

	toStop[0]=toStop1; toStop[1]=toStop2; toStop[2]=toStop3; toStop[3]=toStop4;
	toStop[4]=toStop5; toStop[5]=toStop6; toStop[6]=toStop7; toStop[7]=toStop8;
	toStop[8]=toStop9; toStop[9]=toStop10;

	start_train[0]=start1; start_train[1]=start2; start_train[2]=start3; start_train[3]=start4;
	start_train[4]=start5; start_train[5]=start6; start_train[6]=start7; start_train[7]=start8;
	start_train[8]=start9; start_train[9]=start10;

	end_train[0]=end1; end_train[1]=end2; end_train[2]=end3; end_train[3]=end4;
	end_train[4]=end5; end_train[5]=end6; end_train[6]=end7; end_train[7]=end8;
	end_train[8]=end9; end_train[9]=end10;

	auto_scroll[0]=auto_scroll0; auto_scroll[1]=auto_scroll1; auto_scroll[2]=auto_scroll2;
    auto_scroll[3]=auto_scroll3; auto_scroll[4]=auto_scroll4; auto_scroll[5]=auto_scroll5;
    auto_scroll[6]=auto_scroll6; auto_scroll[7]=auto_scroll7; auto_scroll[8]=auto_scroll8;
    auto_scroll[9]=auto_scroll9; auto_scroll[10]=auto_scroll10; auto_scroll[11]=auto_scroll11;
    auto_scroll[12]=auto_scroll12; auto_scroll[13]=auto_scroll13; auto_scroll[14]=auto_scroll14;
    auto_scroll[15]=auto_scroll15; auto_scroll[16]=auto_scroll16; auto_scroll[17]=auto_scroll17;
	auto_scroll[18]=auto_scroll18; auto_scroll[19]=auto_scroll19;

	train[0]=train1; train[1]=train2; train[2]=train3; train[3]=train4;
	train[4]=train5; train[5]=train6; train[6]=train7; train[7]=train8;
	train[8]=train9; train[9]=train10;
	train[10]=train1a; train[11]=train2a; train[12]=train3a; train[13]=train4a;
	train[14]=train5a; train[15]=train6a; train[16]=train7a; train[17]=train8a;
	train[18]=train9a; train[19]=train10a;

	scaler[0]=scaler1; scaler[1]=scaler2; scaler[2]=scaler3; scaler[3]=scaler4;
	scaler[4]=scaler5; scaler[5]=scaler6; scaler[6]=scaler7; scaler[7]=scaler8;
	scaler[8]=scaler9; scaler[9]=scaler10;

	scalerN[0]=scalerN1; scalerN[1]=scalerN2; scalerN[2]=scalerN3; scalerN[3]=scalerN4;
	scalerN[4]=scalerN5; scalerN[5]=scalerN6; scalerN[6]=scalerN7; scalerN[7]=scalerN8;
	scalerN[8]=scalerN9; scalerN[9]=scalerN10;

	// active or slave
	channel_mode[0]=channel_mode1; channel_mode[1]=channel_mode2; channel_mode[2]=channel_mode3; channel_mode[3]=channel_mode4;
	channel_mode[4]=channel_mode5; channel_mode[5]=channel_mode6; channel_mode[6]=channel_mode7; channel_mode[7]=channel_mode8;
	channel_mode[8]=channel_mode9; channel_mode[9]=channel_mode10;

   	session_Lock[0]=session_lock1; session_Lock[1]=session_lock2; session_Lock[2]=session_lock3; session_Lock[3]=session_lock4;
	session_Lock[4]=session_lock5; session_Lock[5]=session_lock6; session_Lock[6]=session_lock7; session_Lock[7]=session_lock8;
	session_Lock[8]=session_lock9; session_Lock[9]=session_lock10;

	recorded_bar[0]=recorded1; recorded_bar[1]=recorded2; recorded_bar[2]=recorded3; recorded_bar[3]=recorded4;
	recorded_bar[4]=recorded5; recorded_bar[5]=recorded6; recorded_bar[6]=recorded7; recorded_bar[7]=recorded8;
	recorded_bar[8]=recorded9; recorded_bar[9]=recorded10;

	// these are actually the pannels for the alternative file lists (in two choice mode) to allow making visible
	playback_list[0]=list_1a; playback_list[1]=list_2a; playback_list[2]=list_3a; playback_list[3]=list_4a;
	playback_list[4]=list_5a; playback_list[5]=list_6a; playback_list[6]=list_7a; playback_list[7]=list_8a;
	playback_list[8]=list_9a; playback_list[9]=list_10a;

	passive_odds[0]=passive_odds1; passive_odds[1]=passive_odds2; passive_odds[2]=passive_odds3;
	passive_odds[3]=passive_odds4; passive_odds[4]=passive_odds5; passive_odds[5]=passive_odds6;
	passive_odds[6]=passive_odds7; passive_odds[7]=passive_odds8; passive_odds[8]=passive_odds9;
	passive_odds[9]=passive_odds10;
	passive_odds[10]=passive_odds11; passive_odds[11]=passive_odds12; passive_odds[12]=passive_odds13;
	passive_odds[13]=passive_odds14; passive_odds[14]=passive_odds15; passive_odds[15]=passive_odds16;
	passive_odds[16]=passive_odds17; passive_odds[17]=passive_odds18; passive_odds[18]=passive_odds19;
	passive_odds[19]=passive_odds20;

	playbacks_quota[0]=quota1; playbacks_quota[1]=quota2; playbacks_quota[2]=quota3;
	playbacks_quota[3]=quota4; playbacks_quota[4]=quota5; playbacks_quota[5]=quota6;
	playbacks_quota[6]=quota7; playbacks_quota[7]=quota8; playbacks_quota[8]=quota9;
	playbacks_quota[9]=quota10;
	playbacks_quota[10]=quota11; playbacks_quota[11]=quota12; playbacks_quota[12]=quota13;
	playbacks_quota[13]=quota14; playbacks_quota[14]=quota15; playbacks_quota[15]=quota16;
	playbacks_quota[16]=quota17; playbacks_quota[17]=quota18; playbacks_quota[18]=quota19;
	playbacks_quota[19]=quota20;

	playbacks_played[0]=Playbacks1; playbacks_played[1]=Playbacks2; playbacks_played[2]=Playbacks3;
	playbacks_played[3]=Playbacks4; playbacks_played[4]=Playbacks5; playbacks_played[5]=Playbacks6;
	playbacks_played[6]=Playbacks7; playbacks_played[7]=Playbacks8; playbacks_played[8]=Playbacks9;
	playbacks_played[9]=Playbacks10;
	playbacks_played[10]=Playbacks11; playbacks_played[11]=Playbacks12; playbacks_played[12]=Playbacks13;
	playbacks_played[13]=Playbacks14; playbacks_played[14]=Playbacks15; playbacks_played[15]=Playbacks16;
	playbacks_played[16]=Playbacks17; playbacks_played[17]=Playbacks18; playbacks_played[18]=Playbacks19;
	playbacks_played[19]=Playbacks20;

	enable_quota[0]=enable_quota1; enable_quota[1]=enable_quota2; enable_quota[2]=enable_quota3;
	enable_quota[3]=enable_quota4; enable_quota[4]=enable_quota5; enable_quota[5]=enable_quota6;
	enable_quota[6]=enable_quota7; enable_quota[7]=enable_quota8; enable_quota[8]=enable_quota9;
	enable_quota[9]=enable_quota10;
	enable_quota[10]=enable_quota11; enable_quota[11]=enable_quota12; enable_quota[12]=enable_quota13;
	enable_quota[13]=enable_quota14; enable_quota[14]=enable_quota15; enable_quota[15]=enable_quota16;
	enable_quota[16]=enable_quota17; enable_quota[17]=enable_quota18; enable_quota[18]=enable_quota19;
	enable_quota[19]=enable_quota20;

	playback_error[0]=playback_error1; playback_error[1]=playback_error2; playback_error[2]=playback_error3;
	playback_error[3]=playback_error4; playback_error[4]=playback_error5; playback_error[5]=playback_error6;
	playback_error[6]=playback_error7; playback_error[7]=playback_error8; playback_error[8]=playback_error9;
	playback_error[9]=playback_error10;

	onset_timing[0]=onset_timing1; onset_timing[1]=onset_timing2; onset_timing[2]=onset_timing3;
	onset_timing[3]=onset_timing4; onset_timing[4]=onset_timing5; onset_timing[5]=onset_timing6;
	onset_timing[6]=onset_timing7; onset_timing[7]=onset_timing8; onset_timing[8]=onset_timing9;
	onset_timing[9]=onset_timing10;
	onset_timing[10]=onset_timing11; onset_timing[11]=onset_timing12; onset_timing[12]=onset_timing13;
	onset_timing[13]=onset_timing14; onset_timing[14]=onset_timing15; onset_timing[15]=onset_timing16;
	onset_timing[16]=onset_timing17; onset_timing[17]=onset_timing18; onset_timing[18]=onset_timing19;
	onset_timing[19]=onset_timing20;


	passive_odds_UpDown[0]=passive_odds_UpDown1; passive_odds_UpDown[1]=passive_odds_UpDown2; passive_odds_UpDown[2]=passive_odds_UpDown3;
	passive_odds_UpDown[3]=passive_odds_UpDown4; passive_odds_UpDown[4]=passive_odds_UpDown5; passive_odds_UpDown[5]=passive_odds_UpDown6;
	passive_odds_UpDown[6]=passive_odds_UpDown7; passive_odds_UpDown[7]=passive_odds_UpDown8; passive_odds_UpDown[8]=passive_odds_UpDown9;
	passive_odds_UpDown[9]=passive_odds_UpDown10;
	passive_odds_UpDown[10]=passive_odds_UpDown11; passive_odds_UpDown[11]=passive_odds_UpDown12; passive_odds_UpDown[12]=passive_odds_UpDown13;
	passive_odds_UpDown[13]=passive_odds_UpDown14; passive_odds_UpDown[14]=passive_odds_UpDown15; passive_odds_UpDown[15]=passive_odds_UpDown16;
	passive_odds_UpDown[16]=passive_odds_UpDown17; passive_odds_UpDown[17]=passive_odds_UpDown18; passive_odds_UpDown[18]=passive_odds_UpDown19;
	passive_odds_UpDown[19]=passive_odds_UpDown20;

	quota_UpDown[0]=quota_UpDown1; quota_UpDown[1]=quota_UpDown2; quota_UpDown[2]=quota_UpDown3;
	quota_UpDown[3]=quota_UpDown4; quota_UpDown[4]=quota_UpDown5; quota_UpDown[5]=quota_UpDown6;
	quota_UpDown[6]=quota_UpDown7; quota_UpDown[7]=quota_UpDown8; quota_UpDown[8]=quota_UpDown9;
	quota_UpDown[9]=quota_UpDown10;
	quota_UpDown[10]=quota_UpDown11; quota_UpDown[11]=quota_UpDown12; quota_UpDown[12]=quota_UpDown13;
	quota_UpDown[13]=quota_UpDown14; quota_UpDown[14]=quota_UpDown15; quota_UpDown[15]=quota_UpDown16;
	quota_UpDown[16]=quota_UpDown17; quota_UpDown[17]=quota_UpDown18; quota_UpDown[18]=quota_UpDown19;
	quota_UpDown[19]=quota_UpDown20;

	session_playback_list[0]=session_playback_list0; session_playback_list[1]=session_playback_list1; session_playback_list[2]=session_playback_list2;
	session_playback_list[3]=session_playback_list3; session_playback_list[4]=session_playback_list4; session_playback_list[5]=session_playback_list5;
	session_playback_list[6]=session_playback_list6; session_playback_list[7]=session_playback_list7; session_playback_list[8]=session_playback_list8;
	session_playback_list[9]=session_playback_list9;
	session_playback_list[10]=session_playback_list10; session_playback_list[11]=session_playback_list11; session_playback_list[12]=session_playback_list12;
	session_playback_list[13]=session_playback_list13; session_playback_list[14]=session_playback_list14; session_playback_list[15]=session_playback_list15;
	session_playback_list[16]=session_playback_list16; session_playback_list[17]=session_playback_list17; session_playback_list[18]=session_playback_list18;
	session_playback_list[19]=session_playback_list19;

	playbacks_folder[0]=playbacks_folder0; playbacks_folder[1]=playbacks_folder1; playbacks_folder[2]=playbacks_folder2;
	playbacks_folder[3]=playbacks_folder3; playbacks_folder[4]=playbacks_folder4; playbacks_folder[5]=playbacks_folder5;
	playbacks_folder[6]=playbacks_folder6; playbacks_folder[7]=playbacks_folder7; playbacks_folder[8]=playbacks_folder8;
	playbacks_folder[9]=playbacks_folder9; playbacks_folder[10]=playbacks_folder10; playbacks_folder[11]=playbacks_folder11;
	playbacks_folder[12]=playbacks_folder12; playbacks_folder[13]=playbacks_folder13; playbacks_folder[14]=playbacks_folder14;
	playbacks_folder[15]=playbacks_folder15; playbacks_folder[16]=playbacks_folder16; playbacks_folder[17]=playbacks_folder17;
	playbacks_folder[18]=playbacks_folder18; playbacks_folder[19]=playbacks_folder19;

	session_playback_control[0]=session_playback_control0; session_playback_control[1]=session_playback_control1; session_playback_control[2]=session_playback_control2;
	session_playback_control[3]=session_playback_control3; session_playback_control[4]=session_playback_control4; session_playback_control[5]=session_playback_control5;
	session_playback_control[6]=session_playback_control6; session_playback_control[7]=session_playback_control7; session_playback_control[8]=session_playback_control8;
	session_playback_control[9]=session_playback_control9; session_playback_control[10]=session_playback_control10; session_playback_control[11]=session_playback_control11;
	session_playback_control[12]=session_playback_control12; session_playback_control[13]=session_playback_control13; session_playback_control[14]=session_playback_control14;
	session_playback_control[15]=session_playback_control15; session_playback_control[16]=session_playback_control16; session_playback_control[17]=session_playback_control17;
	session_playback_control[18]=session_playback_control18; session_playback_control[19]=session_playback_control19;

	playback_input[0]=input0; playback_input[1]=input1; playback_input[2]=input2;
	playback_input[3]=input3; playback_input[4]=input4; playback_input[5]=input5;
	playback_input[6]=input6; playback_input[7]=input7; playback_input[8]=input8;
	playback_input[9]=input9; playback_input[10]=input10; playback_input[11]=input11;
	playback_input[12]=input12; playback_input[13]=input13; playback_input[14]=input14;
	playback_input[15]=input15; playback_input[16]=input16; playback_input[17]=input17;
	playback_input[18]=input18; playback_input[19]=input19;

	play_sound[0]=play0; play_sound[1]=play1; play_sound[2]=play2;
	play_sound[3]=play3; play_sound[4]=play4; play_sound[5]=play5;
	play_sound[6]=play6; play_sound[7]=play7; play_sound[8]=play8;
	play_sound[9]=play9; play_sound[10]=play10; play_sound[11]=play11;
	play_sound[12]=play12; play_sound[13]=play13; play_sound[14]=play14;
	play_sound[15]=play15; play_sound[16]=play16; play_sound[17]=play17;
	play_sound[18]=play18; play_sound[19]=play19;

	random_switch[0]=random_switch0; random_switch[1]=random_switch1; random_switch[2]=random_switch2;
	random_switch[3]=random_switch3; random_switch[4]=random_switch4; random_switch[5]=random_switch5;
	random_switch[6]=random_switch6; random_switch[7]=random_switch7; random_switch[8]=random_switch8;
	random_switch[9]=random_switch9; random_switch[10]=random_switch10; random_switch[11]=random_switch11;
	random_switch[12]=random_switch12; random_switch[13]=random_switch13; random_switch[14]=random_switch14;
	random_switch[15]=random_switch15; random_switch[16]=random_switch16; random_switch[17]=random_switch17;
	random_switch[18]=random_switch18; random_switch[19]=random_switch19;

	reset_counts[0]=reset_counts0; reset_counts[1]=reset_counts1; reset_counts[2]=reset_counts2;
	reset_counts[3]=reset_counts3; reset_counts[4]=reset_counts4; reset_counts[5]=reset_counts5;
	reset_counts[6]=reset_counts6; reset_counts[7]=reset_counts7; reset_counts[8]=reset_counts8;
	reset_counts[9]=reset_counts9; reset_counts[10]=reset_counts10; reset_counts[11]=reset_counts11;
	reset_counts[12]=reset_counts12; reset_counts[13]=reset_counts13; reset_counts[14]=reset_counts14;
	reset_counts[15]=reset_counts15; reset_counts[16]=reset_counts16; reset_counts[17]=reset_counts17;
	reset_counts[18]=reset_counts18; reset_counts[19]=reset_counts19;

	trigger_effect[0]=key_effect1; trigger_effect[1]=key_effect2; trigger_effect[2]=key_effect3;
	trigger_effect[3]=key_effect4; trigger_effect[4]=key_effect5; trigger_effect[5]=key_effect6;
	trigger_effect[6]=key_effect7; trigger_effect[7]=key_effect8; trigger_effect[8]=key_effect9;
	trigger_effect[9]=key_effect10;
	trigger_effect[10]=key_effect11; trigger_effect[11]=key_effect12; trigger_effect[12]=key_effect13;
	trigger_effect[13]=key_effect14; trigger_effect[14]=key_effect15; trigger_effect[15]=key_effect16;
	trigger_effect[16]=key_effect17; trigger_effect[17]=key_effect18; trigger_effect[18]=key_effect19;
	trigger_effect[19]=key_effect20;

	effect_duration[0]=effect_duration1; effect_duration[1]=effect_duration2; effect_duration[2]=effect_duration3;
	effect_duration[3]=effect_duration4; effect_duration[4]=effect_duration5; effect_duration[5]=effect_duration6;
	effect_duration[6]=effect_duration7; effect_duration[7]=effect_duration8; effect_duration[8]=effect_duration9;
	effect_duration[9]=effect_duration10;
	effect_duration[10]=effect_duration11; effect_duration[11]=effect_duration12; effect_duration[12]=effect_duration13;
	effect_duration[13]=effect_duration14; effect_duration[14]=effect_duration15; effect_duration[15]=effect_duration16;
	effect_duration[16]=effect_duration17; effect_duration[17]=effect_duration18; effect_duration[18]=effect_duration19;
	effect_duration[19]=effect_duration20;

	effect_delay[0]=effect_delay1; effect_delay[1]=effect_delay2; effect_delay[2]=effect_delay3;
	effect_delay[3]=effect_delay4; effect_delay[4]=effect_delay5; effect_delay[5]=effect_delay6;
	effect_delay[6]=effect_delay7; effect_delay[7]=effect_delay8; effect_delay[8]=effect_delay9;
	effect_delay[9]=effect_delay10;
	effect_delay[10]=effect_delay11; effect_delay[11]=effect_delay12; effect_delay[12]=effect_delay13;
	effect_delay[13]=effect_delay14; effect_delay[14]=effect_delay15; effect_delay[15]=effect_delay16;
	effect_delay[16]=effect_delay17; effect_delay[17]=effect_delay18; effect_delay[18]=effect_delay19;
	effect_delay[19]=effect_delay20;

	gap_UpDown[0]=gap_UpDown1; gap_UpDown[1]=gap_UpDown2; gap_UpDown[2]=gap_UpDown3;
	gap_UpDown[3]=gap_UpDown4; gap_UpDown[4]=gap_UpDown5; gap_UpDown[5]=gap_UpDown6;
	gap_UpDown[6]=gap_UpDown7; gap_UpDown[7]=gap_UpDown8; gap_UpDown[8]=gap_UpDown9;
	gap_UpDown[9]=gap_UpDown10;
	gap_UpDown[10]=gap_UpDown11; gap_UpDown[11]=gap_UpDown12; gap_UpDown[12]=gap_UpDown13;
	gap_UpDown[13]=gap_UpDown14; gap_UpDown[14]=gap_UpDown15; gap_UpDown[15]=gap_UpDown16;
	gap_UpDown[16]=gap_UpDown17; gap_UpDown[17]=gap_UpDown18; gap_UpDown[18]=gap_UpDown19;
	gap_UpDown[19]=gap_UpDown20;

	session_startL[0]=session_startL1; session_startL[1]=session_startL2; session_startL[2]=session_startL3;
	session_startL[3]=session_startL4; session_startL[4]=session_startL5; session_startL[5]=session_startL6;
	session_startL[6]=session_startL7; session_startL[7]=session_startL8; session_startL[8]=session_startL9;
	session_startL[9]=session_startL10;
	session_startL[10]=session_startL11; session_startL[11]=session_startL12; session_startL[12]=session_startL13;
	session_startL[13]=session_startL14; session_startL[14]=session_startL15; session_startL[15]=session_startL16;
	session_startL[16]=session_startL17; session_startL[17]=session_startL18; session_startL[18]=session_startL19;
	session_startL[19]=session_startL20;

	session_endL[0]=session_endL1; session_endL[1]=session_endL2; session_endL[2]=session_endL3;
	session_endL[3]=session_endL4; session_endL[4]=session_endL5; session_endL[5]=session_endL6;
	session_endL[6]=session_endL7; session_endL[7]=session_endL8; session_endL[8]=session_endL9;
	session_endL[9]=session_endL10;
	session_endL[10]=session_endL11; session_endL[11]=session_endL12; session_endL[12]=session_endL13;
	session_endL[13]=session_endL14; session_endL[14]=session_endL15; session_endL[15]=session_endL16;
	session_endL[16]=session_endL17; session_endL[17]=session_endL18; session_endL[18]=session_endL19;
	session_endL[19]=session_endL20;

	training_session[0]=session1; training_session[1]=session2; training_session[2]=session3;
	training_session[3]=session4; training_session[4]=session5; training_session[5]=session6;
	training_session[6]=session7; training_session[7]=session8; training_session[8]=session9;
	training_session[9]=session10;
	training_session[10]=session11; training_session[11]=session12; training_session[12]=session13;
	training_session[13]=session14; training_session[14]=session15; training_session[15]=session16;
	training_session[16]=session17; training_session[17]=session18; training_session[18]=session19;
	training_session[19]=session20;

	session_panel[0]=session_panel1; session_panel[1]=session_panel2; session_panel[2]=session_panel3;
	session_panel[3]=session_panel4; session_panel[4]=session_panel5; session_panel[5]=session_panel6;
	session_panel[6]=session_panel7; session_panel[7]=session_panel8; session_panel[8]=session_panel9;
	session_panel[9]=session_panel10;
	session_panel[10]=session_panel11; session_panel[11]=session_panel12; session_panel[12]=session_panel13;
	session_panel[13]=session_panel14; session_panel[14]=session_panel15; session_panel[15]=session_panel16;
	session_panel[16]=session_panel17; session_panel[17]=session_panel18; session_panel[18]=session_panel19;
	session_panel[19]=session_panel20;


	passive_panel[0]=passive_panel1; passive_panel[1]=passive_panel2; passive_panel[2]=passive_panel3;
	passive_panel[3]=passive_panel4; passive_panel[4]=passive_panel5; passive_panel[5]=passive_panel6;
	passive_panel[6]=passive_panel7; passive_panel[7]=passive_panel8; passive_panel[8]=passive_panel9;
	passive_panel[9]=passive_panel10;
	passive_panel[10]=passive_panel11; passive_panel[11]=passive_panel12; passive_panel[12]=passive_panel13;
	passive_panel[13]=passive_panel14; passive_panel[14]=passive_panel15; passive_panel[15]=passive_panel16;
	passive_panel[16]=passive_panel17; passive_panel[17]=passive_panel18; passive_panel[18]=passive_panel19;
	passive_panel[19]=passive_panel20;

	session_hours[0]=session_hours1; session_hours[1]=session_hours2; session_hours[2]=session_hours3;
	session_hours[3]=session_hours4; session_hours[4]=session_hours5; session_hours[5]=session_hours6;
	session_hours[6]=session_hours7; session_hours[7]=session_hours8; session_hours[8]=session_hours9;
	session_hours[9]=session_hours10;
	session_hours[10]=session_hours11; session_hours[11]=session_hours12; session_hours[12]=session_hours13;
	session_hours[13]=session_hours14; session_hours[14]=session_hours15; session_hours[15]=session_hours16;
	session_hours[16]=session_hours17; session_hours[17]=session_hours18; session_hours[18]=session_hours19;
	session_hours[19]=session_hours20;
	session_hours[20]=session_hours21; session_hours[21]=session_hours22; session_hours[22]=session_hours23;
	session_hours[23]=session_hours24; session_hours[24]=session_hours25; session_hours[25]=session_hours26;
	session_hours[26]=session_hours27; session_hours[27]=session_hours28; session_hours[28]=session_hours29;
	session_hours[29]=session_hours30;
	session_hours[30]=session_hours31; session_hours[31]=session_hours32; session_hours[32]=session_hours33;
	session_hours[33]=session_hours34; session_hours[34]=session_hours35; session_hours[35]=session_hours36;
	session_hours[36]=session_hours37; session_hours[37]=session_hours38; session_hours[38]=session_hours39;
	session_hours[39]=session_hours40;

	session_min[0]=session_min1; session_min[1]=session_min2; session_min[2]=session_min3;
	session_min[3]=session_min4; session_min[4]=session_min5; session_min[5]=session_min6;
	session_min[6]=session_min7; session_min[7]=session_min8; session_min[8]=session_min9;
	session_min[9]=session_min10;
	session_min[10]=session_min11; session_min[11]=session_min12; session_min[12]=session_min13;
	session_min[13]=session_min14; session_min[14]=session_min15; session_min[15]=session_min16;
	session_min[16]=session_min17; session_min[17]=session_min18; session_min[18]=session_min19;
	session_min[19]=session_min20;
	session_min[20]=session_min21; session_min[21]=session_min22; session_min[22]=session_min23;
	session_min[23]=session_min24; session_min[24]=session_min25; session_min[25]=session_min26;
	session_min[26]=session_min27; session_min[27]=session_min28; session_min[28]=session_min29;
	session_min[29]=session_min30;
	session_min[30]=session_min31; session_min[31]=session_min32; session_min[32]=session_min33;
	session_min[33]=session_min34; session_min[34]=session_min35; session_min[35]=session_min36;
	session_min[36]=session_min37; session_min[37]=session_min38; session_min[38]=session_min39;
	session_min[39]=session_min40;

	test_key[0]=detect1; test_key[1]=detect2; test_key[2]=detect3;
	test_key[3]=detect4; test_key[4]=detect5; test_key[5]=detect6;
	test_key[6]=detect7; test_key[7]=detect8; test_key[8]=detect9;
	test_key[9]=detect10; test_key[10]=detect11; test_key[11]=detect12;
	test_key[12]=detect13; test_key[13]=detect14; test_key[14]=detect15;
	test_key[15]=detect16; test_key[16]=detect17; test_key[17]=detect18;
	test_key[18]=detect19; test_key[19]=detect20;

	lock_playbacks[0]=lock0; lock_playbacks[1]=lock1; lock_playbacks[2]=lock2;
	lock_playbacks[3]=lock3; lock_playbacks[4]=lock4; lock_playbacks[5]=lock5;
	lock_playbacks[6]=lock6; lock_playbacks[7]=lock7; lock_playbacks[8]=lock8;
	lock_playbacks[9]=lock9;

	playback_mute_rec[0]=playback_mute_rec1; playback_mute_rec[1]=playback_mute_rec2; playback_mute_rec[2]=playback_mute_rec3;
	playback_mute_rec[3]=playback_mute_rec4; playback_mute_rec[4]=playback_mute_rec5; playback_mute_rec[5]=playback_mute_rec6;
	playback_mute_rec[6]=playback_mute_rec7; playback_mute_rec[7]=playback_mute_rec8; playback_mute_rec[8]=playback_mute_rec9;
	playback_mute_rec[9]=playback_mute_rec10;

	biofeedback[0]=biofeedback1;  biofeedback[1]=biofeedback2; biofeedback[2]=biofeedback3; biofeedback[3]=biofeedback4; biofeedback[4]=biofeedback5;
	biofeedback[5]=biofeedback6;  biofeedback[6]=biofeedback7; biofeedback[7]=biofeedback8; biofeedback[8]=biofeedback9; biofeedback[9]=biofeedback10;

	passive_played[0]=passive_played1; passive_played[1]=passive_played2; passive_played[2]=passive_played3;
	passive_played[3]=passive_played4; passive_played[4]=passive_played5; passive_played[5]=passive_played6;
	passive_played[6]=passive_played7; passive_played[7]=passive_played8; passive_played[8]=passive_played9;
	passive_played[9]=passive_played10;

	rec_panel[0]=rec_panel1; rec_panel[1]=rec_panel2; rec_panel[2]=rec_panel3;
	rec_panel[3]=rec_panel4; rec_panel[4]=rec_panel5; rec_panel[5]=rec_panel6;
	rec_panel[6]=rec_panel7; rec_panel[7]=rec_panel8; rec_panel[8]=rec_panel9;
	rec_panel[9]=rec_panel10;


	Play_counts[0]=Play_counts0; Play_counts[1]=Play_counts1;
	Play_counts[2]=Play_counts2; Play_counts[3]=Play_counts3;
	Play_counts[4]=Play_counts4; Play_counts[5]=Play_counts5;
	Play_counts[6]=Play_counts6; Play_counts[7]=Play_counts7;
	Play_counts[8]=Play_counts8; Play_counts[9]=Play_counts9;
	Play_counts[10]=Play_counts10; Play_counts[11]=Play_counts11;
	Play_counts[12]=Play_counts12; Play_counts[13]=Play_counts13;
	Play_counts[14]=Play_counts14; Play_counts[15]=Play_counts15;
	Play_counts[16]=Play_counts16; Play_counts[17]=Play_counts17;
	Play_counts[18]=Play_counts18; Play_counts[19]=Play_counts19;

	playbacks_[0]=playbacks_0; playbacks_[1]=playbacks_1;
	playbacks_[2]=playbacks_2; playbacks_[3]=playbacks_3;
	playbacks_[4]=playbacks_4; playbacks_[5]=playbacks_5;
	playbacks_[6]=playbacks_6; playbacks_[7]=playbacks_7;
	playbacks_[8]=playbacks_8; playbacks_[9]=playbacks_9;
	playbacks_[10]=playbacks_10; playbacks_[11]=playbacks_11;
	playbacks_[12]=playbacks_12; playbacks_[13]=playbacks_13;
	playbacks_[14]=playbacks_14; playbacks_[15]=playbacks_15;
	playbacks_[16]=playbacks_16; playbacks_[17]=playbacks_17;
	playbacks_[18]=playbacks_18; playbacks_[19]=playbacks_19;

	tmpSound[0]=tmpSound1; tmpSound[1]=tmpSound2;  tmpSound[2]=tmpSound3;
	tmpSound[3]=tmpSound4; tmpSound[4]=tmpSound5;  tmpSound[5]=tmpSound6;
	tmpSound[6]=tmpSound7; tmpSound[7]=tmpSound8;  tmpSound[8]=tmpSound9;
	tmpSound[9]=tmpSound10;

	save[0]=save1; save[1]=save2;  save[2]=save3;
	save[3]=save4; save[4]=save5;  save[5]=save6;
	save[6]=save7; save[7]=save8;  save[8]=save9;
	save[9]=save10;

	playing[0]=playing1; playing[1]=playing2;  playing[2]=playing3;
	playing[3]=playing4; playing[4]=playing5;  playing[5]=playing6;
	playing[6]=playing7; playing[7]=playing8;  playing[8]=playing9;
	playing[9]=playing10;

	feedbackL[0]=feedbackL1; feedbackL[1]=feedbackL2;  feedbackL[2]=feedbackL3;
	feedbackL[3]=feedbackL4; feedbackL[4]=feedbackL5;  feedbackL[5]=feedbackL6;
	feedbackL[6]=feedbackL7; feedbackL[7]=feedbackL8;  feedbackL[8]=feedbackL9;
	feedbackL[9]=feedbackL10;

	trigger[0]=trigger1; trigger[1]=trigger2;  trigger[2]=trigger3;
	trigger[3]=trigger4; trigger[4]=trigger5;  trigger[5]=trigger6;
	trigger[6]=trigger7; trigger[7]=trigger8;  trigger[8]=trigger9;
	trigger[9]=trigger10;

	gap[0]=gap1; gap[1]=gap2;  gap[2]=gap3;
	gap[3]=gap4; gap[4]=gap5;  gap[5]=gap6;
	gap[6]=gap7; gap[7]=gap8;  gap[8]=gap9;
	gap[9]=gap10;
	gap[10]=gap11; gap[11]=gap12;  gap[12]=gap13;
	gap[13]=gap14; gap[14]=gap15;  gap[15]=gap16;
	gap[16]=gap17; gap[17]=gap18;  gap[18]=gap19;
	gap[19]=gap20;

	switch_lists[0]=switch_lists1; switch_lists[1]=switch_lists2;  switch_lists[2]=switch_lists3;
	switch_lists[3]=switch_lists4; switch_lists[4]=switch_lists5;  switch_lists[5]=switch_lists6;
	switch_lists[6]=switch_lists7; switch_lists[7]=switch_lists8;  switch_lists[8]=switch_lists9;
	switch_lists[9]=switch_lists10;

	switch_quota[0]=switch_quota1; switch_quota[1]=switch_quota2;  switch_quota[2]=switch_quota3;
	switch_quota[3]=switch_quota4; switch_quota[4]=switch_quota5;  switch_quota[5]=switch_quota6;
	switch_quota[6]=switch_quota7; switch_quota[7]=switch_quota8;  switch_quota[8]=switch_quota9;
	switch_quota[9]=switch_quota10;

	switch_quota_Tb[0]=switch_quota_Tb1; switch_quota_Tb[1]=switch_quota_Tb2;  switch_quota_Tb[2]=switch_quota_Tb3;
	switch_quota_Tb[3]=switch_quota_Tb4; switch_quota_Tb[4]=switch_quota_Tb5;  switch_quota_Tb[5]=switch_quota_Tb6;
	switch_quota_Tb[6]=switch_quota_Tb7; switch_quota_Tb[7]=switch_quota_Tb8;  switch_quota_Tb[8]=switch_quota_Tb9;
	switch_quota_Tb[9]=switch_quota_Tb10;

	 if(!table_exists("settings"))create_settings();
	 if(!table_exists("key_pecks"))create_keypecks();
	 if(!table_exists("nidaq")) create_nidaq();
	 if(!table_exists("channels")) create_channels();
	 if(!table_exists("animals"))Create_animals();
	 if(!table_exists("sessions"))Create_sessions();

	 try{NIDAQ->Active=true;} // check if table is okay:
	 catch(...)
	 {
		Application->MessageBox(L"could not find NIDAQ table, keys will be assigned with default values", NULL, MB_OK);
	 }

	 try{current->Active=true;}
	 catch(...)
	 {
		Application->MessageBox(L"could not find setting table, recorder will start using default values", NULL, MB_OK);
	 }
	 try{NIDAQ->Active=true;}
	 catch(...){Application->MessageBox(L"could not find NIDAQ table. Key pecks wont register", NULL, MB_OK); }
	 try{keypeckTable->Active=true;}
	 catch(...){Application->MessageBox(L"could not find key-pecks table. Training records wont register", NULL, MB_OK); }
	 try{updateIO(true);}  // some additional variable settings and data card state settings
	 catch(...){Application->MessageBox(L"data array error, the NIDAQ table might be corrupted", NULL, MB_OK);}


	 //check if tables are okay:
	 InitBirds();
	 if(!LoadDefault()) try{set_default();}  // setting or loading default state
	 catch(...){Application->MessageBox(L"set default error", NULL, MB_OK);}
	 TrainingClick(this);  // configure data card
	 Update_Labels();

	 for(int bird=0;bird<10;bird++)
	 {
		current->RecNo=bird+3;
		biofeedback[bird]->Checked=current->FieldValues["bio_feedback"];
		if(biofeedback[bird]->Checked)feedbackL[bird]->Visible=true;
	 }

	 if(current->Active)retrieve_settings();
	 for(int i=0;i<10;i++) { str=m_Birds[i]->GetStringValue(vBirdName); name[i]=str; }//.ToInt(); }
     for(int i=0;i<10;i++)
     {
     // session_playback_list[i]->ItemIndex=0;
     // session_playback_list[i]->Visible=false;
	 // playback_list[i]->Visible=false;
     }
	 CheckDates();
	 InitBirds();
	 for(int i=0;i<10;i++)Handle_training_state(i);
	 MaxDurChange(this);
	 speedChange(this);
	 display_typesClick(this);
	 pre_thresh_bufferChange(this);
	 wait->Visible=false;
}


void TForm1::retrieve_settings()
{
	   current->RecNo=3;
	   str=current->FieldValues["bird_ID"];
	   SetBirdString(ztBird1,vBirdName,str.c_str());
	   ID1->Caption=str;
	   hatch1->Date=current->FieldValues["hatching_date"];
	   start1->Date=current->FieldValues["start_training"];
	   end1->Date=current->FieldValues["stop_training"];

	   current->RecNo=4;
	   str=current->FieldValues["bird_ID"];
	   SetBirdString(ztBird2,vBirdName,str.c_str());
	   ID2->Caption=str;
	   hatch2->Date=current->FieldValues["hatching_date"];
	   start2->Date=current->FieldValues["start_training"];
	   end2->Date=current->FieldValues["stop_training"];

	   current->RecNo=5;
	   str=current->FieldValues["bird_ID"];
	   SetBirdString(ztBird3,vBirdName,str.c_str());
	   ID3->Caption=str;
	   hatch3->Date=current->FieldValues["hatching_date"];
	   start3->Date=current->FieldValues["start_training"];
	   end3->Date=current->FieldValues["stop_training"];

	   current->RecNo=6;
	   str=current->FieldValues["bird_ID"];
	   SetBirdString(ztBird4,vBirdName,str.c_str());
	   ID4->Caption=str;
	   hatch4->Date=current->FieldValues["hatching_date"];
	   start4->Date=current->FieldValues["start_training"];
	   end4->Date=current->FieldValues["stop_training"];

		current->RecNo=7;
	   str=current->FieldValues["bird_ID"];
	   SetBirdString(ztBird5,vBirdName,str.c_str());
	   ID5->Caption=str;
	   hatch5->Date=current->FieldValues["hatching_date"];
	   start5->Date=current->FieldValues["start_training"];
	   end5->Date=current->FieldValues["stop_training"];

		current->RecNo=8;
	   str=current->FieldValues["bird_ID"];
	   SetBirdString(ztBird6,vBirdName,str.c_str());
	   ID6->Caption=str;
	   hatch6->Date=current->FieldValues["hatching_date"];
	   start6->Date=current->FieldValues["start_training"];
	   end6->Date=current->FieldValues["stop_training"];

	   current->RecNo=9;
	   str=current->FieldValues["bird_ID"];
	   SetBirdString(ztBird7,vBirdName,str.c_str());
	   ID7->Caption=str;
	   hatch7->Date=current->FieldValues["hatching_date"];
	   start7->Date=current->FieldValues["start_training"];
	   end7->Date=current->FieldValues["stop_training"];

	   current->RecNo=10;
	   str=current->FieldValues["bird_ID"];
	   SetBirdString(ztBird8,vBirdName,str.c_str());
	   ID8->Caption=str;
	   hatch8->Date=current->FieldValues["hatching_date"];
	   start8->Date=current->FieldValues["start_training"];
	   end8->Date=current->FieldValues["stop_training"];

	   current->RecNo=11;
	   str=current->FieldValues["bird_ID"];
	   SetBirdString(ztBird9,vBirdName,str.c_str());
	   ID9->Caption=str;
	   hatch9->Date=current->FieldValues["hatching_date"];
	   start9->Date=current->FieldValues["start_training"];
	   end9->Date=current->FieldValues["stop_training"];

	   current->RecNo=12;
	   str=current->FieldValues["bird_ID"];
	   SetBirdString(ztBird10,vBirdName,str.c_str());
	   ID10->Caption=str;
	   hatch10->Date=current->FieldValues["hatching_date"];
	   start10->Date=current->FieldValues["start_training"];
	   end10->Date=current->FieldValues["stop_training"];
}


//---------------------------------------------------------------------------



void TForm1::set_default()
{
     PeakFrLow=5;
     sleepTime=false;
     IsBusy=false;
     recording=false;
     NIdeviceNum=1;
     float fr=1000/Timer1->Interval;
     for(int i=0;i<20;i++)
     {
        int bird=i/2;
        Session[i].gap_timer=fr*Session[i].gap;
        Session[i].quota=10;
        Session[i].enable_quota=true;
        recdata_accumulated[bird]=0; // accumulating recording sessions over a day?
        recorded[bird]=0;
     }
     for(int i=0;i<10;i++)
     {
        str=i+1;
        SetBirdString(m_Birds[i],vBirdName,str.c_str());
        m_Birds[i]->SetLongValue(vRecordingCutoffMS,MaxDur->Position*1000);
        m_Birds[i]->SetLongValue(vRecordingDisplay,display_VisualizationOnly);
     }

     AnsiString folder="c:\\SAP\\tmpSound"; if(!DirectoryExists(folder)) ForceDirectories(folder);
     for(int i=0;i<10;i++)SetBirdString(m_Birds[i], vBirdTempFilePath, folder.c_str());
     str="c:\\SAP\\in";
     for(int i=0;i<10;i++)
     {
        str2=str+(i+1); folder=str2;
		if(!DirectoryExists(folder)) ForceDirectories(folder);
		SetBirdString(m_Birds[i],vBirdWaveFilePath, folder.c_str());
     }
     for(int i=0;i<20;i++)
     {
       Session[i].playback_list=0; // first sound list is default
       session_playback_list[i]->ItemIndex=0;
       Session[i].operant=3; // key 1 or key 2
       session_playback_control[i]->ItemIndex=3; // default is key1 | 2
       Session[i].gap=2.0; // 2 seconds default, assuming Timer interval=100ms
       Session[i].odds=0.1; // 10% odds for passive playbacks
       training_session[i]->Checked=false;
       session_panel[i]->Visible=false;
       enable_quota[i]->Checked=true;
     }
}






void __fastcall TForm1::lights_onCtrlDownClick(TObject *Sender)
{
  lightsOn->Value--;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::lights_onCtrlUpClick(TObject *Sender)
{
   lightsOn->Value++;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::lights_offCtrlDownClick(TObject *Sender)
{
  lightsOff->Value--;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::lights_offCtrlUpClick(TObject *Sender)
{
   lightsOff->Value++;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::set1Click(TObject *Sender)
{
  TztBird *bird;
  TLabel *id, *age;
  TDateTimePicker *hatch, *start_train;
  int bird_num;
  bool new_bird=false;

	   if(Sender==set1 || Sender==chang_bird1) {bird_num=1; bird=ztBird1; id=ID1; age=Age1; hatch=hatch1; start_train=start1;
												if(Sender==set1) new_bird=true;}
  else if(Sender==set2 || Sender==chang_bird2) {bird_num=2; bird=ztBird2; id=ID2; age=Age2; hatch=hatch2; start_train=start2;
												if(Sender==set2) new_bird=true;}
  else if(Sender==set3 || Sender==chang_bird3) {bird_num=3; bird=ztBird3; id=ID3; age=Age3; hatch=hatch3; start_train=start3;
												if(Sender==set3) new_bird=true;}
  else if(Sender==set4 || Sender==chang_bird4) {bird_num=4; bird=ztBird4; id=ID4; age=Age4; hatch=hatch4; start_train=start4;
												if(Sender==set4) new_bird=true;}
  else if(Sender==set5 || Sender==chang_bird5) {bird_num=5; bird=ztBird5; id=ID5; age=Age5; hatch=hatch5; start_train=start5;
												if(Sender==set5) new_bird=true;}
  else if(Sender==set6 || Sender==chang_bird6) {bird_num=6; bird=ztBird6; id=ID6; age=Age6; hatch=hatch6; start_train=start6;
												if(Sender==set6) new_bird=true;}
  else if(Sender==set7 || Sender==chang_bird7) {bird_num=7; bird=ztBird7; id=ID7; age=Age7; hatch=hatch7; start_train=start7;
												if(Sender==set7) new_bird=true;}
  else if(Sender==set8 || Sender==chang_bird8) {bird_num=8; bird=ztBird8; id=ID8; age=Age8; hatch=hatch8; start_train=start8;
												if(Sender==set8) new_bird=true;}
  else if(Sender==set9 || Sender==chang_bird9) {bird_num=9; bird=ztBird9; id=ID9; age=Age9; hatch=hatch9; start_train=start9;
												if(Sender==set9) new_bird=true;}
  else if(Sender==set10 || Sender==chang_bird10) {bird_num=10; bird=ztBird10; id=ID10; age=Age10; hatch=hatch10; start_train=start10;
												if(Sender==set10) new_bird=true;}

  if(new_bird) animals_form->datapages->ActivePageIndex=1;
  else animals_form->datapages->ActivePageIndex=0;

  if (animals_form->ShowModal() == mrOk)
  {
	if(animals_form->new_bird)new_bird=true;
	str=animals_form->name->Text;//InputBox("Bird ID", "please type bird's name", "");
	if(str=="")return;
	id->Caption=str;
	name[bird_num-1]=str;
	hatch->Date=animals_form->hatched->Date;
	age->Caption=animals_form->age->Value;
	start_train->Date=animals_form->trained->Date;
	SetBirdString(bird,vBirdName,str.c_str());
	recorded[bird_num-1]=0;
	if(current->Active)
	{
		AnsiString syll_table;
		current->RecNo=bird_num+2; current->Edit();
		current->FieldValues["bird_ID"]=str;
		current->FieldValues["hatching_date"]=hatch->Date;
		current->FieldValues["start_training"]=start_train->Date;
		current->FieldValues["song_quota"]=animals_form->song_quota->Text.ToIntDef(0);
		syll_table="syll_"; syll_table+=str;
		current->FieldValues["syllable_table"]=syll_table;
		current->Post();
	}
	if(new_bird) animals_form->Insert_bird_record();
  }
}
//---------------------------------------------------------------------------



void __fastcall TForm1::TrainingClick(TObject *Sender)
{
  /*
  if(operant->Checked)
  {
    try{configDataCard();}
    catch(...){Application->MessageBox("configDataCard error", NULL, MB_OK);}
    t1->Enabled=true; t2->Enabled=true; t3->Enabled=true; t4->Enabled=true;  // enabled the key port-line settup pannels
  }
  else
  {
    t1->Enabled=false; t2->Enabled=false; t3->Enabled=false; t4->Enabled=false;
    for(int i=0;i<9;i++)peck[i]=false;
  }
 */
}
//---------------------------------------------------------------------------

void TForm1::configDataCard()
{
  TRadioGroup *port_mode[6];
  port_mode[0]=port0; port_mode[1]=port1;port_mode[2]=port2;port_mode[3]=port3;port_mode[4]=port4; port_mode[5]=port5;
  NIdeviceNum=NInum->Value;

  read_port0=0; read_port1=0; read_port2=0;
// DAQmx Configure Code


	DAQmxCreateTask("",&read_port0);
    DAQmxCreateTask("",&read_port1);
    DAQmxCreateTask("",&read_port2);

    str="Dev"; str+=NI_device_num->Position; str+="/port0/line0:7";
	DAQmxCreateDIChan(read_port0,str.c_str(),"",DAQmx_Val_ChanForAllLines);
    str="Dev"; str+=NI_device_num->Position; str+="/port1/line0:7";
    DAQmxCreateDIChan(read_port1,str.c_str(),"",DAQmx_Val_ChanForAllLines);
    str="Dev"; str+=NI_device_num->Position; str+="/port2/line0:7";
    DAQmxCreateDIChan(read_port2,str.c_str(),"",DAQmx_Val_ChanForAllLines);

	DAQmxStartTask(read_port0);
    DAQmxStartTask(read_port1);
    DAQmxStartTask(read_port2);


 /* zt_DIG_Prt_Config(1,0,0,0);
  zt_DIG_Prt_Config(1,1,0,0);
  zt_DIG_Prt_Config(1,2,0,1);

   // newer version:
  for(int prt=0;prt<num_ports->Position;prt++)
                zt_DIG_Prt_Config(NIdeviceNum, prt, 0, port_mode[prt]->ItemIndex); */
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
      NIDAQ->RecNo=17; p17->Value=NIDAQ->FieldValues["port"];l17->Value=NIDAQ->FieldValues["line"];
      NIDAQ->RecNo=18; p18->Value=NIDAQ->FieldValues["port"];l18->Value=NIDAQ->FieldValues["line"];
      NIDAQ->RecNo=19; p19->Value=NIDAQ->FieldValues["port"];l19->Value=NIDAQ->FieldValues["line"];
      NIDAQ->RecNo=20; p20->Value=NIDAQ->FieldValues["port"];l20->Value=NIDAQ->FieldValues["line"];
      NIDAQ->RecNo=21; p21->Value=NIDAQ->FieldValues["port"];l21->Value=NIDAQ->FieldValues["line"];
      NIDAQ->RecNo=22; p22->Value=NIDAQ->FieldValues["port"];l22->Value=NIDAQ->FieldValues["line"];
      NIDAQ->RecNo=23; p23->Value=NIDAQ->FieldValues["port"];l23->Value=NIDAQ->FieldValues["line"];
      NIDAQ->RecNo=24; p24->Value=NIDAQ->FieldValues["port"];l24->Value=NIDAQ->FieldValues["line"];
      NIDAQ->RecNo=25; p25->Value=NIDAQ->FieldValues["port"];l25->Value=NIDAQ->FieldValues["line"];
      NIDAQ->RecNo=26; p26->Value=NIDAQ->FieldValues["port"];l26->Value=NIDAQ->FieldValues["line"];
      NIDAQ->RecNo=27; p27->Value=NIDAQ->FieldValues["port"];l27->Value=NIDAQ->FieldValues["line"];
      NIDAQ->RecNo=28; p28->Value=NIDAQ->FieldValues["port"];l28->Value=NIDAQ->FieldValues["line"];
      NIDAQ->RecNo=29; p29->Value=NIDAQ->FieldValues["port"];l29->Value=NIDAQ->FieldValues["line"];
      NIDAQ->RecNo=30; p30->Value=NIDAQ->FieldValues["port"];l30->Value=NIDAQ->FieldValues["line"];
      NIDAQ->RecNo=31; p31->Value=NIDAQ->FieldValues["port"];l31->Value=NIDAQ->FieldValues["line"];
      NIDAQ->RecNo=32; p32->Value=NIDAQ->FieldValues["port"];l32->Value=NIDAQ->FieldValues["line"];
      NIDAQ->RecNo=33; p33->Value=NIDAQ->FieldValues["port"];l33->Value=NIDAQ->FieldValues["line"];
      NIDAQ->RecNo=34; p34->Value=NIDAQ->FieldValues["port"];l34->Value=NIDAQ->FieldValues["line"];
      NIDAQ->RecNo=35; p35->Value=NIDAQ->FieldValues["port"];l35->Value=NIDAQ->FieldValues["line"];
      NIDAQ->RecNo=36; p36->Value=NIDAQ->FieldValues["port"];l36->Value=NIDAQ->FieldValues["line"];
      NIDAQ->RecNo=37; p37->Value=NIDAQ->FieldValues["port"];l37->Value=NIDAQ->FieldValues["line"];
      NIDAQ->RecNo=38; p38->Value=NIDAQ->FieldValues["port"];l38->Value=NIDAQ->FieldValues["line"];
      NIDAQ->RecNo=39; p39->Value=NIDAQ->FieldValues["port"];l39->Value=NIDAQ->FieldValues["line"];
      NIDAQ->RecNo=40; p40->Value=NIDAQ->FieldValues["port"];l40->Value=NIDAQ->FieldValues["line"];


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

     NIDAQ->RecNo=17; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p17->Value;
     NIDAQ->FieldValues["line"]=l17->Value;

     NIDAQ->RecNo=18; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p18->Value;
     NIDAQ->FieldValues["line"]=l18->Value;

     NIDAQ->RecNo=19; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p19->Value;
     NIDAQ->FieldValues["line"]=l19->Value;

     NIDAQ->RecNo=20; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p20->Value;
     NIDAQ->FieldValues["line"]=l20->Value;

     NIDAQ->RecNo=21; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p21->Value;
     NIDAQ->FieldValues["line"]=l21->Value;

     NIDAQ->RecNo=22; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p22->Value;
     NIDAQ->FieldValues["line"]=l22->Value;

     NIDAQ->RecNo=23; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p23->Value;
     NIDAQ->FieldValues["line"]=l23->Value;

     NIDAQ->RecNo=24; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p24->Value;
     NIDAQ->FieldValues["line"]=l24->Value;

     NIDAQ->RecNo=25; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p25->Value;
     NIDAQ->FieldValues["line"]=l25->Value;

     NIDAQ->RecNo=26; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p26->Value;
     NIDAQ->FieldValues["line"]=l26->Value;

     NIDAQ->RecNo=27; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p27->Value;
     NIDAQ->FieldValues["line"]=l27->Value;

     NIDAQ->RecNo=28; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p28->Value;
     NIDAQ->FieldValues["line"]=l28->Value;

     NIDAQ->RecNo=29; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p29->Value;
     NIDAQ->FieldValues["line"]=l29->Value;


     NIDAQ->RecNo=30; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p30->Value;
     NIDAQ->FieldValues["line"]=l30->Value;

     NIDAQ->RecNo=31; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p31->Value;
     NIDAQ->FieldValues["line"]=l31->Value;

     NIDAQ->RecNo=32; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p32->Value;
     NIDAQ->FieldValues["line"]=l32->Value;

     NIDAQ->RecNo=33; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p33->Value;
     NIDAQ->FieldValues["line"]=l33->Value;

     NIDAQ->RecNo=34; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p34->Value;
     NIDAQ->FieldValues["line"]=l34->Value;

     NIDAQ->RecNo=35; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p35->Value;
     NIDAQ->FieldValues["line"]=l35->Value;

     NIDAQ->RecNo=36; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p36->Value;
     NIDAQ->FieldValues["line"]=l36->Value;

     NIDAQ->RecNo=37; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p37->Value;
     NIDAQ->FieldValues["line"]=l37->Value;

     NIDAQ->RecNo=38; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p38->Value;
     NIDAQ->FieldValues["line"]=l38->Value;

     NIDAQ->RecNo=39; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p39->Value;
     NIDAQ->FieldValues["line"]=l39->Value;

     NIDAQ->RecNo=40; NIDAQ->Edit();
     NIDAQ->FieldValues["port"]=p40->Value;
     NIDAQ->FieldValues["line"]=l40->Value;

     NIDAQ->Post();
   }

}


void __fastcall TForm1::p1sDownClick(TObject *Sender)
{
  if(Sender==p1s)p1->Value--;
  else if(Sender==p2s)p2->Value--;
  else if(Sender==p3s)p3->Value--;
  else if(Sender==p4s)p4->Value--;
  else if(Sender==p5s)p5->Value--;
  else if(Sender==p6s)p6->Value--;
  else if(Sender==p7s)p7->Value--;
  else if(Sender==p8s)p8->Value--;
  else if(Sender==p9s)p9->Value--;
  else if(Sender==p10s)p10->Value--;
  else if(Sender==p11s)p11->Value--;
  else if(Sender==p12s)p12->Value--;
  else if(Sender==p13s)p13->Value--;
  else if(Sender==p14s)p14->Value--;
  else if(Sender==p15s)p15->Value--;
  else if(Sender==p16s)p16->Value--;
  else if(Sender==p17s)p17->Value--;
  else if(Sender==p18s)p18->Value--;
  else if(Sender==p19s)p19->Value--;
  else if(Sender==p20s)p20->Value--;
  else if(Sender==p21s)p21->Value--;
  else if(Sender==p22s)p22->Value--;
  else if(Sender==p23s)p23->Value--;
  else if(Sender==p24s)p24->Value--;
  else if(Sender==p25s)p25->Value--;
  else if(Sender==p26s)p26->Value--;
  else if(Sender==p27s)p27->Value--;
  else if(Sender==p28s)p28->Value--;
  else if(Sender==p29s)p29->Value--;
  else if(Sender==p30s)p30->Value--;
  else if(Sender==p31s)p31->Value--;
  else if(Sender==p32s)p32->Value--;
  else if(Sender==p33s)p33->Value--;
  else if(Sender==p34s)p34->Value--;
  else if(Sender==p35s)p35->Value--;
  else if(Sender==p36s)p36->Value--;
  else if(Sender==p37s)p37->Value--;
  else if(Sender==p38s)p38->Value--;
  else if(Sender==p39s)p39->Value--;
  else if(Sender==p40s)p40->Value--;

  updateIO(false);// save the data
}
//---------------------------------------------------------------------------

void __fastcall TForm1::p1sUpClick(TObject *Sender)
{
  if(Sender==p1s)p1->Value++;
  else if(Sender==p2s)p2->Value++;
  else if(Sender==p3s)p3->Value++;
  else if(Sender==p4s)p4->Value++;
  else if(Sender==p5s)p5->Value++;
  else if(Sender==p6s)p6->Value++;
  else if(Sender==p7s)p7->Value++;
  else if(Sender==p8s)p8->Value++;
  else if(Sender==p9s)p9->Value++;
  else if(Sender==p10s)p10->Value++;
  else if(Sender==p11s)p11->Value++;
  else if(Sender==p12s)p12->Value++;
  else if(Sender==p13s)p13->Value++;
  else if(Sender==p14s)p14->Value++;
  else if(Sender==p15s)p15->Value++;
  else if(Sender==p16s)p16->Value++;
  else if(Sender==p17s)p17->Value++;
  else if(Sender==p18s)p18->Value++;
  else if(Sender==p19s)p19->Value++;
  else if(Sender==p20s)p20->Value++;
  else if(Sender==p21s)p21->Value++;
  else if(Sender==p22s)p22->Value++;
  else if(Sender==p23s)p23->Value++;
  else if(Sender==p24s)p24->Value++;
  else if(Sender==p25s)p25->Value++;
  else if(Sender==p26s)p26->Value++;
  else if(Sender==p27s)p27->Value++;
  else if(Sender==p28s)p28->Value++;
  else if(Sender==p29s)p29->Value++;
  else if(Sender==p30s)p30->Value++;
  else if(Sender==p31s)p31->Value++;
  else if(Sender==p32s)p32->Value++;
  else if(Sender==p33s)p33->Value++;
  else if(Sender==p34s)p34->Value++;
  else if(Sender==p35s)p35->Value++;
  else if(Sender==p36s)p36->Value++;
  else if(Sender==p37s)p37->Value++;
  else if(Sender==p38s)p38->Value++;
  else if(Sender==p39s)p39->Value++;
  else if(Sender==p40s)p40->Value++;

  updateIO(false);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::l1sDownClick(TObject *Sender)
{
  if(Sender==l1s)l1->Value--;
  else if(Sender==l2s)l2->Value--;
  else if(Sender==l3s)l3->Value--;
  else if(Sender==l4s)l4->Value--;
  else if(Sender==l5s)l5->Value--;
  else if(Sender==l6s)l6->Value--;
  else if(Sender==l7s)l7->Value--;
  else if(Sender==l8s)l8->Value--;
  else if(Sender==l9s)l9->Value--;
  else if(Sender==l10s)l10->Value--;
  else if(Sender==l11s)l11->Value--;
  else if(Sender==l12s)l12->Value--;
  else if(Sender==l13s)l13->Value--;
  else if(Sender==l14s)l14->Value--;
  else if(Sender==l15s)l15->Value--;
  else if(Sender==l16s)l16->Value--;
  else if(Sender==l17s)l17->Value--;
  else if(Sender==l18s)l18->Value--;
  else if(Sender==l19s)l19->Value--;
  else if(Sender==l20s)l20->Value--;
  else if(Sender==l21s)l21->Value--;
  else if(Sender==l22s)l22->Value--;
  else if(Sender==l23s)l23->Value--;
  else if(Sender==l24s)l24->Value--;
  else if(Sender==l25s)l25->Value--;
  else if(Sender==l26s)l26->Value--;
  else if(Sender==l27s)l27->Value--;
  else if(Sender==l28s)l28->Value--;
  else if(Sender==l29s)l29->Value--;
  else if(Sender==l30s)l30->Value--;
  else if(Sender==l31s)l31->Value--;
  else if(Sender==l32s)l32->Value--;
  else if(Sender==l33s)l33->Value--;
  else if(Sender==l34s)l34->Value--;
  else if(Sender==l35s)l35->Value--;
  else if(Sender==l36s)l36->Value--;
  else if(Sender==l37s)l37->Value--;
  else if(Sender==l38s)l38->Value--;
  else if(Sender==l39s)l39->Value--;
  else if(Sender==l40s)l40->Value--;

  updateIO(false);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::l1sUpClick(TObject *Sender)
{
  if(Sender==l1s)l1->Value++;
  else if(Sender==l2s)l2->Value++;
  else if(Sender==l3s)l3->Value++;
  else if(Sender==l4s)l4->Value++;
  else if(Sender==l5s)l5->Value++;
  else if(Sender==l6s)l6->Value++;
  else if(Sender==l7s)l7->Value++;
  else if(Sender==l8s)l8->Value++;
  else if(Sender==l9s)l9->Value++;
  else if(Sender==l10s)l10->Value++;
  else if(Sender==l11s)l11->Value++;
  else if(Sender==l12s)l12->Value++;
  else if(Sender==l13s)l13->Value++;
  else if(Sender==l14s)l14->Value++;
  else if(Sender==l15s)l15->Value++;
  else if(Sender==l16s)l16->Value++;
  else if(Sender==l17s)l17->Value++;
  else if(Sender==l18s)l18->Value++;
  else if(Sender==l19s)l19->Value++;
  else if(Sender==l20s)l20->Value++;
  else if(Sender==l21s)l21->Value++;
  else if(Sender==l22s)l22->Value++;
  else if(Sender==l23s)l23->Value++;
  else if(Sender==l24s)l24->Value++;
  else if(Sender==l25s)l25->Value++;
  else if(Sender==l26s)l26->Value++;
  else if(Sender==l27s)l27->Value++;
  else if(Sender==l28s)l28->Value++;
  else if(Sender==l29s)l29->Value++;
  else if(Sender==l30s)l30->Value++;
  else if(Sender==l31s)l31->Value++;
  else if(Sender==l32s)l32->Value++;
  else if(Sender==l33s)l33->Value++;
  else if(Sender==l34s)l34->Value++;
  else if(Sender==l35s)l35->Value++;
  else if(Sender==l36s)l36->Value++;
  else if(Sender==l37s)l37->Value++;
  else if(Sender==l38s)l38->Value++;
  else if(Sender==l39s)l39->Value++;
  else if(Sender==l40s)l40->Value++;
  updateIO(false);
}
//---------------------------------------------------------------------------



void __fastcall TForm1::end1Change(TObject *Sender)
{
   TDateTimePicker *end_date;
   int bird;
   if(Sender==end1){bird=3; end_date=end1;}
   else if(Sender==end2){bird=4; end_date=end2;}
   else if(Sender==end3){bird=4; end_date=end3;}
   else {bird=6; end_date=end4;}

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

 ID1->Caption=ztBird1->GetStringValue(vBirdName);
 ID2->Caption=ztBird2->GetStringValue(vBirdName);
 ID3->Caption=ztBird3->GetStringValue(vBirdName);
 ID4->Caption=ztBird4->GetStringValue(vBirdName);
 ID5->Caption=ztBird5->GetStringValue(vBirdName);
 ID6->Caption=ztBird6->GetStringValue(vBirdName);
 ID7->Caption=ztBird7->GetStringValue(vBirdName);
 ID8->Caption=ztBird8->GetStringValue(vBirdName);
 ID9->Caption=ztBird9->GetStringValue(vBirdName);
 ID10->Caption=ztBird10->GetStringValue(vBirdName);

 Age1->Caption=(int)Date()-(int)hatch1->Date;  animal_age[0]=Age1->Caption;
 Age2->Caption=(int)Date()-(int)hatch2->Date;  animal_age[1]=Age2->Caption;
 Age3->Caption=(int)Date()-(int)hatch3->Date;  animal_age[2]=Age3->Caption;
 Age4->Caption=(int)Date()-(int)hatch4->Date;  animal_age[3]=Age4->Caption;
 Age5->Caption=(int)Date()-(int)hatch5->Date;  animal_age[4]=Age5->Caption;
 Age6->Caption=(int)Date()-(int)hatch6->Date;  animal_age[5]=Age6->Caption;
 Age7->Caption=(int)Date()-(int)hatch7->Date;  animal_age[6]=Age7->Caption;
 Age8->Caption=(int)Date()-(int)hatch8->Date;  animal_age[7]=Age8->Caption;
 Age9->Caption=(int)Date()-(int)hatch9->Date;  animal_age[8]=Age9->Caption;
 Age10->Caption=(int)Date()-(int)hatch10->Date; animal_age[9]=Age10->Caption;
}



void __fastcall TForm1::SaveDefaultClick(TObject *Sender)
{
   save_all_configs();

}
//---------------------------------------------------------------------------

void TForm1::retrieve_session_attributes(int i)
{
    sessions->Active=true;
	sessions->RecNo=i+1;
	Session[i].enabled=(int)sessions->FieldValues["enabled"];
    training_session[i]->Checked=Session[i].enabled; update_session(i);
    if(training_session[i]->Checked)session_panel[i]->Visible=true;

    Session[i].state=(int)sessions->FieldValues["state"];
    if(Session[i].state)Box[i/2]->Brush->Color=clTeal;

    Session[i].start_hour=(int)sessions->FieldValues["start_hour"];
    Session[i].start_min=(int)sessions->FieldValues["start_min"];
    Session[i].end_hour=(int)sessions->FieldValues["end_hour"];
    Session[i].end_min=(int)sessions->FieldValues["end_min"];
    session_startL[i]->Caption=calc_time_label(Session[i].start_hour, Session[i].start_min);
    session_endL[i]->Caption=calc_time_label(Session[i].end_hour, Session[i].end_min);

    Session[i].random_onset=(int)sessions->FieldValues["random_onset"];
    Session[i].random_onset_delay=(int)sessions->FieldValues["random_onset_delay"];
    onset_timing[i]->ItemIndex=(int)Session[i].random_onset;


    str=sessions->FieldValues["sound_folder"];
    if(DirectoryExists(str))playbacks_[i]->Directory=str;
    playbacks_folder[i]->Text=playbacks_[i]->Directory+"\\";

    Session[i].enable_quota=(int)sessions->FieldValues["enable_quota"];
    enable_quota[i]->Checked=Session[i].enable_quota;
    playbacks_quota[i]->Visible=enable_quota[i]->Checked;

    Session[i].quota=(int)sessions->FieldValues["quota"];
    playbacks_quota[i]->Value=Session[i].quota;

    Session[i].song_played=0;//(int)sessions->FieldValues["song_played"];

    Session[i].gap=(float)sessions->FieldValues["gap"];
    gap[i]->Value=Session[i].gap;

    Session[i].gap_timer=sessions->FieldValues["gap_timer"];

    Session[i].operant=(int)sessions->FieldValues["operant"];
    session_playback_control[i]->ItemIndex=Session[i].operant;
    if(!Session[i].operant)passive_panel[i]->Visible=true;

    playbacks_[i]->ItemIndex=sessions->FieldValues["song_index"];

    Session[i].odds=(float)sessions->FieldValues["odds"];
    //passive_odds_UpDown[i]->Position=Session[i].odds*100;
    passive_odds[i]->Value=Session[i].odds;
    int ii=passive_odds_UpDown[i]->Position;

    Session[i].playback_list=sessions->FieldValues["playback_list"];
    session_playback_list[i]->ItemIndex=Session[i].playback_list;

  if(session_playback_list[i]->ItemIndex==0)// sounds list 1
  {
        session_playback_control[i]->Enabled=true;
        Session[i].playback_list=0;
  }
  else if(session_playback_list[i]->ItemIndex==1)// sounds list 1a
  {
        session_playback_control[i]->Enabled=true;
        Session[i].playback_list=1;

  }
  else if(session_playback_list[i]->ItemIndex==2)// Key1->list8 ; Key2->list8a
  {
     session_playback_control[i]->ItemIndex=3;
     Session[i].operant=3;
     session_playback_control[i]->Enabled=false;
     passive_panel[i]->Visible=false;
     Session[i].playback_list=2;

  }
}


void TForm1::save_session_attributes(int i)
{
    sessions->Active=true;
    sessions->RecNo=i+1;
    sessions->Edit();
    sessions->FieldValues["enabled"]=(int)Session[i].enabled;
    sessions->FieldValues["state"]=(int)Session[i].state;
    sessions->FieldValues["start_hour"]=(int)Session[i].start_hour;
    sessions->FieldValues["start_min"]=(int)Session[i].start_min;
    sessions->FieldValues["end_hour"]=(int)Session[i].end_hour;
    sessions->FieldValues["end_min"]=(int)Session[i].end_min;
    sessions->FieldValues["random_onset"]=(int)Session[i].random_onset;
    sessions->FieldValues["random_onset_delay"]=(int)Session[i].random_onset_delay;
    sessions->FieldValues["playback_list"]=Session[i].playback_list;
    sessions->FieldValues["sound_folder"]=playbacks_[i]->Directory;
    sessions->FieldValues["enable_quota"]=(int)Session[i].enable_quota;
    sessions->FieldValues["quota"]=(int)Session[i].quota;
    sessions->FieldValues["song_played"]=(int)Session[i].song_played;
    sessions->FieldValues["gap"]=(float)Session[i].gap;
    sessions->FieldValues["gap_timer"]=(int)Session[i].gap_timer;
    sessions->FieldValues["operant"]=(int)Session[i].operant;
    sessions->FieldValues["odds"]=(float)Session[i].odds;
    int index=playbacks_[i]->ItemIndex; if(index<0)index=0;
    sessions->FieldValues["song_index"]=index;
    sessions->Post();
}




bool TForm1::LoadDefault()
{

  for(int i=0;i<20;i++) retrieve_session_attributes(i);

  for(int i=0;i<10;i++)
  {
        retrieve_channel_attributes(m_Birds[i],i+1);
        update_chan_displays(i);
  }
  return(1);
}


void __fastcall TForm1::hatch1Change(TObject *Sender)
{
   TDateTimePicker *hatch_date;
   int bird;
   if(Sender==hatch1){bird=3; hatch_date=hatch1;}
   else if(Sender==hatch2){bird=4; hatch_date=hatch2;}
   else if(Sender==hatch3){bird=5; hatch_date=hatch3;}
   else if(Sender==hatch4){bird=6; hatch_date=hatch4;}
   else if(Sender==hatch5){bird=7; hatch_date=hatch5;}
   else if(Sender==hatch6){bird=8; hatch_date=hatch6;}
   else if(Sender==hatch7){bird=9; hatch_date=hatch7;}
   else if(Sender==hatch8){bird=10; hatch_date=hatch8;}
   else if(Sender==hatch9){bird=11; hatch_date=hatch9;}
   else if(Sender==hatch10){bird=12; hatch_date=hatch10;}
   else return;

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
   messages->Clear();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::options1Click(TObject *Sender)
{
 TztBird* bird;
 CZT_OCX_BirdProps props;
 TCheckBox* active;
 TBitBtn *option_button;
 TTrackBar *thresh, *gain;
 AnsiString config_file;
 int bird_num;

if(Sender==options1)     {bird_num=0; option_button=options1; bird=ztBird1; thresh=trckThreshold1; gain=gain1; active=active1; config_file="c:\\sap\\chan1.txt";}
else if(Sender==options2) {bird_num=1;option_button=options2; bird=ztBird2;thresh=trckThreshold2; gain=gain2;active=active2;config_file="c:\\sap\\chan2.txt";}
 else if(Sender==options3) {bird_num=2;option_button=options3; bird=ztBird3;thresh=trckThreshold3; gain=gain3;active=active3;config_file="c:\\sap\\chan3.txt";}
  else if(Sender==options4) {bird_num=3;option_button=options4; bird=ztBird4;thresh=trckThreshold4; gain=gain4;active=active4;config_file="c:\\sap\\chan4.txt";}
   else if(Sender==options5) {bird_num=4;option_button=options5; bird=ztBird5;thresh=trckThreshold5; gain=gain5;active=active5;config_file="c:\\sap\\chan5.txt";}
	else if(Sender==options6) {bird_num=5;option_button=options6; bird=ztBird6;thresh=trckThreshold6; gain=gain6;active=active6;config_file="c:\\sap\\chan6.txt";}
	 else if(Sender==options7) {bird_num=6;option_button=options7; bird=ztBird7;thresh=trckThreshold7; gain=gain7;active=active7;config_file="c:\\sap\\chan7.txt";}
	  else if(Sender==options8) {bird_num=7;option_button=options8; bird=ztBird8;thresh=trckThreshold8; gain=gain8;active=active8;config_file="c:\sap\\chan8.txt";}
	   else if(Sender==options9) {bird_num=8;option_button=options9; bird=ztBird9;thresh=trckThreshold9; gain=gain9;active=active9;config_file="c:\\sap\\chan9.txt";}
		else if(Sender==options10) {bird_num=9;option_button=options10; bird=ztBird10;thresh=trckThreshold10; gain=gain10;active=active10;config_file="c:\\sap\\chan10.txt";}
 // bold the active channel label to make it easier to identify

 options1->Font->Style=TFontStyles(); options2->Font->Style=TFontStyles(); options3->Font->Style=TFontStyles();
 options4->Font->Style=TFontStyles(); options5->Font->Style=TFontStyles(); options6->Font->Style=TFontStyles();
 options7->Font->Style=TFontStyles(); options8->Font->Style=TFontStyles(); options9->Font->Style=TFontStyles();
 options10->Font->Style=TFontStyles();
 option_button->Font->Style=TFontStyles()<< fsBold;

 if(btnStartEngine->Enabled)
 {
   // we keep here the original properties of the bird, before changing them...
   props.RetrieveAllValues(bird);
   str5=props.m_sBirdName.c_str();//bird->GetStringValue(vBirdName);
   bird->InvokeConfigureDlg(); // call the change properties menu.
   props.RetrieveAllValues(bird); // we now look at the properties once again, after changing some...
   // has the bird name changed? If yes, update the database...
   str=props.m_sBirdName.c_str();
   if(str!=str5)
   {
	recorded[bird_num]=0;
	current->RecNo=bird_num+3; current->Edit();
	current->FieldValues["bird_ID"]=str;
	current->Post();
	str="Important: You have changed bird name but this will not register properly with other SAP modules (e.g., with Live Processing) unless you go to the Animal tab and update the bird name and details.";
	MessageDlgPos(str, mtWarning, TMsgDlgButtons() << mbOK, 0, 10, 80*bird_num);
   }
   update_chan_displays(bird_num);


 }

 else
 {
	str="\r To set this chanel you must stop the recording \r\r ";
	str+="\r Sound input: ";
	str+=m_Birds[bird_num]->GetStringValue(vAudioSrcDescription);
	str+="\r\r Sound playbacks: ";
	str+=m_Birds[bird_num]->GetStringValue(vAudioTrgDescription);
	MessageDlgPos(str, mtInformation, TMsgDlgButtons() << mbOK, 0, 10, 80*bird_num);
 }

}


//---------------------------------------------------------------------------


void __fastcall TForm1::quota_UpDown1Changing(TObject *Sender, bool &AllowChange)
{
 int i=0;
 if(Sender==quota_UpDown1) i=0;
 else if(Sender==quota_UpDown2) i=1;
 else if(Sender==quota_UpDown3) i=2;
 else if(Sender==quota_UpDown4) i=3;
 else if(Sender==quota_UpDown5) i=4;
 else if(Sender==quota_UpDown6) i=5;
 else if(Sender==quota_UpDown7) i=6;
 else if(Sender==quota_UpDown8) i=7;
 else if(Sender==quota_UpDown9) i=8;
 else if(Sender==quota_UpDown10)i=9;
 else if(Sender==quota_UpDown11)i=10;
 else if(Sender==quota_UpDown12)i=11;
 else if(Sender==quota_UpDown13)i=12;
 else if(Sender==quota_UpDown14)i=13;
 else if(Sender==quota_UpDown15)i=14;
 else if(Sender==quota_UpDown16)i=15;
 else if(Sender==quota_UpDown17)i=16;
 else if(Sender==quota_UpDown18)i=17;
 else if(Sender==quota_UpDown19)i=18;
 else if(Sender==quota_UpDown20)i=19;

 playbacks_quota[i]->Value=quota_UpDown[i]->Position;
 Session[i].quota=playbacks_quota[i]->Value;

}
//---------------------------------------------------------------------------





void __fastcall TForm1::gain1Change(TObject *Sender)
{
  TztBird* Bird; TNumLab *gainN; TTrackBar *gainTb;
  if(Sender==gain1)      {gainTb=gain1; gainN=gainN1; Bird=m_Birds[0];}
  else if(Sender==gain2) {gainTb=gain2; gainN=gainN2; Bird=m_Birds[1];}
  else if(Sender==gain3) {gainTb=gain3; gainN=gainN3; Bird=m_Birds[2];}
  else if(Sender==gain4) {gainTb=gain4; gainN=gainN4; Bird=m_Birds[3];}
  else if(Sender==gain5) {gainTb=gain5; gainN=gainN5; Bird=m_Birds[4];}
  else if(Sender==gain6) {gainTb=gain6; gainN=gainN6; Bird=m_Birds[5];}
  else if(Sender==gain7) {gainTb=gain7; gainN=gainN7; Bird=m_Birds[6];}
  else if(Sender==gain8) {gainTb=gain8; gainN=gainN8; Bird=m_Birds[7];}
  else if(Sender==gain9) {gainTb=gain9; gainN=gainN9; Bird=m_Birds[8];}
  else if(Sender==gain10){gainTb=gain10; gainN=gainN10; Bird=m_Birds[9];}

  gainN->Value=-gainTb->Position;//(-gainTb->Position/100.0);
  Bird->SetFloatValue(vPeakGain,gainN->Value);

}
//---------------------------------------------------------------------------



void __fastcall TForm1::ztBird1BirdNotification(TObject *Sender,
      long nEventID)
{
        int i;
        if(Sender==ztBird1)i=0;
        else if(Sender==ztBird2)i=1;
        else if(Sender==ztBird3)i=2;
        else if(Sender==ztBird4)i=3;
        else if(Sender==ztBird5)i=4;
        else if(Sender==ztBird6)i=5;
        else if(Sender==ztBird7)i=6;
        else if(Sender==ztBird8)i=7;
        else if(Sender==ztBird9)i=8;
        else if(Sender==ztBird10)i=9;


        using namespace ztBirdEngine;
        switch(nEventID)
        {

                case ztbe_PeakCounterTrigger:
                if(trigger[i]->Down)m_Birds[i]->TriggerSound();
                break;
                case ztbe_PlaySoundCompleted:
                // un-mute
                m_Birds[i]->SetLongValue(vMuteBirdData,0);
                break;
                case ztbe_PeakRecordingStart:
                recdata_accumulated[i]++;
                recorded_bar[i]->Position=recdata_accumulated[i];
        }
}
//---------------------------------------------------------------------------





void __fastcall TForm1::feedback_delay1Change(TObject *Sender)
{
    if(Sender==feedback_delay1)
    {
        delay1->Value=feedback_delay1->Position;
        ztBird1->SetLongValue(vPeakTrigger, delay1->Value);
    }
    else if(Sender==feedback_delay2)
    {
        delay2->Value=feedback_delay2->Position;
        ztBird2->SetLongValue(vPeakTrigger, delay2->Value);
    }
     else if(Sender==feedback_delay3)
    {
        delay3->Value=feedback_delay3->Position;
        ztBird3->SetLongValue(vPeakTrigger, delay3->Value);
    }
     else if(Sender==feedback_delay4)
    {
        delay4->Value=feedback_delay4->Position;
        ztBird4->SetLongValue(vPeakTrigger, delay4->Value);
    }

}
//---------------------------------------------------------------------------


void __fastcall TForm1::NI_device_numChange(TObject *Sender)
{
  NInum->Value=NI_device_num->Position;
  NIdeviceNum=NI_device_num->Position;
  configDataCard();
}
//---------------------------------------------------------------------------





void __fastcall TForm1::start1Change(TObject *Sender)
{
   TDateTimePicker *start_date;
   int bird;
   if(Sender==start1){bird=3; start_date=start1;}
   else if(Sender==start2){bird=4; start_date=start2;}
   else if(Sender==start3){bird=5; start_date=start3;}
   else {bird=6; start_date=start4;}

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
  else if(Sender==trigger4) {trigger=trigger4;tl=tl4; feedback_delay=feedback_delay4; delayN=delay4; }
  else if(Sender==trigger5) {trigger=trigger5;tl=tl5; feedback_delay=feedback_delay5; delayN=delay5; }
  else if(Sender==trigger6) {trigger=trigger6;tl=tl6; feedback_delay=feedback_delay6; delayN=delay6; }
  else if(Sender==trigger7) {trigger=trigger7;tl=tl7; feedback_delay=feedback_delay7; delayN=delay7; }
  else if(Sender==trigger8) {trigger=trigger8;tl=tl8; feedback_delay=feedback_delay8; delayN=delay8; }
  else if(Sender==trigger9) {trigger=trigger9;tl=tl9; feedback_delay=feedback_delay9; delayN=delay9; }
  else if(Sender==trigger10) {trigger=trigger10;tl=tl10; feedback_delay=feedback_delay10; delayN=delay10; }


  if(trigger->Down)
  {trigger->Caption="On"; tl->Visible=true;feedback_delay->Visible=true;delayN->Visible=true;}
  else
  {trigger->Caption="Off"; tl->Visible=false;feedback_delay->Visible=false;delayN->Visible=false;}
}
//---------------------------------------------------------------------------


void __fastcall TForm1::songList1Click(TObject *Sender)
{
   /*
   TListBox* list;
   int bird;
        if(Sender==songList1) { list=songList1; bird=0; left_list=true; }
   else if(Sender==songList1a) { list=songList1a; bird=0; left_list=false; }
   else if(Sender==songList2) { list=songList2; bird=1; left_list=true; }
   else if(Sender==songList2a) { list=songList2a; bird=1; left_list=false; }
   else if(Sender==songList3) { list=songList3; bird=2; left_list=true; }
   else if(Sender==songList3a) { list=songList3a; bird=2; left_list=false; }
   else if(Sender==songList4) { list=songList4; bird=3; left_list=true; }
   else if(Sender==songList4a) { list=songList4a; bird=3; left_list=false; }
   else if(Sender==songList5) { list=songList5; bird=4; left_list=true; }
   else if(Sender==songList5a) { list=songList5a; bird=4; left_list=false; }
   else if(Sender==songList6) { list=songList6; bird=5; left_list=true; }
   else if(Sender==songList6a) { list=songList6a; bird=5; left_list=false; }
   else if(Sender==songList7) { list=songList7; bird=6; left_list=true; }
   else if(Sender==songList7a) { list=songList7a; bird=6; left_list=false; }
   else if(Sender==songList8) { list=songList8; bird=7; left_list=true; }
   else if(Sender==songList8a) { list=songList8a; bird=7; left_list=false; }
   else if(Sender==songList9) { list=songList9; bird=8; left_list=true; }
   else if(Sender==songList9a) { list=songList9a; bird=8; left_list=false; }
   else if(Sender==songList10) { list=songList10; bird=9;left_list=true;  }
   else if(Sender==songList10a) { list=songList10a; bird=9; left_list=false; }
   else return;

   if(list->ItemIndex>-1){
   SetBirdString(m_Birds[bird],vBirdWaveFile,list->Items->Strings[list->ItemIndex].c_str());
   song[bird]->Caption=ExtractFileName(list->Items->Strings[list->ItemIndex]); }
   */
}
//---------------------------------------------------------------------------







void TForm1::SaveKeyPecks(AnsiString animal, int peckNo, AnsiString theSong, AnsiString theDate, AnsiString theTime)
{
 str="INSERT INTO key_pecks";
 str+="(bird_ID, key_pecked, song_played, date, time) VALUES('";
 str+= animal; str+="',";
 str+= peckNo; str+=", '";
 str+= theSong; str+="', '";
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
  //xlReport1->Report();
}
//---------------------------------------------------------------------------





void __fastcall TForm1::effect_duration1Change(TObject *Sender)
{
  TNumLab *effect_durationNum;
  TTrackBar *effect_duration;
  if(Sender==effect_duration1) {effect_duration=effect_duration1; effect_durationNum=effect_durationNum1;}
  else if(Sender==effect_duration2) {effect_duration=effect_duration2; effect_durationNum=effect_durationNum2;}
  else if(Sender==effect_duration3) {effect_duration=effect_duration3; effect_durationNum=effect_durationNum3;}
  else if(Sender==effect_duration4) {effect_duration=effect_duration4; effect_durationNum=effect_durationNum4;}
  else if(Sender==effect_duration5) {effect_duration=effect_duration5; effect_durationNum=effect_durationNum5;}
  else if(Sender==effect_duration6) {effect_duration=effect_duration6; effect_durationNum=effect_durationNum6;}
  else if(Sender==effect_duration7) {effect_duration=effect_duration7; effect_durationNum=effect_durationNum7;}
  else if(Sender==effect_duration8) {effect_duration=effect_duration8; effect_durationNum=effect_durationNum8;}
  else if(Sender==effect_duration9) {effect_duration=effect_duration9; effect_durationNum=effect_durationNum9;}
  else if(Sender==effect_duration10) {effect_duration=effect_duration10; effect_durationNum=effect_durationNum10;}
  else if(Sender==effect_duration11) {effect_duration=effect_duration11; effect_durationNum=effect_durationNum11;}
  else if(Sender==effect_duration12) {effect_duration=effect_duration12; effect_durationNum=effect_durationNum12;}
  else if(Sender==effect_duration13) {effect_duration=effect_duration13; effect_durationNum=effect_durationNum13;}
  else if(Sender==effect_duration14) {effect_duration=effect_duration14; effect_durationNum=effect_durationNum14;}
  else if(Sender==effect_duration15) {effect_duration=effect_duration15; effect_durationNum=effect_durationNum15;}
  else if(Sender==effect_duration16) {effect_duration=effect_duration16; effect_durationNum=effect_durationNum16;}
  else if(Sender==effect_duration17) {effect_duration=effect_duration17; effect_durationNum=effect_durationNum17;}
  else if(Sender==effect_duration18) {effect_duration=effect_duration18; effect_durationNum=effect_durationNum18;}
  else if(Sender==effect_duration19) {effect_duration=effect_duration19; effect_durationNum=effect_durationNum19;}
  else if(Sender==effect_duration20) {effect_duration=effect_duration20; effect_durationNum=effect_durationNum20;}

  effect_durationNum->Value=effect_duration->Position;
}
//---------------------------------------------------------------------------











void __fastcall TForm1::effect_delay1Change(TObject *Sender)
{
  TNumLab *effect_delayNum;
  TTrackBar *effect_delay;
  if(Sender==effect_delay1){effect_delay=effect_delay1; effect_delayNum=effect_delayNum1;}
  else if(Sender==effect_delay2){effect_delay=effect_delay2; effect_delayNum=effect_delayNum2;}
  else if(Sender==effect_delay3){effect_delay=effect_delay3; effect_delayNum=effect_delayNum3;}
  else if(Sender==effect_delay4){effect_delay=effect_delay4; effect_delayNum=effect_delayNum4;}
  else if(Sender==effect_delay5){effect_delay=effect_delay5; effect_delayNum=effect_delayNum5;}
  else if(Sender==effect_delay6){effect_delay=effect_delay6; effect_delayNum=effect_delayNum6;}
  else if(Sender==effect_delay7){effect_delay=effect_delay7; effect_delayNum=effect_delayNum7;}
  else if(Sender==effect_delay8){effect_delay=effect_delay8; effect_delayNum=effect_delayNum8;}
  else if(Sender==effect_delay9){effect_delay=effect_delay9; effect_delayNum=effect_delayNum9;}
  else if(Sender==effect_delay10){effect_delay=effect_delay10; effect_delayNum=effect_delayNum10;}
  else if(Sender==effect_delay11){effect_delay=effect_delay11; effect_delayNum=effect_delayNum11;}
  else if(Sender==effect_delay12){effect_delay=effect_delay12; effect_delayNum=effect_delayNum12;}
  else if(Sender==effect_delay13){effect_delay=effect_delay13; effect_delayNum=effect_delayNum13;}
  else if(Sender==effect_delay14){effect_delay=effect_delay14; effect_delayNum=effect_delayNum14;}

  effect_delayNum->Value=effect_delay->Position;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::FormCloseQuery(TObject *Sender, bool &CanClose)
{
    Form1->Caption="Please wait";
    DAQmxStopTask(read_port0);
    DAQmxStopTask(read_port1);
    DAQmxStopTask(read_port2);

	DAQmxClearTask(read_port0);
    DAQmxClearTask(read_port1);
    DAQmxClearTask(read_port2);

   ExitClick(this);
}
//---------------------------------------------------------------------------

void TForm1::Handle_training(int this_session) // 20 sessions, 2 for each bird = 0 to 19
{
	if(!Session[this_session].enabled)return; // session is not enabled
	if(!Session[this_session].state)return; // session is enabled, but not currently active
    int bird=this_session/2;    // bird is 0-9

    // handle training is fired if 1. session is active, 2. refractory time is over
/*
    // first check if session is active, and manage gap timer
    if(!Session[this_session].enabled)return; // session is not enabled
    if(!Session[this_session].state)return; // session is enabled, but not currently active
    if(Session[this_session].gap_timer){ Session[this_session].gap_timer--; return; }
    if(Session[this_session].enable_quota && Session[this_session].song_played>=Session[this_session].quota)return;
*/
    // initialize vars
    int detector1=bird*2;  // we have two detectors per bird, hense the funny indexing method that relates a detector to a bird
    int detector2=detector1+1;
    peck[detector1]=false; peck[detector2]=false;
    bool passivePlay=false, operantPlay=false;

    // case 0: passvie playbacks
    if(Session[this_session].operant==false) // passive playbacks
    {
     if(passive_odds[this_session]->Value>0) if(random(10*(1.0/passive_odds[this_session]->Value))==0)
     {
        passivePlay=true;
        //playing[bird]->Visible=true;
     }
     //else playing[bird]->Visible=false;
    }

    // case 2: operant [key 1, key 2, both]
     if(Session[this_session].operant)
     {
      bool key1_stuck=false, key2_stuck=false;
      // first check the first detectors (keys) position
      // note that the port array holds 2 detector and 2 effectos per bird, each with port an line -- total of 8
      // so that bird*8 gives the first port, bird*8+1 is the first line, etc.
      switch((int)ports[bird*8]->Value) // switch according to port number 0, 1, 2
      {
        case 0: InState=port0_data[(int)ports[bird*8+1]->Value]; break; // find line value
        case 1: InState=port1_data[(int)ports[bird*8+1]->Value]; break;
        case 2: InState=port2_data[(int)ports[bird*8+1]->Value]; break;
      }



     // zt_DIG_In_Line(NIdeviceNum, ports[bird*8]->Value, ports[bird*8+1]->Value, &InState);
      // This function 'returns' the variable Instate via NIDAQ. Note that it is a reverse logic: true if key is off, false when key is on.

      // if key is off
	 // if(test->Down)InState=k1->Down;
      if(InState){ key[detector1]->Brush->Color=clYellow; peck[detector1]=false;} // peck[] indicate the detector position
        // key is on
      else
      {
          if(let_go_of_key->Checked && key[detector1]->Brush->Color==clRed){ key1_stuck=true; peck[detector1]=false;}// no more playbacks
          else
          {
			if(key_track->Checked)
				   SaveKeyPecks(name[bird], 1 ,"keypeck event", date, tm);
            peck[detector1]=true;
            if(Session[this_session].state && (Session[this_session].operant==1 || Session[this_session].operant==3)) // key 1 or both are active
            {
               key[detector1]->Brush->Color=clRed;
               operantPlay=true;
            }
            else // key is not active
            {
               key[detector1]->Brush->Color=clBlue;
            }
          }
      }

      // second key
      switch((int)ports[bird*8+4]->Value)
      {
        case 0: InState=port0_data[(int)ports[bird*8+5]->Value]; break;
        case 1: InState=port1_data[(int)ports[bird*8+5]->Value]; break;
        case 2: InState=port2_data[(int)ports[bird*8+5]->Value]; break;
      }
    //  if(test->Down)InState=k2->Down;

     // zt_DIG_In_Line(NIdeviceNum,ports[bird*8+4]->Value, ports[bird*8+5]->Value,&InState);
      if(InState){ key[detector2]->Brush->Color=clYellow; peck[detector2]=false;}
      else
      {
            if(let_go_of_key->Checked && key[detector2]->Brush->Color==clRed) {key2_stuck=true; peck[detector2]=false;}//return; // no more playbacks
            else
            {
              if(key_track->Checked) SaveKeyPecks(name[bird], 2 ,"keypeck event", date, tm);
              peck[detector2]=true;
              if(Session[this_session].state && (Session[this_session].operant==2 || Session[this_session].operant==3)) // key 2 or both are active
              {
                   key[detector2]->Brush->Color=clRed;
                   operantPlay=true;
              }
              else
              {
                  key[detector2]->Brush->Color=clBlue;
              }
            }
      }
     }// done "if operant" and we figured out if any key was pecked.
     // else -- if the session is not operant, than it is passive -- set the odds.

    // now check if session is active, and manage gap timer

    if(!Session[this_session].state){return;} // session is enabled, but not currently active
    if(Session[this_session].gap_timer)
    {
       Box[bird]->Brush->Color=clMoneyGreen;
       Session[this_session].gap_timer--; return;
    }
    if(Session[this_session].enable_quota && Session[this_session].song_played>=Session[this_session].quota)
    {
       Box[bird]->Brush->Color=clOlive;
       /*
       str="en-";
       str+=(int)Session[this_session].enable_quota;
       str+=";pl-"; str+=Session[this_session].song_played;
       str+=";q-"; str+=Session[this_session].quota;
	   playback_error[bird]->Caption=str;
	   playback_error[bird]->Visible=true;
       */
       return;
    }
    if( !passivePlay && !operantPlay) {Box[bird]->Brush->Color=clTeal; return;}
    else // do playbacks
    {
      Box[bird]->Brush->Color=clTeal;
      // choose a playback list
      bool list_selected;
      if(Session[this_session].playback_list==0)
      {
         list_selected=0;
         the_list=playbacks_[bird*2];
         randomize_playbacks=random_switch[bird*2];
         scroll_playbacks=random_switch[bird*2];
      }
      else if(Session[this_session].playback_list==1)
      {
		list_selected=1;
		the_list=playbacks_[bird*2+1];
		randomize_playbacks=random_switch[bird*2+1];
		scroll_playbacks=random_switch[bird*2+1];
      }
	  else if(Session[this_session].playback_list==2)
      {
        if(peck[detector1])
        {
          list_selected=0;
          the_list=playbacks_[bird*2];
          randomize_playbacks=random_switch[bird*2];
          scroll_playbacks=random_switch[bird*2];
        }
        else
        {
          list_selected=1;
          the_list=playbacks_[bird*2+1];
          randomize_playbacks=random_switch[bird*2+1];
          scroll_playbacks=random_switch[bird*2+1];
        }
      }
      //song_to_play=the_list->FileName;

  // validate that there is a song to play
	  if(the_list->ItemIndex>=0)
	  {
		  song_to_play=the_list->FileName;//Items->Strings[the_list->ItemIndex];
		  if(playback_error[bird]->Visible==true)playback_error[bird]->Visible=false;
      }
      else
      {
         the_list->ItemIndex=0;
         if(the_list->ItemIndex==-1)
         {
            playback_error[bird]->Caption="Nothing to play";
			playback_error[bird]->Visible=true;
            Box[bird]->Brush->Color=clRed;
            //if(peck[detector1])key[detector1]->Brush->Color=clBlack;
            //else key[detector2]->Brush->Color=clBlack;
            return;
         }
		 song_to_play=the_list->FileName;
      }
      int song_lable;
      if(list_selected)song_lable=bird*2+1;
	  else song_lable=bird*2;
	  song[song_lable]->Caption=ExtractFileName(song_to_play);
  // deliver a song playback
	  if(biofeedback[bird]->Checked)
	  {
         if(!FileExists(song_to_play))
         {
           int ind=playbacks_[bird*2]->ItemIndex;
           playbacks_[bird*2]->Update();
           Sleep(10);
           playbacks_[bird*2]->ItemIndex=ind;
           song_to_play=playbacks_[bird*2]->FileName;
         }
      }
      SetBirdString(m_Birds[bird],vBirdWaveFile,song_to_play.c_str());
      if(playback_mute_rec[bird]->Checked) m_Birds[bird]->SetLongValue(vMuteBirdData,1); // avoid recording playbacks
      m_Birds[bird]->TriggerSound();  // play the sound

  // post playback events:
      // first, set refractory period
      Session[this_session].song_played++;
	  float fr=1000/Timer1->Interval;
      Session[this_session].gap_timer=fr*Session[this_session].gap; // reset the refractory period until the next playback

      // set key peck counters and records
      int pki;
      if(passivePlay) {pki=0; passive_played[bird]->Value++; }
      else if(peck[detector1]){pki=1; playbacks_played[bird*2]->Value++; }
      else if(peck[detector2]){pki=2; playbacks_played[bird*2+1]->Value++; }
      if(peck[detector1] && peck[detector2])pki=-1;  // two keys are pecked at the same time!
      if(list_selected) Play_counts[bird*2+1]->Value++;
      else Play_counts[bird*2]->Value++;
      // save to database
      SaveKeyPecks(name[bird], pki ,song[song_lable]->Caption, date, tm);  // save events to the table (need more work here)

  // prepare for next playback
      // choose the file to play
      if(randomize_playbacks->Checked)  // chose a song from the list if so indicated
      {
       int sw=random(the_list->Items->Count);
       the_list->ItemIndex=sw;
      }
      else if(scroll_playbacks->Checked)
      {
       if(the_list->ItemIndex>the_list->Items->Count-2) the_list->ItemIndex=0;
       else the_list->ItemIndex++;
      }
     }
/* done, below is previous version with instrumentation:
      // operant instrumentation (off by default)
   if(enable_operant->Checked)
   {
      if(trigger_effect[detector1]->Checked || trigger_effect[detector2]->Checked || passive_effect->Checked) // passive effect if for all birds!
           // in this mode, activating detectors can cause different playbacks and different post-playbacks effects
      {                          // use this mode for song discrimination task and for go-no-go tasks.
                                 // activate an effector if:
       if(  !effector_delay[bird]  // effector delay time has ellapsed
            && !effector_on[bird]  // and effector state is still off
            && effector_timer[bird]  // and effector timer is not zero (effect has been triggered)
         )
         {
              if(trigger_effect[detector1]->Checked || passive_effect->Checked)
            //  {zt_DIG_Out_Line(NIdeviceNum,affector1_ports->Value,affector1_line->Value,1); box->Brush->Color=clBlue;}// activate effector
                {zt_DIG_Out_Line(NIdeviceNum,ports[bird*8+2]->Value,ports[bird*8+3]->Value,1); Box[bird]->Brush->Color=clBlue;}// activate effector  1
              else if(trigger_effect[detector2]->Checked)
                {zt_DIG_Out_Line(NIdeviceNum,ports[bird*8+6]->Value,ports[bird*8+7]->Value,1); Box[bird]->Brush->Color=clAqua;}// activate effector  2


             effector_on[bird]=true;   // indicates that the effector is now on
         }
         if(effector_on[bird] && !effector_timer[bird]) // if effector is on and its timer is zero -- stop the effect
         {
           zt_DIG_Out_Line(NIdeviceNum,ports[bird*8+2]->Value,ports[bird*8+3]->Value,0); // turn off effector
           zt_DIG_Out_Line(NIdeviceNum,ports[bird*8+6]->Value,ports[bird*8+7]->Value,0);
           Box[bird]->Brush->Color=clTeal;
           effector_on[bird]=false;
         }
         if(effector_delay[bird]) effector_delay[bird]--;  // count down the delay of the effect
         else if(effector_timer[bird]) effector_timer[bird]--;  // count down the duration of the effect
      }
   }


// check the keys for pecking
      if(operant->Checked)  // if ' enable operant training' is checked, see in the 'training method' page.
      {
        // first check the first detectors (keys) position
        zt_DIG_In_Line(NIdeviceNum, ports[bird*8]->Value, ports[bird*8+1]->Value, &InState);

        // Instate is a reverse logic: true if key is off, false when key is on.

        // if key is off
        if(InState){ key[detector1]->Brush->Color=clYellow; peck[detector1]=false;} // peck[] indicate the detector position
        // key is on
        else
        {
                if(let_go_of_key->Checked && key[detector1]->Brush->Color==clRed){ key1_stuck=true; peck[detector1]=false;}// no more playbacks
                else
                {
                  if(key_track->Checked) SaveKeyPecks(name[bird], 1 ,"keypeck event", date, tm);
                  peck[detector1]=true;
                  key[detector1]->Brush->Color=clRed;
                }
        }

        zt_DIG_In_Line(NIdeviceNum,ports[bird*8+4]->Value, ports[bird*8+5]->Value,&InState);
        //str=NIdeviceNum; str+=","; str+=ports[bird*4+4]->Value; str+=","; str+=ports[bird*4+5]->Value;
       // messages->Lines->Add(str);
        //zt_DIG_In_Line(1,0,1,&InState);
        if(InState){ key[detector2]->Brush->Color=clYellow; peck[detector2]=false;}
        else
        {
                if(let_go_of_key->Checked && key[detector2]->Brush->Color==clRed) {key2_stuck=true; peck[detector2]=false;}//return; // no more playbacks
                else
                {
                  if(key_track->Checked) SaveKeyPecks(name[bird], 2 ,"keypeck event", date, tm);
                  peck[detector2]=true;
                  key[detector2]->Brush->Color=clRed;
                }
        }
      }


// should passive playbacks be delivered?
      if(Latent[bird]<=training_latency[bird])return;
      bool passivePlay=false;            // we now decide if passive playback should occur

      if(passive_playbacks->Checked
         && allow_passive[bird]->Checked
      //   && hour>=random_start->Value
      //   && hour<=random_stop->Value
         && ( !passive_quota_check[bird]->Checked || passive_played[bird]->Value<=passive_quota[bird]->Value) )

                if(random(passive_odds[bird]->Position)==0)// && hour>=random_start->Value && hour<=random_stop->Value)
                   passivePlay=true;


// handle playbacks
     if( passivePlay || (   (peck[detector1] || peck[detector2])
           && (playbacks_played[bird]->Value<playbacks_quota[bird]->Value || !enable_quota[bird]->Checked) ) )
      {
           // should we switch the lists?
           if(switch_lists[bird]->Checked)
           {
              if(switch_quota[bird]->Value<playbacks_played[bird]->Value)
              {
                  playbacks_played[bird]->Value=0;
                  switch_the_lists(bird);
              }
           }

           // find the correct list

           bool sl;
           if(peck[detector1])      sl=detector_list[bird*3]->ItemIndex;
           else if(peck[detector2]) sl=detector_list[bird*3+1]->ItemIndex;
           else if(passivePlay)     sl=detector_list[bird*3+2]->ItemIndex;
           the_list=playbacks_[bird*2+sl];
           if(random_switch[bird]->Checked)  // chose a song from the list if so indicated
           {
             int sw=random(the_list->Items->Count);
             the_list->ItemIndex=sw;
           }

           else if(auto_scroll[bird]->Checked)
           {
             if(the_list->ItemIndex>the_list->Items->Count-2) the_list->ItemIndex=0;
             else the_list->ItemIndex++;
           }


           if(the_list->ItemIndex>=0)song_to_play=the_list->FileName;//Items->Strings[the_list->ItemIndex];
           else
           {
                the_list->ItemIndex=0;
                if(the_list->ItemIndex<0)
                {
                        if(peck[detector1])key[detector1]->Brush->Color=clBlue;
                        else key[detector2]->Brush->Color=clBlue;
                        return;
                }
           }
           if(current_song[bird]!=song_to_play && the_list->ItemIndex!=-1)
           {
             SetBirdString(m_Birds[bird],vBirdWaveFile,song_to_play.c_str());
             current_song[bird]=song_to_play;
             song[bird]->Caption=ExtractFileName(song_to_play);
           }


           if(trigger_effect[detector1]->Checked && peck[detector1]                // the bird pecked a 'loaded' key
                 && !effector_timer[bird]                               // effect is not on already
                 && the_list->ItemIndex>=gonogo_thresh->Position) // go no go
                   // The bird has activated a detector that should (eventually) trigger an effect
           {
                effector_delay[bird]=effect_delay[detector1]->Position*Timer_freq; // activate the effector delay timer, how long before the effect start
                effector_timer[bird]=effect_duration[detector1]->Position*Timer_freq;  // activate the effector activation timer, how long should the effect continue
           }

           // same for the second detector
           if(trigger_effect[detector2]->Checked && peck[detector2]  // the bird pecked a 'loaded' key
                 && !effector_timer[bird] // same for the second detector
                 && the_list->ItemIndex>=gonogo_thresh->Position) // go no go
           {
                effector_delay[bird]=effect_delay[detector2]->Position*Timer_freq;
                effector_timer[bird]=effect_duration[detector2]->Position*Timer_freq;
           }


           if(passive_effect->Checked && passivePlay  // the bird pecked a 'loaded' key
                 && !effector_timer[bird] // same for the second detector
                 && the_list->ItemIndex>=gonogo_thresh->Position) // go no go
           {
                effector_delay[bird]=effect_delay[detector2]->Position*Timer_freq;
                effector_timer[bird]=effect_duration[detector2]->Position*Timer_freq;
           }

             if(playback_mute_rec[bird]->Checked) m_Birds[bird]->SetLongValue(vMuteBirdData,1); // avoid recording playbacks
             m_Birds[bird]->TriggerSound();  // play the sound
             Latent[bird]=0;  // reset the refractory period until the next playback
             if(passivePlay) passive_played[bird]->Value++;
             else playbacks_played[bird]->Value++;
             int pki;
             if(peck[detector1]==0 && peck[detector2]==0)pki=0;
             else if(peck[detector1]==1 && peck[detector2]==0)pki=1;
             else if(peck[detector1]==0 && peck[detector2]==1)pki=2;
             else pki=-1;  // two keys are pecked at the same time!
             SaveKeyPecks(name[bird], pki ,song[bird]->Caption, date, tm);  // save events to the table (need more work here)
             if(pki==1)Play_counts[bird*2]->Value++;
             else if(pki==2)Play_counts[bird*2+1]->Value++;
      }  */
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
     if(database_exists(name))return;
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
	 str=MyQuery1->Fields->Fields[0]->AsString;
	 if(database_name==str)return(true);
	 MyQuery1->Next();
  }
  return(false);
}


void TForm1::create_settings()
{
  current->Active=false;
  str="CREATE TABLE `settings` ( `num` tinyint(4) NOT NULL auto_increment,";
  str+="`bird_ID` char(10) NOT NULL default '99',";
  str+=" `hatching_date` date NOT NULL default '2010-01-01',";
  str+="`start_training` date NOT NULL default '2010-01-01',";
  str+="  `stop_training` date NOT NULL default '2010-01-01',";
  str+="`training` tinyint(4) NOT NULL default '0',";
  str+="   `pecks` smallint(6) NOT NULL default '0',";
  str+="`song_quota` smallint(6) NOT NULL default '10',";
  str+=" `input_folder` varchar(50) NOT NULL default 'c:\\\\sap\\\\in1',";
  str+="`sound_folder_size` smallint(6) NOT NULL default '0',";
  str+=" `output_folder_sound` varchar(50) NOT NULL default 'c:\\\\sap\\\\out1',";
  str+="`output_folder_features` varchar(50) NOT NULL default 'recs0_features',";
  str+=" `syllable_table` varchar(50) NOT NULL default 'syll_R0',";
  str+="`entropy_thresh` smallint(6) NOT NULL default '282',";
  str+=" `deriv_thresh` smallint(6) NOT NULL default '116',";
  str+=" `contrast_thresh` smallint(6) NOT NULL default '300',";  // contours contast
  str+="`syllable_duration_reject` smallint(6) NOT NULL default '300',";
  str+=" `bout_duration_reject` smallint(6) NOT NULL default '0',";
  str+="`number_of_syllables_reject` smallint(3) unsigned NOT NULL default '255',";
  str+=" `display_contrast` smallint(3) unsigned NOT NULL default '4500',";
  str+="`fine_segmentation` tinyint(4) NOT NULL default '0',";
  str+=" `hp_small` smallint(6) NOT NULL default '50',";
  str+=" `hp_big` smallint(6) NOT NULL default '500',";
  str+="`select_list1` tinyint(4) NOT NULL default '1',";
  str+="`quota` tinyint(4) NOT NULL default '1',"; // check -- see song quota...
  str+="`select_list2` tinyint(4) NOT NULL default '1',";
  str+="`passive_playbacks` tinyint(4) NOT NULL default '0', ";
  str+="`passive_odds` float NOT NULL default '0',";
  str+="`passive_quota` smallint(6) NOT NULL default '10',";
  str+="`passive_quota_check` tinyint(4) NOT NULL default '0',";
  str+="`bio_feedback` tinyint(4) NOT NULL default '0',";
  str+="`update` tinyint(4) NOT NULL default '0',";
  str+="`save` tinyint(4) NOT NULL default '0',";
  str+="`slave_output` tinyint(4) NOT NULL default '1',";
  str+="`advance_window` smallint(6) NOT NULL default '44',";
  str+="`FFT_window` smallint(6) NOT NULL default '441',";
  str+="`frequency_range` smallint(6) NOT NULL default '2',";
  str+="`active_or_slave` tinyint(4) NOT NULL default '0',";

  str+="`pri_segmented_feature` tinyint(4) NOT NULL default '0',";
  str+="`adapt_thresh` tinyint(4) NOT NULL default '0',";
  str+="`smooth_feature` tinyint(4) NOT NULL default '0',";
  str+="`pri_lt` tinyint(4) NOT NULL default '0',";
  str+="`sec_segmentation` tinyint(4) NOT NULL default '0',";
  str+="`sec_segmented_feature` tinyint(4) NOT NULL default '0',";
  str+="`sec_lt` tinyint(4) NOT NULL default '0',";
  str+="`sec_logic` tinyint(4) NOT NULL default '0',";
  str+="`pagination` tinyint(4) NOT NULL default '0',";
  str+="`stop_duration_thresh` smallint(6) NOT NULL default '7',";
  str+="`bout_duration_thresh` smallint(6) NOT NULL default '100',";
  str+="PRIMARY KEY  (`num`))";
  MyCommand1->SQL->Clear();
  MyCommand1->SQL->Add(str);
  MyCommand1->Execute(1);

  current->Active=true;
  current->Refresh();

  current->First();
  for(int i=0;i<13;i++)
  {
	current->Edit();
	current->Append();
	current->Post();
  }

  current->First();
  str="c:\\sap\\";
  for(int i=0;i<13;i++)
  {
	 current->Edit();
	 current->FieldValues["bird_ID"]=i;
	 current->FieldValues["hatching_date"]="2010-01-02";
	 current->FieldValues["start_training"]="2010-02-02";
	 current->FieldValues["stop_training"]="2010-03-02";
	 current->FieldValues["quota"]=10;

	  current->FieldValues["FFT_window"]="441";
	  current->FieldValues["FFT_window"]="441";
	  current->FieldValues["advance_window"]="44";
	  current->FieldValues["frequency_range"]="2";
	  current->FieldValues["hp_big"]="500";
	  current->FieldValues["hp_small"]="50";

	 str2=str+"in"; str2+=max(i-1,0);
	 current->FieldValues["input_folder"]=str2;
	 str2=str+"out"; str2+=max(i-1,0);
	 current->FieldValues["output_folder_sound"]=str2;
	 current->Post();
	 current->Next();
  }

  current->Refresh();
  current->First();
}


bool TForm1::table_exists(AnsiString table_name)
{
  showTables();
  MyQuery1->First();
  for (int i=0; i < MyQuery1->RecordCount; i++)
  {
	 str=MyQuery1->Fields->Fields[0]->AsString;
	 if(table_name==str)return(true);
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
  str+="`bird_ID` char(16) default '0',";
  str+="`key_pecked` tinyint(4) default '0',";
  str+="`song_played` char(80) default '',";
  str+="`date` date NOT NULL default '2000-01-01',";
  str+="`time` time NOT NULL default '00:00:00')";
  MyCommand1->SQL->Clear();
  MyCommand1->SQL->Add(str);
  MyCommand1->Execute(1);
  keypeckTable->Active=true;
  keypeckTable->Refresh();
 }


 void TForm1::create_nidaq()
 {
  NIDAQ->Active=false;

  DeleteFile("c:\\sap\\birds.txt");
  FILE *stream;
  stream = fopen("c:\\sap\\birds.txt", "w+");


  str="CREATE TABLE `nidaq`(";
  str+="`recnum` int(11) NOT NULL auto_increment,";
  str+="`device_ID` tinyint(4) default '0',";
  str+="`port` tinyint(4) default '0',";
  str+="`line` tinyint(4) default '0',";
  str+="PRIMARY KEY  (`recnum`))";
  MyCommand1->SQL->Clear();
  MyCommand1->SQL->Add(str);
  MyCommand1->Execute(1);

  NIDAQ->Active=true;
  NIDAQ->Refresh();

  NIDAQ->First();
  for(int i=0;i<40;i++)
  {
	NIDAQ->Edit();
	NIDAQ->Append();
	NIDAQ->Post();
  }

  NIDAQ->First();
  for(int i=0;i<40;i++)
  {
	 NIDAQ->Edit();
	 NIDAQ->FieldValues["device_ID"]=1;
	 NIDAQ->FieldValues["port"]=0;
     NIDAQ->FieldValues["line"]=i%8;
	 NIDAQ->Post();
	 NIDAQ->Next();
  }
  NIDAQ->Refresh();
  NIDAQ->First();
}


void __fastcall TForm1::enable_quota1Click(TObject *Sender)
{
    int i;
    if(Sender==enable_quota1)i=0;
    else if(Sender==enable_quota2)i=1;
    else if(Sender==enable_quota3)i=2;
    else if(Sender==enable_quota4)i=3;
    else if(Sender==enable_quota5)i=4;
    else if(Sender==enable_quota6)i=5;
    else if(Sender==enable_quota7)i=6;
    else if(Sender==enable_quota8)i=7;
    else if(Sender==enable_quota9)i=8;
    else if(Sender==enable_quota10)i=9;
    else if(Sender==enable_quota11)i=10;
    else if(Sender==enable_quota12)i=11;
    else if(Sender==enable_quota13)i=12;
    else if(Sender==enable_quota14)i=13;
    else if(Sender==enable_quota15)i=14;
    else if(Sender==enable_quota16)i=15;
    else if(Sender==enable_quota17)i=16;
    else if(Sender==enable_quota18)i=17;
    else if(Sender==enable_quota19)i=18;
    else if(Sender==enable_quota20)i=19;

    if(enable_quota[i]->Checked)
    {playbacks_quota[i]->Visible=true; Session[i].enable_quota=true; quota_UpDown[i]->Visible=true;}
    else {playbacks_quota[i]->Visible=false; Session[i].enable_quota=false; quota_UpDown[i]->Visible=false;}
}
//---------------------------------------------------------------------------





void __fastcall TForm1::active1Click(TObject *Sender)
{
 if(!update_check)return;
 CZT_OCX_BirdProps props;
 int Bird;
 if(Sender==active1)     Bird=0;
 else if(Sender==active2) Bird=1;
 else if(Sender==active3) Bird=2;
 else if(Sender==active4) Bird=3;
 else if(Sender==active5) Bird=4;
 else if(Sender==active6) Bird=5;
 else if(Sender==active7) Bird=6;
 else if(Sender==active8) Bird=7;
 else if(Sender==active9) Bird=8;
 else if(Sender==active10)Bird=9;

 // add mute function if engine is working
 /*
   if(mt->Down){m_Birds[bird]->SetLongValue(vMuteBirdData,1); m_Birds[bird]->SetLongValue(vRecordingDisplay,3); }
  else {m_Birds[bird]->SetLongValue(vMuteBirdData,0); m_Birds[bird]->SetLongValue(vRecordingDisplay,3); }

 */

 if(recording)
 {
        if(bird_active[Bird]->Checked)
        {
                m_Birds[Bird]->SetLongValue(vMuteBirdData,0);
                update_chan_displays(Bird);
                rec_panel[Bird]->Color=clBtnFace;
        }
        else
        {
                m_Birds[Bird]->SetLongValue(vMuteBirdData,1);
                bird_active[Bird]->Caption="Paused";
                rec_panel[Bird]->Color=clBlack;
        }
 }

 else
 {
   bool device_configured=true;
   props.RetrieveAllValues(m_Birds[Bird]);   // we keep here the original properties of the bird, before changing them...
   if(bird_active[Bird]->Checked)
   {

    int i=props.m_lRecordingDisplay;
    str=props.m_sAudioSrcDescription.c_str();
    // if invalid channel
    if(str[1]=='i' && str[2]=='n' && str[3]=='v' && str[4]=='a') device_configured=false;
    //if(!props.m_sIOM_InputModuleID)device_configured=false; //m_lIOM_InputModuleVerMaj)device_configured=false;
    if(!device_configured)
    {
          str="Sorry, SAP cannot activate this device because the input channel is not configured. Click the settings button above and select a valid input channel";
          MessageDlgPos(str, mtError, TMsgDlgButtons() << mbOK, 0, 10, 80);
          bird_active[Bird]->Checked=false;
          return;
    }
    // else:
    switch(props.m_lBirdState)
    {
    case 0: props.m_lBirdState++; break;
    case 4: props.m_lBirdState++; break;
    case 8: props.m_lBirdState++; break;
    default:props.m_lBirdState=1; break;
    }

    props.m_lRecordingDisplay=3;
   }

   else //active->Checked=false;
   {
    switch(props.m_lBirdState)
    {
    case 1: props.m_lBirdState--; break;
    case 5: props.m_lBirdState--; break;
    case 9: props.m_lBirdState--; break;
    default:props.m_lBirdState=0; break;
    }
   // props.m_lBirdState--;
    props.m_lRecordingDisplay=3;
   }

   props.PutAllValues(m_Birds[Bird]);
   update_chan_displays(Bird);
 }
}
//---------------------------------------------------------------------------



int TForm1::retrieve_channel_attributes(TztBird* ztBird, int chan)
{
		int state;
		CZT_OCX_BirdProps props1;
		channels->Active=true;
		channels->RecNo=chan;       // go to the correct row according to chan.
		current->RecNo=chan+2;
		/*
		float mr=channels->FieldValues["morning_session_start"];
		MorningSession->Value=floor(mr/100);
		if(MorningSession->Value<0)MorningSession->Value=0;
		MorningSession_min->Value=mr-(MorningSession->Value*100);
		if(MorningSession_min->Value<0)MorningSession_min->Value=0;

		mr=channels->FieldValues["morning_session_end"];
		MorningSession_end->Value=floor(mr/100);
		if(MorningSession_end->Value<0)MorningSession_end->Value=0;
		MorningSession_end_min->Value=mr-(MorningSession_end->Value*100);
		if(MorningSession_end_min->Value<0)MorningSession_end_min->Value=0;

		mr=channels->FieldValues["afternoon_session_start"];
		AfternoonSession->Value=floor(mr/100);
		if(AfternoonSession->Value<0)AfternoonSession->Value=0;
		AfternoonSession_min->Value=mr-(AfternoonSession->Value*100);
		if(AfternoonSession_min->Value<0)AfternoonSession_min->Value=0;

		mr=channels->FieldValues["afternoon_session_end"];
		AfternoonSession_end->Value=floor(mr/100);
		if(AfternoonSession_end->Value<0)AfternoonSession_end->Value=0;
		AfternoonSession_end_min->Value=mr-(AfternoonSession_end->Value*100);
		if(AfternoonSession_end_min->Value<0)AfternoonSession_end_min->Value=0;
		*/

		NIcard_exists->Checked=channels->FieldValues["NIDAQ_exists"];
		direct_recording_duration->Position=channels->FieldValues["direct_recording_epochs"];
		key_track->Checked=channels->FieldValues["keep_track_of_keypecks"];
		mode->ItemIndex=channels->FieldValues["two_soundlists"];
		modeClick(this);
		biofeedback[chan]->Checked=channels->FieldValues["self_feedback"];
		random_switch[(chan-1)*2]->Checked=channels->FieldValues["randomly_select_sounds1"];
		random_switch[(chan-1)*2+1]->Checked=channels->FieldValues["randomly_select_sounds2"];
		auto_scroll[(chan-1)*2]->Checked=channels->FieldValues["auto_scroll_sounds1"];
		auto_scroll[(chan-1)*2+1]->Checked=channels->FieldValues["auto_scroll_sounds2"];
		playback_mute_rec[(chan-1)]->Checked=channels->FieldValues["mute_playbacks"];
		session_Lock[chan-1]->Down=channels->FieldValues["lock_sessions"];
		Session_lock(chan-1);
		lock_playbacks[chan-1]->Down=channels->FieldValues["lock_playbacks"];
		Playback_lock(chan-1);


		lightsOn->Value=channels->FieldValues["lights_on"];
		lightsOff->Value=channels->FieldValues["lights_off"];
		dont_train_at_night->Checked=channels->FieldValues["stop_training_at_night"];
	  //  gap[chan-1]->Value=channels->FieldValues["interval_between_playbacks"];
		let_go_of_key->Checked=channels->FieldValues["let_go_of_key"];
		//pre_thresh_buffer_val->Value=channels->FieldValues["pre_trigger"];
	   // duration->Value=channels->FieldValues["post_trigger"];

		str=channels->FieldValues["id"];
		props1.m_sBirdID=str.c_str();
		str=current->FieldValues["bird_ID"]; //
		//str=channels->FieldValues["m_sBirdName"];//name of bird
		props1.m_sBirdName=str.c_str();
		props1.m_fPeakGain=channels->FieldValues["m_fPeakGain"];
		props1.m_lPrebufferSamples=channels->FieldValues["m_lPrebufferSamples"];// pre buffer

		state=props1.m_lBirdState=channels->FieldValues["m_lBirdState"];
		if(state==ztbs_Active)channel_mode[chan-1]->ItemIndex=0;
		else if(state==5)channel_mode[chan-1]->ItemIndex=1;
		else if(state==ztbs_Unitialized)channel_mode[chan-1]->ItemIndex=2;



		props1.m_lPeakThreshold=channels->FieldValues["m_lPeakThreshold"];
		props1.m_lPeakRecordDuration=channels->FieldValues["m_lPeakRecordDuration"];
		if(chan==1)
		{
				minimum_recording_duration->Position=props1.m_lPeakRecordDuration/1000;
				pre_thresh_buffer->Position=props1.m_lPrebufferSamples/44.1;
		}
		props1.m_lMonitorDisplay=channels->FieldValues["m_lMonitorDisplay"]; // what is viewed when monitoring
		props1.m_lRecordingDisplay=3;//channels->FieldValues["m_lRecordingDisplay"];// what is viewed when recording (see above ztDisplays enums for valid values)
		props1.m_lRecordingCutoffMS=channels->FieldValues["m_lRecordingCutoffMS"]; // maximum number of milliseconds to record
		MaxDur->Position=props1.m_lRecordingCutoffMS/1000;
		MaxDurChange(this);
		props1.m_lPeakComparator=channels->FieldValues["m_lPeakComparator"]; // how many peaks to accept the recording?
		SoundDurTb->Position=props1.m_lPeakComparator;
		SoundDurTbChange(this);

		props1.m_lPeakHP_Filter=channels->FieldValues["m_lPeakHP_Filter"];// use a high-pass filter when looking for peaks
		props1.m_lPeakTrigger=channels->FieldValues["m_lPeakTrigger"];// number of peaks required to intiate a trigger event (0 turns off triggering)
		long x=channels->FieldValues["m_lIOM_InputFormatIDX"];        //
		props1.m_lIOM_InputFormatIDX=x;//  input format index
		props1.m_lIOM_InputChannel=channels->FieldValues["m_lIOM_InputChannel"];// which channel
		props1.m_lIOM_InputPan=channels->FieldValues["m_lIOM_InputPan"];// not used at the moment
		props1.m_lIOM_InputGain=channels->FieldValues["m_lIOM_InputGain"];//(-10,000 = silence  0 = full volume)
		props1.m_fIOM_InputScaler=channels->FieldValues["m_fIOM_InputScaler"];
		props1.m_lIOM_OutputFormatIDX=channels->FieldValues["m_lIOM_OutputFormatIDX"];// output format index
		props1.m_lIOM_OutputChannel=channels->FieldValues["m_lIOM_OutputChannel"];// not used at the moment
		props1.m_lIOM_OutputPan=channels->FieldValues["m_lIOM_OutputPan"];// panning of output (left,right,center)
		props1.m_lIOM_OutputGain=channels->FieldValues["m_lIOM_OutputGain"];//(-10,000 = silence  0 = full volum
		str= channels->FieldValues["m_sTempFilePath"];// temp folder to write wavs into
		props1.m_sTempFilePath=str.c_str();//channels->FieldValues["m_sTempFilePath"];// temp folder to write wavs into

		str=current->FieldValues["input_folder"];
	 //   str=channels->FieldValues["m_sWaveFilePath"];

		props1.m_sWaveFilePath=str.c_str();// where accepted wavs end up
		str=channels->FieldValues["m_sWaveFile"];
		props1.m_sWaveFile=str.c_str();//output wave file
		str=channels->FieldValues["m_sMasterID"];
		props1.m_sMasterID=str.c_str();// MasterID for a bird in slave mode
		str=channels->FieldValues["m_sIOM_InputModuleID"];
		props1.m_sIOM_InputModuleID=str.c_str();// id of the io module selected for input
		str=channels->FieldValues["m_sIOM_InputDeviceID"];
		props1.m_sIOM_InputDeviceID=str.c_str();// id of the selected input device
		str=channels->FieldValues["m_sIOM_OutputModuleID"];
		props1.m_sIOM_OutputModuleID=str.c_str();// id of the io module selected for output
		str=channels->FieldValues["m_sIOM_OutputDeviceID"];
		props1.m_sIOM_OutputDeviceID=str.c_str();// id of the selected output device

		props1.PutAllValues(ztBird);
		return(state);
}


void TForm1::save_channel_attributes(TztBird* ztBird, int chan)
{

		CZT_OCX_BirdProps props1;
		channels->Active=true;
		channels->RecNo=chan;       // go to the correct row according to chan.
		props1.RetrieveAllValues(ztBird);

		channels->Edit();

		channels->FieldValues["NIDAQ_exists"]=(NIcard_exists->Checked)? 1:0;
		channels->FieldValues["direct_recording_epochs"]=direct_recording_duration->Position;
		channels->FieldValues["keep_track_of_keypecks"]=(key_track->Checked)? 1:0;
		channels->FieldValues["two_soundlists"]=mode->ItemIndex;
		channels->FieldValues["self_feedback"]=(biofeedback[chan-1]->Checked)? 1:0;
		channels->FieldValues["randomly_select_sounds1"]=(random_switch[(chan-1)*2]->Checked)? 1:0;
		channels->FieldValues["randomly_select_sounds2"]=(random_switch[(chan-1)*2+1]->Checked)? 1:0;
		channels->FieldValues["auto_scroll_sounds1"]=(auto_scroll[(chan-1)*2]->Checked)? 1:0;
		channels->FieldValues["auto_scroll_sounds2"]=(auto_scroll[(chan-1)*2+1]->Checked)? 1:0;
		channels->FieldValues["lock_playbacks"]=lock_playbacks[chan-1]->Down;
		channels->FieldValues["lock_sessions"]=session_Lock[chan-1]->Down;
		channels->FieldValues["mute_playbacks"]=playback_mute_rec[(chan-1)]->Checked;

		channels->FieldValues["lights_on"]=lightsOn->Value;
		channels->FieldValues["lights_off"]=lightsOff->Value;
		channels->FieldValues["stop_training_at_night"]=(dont_train_at_night->Checked)? 1:0;
		channels->FieldValues["interval_between_playbacks"]=gap[chan-1]->Value;
		channels->FieldValues["let_go_of_key"]=let_go_of_key->Checked;
		channels->FieldValues["m_lPrebufferSamples"]=props1.m_lPrebufferSamples;// pre buffer
		channels->FieldValues["m_fPeakGain"]=props1.m_fPeakGain;
		channels->FieldValues["m_lBirdState"]=props1.m_lBirdState;
		channels->FieldValues["m_lPeakThreshold"]=props1.m_lPeakThreshold;
		channels->FieldValues["m_lPeakRecordDuration"]=props1.m_lPeakRecordDuration;
		channels->FieldValues["m_lMonitorDisplay"]=props1.m_lMonitorDisplay; // what is viewed when monitoring
		channels->FieldValues["m_lRecordingDisplay"]=3;//props1.m_lRecordingDisplay;// what is viewed when recording (see above ztDisplays enums for valid values)
		channels->FieldValues["m_lRecordingCutoffMS"]=MaxDur->Position*1000;//props1.m_lRecordingCutoffMS; // maximum number of milliseconds to record
		channels->FieldValues["m_lPeakComparator"]=props1.m_lPeakComparator; // how many peaks to accept the recording?
		channels->FieldValues["m_lPeakHP_Filter"]=props1.m_lPeakHP_Filter;// use a high-pass filter when looking for peaks
		channels->FieldValues["m_lPeakTrigger"]=props1.m_lPeakTrigger;// number of peaks required to intiate a trigger event (0 turns off triggering)
		channels->FieldValues["m_lIOM_InputFormatIDX"]=props1.m_lIOM_InputFormatIDX;// input format index
		channels->FieldValues["m_lIOM_InputChannel"]=props1.m_lIOM_InputChannel;// which channel
		channels->FieldValues["m_lIOM_InputPan"]=props1.m_lIOM_InputPan;// not used at the moment
		channels->FieldValues["m_lIOM_InputGain"]=props1.m_lIOM_InputGain;//(-10,000 = silence  0 = full volume)
		channels->FieldValues["m_fIOM_InputScaler"]=props1.m_fIOM_InputScaler;
		channels->FieldValues["m_lIOM_OutputFormatIDX"]=props1.m_lIOM_OutputFormatIDX;// output format index
		channels->FieldValues["m_lIOM_OutputChannel"]=props1.m_lIOM_OutputChannel;// not used at the moment
		channels->FieldValues["m_lIOM_OutputPan"]=props1.m_lIOM_OutputPan;// panning of output (left,right,center)
		channels->FieldValues["m_lIOM_OutputGain"]=props1.m_lIOM_OutputGain;//(-10,000 = silence  0 = full volum
		channels->FieldValues["m_sTempFilePath"]=props1.m_sTempFilePath.data();// temp folder to write wavs into

	  //	channels->FieldValues["m_sWaveFilePath"]=props1.m_sWaveFilePath.data();// where accepted wavs end up
		channels->FieldValues["m_sWaveFile"]=props1.m_sWaveFile.data();//output wave file
		channels->FieldValues["m_sMasterID"]=props1.m_sMasterID.data();// MasterID for a bird in slave mode
		channels->FieldValues["m_sIOM_InputModuleID"]=props1.m_sIOM_InputModuleID.data();// id of the io module selected for input
		channels->FieldValues["m_sIOM_InputDeviceID"]=props1.m_sIOM_InputDeviceID.data();// id of the selected input device
		channels->FieldValues["m_sIOM_OutputModuleID"]=props1.m_sIOM_OutputModuleID.data();// id of the io module selected for output
		channels->FieldValues["m_sIOM_OutputDeviceID"]=props1.m_sIOM_OutputDeviceID.data();// id of the selected output device
		channels->Post();

		current->RecNo=chan+2;
		current->Edit();
		current->FieldValues["bird_ID"]=props1.m_sBirdName.data();
		current->FieldValues["input_folder"]=props1.m_sWaveFilePath.data();
		current->Post();
}




void TForm1::Update_Labels()
{
	io_folders_enable=false;
    for(int i=0;i<10;i++)
    {
        vol[i]->Position=max(-(int)m_Birds[i]->GetLongValue(vOutputVolume)/1000,0);
	   //	song[i]->Caption=Sysutils::ExtractFileName(m_Birds[i]->GetStringValue(vBirdWaveFile));
        tmpSound[i]->Text=m_Birds[i]->GetStringValue(vBirdTempFilePath);
        save[i]->Text=m_Birds[i]->GetStringValue(vBirdWaveFilePath);
		Threshold[i]->Position=m_Birds[i]->GetLongValue(vPeakThreshold)/320;
        gain[i]->Position=-m_Birds[i]->GetFloatValue(vPeakGain);
    }
    io_folders_enable=true;
}



void TForm1::save_all_configs()
{
   // first update the channels
   for(int i=0;i<10;i++) save_channel_attributes(m_Birds[i], i+1);

   // then update the settings table with songs and quotas
   for(int bird=0;bird<10;bird++)
   {
	   current->RecNo=bird+3;
	   current->Edit();
       /*
	   if(detector_list[bird*3]->ItemIndex)current->FieldValues["select_list1"]=1;
       else current->FieldValues["select_list1"]=0;

       if(detector_list[bird*3+1]->ItemIndex)current->FieldValues["select_list2"]=1;
       else current->FieldValues["select_list2"]=0;
        */
       if(enable_quota[bird]->Checked)current->FieldValues["quota"]=1;
       else current->FieldValues["quota"]=0;

       current->FieldValues["song_quota"]=playbacks_quota[bird]->Value;

       /*
       song_list[bird*2]->ItemIndex=0;
       if(song_list[bird*2]->ItemIndex==0 && song_list[bird*2]->Items->Strings[0].Length()>2)
                current->FieldValues["song1_list1"]=song_list[bird*2]->Items->Strings[0];
       else current->FieldValues["song1_list1"]="";

       song_list[bird*2]->ItemIndex=1;
       if(song_list[bird*2]->ItemIndex==1 && song_list[bird*2]->Items->Strings[1].Length()>2)
                current->FieldValues["song2_list1"]=song_list[bird*2]->Items->Strings[1];
       else current->FieldValues["song2_list1"]="";

       song_list[bird*2]->ItemIndex=2;
       if(song_list[bird*2]->ItemIndex==2 && song_list[bird*2]->Items->Strings[2].Length()>2)
                current->FieldValues["song3_list1"]=song_list[bird*2]->Items->Strings[2];
       else current->FieldValues["song3_list1"]="";

       song_list[bird*2+1]->ItemIndex=0;
       if(song_list[bird*2+1]->ItemIndex==0 && song_list[bird*2+1]->Items->Strings[0].Length()>2)
                current->FieldValues["song1_list2"]=song_list[bird*2+1]->Items->Strings[0];
       else current->FieldValues["song1_list2"]="";

       song_list[bird*2+1]->ItemIndex=1;
       if(song_list[bird*2+1]->ItemIndex==1 && song_list[bird*2+1]->Items->Strings[1].Length()>2)
                current->FieldValues["song2_list2"]=song_list[bird*2+1]->Items->Strings[1];
       else current->FieldValues["song2_list2"]="";

       song_list[bird*2+1]->ItemIndex=2;
       if(song_list[bird*2+1]->ItemIndex==2 && song_list[bird*2+1]->Items->Strings[2].Length()>2)
                current->FieldValues["song3_list2"]=song_list[bird*2+1]->Items->Strings[2];
       else current->FieldValues["song3_list2"]="";
        */
      // current->FieldValues["passive_playbacks"]=allow_passive[bird]->Checked;
     //  current->FieldValues["passive_odds"]=passive_odds[bird]->Value;
     //  current->FieldValues["passive_quota"]=passive_quota[bird]->Value;
     //  current->FieldValues["passive_quota_check"]=passive_quota_check[bird]->Checked;
       current->FieldValues["bio_feedback"]=biofeedback[bird]->Checked;

       current->Post();
   }
}



void TForm1::update_chan_displays(int bird)
{

   CZT_OCX_BirdProps props;
   props.RetrieveAllValues(m_Birds[bird]);
  // update the state display (active, slave, dirrect recording...)
   int state=props.m_lBirdState;//m_Birds[bird]->GetLongValue(vBirdState);
   update_check=false;
   //str=bird+1; str+=". ";

   if(state==1){bird_active[bird]->Checked=true; str2="On trigger"; channel_mode[bird]->ItemIndex=0; }
   else  if(state==5){bird_active[bird]->Checked=true; str2="Simple recorder"; channel_mode[bird]->ItemIndex=1; }
   else  if(state==9){bird_active[bird]->Checked=true; str2="Slave"; channel_mode[bird]->ItemIndex=0; }
   else  if(state==0){bird_active[bird]->Checked=false; str2="Off"; channel_mode[bird]->ItemIndex=2; }


   bird_active[bird]->Caption=str2;
   //save[bird]->Text=props.m_sWaveFilePath.c_str();
   // update real-gain scaler position
   scaler[bird]->Position=10*props.m_fIOM_InputScaler;//m_Birds[bird]->GetFloatValue(vBird_IOM_InputScaler);
   scalerN[bird]->Value=float(scaler[bird]->Position)/10.0;

   // update detection threshold position
   str=props.m_lPeakThreshold;
   Threshold[bird]->Position=str.ToInt()/320;
   gain[bird]->Position=(int)-props.m_fPeakGain;

   // audio channel display
//   str=props.m_sAudioSrcDescription.c_str();//bird->GetStringValue(vAudioSrcDescription);
 //  sound_source[bird]->Caption= str;//.SubString(1, 85);// + "-" + str.SubString(str.Length()-19,20);

   //bird name display
   str=props.m_sBirdName.c_str();
   name[bird]=str;//.ToInt();
   update_check=true;
}




void __fastcall TForm1::direct_recording_durationChange(TObject *Sender)
{
  direct_recording_duration_val->Value=direct_recording_duration->Position;

}
//---------------------------------------------------------------------------



void __fastcall TForm1::scaler1Change(TObject *Sender)
{
 /* static bool first_time=true;
  if(first_time)
  {
        first_time=false;
        Application->MessageBox("Changing recording gain is now enabled (see yellow sliders in the Main tab). Do not change it unless really necessary, and if possible, change input gain instead.", "warning:", MB_OK);
  }*/
  TztBird* Bird; TNumLab *scalerN; TTrackBar *scaler;
  if(Sender==scaler1) { Bird=ztBird1; scalerN=scalerN1; scaler=scaler1; }
  else if(Sender==scaler2) { Bird=ztBird2; scalerN=scalerN2; scaler=scaler2; }
  else if(Sender==scaler3) { Bird=ztBird3; scalerN=scalerN3; scaler=scaler3; }
  else if(Sender==scaler4) { Bird=ztBird4; scalerN=scalerN4; scaler=scaler4; }
  else if(Sender==scaler5) { Bird=ztBird5; scalerN=scalerN5; scaler=scaler5; }
  else if(Sender==scaler6) { Bird=ztBird6; scalerN=scalerN6; scaler=scaler6; }
  else if(Sender==scaler7) { Bird=ztBird7; scalerN=scalerN7; scaler=scaler7; }
  else if(Sender==scaler8) { Bird=ztBird8; scalerN=scalerN8; scaler=scaler8; }
  else if(Sender==scaler9) { Bird=ztBird9; scalerN=scalerN9; scaler=scaler9; }
  else if(Sender==scaler10){ Bird=ztBird10;scalerN=scalerN10;scaler=scaler10; }
  else return;
  Bird->SetFloatValue(vBird_IOM_InputScaler,scaler->Position/10.0);
  scalerN->Value=scaler->Position/10.0;
}
//---------------------------------------------------------------------------




void __fastcall TForm1::displayClick(TObject *Sender)
{

  if(display->Down)
  {
    Form1->Width=970;
    for(int i=0;i<5;i++)
    {
        rec_panel[i*2+1]->Left=470;
        rec_panel[i*2+1]->Top-=97;
    }
    for(int i=0;i<10;i++)
    {
       rec_panel[i]->Height=200;
       m_Birds[i]->Height=195;
    }
    display->Caption="Smaller";
  }
  else
  {
    Form1->Width=510;
    for(int i=0;i<10;i++)
    {
       rec_panel[i]->Height=97;
       m_Birds[i]->Height=95;
    }
    for(int i=0;i<5;i++)
    {
        rec_panel[i*2+1]->Left=2;
        rec_panel[i*2+1]->Top+=97;
    }
    display->Caption="Bigger";
  }
}

//---------------------------------------------------------------------------



void __fastcall TForm1::vol1Change(TObject *Sender)
{
   if(Sender==vol1)ztBird1->SetLongValue(vOutputVolume, -1000*(vol1->Position));
   else if(Sender==vol2)ztBird2->SetLongValue(vOutputVolume, -1000*(vol2->Position));
   else if(Sender==vol3)ztBird3->SetLongValue(vOutputVolume, -1000*(vol3->Position));
   else if(Sender==vol4)ztBird4->SetLongValue(vOutputVolume, -1000*(vol4->Position));
   else if(Sender==vol5)ztBird5->SetLongValue(vOutputVolume, -1000*(vol5->Position));
   else if(Sender==vol6)ztBird6->SetLongValue(vOutputVolume, -1000*(vol6->Position));
   else if(Sender==vol7)ztBird7->SetLongValue(vOutputVolume, -1000*(vol7->Position));
   else if(Sender==vol8)ztBird8->SetLongValue(vOutputVolume, -1000*(vol8->Position));
   else if(Sender==vol9)ztBird9->SetLongValue(vOutputVolume, -1000*(vol9->Position));
   else if(Sender==vol10)ztBird10->SetLongValue(vOutputVolume, -1000*(vol10->Position));
  //if(Sender==vol1)
 // do something...
}


void __fastcall TForm1::keypeck_TbChange(TObject *Sender)
{
  keypeckVal->Value=keypeck_Tb->Position;
  keypeckTable->Limit=keypeckVal->Value;
}
//---------------------------------------------------------------------------



void __fastcall TForm1::statClick(TObject *Sender)
{
  if(stat->Down) for(int i=0;i<10;i++) m_Birds[i]->SetLongValue(vRecordingDisplay,display_VisualizationStats);
  else  for(int i=0;i<10;i++) m_Birds[i]->SetLongValue(vRecordingDisplay,display_VisualizationOnly);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::enable_operantClick(TObject *Sender)
{
  if(enable_operant->Checked)operant_tasks->Visible=true;
  else operant_tasks->Visible=false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::num_portsChange(TObject *Sender)
{
  num_portsVal->Value=num_ports->Position;
  configDataCard();
}
//---------------------------------------------------------------------------


// this function addbackslash to strings containing folder addresses
// e.g., c:\\ofer will turn into c:\\\\ofer which is going to be handled
// as c:\ofer in the mySQL command (via myCommand).
AnsiString TForm1::fix_folder_string(AnsiString folder_str)
{
   AnsiString fixed_string;
   fixed_string.SetLength(100);
   int fixed_cursor=1;
   int length=folder_str.Length();
   for(int i=1;i<=length;i++)
   {
        if(folder_str[i]=='\\')
        {
           fixed_string[fixed_cursor]='\\';
           fixed_cursor++;
           fixed_string[fixed_cursor]='\\';
           fixed_cursor++;
        }
        else
        {

           fixed_string[fixed_cursor]=folder_str[i];
           fixed_cursor++;
        }
   }
   fixed_string.SetLength(fixed_cursor-1);
   return(fixed_string);
}









void __fastcall TForm1::save_configClick(TObject *Sender)
{
  Enable_folder_changing();
  save_all_configs();
  for(int i=0;i<10;i++)save_channel_attributes(m_Birds[i], i+1);
  for(int i=0;i<20;i++)save_session_attributes(i);
  MessageDlgPos("Configuration saved.", mtInformation, TMsgDlgButtons() << mbOK, 0, 40, 20);
}
//---------------------------------------------------------------------------





void __fastcall TForm1::reset_counts0Click(TObject *Sender)
{
       if(Sender==reset_counts0) Play_counts0->Value=0;
  else if(Sender==reset_counts1) Play_counts1->Value=0;
  else if(Sender==reset_counts2) Play_counts2->Value=0;
  else if(Sender==reset_counts3) Play_counts3->Value=0;
  else if(Sender==reset_counts4) Play_counts4->Value=0;
  else if(Sender==reset_counts5) Play_counts5->Value=0;
  else if(Sender==reset_counts6) Play_counts6->Value=0;
  else if(Sender==reset_counts7) Play_counts7->Value=0;
  else if(Sender==reset_counts8) Play_counts8->Value=0;
  else if(Sender==reset_counts9) Play_counts9->Value=0;
  else if(Sender==reset_counts10) Play_counts10->Value=0;
  else if(Sender==reset_counts11) Play_counts11->Value=0;
  else if(Sender==reset_counts12) Play_counts12->Value=0;
  else if(Sender==reset_counts13) Play_counts13->Value=0;
  else if(Sender==reset_counts14) Play_counts14->Value=0;
  else if(Sender==reset_counts15) Play_counts15->Value=0;
  else if(Sender==reset_counts16) Play_counts16->Value=0;
  else if(Sender==reset_counts17) Play_counts17->Value=0;
  else if(Sender==reset_counts18) Play_counts18->Value=0;
  else if(Sender==reset_counts19) Play_counts19->Value=0;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::tmpSound1Change(TObject *Sender)
{

  if(!io_folders_enable)return;
  int bird;
	   if(Sender==tmpSound1 || Sender==save1)bird=0;
  else if(Sender==tmpSound2 || Sender==save2)bird=1;
  else if(Sender==tmpSound3 || Sender==save3)bird=2;
  else if(Sender==tmpSound4 || Sender==save4)bird=3;
  else if(Sender==tmpSound5 || Sender==save5)bird=4;
  else if(Sender==tmpSound6 || Sender==save6)bird=5;
  else if(Sender==tmpSound7 || Sender==save7)bird=6;
  else if(Sender==tmpSound8 || Sender==save8)bird=7;
  else if(Sender==tmpSound9 || Sender==save9)bird=8;
  else if(Sender==tmpSound10 || Sender==save10)bird=9;
  else return;
  //validate_folders(bird);

  CZT_OCX_BirdProps props;
  props.RetrieveAllValues(m_Birds[bird]);
  str=tmpSound[bird]->Text;
  props.m_sTempFilePath=str.c_str();
  str=save[bird]->Text;
  props.m_sWaveFilePath=str.c_str();
  props.PutAllValues(m_Birds[bird]);
 /*
  BSTR myBstr = AnsiToOLESTR((char*)tmpSound[bird]->Text.c_str());
  m_Birds[bird]->SetStringValue(vBirdTempFilePath, myBstr);
  myBstr = AnsiToOLESTR((char*)save[bird]->Text.c_str());
  m_Birds[bird]->SetStringValue(vBirdWaveFilePath, myBstr);
 */
}
//---------------------------------------------------------------------------


void TForm1::validate_folders(int bird)
{
  if(DirectoryExists(tmpSound[bird]->Text)==0)
  {
    try{
          ForceDirectories(tmpSound[bird]->Text);
       }
       catch(...)
       {
          folder_problems=true;
       }
  }

  if(DirectoryExists(save[bird]->Text)==0)
  {
	try{
          ForceDirectories(save[bird]->Text);
	   }
       catch(...)
       {
         folder_problems=true;
       }
  }
}





void TForm1::create_channels()
{
  str="CREATE TABLE `channels` (";
  str+="`chan` int(11) NOT NULL auto_increment,";
  str+="`id` varchar(100) NOT NULL default '',";
  str+="`m_sTempFilePath` varchar(100) NOT NULL default 'c:\\\\SAP\\\\tmpSound\\\\',";
  str+="`morning_session_start` smallint(6) NOT NULL default '900',";
  str+="`morning_session_end` smallint(6) NOT NULL default '1600',";
  str+="`afternoon_session_start` smallint(6) NOT NULL default '1700',";
  str+="`afternoon_session_end` smallint(6) NOT NULL default '2000',";
  str+="`lights_on` smallint(6) NOT NULL default '9',";
  str+="`lights_off` smallint(6) NOT NULL default '21',";
  str+="`stop_training_at_night` tinyint(4) NOT NULL default '0',";
  str+="`direct_recording_epochs` smallint(6) NOT NULL default '30',";
  str+="`interval_between_playbacks` smallint(6) NOT NULL default '20',";
  str+="`NIDAQ_exists` tinyint(4) NOT NULL default '0',";
  str+="`keep_track_of_keypecks` tinyint(4) NOT NULL default '1',";
  str+="`two_soundlists` tinyint(4) NOT NULL default '0',";
  str+="`self_feedback` tinyint(4) NOT NULL default '0',";
  str+="`randomly_select_sounds1` tinyint(4) NOT NULL default '0',";
  str+="`randomly_select_sounds2` tinyint(4) NOT NULL default '0',";
  str+="`auto_Scroll_sounds1` tinyint(4) NOT NULL default '0',";
  str+="`auto_Scroll_sounds2` tinyint(4) NOT NULL default '0',";
  str+="`mute_playbacks` tinyint(4) NOT NULL default '0',";
  str+="`lock_playbacks` tinyint(4) NOT NULL default '0',";
  str+="`lock_sessions` tinyint(4) NOT NULL default '0',";
  str+="`let_go_of_key` tinyint(4) NOT NULL default '1',";
  str+="`m_lPrebufferSamples` int(11) NOT NULL default '300',";
    str+="`m_fPeakGain` float NOT NULL default '30',";
  str+="`m_lBirdState` int(11) NOT NULL default '0',";
  str+="`m_lPeakThreshold` int(11) NOT NULL default '10000',";
  str+="`m_lPeakRecordDuration` int(11) NOT NULL default '1000',";
  str+="`m_lMonitorDisplay` int(11) NOT NULL default '3',";
  str+="`m_lRecordingDisplay` int(11) NOT NULL default '3',";
  str+="`m_lRecordingCutoffMS` int(11) NOT NULL default '30000',";
  str+="`m_lPeakComparator` int(11) NOT NULL default '1000',";
  str+="`m_lPeakHP_Filter` int(11) NOT NULL default '1',";
  str+="`m_lPeakTrigger` int(11) NOT NULL default '0',";
  str+="`m_lIOM_InputFormatIDX` int(20) NOT NULL default '0',";
  str+="`m_lIOM_InputChannel` int(20) NOT NULL default '0',";
  str+="`m_lIOM_InputPan` int(20) NOT NULL default '0',";
  str+="`m_fIOM_InputScaler` float NOT NULL default '1.0',";
  str+="`m_lIOM_InputGain` int(11) NOT NULL default '30',";
  str+="`m_lIOM_OutputFormatIDX` int(20) NOT NULL default '0',";
  str+="`m_lIOM_OutputChannel` int(20) NOT NULL default '0',";
  str+="`m_lIOM_OutputPan` int(11) NOT NULL default '0',";
  str+="`m_lIOM_OutputGain` int(11) NOT NULL default '0',";
  str+="`m_sWaveFile` varchar(100) NOT NULL default '',";
  str+="`m_sMasterID` varchar(100) NOT NULL default '0',";
  str+="`m_sIOM_InputModuleID` varchar(100) NOT NULL default '0',";
  str+="`m_sIOM_InputDeviceID` varchar(100) NOT NULL default '0',";
  str+="`m_sIOM_OutputModuleID` varchar(100) NOT NULL default '0',";
  str+="`m_sIOM_OutputDeviceID` varchar(100) NOT NULL default '0',";
  str+="PRIMARY KEY  (`chan`))";
  MyCommand1->SQL->Clear();
  MyCommand1->SQL->Add(str);
  MyCommand1->Execute(1);

  channels->Active=true;
  channels->Edit();
  channels->Insert(); channels->FieldValues["id"]="a";
  channels->Insert(); channels->FieldValues["id"]="b";
  channels->Insert(); channels->FieldValues["id"]="c";
  channels->Insert(); channels->FieldValues["id"]="d";
  channels->Insert(); channels->FieldValues["id"]="e";
  channels->Insert(); channels->FieldValues["id"]="f";
  channels->Insert(); channels->FieldValues["id"]="g";
  channels->Insert(); channels->FieldValues["id"]="h";
  channels->Insert(); channels->FieldValues["id"]="i";
  channels->Insert(); channels->FieldValues["id"]="j";
  channels->Post();
  channels->Refresh();
}

//---------------------------------------------------------------------------

void __fastcall TForm1::speedChange(TObject *Sender)
{
  int spd=6000-speed->Position;
  for(int i=0;i<10;i++)m_Birds[i]->SetLongValue(vSamplesPerPixel, spd);

}
//---------------------------------------------------------------------------



void __fastcall TForm1::display_typesClick(TObject *Sender)
{
   for(int i=0;i<10;i++)
   {
     if(display_types->ItemIndex==1) // scroll
     {
        speed->Min=10;
        speed->Position=5500;
        m_Birds[i]->SetLongValue(vVisualization,vis_ScrollingWaveform);
        m_Birds[i]->SetLongValue(vSamplesPerPixel, 6000-speed->Position);
     }
     else if(display_types->ItemIndex==0) // osciloscope
     {
        speed->Min=5900;
        speed->Position=5995;
        m_Birds[i]->SetLongValue(vVisualization,vis_WaveformPlot);
        m_Birds[i]->SetLongValue(vSamplesPerPixel, 6000-speed->Position);
     }

     //else  m_Birds[i]->SetLongValue(vVisualization,vis_None);

   }
}
//---------------------------------------------------------------------------



void __fastcall TForm1::pre_thresh_bufferChange(TObject *Sender)
{
     pre_thresh_buffer_val->Value=pre_thresh_buffer->Position;
     int ms_to_samples=pre_thresh_buffer_val->Value*44.1;
     for(int i=0;i<10;i++) m_Birds[i]->SetLongValue(vPrebufferSamples,ms_to_samples);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::port0Click(TObject *Sender)
{
  configDataCard();
}
//---------------------------------------------------------------------------



void __fastcall TForm1::switch_quota_Tb1Change(TObject *Sender)
{
  int bird;
       if(Sender==switch_quota_Tb1)bird=0;
  else if(Sender==switch_quota_Tb2)bird=1;
  else if(Sender==switch_quota_Tb3)bird=2;
  else if(Sender==switch_quota_Tb4)bird=3;
  else if(Sender==switch_quota_Tb5)bird=4;
  else if(Sender==switch_quota_Tb6)bird=5;
  else if(Sender==switch_quota_Tb7)bird=6;
  else if(Sender==switch_quota_Tb8)bird=7;
  else if(Sender==switch_quota_Tb9)bird=8;
  else if(Sender==switch_quota_Tb10)bird=9;
  else return;
  switch_quota[bird]->Value=switch_quota_Tb[bird]->Position;
}
//---------------------------------------------------------------------------




void TForm1::switch_the_lists(int chan)// chan is the chanell index from 0 to 9 for each channel
{
  // detector_list[] comes in trios for each channel: the first is key1, then key2 and then passive
  /*
  if(detector_list[chan*3]->ItemIndex)detector_list[chan*3]->ItemIndex=0; else detector_list[chan*3]->ItemIndex=1;
  if(detector_list[chan*3+1]->ItemIndex)detector_list[chan*3+1]->ItemIndex=0; else detector_list[chan*3+1]->ItemIndex=1;
  if(detector_list[chan*3+2]->ItemIndex)detector_list[chan*3+2]->ItemIndex=0; else detector_list[chan*3+2]->ItemIndex=1;
  */
}



void TForm1::Create_sessions()
{
  DeleteFile("c:\\sap\\birds.txt");
  if(table_exists("sessions")) return;
  str="CREATE TABLE `sessions` (";
  str+="`session_ID` int(11) NOT NULL auto_increment,";
  str+="`sound_folder` varchar(100) NOT NULL default 'c:\\\\sap',";
  str+="`quota` int(11) NOT NULL default '10',";
  str+="`gap` float NOT NULL default '2',";
  str+="`random_switch` tinyint(4) NOT NULL default '0', ";
  str+="`auto_scroll` tinyint(4) NOT NULL default '0',";
  str+="`locked` tinyint(4) NOT NULL default '0',";
  str+="`random_onset` tinyint(4) NOT NULL default '0',";
  str+="`enabled` tinyint(4) NOT NULL default '0',";
  str+="`state` tinyint(4) NOT NULL default '0',";
  str+="`start_hour` int(11) NOT NULL default '0',";
  str+="`start_min` int(11) NOT NULL default '0',";
  str+="`end_hour` int(11) NOT NULL default '0',";
  str+="`end_min` int(11) NOT NULL default '0',";
  str+="`random_onset_delay` int(11) NOT NULL default '0',";
  str+="`playback_list` smallint(6) NOT NULL default '0',";
  str+="`enable_quota` tinyint(4) NOT NULL default '0',";
  str+="`song_played` int(11) NOT NULL default '0',";
  str+="`song_index` int(11) NOT NULL default '0',";
  str+="`gap_timer` int(11) NOT NULL default '0',";
  str+="`operant` int(11) NOT NULL default '0',";
  str+="`odds` float NOT NULL default '0.1',";
  str+="PRIMARY KEY  (`session_ID`));";
  MyCommand1->SQL->Clear();
  MyCommand1->SQL->Add(str);
  MyCommand1->Execute(1);

  sessions->Active=true;
  sessions->Refresh();

  sessions->First();
  for(int i=0;i<20;i++)
  {
	sessions->Edit();
	sessions->Append();
	sessions->Post();
  }
  /*
  current->First();
  for(int i=0;i<11;i++)
  {
	 current->Edit();
	 current->FieldValues["bird_ID"]=i;
	 current->FieldValues["hatching_date"]="2010-01-02";
     current->FieldValues["start_training"]="2010-02-02";
     current->FieldValues["stop_training"]="2010-03-02";
	 current->FieldValues["quota"]=10;
	 current->Post();
	 current->Next();
  }
  */
  sessions->Refresh();
  sessions->First();
}




void TForm1::Create_animals()
{
  if(table_exists("animals")) return;
  str="CREATE TABLE  `animals` (";
  str+="`bird_name` char(10) NOT NULL default '',";
  str+="`sex` char(1) NOT NULL default 'M',";
  str+="`hatched` date NOT NULL default '0000-00-00',";
  str+="`weight` float NOT NULL default '0',";
  str+="`location` smallint(6) NOT NULL default '0',";
  str+="`procedure1` char(12) NOT NULL default 'isolation',";
  str+="`proc1_date` datetime NOT NULL default '0000-00-00 00:00:00',";
  str+="`procedure2` char(12) NOT NULL default 'surgery',";
  str+="`proc2_date` datetime NOT NULL default '0000-00-00 00:00:00',";
  str+="`procedure3` char(12) NOT NULL default 'finish',";
  str+="`proc3_date` datetime NOT NULL default '0000-00-00 00:00:00',";
  str+="`training` datetime NOT NULL default '0000-00-00 00:00:00',";
  str+="`model` char(12) NOT NULL default '',";
  str+="`quota` smallint(6) NOT NULL default '0',";
  str+="`audio` char(16) NOT NULL default '',";
  str+="`attributes` char(12) NOT NULL default '',";
  str+="`photoperiod` char(12) NOT NULL default '',";
  str+="`details` char(80) NOT NULL default '',";
  str+="PRIMARY KEY  (`bird_name`))";
  MyCommand1->SQL->Clear();
  MyCommand1->SQL->Add(str);
  MyCommand1->Execute(1);
}


void __fastcall TForm1::update_tablesClick(TObject *Sender)
{
  current->Refresh();
  retrieve_settings();
  CheckDates();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::help_keysClick(TObject *Sender)
{
Application->HelpContext(890);
}
//---------------------------------------------------------------------------





void __fastcall TForm1::help_playbacks2Click(TObject *Sender)
{
    Application->HelpContext(850);
}
//---------------------------------------------------------------------------




void __fastcall TForm1::timer_intervalChange(TObject *Sender)
{
  timer_val->Value=timer_interval->Position;
  Timer1->Interval=timer_val->Value;
}


//---------------------------------------------------------------------------


void __fastcall TForm1::biofeedbacktimerTimer(TObject *Sender)
{
   AnsiString query, song_file, bird_name;
   for(int i=0;i<10;i++)
   {
       /*
        if(biofeedback[i]->Checked==true)
        {
           query="select bout_duration, file from bouts where bird_name='";
           query+=m_Birds[i]->GetStringValue(vBirdName);
           query+="' order by -1*recnum   limit 10;";
           MyQuery1->SQL->Clear();
           MyQuery1->SQL->Add(query);
           MyQuery1->Execute();
           MyQuery1->First();
           int bout_dur=-1;

           if(!MyQuery1->RecordCount)
           {
             train[i]->Down=false;
			 Application->MessageBox(L"biofeedback trained was stopped because bout table has no data for your bird", L"warning:", MB_OK);
		   }

           else for(int j=0;j<MyQuery1->RecordCount;j++)
           {
                float x=MyQuery1->FieldValues["bout_duration"];
                if(x>bout_dur){bout_dur=x; song_file=MyQuery1->FieldValues["file"];}
                MyQuery1->Next();
           }
           SetBirdString(m_Birds[i],vBirdWaveFile, song_file.c_str());
           song[i]->Caption=ExtractFileName(song_file);

        }
        */

   }
}

//---------------------------------------------------------------------------



void __fastcall TForm1::biofeedback1Click(TObject *Sender)
{
  int i;
  if(Sender==biofeedback1)i=0;
  else if(Sender==biofeedback2)i=1;
  else if(Sender==biofeedback3)i=2;
  else if(Sender==biofeedback4)i=3;
  else if(Sender==biofeedback5)i=4;
  else if(Sender==biofeedback6)i=5;
  else if(Sender==biofeedback7)i=6;
  else if(Sender==biofeedback8)i=7;
  else if(Sender==biofeedback9)i=8;
  if(biofeedback[i]->Checked)feedbackL[i]->Visible=true;
  else feedbackL[i]->Visible=false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::key2MouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
   str="An operant traning session is enabled. ";
   str+="Circles represent keys (buttons) that the bird peck to trigger playbacks. \r";
   str+="The keys are connected to a National Instruments digital I/O card. \r\r";
   str+="The color of the key represents it's state: \r";
   str+="Yellow = position off; \r Red = position on; \r Blue = training inactive (not in session or nothing to play); ";
    MessageDlgPos(str, mtInformation, TMsgDlgButtons() << mbOK, 0, 20, 300);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::recorded1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    str="This bar represents the number of recording sessions (when sound passed threshold)  since the morning.";
    str+=" Max in this bar is 1000.";
    MessageDlgPos(str, mtInformation, TMsgDlgButtons() << mbOK, 0, 20, 300);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Playbacks2MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    str="This is the number of operant playbacks (usually trigered by key-peck) the bird heard during the current session\r\r";
    str+="For operant playabacks to work you will need to have appropriate NI card, and you need to set the Digital I/O channels";
    str+="\r For more information see help chapter 9, step 6: setting playbacks. Note that setting passive playbacks is much easier.";
    MessageDlgPos(str, mtInformation, TMsgDlgButtons() << mbOK, 0, 20, 300);
}
//---------------------------------------------------------------------------



void __fastcall TForm1::passive_played1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    str="This is the number of passive playbacks (usually trigered by key-peck) the bird heard during the current session";
    str+="setting passive playbacks is easy and requires no additional hardware. See help chapter 9 step 6: setting playbacks";
    MessageDlgPos(str, mtInformation, TMsgDlgButtons() << mbOK, 0, 20, 300);
}
//---------------------------------------------------------------------------



void __fastcall TForm1::input0Click(TObject *Sender)
{
   int i;
   if(Sender==input0) i=0;
   else if(Sender==input1) i=1;
   else if(Sender==input2) i=2;
   else if(Sender==input3) i=3;
   else if(Sender==input4) i=4;
   else if(Sender==input5) i=5;
   else if(Sender==input6) i=6;
   else if(Sender==input7) i=7;
   else if(Sender==input8) i=8;
   else if(Sender==input9) i=9;
   else if(Sender==input10) i=10;
   else if(Sender==input11) i=11;
   else if(Sender==input12) i=12;
   else if(Sender==input13) i=13;
   else if(Sender==input14) i=14;
   else if(Sender==input15) i=15;
   else if(Sender==input16) i=16;
   else if(Sender==input17) i=17;
   else if(Sender==input18) i=18;
   else if(Sender==input19) i=19;
   OpenDialog1->Filter = "Sound files (*.wav)|*.WAV";
   OpenDialog1->FileName="not relevant";
   if(OpenDialog1->Execute())
   {
      playbacks_[i]->Directory=ExtractFileDir(OpenDialog1->FileName);
      playbacks_folder[i]->Text=playbacks_[i]->Directory+"\\";
   }
}
//---------------------------------------------------------------------------




void __fastcall TForm1::playbacks_0Click(TObject *Sender)
{
   int i=0;
   if(Sender==playbacks_0)i=0;
   else if(Sender==playbacks_1)i=1;
   else if(Sender==playbacks_2)i=2;
   else if(Sender==playbacks_3)i=3;
   else if(Sender==playbacks_4)i=4;
   else if(Sender==playbacks_5)i=5;
   else if(Sender==playbacks_6)i=6;
   else if(Sender==playbacks_7)i=7;
   else if(Sender==playbacks_8)i=8;
   else if(Sender==playbacks_9)i=9;
   else if(Sender==playbacks_10)i=10;
   else if(Sender==playbacks_11)i=11;
   else if(Sender==playbacks_12)i=12;
   else if(Sender==playbacks_13)i=13;
   else if(Sender==playbacks_14)i=14;
   else if(Sender==playbacks_15)i=15;
   else if(Sender==playbacks_16)i=16;
   else if(Sender==playbacks_17)i=17;
   else if(Sender==playbacks_18)i=18;
   else if(Sender==playbacks_19)i=19;

   song[i]->Caption=ExtractFileName(playbacks_[i]->FileName);
   str=playbacks_[i]->FileName;
   if(playbacks_[i]->ItemIndex>-1) SetBirdString(m_Birds[i/2],vBirdWaveFile,str.c_str());
  // m_Birds[0]->TriggerSound();

}
//---------------------------------------------------------------------------





void __fastcall TForm1::playbacks_folder0KeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
   try
   {playbacks_0->Directory=playbacks_folder0->Text;}
   catch(...)
   {int i=0;};
   //playbacks_folder1->Text=playbacks_1->Directory+"\\";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::modeClick(TObject *Sender)
{
  if(mode->ItemIndex)
  {
        Form1->Width=810;
        sound_scrollBox->Width=780;
        for(int i=0;i<20;i++)
        {
         session_playback_list[i]->Visible=true;
         playback_list[i]->Visible=true;
        // Session[i].playback_list=0;
        }
  }
  else
  {
        Form1->Width=510;
        sound_scrollBox->Width=400;
        for(int i=0;i<20;i++)
        {
        // session_playback_list[i]->ItemIndex=0;
         session_playback_list[i]->Visible=false;
         playback_list[i]->Visible=false;
        }
  }
}
//---------------------------------------------------------------------------



void __fastcall TForm1::session_min1DownClick(TObject *Sender)
{
   int i;
   if(Sender==session_min1)i=0;
   else if(Sender==session_min2)i=1;
   else if(Sender==session_min3)i=2;
   else if(Sender==session_min4)i=3;
   else if(Sender==session_min5)i=4;
   else if(Sender==session_min6)i=5;
   else if(Sender==session_min7)i=6;
   else if(Sender==session_min8)i=7;
   else if(Sender==session_min9)i=8;
   else if(Sender==session_min10)i=9;
   else if(Sender==session_min11)i=10;
   else if(Sender==session_min12)i=11;
   else if(Sender==session_min13)i=12;
   else if(Sender==session_min14)i=13;
   else if(Sender==session_min15)i=14;
   else if(Sender==session_min16)i=15;
   else if(Sender==session_min17)i=16;
   else if(Sender==session_min18)i=17;
   else if(Sender==session_min19)i=18;
   else if(Sender==session_min20)i=19;
   else if(Sender==session_min21)i=20;
   else if(Sender==session_min22)i=21;
   else if(Sender==session_min23)i=22;
   else if(Sender==session_min24)i=23;
   else if(Sender==session_min25)i=24;
   else if(Sender==session_min26)i=25;
   else if(Sender==session_min27)i=26;
   else if(Sender==session_min28)i=27;
   else if(Sender==session_min29)i=28;
   else if(Sender==session_min30)i=29;
   else if(Sender==session_min31)i=30;
   else if(Sender==session_min32)i=31;
   else if(Sender==session_min33)i=32;
   else if(Sender==session_min34)i=33;
   else if(Sender==session_min35)i=34;
   else if(Sender==session_min36)i=35;
   else if(Sender==session_min37)i=36;
   else if(Sender==session_min38)i=37;
   else if(Sender==session_min39)i=38;
   else if(Sender==session_min40)i=39;

   if(!(i%2)) // even i means start
   {
     if(Session[i/2].start_min>0)Session[i/2].start_min--;
     else
     {
       Session[i/2].start_min=59;
       if(Session[i/2].start_hour>1)Session[i/2].start_hour--;
       else Session[i/2].start_hour=24;
     }
     session_startL[i/2]->Caption=calc_time_label(Session[i/2].start_hour, Session[i/2].start_min);
   }
   else
   {
     if(Session[i/2].end_min>0)Session[i/2].end_min--;
     else
     {
       Session[i/2].end_min=59;
       if(Session[i/2].end_hour>1)Session[i/2].end_hour--;
       else Session[i/2].end_hour=24;
     }
     session_endL[i/2]->Caption=calc_time_label(Session[i/2].end_hour, Session[i/2].end_min);
   }
   Handle_training_state(i/4);
}

//---------------------------------------------------------------------------

AnsiString TForm1::calc_time_label(int hour, int minute)
{
   AnsiString ampm;
   int hour_s;
   if(hour>12) {ampm="PM"; hour_s=hour-12; }
   else {ampm="AM"; hour_s=hour; }
   str="";
   if(hour_s<10)str="0";
   str+=hour_s;
   str+=":";
   if(minute<10)str+="0";
   str+=minute;
   str+=ampm;
   return(str);
}

//---------------------------------------------------------------------------


void __fastcall TForm1::session_min1UpClick(TObject *Sender)
{
   int i;
   if(Sender==session_min1)i=0;
   else if(Sender==session_min2)i=1;
   else if(Sender==session_min3)i=2;
   else if(Sender==session_min4)i=3;
   else if(Sender==session_min5)i=4;
   else if(Sender==session_min6)i=5;
   else if(Sender==session_min7)i=6;
   else if(Sender==session_min8)i=7;
   else if(Sender==session_min9)i=8;
   else if(Sender==session_min10)i=9;
   else if(Sender==session_min11)i=10;
   else if(Sender==session_min12)i=11;
   else if(Sender==session_min13)i=12;
   else if(Sender==session_min14)i=13;
   else if(Sender==session_min15)i=14;
   else if(Sender==session_min16)i=15;
   else if(Sender==session_min17)i=16;
   else if(Sender==session_min18)i=17;
   else if(Sender==session_min19)i=18;
   else if(Sender==session_min20)i=19;
   else if(Sender==session_min21)i=20;
   else if(Sender==session_min22)i=21;
   else if(Sender==session_min23)i=22;
   else if(Sender==session_min24)i=23;
   else if(Sender==session_min25)i=24;
   else if(Sender==session_min26)i=25;
   else if(Sender==session_min27)i=26;
   else if(Sender==session_min28)i=27;
   else if(Sender==session_min29)i=28;
   else if(Sender==session_min30)i=29;
   else if(Sender==session_min31)i=30;
   else if(Sender==session_min32)i=31;
   else if(Sender==session_min33)i=32;
   else if(Sender==session_min34)i=33;
   else if(Sender==session_min35)i=34;
   else if(Sender==session_min36)i=35;
   else if(Sender==session_min37)i=36;
   else if(Sender==session_min38)i=37;
   else if(Sender==session_min39)i=38;
   else if(Sender==session_min40)i=39;

   if(!(i%2)) // even i means start
   {
     if(Session[i/2].start_min<59)Session[i/2].start_min++;
     else
     {
       Session[i/2].start_min=0;
       if(Session[i/2].start_hour<24)Session[i/2].start_hour++;
       else Session[i/2].start_hour=1;
     }
     session_startL[i/2]->Caption=calc_time_label(Session[i/2].start_hour, Session[i/2].start_min);
  }
  else
  {
     if(Session[i/2].end_min<59)Session[i/2].end_min++;
     else
     {
       Session[i/2].end_min=0;
       if(Session[i/2].end_hour<24)Session[i/2].end_hour++;
       else Session[i/2].end_hour=1;
     }
     session_endL[i/2]->Caption=calc_time_label(Session[i/2].end_hour, Session[i/2].end_min);
  }
  Handle_training_state(i/4);
}


//---------------------------------------------------------------------------

void __fastcall TForm1::session_hours1UpClick(TObject *Sender)
{
   int i;
   if(Sender==session_hours1)i=0;
   else if(Sender==session_hours2)i=1;
   else if(Sender==session_hours3)i=2;
   else if(Sender==session_hours4)i=3;
   else if(Sender==session_hours5)i=4;
   else if(Sender==session_hours6)i=5;
   else if(Sender==session_hours7)i=6;
   else if(Sender==session_hours8)i=7;
   else if(Sender==session_hours9)i=8;
   else if(Sender==session_hours10)i=9;
   else if(Sender==session_hours11)i=10;
   else if(Sender==session_hours12)i=11;
   else if(Sender==session_hours13)i=12;
   else if(Sender==session_hours14)i=13;
   else if(Sender==session_hours15)i=14;
   else if(Sender==session_hours16)i=15;
   else if(Sender==session_hours17)i=16;
   else if(Sender==session_hours18)i=17;
   else if(Sender==session_hours19)i=18;
   else if(Sender==session_hours20)i=19;
   else if(Sender==session_hours21)i=20;
   else if(Sender==session_hours22)i=21;
   else if(Sender==session_hours23)i=22;
   else if(Sender==session_hours24)i=23;
   else if(Sender==session_hours25)i=24;
   else if(Sender==session_hours26)i=25;
   else if(Sender==session_hours27)i=26;
   else if(Sender==session_hours28)i=27;
   else if(Sender==session_hours29)i=28;
   else if(Sender==session_hours30)i=29;
   else if(Sender==session_hours31)i=30;
   else if(Sender==session_hours32)i=31;
   else if(Sender==session_hours33)i=32;
   else if(Sender==session_hours34)i=33;
   else if(Sender==session_hours35)i=34;
   else if(Sender==session_hours36)i=35;
   else if(Sender==session_hours37)i=36;
   else if(Sender==session_hours38)i=37;
   else if(Sender==session_hours39)i=38;
   else if(Sender==session_hours40)i=39;

   if(!(i%2)) // even i means start
   {
      if(Session[i/2].start_hour<24)Session[i/2].start_hour++;
      else Session[i/2].start_hour=1;
      session_startL[i/2]->Caption=calc_time_label(Session[i/2].start_hour, Session[i/2].start_min);
   }

   else
   {
      if(Session[i/2].end_hour<24)Session[i/2].end_hour++;
      else Session[i/2].end_hour=1;
      session_endL[i/2]->Caption=calc_time_label(Session[i/2].end_hour, Session[i/2].end_min);
   }

   Handle_training_state(i/4);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::session_hours1DownClick(TObject *Sender)
{
   int i;
   if(Sender==session_hours1)i=0;
   else if(Sender==session_hours2)i=1;
   else if(Sender==session_hours3)i=2;
   else if(Sender==session_hours4)i=3;
   else if(Sender==session_hours5)i=4;
   else if(Sender==session_hours6)i=5;
   else if(Sender==session_hours7)i=6;
   else if(Sender==session_hours8)i=7;
   else if(Sender==session_hours9)i=8;
   else if(Sender==session_hours10)i=9;
   else if(Sender==session_hours11)i=10;
   else if(Sender==session_hours12)i=11;
   else if(Sender==session_hours13)i=12;
   else if(Sender==session_hours14)i=13;
   else if(Sender==session_hours15)i=14;
   else if(Sender==session_hours16)i=15;
   else if(Sender==session_hours17)i=16;
   else if(Sender==session_hours18)i=17;
   else if(Sender==session_hours19)i=18;
   else if(Sender==session_hours20)i=19;
   else if(Sender==session_hours21)i=20;
   else if(Sender==session_hours22)i=21;
   else if(Sender==session_hours23)i=22;
   else if(Sender==session_hours24)i=23;
   else if(Sender==session_hours25)i=24;
   else if(Sender==session_hours26)i=25;
   else if(Sender==session_hours27)i=26;
   else if(Sender==session_hours28)i=27;
   else if(Sender==session_hours29)i=28;
   else if(Sender==session_hours30)i=29;
   else if(Sender==session_hours31)i=30;
   else if(Sender==session_hours32)i=31;
   else if(Sender==session_hours33)i=32;
   else if(Sender==session_hours34)i=33;
   else if(Sender==session_hours35)i=34;
   else if(Sender==session_hours36)i=35;
   else if(Sender==session_hours37)i=36;
   else if(Sender==session_hours38)i=37;
   else if(Sender==session_hours39)i=38;
   else if(Sender==session_hours40)i=39;

   if(!(i%2)) // even i means start_hour
   {
     if(Session[i/2].start_hour>1)Session[i/2].start_hour--;
     else Session[i/2].start_hour=24;
     session_startL[i/2]->Caption=calc_time_label(Session[i/2].start_hour, Session[i/2].start_min);
   }
   else
   {
     if(Session[i/2].end_hour>1)Session[i/2].end_hour--;
     else Session[i/2].end_hour=24;
     session_endL[i/2]->Caption=calc_time_label(Session[i/2].end_hour, Session[i/2].end_min);
   }
   Handle_training_state(i/4);
}
//---------------------------------------------------------------------------






void __fastcall TForm1::key_track1Click(TObject *Sender)
{
  key_track->Checked=key_track1->Checked;
}
//---------------------------------------------------------------------------




void __fastcall TForm1::lock0Click(TObject *Sender)
{
   int i;
   if(Sender==lock0)i=0;
   else if(Sender==lock1)i=1;
   else if(Sender==lock2)i=2;
   else if(Sender==lock3)i=3;
   else if(Sender==lock4)i=4;
   else if(Sender==lock5)i=5;
   else if(Sender==lock6)i=6;
   else if(Sender==lock7)i=7;
   else if(Sender==lock8)i=8;
   else if(Sender==lock9)i=9;

   Playback_lock(i);
}



void TForm1::Playback_lock(int i)
{
   int k;
   if(lock_playbacks[i]->Down)
   {
       for(k=0;k<2;k++)
       {
        playbacks_[i*2+k]->Color=clBtnFace;
        playbacks_[i*2+k]->Enabled=false;
        playback_mute_rec[i]->Enabled=false;
        //session_playback_control[i*2+k]->Color=clBtnFace;
        //session_playback_control[i*2+k]->Enabled=false;
        playbacks_folder[i*2+k]->Color=clBtnFace;
        playbacks_folder[i*2+k]->Enabled=false;
        playback_input[i*2+k]->Enabled=false;
        play_sound[i*2+k]->Enabled=false;
        reset_counts[i*2+k]->Enabled=false;
        random_switch[i*2+k]->Enabled=false;
        auto_scroll[i*2+k]->Enabled=false;
       }
        switch_lists[i]->Enabled=false;
        switch_quota_Tb[i]->Enabled=false;
        lock_playbacks[i]->Caption="Unlock";
   }
   else
   {
      for(k=0;k<2;k++)
      {
       playbacks_[i*2+k]->Enabled=true;
       playbacks_[i*2+k]->Color=clWhite;
       playback_mute_rec[i]->Enabled=true;
       //session_playback_control[i*2+k]->Enabled=true;
       //session_playback_control[i*2+k]->Color=clWhite;
       playbacks_folder[i*2+k]->Color=clWhite;
       playbacks_folder[i*2+k]->Enabled=true;
       playback_input[i*2+k]->Enabled=true;
       if(!btnStartEngine->Enabled)play_sound[i*2+k]->Enabled=true;
       reset_counts[i*2+k]->Enabled=true;
       random_switch[i*2+k]->Enabled=true;
       auto_scroll[i*2+k]->Enabled=true;
      }
      switch_lists[i]->Enabled=true;
      switch_quota_Tb[i]->Enabled=true;
      lock_playbacks[i]->Caption="Lock";
   }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::session_lock2Click(TObject *Sender)
{
int i;
  if(Sender==session_lock1)i=0;
  else if(Sender==session_lock2)i=1;
  else if(Sender==session_lock3)i=2;
  else if(Sender==session_lock4)i=3;
  else if(Sender==session_lock5)i=4;
  else if(Sender==session_lock6)i=5;
  else if(Sender==session_lock7)i=6;
  else if(Sender==session_lock8)i=7;
  else if(Sender==session_lock9)i=8;
  else if(Sender==session_lock10)i=9;

  Session_lock(i);
}

void TForm1::Session_lock(int i)
{
  if(session_Lock[i]->Down)
  {
        session_Lock[i]->Caption="Unlock";
        training_session[i*2]->Enabled=false;
        training_session[i*2+1]->Enabled=false;

        session_startL[i*2]->Enabled=false;
        session_startL[i*2+1]->Enabled=false;

        session_endL[i*2]->Enabled=false;
        session_endL[i*2+1]->Enabled=false;

        onset_timing[i*2]->Enabled=false;
        onset_timing[i*2+1]->Enabled=false;

        enable_quota[i*2]->Enabled=false;
        enable_quota[i*2+1]->Enabled=false;

        quota_UpDown[i*2]->Enabled=false;
        quota_UpDown[i*2+1]->Enabled=false;

        gap_UpDown[i*2]->Enabled=false;
        gap_UpDown[i*2+1]->Enabled=false;

        passive_odds_UpDown[i*2]->Enabled=false;
        passive_odds_UpDown[i*2+1]->Enabled=false;

        session_playback_list[i*2]->Enabled=false;
        session_playback_list[i*2+1]->Enabled=false;
        session_playback_control[i*2]->Enabled=false;
        session_playback_control[i*2+1]->Enabled=false;

        session_hours[i*4]->Visible=false;
        session_hours[i*4+1]->Visible=false;
        session_hours[i*4+2]->Visible=false;
        session_hours[i*4+3]->Visible=false;

        session_min[i*4]->Visible=false;
        session_min[i*4+1]->Visible=false;
        session_min[i*4+2]->Visible=false;
        session_min[i*4+3]->Visible=false;
  }
  else
  {
        session_Lock[i]->Caption="Lock";
        training_session[i*2]->Enabled=true;
        training_session[i*2+1]->Enabled=true;

        session_startL[i*2]->Enabled=true;
        session_startL[i*2+1]->Enabled=true;

        session_endL[i*2]->Enabled=true;
        session_endL[i*2+1]->Enabled=true;

        onset_timing[i*2]->Enabled=true;
        onset_timing[i*2+1]->Enabled=true;

        enable_quota[i*2]->Enabled=true;
        enable_quota[i*2+1]->Enabled=true;

        quota_UpDown[i*2]->Enabled=true;
        quota_UpDown[i*2+1]->Enabled=true;

        gap_UpDown[i*2]->Enabled=true;
        gap_UpDown[i*2+1]->Enabled=true;

        gap_UpDown[i*2]->Enabled=true;
        gap_UpDown[i*2+1]->Enabled=true;

        passive_odds_UpDown[i*2]->Enabled=true;
        passive_odds_UpDown[i*2+1]->Enabled=true;

        session_playback_list[i*2]->Enabled=true;
        session_playback_list[i*2+1]->Enabled=true;
        session_playback_control[i*2]->Enabled=true;
        session_playback_control[i*2+1]->Enabled=true;

        session_hours[i*4]->Visible=true;
        session_hours[i*4+1]->Visible=true;
        session_hours[i*4+2]->Visible=true;
        session_hours[i*4+3]->Visible=true;

        session_min[i*4]->Visible=true;
        session_min[i*4+1]->Visible=true;
        session_min[i*4+2]->Visible=true;
        session_min[i*4+3]->Visible=true;
  }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::BitBtn2Click(TObject *Sender)
{
  playbacks_control->ActivePage=playbacks_sounds;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn3Click(TObject *Sender)
{
  playbacks_control->ActivePage=playbacks_sessions;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn4Click(TObject *Sender)
{
  controls->ActivePage=devices;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn5Click(TObject *Sender)
{
   playbacks_control->ActivePage=playbacks_feedback;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::session1Click(TObject *Sender)
{
  int i;
  if(Sender==session1)i=0;
  else if(Sender==session2)i=1;
  else if(Sender==session3)i=2;
  else if(Sender==session4)i=3;
  else if(Sender==session5)i=4;
  else if(Sender==session6)i=5;
  else if(Sender==session7)i=6;
  else if(Sender==session8)i=7;
  else if(Sender==session9)i=8;
  else if(Sender==session10)i=9;
  else if(Sender==session11)i=10;
  else if(Sender==session12)i=11;
  else if(Sender==session13)i=12;
  else if(Sender==session14)i=13;
  else if(Sender==session15)i=14;
  else if(Sender==session16)i=15;
  else if(Sender==session17)i=16;
  else if(Sender==session18)i=17;
  else if(Sender==session19)i=18;
  else if(Sender==session20)i=19;
  update_session(i);
}
//---------------------------------------------------------------------------

void TForm1::update_session(int i)
{
  if(training_session[i]->Checked){Session[i].enabled=true; session_panel[i]->Visible=true;}
  else {Session[i].enabled=false; session_panel[i]->Visible=false;}
  Handle_training_state(i/2);

}



void __fastcall TForm1::gap_UpDown1Changing(TObject *Sender,
      bool &AllowChange)
{
 //float fr=1000/Timer1->Interval;
 if(Sender==gap_UpDown1) { gap1->Value=(float)gap_UpDown1->Position/10.0; Session[0].gap=gap1->Value; return; }
 if(Sender==gap_UpDown2) { gap2->Value=(float)gap_UpDown2->Position/10.0; Session[1].gap=gap2->Value; return; }
 if(Sender==gap_UpDown3) { gap3->Value=(float)gap_UpDown3->Position/10.0; Session[2].gap=gap3->Value; return; }
 if(Sender==gap_UpDown4) { gap4->Value=(float)gap_UpDown4->Position/10.0; Session[3].gap=gap4->Value; return; }
 if(Sender==gap_UpDown5) { gap5->Value=(float)gap_UpDown5->Position/10.0; Session[4].gap=gap5->Value; return; }
 if(Sender==gap_UpDown6) { gap6->Value=(float)gap_UpDown6->Position/10.0; Session[5].gap=gap6->Value; return; }
 if(Sender==gap_UpDown7) { gap7->Value=(float)gap_UpDown7->Position/10.0; Session[6].gap=gap7->Value; return; }
 if(Sender==gap_UpDown8) { gap8->Value=(float)gap_UpDown8->Position/10.0; Session[7].gap=gap8->Value; return; }
 if(Sender==gap_UpDown9) { gap9->Value=(float)gap_UpDown9->Position/10.0; Session[8].gap=gap9->Value; return; }
 if(Sender==gap_UpDown10){gap10->Value=(float)gap_UpDown10->Position/10.0; Session[9].gap=gap10->Value; return; }
 if(Sender==gap_UpDown11) { gap11->Value=(float)gap_UpDown11->Position/10.0; Session[10].gap=gap11->Value; return; }
 if(Sender==gap_UpDown12) { gap12->Value=(float)gap_UpDown12->Position/10.0; Session[11].gap=gap12->Value; return; }
 if(Sender==gap_UpDown13) { gap13->Value=(float)gap_UpDown13->Position/10.0; Session[12].gap=gap13->Value; return; }
 if(Sender==gap_UpDown14) { gap14->Value=(float)gap_UpDown14->Position/10.0; Session[13].gap=gap14->Value; return; }
 if(Sender==gap_UpDown15) { gap15->Value=(float)gap_UpDown15->Position/10.0; Session[14].gap=gap15->Value; return; }
 if(Sender==gap_UpDown16) { gap16->Value=(float)gap_UpDown16->Position/10.0; Session[15].gap=gap16->Value; return; }
 if(Sender==gap_UpDown17) { gap17->Value=(float)gap_UpDown17->Position/10.0; Session[16].gap=gap17->Value; return; }
 if(Sender==gap_UpDown18) { gap18->Value=(float)gap_UpDown18->Position/10.0; Session[17].gap=gap18->Value; return; }
 if(Sender==gap_UpDown19) { gap19->Value=(float)gap_UpDown19->Position/10.0; Session[18].gap=gap19->Value; return; }
 if(Sender==gap_UpDown20){gap20->Value=(float)gap_UpDown20->Position/10.0; Session[19].gap=gap20->Value; return; }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::passive_odds_UpDown1Changing(TObject *Sender,
      bool &AllowChange)
{
 int i=0;
 if(Sender==passive_odds_UpDown1) i=0;//{ passive_odds1->Value=(float)passive_odds_UpDown1->Position/100.0; Session[0].odds=passive_odds1->Value; return; }
 if(Sender==passive_odds_UpDown2) i=1;//{ passive_odds2->Value=(float)passive_odds_UpDown2->Position/100.0; Session[1].odds=passive_odds2->Value;return; }
 if(Sender==passive_odds_UpDown3) i=2;//{ passive_odds3->Value=(float)passive_odds_UpDown3->Position/100.0; Session[2].odds=passive_odds3->Value;return; }
 if(Sender==passive_odds_UpDown4) i=3;//{ passive_odds4->Value=(float)passive_odds_UpDown4->Position/100.0; Session[3].odds=passive_odds4->Value;return; }
 if(Sender==passive_odds_UpDown5) i=4;//{ passive_odds5->Value=(float)passive_odds_UpDown5->Position/100.0; Session[4].odds=passive_odds5->Value;return; }
 if(Sender==passive_odds_UpDown6) i=5;//{ passive_odds6->Value=(float)passive_odds_UpDown6->Position/100.0; Session[5].odds=passive_odds6->Value;return; }
 if(Sender==passive_odds_UpDown7) i=6;//{ passive_odds7->Value=(float)passive_odds_UpDown7->Position/100.0; Session[6].odds=passive_odds7->Value;return; }
 if(Sender==passive_odds_UpDown8) i=7;//{ passive_odds8->Value=(float)passive_odds_UpDown8->Position/100.0; Session[7].odds=passive_odds8->Value;return; }
 if(Sender==passive_odds_UpDown9) i=8;//{ passive_odds9->Value=(float)passive_odds_UpDown9->Position/100.0; Session[8].odds=passive_odds9->Value;return; }
 if(Sender==passive_odds_UpDown10)i=9;//{passive_odds10->Value=(float)passive_odds_UpDown10->Position/100.0;Session[9].odds=passive_odds10->Value;return; }
 if(Sender==passive_odds_UpDown11)i=10;// { passive_odds11->Value=(float)passive_odds_UpDown11->Position/100.0; Session[10].odds=passive_odds11->Value;return; }
 if(Sender==passive_odds_UpDown12)i=11;// { passive_odds12->Value=(float)passive_odds_UpDown12->Position/100.0; Session[11].odds=passive_odds12->Value;return; }
 if(Sender==passive_odds_UpDown13)i=12;// { passive_odds13->Value=(float)passive_odds_UpDown13->Position/100.0; Session[12].odds=passive_odds13->Value;return; }
 if(Sender==passive_odds_UpDown14)i=13;// { passive_odds14->Value=(float)passive_odds_UpDown14->Position/100.0; Session[13].odds=passive_odds14->Value;return; }
 if(Sender==passive_odds_UpDown15)i=14;// { passive_odds15->Value=(float)passive_odds_UpDown15->Position/100.0; Session[14].odds=passive_odds15->Value;return; }
 if(Sender==passive_odds_UpDown16)i=15;// { passive_odds16->Value=(float)passive_odds_UpDown16->Position/100.0; Session[15].odds=passive_odds16->Value;return; }
 if(Sender==passive_odds_UpDown17)i=16;// { passive_odds17->Value=(float)passive_odds_UpDown17->Position/100.0; Session[16].odds=passive_odds17->Value;return; }
 if(Sender==passive_odds_UpDown18)i=17;// { passive_odds18->Value=(float)passive_odds_UpDown18->Position/100.0; Session[17].odds=passive_odds18->Value;return; }
 if(Sender==passive_odds_UpDown19)i=18;// { passive_odds19->Value=(float)passive_odds_UpDown19->Position/100.0; Session[18].odds=passive_odds19->Value;return; }
 if(Sender==passive_odds_UpDown20)i=19;//{passive_odds20->Value=(float)passive_odds_UpDown20->Position/100.0;Session[19].odds=passive_odds20->Value;return; }

  float x=passive_odds_UpDown[i]->Position/100.0;
  if(x>0.01)passive_odds[i]->Value=x;
  else passive_odds[i]->Value=passive_odds[i]->Value/2;
  Session[i].odds=passive_odds[i]->Value;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::session_playback_control0Change(TObject *Sender)
{
  int i;
  if(Sender==session_playback_control0)i=0;
  else if(Sender==session_playback_control1)i=1;
  else if(Sender==session_playback_control2)i=2;
  else if(Sender==session_playback_control3)i=3;
  else if(Sender==session_playback_control4)i=4;
  else if(Sender==session_playback_control5)i=5;
  else if(Sender==session_playback_control6)i=6;
  else if(Sender==session_playback_control7)i=7;
  else if(Sender==session_playback_control8)i=8;
  else if(Sender==session_playback_control9)i=9;
  else if(Sender==session_playback_control10)i=10;
  else if(Sender==session_playback_control11)i=11;
  else if(Sender==session_playback_control12)i=12;
  else if(Sender==session_playback_control13)i=13;
  else if(Sender==session_playback_control14)i=14;
  else if(Sender==session_playback_control15)i=15;
  else if(Sender==session_playback_control16)i=16;
  else if(Sender==session_playback_control17)i=17;
  else if(Sender==session_playback_control18)i=18;
  else if(Sender==session_playback_control19)i=19;

  // first set the session parameters
  if(!session_playback_control[i]->ItemIndex) // Passive playbacks
  {Session[i].operant=0; passive_panel[i]->Visible=true;}
  else if(session_playback_control[i]->ItemIndex==1) // Key 1 activated
  {Session[i].operant=1;passive_panel[i]->Visible=false;}
  else if(session_playback_control[i]->ItemIndex==2) // Key 2 activated
  {Session[i].operant=2;passive_panel[i]->Visible=false;}
  else if(session_playback_control[i]->ItemIndex==3) // Key 1 | 2 activated
  {Session[i].operant=3;passive_panel[i]->Visible=false;}
  Handle_training_state(i/2);


  // then set visiblity and states
}
//---------------------------------------------------------------------------

void __fastcall TForm1::session_playback_list0Change(TObject *Sender)
{
  int i;
  if(Sender==session_playback_list0)i=0;
  else if(Sender==session_playback_list1)i=1;
  else if(Sender==session_playback_list2)i=2;
  else if(Sender==session_playback_list3)i=3;
  else if(Sender==session_playback_list4)i=4;
  else if(Sender==session_playback_list5)i=5;
  else if(Sender==session_playback_list6)i=6;
  else if(Sender==session_playback_list7)i=7;
  else if(Sender==session_playback_list8)i=8;
  else if(Sender==session_playback_list9)i=9;
  else if(Sender==session_playback_list10)i=10;
  else if(Sender==session_playback_list11)i=11;
  else if(Sender==session_playback_list12)i=12;
  else if(Sender==session_playback_list13)i=13;
  else if(Sender==session_playback_list14)i=14;
  else if(Sender==session_playback_list15)i=15;
  else if(Sender==session_playback_list16)i=16;
  else if(Sender==session_playback_list17)i=17;
  else if(Sender==session_playback_list18)i=18;
  else if(Sender==session_playback_list19)i=19;
  // playback_list; // 0 -- list 1; 1 -- list 2; 2 -- key dependent key1-list 1 key2 -- list2


  if(session_playback_list[i]->ItemIndex==0)// sounds list 1
  {
        session_playback_control[i]->Enabled=true;
        Session[i].playback_list=0;
  }
  else if(session_playback_list[i]->ItemIndex==1)// sounds list 1a
  {
        session_playback_control[i]->Enabled=true;
        Session[i].playback_list=1;

  }
  else if(session_playback_list[i]->ItemIndex==2)// Key1->list8 ; Key2->list8a
  {
     session_playback_control[i]->ItemIndex=3;
     Session[i].operant=3;
     session_playback_control[i]->Enabled=false;
     passive_panel[i]->Visible=false;
     Session[i].playback_list=2;

  }

}
//---------------------------------------------------------------------------



void __fastcall TForm1::channel_mode1Click(TObject *Sender)
{
 int i;
 if(Sender==channel_mode1)i=0;
 else if(Sender==channel_mode2)i=1;
 else if(Sender==channel_mode3)i=2;
 else if(Sender==channel_mode4)i=3;
 else if(Sender==channel_mode5)i=4;
 else if(Sender==channel_mode6)i=5;
 else if(Sender==channel_mode7)i=6;
 else if(Sender==channel_mode8)i=7;
 else if(Sender==channel_mode9)i=8;
 else if(Sender==channel_mode10)i=9;

 if(channel_mode[i]->ItemIndex==0) m_Birds[i]->SetLongValue(vBirdState,ztbs_Active);
 else if(channel_mode[i]->ItemIndex==1)m_Birds[i]->SetLongValue(vBirdState,5);
 else if(channel_mode[i]->ItemIndex==2)m_Birds[i]->SetLongValue(vBirdState,ztbs_Unitialized);
 update_chan_displays(i);
}
//---------------------------------------------------------------------------






void __fastcall TForm1::FormShow(TObject *Sender)
{
	Form1->Top=0;
	Form1->Left=0;
	gain[0]=gain1; gain[1]=gain2; gain[2]=gain3; gain[3]=gain4;
    gain[4]=gain5; gain[5]=gain6; gain[6]=gain7; gain[7]=gain8;
    gain[8]=gain9; gain[9]=gain10;

    Threshold[0]=trckThreshold1; Threshold[1]=trckThreshold2; Threshold[2]=trckThreshold3; Threshold[3]=trckThreshold4;
    Threshold[4]=trckThreshold5; Threshold[5]=trckThreshold6; Threshold[6]=trckThreshold7; Threshold[7]=trckThreshold8;
    Threshold[8]=trckThreshold9; Threshold[9]=trckThreshold10;

     for(int i=0;i<10;i++)
     {
      gain[i]->Brush->Color=clBlue;
      Threshold[i]->Brush->Color=clRed;
     }
}
//---------------------------------------------------------------------------



void __fastcall TForm1::onset_timing1Click(TObject *Sender)
{
  int i;
  if(Sender==onset_timing1)i=0;
  else if(Sender==onset_timing2)i=1;
  else if(Sender==onset_timing3)i=2;
  else if(Sender==onset_timing4)i=3;
  else if(Sender==onset_timing5)i=4;
  else if(Sender==onset_timing6)i=5;
  else if(Sender==onset_timing7)i=6;
  else if(Sender==onset_timing8)i=7;
  else if(Sender==onset_timing9)i=8;
  else if(Sender==onset_timing10)i=9;
  else if(Sender==onset_timing11)i=10;
  else if(Sender==onset_timing12)i=11;
  else if(Sender==onset_timing13)i=12;
  else if(Sender==onset_timing14)i=13;
  else if(Sender==onset_timing15)i=14;
  else if(Sender==onset_timing16)i=15;
  else if(Sender==onset_timing17)i=16;
  else if(Sender==onset_timing18)i=17;
  else if(Sender==onset_timing19)i=18;
  else if(Sender==onset_timing20)i=19;
  Session[i].random_onset=onset_timing[i]->ItemIndex;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::defaultsClick(TObject *Sender)
{
 set_default();
}
//---------------------------------------------------------------------------


void TForm1::read_NIDAQ()
{
   //DAQmxReadDigitalLines(
   //TaskHandle taskHandle, int32 numSampsPerChan, float64 timeout, bool32 fillMode,
   //uInt8 readArray[], uInt32 arraySizeInBytes, int32 *sampsPerChanRead,
   // int32 *numBytesPerSamp, bool32 *reserved);
   DAQmxReadDigitalLines(read_port0,1,10.0,DAQmx_Val_GroupByChannel,port0_data,100,&read,&bytesPerSamp,NULL);
   DAQmxReadDigitalLines(read_port1,1,10.0,DAQmx_Val_GroupByChannel,port1_data,100,&read,&bytesPerSamp,NULL);
   DAQmxReadDigitalLines(read_port2,1,10.0,DAQmx_Val_GroupByChannel,port2_data,100,&read,&bytesPerSamp,NULL);

}

void __fastcall TForm1::NIcard_existsClick(TObject *Sender)
{
   if(NIcard_exists->Checked==true)
   {
      try{configDataCard();}
	  catch(...)
	  {
			Application->MessageBox(L"NI Card configuration error, training might not work properly", NULL, MB_OK);
	  }
      NI_panel->Visible=true;

   }
   else
   {
      NI_panel->Visible=false;
   }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Update_listsClick(TObject *Sender)
{
   for(int i=0;i<20;i++)
   {
    int ind=playbacks_[i]->ItemIndex;
    playbacks_[i]->Update();
    Sleep(50);
    playbacks_[i]->ItemIndex=ind;
   }
}
//---------------------------------------------------------------------------

void TForm1::paint_test_keys()
{
   //static int del=0;
   //del++;
   //if(del<5)return;
  // else del=0;
   int line_index;

   for(int i=0; i<20; i++)
   {

	 int prt=ports[i*4]->Value;
	 if(prt==0)port=port0_data;
	 else if(prt==1)port=port1_data;
	 else port=port2_data;
     if(port[(int)ports[i*4+1]->Value])test_key[i]->Brush->Color=clYellow;
	 else test_key[i]->Brush->Color=clRed;
   }

}

void TForm1::update_ages()
{
  Age1->Caption=(int)Now()-(int)hatch1->Date;
  Age2->Caption=(int)Now()-(int)hatch2->Date;
  Age3->Caption=(int)Now()-(int)hatch3->Date;
  Age4->Caption=(int)Now()-(int)hatch4->Date;
  Age5->Caption=(int)Now()-(int)hatch5->Date;
  Age6->Caption=(int)Now()-(int)hatch6->Date;
  Age7->Caption=(int)Now()-(int)hatch7->Date;
  Age8->Caption=(int)Now()-(int)hatch8->Date;
  Age9->Caption=(int)Now()-(int)hatch9->Date;
  Age10->Caption=(int)Now()-(int)hatch10->Date;

}



void __fastcall TForm1::Box1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y)
{
  int i=0;
  if(Sender==Box1)i=0;
  else if(Sender==Box2)i=1;
  else if(Sender==Box3)i=2;
  else if(Sender==Box4)i=3;
  else if(Sender==Box5)i=4;
  else if(Sender==Box6)i=5;
  else if(Sender==Box7)i=6;
  else if(Sender==Box8)i=7;
  else if(Sender==Box9)i=8;
  else if(Sender==Box10)i=9;

  str="The color of the box represents the state of training sessions:\r";
  str+="Black - no training session is currently active \r";
  str+="Teal - at least one training session is currently active \r";
  str+="MoneyGreen - playback event (during gap)\r";
  str+="Olive - playback quota for this playback is finished \r";
  str+="Red - if key - pecking event, if the box is red -- playback error \r";

  str+="\r\rSession 1: ";
  if(Session[i*2].operant==0){str+="passive playbacks, odds per sec="; str2=Session[i*2].odds;str+=str2.SubString(1,5); }
  else if(Session[i*2].operant==1)str+="operant, key 1";
  else if(Session[i*2].operant==2)str+="operant, key 2";
  else if(Session[i*2].operant==3)str+="operant, both keys";
  str+=" \r Session Enabled="; if(Session[i*2].enabled) str+="true"; else str+="false";
  str+= "\r Currently active="; if(Session[i*2].state) str+="true"; else str+="false";
  str+="\r Active from "; str+=Session[i*2].start_hour; str+=":"; str+= Session[i*2].start_min;
  str+=" To "; str+=Session[i*2].end_hour; str+=":"; str+= Session[i*2].end_min;
  str+=" Random onset="; if(Session[i*2].random_onset)str+="true"; else str+="false";
  str+=";\r Quota enabled=";
  if(Session[i*2].enable_quota)
   { str+="true"; str+=", ";str+=(int)Session[i*2].quota; }
    else str+="false";
  str+="\r Song played="; str+=Session[i*2].song_played;
  str+="\r Minimum gap between playbacks="; str2=Session[i*2].gap; str+=str2.SubString(1,4);str+=" sec";

  str+="\r\r\Session 2: ";
  if(Session[i*2+1].operant==0){str+="passive playbacks, odds per sec="; str2=Session[i*2+1].odds;str+=str2.SubString(1,5); }
  else if(Session[i*2+1].operant==1)str+="operant, key 1";
  else if(Session[i*2+1].operant==2)str+="operant, key 2";
  else if(Session[i*2+1].operant==3)str+="operant, both keys";
  str+=" \r Session Enabled="; if(Session[i*2+1].enabled) str+="true"; else str+="false";
  str+= "\r Currently active="; if(Session[i*2+1].state) str+="true"; else str+="false";
  str+="\r Active from "; str+=Session[i*2+1].start_hour; str+=":"; str+= Session[i*2+1].start_min;
  str+=" To "; str+=Session[i*2+1].end_hour; str+=":"; str+= Session[i*2+1].end_min;
  str+=" Random onset="; if(Session[i*2+1].random_onset)str+="true"; else str+="false";
  str+=";\r Quota enabled=";
  if(Session[i*2+1].enable_quota)
   { str+="true"; str+=", ";str+=(int)Session[i*2+1].quota; }
    else str+="false";
  str+="\r Song played="; str+=Session[i*2+1].song_played;
  str+="\r Minimum gap between playbacks="; str2=Session[i*2+1].gap; str+=str2.SubString(1,4);str+=" sec";


  MessageDlgPos(str, mtInformation, TMsgDlgButtons() << mbOK, 0, 20, 300);



}
//---------------------------------------------------------------------------

void __fastcall TForm1::playing1Click(TObject *Sender)
{
   str="A passive playbacks session is enabled";
   MessageDlgPos(str, mtInformation, TMsgDlgButtons() << mbOK, 0, 20, 300);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::key_trackClick(TObject *Sender)
{
   key_track1->Checked=key_track->Checked;
}
//---------------------------------------------------------------------------



void __fastcall TForm1::LinkLabel1LinkClick(TObject *Sender, const UnicodeString Link,
          TSysLinkType LinkType)
{
AnsiString link=Link;
  ShellExecute(0, "open", link.c_str(), 0, 0, 1);
}
//---------------------------------------------------------------------------

