//----------------------------------------------------------------------------
#ifndef table_dialogH
#define table_dialogH
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
class TtableDialog : public TForm
{
__published:        
	TButton *OKBtn;
	TButton *CancelBtn;
	TBevel *Bevel1;
        TListBox *ListBox1;
        TMemo *Memo1;
        void __fastcall ListBox1Click(TObject *Sender);
        void __fastcall ListBox1DblClick(TObject *Sender);
private:
public:
	virtual __fastcall TtableDialog(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TtableDialog *tableDialog;
//----------------------------------------------------------------------------
#endif    
