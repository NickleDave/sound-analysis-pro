//---------------------------------------------------------------------
#include "vcl.h"
#pragma hdrstop

#include "table_dialog.h"
#include "david.h"
//---------------------------------------------------------------------
#pragma resource "*.dfm"
TtableDialog *tableDialog;
//---------------------------------------------------------------------
__fastcall TtableDialog::TtableDialog(TComponent* AOwner)
	: TForm(AOwner)
{
    ListBox1->Clear();
    for (int i=0; i < Form1->MyQuery1->RecordCount; i++)
    {
     ListBox1->Items->Add( Form1->MyQuery1->Fields->Fields[0]->AsString);
     Form1->MyQuery1->Next();
    }

}
//---------------------------------------------------------------------
void __fastcall TtableDialog::ListBox1Click(TObject *Sender)
{
   OKBtn->Enabled=true;
   
}
//---------------------------------------------------------------------------


void __fastcall TtableDialog::ListBox1DblClick(TObject *Sender)
{
  tableDialog->ModalResult=1;
}
//---------------------------------------------------------------------------
  
