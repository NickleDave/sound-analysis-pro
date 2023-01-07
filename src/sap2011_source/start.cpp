//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "start.h"
#include "live.h"
#include "options3.h"
#include "cluster.h"
#include "batchIt.h"
#include "records.h"
#include "SongMeasures.h"
#include "DVD.h"
#include "shellapi.h"
#include "sound_live.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DBAccess"
#pragma link "MyAccess"
#pragma link "MyDacVcl"
#pragma resource "*.dfm"
#include "tapers.h"

TStartForm *StartForm;
//---------------------------------------------------------------------------
__fastcall TStartForm::TStartForm(TComponent* Owner)
        : TForm(Owner)
{
	int i=0;
}
//---------------------------------------------------------------------------
void __fastcall TStartForm::liveClick(TObject *Sender)
{
  please_wait->Visible=true;
  please_wait->Repaint();
  Application->CreateForm(__classid(Tsound_live), &sound_live);
  sound_live->Show();   // show a splash screen contain ProgressBar control
  please_wait->Visible=false;
  StartForm->Visible=false;

  if(FileExists("C:\\Program Files (x86)\\SAP2011\\Recorder2011.exe"))
	 ShellExecute(Application->Handle, "open", "C:\\Program Files (x86)\\SAP2011\\Recorder2011.exe", NULL, NULL, SW_SHOWNORMAL);
	 else if(FileExists("C:\\Program Files\\SAP2011\\Recorder2011.exe"))
		  ShellExecute(Application->Handle, "open", "C:\\Program Files\\SAP2011\\Recorder2011.exe", NULL, NULL, SW_SHOWNORMAL);
	 else ShellExecute(Application->Handle, "open", "Recorder2011.exe", NULL, NULL, SW_SHOWNORMAL);
 /* */
}


//---------------------------------------------------------------------------
void __fastcall TStartForm::batchClick(TObject *Sender)
{
  please_wait->Visible=true;
  please_wait->Repaint();
  Application->CreateForm(__classid(Tbatch), &batch);
  //batch->Visible=true;
  batch->Show();   // show a splash screen contain ProgressBar control
  batch->Update();
  please_wait->Visible=false;
  StartForm->Visible=false;
}

//---------------------------------------------------------------------------
void __fastcall TStartForm::optionsClick(TObject *Sender)
{
  option->Visible=true;
  StartForm->Visible=false;
  option->show_start=true;
}
//---------------------------------------------------------------------------

void __fastcall TStartForm::clustersClick(TObject *Sender)
{
  please_wait->Visible=true;
  please_wait->Repaint();
  Application->CreateForm(__classid(TclusterIt), &clusterIt);
  clusterIt->Show();   // show a splash screen contain ProgressBar control
  clusterIt->Update();
  please_wait->Visible=false;
  StartForm->Visible=false;
}
//---------------------------------------------------------------------------


void __fastcall TStartForm::data_managementClick(TObject *Sender)
{
  DataForm->Visible=true;
  StartForm->Visible=false;
  DataForm->show_start=true;
}
//---------------------------------------------------------------------------

void TStartForm::ReadTapers()
{
     int samples=option->ztSamplesPerWindow;
	 if(option->tapers_done) CZT_MultiTaper::Instance().ReleaseTapers();
     CZT_MultiTaper::Instance().Init(samples);
	 pTape1 =CZT_MultiTaper::Instance().m_pTaper1;
	 pTape2 =CZT_MultiTaper::Instance().m_pTaper2;

	 TapersOut=new float[100+samples*4];
	 proTapers(samples, 4, 1.5, TapersOut);

	 if(TapersOut[2]<0) for(int i=0; i<samples; i++) TapersOut[i]*=-1;
	 if(TapersOut[samples+2]<0) for(int i=0; i<samples; i++) TapersOut[samples+i]*=-1;
	 if(TapersOut[2*samples+2]<0) for(int i=0; i<samples; i++) TapersOut[2*samples+i]*=-1;
	 if(TapersOut[3*samples+2]<0) for(int i=0; i<samples; i++) TapersOut[3*samples+i]*=-1;
	 for(int i=0;i<samples;i++)
	 {
	   pTape1[i]=TapersOut[i];
	   pTape2[i]=TapersOut[i+samples];
	 }
}


