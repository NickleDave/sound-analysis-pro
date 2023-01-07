//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "preview.h"
#include "open_soundD.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TpreviewSound *previewSound;
//---------------------------------------------------------------------------
__fastcall TpreviewSound::TpreviewSound(TComponent* Owner)
	: TForm(Owner)
{

  wave.OpenFile(m_Calculator1.newFile.c_str());
  /*//if(!wave.OpenFile("little_chirp.wav")==pcmpos_error)
   //{


 //OKBtn->Enabled=true;
 sampling_rate=wave.m_WaveHeader.nSamplesPerSec;
		  int i=wave.GetTotalSamples();
		  //int dur=i/sampling_rate;
		   m_Calculator1.newFile="little_chirp.wav"; //sound_list->FileName;
		  //duration->Caption=dur;
		  //sampling->Caption=sampling_rate;
		  option->sampling_rate=sampling_rate;
		  //frequency_rangeChange(this);
		  //data_windowChange(this);
		 // advanceChange(this);

		  //invalid->Visible=false;


		  try{
		  m_Calculator1.init_viewer(wave);
		  }
		  catch(...)
		  {Application->MessageBox(L"Error: could not innitiate", NULL, MB_OK);}

		  try{  m_Calculator1.viewer(wave, 400, 1);}
		  catch(...)
		  {Application->MessageBox(L"Error: could not run viewer", NULL, MB_OK);}



		  //FileName =
		  m_Calculator1.GetImage().DrawViewerSlices(Image1);
		  m_Calculator1.GetImage().DrawViewerSlices(Image1);  */
		  //}

}
void __fastcall TpreviewSound::Button1Click(TObject *Sender)
{
	 wave.ResetContents();
  // m_Calculator1.newFile=FileName;
   //m_Calculator1.RenderImage(1);
   //m_Calculator1.render_image=true;
   //wave.OpenFile(FileName.c_str());
   //Button1->Visible=true;
   Image1->Picture=open_sound->Image1->Picture;
   /*	try{
		  m_Calculator1.init_viewer(wave);
		  }
		  catch(...)
		  {Application->MessageBox(L"Error: could not initiate", NULL, MB_OK);}

		  try{  m_Calculator1.viewer(wave, 400, 1);}
		  catch(...)
		  {Application->MessageBox(L"Error: could not run viewer", NULL, MB_OK);}

		  m_Calculator1.GetImage().DrawViewerSlices(Image1);
		  //m_Calculator1.GetImage().DrawViewerSlices(Image1);      */

		  Image1->Canvas->Pen->Width = 1;
	Image1->Canvas->Pen->Color = clBlue;
	int x1 = 1;
	int x2 = 832;
	if (option->paginationOn) {
			 x1 =  (option->pageNumber-1)*1000/option->Advance;
			 x2 =  option->pageNumber*1000/option->Advance;
		}
	Image1->Canvas->MoveTo(x1, 0);
	Image1->Canvas->LineTo(x1, 125);
	Image1->Canvas->MoveTo(x2, 0);
	Image1->Canvas->LineTo(x2, 125);
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

void __fastcall TpreviewSound::Image1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
  // use for
}
//---------------------------------------------------------------------------

