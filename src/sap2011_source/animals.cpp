//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop                      
#include "animals.h"
#include "records.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "CRGrid"
#pragma link "DBAccess"
#pragma link "MemDS"
#pragma link "MyAccess"
//#pragma link "NumLab"
#pragma link "SDL_NumLab"
#pragma link "AdvSmoothDatePicker"
#pragma link "AdvSmoothEdit"
#pragma link "AdvSmoothEditButton"
#pragma resource "*.dfm"
Tanimals_form *animals_form;
//---------------------------------------------------------------------------
__fastcall Tanimals_form::Tanimals_form(TComponent* Owner)
        : TForm(Owner)
{
 new_bird=false;
 DataForm->birds->Active=true;
 //ShortDateFormat = "yyyy-m-d";
 //LongTimeFormat = "hh:mm:ss";
}
//---------------------------------------------------------------------------

void __fastcall Tanimals_form::animals_gridCellClick(TColumn *Column)
{
  OKBtn->Enabled=true;
  bird_update();
}
//---------------------------------------------------------------------------

void __fastcall Tanimals_form::animals_gridDrawColumnCell(TObject *Sender,
      const TRect &Rect, int DataCol, TColumn *Column,
      TGridDrawState State)
{
  bird_update();        
}
//---------------------------------------------------------------------------

void __fastcall Tanimals_form::animals_gridTitleClick(TColumn *Column)
{
   messages->Caption="Click field name above to sort, then search (case sensitive). To filter click 'enter', then 'Apply filter'.";
}
//---------------------------------------------------------------------------

void __fastcall Tanimals_form::filterClick(TObject *Sender)
{
  allow_update=false;
  animals_grid->ApplyFilter();
}
//---------------------------------------------------------------------------

void __fastcall Tanimals_form::clear_filtersClick(TObject *Sender)
{
  animals_grid->ClearFilters();
   animals_grid->ClearSorting();
   animals_grid->ApplyFilter();
}
//---------------------------------------------------------------------------

void Tanimals_form::bird_update()
{
        hatched->Date=DataForm->birds->FieldByName("hatched")->AsDateTime;
        age->Value=(int)Date()-(int)hatched->Date;
       // age_lb->Value=age->Value;
        procedure1->Text=DataForm->birds->FieldByName("procedure1")->AsString;
        procedure1_schedule->Date=DataForm->birds->FieldByName("proc1_date")->AsDateTime;
        procedure1_age->Value=procedure1_schedule->Date-hatched->Date;
        procedure1_time->Time=DataForm->birds->FieldByName("proc1_date")->AsDateTime;
        if(procedure1_age->Value<0)procedure1_age->Value=0;

        procedure2->Text=DataForm->birds->FieldByName("procedure2")->AsString;
        procedure2_schedule->Date=DataForm->birds->FieldByName("proc2_date")->AsDateTime;
        procedure2_time->Time=DataForm->birds->FieldByName("proc2_date")->AsDateTime;
        procedure2_age->Value=procedure2_schedule->Date-hatched->Date;
        if(procedure2_age->Value<0)procedure2_age->Value=0;

        trained->Date=DataForm->birds->FieldByName("training")->AsDateTime;
        train_time->Time=DataForm->birds->FieldByName("training")->AsDateTime;
        training->Text=(int)(trained->Date-hatched->Date);
        if(training->Text.ToIntDef(0)<0)training->Text=0;

        audio->Text=DataForm->birds->FieldByName("audio")->AsString;
        attributes->Text=DataForm->birds->FieldByName("attributes")->AsString;
        photoperiod->Text=DataForm->birds->FieldByName("photoperiod")->AsString;

		name->Text=DataForm->birds->FieldByName("bird_name")->AsString;
        name_view->Text=name->Text;
        str=DataForm->birds->FieldByName("sex")->AsString;//FieldValues["gender"];
		if(str=="M")gender->ItemIndex=0; else gender->ItemIndex=1;
        weight->Text=DataForm->birds->FieldByName("weight")->AsString;;
        cage->Text=DataForm->birds->FieldByName("location")->AsString;
        song->Text=DataForm->birds->FieldByName("model")->AsString;
        song_quota->Text=DataForm->birds->FieldByName("quota")->AsString;
        bird_comments->Text=DataForm->birds->FieldByName("details")->AsString;
}
void __fastcall Tanimals_form::set_time1Click(TObject *Sender)
{
  hatched->Date=Date();
  age->Value=0;        
}
//---------------------------------------------------------------------------

