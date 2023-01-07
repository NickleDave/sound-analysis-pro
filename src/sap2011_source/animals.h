//---------------------------------------------------------------------------

#ifndef animalsH
#define animalsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "CRGrid.hpp"
#include "DBAccess.hpp"
#include "MemDS.hpp"
#include "MyAccess.hpp"
#include "SDL_NumLab.hpp"
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <Db.hpp>
#include <DBCtrls.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include "SDL_NumLab.hpp"
#include "AdvSmoothDatePicker.hpp"
#include "AdvSmoothEdit.hpp"
#include "AdvSmoothEditButton.hpp"
//---------------------------------------------------------------------------
class Tanimals_form : public TForm
{
__published:	// IDE-managed Components
        TPageControl *datapages;
        TTabSheet *select_animal;
        TLabel *messages;
        TDBNavigator *birds_navigator;
        TCRDBGrid *animals_grid;
        TBitBtn *filter;
        TBitBtn *clear_filters;
        TBitBtn *BitBtn32;
        TDataSource *DataSource1;
        TLabel *Label1;
        TTabSheet *new_animal;
        TPanel *Panel5;
        TLabel *Label11;
        TNumLab *procedure1_age;
        TLabel *Label12;
        TEdit *procedure1;
        TDateTimePicker *procedure1_time;
        TBitBtn *set_time3;
        TPanel *Panel4;
        TLabel *Label9;
        TLabel *Label10;
        TDateTimePicker *train_time;
        TBitBtn *set_time2;
        TPanel *Panel3;
        TLabel *Label8;
        TNumLab *age;
        TLabel *Label7;
        TBitBtn *set_time1;
        TEdit *bird_comments;
        TStaticText *StaticText39;
        TComboBox *gender;
        TEdit *cage;
        TStaticText *StaticText33;
        TStaticText *StaticText38;
        TEdit *song_quota;
        TStaticText *StaticText35;
        TEdit *attributes;
        TEdit *photoperiod;
        TStaticText *StaticText41;
        TEdit *audio;
        TStaticText *StaticText34;
        TEdit *song;
        TStaticText *StaticText22;
        TEdit *weight;
        TStaticText *StaticText23;
        TEdit *name;
        TStaticText *StaticText21;
        TStaticText *StaticText24;
        TPanel *Panel6;
        TLabel *Label13;
        TLabel *Label14;
        TNumLab *procedure2_age;
        TEdit *procedure2;
        TDateTimePicker *procedure2_time;
        TBitBtn *set_time4;
        TStaticText *StaticText1;
        TEdit *name_view;
        TButton *OKBtn;
        TButton *CancelBtn;
        TEdit *training;
        TBitBtn *BitBtn17;
	TAdvSmoothDatePicker *hatched;
	TAdvSmoothDatePicker *trained;
	TAdvSmoothDatePicker *procedure1_schedule;
	TAdvSmoothDatePicker *procedure2_schedule;
    TBitBtn *BitBtn1;
	TStaticText *StaticText2;
        void __fastcall animals_gridCellClick(TColumn *Column);
        void __fastcall animals_gridDrawColumnCell(TObject *Sender,
          const TRect &Rect, int DataCol, TColumn *Column,
          TGridDrawState State);
        void __fastcall animals_gridTitleClick(TColumn *Column);
        void __fastcall filterClick(TObject *Sender);
        void __fastcall clear_filtersClick(TObject *Sender);
        void __fastcall set_time1Click(TObject *Sender);
        void __fastcall hatchedClick(TObject *Sender);
        void __fastcall trainingChange(TObject *Sender);
        void __fastcall nameChange(TObject *Sender);
        void __fastcall OKBtnClick(TObject *Sender);
        void __fastcall set_time2Click(TObject *Sender);
        void __fastcall set_time3Click(TObject *Sender);
        void __fastcall set_time4Click(TObject *Sender);
        void __fastcall BitBtn17Click(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
private:	// User declarations
              void bird_update();
              bool allow_update;
              AnsiString str;  
public:		// User declarations
        __fastcall Tanimals_form(TComponent* Owner);
        void Insert_bird_record();
        bool new_bird;
        void Animals_to_settings(int chan);
};
//---------------------------------------------------------------------------
extern PACKAGE Tanimals_form *animals_form;
//---------------------------------------------------------------------------
#endif