void __fastcall TStartForm::singleClick(TObject *Sender)
{
  please_wait->Visible=true;
  please_wait->Repaint();
  Application->CreateForm(__classid(TForm1), &Form1);
  Form1->Visible=true;
  Form1->Show();
  Form1->Update();
  please_wait->Visible=false;
  StartForm->Visible=false;

  
}
//---------------------------------------------------------------------------

void __fastcall TStartForm::dvdClick(TObject *Sender)
{
  Application->CreateForm(__classid(TDVD_map), &DVD_map);
  DVD_map->Show();   // show a splash screen contain ProgressBar control
  DVD_map->Update();
  StartForm->Visible=false;
}
//---------------------------------------------------------------------------

void __fastcall TStartForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  delete [] pTape1;
  delete [] pTape2;
  delete[] TapersOut;
  DataForm->MyConnection1->Connected=false;
  delete option;
  delete DataForm;
  delete Form1;
  //delete tableDialog;
  //delete animals_form;

}
//---------------------------------------------------------------------------

void __fastcall TStartForm::help5Click(TObject *Sender)
{
 str="Single Task: \r";
 str+=" Use this module to examine features of sound files and  \r";
 str+=" to perform similarity measurment in one or in a few files \r \r";
 str+="* Presents spectral derivative images \r";
 str+="* Allow an easy comparison and alignment of sound files \r";
 str+="* Perform fully automated similarity measurments \r";
 str+="* Presents 3D feature space \r";
  str+="* Export your data to Excel \r";
 MessageDlgPos(str, mtInformation, TMsgDlgButtons() << mbOK, 0, 10,162);

}
//---------------------------------------------------------------------------

void __fastcall TStartForm::BitBtn2Click(TObject *Sender)
{
 str="Batch module: \r";
 str+=" Examine features in a large quantity of sound data, or perform multiple similarity measurments \r";
 str+=" It automatically partition sound to syllables and save their features in tables. \r \r";
 str+="* Table of syllable features can be then shown as a movie (DVD-map), or clustered to types \r";
 str+="* Automated similarity measuremnts can be done across matrixes of sounds \r";
 str+="* You can analyse sound via the network \r";
 MessageDlgPos(str, mtInformation, TMsgDlgButtons() << mbOK, 0, 80,162);

}
//---------------------------------------------------------------------------

void __fastcall TStartForm::BitBtn3Click(TObject *Sender)
{
 str="Sound Analysis Live: \r";
 str+=" This module works in conjunction with the recorder application to analyze sound in nearly real-time \r";
 str+=" It automatically recognize animal sounds, save and partition them. Features are saved to database. \r \r";
 str+="* Automated multitaper analysis and feature extraction around the clock \r";
 str+="* Up to 4 channels can be recorded and analyzed simultanuously and continuously \r";
 str+="* Each table can contain millions of sounds (e.g., an entire ontogeny) in a single, moderate size file \r";
 str+="* Table of syllable features are automatically created and displayed as a movie (DVD-map)  \r";
 str+="* Raw feature curves are saved to binary files, to allow revision of segmentation procedure \r";
 str+="* User can set specific analysis prefferneces to each channel, those are automatically saved \r";
 MessageDlgPos(str, mtInformation, TMsgDlgButtons() << mbOK, 0, 140,162);

}
//---------------------------------------------------------------------------

void __fastcall TStartForm::BitBtn4Click(TObject *Sender)
{
   str="DVD maps: \r";
 str+=" Presents movies of the distribution of syllable features as it evolve during an experiment \r";
 str+=" Movies can emphasis the formation of types (clusters), syntax, ortime (circadian or linear) \r \r";
 str+="* Color of dots can show time of day, age, syllable identity (after clustering) and more \r";
 str+="* On line contorl of features to present, speed of movie, interval size, formats and colors \r";
 str+="* Combined sintax-phonetic representation of streaming from one syllable type to the other \r";
 str+="* Syntax and rythms are also presented as percusion sounds with flow images \r";
 MessageDlgPos(str, mtInformation, TMsgDlgButtons() << mbOK, 0, 220,162);
}
//---------------------------------------------------------------------------


