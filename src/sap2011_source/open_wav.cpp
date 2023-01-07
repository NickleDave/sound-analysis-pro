//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "open_wav.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Topenwav *openwav;
//---------------------------------------------------------------------------
__fastcall Topenwav::Topenwav(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall Topenwav::sound_listClick(TObject *Sender)
{
   wave.ResetContents();
   m_Calculator1.brightness=gain->Position*0.000001;
   //active_view=true;
   //GUI=true;
   m_Calculator1.newFile=sound_list->FileName;
   CZT_PcmEq equalizer;
   wave.UseFilter(equalizer);
   m_Calculator1.RenderImage(1);
   m_Calculator1.render_image=true;
   AnsiString str=sound_list->FileName;
   if(wave.OpenFile(str.c_str())==pcmpos_error)
   {
	invalid->Visible=true;
	OKBtn->Enabled=false;
   }
   //Application->MessageBox(L"Error: could not open wave file", NULL, MB_OK);// != pcmpos_error)
   else
   {
		  OKBtn->Enabled=true;
		  int i=wave.GetTotalSamples();
		  invalid->Visible=false;
		  m_Calculator1.init_viewer(wave);
		  m_Calculator1.viewer(wave, 400, 1);
		  m_Calculator1.GetImage().DrawViewerSlices(Image1);
		  m_Calculator1.GetImage().DrawViewerSlices(Image1);

   }
}
//---------------------------------------------------------------------------
void __fastcall Topenwav::OKBtnClick(TObject *Sender)
{
  wave.ResetContents();
  sound_to_open=sound_list->FileName;
  done=true;
  openwav->Visible=false;
}
//---------------------------------------------------------------------------
void __fastcall Topenwav::CancelBtnClick(TObject *Sender)
{
  wave.ResetContents();
  done=false;
  openwav->Visible=false;
}
//---------------------------------------------------------------------------
