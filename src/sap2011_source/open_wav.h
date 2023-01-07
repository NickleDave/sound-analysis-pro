//---------------------------------------------------------------------------

#ifndef open_wavH
#define open_wavH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <FileCtrl.hpp>
#include <ExtCtrls.hpp>
#include "CZT_FeatureCalc.h"
#include "CZT_PcmEq.h"
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class Topenwav : public TForm
{
__published:	// IDE-managed Components
	TDriveComboBox *DriveComboBox1;
	TDirectoryListBox *DirectoryListBox1;
	TFileListBox *sound_list;
	TButton *OKBtn;
	TButton *CancelBtn;
	TImage *Image1;
	TStaticText *invalid;
	TTrackBar *gain;
	void __fastcall sound_listClick(TObject *Sender);
	void __fastcall OKBtnClick(TObject *Sender);
	void __fastcall CancelBtnClick(TObject *Sender);
private:	// User declarations
	CWavIterator wave;
	CZT_FeatureCalc m_Calculator1;
public:		// User declarations
	__fastcall Topenwav(TComponent* Owner);
	AnsiString sound_to_open;
	bool done;
};
//---------------------------------------------------------------------------
extern PACKAGE Topenwav *openwav;
//---------------------------------------------------------------------------
#endif
