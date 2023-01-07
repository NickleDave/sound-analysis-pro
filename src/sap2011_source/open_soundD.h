//----------------------------------------------------------------------------
#ifndef open_soundDH
#define open_soundDH
//----------------------------------------------------------------------------
#include <OKCANCL1.h>
#include <vcl\ExtCtrls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\Classes.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Windows.hpp>
#include <vcl\System.hpp>
#include <ComCtrls.hpp>
#include <FileCtrl.hpp>
#include "CZT_FeatureCalc.h"
#include "CZT_PcmEq.h"
#include <Dialogs.hpp>
#include "options3.h"
#include <MPlayer.hpp>
#include "SongMeasures.h"
#include "SDL_NumLab.hpp"
#include "SDL_fourier.hpp"
#include "SDL_rchart.hpp"
#include "CZT_FeatureImage.h"
#include "preview.h"
//----------------------------------------------------------------------------
class Topen_sound : public TOKBottomDlg
{
__published:
	TButton *HelpBtn;
	TDriveComboBox *DriveComboBox1;
	TDirectoryListBox *DirectoryListBox1;
	TFileListBox *sound_list;
	TImage *Image1;
	TStaticText *invalid;
	TTrackBar *gain;
	TLabel *Label1;
	TLabel *duration;
	TFileOpenDialog *open;
	TBitBtn *search_file;
	TStaticText *too_long;
	TCheckBox *warning;
	TMediaPlayer *play;
	TCheckBox *playbacks;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *sampling;
    TTrackBar *frequency_range;
    TStaticText *StaticText144;
    TNumLab *frequency_range_val;
    TStaticText *StaticText126;
    TStaticText *StaticText158;
    TTrackBar *data_window;
    TNumLab *windowTB;
    TStaticText *StaticText157;
    TStaticText *StaticText106;
    TTrackBar *advance;
    TNumLab *advanceTb;
    TStaticText *StaticText1;
	TRadioGroup *pagination;
    TMemo *file_comments;
    TStaticText *StaticText226;
    TBitBtn *save_comments;
	TEdit *file_edit;
	TTrackBar *paginate_tb;
	TStaticText *StaticText2;
	TNumLab *paginate_val;
	TStaticText *StaticText3;
	void __fastcall HelpBtnClick(TObject *Sender);
	void __fastcall sound_listClick(TObject *Sender);
	void __fastcall OKBtnClick(TObject *Sender);
	void __fastcall CancelBtnClick(TObject *Sender);
	void __fastcall search_fileClick(TObject *Sender);
	void __fastcall warningClick(TObject *Sender);
	void __fastcall file_editChange(TObject *Sender);
    void __fastcall sound_listDblClick(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall frequency_rangeChange(TObject *Sender);
    void __fastcall data_windowChange(TObject *Sender);
    void __fastcall advanceChange(TObject *Sender);
	void __fastcall paginationClick(TObject *Sender);
    void __fastcall save_commentsClick(TObject *Sender);
	void __fastcall paginate_tbChange(TObject *Sender);
private:
	CWavIterator wave;
	CZT_FeatureCalc m_Calculator1;
	CZT_FeatureImage  m_Image;

public:
	virtual __fastcall Topen_sound(TComponent* AOwner);
	AnsiString FileName, FileName2;
    bool done;
	int sampling_rate;
};
//----------------------------------------------------------------------------
extern PACKAGE Topen_sound *open_sound;
//----------------------------------------------------------------------------
#endif    