void __fastcall TStartForm::BitBtn5Click(TObject *Sender)
{
   str="Cluster Analysis of syllable type: \r";
 str+=" This module uses syllable tables as an input to clasify the recorded sounds into types \r";
 str+=" It uses a procedure called 'nearest neighboor hyrarchial clustering' to identify the types. \r \r";
 str+="* Clusters are calculated across selected features and are displayed with different colors \r";
 str+="* User can select a cluster and trace it back. Tracking show continuous visual feedback \r";
 str+="* Clusters are automatically registered to database and shown as movies, or filtered in or out \r";
 str+="* User can set constraints on clustering procedures. Preferences are automatically saved. \r";
 MessageDlgPos(str, mtInformation, TMsgDlgButtons() << mbOK, 0, 300,162);
}
//---------------------------------------------------------------------------

void __fastcall TStartForm::BitBtn6Click(TObject *Sender)
{
    str="Data module: \r";
 str+=" This module can be used to brouse through your data \r";
 str+=" Exporting data to Matlab or Excel can be done here too \r";
 MessageDlgPos(str, mtInformation, TMsgDlgButtons() << mbOK, 0, 380,162);

}
//---------------------------------------------------------------------------

void __fastcall TStartForm::similarity_batchClick(TObject *Sender)
{
  Application->CreateForm(__classid(TForm1), &Form1);
  StartForm->Visible=false;
  Form1->display->ActivePage=Form1->display->Pages[5];
  Form1->Visible=true;
  Form1->Show();
  Form1->Update();
}
//---------------------------------------------------------------------------


void __fastcall TStartForm::move_awayClick(TObject *Sender)
{
  please_wait->Visible=false;
  StartForm->SendToBack();
}
//---------------------------------------------------------------------------

void __fastcall TStartForm::brainClick(TObject *Sender)
{
  please_wait->Visible=true;
  please_wait->Repaint();
  ShellExecute(Application->Handle, "open", "c:\\Program Files\\Sound Analysis Pro\\SAP_recorder.exe", NULL, NULL, SW_SHOWNORMAL);
  Application->CreateForm(__classid(Tsal), &sal);
  sal->Show();   // show a splash screen contain ProgressBar control        
  please_wait->Visible=false;
  StartForm->Visible=false;
}
//---------------------------------------------------------------------------
                      
void __fastcall TStartForm::new_settingsClick(TObject *Sender)
{
  DataForm->new_settingsClick(this);
  settings_table->Caption=DataForm->settings_table_name;
}
//---------------------------------------------------------------------------

void __fastcall TStartForm::change_settingsClick(TObject *Sender)
{
  DataForm->open_settingsClick(this);
  settings_table->Caption=DataForm->settings_table_name;
}
//---------------------------------------------------------------------------



void __fastcall TStartForm::BitBtn10Click(TObject *Sender)
{
 str="SAP2 has many state parameters, e.g, the position of sliders, table names, etc. \r";
 str+=" When you exit SAP2, those states are saved in the settings table, so that next time you open \r";
 str+=" SAP2, it automatically return to the last state. If you would like to save different states, you need \r";
 str+="to create additional settings tables. To do so, click 'new settings' and choose the table name.\r";
 str+="To switch between settings, click 'change settins'. \r";
 MessageDlgPos(str, mtInformation, TMsgDlgButtons() << mbOK, 0, 140,162);

}
//---------------------------------------------------------------------------


void __fastcall TStartForm::LinkLabel1LinkClick(TObject *Sender, const UnicodeString Link,
          TSysLinkType LinkType)
{
  //  <a href="http://ofer.sci.ccny.cuny.edu/sound_analysis_pro">Go to Sound Analysis Pro site</a>
  AnsiString link=Link;
  ShellExecute(0, "open", link.c_str(), 0, 0, 1);
}
//---------------------------------------------------------------------------



void __fastcall TStartForm::ExitClick(TObject *Sender)
{
 Application->Terminate();
}
//---------------------------------------------------------------------------


