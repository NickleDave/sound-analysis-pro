//---------------------------------------------------------------------------

#ifndef startH
#define startH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include "segmentation.h"
#include "DBAccess.hpp"
#include "MyAccess.hpp"
#include "MyDacVcl.hpp"
#include <pngimage.hpp>
#include <fstream>
#pragma link "C:\\Program Files\\Embarcadero\\RAD Studio\\7.0\\lib\\PSDK\\oleacc.lib"
	//added by Alexei to compensate the bug of the first 2010 release


//---------------------------------------------------------------------------
class TStartForm : public TForm
{
__published:	// IDE-managed Components
        TBitBtn *single;
        TBitBtn *batch;
        TBitBtn *live;
        TBitBtn *dvd;
        TBitBtn *clusters;
        TBitBtn *options;
    TBitBtn *data_management;
        TStaticText *StaticText2;
        TImage *Image1;
        TImage *Image2;
        TBitBtn *help5;
        TBitBtn *BitBtn2;
        TBitBtn *BitBtn3;
        TBitBtn *BitBtn4;
        TBitBtn *BitBtn5;
        TBitBtn *BitBtn6;
    TBitBtn *similarity_batch;
    TBitBtn *Exit;
    TBitBtn *move_away;
        TBitBtn *brain;
        TBitBtn *BitBtn11;
        TStaticText *please_wait;
        TBitBtn *new_settings;
        TBitBtn *change_settings;
        TStaticText *StaticText5;
        TStaticText *settings_table;
        TBitBtn *BitBtn10;
	TLinkLabel *LinkLabel1;
    TLabel *Label1;
	TLinkLabel *LinkLabel2;
	TLinkLabel *LinkLabel3;
        void __fastcall liveClick(TObject *Sender);
        void __fastcall batchClick(TObject *Sender);
        void __fastcall optionsClick(TObject *Sender);
        void __fastcall clustersClick(TObject *Sender);
        void __fastcall data_managementClick(TObject *Sender);
        void __fastcall singleClick(TObject *Sender);
        void __fastcall dvdClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall help5Click(TObject *Sender);
        void __fastcall BitBtn2Click(TObject *Sender);
        void __fastcall BitBtn3Click(TObject *Sender);
        void __fastcall BitBtn4Click(TObject *Sender);
        void __fastcall BitBtn5Click(TObject *Sender);
        void __fastcall BitBtn6Click(TObject *Sender);
        void __fastcall similarity_batchClick(TObject *Sender);
        void __fastcall move_awayClick(TObject *Sender);
        void __fastcall brainClick(TObject *Sender);
        void __fastcall new_settingsClick(TObject *Sender);
        void __fastcall change_settingsClick(TObject *Sender);
        void __fastcall BitBtn10Click(TObject *Sender);
	void __fastcall LinkLabel1LinkClick(TObject *Sender, const UnicodeString Link, TSysLinkType LinkType);
    void __fastcall ExitClick(TObject *Sender);

private: 	// User declarations

float *TapersOut;
float* pTape1;
float* pTape2;
ofstream myfile;
AnsiString str;
public:		// User declarations
        __fastcall TStartForm(TComponent* Owner);


        void ReadTapers();
};
//---------------------------------------------------------------------------
extern PACKAGE TStartForm *StartForm;
//---------------------------------------------------------------------------
#endif
