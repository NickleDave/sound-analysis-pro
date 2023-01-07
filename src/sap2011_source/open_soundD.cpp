//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include "records.h"
#include "open_soundD.h"
//---------------------------------------------------------------------
#pragma link "SDL_NumLab"
#pragma link "SDL_fourier"
#pragma resource "*.dfm"
Topen_sound *open_sound;
//--------------------------------------------------------------------- 
__fastcall Topen_sound::Topen_sound(TComponent* AOwner)
	: TOKBottomDlg(AOwner)
{
  sampling_rate=44100;
  if (option->openFromSound1) {
	  option->paginationOn = true;
  }  else {
	option->paginationOn2 = true;
  }
}
//---------------------------------------------------------------------
void __fastcall Topen_sound::HelpBtnClick(TObject *Sender)
{
	Application->HelpContext(HelpContext);
}
//---------------------------------------------------------------------
void __fastcall Topen_sound::sound_listClick(TObject *Sender)
{
  OKBtn->Enabled=true;
  //file_edit->Text=sound_list->FileName;
 // wave.ResetContents();
  m_Calculator1.brightness=gain->Position*0.000001;
   //active_view=true;
   //GUI=true;
   m_Calculator1.newFile=sound_list->FileName;
  // m_soundPreview=sound_list->FileName;
   CZT_PcmEq equalizer;
   //wave.UseFilter(equalizer);
   m_Calculator1.RenderImage(1);
   m_Calculator1.render_image=true;
   AnsiString str=sound_list->FileName;
   try{
	 file_comments->Lines->Text = DataForm->Retrieve_comments(ExtractFileName(str));
   }catch(Exception & exception) {file_comments->Lines->Text ="";}

   if(wave.OpenFile(str.c_str())==pcmpos_error)
   {
	  invalid->Visible=true;
	  OKBtn->Enabled=false;
   }
   //Application->MessageBox(L"Error: could not open wave file", NULL, MB_OK);// != pcmpos_error)
   else
   {

		  OKBtn->Enabled=true;
		  sampling_rate=wave.m_WaveHeader.nSamplesPerSec;
		  int i=wave.GetTotalSamples();
		  int dur=i/sampling_rate;
		  //if(dur>50)too_long->Visible=true;
		  //else too_long->Visible=false;

			 if (option->openFromSound1) {
				option->paginationOn = true;
			 }  else {
				option->paginationOn2 = true;
			 }


		 if(dur< paginate_val->Value) {
			//if(dur>-1) {
			if(dur<1)pagination->Controls[1]->Enabled=false;
			pagination->ItemIndex=0;
		  } else {
			 pagination->Controls[1]->Enabled=true;
			 if(dur>paginate_val->Value)pagination->ItemIndex=1;
			 else pagination->ItemIndex=0;//DataForm->Retrieve(1, "pagination").ToInt(); //1;
		 }
		  duration->Caption=dur;
		  sampling->Caption=sampling_rate;
		  option->sampling_rate=sampling_rate;
		  frequency_rangeChange(this);
		  data_windowChange(this);
		  advanceChange(this);

		  invalid->Visible=false;


		   m_Calculator1.init_viewer(wave);
		  }
		  //catch(...)   {Application->MessageBox(L"Error: could not innitiate", NULL, MB_OK);}
		 /*try{
		  try{  m_Calculator1.viewer(wave, 400, 1);}
		  catch(...)
		  {Application->MessageBox(L"Error: could not run viewer", NULL, MB_OK);}
		  */



		  m_Calculator1.GetImage().DrawViewerSlices(Image1);
		  //m_Calculator1.GetImage().DrawViewerSlices(Image1);

		  if(playbacks->Checked)
		  {
			wave.ResetContents();
		  //	FastFourier1->Clear();
			try{play->FileName =sound_list->FileName;
			play->Open();  }catch(Exception & exception) { playbacks->Checked=false;}
		  }
   //}
}
//---------------------------------------------------------------------------

void __fastcall Topen_sound::OKBtnClick(TObject *Sender)
{
  paginationClick(this);
  play->Close();
  wave.ResetContents();
  if (option->openFromSound1) {
	  FileName=sound_list->FileName;
  }  else {
	FileName2=sound_list->FileName;
  }

}
//---------------------------------------------------------------------------

void __fastcall Topen_sound::CancelBtnClick(TObject *Sender)
{
  play->Close();
  wave.ResetContents();
}
//---------------------------------------------------------------------------

