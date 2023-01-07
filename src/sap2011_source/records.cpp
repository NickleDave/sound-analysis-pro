//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "records.h"
#include "start.h"
#include "CZT_FeatureCalc.h"
#include "table_dialog.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DBAccess"
#pragma link "MemDS"
#pragma link "MyAccess"
#pragma link "MyDump"
#pragma link "SDL_NumLab"
#pragma link "UCustomFlexCelReport"
#pragma link "UExcelAdapter"
#pragma link "UFlexCelReport"
#pragma link "XLSAdapter"
#pragma link "UFlexCelGrid"
#pragma link "UFlexCelImport"
#pragma link "UFlexCelPreview"
#pragma link "DADump"
#pragma link "MyDacVcl"
#pragma resource "*.dfm"
TDataForm *DataForm;
//---------------------------------------------------------------------------
__fastcall TDataForm::TDataForm(TComponent* Owner)
		: TForm(Owner)
{

   if(!FileExists("c://sap//db.properties")) {
      MyConnection1->Connect();
      myfile.open ("c:\\sap\\db.properties");
      if (!myfile) {
	   return ;
      }
      UnicodeString schema = MyConnection1->Database;
      UnicodeString user = MyConnection1->Username;
      UnicodeString password = MyConnection1->Password;

      char sch [80];
      char usr [80];
      char pwd [80];

      WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,schema.w_str(),-1,sch,sizeof(sch),NULL,NULL);
      WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,user.w_str(),-1,usr,sizeof(usr),NULL,NULL);
      WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,password.w_str(),-1,pwd,sizeof(pwd),NULL,NULL);


      myfile <<"schema = "<<sch<<"\n";
      myfile <<"username = "<<usr<<"\n";
      myfile <<"password = "<<pwd<<"\n";
      myfile.flush();
   }
   Config config((string)"c:/sap/db.properties");
   string schema = config.pString((string)"schema");
   string user = config.pString((string)"username");
   string pwd = config.pString((string)"password");
   const char* schemaStr = schema.c_str();
   const char* pwdStr = pwd.c_str();
   const char* userStr = user.c_str();
   wchar_t schemaUnicodeStr[80];
   wchar_t pwdUnicodeStr[80];
   wchar_t userUnicodeStr[80];
   MultiByteToWideChar(CP_ACP, 0, schemaStr, -1, schemaUnicodeStr, 1024);
   MultiByteToWideChar(CP_ACP, 0, pwdStr, -1, pwdUnicodeStr, 1024);
   MultiByteToWideChar(CP_ACP, 0, userStr, -1, userUnicodeStr, 1024);
   MyConnection1->Database = schemaUnicodeStr;
   MyConnection1->Username = userUnicodeStr;
   MyConnection1->Password=pwdUnicodeStr;
   MyConnection1->LoginPrompt=false;
   MyConnection1->Connect();

  if(!table_exists("settings"))create_settings("settings");
  Create_table("recs0");

  for(int i=0;i<10;i++)file_indexes[i]=0;
  create_similarity("similarity");
  Create_similarity_sections();
  file_table_name="file_table";
  Create_File_Table();
  Create_animals();
  Create_events();
  Create_actions();
  create_durations();
  
  birds->Active=true;
  experiments->Active=true;
  actions->Active=true;

  show_start=true;
  advance=1000;
  TheTable=Records;
  DirectoryListBox1->Directory ="c:\\sap\\";
 // File_index=Count_Table_Recs(Records4);
  allow_action_animal_changes->Down=true;
  allow_action_animal_changesClick(this);
  ShortDateFormat = "yyyy-m-d";//"m/d/yyyy";
  LongTimeFormat = "hh:mm:ss";
  datapages->Visible=true;
 /*
    myfile.open ("c:\\sap\\log_records.txt");
  if (!myfile) {
	return ;
  }   */
}


//---------------------------------------------------------------------------


void __fastcall TDataForm::FormClose(TObject *Sender, TCloseAction &Action)
{
show_table=false;
if(show_start)StartForm->Visible=true;

}
//---------------------------------------------------------------------------


 void TDataForm::showDatabases()
 {
    MyQuery1->SQL->Clear();
    AnsiString str="SHOW DATABASES;";
    MyQuery1->SQL->Add(str);
    MyQuery1->Execute();                                                                              
    MyQuery1->First();
 }


 void TDataForm::showTables()
 {
    MyQuery1->SQL->Clear();
    AnsiString str="SHOW TABLES;";
    MyQuery1->SQL->Add(str);
    MyQuery1->Execute();
    MyQuery1->First();
 }




void __fastcall TDataForm::openTableClick(TObject *Sender)
{
    //showTables();
    Application->CreateForm(__classid(TtableDialog), &tableDialog);
    tableDialog->show_type->ItemIndex=1;
    tableDialog->show_typeClick(this);
    if (tableDialog->ShowModal() == mrOk)
    {
      if(tableDialog->tables->ItemIndex==-1)return;
      TheTable->Active=false;
      TheTable->TableName= tableDialog->tables->Items->Strings[tableDialog->tables->ItemIndex];
      TheTable->Active=true;
      count_records(TheTable->TableName);
      //CountRecsClick(this);
      MyQuery1->Active=false;

      if(show_table){DataSource1->DataSet=TheTable;  DataSource1->Enabled=true;}
      else DataSource1->Enabled=false;
      table_name->Caption=TheTable->TableName;
      Song1->TableName=Records->TableName;
    }
}
//---------------------------------------------------------------------------


AnsiString TDataForm::open_Raw_Table()
{
    Application->CreateForm(__classid(TtableDialog), &tableDialog);
    tableDialog->show_type->ItemIndex=2;
    tableDialog->show_typeClick(this);
    if (tableDialog->ShowModal() == mrOk)
    {
      if(tableDialog->tables->ItemIndex==-1)return("");
      return(tableDialog->tables->Items->Strings[tableDialog->tables->ItemIndex]);
    }
    return("");
}





void __fastcall TDataForm::ToExelClick(TObject *Sender)
{
  SaveDialog1->DefaultExt="xls";
  if (!SaveDialog1->Execute()) return;
  if (FileExists(SaveDialog1->FileName)) DeleteFile(SaveDialog1->FileName);
  SongReport->FileName=SaveDialog1->FileName;
  SongReport->Run();
  str=SaveDialog1->FileName;
  ShellExecute(Application->Handle, "open", str.c_str(), NULL, NULL, SW_SHOWNORMAL);
  SaveDialog1->DefaultExt="sql";

/*
  AnsiString tname=TheTable->TableName;
  TheTable->Active=false;
  TheTable->TableName= tname;
  TheTable->Active=true;
*/
  //xlReport1->Report();
}
//---------------------------------------------------------------------------


void __fastcall TDataForm::CountRecsClick(TObject *Sender)
{
    if(!table_exists(TheTable->TableName))openTableClick(this);
    AnsiString tname=TheTable->TableName;
    str="SELECT COUNT(*) FROM "; str+=tname;
 //   MyQuery1->Active=true;  SHOW [FULL] COLUMNS FROM tbl_name
    MyQuery1->SQL->Clear();
    MyQuery1->SQL->Add(str);
    MyQuery1->Execute();
    recNum=MyQuery1->Fields->Fields[0]->AsInteger;
    recordsNum->Value=recNum;
}
//---------------------------------------------------------------------------

int TDataForm::Count_Table_Recs(TMyTable* table)
{
    int rc;
    if(!table_exists(table->TableName))return(-1); // error!!!
    AnsiString tname=table->TableName;
    str="SELECT COUNT(*) FROM "; str+=tname;
 //   MyQuery1->Active=true;  SHOW [FULL] COLUMNS FROM tbl_name
    MyQuery1->SQL->Clear();
    MyQuery1->SQL->Add(str);
    MyQuery1->Execute();
    rc=MyQuery1->Fields->Fields[0]->AsInteger;
    return(rc);
}




int TDataForm::count_records(AnsiString name)
{
   /*AnsiString tname;
   switch(table_num)
   {
     case 0: tname=Records->TableName;  break;
     case 1: tname=Records1->TableName; break;
     case 2: tname=Records2->TableName; break;
     case 3: tname=Records3->TableName; break;
     case 4: tname=Records4->TableName; break;
   }*/
   if(!table_exists(name))return(false);
   str="SELECT COUNT(*) FROM "; str+=name;
   MyQuery1->SQL->Clear();
   MyQuery1->SQL->Add(str);
   MyQuery1->Execute();
   recNum=MyQuery1->Fields->Fields[0]->AsInteger;
    recordsNum->Value=recNum;
   return(recNum);
}



void TDataForm::Settings(int bird, AnsiString feature, AnsiString value)
{
     settings->RecNo=bird;
     settings->Edit();
	 settings->FieldValues[feature]=value;
     settings->Post();
}


// This function retrives the settings preferences for a channel, e.g., bird ID
AnsiString TDataForm::Retrieve(int bird, AnsiString feature)
{
     AnsiString theFeature;
     settings->RecNo=bird;
	 settings->Refresh();
	 theFeature=settings->FieldValues[feature];
	 return(theFeature);
}


 int TDataForm::Retrieve_int(int bird, AnsiString feature)
{
     int theFeature;
   	 settings->RecNo=bird;
	 settings->Refresh();
	 theFeature=(int)settings->FieldValues[feature];
	 return(theFeature);
}


// this is a quary that fetch some data from Records into  MyQuary1
void TDataForm::Fetch_data(int channel, int location, int min_duration, int max_duration, int amount)
{
    MyQuery1->Active=true;
    MyQuery1->UnPrepare();
    str="SELECT * FROM ";
    switch(channel)
    {
      case 0: str+=Records->TableName; break;
      case 1: str+=Records1->TableName; break;
  //    case 2: str+=Records2->TableName; break;
  //    case 3: str+=Records3->TableName; break;
 //     case 4: str+=Records4->TableName; break;
    }
    
    str+=" WHERE recnum>"; str+=location;
    str+=" AND duration>"; str+=min_duration;
    str+=" AND duration<"; str+=max_duration;
    str+=" LIMIT "; str+=amount;
    MyQuery1->SQL->Clear();
    MyQuery1->SQL->Add(str);
    MyQuery1->Execute();
}

// this is a quary that fetch some data from Records into  MyQuary1 based on arbitrary criteria
void TDataForm::Select_data(AnsiString quary)
{
    str="SELECT * FROM ";
    str+=Records->TableName;
    str+=" WHERE ";
    str+=quary;
    MyQuery1->SQL->Clear();
    MyQuery1->SQL->Add(str);
    MyQuery1->Execute();
}

void TDataForm::Select_data_freeStyle(AnsiString quary)
{
    MyQuery1->SQL->Clear();
    MyQuery1->SQL->Add(quary);
    MyQuery1->Execute();
}


void TDataForm::ChangeTable(AnsiString name, AnsiString feature, AnsiString val, int location)
{
    //DataForm->Records->Active=false;
    str="update "; str+=name; str+=" set "; str+=feature; str+="="; str+=val; str+=" where recnum=";  str+=location;
    MyCommand1->SQL->Clear();
    MyCommand1->SQL->Add(str);
    MyCommand1->Execute(1);
    

}

void TDataForm::ChangeTable_freestyle(AnsiString command)
{

    MyCommand1->SQL->Clear();
    MyCommand1->SQL->Add(command);
    MyCommand1->Execute(1);
    

}


