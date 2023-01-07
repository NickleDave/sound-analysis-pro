//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "table_dialog.h"
#include "records.h"
//---------------------------------------------------------------------
#pragma resource "*.dfm"
TtableDialog *tableDialog;
//---------------------------------------------------------------------
__fastcall TtableDialog::TtableDialog(TComponent* AOwner)
	: TForm(AOwner)
{
    show_databases();
    DataForm->showTables();
    tables->Clear();
    for (int i=0; i < DataForm->MyQuery1->RecordCount; i++)
    {
     tables->Items->Add( DataForm->MyQuery1->Fields->Fields[0]->AsString);
     DataForm->MyQuery1->Next();
    }

}
//---------------------------------------------------------------------
void __fastcall TtableDialog::tablesClick(TObject *Sender)
{
   OKBtn->Enabled=true;
   AnsiString table_name=tables->Items->Strings[tableDialog->tables->ItemIndex];
   nameLb->Text=table_name;
   try{count_recs->Caption=DataForm->count_records(table_name);}
    catch(...)
   {
      Application->MessageBox(L"Table is corrupted", NULL, MB_OK);
      Delete_tblClick(this);
   }
}
//---------------------------------------------------------------------------


void __fastcall TtableDialog::tablesDblClick(TObject *Sender)
{
  tableDialog->ModalResult=1;
}
//---------------------------------------------------------------------------




void __fastcall TtableDialog::databasesClick(TObject *Sender)
{
   OKBtn->Enabled=true;
   DataForm->Change_Database(databases->Items->Strings[tableDialog->databases->ItemIndex]);
   DataForm->showTables();
    tables->Clear();
    for (int i=0; i < DataForm->MyQuery1->RecordCount; i++)
    {
     tables->Items->Add( DataForm->MyQuery1->Fields->Fields[0]->AsString);
     DataForm->MyQuery1->Next();
    }
    tables->ItemIndex=0;
}
//---------------------------------------------------------------------------


void __fastcall TtableDialog::Delete_tblClick(TObject *Sender)
{
  if (MessageDlg("Are you sure? This will permanently delete the table.", mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
  {
   AnsiString table_name=tables->Items->Strings[tableDialog->tables->ItemIndex];
   DataForm->DeleteTable(table_name);
   databasesClick(this);
  }
}


//---------------------------------------------------------------------------



void __fastcall TtableDialog::tablesKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if(Key==46 || Key==110) Delete_tblClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TtableDialog::new_tblClick(TObject *Sender)
{
   DataForm->new_tableClick(this);
   databasesClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TtableDialog::delete_dbClick(TObject *Sender)
{
    if (MessageDlg("Are you sure? This will permanently delete the database.", mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
  {
    AnsiString db_name=databases->Items->Strings[tableDialog->databases->ItemIndex];
    AnsiString str="DROP DATABASE ";
    str+=db_name;
    DataForm->MyCommand1->SQL->Clear();
    DataForm->MyCommand1->SQL->Add(str);
    DataForm->MyCommand1->Execute(1);
    show_databases();
  }
}
//---------------------------------------------------------------------------

void __fastcall TtableDialog::new_dbClick(TObject *Sender)
{
   AnsiString database_name = InputBox("New Database", "Database name must start with a letter, e.g., mydata:", "");
   AnsiString str="CREATE DATABASE ";
    str+=database_name;
    DataForm->MyCommand1->SQL->Clear();
    DataForm->MyCommand1->SQL->Add(str);
    DataForm->MyCommand1->Execute(1);
    show_databases();
}
//---------------------------------------------------------------------------

void TtableDialog::show_databases()
{
    DataForm->showDatabases();
    databases->Clear();
    int currentDB=0;
    for (int i=0; i < DataForm->MyQuery1->RecordCount; i++)
    {
     databases->Items->Add( DataForm->MyQuery1->Fields->Fields[0]->AsString);
	 AnsiString str=DataForm->MyQuery1->Fields->Fields[0]->AsString;
	 if(str.AnsiCompareIC(DataForm->MyConnection1->Database)==0)
	 //if(DataForm->MyQuery1->Fields->Fields[0]->AsString.AnsiCompareIC(DataForm->MyConnection1->Database)==0)
		 currentDB=i;
     DataForm->MyQuery1->Next();
    }
    databases->ItemIndex=currentDB;
    tables->ItemIndex=0;
}



void __fastcall TtableDialog::show_typeClick(TObject *Sender)
{
  AnsiString str;
  switch(show_type->ItemIndex)
  {
  case 0:
  tables->Clear();
  DataForm->MyQuery1->First();
  for (int i=0; i < DataForm->MyQuery1->RecordCount; i++)
  {
     tables->Items->Add( DataForm->MyQuery1->Fields->Fields[0]->AsString);
     DataForm->MyQuery1->Next();
  }
  break;

  case 1:
  tables->Clear();
  DataForm->MyQuery1->First();
  for (int i=0; i < DataForm->MyQuery1->RecordCount; i++)
  {
     str=DataForm->MyQuery1->Fields->Fields[0]->AsString;
     if(str[1]=='s' && str[2]=='y' && str[3]=='l') tables->Items->Add(str);
     DataForm->MyQuery1->Next();
  }
  break;

  case 2:
  tables->Clear();
  DataForm->MyQuery1->First();
  for (int i=0; i < DataForm->MyQuery1->RecordCount; i++)
  {
     str=DataForm->MyQuery1->Fields->Fields[0]->AsString;
     if(str[1]=='r' && str[2]=='a' && str[3]=='w') tables->Items->Add(str);
     DataForm->MyQuery1->Next();
  }
  break;

  case 3:
  tables->Clear();
  DataForm->MyQuery1->First();
  for (int i=0; i < DataForm->MyQuery1->RecordCount; i++)
  {
     str=DataForm->MyQuery1->Fields->Fields[0]->AsString;
     if(str[1]=='f' && str[2]=='i' && str[3]=='l') tables->Items->Add(str);
     DataForm->MyQuery1->Next();
  }
  break;

  case 4:
  tables->Clear();
  DataForm->MyQuery1->First();
  for (int i=0; i < DataForm->MyQuery1->RecordCount; i++)
  {
     str=DataForm->MyQuery1->Fields->Fields[0]->AsString;
     if(str[1]=='s' && str[2]=='i' && str[3]=='m') tables->Items->Add(str);
     DataForm->MyQuery1->Next();
  }
  break;

  case 5:
  tables->Clear();
  DataForm->MyQuery1->First();
  for (int i=0; i < DataForm->MyQuery1->RecordCount; i++)
  {
     str=DataForm->MyQuery1->Fields->Fields[0]->AsString;
     if(str[1]=='s' && str[2]=='e' && str[3]=='t') tables->Items->Add(str);
     DataForm->MyQuery1->Next();
  }
  break;

  }
}
//---------------------------------------------------------------------------