void __fastcall Topen_sound::search_fileClick(TObject *Sender)
{
  if(open->Execute())
  {
	AnsiString str=open->FileName;
	DirectoryListBox1->Directory=ExtractFileDir(str);
	sound_list->FileName=open->FileName;
	sound_listClick(this);
  }
}
//---------------------------------------------------------------------------

void __fastcall Topen_sound::warningClick(TObject *Sender)
{
   if(warning->Checked)option->warning=true;
   else option->warning=false;
}
//---------------------------------------------------------------------------

void __fastcall Topen_sound::file_editChange(TObject *Sender)
{
     sound_list->ApplyFilePath(file_edit->Text);
}
//---------------------------------------------------------------------------



void __fastcall Topen_sound::sound_listDblClick(TObject *Sender)
{
  sound_listClick(this);
  OKBtnClick(this);
  open_sound->ModalResult=mrOk;
}
//---------------------------------------------------------------------------


void __fastcall Topen_sound::FormActivate(TObject *Sender)
{
  OKBtn->Enabled=false;
  int i=sound_list->ItemIndex;
  try{sound_list->Update();
  }catch(Exception & exception) {sound_list->Directory="c:\\sap";}
 sound_list->ItemIndex=i;
  //file_edit->Text=FileName;
  /*
  for (int i = 0; i < sound_list->Items->Count; i++)
  {
   //if (FileListBox1->Selected[i])
  } */
}
//---------------------------------------------------------------------------


void __fastcall Topen_sound::frequency_rangeChange(TObject *Sender)
{
  int i=pow(2,4-frequency_range->Position);
  frequency_range_val->Value=sampling_rate/i;
  Form1->frequency_range->Position=frequency_range->Position;
  Form1->frequency_range2->Position=frequency_range->Position;
  option->frequency_range->Position=frequency_range->Position;
  option->frequency_rangeChange(this);
  switch(frequency_range->Position)
  {
	case 1:  // show quater of the range of FFT
	//frequency_range_val->Value=5000;
	option->range_x2=false;
	option->range_x05=true;
	break;

	case 2: // show half of the range of FFT
	//frequency_range_val->Value=10000;
	option->range_x2=false;
	option->range_x05=false;
	break;

	case 3: // show all the range of FFT (well, almost)
	//frequency_range_val->Value=20000;
	option->range_x2=true;
	option->range_x05=false;
	break;
  }
  //Form1->frequency_rangeChange(this);
}
//---------------------------------------------------------------------------

void __fastcall Topen_sound::data_windowChange(TObject *Sender)
{
  windowTB->Value=1000.0*(float)data_window->Position/(float)sampling_rate;
  Form1->data_window->Position=data_window->Position;
  Form1->data_window2->Position=data_window->Position;
  option->data_window->Position=data_window->Position;
  option->data_windowChange(this);
  //Form1->data_windowChange(this);
}
//---------------------------------------------------------------------------

void __fastcall Topen_sound::advanceChange(TObject *Sender)
{
  advanceTb->Value=1000.0*(float)advance->Position/(float)sampling_rate;
  Form1->advance->Position=advance->Position;
  Form1->advance2->Position=advance->Position;
  option->advance->Position=advance->Position;
  option->advanceChange(this);
}
//---------------------------------------------------------------------------



void __fastcall Topen_sound::paginationClick(TObject *Sender)
{
	TImage *image;
	TRadioGroup *page;
	bool *pagination_On;
    image = Image1;
	page = pagination;

	if (option->openFromSound1) {
		pagination_On=&option->paginationOn;
	}  else {
		pagination_On=&option->paginationOn2;
	}
	if (!page->ItemIndex) {
		*pagination_On = false;
	} else {
		*pagination_On = true;
	}
}
//---------------------------------------------------------------------------





void __fastcall Topen_sound::save_commentsClick(TObject *Sender)
{
  AnsiString file_name=ExtractFileName(sound_list->FileName);
  AnsiString bird_name;
  int size=file_name.Length();
  int i=1;
  do{
     bird_name+=file_name[i];
     i++;
     }while(i<size && i<8 && file_name[i]!='_' && file_name[i]!='.');
  DataForm->insert_comment(bird_name, file_name,(AnsiString)file_comments->Lines->Text);
}
//---------------------------------------------------------------------------



void __fastcall Topen_sound::paginate_tbChange(TObject *Sender)
{
  paginate_val->Value=paginate_tb->Position;
  DataForm->Settings(1,"pagination_thresh", paginate_tb->Position);
}
//---------------------------------------------------------------------------

