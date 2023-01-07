//----------------------------------------------------------------------------
#ifndef table_dialogH
#define table_dialogH
//----------------------------------------------------------------------------
#include <vcl\System.hpp>
#include <vcl\Windows.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\ExtCtrls.hpp>
//----------------------------------------------------------------------------
class TtableDialog : public TForm
{
__published:        
	TButton *OKBtn;
	TButton *CancelBtn;
	TBevel *Bevel1;
        TListBox *tables;
        TListBox *databases;
        TBevel *Bevel2;
        TLabel *Label1;
        TLabel *Label2;
        TBitBtn *Delete_tbl;
        TBitBtn *new_db;
        TBitBtn *new_tbl;
        TBitBtn *delete_db;
    TLabel *count_recs;
    TStaticText *StaticText45;
    TEdit *nameLb;
    TStaticText *StaticText1;
    TRadioGroup *show_type;
        void __fastcall tablesClick(TObject *Sender);
        void __fastcall tablesDblClick(TObject *Sender);
        void __fastcall databasesClick(TObject *Sender);
        void __fastcall Delete_tblClick(TObject *Sender);
        void __fastcall tablesKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall new_tblClick(TObject *Sender);
        void __fastcall delete_dbClick(TObject *Sender);
        void __fastcall new_dbClick(TObject *Sender);
    void __fastcall show_typeClick(TObject *Sender);
private:   void show_databases();
public:
	virtual __fastcall TtableDialog(TComponent* AOwner);
       
};
//----------------------------------------------------------------------------
extern PACKAGE TtableDialog *tableDialog;
//----------------------------------------------------------------------------
#endif    