void __fastcall TDataForm::empty_tableClick(TObject *Sender)
{

  if (MessageDlg("Are you sure? All records will be deleted", mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
  {
  Records->Active=false;
  Records->EmptyTable();
  Records->Active=true;
  }
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::FormShow(TObject *Sender)
{
show_table=false;
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::convertClick(TObject *Sender)
{
   /*
  AnsiString str1,str2;
  AnsiString fields[20];
  MATFile *pmat;
  mxArray *pa1;//, *pa2, *pa3;
  //dataCount->Value=150;
  Memo2->Clear();
  DataSource1->Enabled=false;
  if(OpenDialog1->Execute())str1=OpenDialog1->FileName; else return;
  const char *file = str1.c_str();//"c:\\ofer_mattest4.mat";

  // we now find which fields to copy
  int col=0;
  for(int i=0;i<CheckListBox1->Items->Count; i++)
  {
    if(CheckListBox1->Checked[i])
      {fields[col]=CheckListBox1->Items->Strings[i]; col++; }
  }
  if(!col){Application->MessageBox("Please select fields to copy first. Make sure you select numeric fields only.", NULL, MB_OK);return;}
  //data->First();

  double *recs=new double[col*dataCount->Value];
  //int records=dataCount->Value;
  ProgressBar1->Max=dataCount->Value;
  //int addit=0;
  Fetch_data(0, start->Value,0, 99999, dataCount->Value);
  MyQuery1->First();
  //for(int i=start->Value; i<start->Value+dataCount->Value; i++)//start->Value; i<start->Value+dataCount->Value; i++)
  for(int i=0; i<dataCount->Value; i++)
  {
     for(int j=0;j<col;j++)
     {
       recs[(i*col)+j]=MyQuery1->FieldValues[fields[j]];
       //int of=recs[(i*col)+j];
     }
     ProgressBar1->StepIt();
     MyQuery1->Next();
  }
  int status;
  pmat = matOpen(file, "w"); if (pmat == NULL) { Memo2->Lines->Add("Error creating file"); return; }
  pa1 = mxCreateDoubleMatrix(col, dataCount->Value, mxREAL); if (pa1 == NULL) { Memo2->Lines->Add("Out of memory"); return; }
  memcpy((void *)(mxGetPr(pa1)), (void *)recs, sizeof(double)*col*dataCount->Value);//recs));
  status = matPutVariable(pmat, "LocalDouble", pa1); if (status != 0) { Memo2->Lines->Add("Error using matPutVariable"); return; }
  mxDestroyArray(pa1);
  if (matClose(pmat) != 0)  { Memo2->Lines->Add("Error closing file"); return; }
  Memo2->Lines->Add("Mat file created successfully");
  delete[] recs;
  ProgressBar1->Position=0;
                              */
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::open_tableClick(TObject *Sender)
{
   openTableClick(this);
   dataCount->Value=recordsNum->Value;
   dataCountTb->Max=recordsNum->Value;
   start->Value=0;
   startTb->Max=recordsNum->Value;
   CheckListBox1->Clear();
   for (int i=0;i<Records->FieldDefs->Count;i++) CheckListBox1->Items->Add(Records->FieldDefs->Items[i]->Name);
   for (int i=0;i<Records->FieldDefs->Count;i++)
   {
   if(checked[i])CheckListBox1->Checked[i]=true; else CheckListBox1->Checked[i]=false;
   }
}        

//---------------------------------------------------------------------------

void __fastcall TDataForm::dataCountTbChange(TObject *Sender)
{
dataCount->Value=dataCountTb->Position;
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::startTbChange(TObject *Sender)
{
start->Value=startTb->Position;
 dataCountTb->Max=recordsNum->Value-start->Value;
 if(dataCount->Value>dataCountTb->Max)dataCount->Value=dataCountTb->Max;        
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::BitBtn2Click(TObject *Sender)
{
for(int i=0;i<CheckListBox1->Items->Count; i++)
  {
     if(CheckListBox1->Checked[i])checked[i]=true; else checked[i]=false;

  }
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::forwardClick(TObject *Sender)
{
   startTb->Position+=advance;
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::stepsClick(TObject *Sender)
{
  switch(steps->ItemIndex)
  {
    case 0: advance=1000; break;
    case 1: advance=10000; break;
    case 2: advance=50000; break;
    case 3: advance=100000; break;
    case 4: advance=250000; break;
  }
  dataCountTb->Position=advance;
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::new_tableClick(TObject *Sender)
{
	AnsiString New_Table = InputBox("New Table", "The prefix syll_ will be automatically added to the name you type", "");
    New_Table="syll_"+New_Table;
	if(!New_Table.Length())return;
    if(New_Table[1]=='0' || New_Table[1]=='1' || New_Table[1]=='2'|| New_Table[1]=='3'|| New_Table[1]=='4'
       || New_Table[1]=='5'|| New_Table[1]=='6'|| New_Table[1]=='7'|| New_Table[1]=='8'|| New_Table[1]=='9')
       {MessageDlg("Error: table name must start with a letter.", mtWarning, TMsgDlgButtons() << mbOK, 0);return;}
    Create_table(New_Table);
    TheTable->Active=false;
    TheTable->TableName=New_Table;
    TheTable->Active=true;
    CountRecsClick(this);
    table_name->Caption=TheTable->TableName;
    if(show_table){DataSource1->DataSet=TheTable;  DataSource1->Enabled=true;}
    else DataSource1->Enabled=false;
    Song1->TableName=DataForm->Records->TableName;

}



// create syllable table
void TDataForm::Create_table(AnsiString table_name)
{
     // if(table_exists(table_name)) return;
      str="CREATE TABLE IF NOT EXISTS ";
      str+=table_name;
      str+="(  `recnum` int(11) NOT NULL auto_increment, ";
      str+="`serial_number` double(20,9) unsigned default '0.000000000',";
      str+="`bird_ID` char(10) default '',";
      str+="`start_on` float default '0',";
      str+="`duration` float default '0',";
      str+="`mean_pitch` float default '0',";
	  str+="`mean_peak_freq` float default '0',";
      str+="`mean_FM` float default '0',";
      str+="`mean_entropy` float default '0',";
      str+="`mean_goodness_of_pitch` float default '0',";
      str+="`mean_mean_frequency` float default '0',";
      str+="`mean_am2` float default '0',";
      str+="`mean_amplitude` float default '0',";
      str+="`mean_continuity_f` float default '0',";
      str+="`mean_continuity_t` float default '0',";
	  str+="`mean_DAS` float default '0',";
      str+="`min_pitch` float default '0',";
	  str+="`min_peak_freq` float default '0',";
	  str+="`min_FM` float default '0',";
	  str+="`min_entropy` float default '0',";
	  str+="`min_meanFr` float default '0',";
	  str+="`min_DAS` float default '0',";
	  str+="`max_pitch` float default '0',";
	  str+="`max_peak_freq` float default '0',";
	  str+="`max_FM` float default '0',";
	  str+="`max_amplitude` float default '0',";
	  str+="`max_entropy` float default '0',";
	  str+="`max_meanFr` float default '0',";
	  str+="`max_DAS` float default '0',";
	  str+="`max_continuity_f` float default '0',";
	  str+="`max_continuity_t` float default '0',";
	  str+="`var_pitch` float default '0',";
	  str+="`var_FM` float default '0',";
	  str+="`var_entropy` float default '0',";
	  str+="`var_goodness_of_pitch` float default '0',";
	  str+="`var_mean_frequency` float default '0',";
	  str+="`var_am` float default '0',";
	  str+="`month` tinyint(4) default '0',";
	  str+="`day` tinyint(4) default '0',";
	  str+="`hour` tinyint(4) default '0',";
	  str+="`minute` tinyint(4) default '0',";
	  str+="`second` tinyint(4) default '0',";
	  str+="`cluster` tinyint(4) NOT NULL default '0',";
	  str+="`file_name` varchar(80) default '0',";
	  str+="`comments` varchar(80) default '0',";
	  str+="PRIMARY KEY  (`recnum`))";
	  MyCommand1->SQL->Clear();
	  MyCommand1->SQL->Add(str);
	  MyCommand1->Execute(1);
}


void TDataForm::Create_actions()
{
  str="CREATE TABLE IF NOT EXISTS `action` (";
  str+="`action_ID` int(11) NOT NULL auto_increment,";
  str+="`type` char(20) NOT NULL default '',";
  str+="`attribute1_name` char(20) NOT NULL default '',";
  str+="`attribute1_value` char(20) NOT NULL default '',";
  str+="`attribute2_name` char(20) NOT NULL default '',";
  str+="`attribute2_value` char(20) NOT NULL default '',";
  str+="`attribute3_name` char(20) NOT NULL default '',";
  str+="`attribute3_value` char(20) NOT NULL default '',";
  str+="`attribute4_name` char(20) NOT NULL default '',";
  str+="`attribute4_value` char(20) NOT NULL default '',";
  str+="`details` char(80) NOT NULL default '',";
  str+="PRIMARY KEY  (`action_ID`))";
  MyCommand1->SQL->Clear();
  MyCommand1->SQL->Add(str);
  MyCommand1->Execute(1);

}



//---------------------------------------------------------------------------



void TDataForm::Create_events()
{
  //if(table_exists("animal_actions_log")) return;
  str="CREATE TABLE IF NOT EXISTS `animal_actions_log` (";
  str+="`session_ID` int(11) NOT NULL auto_increment,";
  str+="`date` date NOT NULL default '2000-02-02',";
  str+="`time` time NOT NULL default '00:00:00',";
  str+="`bird_name` char(10) NOT NULL default '',";
  str+="`action_ID` int(11) NOT NULL default '0',";
  str+="`action_Type` char(20) NOT NULL default '',";
  str+="`resource_location` char(20) NOT NULL default '',";
  str+="`file1` char(80) NOT NULL default '',";
  str+="`file2` char(80) NOT NULL default '',";
  str+="`details` char(80) NOT NULL default '',";
  str+="PRIMARY KEY  (`session_ID`))";
  MyCommand1->SQL->Clear();
  MyCommand1->SQL->Add(str);
  MyCommand1->Execute(1);
}



//---------------------------------------------------------------------------




void TDataForm::Create_animals()
{
 // if(table_exists("animals")) return;
  str="CREATE TABLE IF NOT EXISTS `animals` (";
  str+="`bird_name` char(10) NOT NULL default '',";
  str+="`sex` char(1) NOT NULL default 'M',";
  str+="`hatched` date NOT NULL default '0000-00-00',";
  str+="`weight` float NOT NULL default '0',";
  str+="`location` smallint(6) NOT NULL default '0',";
  str+="`procedure1` char(12) NOT NULL default 'isolation',";
  str+="`proc1_date` datetime NOT NULL default '0000-00-00 00:00:00',";
  str+="`procedure2` char(12) NOT NULL default 'surgery',";
  str+="`proc2_date` datetime NOT NULL default '0000-00-00 00:00:00',";
  str+="`procedure3` char(12) NOT NULL default 'finish',";
  str+="`proc3_date` datetime NOT NULL default '0000-00-00 00:00:00',";
  str+="`training` datetime NOT NULL default '0000-00-00 00:00:00',";
  str+="`model` char(12) NOT NULL default '',";
  str+="`quota` smallint(6) NOT NULL default '0',";
  str+="`audio` char(16) NOT NULL default '',";
  str+="`attributes` char(12) NOT NULL default '',";
  str+="`photoperiod` char(12) NOT NULL default '',";
  str+="`details` char(80) NOT NULL default '',";
  str+="PRIMARY KEY  (`bird_name`))";
  MyCommand1->SQL->Clear();
  MyCommand1->SQL->Add(str);
  MyCommand1->Execute(1);
 }


//---------------------------------------------------------------------------

void TDataForm::Create_similarity_sections()
{
  //if(table_exists("similarity_sections")) return;
  str="CREATE TABLE IF NOT EXISTS `similarity_sections` (";
  str+="`recnum` int(11) NOT NULL default '0',";
  str+="`start1` float NOT NULL default '0',";
  str+="`end1` float NOT NULL default '0',";
  str+="`start2` float NOT NULL default '0',";
  str+="`end2` float NOT NULL default '0',";
  str+="`accuracy` float NOT NULL default '0');";
  MyCommand1->SQL->Clear();
  MyCommand1->SQL->Add(str);
  MyCommand1->Execute(1);
}


void TDataForm::Create_bouts()
{
  //if(table_exists("similarity_sections")) return;
  str="CREATE TABLE IF NOT EXISTS `bouts` (";
  str+="`recnum` int(11) NOT NULL auto_increment,";
  str+="`bird_name` char(10) NOT NULL default '',";
  str+="`bout_duration` float NOT NULL default '0',";
  str+="`file` char(100) NOT NULL default '', PRIMARY KEY  (`recnum`))";
  MyCommand1->SQL->Clear();
  MyCommand1->SQL->Add(str);
  MyCommand1->Execute(1);
}

void TDataForm::insert_bout(AnsiString bird, int bout_dur, AnsiString newFile)
{
   AnsiString song_file;
   for(int i=1; i<=newFile.Length(); i++) {song_file+=newFile[i]; if(newFile[i]=='\\')song_file+='\\';  }
   str="INSERT INTO bouts(bird_name, bout_duration,file)VALUES ('";
    str+=bird; str+="',";
    str+=bout_dur; str+=",'";
    str+=song_file;
    str+="');";
	MyCommand1->SQL->Clear();
    MyCommand1->SQL->Add(str);
    MyCommand1->Execute(1);
    MyCommand1->UnPrepare();
}


// this function inserts record into the animal action log table, which holds comments about sound files in this case
void TDataForm::insert_comment(AnsiString bird_name, AnsiString file_name, AnsiString comments)
{
	// do we have comments for this file? If yes replace
	str="Select COUNT(*) from animal_actions_log where file1='";
    str+=file_name; str+="'";
    MyQuery1->SQL->Clear();
	MyQuery1->SQL->Add(str);
	MyQuery1->Execute();
	int num=MyQuery1->Fields->Fields[0]->AsInteger;
    if(num)
    {
       str="Select * from animal_actions_log where file1='";
       str+=file_name; str+="'";
       MyQuery1->SQL->Clear();
       MyQuery1->SQL->Add(str);
       MyQuery1->Execute();
       MyQuery1->Edit();
       MyQuery1->FieldValues["details"]=comments;
       MyQuery1->Post();
       MyQuery1->ApplyUpdates();
    }

    else
    {
      // if this is the first time we place comments insert new file record:
      str="INSERT INTO animal_actions_log(date, time, bird_name, file1, details)VALUES ('";
	  str2=FormatDateTime(Date(),1);
	  str=str+str2;//Date();
	  str+="','";
	  str2=FormatDateTime (Time(),0);
	  str+=str2;//Time();
	  str+="','";
      str+=bird_name; str+="','";
      str+=file_name; str+="','";
      str+=comments; str+="');";
      MyCommand1->SQL->Clear();
	  MyCommand1->SQL->Add(str);
	  MyCommand1->Execute(1);
	  MyCommand1->UnPrepare();
    }
}

AnsiString TDataForm::Retrieve_comments(AnsiString file_name)
{
	   str="Select details from animal_actions_log where file1='";
       str+=file_name; str+="'";
       MyQuery1->SQL->Clear();
       MyQuery1->SQL->Add(str);
       MyQuery1->Execute();
       if(MyQuery1->RecordCount)str=MyQuery1->FieldValues["details"];
       else str="";
       return(str);
}



bool TDataForm::retrieve_animal(AnsiString name) // this can be a file name or a table name or just the bird's name from elsewhere...
{

   //myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();

   AnsiString current_database=MyConnection1->Database;

   //myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();

   if(current_database!="sap")Change_Database("SAP");

   //myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();

   bool giveup=false;
   int i=1, found;
   int string_size=name.Length();
   AnsiString query, n, bird_name="";
   // get the name from the file or table name

   // if starts with syll_ or raw_ remove those
   if( (name[1]=='s' && name[2]=='y' && name[3]=='l') || (name[1]=='r' && name[2]=='a' && name[3]=='w') )
   {

   //myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();

    do{i++;}while(name[i]!='_');
	i++;

	//myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();

	name=name.SubString(i,name.Length()-i+1);

	//myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();

   }
   for(int j=1;j<=name.Length();j++)
                        bird_name+=name[j];
   str="select * from animals where bird_name";
      // first try literal
   query=str;
   query+="='"; query+=bird_name; query+="' limit 1";

   //myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();

   MyQuery1->SQL->Clear();

   //myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();

   MyQuery1->SQL->Add(query);

   //myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();

   MyQuery1->Execute();

   //myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();

   found=MyQuery1->RecordCount;

   //myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();

   if(!found)
   {
     // leave only the number
	 name="";

	 //myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();

	 string_size=bird_name.Length();

	 //myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();

     int i=0;
     do{
		i++;

		//myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();

		if(bird_name[i]>='0' && bird_name[i]<='9') name+=bird_name[i];

		//myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();

        }while(i<string_size && bird_name[i]!='_');
     bird_name=name;
     // then try with one missing charecter at the left "_"
     if(bird_name.Length())
	 {

	   //myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();

       query=str; query+=" like '"; query+="_"; query+=bird_name; query+="' limit 1";

	   //myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();

	   MyQuery1->SQL->Clear();

	   //myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();

	   MyQuery1->SQL->Add(query);

	   //myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();

	   MyQuery1->Execute();

	   //myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();

	   found=MyQuery1->RecordCount;
     }
     else {found=false; giveup=true;}
   }
   if(!found && !giveup)
   {
     // then try with any number of missing charecters at the left "%"
     query=str; query+=" like '"; query+="%"; query+=bird_name; query+="' limit 1";

	 //myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();

	 MyQuery1->SQL->Clear();

	 //myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();

	 MyQuery1->SQL->Add(query);

	 //myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();

	 MyQuery1->Execute();

	 //myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();

	 found=MyQuery1->RecordCount;

	 //myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();

   }
   if(!found && !giveup)
   {
     // then try with any number of missing charecters at the left or right "% %"
     query=str; query+=" like '"; query+="%"; query+=bird_name; query+="%"; query+="' limit 1";

	 //myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();

	 MyQuery1->SQL->Clear();

	 //myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();

	 MyQuery1->SQL->Add(query);

	 //myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();

	 MyQuery1->Execute();

	 //myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();

	 found=MyQuery1->RecordCount;

	 //myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();

   }

   //myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();

   if(current_database!="sap")Change_Database(current_database);

   //myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();

   if(!found)return(false);
   else return(true);   
}

//---------------------------------------------------------------------------



void TDataForm::Create_Brain_syllable_table(AnsiString table_name)
{
      if(table_exists(table_name))
      {
         if(!brain_format(table_name))
         {
             if (MessageDlg("A table name already exists, but its format does not match this module -- replace by a new table?", mtWarning, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
                    DeleteTable(table_name);
             else return;
         }
         else return; // keep the exsisting table
      }

      str="CREATE TABLE IF NOT EXISTS ";
      str+=table_name;
      str+="(  `recnum` int(11) NOT NULL auto_increment, ";
      str+="`serial_number` double(20,9) unsigned default '0.000000000',";
      str+="`bird_ID` char(10) default '',";
      str+="`start_on` float default '0',";
      str+="`duration` float default '0',";
      str+="`mean_pitch` float default '0',";
      str+="`mean_principal_contour` float default '0',";
      str+="`mean_FM` float default '0',";
      str+="`mean_entropy` float default '0',";
      str+="`mean_goodness_of_pitch` float default '0',";
      str+="`mean_mean_frequency` float default '0',";
      str+="`mean_am2` float default '0',";
      str+="`mean_amplitude` float default '0',";
      str+="`mean_continuity_f` float default '0',";
      str+="`mean_continuity_t` float default '0',";
      str+="`mean_slope` float default '0',";
      str+="`min_pitch` float default '0',";
      str+="`min_principal_contour` float default '0',";
      str+="`min_FM` float default '0',";
      str+="`min_entropy` float default '0',";
      str+="`min_peakFr` float default '0',";
      str+="`min_slope` float default '0',";
      str+="`max_pitch` float default '0',";
      str+="`max_principal_contour` float default '0',";
      str+="`max_FM` float default '0',";
      str+="`max_amplitude` float default '0',";
      str+="`max_entropy` float default '0',";
      str+="`max_peakFr` float default '0',";
      str+="`max_slope` float default '0',";
      str+="`max_continuity_f` float default '0',";
      str+="`max_continuity_t` float default '0',";
      str+="`var_pitch` float default '0',";
      str+="`var_FM` float default '0',";
      str+="`var_entropy` float default '0',";
      str+="`var_goodness_of_pitch` float default '0',";
      str+="`var_mean_frequency` float default '0',";
      str+="`var_am` float default '0',";

      str+="`ch1_amplitude` float default '0',";
      str+="`ch1_entropy` float default '0',";
      str+="`ch1_peak_frequency` float default '0',";
      str+="`ch1_peak_frequency_power` float default '0',";
      str+="`ch1_highpath_power` float default '0',";

      str+="`ch2_amplitude` float default '0',";
      str+="`ch2_entropy` float default '0',";
      str+="`ch2_peak_frequency` float default '0',";
      str+="`ch2_peak_frequency_power` float default '0',";
      str+="`ch2_highpath_power` float default '0',";

      str+="`ch3_amplitude` float default '0',";
      str+="`ch3_entropy` float default '0',";
      str+="`ch3_peak_frequency` float default '0',";
      str+="`ch3_peak_frequency_power` float default '0',";
      str+="`ch3_highpath_power` float default '0',";

      str+="`ch4_amplitude` float default '0',";
      str+="`ch4_entropy` float default '0',";
      str+="`ch4_peak_frequency` float default '0',";
      str+="`ch4_peak_frequency_power` float default '0',";
      str+="`ch4_highpath_power` float default '0',";

      str+="`ch5_amplitude` float default '0',";
      str+="`ch5_entropy` float default '0',";
      str+="`ch5_peak_frequency` float default '0',";
      str+="`ch5_peak_frequency_power` float default '0',";
      str+="`ch5_highpath_power` float default '0',";

      str+="`month` tinyint(4) default '0',";
      str+="`day` tinyint(4) default '0',";
      str+="`hour` tinyint(4) default '0',";
      str+="`minute` tinyint(4) default '0',";
      str+="`second` tinyint(4) default '0',";
      str+="`cluster` tinyint(4) NOT NULL default '0',";
      str+="`file_name` varchar(80) default '0',";
      str+="PRIMARY KEY  (`recnum`))";
      MyCommand1->SQL->Clear();
      MyCommand1->SQL->Add(str);
      MyCommand1->Execute(1);






      /*
      str="CREATE TABLE IF NOT EXISTS ";
      str+=table_name;
      str+="(  `recnum` int(11) NOT NULL auto_increment, ";
      str+="`serial_number` double(20,9) unsigned default '0.000000000',";
      str+="`bird_ID` char(10) default '0',";
      str+="`start_on` float default '0',";
      str+="`duration` float default '0',";
      str+="`mean_pitch` float default '0',";
      str+="`mean_FM` float default '0',";
      str+="`mean_entropy` float default '0',";
      str+="`mean_goodness_of_pitch` float default '0',";
      str+="`mean_mean_frequency` float default '0',";
      str+="`mean_am2` float default '0',";
      str+="`mean_amplitude` float default '0',";
      str+="`var_pitch` float default '0',";
      str+="`var_FM` float default '0',";
      str+="`var_entropy` float default '0',";
      str+="`var_goodness_of_pitch` float default '0',";
      str+="`var_mean_frequency` float default '0',";
      str+="`var_am` float default '0',";

      str+="`ch1f1`float default '0',";
      str+="`ch1f2`float default '0',";
      str+="`ch2f1`float default '0',";
      str+="`ch2f2`float default '0',";
      str+="`ch3f1`float default '0',";
      str+="`ch3f2`float default '0',";
      str+="`ch4f1`float default '0',";
      str+="`ch4f2`float default '0',";
      str+="`ch5f1`float default '0',";
      str+="`ch5f2`float default '0',";
      str+="`ch6f1`float default '0',";
      str+="`ch6f2`float default '0',";
      str+="`ch7f1`float default '0',";
      str+="`ch7f2`float default '0',";
      str+="`ch8f1`float default '0',";
      str+="`ch8f2`float default '0',";
      str+="`ch9f1`float default '0',";
      str+="`ch9f2`float default '0',";
      str+="`ch10f1`float default '0',";
      str+="`ch10f2`float default '0',";

      str+="`month` tinyint(4) default '0',";
      str+="`day` tinyint(4) default '0',";
      str+="`hour` tinyint(4) default '0',";
      str+="`minute` tinyint(4) default '0',";
      str+="`second` tinyint(4) default '0',";
      str+="`cluster` tinyint(4) NOT NULL default '0',";
      str+="`file_name` varchar(80) default '0',";
      str+="PRIMARY KEY  (`recnum`))";
      MyCommand1->SQL->Clear();
      MyCommand1->SQL->Add(str);
      MyCommand1->Execute(1);

      */
}
//---------------------------------------------------------------------------
 void TDataForm::Insert_syllable_record(
                    AnsiString table_name,
                    double serial_number,
                    //int
                    AnsiString bird_ID,
                    float start_on,
                    float duration,
                    float mean_pitch,
                    float mean_principal_contour,
                    float mean_FM,
                    float mean_entropy,
                    float mean_goodness_of_pitch,
                    float mean_mean_frequency,
                    float mean_am2,
                    float mean_amplitude,
                    float mean_continuity_f,
                    float mean_continuity_t,
                    float mean_slope,

                    float min_pitch,
                    float min_principal_contour,
                    float min_FM,
                    float min_entropy,
                    float min_peakFr,
                    float min_slope,

                    float max_pitch,
                    float max_principal_contour,
                    float max_FM,
                    float max_entropy,
                    float max_peakFr,
                    float max_slope,
                    float max_amplitude,
                    float max_continuity_f,
                    float max_continuity_t,

                    float var_pitch,
                    float var_FM,
                    float var_entropy,
                    float var_goodness_of_pitch,
                    float var_mean_frequency,
                    float var_am,
                    int month,
                    int day,
                    int hour,
                    int minute,
                    int second,
                    AnsiString file_name,
                    AnsiString comments
				)
{
 str="INSERT INTO ";
 str+=table_name;
 str+=" VALUES(NULL,";

 /*
 str="INSERT INTO ";
 str+=table_name;
 str+="(serial_number, bird_ID, start_on, duration, mean_pitch, mean_principal_contour, mean_FM, mean_entropy, mean_goodness_of_pitch, ";
 str+="mean_mean_frequency, mean_am2, mean_amplitude,  mean_continuity_f, mean_continuity_t, mean_slope, ";
 str+="min_pitch, min_principal_contour, min_FM, min_entropy, min_peakFr, min_slope, ";
 str+="max_pitch, max_principal_contour, max_FM, max_entropy, max_peakFr, max_slope, max_amplitude, max_continuity_f, max_continuity_t,";
 str+="var_pitch, var_FM, var_entropy, var_goodness_of_pitch," ;
 str+="var_mean_frequency, var_am, month, day, hour, minute, second, file_name, comments)" ;
 str+="VALUES (";
 */
 str+= serial_number; str+=", '";
 str+=bird_ID  ; str+="',";
 str+=start_on  ; str+=",";
 str+=duration  ; str+=",";
 str+=mean_pitch  ; str+=",";
 str+=mean_principal_contour  ; str+=",";
 str+=mean_FM  ; str+=",";
 str+=mean_entropy  ;  str+=",";
 str+= mean_goodness_of_pitch;  str+=",";
 str+=mean_mean_frequency;  str+=",";
 str+=mean_am2;  str+=",";
 str+=mean_amplitude;  str+=",";
 str+=mean_continuity_f;  str+=",";
 str+=mean_continuity_t;  str+=",";
 str+=mean_slope;  str+=",";

 str+= min_pitch;  str+=",";
 str+=min_principal_contour  ; str+=",";
 str+= min_FM;  str+=",";
 str+= min_entropy;  str+=",";
 str+= min_peakFr;  str+=",";
 str+= min_slope;  str+=",";

 /*
 recnum, serial_number, bird_ID, start_on, duration, mean_pitch, mean_peak_freq, mean_FM,
 mean_entropy, mean_goodness_of_pitch, mean_mean_frequency, mean_am2, mean_amplitude,
 mean_continuity_f, mean_continuity_t, mean_DAS, min_pitch, min_peak_freq, min_FM, min_entropy,
 min_meanFr, min_DAS, max_pitch, max_peak_freq, max_FM, max_amplitude, max_entropy, max_meanFr,
 max_DAS, max_continuity_f, max_continuity_t,

 var_pitch, var_FM, var_entropy,
 var_goodness_of_pitch, var_mean_frequency, var_am, month, day, hour, minute, second, cluster,
  file_name, comments
 */

 str+= max_pitch;  str+=",";
 str+=max_principal_contour  ; str+=",";
 str+= max_FM;  str+=",";
 str+= max_amplitude;  str+=",";
 str+= max_entropy;  str+=",";
 str+= max_peakFr;  str+=",";
 str+= max_slope;  str+=",";
 str+= max_continuity_f;  str+=",";
 str+= max_continuity_t;  str+=",";

 str+=var_pitch;  str+=",";
 str+=var_FM;  str+=",";
 str+=var_entropy;  str+=",";
 str+=var_goodness_of_pitch;  str+=",";
 str+=var_mean_frequency;  str+=",";
 str+=var_am;  str+=",";
 str+=month;  str+=",";
 str+=day;  str+=",";
 str+=hour;  str+=",";
 str+=minute;  str+=",";
 str+=second;  str+=",";// '";
 str+="0, '";  //str+=", '";
 str+=file_name; str+="', '";
 str+=comments;
 str+="')";
 Memo12->Lines->Add(str);
 //recs0(start_on, duration, mean_FM, mean_pitch) VALUES (1,2,3,4);
 MyCommand1->SQL->Clear();
 MyCommand1->SQL->Add(str);
 MyCommand1->Execute(1);
 //MyConnection1->Commit();
 MyCommand1->UnPrepare();
// MyConnection1->ApplyUpdates();


}


void __fastcall TDataForm::SimToExcelClick(TObject *Sender)
{
//xlReport2->Report();
}
//---------------------------------------------------------------------------


void TDataForm::Insert_similarity_section(int num,float Xs, float Xe, float Ys, float Ye, float accuracy)
{
    str="INSERT INTO similarity_sections(recnum,start1,end1,start2,end2,accuracy)VALUES (";
    str+=num; str+=",";
    str+=Xs; str+=",";
    str+=Xe; str+=",";
    str+=Ys; str+=",";
    str+=Ye; str+=",";
    str+=accuracy;
    str+=");";
    MyCommand1->SQL->Clear();
    MyCommand1->SQL->Add(str);
    MyCommand1->Execute(1);
    MyCommand1->UnPrepare(); 
}



void TDataForm::Insert_syllable_brain_record(

                AnsiString table_name,
                double serial_number,
                //int
                AnsiString bird_ID,
                float start_on,
                float duration,
                float mean_pitch,
                float mean_FM,
                float mean_entropy,
                float mean_goodness_of_pitch,
                float mean_mean_frequency,
                float mean_am2,
                float mean_amplitude,
                float var_pitch,
                float var_FM,
                float var_entropy,
                float var_goodness_of_pitch,
                float var_mean_frequency,
                float var_am,

                float s1_amplitude,
                float s1_entropy,
                float s1_peak_frequency,
                float s1_peak_power,
                float s1_highpass_power,

                float s2_amplitude,
                float s2_entropy,
                float s2_peak_frequency,
                float s2_peak_power,
                float s2_highpass_power,

                float s3_amplitude,
                float s3_entropy,
                float s3_peak_frequency,
                float s3_peak_power,
                float s3_highpass_power,

                float s4_amplitude,
                float s4_entropy,
                float s4_peak_frequency,
                float s4_peak_power,
                float s4_highpass_power,

                float s5_amplitude,
                float s5_entropy,
                float s5_peak_frequency,
                float s5_peak_power,
                float s5_highpass_power,

                int month,
                int day,
                int hour,
                int minute,
                int second,
                AnsiString file_name
        )
{
 //MyConnection1->StartTransaction();
 str="INSERT INTO ";
 str+=table_name;
 str+="(serial_number, bird_ID, start_on, duration, mean_pitch, mean_FM, mean_entropy, mean_goodness_of_pitch,";
 str+="mean_mean_frequency, mean_am2, mean_amplitude, var_pitch, var_FM, var_entropy, var_goodness_of_pitch," ;
 str+="var_mean_frequency, var_am, ch1f1, ch1f2, ch2f1, ch2f2, ch3f1, ch3f2,  ch4f1, ch4f2,  ch5f1, ch5f2,  ";
 str+=" ch6f1, ch6f2, ch7f1, ch7f2, ch8f1, ch8f2, ch9f1, ch9f2, ch10f1, ch10f2,";
 str+="month, day, hour, minute, second, file_name)" ;
 str+="VALUES (";
 str+= serial_number; str+=",";
 str+=bird_ID  ; str+=",";
 str+=start_on  ; str+=",";
 str+=duration  ; str+=",";
 str+=mean_pitch  ; str+=",";
 str+=mean_FM  ; str+=",";
 str+=mean_entropy  ;  str+=",";
 str+= mean_goodness_of_pitch;  str+=",";
 str+=mean_mean_frequency;  str+=",";
 str+=mean_am2;  str+=",";
 str+=mean_amplitude;  str+=",";
 str+=var_pitch;  str+=",";
 str+=var_FM;  str+=",";
 str+=var_entropy;  str+=",";
 str+=var_goodness_of_pitch;  str+=",";
 str+=var_mean_frequency;  str+=",";
 str+=var_am;  str+=",";
/*
 str+=ch1f1; str+=",";
 str+=ch1f2; str+=",";
 str+=ch2f1; str+=",";
 str+=ch2f2; str+=",";
 str+=ch3f1; str+=",";
 str+=ch3f2; str+=",";
 str+=ch4f1; str+=",";
 str+=ch4f2; str+=",";
 str+=ch5f1; str+=",";
 str+=ch5f2; str+=",";
 str+=ch6f1; str+=",";
 str+=ch6f2; str+=",";
 str+=ch7f1; str+=",";
 str+=ch7f2; str+=",";
 str+=ch8f1; str+=",";
 str+=ch8f2; str+=",";
 str+=ch9f1; str+=",";
 str+=ch9f2; str+=",";
 str+=ch10f1; str+=",";
 str+=ch10f2; str+=",";
*/
 str+=month;  str+=",";
 str+=day;  str+=",";
 str+=hour;  str+=",";
 str+=minute;  str+=",";
 str+=second;  str+=", '";
 str+=file_name;
 str+="')";

 //recs0(start_on, duration, mean_FM, mean_pitch) VALUES (1,2,3,4);
 MyCommand1->SQL->Clear();
 MyCommand1->SQL->Add(str);
 MyCommand1->Execute(1);
 //MyConnection1->Commit();
 MyCommand1->UnPrepare();
// MyConnection1->ApplyUpdates();


}





void __fastcall TDataForm::Add_daysClick(TObject *Sender)
{
     int i=0, Current_day=1, current_hour, previous_hour;
     TheTable=Records;
     Records->FetchAll=true;
     Records->Limit=-1;
     openTableClick(this);
     //Records->First();
     //ChangeTable(Records->TableName, "cluster", 1, 1000);
     previous_hour=Records->FieldByName("hour")->AsInteger;
     Records->Next();

     do{
          current_hour=Records->FieldByName("hour")->AsInteger;
          if(current_hour<previous_hour) Current_day++;
          Records->Edit();
          Records->FieldByName("day")->AsInteger=Current_day;
          Records->Post();
          //ChangeTable(Records->TableName, "cluster", 1, Records->RecNo);
          Records->Next();
          previous_hour=current_hour;
          progress->Value++;
          i++;
          if(i==50){i=0;}

     }while(Records->RecNo < Records->RecordCount-2);
     Records->FetchAll=false;
     Records->Limit=60000;
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::Button1Click(TObject *Sender)
{
MyQuery1->Active=true;
DataSource1->DataSet=MyQuery1;  DataSource1->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::New_table_rawClick(TObject *Sender)
{
   AnsiString New_Table = InputBox("New Raw Table", "Table name must start with a letter, e.g., Bird_109raw:", "");
   new_raw_table(New_Table,slaves->Text.ToInt());
}
//---------------------------------------------------------------------------

int TDataForm::find_age(int bird) // compared to current date!!
{
	 TDate dt;
	 str=Retrieve(bird, "hatching_date");
     ShortDateFormat = "m/d/yyyy";
	 dt=StrToDate(str);//FormatDateTime(str,1);
	 int d=(int)Date() - (int)dt;
	 if(d<0)d=0;
	 return(d);// (int)Date() - (int)dt );
}

///////////////////////////////////////////////////////////////////////////////


bool TDataForm::table_exists(AnsiString table_name)
{
  table_name=table_name.LowerCase();
  showTables();
  DataForm->MyQuery1->First();
  for (int i=0; i < MyQuery1->RecordCount; i++)
  {
     if(table_name==MyQuery1->Fields->Fields[0]->AsString)return(true);
     DataForm->MyQuery1->Next();
  }
  return(false);
}



bool TDataForm::database_exists(AnsiString database_name)
{
  showDatabases();
  DataForm->MyQuery1->First();
  for (int i=0; i < MyQuery1->RecordCount; i++)
  {
     if(database_name==MyQuery1->Fields->Fields[0]->AsString)return(true);
     DataForm->MyQuery1->Next();
  }
  return(false);
}


AnsiString TDataForm::new_raw_table(AnsiString name, int num_slaves)
{
   //static int suffex=1;
   if(!name.Length())
    {MessageDlg("failed to generate raw feature table (empty name string)", mtWarning, TMsgDlgButtons() << mbOK, 0);return("");}
   if(name[1]=='0' || name[1]=='1' || name[1]=='2'|| name[1]=='3'|| name[1]=='4'
       || name[1]=='5'|| name[1]=='6'|| name[1]=='7'|| name[1]=='8'|| name[1]=='9')
       {MessageDlg("Error: table name must start with a letter.", mtWarning, TMsgDlgButtons() << mbOK, 0);return("");}

 //  bool table_type_match=true;
   if(table_exists(name))
   {
		if(chans_exist(name, num_slaves)) return(name);
		else {MessageDlg("Existing table does not contain enough fields for the slave channels.", mtWarning, TMsgDlgButtons() << mbOK, 0);return("");}
		//table_type_match=false; name+=suffex; suffex++; } // in this case, we are going to generate automatically a new and matching table
   }

  // if(!table_type_match)
   {
	 str="CREATE TABLE IF NOT EXISTS ";
     str+=name;
     //str+=" SELECT * FROM features_template";
     str+=" (  `time` int(9) NOT NULL default '0',";
     str+="`file_index` int(7) NOT NULL default '0',";
	 str+="`amplitude` smallint(6) NOT NULL default '0',";
	 str+="`mean_frequency_amp` smallint(6) NOT NULL default '0',";
	 str+="`pitch` smallint(6) NOT NULL default '0',";
	 str+="`mean_frequency` smallint(6) NOT NULL default '0',";
	 str+="`FM` smallint(6) NOT NULL default '0',";
	 str+="`am` smallint(6) NOT NULL default '0',";
	 str+="`goodness` smallint(6) NOT NULL default '0',";
	 str+="`entropy` smallint(6) NOT NULL default '0',";
	 str+="`peak_frequency` smallint(6) NOT NULL default '0',";
	 str+="`DAS` smallint(6) NOT NULL default '0',";
	 str+="`continuity_t` smallint(6) NOT NULL default '0',";
	 str+="`continuity_f` smallint(6) NOT NULL default '0',";
     if(num_slaves)
     {
       for(int i=1;i<=num_slaves;i++)
       {
        str+="`s"; str+=i; str+="_amplitude` smallint(6) NOT NULL default '0',";
        str+="`s"; str+=i; str+="_entropy` smallint(6) NOT NULL default '0',";
        str+="`s"; str+=i; str+="_PeakFr` smallint(6) NOT NULL default '0',";
        str+="`s"; str+=i; str+="_PeakFr_power` smallint(6) NOT NULL default '0',";
        str+="`s"; str+=i; str+="_highpass_power` smallint(6) NOT NULL default '0',";
       }
	 }
    /*
     if(num_slaves>0)str+="`s1` smallint(6) NOT NULL default '0',";
     if(num_slaves>1)str+="`s2` smallint(6) NOT NULL default '0',";
     if(num_slaves>2)str+="`s3` smallint(6) NOT NULL default '0',";
     if(num_slaves>3)str+="`s4` smallint(6) NOT NULL default '0',";
     if(num_slaves>4)str+="`s5` smallint(6) NOT NULL default '0',";
     if(num_slaves>5)str+="`s6` smallint(6) NOT NULL default '0',";
     if(num_slaves>6)str+="`s7` smallint(6) NOT NULL default '0',";
     if(num_slaves>7)str+="`s8` smallint(6) NOT NULL default '0',";
     if(num_slaves>8)str+="`s9` smallint(6) NOT NULL default '0',";
     if(num_slaves>9)str+="`s10` smallint(6) NOT NULL default '0',";     */
     str+="PRIMARY KEY(`file_index`, `time`));";
     //str+=")";// ENGINE=MyISAM DEFAULT CHARSET=latin1;";
     MyCommand1->SQL->Clear();
     MyCommand1->SQL->Add(str);
     MyCommand1->Execute(1);
   }
   return(name);
}




// check features table to see if the expected slave channel fields exist, very light test...
int TDataForm::chans_exist(AnsiString tbl_name, int num_slaves)
{
  AnsiString tbl_name1;
  if(!num_slaves)return(true); // trivial, no slaves are asked for!
  tbl_name1=Records->TableName;
  //TMyTable *tbl=TheTable; //remember...
  Records->TableName=tbl_name;
  Records->Active=true;
  if(Records->Fields->Count<14+num_slaves) // not enough channels!
  {
    Records->Active=false;
    Records->TableName=tbl_name1;
    Records->Active=true;
    return(false);
  }///TheTable->Fields->Fields[10]->FieldName;
  else
  {
        int chans=(Records->Fields->Count-14)/5;
        Records->Active=false;
        Records->TableName=tbl_name1;
        Records->Active=true;
        return(chans);
  }// back to where we started...
}



void TDataForm::Change_Database(AnsiString name)
{
  MyConnection1->Database=name;
}

 void TDataForm::create_database(AnsiString name)
 {
     str="CREATE DATABASE ";
     str+=name;
     MyCommand1->SQL->Clear();
     MyCommand1->SQL->Add(str);
     MyCommand1->Execute(1);
 }


/////////////////////////////////////////////////////////////////////////////////////////////////////

void TDataForm::create_settings(AnsiString settings_table)
{
  settings->Active=false;
  str="CREATE TABLE ";//`settings` ( `num` tinyint(4) NOT NULL auto_increment,";
  str+=settings_table;
  str+=" ( `num` tinyint(4) NOT NULL auto_increment,";
  str+="`bird_ID` char(10) NOT NULL default '99',";
  str+=" `hatching_date` date NOT NULL default '2010-01-01',";
  str+="`start_training` date NOT NULL default '2010-01-01',";
  str+="  `stop_training` date NOT NULL default '2010-01-01',";
  str+="`training` tinyint(4) NOT NULL default '0',";
  str+="   `pecks` smallint(6) NOT NULL default '0',";
  str+="`song_quota` smallint(6) NOT NULL default '10',";
  str+=" `input_folder` varchar(50) NOT NULL default 'c:\\\\sap',";
  str+="`sound_folder_size` smallint(6) NOT NULL default '0',";
  str+=" `output_folder_sound` varchar(50) NOT NULL default 'c:\\\\sap\\\\out1',";
  str+="`output_folder_features` varchar(50) NOT NULL default 'recs0_features',";
  str+=" `syllable_table` varchar(50) NOT NULL default 'syll_R0',";
  str+="`entropy_thresh` smallint(6) NOT NULL default '282',";
  str+=" `deriv_thresh` smallint(6) NOT NULL default '100',";
  str+=" `contrast_thresh` smallint(6) NOT NULL default '300',";  // contours contast
  str+="`syllable_duration_reject` smallint(6) NOT NULL default '300',";
  str+=" `bout_duration_reject` smallint(6) NOT NULL default '0',";
  str+="`number_of_syllables_reject` smallint(3) unsigned NOT NULL default '255',";
  str+=" `display_contrast` smallint(3) unsigned NOT NULL default '4500',";
  str+="`fine_segmentation` tinyint(4) NOT NULL default '0',";
  str+=" `hp_small` smallint(6) NOT NULL default '50',";
  str+=" `hp_big` smallint(6) NOT NULL default '500',";
  str+="`select_list1` tinyint(4) NOT NULL default '1',";
  str+="`quota` tinyint(4) NOT NULL default '1',"; // check -- see song quota...
  str+="`select_list2` tinyint(4) NOT NULL default '1',";
  str+="`passive_playbacks` tinyint(4) NOT NULL default '0', ";
  str+="`passive_odds` float NOT NULL default '0',";
  str+="`passive_quota` smallint(6) NOT NULL default '10',";
  str+="`passive_quota_check` tinyint(4) NOT NULL default '0',";
  str+="`bio_feedback` tinyint(4) NOT NULL default '0',";
  str+="`update` tinyint(4) NOT NULL default '0',";
  str+="`save` tinyint(4) NOT NULL default '0',";
  str+="`slave_output` tinyint(4) NOT NULL default '1',";
  str+="`advance_window` smallint(6) NOT NULL default '44',";
  str+="`FFT_window` smallint(6) NOT NULL default '441',";
  str+="`frequency_range` smallint(6) NOT NULL default '2',";
  str+="`active_or_slave` tinyint(4) NOT NULL default '0',";
  str+="`pri_segmented_feature` tinyint(4) NOT NULL default '0',";
  str+="`adapt_thresh` tinyint(4) NOT NULL default '0',";
  str+="`smooth_feature` tinyint(4) NOT NULL default '0',";
  str+="`pri_lt` tinyint(4) NOT NULL default '0',";
  str+="`sec_segmentation` tinyint(4) NOT NULL default '0',";
  str+="`sec_segmented_feature` tinyint(4) NOT NULL default '0',";
  str+="`sec_lt` tinyint(4) NOT NULL default '0',";
  str+="`sec_logic` tinyint(4) NOT NULL default '0',";
  //str+="`pagination` tinyint(4) NOT NULL default '0',";
  str+="`stop_duration_thresh` smallint(6) NOT NULL default '7',";
  str+="`bout_duration_thresh` smallint(6) NOT NULL default '100',";
  str+=" `sonogram_folder` varchar(50) NOT NULL default 'c:\\\\sap',";
  str+="`pagination_thresh` smallint(6) NOT NULL default '7',";
  str+="`tabs` smallint(6) NOT NULL default '0',";
  str+="`tabs2` smallint(6) NOT NULL default '0',";
  str+="`pagination` tinyint(6) NOT NULL default '0',";
  str+="PRIMARY KEY  (`num`))";
  MyCommand1->SQL->Clear();
  MyCommand1->SQL->Add(str);
  MyCommand1->Execute(1);

  settings->Active=true;
  settings->Refresh();

  settings->First();
  for(int i=0;i<13;i++)
  {
	settings->Edit();
	settings->Append();
	settings->Post();
  }

  settings->First();
  str="c:\\sap\\";
  for(int i=0;i<13;i++)
  {
	 settings->Edit();
	 settings->FieldValues["bird_ID"]=i;
	 settings->FieldValues["hatching_date"]="2010-01-02";
	 settings->FieldValues["start_training"]="2010-02-02";
	 settings->FieldValues["stop_training"]="2010-03-02";
	 settings->FieldValues["quota"]=10;
	 str2=str+"in"; str2+=max(i-1,0); str2+="\\";
	 if(i>0)settings->FieldValues["input_folder"]=str2;
	 str2=str+"out"; str2+=max(i-1,0); str2+="\\";
	 settings->FieldValues["output_folder_sound"]=str2;
	 settings->Post();
	 settings->Next();
  }

  Settings(1, "FFT_window", 441);
  Settings(2, "FFT_window", 441);
  Settings(1, "advance_window", 44);
  Settings(2, "advance_window", 44);
  Settings(1, "frequency_range", 2);
  Settings(1, "hp_big", 500);
  Settings(1, "hp_small", 50);

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////


void TDataForm::create_similarity(AnsiString similarity_table)
{
  similarity->Active=false;
  str="CREATE TABLE IF NOT EXISTS ";//`similarity` (`recnum` int(11) NOT NULL auto_increment,";
  str+=similarity_table;
  str+=" (`recnum` int(11) NOT NULL auto_increment,";
  str+="`song1` varchar(30) NOT NULL default '', `song2` varchar(100) NOT NULL default '',";
  str+="`start1` float NOT NULL default '0', `end1` float NOT NULL default '0',";
  str+="`start2` float NOT NULL default '0', `end2` float NOT NULL default '0',";
  str+="`score` int(11) NOT NULL default '0', `similarity` float NOT NULL default '0',";
  str+="`accuracy` float NOT NULL default '0', `Kolmogorov_Smirnov` float NOT NULL default '0',";
  str+="`period` int(11) NOT NULL default '0',`random_period` int(11) NOT NULL default '0',";
  str+="`comments` varchar(15) NOT NULL default '',`sequential_match` float NOT NULL default '0',";
  str+="PRIMARY KEY(`recnum`));";
  MyCommand1->SQL->Clear();
  MyCommand1->SQL->Add(str);
  MyCommand1->Execute(1);
}

void TDataForm::create_durations()
{
  similarity->Active=false;
  str="CREATE TABLE IF NOT EXISTS `state_duration` (`duration` int(11) NOT NULL default '0');";
  MyCommand1->SQL->Clear();
  MyCommand1->SQL->Add(str);
  MyCommand1->Execute(1);
}


void __fastcall TDataForm::open_SimilarityClick(TObject *Sender)
{
    showTables();
    Application->CreateForm(__classid(TtableDialog), &tableDialog);
    tableDialog->show_type->ItemIndex=4;
    tableDialog->show_typeClick(this);
    if (tableDialog->ShowModal() == mrOk)
    {
      TheTable->Active=false;
      TheTable->TableName= tableDialog->tables->Items->Strings[tableDialog->tables->ItemIndex];
      TheTable->Active=true;
      CountRecsClick(this);
      table_name->Caption=TheTable->TableName;
      MyQuery1->Active=false;

      if(show_table){DataSource1->DataSet=TheTable;  DataSource1->Enabled=true;}
      else DataSource1->Enabled=false;
    }
}




//---------------------------------------------------------------------------

void __fastcall TDataForm::DirectoryListBox1Change(TObject *Sender)
{
 if(FileListBox1->Items->Strings[0]=="[..]" || FileListBox1->Items->Strings[0]=="[.]")FileListBox1->Items->Delete(0);
 if(FileListBox1->Items->Strings[0]=="[..]" || FileListBox1->Items->Strings[0]=="[.]")FileListBox1->Items->Delete(0);
 if(FileListBox1->Items->Count)
 {
   FileListBox1->ItemIndex=0;
   FileListBox1Click(this);
   FileListBox2->Directory=Edit1->Text;
 }
 else FileListBox2->Clear();
}

//---------------------------------------------------------------------------

void __fastcall TDataForm::FileListBox1Click(TObject *Sender)
{
  AnsiString str=FileListBox1->FileName;
  int i=1; do{if(str[i]=='[' || str[i]==']') str.Delete(i,1); i++;}while(i<=str.Length());
  str+="\\";
  Edit1->Text=str;
  FileListBox2->ItemIndex=0;
  FileListBox2->Directory=Edit1->Text;
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::FileListBox1Change(TObject *Sender)
{
  FileListBox2->Directory=Edit1->Text;
  FileListBox2->ItemIndex=0;
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::dailyfoldersClick(TObject *Sender)
{
 //if idle...
  AnsiString destination_folder;
  TDateTime file_age;
  int bird_age;
  if(FileListBox2->Items->Count)FileListBox2->ItemIndex=0;
  do{
    int num=FileListBox2->Items->Count;
    FileListBox2->Update();

    if(FileListBox2->Items->Count)
    {
    FileListBox2->ItemIndex=0;
    for(int i=0;i<num;i++)
    {
    //  FileListBox2->Update();
    //  if(!FileListBox2->Items->Count)break;//FileListBox2->ItemIndex++;
    //  FileListBox2->ItemIndex=0;
	  file_age=file_age.FileDateToDateTime(FileAge(FileListBox2->FileName));
      bird_age=file_age-hatched->Date;  // file_age is when recording occured
      if(bird_age<1){MessageDlg("hatching date must be wrong!", mtWarning, TMsgDlgButtons() << mbOK, 0); return;}
      destination_folder=DirectoryListBox2->Directory+"\\"+bird_age+"\\";
      if(!DirectoryExists(destination_folder))if(!ForceDirectories(destination_folder))
         {MessageDlg("failed to create folder!", mtWarning, TMsgDlgButtons() << mbOK, 0); return;}
      RenameFile(FileListBox2->FileName, destination_folder+ExtractFileName(FileListBox2->FileName));
      FileListBox2->ItemIndex++;
    }
    }
  if(FileListBox1->ItemIndex==FileListBox1->Items->Count-1)break;
  FileListBox1->ItemIndex++;
  FileListBox1Click(this);
  Edit1->Update();
  //FileListBox1->Update();
  FileListBox2->Update();
  Edit1->Text;
  //int xx=FileListBox2->Items->Count;
  }while(1);
  FileListBox2->Update();
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::new_brain_tableClick(TObject *Sender)
{
    AnsiString New_Table = InputBox("New Brain Table", "Table name must start with a letter, e.g., BirdBrain_109:", "");
    if(!New_Table.Length())return;
    if(New_Table[1]=='0' || New_Table[1]=='1' || New_Table[1]=='2'|| New_Table[1]=='3'|| New_Table[1]=='4'
       || New_Table[1]=='5'|| New_Table[1]=='6'|| New_Table[1]=='7'|| New_Table[1]=='8'|| New_Table[1]=='9')
       {MessageDlg("Error: table name must start with a letter.", mtWarning, TMsgDlgButtons() << mbOK, 0);return;}
    Create_Brain_syllable_table(New_Table);
    TheTable->Active=false;
    TheTable->TableName=New_Table;
    TheTable->Active=true;
    CountRecsClick(this);
    table_name->Caption=TheTable->TableName;
    if(show_table){DataSource1->DataSet=TheTable;  DataSource1->Enabled=true;}
    else DataSource1->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::delTableClick(TObject *Sender)
{
   AnsiString table_name=InputBox("Delete Table", "Table name must be accurately typed!", "");
   DeleteTable(table_name);
}

void TDataForm::DeleteTable(AnsiString table_name)
{
    AnsiString str="DROP TABLE ";
    str+=table_name;
    DataForm->MyCommand1->SQL->Clear();
    DataForm->MyCommand1->SQL->Add(str);
    DataForm->MyCommand1->Execute(1);  
}
//---------------------------------------------------------------------------

bool TDataForm::brain_format(AnsiString table_name)
{
  bool match=false;
  str="SHOW COLUMNS FROM "; str+=table_name;
  MyQuery1->SQL->Clear();
  MyQuery1->SQL->Add(str);
  MyQuery1->Execute();
  for(int i=0; i<MyQuery1->RecordCount; i++)
  {
     str=MyQuery1->Fields->Fields[0]->AsString;
     if(str=="ch1_entropy") {match=true; break;}
     MyQuery1->Next();
  }
  return(match);
}



// insert a record into the file table
int TDataForm::File_to_table(AnsiString bird_name, AnsiString SoundFile, double file_age, int bird_age, int chan)
{
  //File_index++;
  str="INSERT INTO ";
  str+=file_table_name;
  str+= " (bird_ID, file_index, file_name, file_age, bird_age)VALUES('";
  str+=bird_name; str+="',";
  str+= file_indexes[chan]; str+=", '";
  str+=ExtractFileName(SoundFile) ; str+="' ,";
  str+=file_age  ; str+=",";
  str+=bird_age;
  str+=")";
  file_indexes[chan]++;
  MyCommand1->SQL->Clear();
  MyCommand1->SQL->Add(str);
  MyCommand1->Execute(1);
  MyCommand1->UnPrepare();
  return(file_indexes[chan]-1);
}



bool TDataForm::Create_File_Table()
{
  str="CREATE TABLE IF NOT EXISTS `";
  str+=file_table_name;
  str+="` (";
  str+="`bird_ID` char(10) NOT NULL default '0',";
  str+="`file_index` int(11) NOT NULL default '0',";
  str+="`file_name` char(60) NOT NULL default '',";
  str+="`file_age` double NOT NULL default '0',";
  str+="`bird_age` float(9,4) NOT NULL default '0');";
  //str+=", PRIMARY KEY  (`file_index`,`bird_ID`));";
  MyCommand1->SQL->Clear();
  MyCommand1->SQL->Add(str);
  MyCommand1->Execute(1);
  return(1);
}



void __fastcall TDataForm::new_file_tableBClick(TObject *Sender)
{
  AnsiString New_Table = InputBox("New Table", "Table name must start with a letter, e.g., Bird_109:", "");
    if(!New_Table.Length())return;
    if(New_Table[1]=='0' || New_Table[1]=='1' || New_Table[1]=='2'|| New_Table[1]=='3'|| New_Table[1]=='4'
       || New_Table[1]=='5'|| New_Table[1]=='6'|| New_Table[1]=='7'|| New_Table[1]=='8'|| New_Table[1]=='9')
       {MessageDlg("Error: table name must start with a letter.", mtWarning, TMsgDlgButtons() << mbOK, 0);return;}
   file_table_name=New_Table;
   Create_File_Table();
   table_name->Caption=file_table_name;
}
//---------------------------------------------------------------------------



// usage: initiate_handler is followed by get_recs(say, 2334). the results are stored in myqeury.

void TDataForm::initiate_handler(AnsiString database, AnsiString table)
 {
        // open databse
        str="use ";
        str+=database;
        MyCommand1->SQL->Clear();
        MyCommand1->SQL->Add(str);
        MyCommand1->Execute(1);


        // open the table
        str="HANDLER `";
        str+=table;
        str+="` open;";
        MyCommand1->SQL->Clear();
        MyCommand1->SQL->Add(str);
        try
        {
           MyCommand1->Execute(1);
        }
        catch(...)
        {
          str="HANDLER `";
          str+=table;
          str+="` close;";
          MyCommand1->SQL->Clear();
          MyCommand1->SQL->Add(str);
          MyCommand1->Execute(1);

          str="HANDLER `";
          str+=table;
          str+="` open;";
          MyCommand1->SQL->Clear();
          MyCommand1->SQL->Add(str);
          MyCommand1->Execute(1);
        }

        // go to the begining
        str="HANDLER `";
        str+=table;
        str+="` read first limit 1;";
        MyCommand1->SQL->Clear();
        MyCommand1->SQL->Add(str);
        MyCommand1->Execute(1);
}


// usage: initiate_handler is followed by get_recs(say, 2334). the results are stored in myqeury.
 void TDataForm::get_recs(AnsiString table, int amount)
 {
    str="HANDLER `";
    str+=table;
    str+="` read next limit ";
    str+=amount; str+=";";
    MyQuery1->SQL->Clear();
    MyQuery1->SQL->Add(str);
    MyQuery1->Execute();
    MyQuery1->First();
    MyQuery1->RecordCount;
 }




AnsiString TDataForm::GetFileName(AnsiString table_name, int index)
{
    AnsiString q;
  //  if(!table_exists(table_name))return("file table not found");
    q="SELECT file_name FROM ";
    q+=table_name;
    q+=" where file_index=";
    q+=index;
 //   MyQuery1->Active=true;  SHOW [FULL] COLUMNS FROM tbl_name
    MyQuery3->SQL->Clear();
    MyQuery3->SQL->Add(q);
    MyQuery3->Execute();
  //  if(!MyQuery3->RecordCount)return("no file matching the index");
    q=MyQuery3->Fields->Fields[0]->AsString;
    return(q);
}


double TDataForm::GetFile_serial(AnsiString table_name, int index)
{
    AnsiString q;
    double serial_number;
   // if(!table_exists(table_name))return(0);
    q="SELECT file_age FROM ";
    q+=table_name;
    q+=" where file_index=";
    q+=index;
    MyQuery3->SQL->Clear();
    MyQuery3->SQL->Add(q);
    MyQuery3->Execute();
 //   if(!MyQuery3->RecordCount)return(0);
    serial_number=MyQuery3->Fields->Fields[0]->AsFloat;
    return(serial_number);
}



void __fastcall TDataForm::open_FILE_tableClick(TObject *Sender)
{
   Application->CreateForm(__classid(TtableDialog), &tableDialog);
   tableDialog->show_type->ItemIndex=3;
   tableDialog->show_typeClick(this);
    if (tableDialog->ShowModal() == mrOk)
    {
      if(tableDialog->tables->ItemIndex==-1)return;
      file_table_name=tableDialog->tables->Items->Strings[tableDialog->tables->ItemIndex];
      MyQuery1->Active=false;
      table_name->Caption=file_table_name;
    }

    TheTable->Active=false;
    TheTable->TableName= file_table_name;
   TheTable->Active=true;
   if(show_table){DataSource1->DataSet=TheTable;  DataSource1->Enabled=true;}
   else DataSource1->Enabled=false;

}

//---------------------------------------------------------------------------


void __fastcall TDataForm::hatchedClick(TObject *Sender)
{
  str2="";
  age->Value=int(Date()-hatched->Date);        
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::Timer1Timer(TObject *Sender)
{
date_Label->Caption=FormatDateTime("dddd, mmmm d, yyyy ' at ' hh:mm:ss AM/PM", Now());

}
//---------------------------------------------------------------------------

void __fastcall TDataForm::DBNavigator1Click(TObject *Sender,
      TNavigateBtn Button)
{
 try{bird_update();}
  catch(...){Application->MessageBox(L"bird_update function failed in records module", NULL, MB_OK);}
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::procedure1_scheduleChange(TObject *Sender)
{
        procedure1_age->Value=procedure1_schedule->Date-hatched->Date;
        if(procedure1_age->Value<0)procedure1_age->Value=0;        
}

//---------------------------------------------------------------------------

void __fastcall TDataForm::trainedClick(TObject *Sender)
{
  training->Value=trained->Date-hatched->Date;
  if(training->Value<0)training->Value=0;        
}

//---------------------------------------------------------------------------

void __fastcall TDataForm::procedure2_scheduleClick(TObject *Sender)
{
        procedure2_age->Value=procedure2_schedule->Date-hatched->Date;
        if(procedure2_age->Value<0)procedure2_age->Value=0;
}

//---------------------------------------------------------------------------


void __fastcall TDataForm::Add_experiment_recordClick(TObject *Sender)
{
   AnsiString a1, a2, a3, a4, a5, a6,a7,a8,a9;
   a1= FormatDateTime(Date(),1);
   a2=FormatDateTime(Time(),4);
   a3=bird_name->Field->AsString;
   a4=action_ID->Field->AsString;
   a5=action_typeLb->Field->AsString;
   a6=resource_location->Text;
   a7=file1->Text;
   a8=file2->Text;
   a9=comments->Text;
   experiments->AppendRecord(ARRAYOFCONST((Null, a1, a2, a3, a4, a5, a6, a7, a8, a9)));
}

//---------------------------------------------------------------------------

void __fastcall TDataForm::update_tableClick(TObject *Sender)
{
  AnsiString a1=name->Text;

    // add or modify?
   str="SELECT COUNT(*) FROM animals where bird_name='"; str+=a1; str+="';";
   MyQuery1->SQL->Clear();
   MyQuery1->SQL->Add(str);
   MyQuery1->Execute();
   int recNum=MyQuery1->Fields->Fields[0]->AsInteger;

   if(recNum)// modify!
   {
        birds->Edit();
        birds->FieldValues["bird_name"]=name->Text;
        if(gender->Text=="male")birds->FieldValues["sex"]="M"; else birds->FieldValues["sex"]="F";
        birds->FieldValues["weight"]=weight->Text;
        birds->FieldValues["location"]=cage->Text;
        birds->FieldValues["model"]=song->Text;
        birds->FieldValues["quota"]=song_quota->Text;
        birds->FieldValues["details"]=bird_comments->Text;
        birds->FieldValues["hatched"]=hatched->Date;
        birds->FieldValues["training"]=trained->Date;
        birds->FieldValues["procedure1"]=procedure1->Text;
        birds->FieldValues["procedure2"]=procedure2->Text;
        birds->FieldValues["procedure3"]=procedure3->Text;
        birds->FieldValues["proc1_date"]=procedure1_schedule->Date;
        birds->FieldValues["proc2_date"]=procedure2_schedule->Date;
        birds->FieldValues["proc3_date"]=procedure3_schedule->Date;
        birds->FieldValues["audio"]=audio->Text;
        birds->FieldValues["attributes"]=attributes->Text;
        birds->FieldValues["photoperiod"]=photoperiod->Text;

        try{
        birds->Post();
        }catch(...)
        {

         Beep();Beep();Beep();
         str="The active record in the table above is not the same record you are editing. This is probably because you were ";
         str+="browsing the table using the back-arrow keys, which did not properly update the Edit fields. ";
         str+="SAP2 now fixed this, but you need to edit the field again. If the active record is incorrect, use the mouse to point to the correct record in the table.";
         MessageDlg(str, mtError, TMsgDlgButtons() << mbOK, 0);
         //birds_navigator->BtnClick(nbCancel);
         return;
        }
        Beep();
   }
      
   
   else
   {
       AnsiString a2, a3, a4, a5, a6,a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19;
       ShortDateFormat = "m/d/yyyy";//"yyyy-m-d";
       if(gender->Text=="male")a2="M"; else a2="F";
       a3=DateToStr(hatched->Date);  ////
       a4=weight->Text;
       a5=cage->Text;
       a6=procedure1->Text;
       a7= DateToStr(procedure1_schedule->Date);
       a8=procedure2->Text;
       a9= DateToStr(procedure2_schedule->Date);
       a10=procedure3->Text;
       a11= DateToStr(procedure3_schedule->Date);
       a12=DateToStr(trained->Date);
       //a13=(int)training->Value;
       a14=song->Text;
       a15=song_quota->Text;
       a16= bird_comments->Text;
       a17=audio->Text;
       a18=attributes->Text;
       a19=photoperiod->Text;

       birds->AppendRecord(ARRAYOFCONST((a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a14, a15, a17, a18, a19, a16)));
       Beep();
       ShortDateFormat = "yyyy-m-d";
   }
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::add_actionClick(TObject *Sender)
{
   AnsiString a1, a2, a3, a4, a5, a6, a7, a8, a9, a10;
   a1=action_type->Text;
   a2=attribute1_name->Text;
   a3=attribute1_val->Text;
   a4= attribute2_name->Text;
   a5=attribute2_val->Text;
   a6= attribute3_name->Text;
   a7=attribute3_val->Text;
   a8= attribute4_name->Text;
   a9=attribute4_val->Text;
   a10= action_comments->Text;
   actions->AppendRecord(ARRAYOFCONST((Null, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)));
}
//---------------------------------------------------------------------------

void TDataForm::bird_update()
{
       // allow_changesClick(this);
        hatched->Date=birds->FieldByName("hatched")->AsDateTime;
        age->Value=int(Date()-hatched->Date);
        age_lb->Value=age->Value;
        procedure1->Text=birds->FieldByName("procedure1")->AsString;
        procedure1_schedule->Date=birds->FieldByName("proc1_date")->AsDateTime;
        procedure1_age->Value=procedure1_schedule->Date-hatched->Date;
        procedure1_time->Time=birds->FieldByName("proc1_date")->AsDateTime;
        if(procedure1_age->Value<0)procedure1_age->Value=0;

        procedure2->Text=birds->FieldByName("procedure2")->AsString;
        procedure2_schedule->Date=birds->FieldByName("proc2_date")->AsDateTime;
        procedure2_time->Time=birds->FieldByName("proc2_date")->AsDateTime;
        //procedure2_schedule->Date=birds->FieldValues["procedure2_date"];
        procedure2_age->Value=procedure2_schedule->Date-hatched->Date;
        if(procedure2_age->Value<0)procedure2_age->Value=0;

        procedure3->Text=birds->FieldByName("procedure3")->AsString;
        procedure3_schedule->Date=birds->FieldByName("proc3_date")->AsDateTime;
        procedure3_time->Time=birds->FieldByName("proc3_date")->AsDateTime;
        //procedure3_schedule->Date=birds->FieldValues["procedure3_date"];
        procedure3_age->Value=procedure3_schedule->Date-hatched->Date;
        if(procedure3_age->Value<0)procedure3_age->Value=0;

        trained->Date=birds->FieldByName("training")->AsDateTime;
        train_time->Time=birds->FieldByName("training")->AsDateTime;
        //trained->Date=birds->FieldValues["start_training"];
        training->Value=trained->Date-hatched->Date;
        if(training->Value<0)training->Value=0;

        audio->Text=birds->FieldByName("audio")->AsString;
        attributes->Text=birds->FieldByName("attributes")->AsString;
        photoperiod->Text=birds->FieldByName("photoperiod")->AsString;

        name->Text=birds->FieldByName("bird_name")->AsString;//FieldValues["bird_name"];
        str=birds->FieldByName("sex")->AsString;//FieldValues["gender"];
        if(str=="M")gender->ItemIndex=0; else gender->ItemIndex=1;
        weight->Text=birds->FieldByName("weight")->AsString;;
        cage->Text=birds->FieldByName("location")->AsString;
        song->Text=birds->FieldByName("model")->AsString;
        song_quota->Text=birds->FieldByName("quota")->AsString;
        bird_comments->Text=birds->FieldByName("details")->AsString;
}



void TDataForm::actions_update()
{
       // allow_action_changesClick(this);
        action_type->Text=actions->FieldByName("type")->AsString;
        attribute1_name->Text=actions->FieldByName("attribute1_name")->AsString;
        attribute1_val->Text=actions->FieldByName("attribute1_value")->AsString;
        attribute2_name->Text=actions->FieldByName("attribute2_name")->AsString;
        attribute2_val->Text=actions->FieldByName("attribute2_value")->AsString;
        attribute3_name->Text=actions->FieldByName("attribute3_name")->AsString;
        attribute3_val->Text=actions->FieldByName("attribute3_value")->AsString;
        attribute4_name->Text=actions->FieldByName("attribute4_name")->AsString;
        attribute4_val->Text=actions->FieldByName("attribute4_value")->AsString;
        action_comments->Text=actions->FieldByName("details")->AsString;
}



void TDataForm::animal_actions_update()
{
       comments->Text=experiments->FieldByName("details")->AsString;
       resource_location->Text=experiments->FieldByName("resource_location")->AsString;
       file1->Text=experiments->FieldByName("file1")->AsString;
       file2->Text=experiments->FieldByName("file2")->AsString;
       if(datapages->ActivePageIndex==2 && !allow_action_animal_changes->Down)
        {
          TLocateOptions boob;
          birds->Locate("bird_name", experiments->FieldByName("bird_name")->AsString ,boob);
          actions->Locate("action_ID", experiments->FieldByName("action_ID")->AsString ,boob);
        }
       
}





void __fastcall TDataForm::procedure3_scheduleClick(TObject *Sender)
{
       procedure3_age->Value=procedure3_schedule->Date-hatched->Date;
        if(procedure3_age->Value<0)procedure3_age->Value=0;
        
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::animals_gridCellClick(TColumn *Column)
{
 bird_update();
}
//---------------------------------------------------------------------------



void __fastcall TDataForm::allow_changesClick(TObject *Sender)
{
if(allow_changes->Down)
  {
    // messages->Caption="Only mouse click will refresh display. Validate that display matches  table pointer before updating.";
     edit_box->Visible=true;
     update_table->Enabled=true;

  }
  else
  {
    // messages->Caption="Navigate with arrow && up/down keys. Click field names to sort. Use search after sorting.";
     edit_box->Visible=false;
      update_table->Enabled=false;
  }
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::set_time2Click(TObject *Sender)
{
trained->Date=Date();
  trained->Time=Time();
  train_time->Time=Time();        
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::set_time1Click(TObject *Sender)
{
hatched->Date=Date();        
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::set_time3Click(TObject *Sender)
{
procedure1_schedule->Date=Date();
  procedure1_schedule->Time=Time();
  procedure1_time->Time=Time();        
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::set_time4Click(TObject *Sender)
{
  procedure2_schedule->Date=Date();
  procedure2_schedule->Time=Time();
  procedure2_time->Time=Time();        
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::set_time5Click(TObject *Sender)
{
  procedure3_schedule->Date=Date();
  procedure3_schedule->Time=Time();
  procedure3_time->Time=Time();        
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::procedure1_timeChange(TObject *Sender)
{
   procedure1_schedule->Time=procedure1_time->Time;        
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::procedure2_timeChange(TObject *Sender)
{
 procedure2_schedule->Time=procedure2_time->Time;                
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::procedure3_timeChange(TObject *Sender)
{
procedure3_schedule->Time=procedure3_time->Time;                
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::train_timeChange(TObject *Sender)
{
trained->Time=train_time->Time;        
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::animals_gridDrawColumnCell(TObject *Sender,
      const TRect &Rect, int DataCol, TColumn *Column,
      TGridDrawState State)
{
if(!allow_changes->Down) bird_update();
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::animals_gridTitleClick(TColumn *Column)
{
// messages->Caption="Click field name above to sort, then search (case sensitive). To filter click 'enter', then 'Apply filter'.";
}
//---------------------------------------------------------------------------



void __fastcall TDataForm::allow_action_changesClick(TObject *Sender)
{
   if(allow_action_changes->Down)
  {
     actions_grid->Color=clWhite;
     action_type->Color=clWhite;
     action_type->ReadOnly=false;
     attribute1_name->Color=clWhite;
     attribute1_name->ReadOnly=false;
     attribute2_name->Color=clWhite;
     attribute2_name->ReadOnly=false;
     attribute3_name->Color=clWhite;
     attribute3_name->ReadOnly=false;
     attribute4_name->Color=clWhite;
     attribute4_name->ReadOnly=false;
     attribute1_val->Color=clWhite;
     attribute1_val->ReadOnly=false;
     attribute2_val->Color=clWhite;
     attribute2_val->ReadOnly=false;
     attribute3_val->Color=clWhite;
     attribute3_val->ReadOnly=false;
     attribute4_val->Color=clWhite;
     attribute4_val->ReadOnly=false;
     //action_file_name->Color=clWhite;
    // action_file_name->ReadOnly=false;
     action_comments->Color=clWhite;
     action_comments->ReadOnly=false;
     add_action->Enabled=true;
     update_action->Enabled=true;
  }

  else
  {
     actions_grid->Color=clScrollBar;
     action_type->Color=clScrollBar;
     action_type->ReadOnly=true;
     attribute1_name->Color=clScrollBar;
     attribute1_name->ReadOnly=true;
     attribute2_name->Color=clScrollBar;
     attribute2_name->ReadOnly=true;
     attribute3_name->Color=clScrollBar;
     attribute3_name->ReadOnly=true;
     attribute4_name->Color=clScrollBar;
     attribute4_name->ReadOnly=true;
     attribute1_val->Color=clScrollBar;
     attribute1_val->ReadOnly=true;
     attribute2_val->Color=clScrollBar;
     attribute2_val->ReadOnly=true;
     attribute3_val->Color=clScrollBar;
     attribute3_val->ReadOnly=true;
     attribute4_val->Color=clScrollBar;
     attribute4_val->ReadOnly=true;
    // action_file_name->Color=clScrollBar;
    // action_file_name->ReadOnly=true;
     action_comments->Color=clScrollBar;
     action_comments->ReadOnly=true;
     add_action->Enabled=false;
     update_action->Enabled=false;
  }
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::actions_gridCellClick(TColumn *Column)
{
  actions_update();
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::actions_gridDrawColumnCell(TObject *Sender,
      const TRect &Rect, int DataCol, TColumn *Column,
      TGridDrawState State)
{
  if(!allow_action_changes->Down) actions_update();        
}
//---------------------------------------------------------------------------




void __fastcall TDataForm::update_actionClick(TObject *Sender)
{

        actions->Edit();
        actions->FieldValues["type"]=action_type->Text;
        actions->FieldValues["attribute1_name"] =attribute1_name->Text;
        actions->FieldValues["attribute1_value"]=attribute1_val->Text;
        actions->FieldValues["attribute2_name"] =attribute2_name->Text;
        actions->FieldValues["attribute2_value"]=attribute2_val->Text;
        actions->FieldValues["attribute3_name"] =attribute3_name->Text;
        actions->FieldValues["attribute3_value"]=attribute3_val->Text;
        actions->FieldValues["attribute4_name"] =attribute4_name->Text;
        actions->FieldValues["attribute4_value"]=attribute4_val->Text;
		actions->FieldValues["details"]=action_comments->Text;
        try{
		actions->Post();
		}catch(...)
        {
         return;
        }
        Beep();
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::allow_action_animal_changesClick(
      TObject *Sender)
{
  if(allow_action_animal_changes->Down)
  {
     action_animal_grid->Color=clWhite;
     animal_pannel->Color=clWhite;
     action_pannel->Color=clWhite;
     age_lb->ColorLabBakG=clWhite;
     comments->Color=clWhite;
     comments->ReadOnly=false;
     resource_location->Color=clWhite;
     resource_location->ReadOnly=false;
     file1->Color=clWhite;
     file1->ReadOnly=false;
     file2->Color=clWhite;
     file2->ReadOnly=false;
     Add_experiment_record->Enabled=true;
     update_experiment_record->Enabled=true;
  }

  else
  {
     action_animal_grid->Color=clScrollBar;
     animal_pannel->Color=clScrollBar;
     action_pannel->Color=clScrollBar;
     age_lb->ColorLabBakG=clScrollBar;
     comments->Color=clScrollBar;
     comments->ReadOnly=true;
     resource_location->Color=clScrollBar;
     resource_location->ReadOnly=true;
     file1->Color=clScrollBar;
     file1->ReadOnly=true;
     file2->Color=clScrollBar;
     file2->ReadOnly=true;
     Add_experiment_record->Enabled=false;
     update_experiment_record->Enabled=false;
  }
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::action_animal_gridCellClick(TColumn *Column)
{
   animal_actions_update();
   TLocateOptions boob;
   birds->Locate("bird_name", experiments->FieldByName("bird_name")->AsString ,boob);
   actions->Locate("action_ID", experiments->FieldByName("action_ID")->AsString ,boob);
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::action_animal_gridDrawColumnCell(
      TObject *Sender, const TRect &Rect, int DataCol, TColumn *Column,
      TGridDrawState State)
{
  if(!allow_action_animal_changes->Down) animal_actions_update();
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::new_settingsClick(TObject *Sender)
{
  AnsiString New_Table = InputBox("New Table", "Please type new settings table name", "");
  if(!New_Table.Length())return;
  create_settings(New_Table);
  settings->Active=false;
  settings->TableName=New_Table;
  settings->Active=true;
  settings_table_name=New_Table;
  table_name->Caption=settings_table_name;
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::open_settingsClick(TObject *Sender)
{
   Application->CreateForm(__classid(TtableDialog), &tableDialog);
   tableDialog->show_type->ItemIndex=5;
   tableDialog->show_typeClick(this);
    if (tableDialog->ShowModal() == mrOk)
    {
      if(tableDialog->tables->ItemIndex==-1)return;
      settings->Active=false;
      settings->TableName=tableDialog->tables->Items->Strings[tableDialog->tables->ItemIndex];
      settings->Active=true;
      settings_table_name=tableDialog->tables->Items->Strings[tableDialog->tables->ItemIndex];
      table_name->Caption=settings_table_name;
    }
}
//---------------------------------------------------------------------------

int TDataForm::update_file_table_index(int chan)
{
    str="select max(file_index) from ";
    str+=file_table_name;
    if(!table_exists(file_table_name))Create_File_Table();
    MyQuery1->SQL->Clear();
    MyQuery1->SQL->Add(str);
    MyQuery1->Execute();
    int num=MyQuery1->Fields->Fields[0]->AsInteger;
    if(num>0) { file_indexes[chan]=num; return(num);}
    else return(0);

}


int TDataForm::count_files()//AnsiString folder)
{
      /*
        string s = "C:\\sap\\";
        DirectoryInfo dd;
        System.IO.DirectoryInfo d = new System.IO.DirectoryInfo(s);
        int intFolders;
        intFolders = (d.GetDirectories.GetUpperBound(0) + 1);
        intFiles = (d.GetFiles.GetUpperBound(0) + 1);
        MsgBox(("This directory ("
                + (s + (") has "
                + (intFolders.ToString + (" folders and "
                + (intFiles.ToString + " files.")))))));
           */
           return(0);
}
     

void __fastcall TDataForm::BitBtn32Click(TObject *Sender)
{
  Application->HelpContext(530);
}
//---------------------------------------------------------------------------


void TDataForm::insert_dur(int i)
{
    str="INSERT INTO state_duration(duration)VALUES (";
    str+=i;
    str+=");";
    MyCommand1->SQL->Clear();
    MyCommand1->SQL->Add(str);
    MyCommand1->Execute(1);
    MyCommand1->UnPrepare();
}



// add comment fields to syllable table if needed
bool TDataForm::fix_syll_table_fields(AnsiString table_name)
{
   str="describe "; str+=table_name;
   MyQuery1->SQL->Clear();
   MyQuery1->SQL->Add(str);
   MyQuery1->Execute();
   if(MyQuery1->RecordCount==44)// no comments record
   {
       str="ALTER TABLE ";
       str+=table_name;
       str+=" ADD `comments` varchar(80) default '0';";
       MyCommand1->SQL->Clear();
       MyCommand1->SQL->Add(str);
       MyCommand1->Execute(1);
       MyCommand1->UnPrepare();
   }
}
void __fastcall TDataForm::backup_tableClick(TObject *Sender)
{
  // select tables
  // file name to store to
 // dump->Connection->CreateDataSet();
 backup_error->Visible=false;
 Application->CreateForm(__classid(TtableDialog), &tableDialog);
 if (tableDialog->ShowModal() == mrOk)
 {
      if(tableDialog->tables->ItemIndex==-1)return;
      dump->TableNames= tableDialog->tables->Items->Strings[tableDialog->tables->ItemIndex];
      if(SaveDialog1->Execute()) str=SaveDialog1->FileName;
                                 else return;
      dump->BackupToFile(str);//"c:\\sap\\oferback.sql");
    }
    return;
}


//---------------------------------------------------------------------------

void __fastcall TDataForm::restore_tableClick(TObject *Sender)
{
	// select file to restore from
    backup_error->Visible=false;
    if(OpenDialog1->Execute()) str=OpenDialog1->FileName;
         else return;
    dump->RestoreFromFile(str);//"c:\\sap\\oferback.sql");
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::BitBtn4Click(TObject *Sender)
{
     str= InputBox("Change Database", "Type name of database", "");
     if(database_exists(str))Change_Database(str);
     else Application->MessageBox(L"Error: database Does not exist", NULL, MB_OK);
}

//---------------------------------------------------------------------------

void __fastcall TDataForm::BitBtn5Click(TObject *Sender)
{
  str= InputBox("Create new database", "Type name of new database", "");
  if(!database_exists(str)) create_database(str);
  else
  {
     Change_Database(str);
     Application->MessageBox(L"Note: database already exist", NULL, MB_OK);
  }
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::open_rawClick(TObject *Sender)
{
   str=open_Raw_Table();
   TheTable->Active=false;
   TheTable->TableName= str;
   TheTable->Active=true;
   if(show_table){DataSource1->DataSet=TheTable;  DataSource1->Enabled=true;}
   else DataSource1->Enabled=false;
   table_name->Caption=str;
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::dumpBackupProgress(TObject *Sender, UnicodeString ObjectName,
          int ObjectNum, int ObjectCount, int Percent)
{
  backup_progress->Position=Percent;
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::dumpRestoreProgress(TObject *Sender, int Percent)
{
   restore_progress->Position=Percent;
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::dumpError(TObject *Sender, Exception *E, UnicodeString SQL,
          TErrorAction &Action)
{
  backup_error->Caption="Sorry, error occured";
  backup_error->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::backup_allClick(TObject *Sender)
{
 backup_error->Visible=false;
 dump->TableNames="";
 if(SaveDialog1->Execute()) str=SaveDialog1->FileName;
 else return;
 dump->BackupToFile(str);//"c:\\sap\\oferback.sql");
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::restore_allClick(TObject *Sender)
{
   restore_tableClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::animalsClick(TObject *Sender)
{
 datapages->ActivePage=animals_table;
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::view_data_btnClick(TObject *Sender)
{
  datapages->ActivePage=data_table;
  manage_tables->ActivePage=view_tables;
}
//---------------------------------------------------------------------------


void __fastcall TDataForm::export_btnClick(TObject *Sender)
{
    datapages->ActivePage=Export;
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::data_tableEnter(TObject *Sender)
{
   show_table=true;
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::data_tableExit(TObject *Sender)
{
  show_table=false;
}
//---------------------------------------------------------------------------


void __fastcall TDataForm::update_displayClick(TObject *Sender)
{
    Records->Active=false;
    Records->Active=true;    // this get the table data refreshed, dont ask why...
    DataSource1->DataSet=DataForm->Records;
    DataSource1->Enabled=true;
}
//---------------------------------------------------------------------------


void __fastcall TDataForm::export_dataClick(TObject *Sender)
{
datapages->ActivePage=Export;
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::backup_btnClick(TObject *Sender)
{
  datapages->ActivePage=data_table;
  manage_tables->ActivePage=backup;
}
//---------------------------------------------------------------------------

void __fastcall TDataForm::create_table_btnClick(TObject *Sender)
{
  datapages->ActivePage=data_table;
  manage_tables->ActivePage=create_tables;
}
//---------------------------------------------------------------------------


void TDataForm::Retrieve_segmentation_to_options(int chan)
{
	  if (chan == 1) {
		option->pri_segmented_feature1 = Retrieve(chan, "pri_segmented_feature").ToInt();
		option->sec_segmented_feature1 = Retrieve(chan, "sec_segmented_feature").ToInt();
		option->reverse_segmentation1 = Retrieve(chan, "pri_lt").ToInt();
		option->sec_lt1 = Retrieve(chan, "sec_lt").ToInt();
		option->smooth_feature1 = Retrieve(chan, "smooth_feature").ToInt();
		option->adapt_threshold1 = Retrieve(chan, "adapt_thresh").ToInt();
		option->sec_segmentation1 = Retrieve(chan, "sec_segmentation").ToInt();
		option->sec_logic1  = Retrieve(chan, "sec_logic").ToInt();
		option->hp_small1 = 100* Retrieve(chan, "hp_small").ToInt();
		float x =(float)Retrieve(chan, "hp_big").ToInt()/30.0;
		option->hp_big1 = exp(x);
	 } else if (chan == 2) {
		option->pri_segmented_feature2 = Retrieve(chan, "pri_segmented_feature").ToInt();
		option->sec_segmented_feature2 = Retrieve(chan, "sec_segmented_feature").ToInt();
		option->reverse_segmentation2 = Retrieve(chan, "pri_lt").ToInt();
		option->sec_lt2 = Retrieve(chan, "sec_lt").ToInt();
		option->smooth_feature2 = Retrieve(chan, "smooth_feature").ToInt();
		option->adapt_threshold2 = Retrieve(chan, "adapt_thresh").ToInt();
		option->sec_segmentation2 = Retrieve(chan, "sec_segmentation").ToInt();
		option->sec_logic2  = Retrieve(chan, "sec_logic").ToInt();
		option->hp_small2 = 100* Retrieve(chan, "hp_small").ToInt();
		float x =(float)Retrieve(chan, "hp_big").ToInt()/30.0;
		option->hp_big2 = exp(x);
	 } else {
		option->pri_segmented_feature = Retrieve(chan, "pri_segmented_feature").ToInt();
		option->pri_thresh_raw = Retrieve(chan, "deriv_thresh").ToInt();
		option->sec_segmented_feature = Retrieve(chan, "sec_segmented_feature").ToInt();
		option->sec_thresh_raw = Retrieve(chan, "entropy_thresh").ToInt();
		option->reverse_segmentation = Retrieve(chan, "pri_lt").ToInt();
		option->sec_lt = Retrieve(chan, "sec_lt").ToInt();
		option->smooth_feature = Retrieve(chan, "smooth_feature").ToInt();
		option->adapt_threshold = Retrieve(chan, "adapt_thresh").ToInt();
		option->sec_segmentation = Retrieve(chan, "sec_segmentation").ToInt();
		option->sec_logic  = Retrieve(chan, "sec_logic").ToInt();
		option->hp_small = 100* Retrieve(chan, "hp_small").ToInt();
		float x =(float)Retrieve(chan, "hp_big").ToInt()/30.0;
		option->hp_big = exp(x);
	 }

	 //These don't exist in DB!!!!!
	//option->stop_duration_thresh = Retrieve(chan, "stop_duration_thresh").ToInt();
	//option->bout_duration_thresh = Retrieve(chan, "bout_duration_thresh").ToInt();
}


void TDataForm::Retrieve_frame_params(TFrame1 *frame, int chan)
{
	if(chan<0 || chan>11)return; // incorrect bird/channel

	Retrieve_segmentation_to_options(chan); // first retrive values from the database

	// set frame controllers
	if (chan == 1) {
		frame->segment_by->ItemIndex = option->pri_segmented_feature1;
		frame->secondaryFeatureDD->ItemIndex = option->sec_segmented_feature1;
		frame->segmentation_flip->ItemIndex = option->reverse_segmentation1;
		frame->secondaryComparisonRB->ItemIndex = option->sec_lt1;
		frame->smoothGraphCB->Checked = option->smooth_feature1;
		frame->adaptThreshCB->Checked = option->adapt_threshold1;
		frame->secondaryFeatureCB->Checked = option->sec_segmentation1;
		frame->secondaryLogicRB->ItemIndex = option->sec_logic1;
		frame->hp_small->Position=option->hp_small1/100;
		frame->hp_big->Position=30*log(option->hp_big1);
	} else if (chan == 2) { // song 2 in explore and score (special case)
		frame->segment_by->ItemIndex = option->pri_segmented_feature2;
		frame->secondaryFeatureDD->ItemIndex = option->sec_segmented_feature2;
		frame->segmentation_flip->ItemIndex = option->reverse_segmentation2;
		frame->secondaryComparisonRB->ItemIndex = option->sec_lt2;
		frame->smoothGraphCB->Checked = option->smooth_feature2;
		frame->adaptThreshCB->Checked = option->adapt_threshold2;
		frame->secondaryFeatureCB->Checked = option->sec_segmentation2;
		frame->secondaryLogicRB->ItemIndex = option->sec_logic2;
		frame->hp_small->Position=option->hp_small2/100;
		frame->hp_big->Position=30*log(option->hp_big2);
	} else {
		frame->segment_by->ItemIndex = option->pri_segmented_feature;
		frame->secondaryFeatureDD->ItemIndex = option->sec_segmented_feature;
		frame->segmentation_flip->ItemIndex = option->reverse_segmentation;
		frame->secondaryComparisonRB->ItemIndex = option->sec_lt;
		frame->smoothGraphCB->Checked = option->smooth_feature;
		frame->adaptThreshCB->Checked = option->adapt_threshold;
		frame->secondaryFeatureCB->Checked = option->sec_segmentation;
		frame->secondaryLogicRB->ItemIndex = option->sec_logic;
		frame->hp_small->Position=option->hp_small/100;
		frame->hp_big->Position=30*log(option->hp_big);
		// set threshold display for primary feature:
		frame->NumLab3->Value = option->adjustThreshold(frame->segment_by->ItemIndex, option->pri_thresh_raw, option->frequency, 1);
		frame->secondaryThresholdTB->Value = option->adjustThreshold(frame->secondaryFeatureDD->ItemIndex, option->sec_thresh_raw, option->frequency, 1);


	}
	frame->min_stop_durTb->Position=option->stop_duration_thresh;
	frame->bout_stop->Position=option->bout_duration_thresh;
	frame->smoothGraphCBClick(frame->smoothGraphCB);
	frame->secondaryFeatureCBClick(frame->secondaryFeatureCB);
	frame->adaptThresholdCBClick(frame->adaptThreshCB);

}



void TDataForm::Save_frame_params(TFrame1 *frame, int chan)
{
	AnsiString str;
   //	chan=1;
	str=frame->segment_by->ItemIndex;
	Settings(chan, "pri_segmented_feature", str);
	str=frame->secondaryFeatureDD->ItemIndex;
	Settings(chan, "sec_segmented_feature", str);
	str=frame->segmentation_flip->ItemIndex;
	Settings(chan, "pri_lt", str);
	str=frame->secondaryComparisonRB->ItemIndex;
	Settings(chan, "sec_lt", str);
	str= frame->smoothGraphCB->Checked? 1:0;
	Settings(chan,"smooth_feature" , str);
	str= frame->hp_small->Position;
	Settings(chan,"hp_small" , str);
	str= frame->hp_big->Position;
	Settings(chan,"hp_big" , str);
	str= frame->adaptThreshCB->Checked? 1:0;
	Settings(chan,"adapt_thresh" , str);
	str= frame->secondaryFeatureCB->Checked? 1:0;
	Settings(chan, "sec_segmentation", str);
	str= frame->secondaryLogicRB->ItemIndex;
	Settings(chan, "sec_logic", str);
	str= frame->min_stop_durTb->Position;
	Settings(chan, "syllable_duration_reject", str);
	str= frame->bout_stop->Position;
	Settings(chan, "bout_duration_reject", str);
    //Gives trouble!!!
	//Settings(chan, "stop_duration_thresh",frame->min_stop_durTb->Position);
	//Settings(chan, "bout_duration_thresh",frame->bout_stop->Position);
}

void __fastcall TDataForm::nameChange(TObject *Sender)
{
  str=name->Text;
  AnsiString str1="";
  for (int i = 1; i <= str.Length(); i++) if(str[i]!='_' && str[i]!='-' && str[i]!=' ')str1+=str[i];
  name->Text=str1;
  //name_view->Text=name->Text;
}
//---------------------------------------------------------------------------