void __fastcall Tanimals_form::hatchedClick(TObject *Sender)
{
   age->Value=(int)Date()-(int)hatched->Date;
}
//---------------------------------------------------------------------------

void __fastcall Tanimals_form::trainingChange(TObject *Sender)
{
  int train_age=training->Text.ToIntDef(0);
  trained->Date=hatched->Date+train_age;        
}
//---------------------------------------------------------------------------



void __fastcall Tanimals_form::nameChange(TObject *Sender)
{
  str=name->Text;
  AnsiString str1="";
  for (int i = 1; i <= str.Length(); i++) if(str[i]!='_' && str[i]!='-' && str[i]!=' ')str1+=str[i];
  name->Text=str1;
  name_view->Text=name->Text;
}
//---------------------------------------------------------------------------

      

void Tanimals_form::Insert_bird_record()
{
   AnsiString a1=name->Text;
   ShortDateFormat = "m/d/yyyy";
   AnsiString a2, a3, a4, a5, a6,a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19;
   str="SELECT COUNT(*) FROM animals where bird_name='"; str+=a1; str+="';";
   DataForm->MyQuery1->SQL->Clear();
   DataForm->MyQuery1->SQL->Add(str);
   DataForm->MyQuery1->Execute();
   int recNum=DataForm->MyQuery1->Fields->Fields[0]->AsInteger;
   if(recNum) return;
   if(gender->Text=="male")a2="M"; else a2="F";
   a3=FormatDateTime(hatched->Date,2);//FormatDateTime(hatched->Date,2);
   //DateToStr(hatched->Date);
   a4=weight->Text;
   a5=cage->Text;
   a6=procedure1->Text;
   a7= FormatDateTime(procedure1_schedule->Date,2);//DateToStr(procedure1_schedule->Date);
   a8=procedure2->Text;
   a9= FormatDateTime(procedure2_schedule->Date,2);//DateToStr(procedure2_schedule->Date);
   a10="";
   a11="";
   a12=FormatDateTime(trained->Date,2);//DateToStr(trained->Date);
   a14=song->Text;
   a15=song_quota->Text;
   a16=bird_comments->Text;
   a17=audio->Text;
   a18=attributes->Text;
   a19=photoperiod->Text;
   DataForm->birds->AppendRecord(ARRAYOFCONST((a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a14, a15, a17, a18, a19, a16)));
   Beep();
   ShortDateFormat = "yyyy-m-d";
}
   


void __fastcall Tanimals_form::OKBtnClick(TObject *Sender)
{
  if(datapages->ActivePageIndex)new_bird=true;        
}
//---------------------------------------------------------------------------


void Tanimals_form::Animals_to_settings(int chan)
{
        AnsiString syll_table;
        DataForm->settings->RecNo=chan;
        DataForm->settings->Edit();
        DataForm->settings->FieldValues["bird_ID"]=name->Text;
        DataForm->settings->FieldValues["hatching_date"]=hatched->Date;
        DataForm->settings->FieldValues["start_training"]=trained->Date;
        DataForm->settings->FieldValues["song_quota"]=song_quota->Text.ToIntDef(0);
        syll_table="syll_"; syll_table+=name->Text;
        DataForm->settings->FieldValues["syllable_table"]=syll_table;
        DataForm->settings->Post();
}


void __fastcall Tanimals_form::set_time2Click(TObject *Sender)
{
   trained->Date=Date();
   train_time->Time=Time();
}
//---------------------------------------------------------------------------

void __fastcall Tanimals_form::set_time3Click(TObject *Sender)
{
   procedure1_schedule->Date=Date();
   procedure1_time->Time=Time();
}
//---------------------------------------------------------------------------

void __fastcall Tanimals_form::set_time4Click(TObject *Sender)
{
   procedure2_schedule->Date=Date();
   procedure2_time->Time=Time();
}
//---------------------------------------------------------------------------

void __fastcall Tanimals_form::BitBtn17Click(TObject *Sender)
{
  Application->HelpContext(500);
}
//---------------------------------------------------------------------------

void __fastcall Tanimals_form::BitBtn1Click(TObject *Sender)
{
  datapages->ActivePage=new_animal;
}
//---------------------------------------------------------------------------

