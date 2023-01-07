//---------------------------------------------------------------------------

#ifndef sound_liveH
#define sound_liveH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "CZT_FFT_Utils_Framework.h"
#include "CWavIterator.h"
#include <Dialogs.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include "CZT_FeatureCalc.h"
#include <FileCtrl.hpp>
#include "SDL_NumLab.hpp"
#include <Graphics.hpp>
#include <Chart.hpp>
#include <Series.hpp>
#include <TeEngine.hpp>

#include "SDL_rchart.hpp"
#include "SDL_NumLab.hpp"
#include "TeeProcs.hpp"
#include <Menus.hpp>
#include "segmentation.h"
//#include "segmentation.h"

using namespace ztFFTAPI;
//---------------------------------------------------------------------------
class Tsound_live : public TForm
{
__published:	// IDE-managed Components
        TOpenDialog *OpenDialog1;
        TPageControl *PageControl1;
        TTabSheet *input_output;
        TLabel *Label16;
        TTabSheet *options;
        TTabSheet *TabSheet4;
        TChart *Chart7;
        TPointSeries *Series1;
        TPointSeries *Series2;
        TPointSeries *Series3;
        TPointSeries *Series4;
        TPointSeries *Series5;
        TPointSeries *Series6;
        TChart *Chart0;
        TPointSeries *PointSeries19;
        TPointSeries *PointSeries20;
        TPointSeries *PointSeries21;
        TPointSeries *PointSeries22;
        TPointSeries *PointSeries23;
        TPointSeries *PointSeries24;
        TPanel *Panel1;
        TFileListBox *FileListBox0;
        TBitBtn *input0;
        TBitBtn *output0;
        TEdit *chan0_out;
        TEdit *chan0_in;
        TPanel *Panel2;
        TPanel *Panel3;
        TPanel *Panel4;
        TPanel *Panel6;
        TFileListBox *FileListBox1;
        TBitBtn *input1;
        TBitBtn *output1;
        TEdit *chan1_in;
        TEdit *chan1_out;
        TFileListBox *FileListBox2;
        TEdit *chan2_in;
        TEdit *chan2_out;
        TBitBtn *output2;
        TBitBtn *input2;
        TFileListBox *FileListBox3;
        TBitBtn *input3;
        TBitBtn *output3;
        TEdit *chan3_in;
        TEdit *chan3_out;
        TPanel *Panel7;
        TPanel *Panel8;
        TPanel *Panel9;
        TFileListBox *FileListBox4;
        TBitBtn *input4;
        TBitBtn *output4;
        TEdit *chan4_in;
        TEdit *chan4_out;
        TFileListBox *FileListBox5;
        TEdit *chan5_in;
        TEdit *chan5_out;
        TBitBtn *input5;
        TBitBtn *output5;
        TFileListBox *FileListBox6;
        TFileListBox *FileListBox7;
        TEdit *chan7_out;
        TEdit *chan7_in;
        TBitBtn *input7;
        TBitBtn *output7;
        TBitBtn *output6;
        TBitBtn *input6;
        TEdit *chan6_in;
        TEdit *chan6_out;
        TPanel *Panel11;
        TPanel *Panel13;
        TPanel *Panel10;
        TPanel *Panel12;
        TPanel *Panel14;
        TPanel *Panel15;
        TPanel *Panel16;
        TPanel *Panel17;
        TTimer *Timer1;
        TChart *Chart1;
        TPointSeries *PointSeries1;
        TPointSeries *PointSeries2;
        TPointSeries *PointSeries3;
        TPointSeries *PointSeries4;
        TPointSeries *PointSeries5;
        TPointSeries *PointSeries6;
        TChart *Chart2;
        TPointSeries *PointSeries7;
        TPointSeries *PointSeries8;
        TPointSeries *PointSeries9;
        TPointSeries *PointSeries10;
        TPointSeries *PointSeries11;
        TPointSeries *PointSeries12;
        TChart *Chart3;
        TPointSeries *PointSeries13;
        TPointSeries *PointSeries14;
        TPointSeries *PointSeries15;
        TPointSeries *PointSeries16;
        TPointSeries *PointSeries17;
        TPointSeries *PointSeries18;
        TChart *Chart4;
        TPointSeries *PointSeries25;
        TPointSeries *PointSeries26;
        TPointSeries *PointSeries27;
        TPointSeries *PointSeries28;
        TPointSeries *PointSeries29;
        TPointSeries *PointSeries30;
        TChart *Chart5;
        TPointSeries *PointSeries31;
        TPointSeries *PointSeries32;
        TPointSeries *PointSeries33;
        TPointSeries *PointSeries34;
        TPointSeries *PointSeries35;
        TPointSeries *PointSeries36;
        TChart *Chart6;
        TPointSeries *PointSeries37;
        TPointSeries *PointSeries38;
        TPointSeries *PointSeries39;
        TPointSeries *PointSeries40;
        TPointSeries *PointSeries41;
        TPointSeries *PointSeries42;
        TNumLab *ms_recs0;
        TNumLab *ms_recs1;
        TNumLab *ms_recs2;
        TNumLab *ms_recs3;
        TNumLab *ms_recs4;
        TNumLab *ms_recs5;
        TNumLab *ms_recs6;
        TNumLab *ms_recs7;
        TNumLab *recnum0;
        TNumLab *recnum1;
        TNumLab *recnum2;
        TNumLab *recnum3;
        TNumLab *recnum4;
        TNumLab *recnum5;
        TNumLab *recnum6;
        TNumLab *recnum7;
        TTabSheet *settings;
        TPanel *Panel5;
        TLabel *Label1;
        TNumLab *SyllDurLow;
        TLabel *Label18;
        TNumLab *BoutDurLow;
        TNumLab *mean_syllable_num;
        TTrackBar *SyllMinDurTb;
        TStaticText *StaticText13;
        TTrackBar *BoutMinDurTB;
        TStaticText *StaticText83;
        TStaticText *StaticText15;
        TTrackBar *min_syllables;
        TPanel *pannel1;
        TNumLab *folder_sizeN;
        TLabel *sizeLb;
        TStaticText *sizeLb2;
        TTrackBar *folder_size_limit;
        TRadioGroup *folder_choice;
        TMemo *sizeH2;
        TPanel *pannel2;
        TLabel *Label6;
        TNumLab *ProcTreshN;
        TLabel *Label2;
        TTrackBar *ProcTreshTB;
        TStaticText *StaticText12;
        TTrackBar *slow_execution;
        TPanel *pannel3;
        TLabel *problem_filename;
        TMemo *Memo3;
        TBitBtn *features_output1;
        TBitBtn *BitBtn1;
        TLabel *file_ch0;
        TBitBtn *new_table0;
        TBitBtn *change_table0;
        TStaticText *StaticText17;
        TNumLab *folder_size0;
        TStaticText *StaticText34;
        TStaticText *StaticText1;
        TStaticText *syllable_table0;
        TStaticText *StaticText56;
        TStaticText *ms_table0;
        TStaticText *StaticText37;
        TNumLab *age0;
        TStaticText *StaticText40;
        TStaticText *StaticText36;
        TStaticText *ms_table1;
        TStaticText *syllable_table1;
        TStaticText *StaticText46;
        TStaticText *syllable_table2;
        TStaticText *ms_table2;
        TStaticText *StaticText42;
        TStaticText *StaticText52;
        TStaticText *syllable_table3;
        TStaticText *ms_table3;
        TStaticText *StaticText48;
        TStaticText *StaticText105;
        TStaticText *StaticText102;
        TStaticText *syllable_table5;
        TStaticText *ms_table5;
        TStaticText *StaticText20;
        TStaticText *StaticText39;
        TStaticText *syllable_table4;
        TStaticText *ms_table4;
        TStaticText *StaticText109;
        TStaticText *syllable_table6;
        TStaticText *StaticText106;
        TStaticText *ms_table6;
        TStaticText *StaticText113;
        TStaticText *syllable_table7;
        TStaticText *ms_table7;
        TStaticText *StaticText110;
        TStaticText *StaticText57;
        TNumLab *folder_size1;
        TStaticText *StaticText62;
        TStaticText *StaticText63;
        TNumLab *folder_size2;
        TStaticText *StaticText68;
        TStaticText *StaticText81;
        TNumLab *folder_size3;
        TStaticText *StaticText87;
        TStaticText *StaticText69;
        TNumLab *folder_size4;
        TStaticText *StaticText74;
        TStaticText *StaticText88;
        TNumLab *folder_size5;
        TStaticText *StaticText93;
        TStaticText *StaticText75;
        TNumLab *folder_size6;
        TStaticText *StaticText80;
        TStaticText *StaticText94;
        TNumLab *folder_size7;
        TStaticText *StaticText99;
        TLabel *file_ch1;
        TLabel *file_ch2;
        TLabel *file_ch3;
        TLabel *file_ch4;
        TLabel *file_ch5;
        TLabel *file_ch6;
        TLabel *file_ch7;
        TBitBtn *new_table1;
        TBitBtn *change_table1;
        TBitBtn *new_table2;
        TBitBtn *change_table2;
        TBitBtn *new_table3;
        TBitBtn *change_table3;
        TBitBtn *new_table4;
        TBitBtn *change_table4;
        TBitBtn *new_table5;
        TBitBtn *change_table5;
        TBitBtn *new_table6;
        TBitBtn *change_table6;
        TBitBtn *new_table7;
               TBitBtn *change_table7;
        TStaticText *StaticText43;
        TNumLab *age1;
        TStaticText *StaticText45;
        TNumLab *age2;
        TStaticText *StaticText49;
        TNumLab *age3;
        TStaticText *StaticText51;
        TNumLab *age4;
        TStaticText *StaticText54;
        TNumLab *age5;
        TStaticText *StaticText55;
        TNumLab *age6;
        TStaticText *StaticText60;
        TNumLab *age7;
        TStaticText *StaticText86;
        TStaticText *StaticText91;
        TStaticText *StaticText92;
        TStaticText *StaticText97;
        TStaticText *StaticText98;
        TStaticText *StaticText100;
        TStaticText *StaticText101;
        TStaticText *StaticText103;
        TStaticText *StaticText104;
        TStaticText *StaticText107;
        TStaticText *StaticText108;
        TStaticText *StaticText111;
        TStaticText *StaticText112;
        TStaticText *StaticText114;
        TStaticText *StaticText115;
        TStaticText *StaticText116;
        TRadioGroup *save_mem;
        TStaticText *StaticText117;
        TStaticText *file_table1;
        TStaticText *StaticText118;
        TStaticText *file_table2;
        TStaticText *StaticText120;
        TStaticText *file_table3;
        TStaticText *StaticText122;
        TStaticText *file_table4;
        TStaticText *StaticText124;
        TStaticText *file_table5;
        TStaticText *StaticText126;
        TStaticText *file_table6;
        TStaticText *StaticText128;
        TStaticText *file_table7;
        TStaticText *StaticText130;
        TStaticText *file_table8;
        TTimer *on_start_timer;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label7;
        TLabel *Label8;
        TLabel *Label9;
        TBitBtn *update_tables;
        TBitBtn *change_bird1;
        TBitBtn *change_bird3;
        TBitBtn *change_bird5;
        TBitBtn *change_bird7;
        TBitBtn *change_bird2;
        TBitBtn *change_bird4;
        TBitBtn *change_bird6;
        TBitBtn *change_bird8;
        TNumLab *file_table_recs0;
        TNumLab *file_table_recs1;
        TNumLab *file_table_recs2;
        TNumLab *file_table_recs3;
        TNumLab *file_table_recs4;
        TNumLab *file_table_recs5;
        TNumLab *file_table_recs6;
        TNumLab *file_table_recs7;
        TBitBtn *BitBtn7;
        TCheckBox *sampling1;
        TCheckBox *sampling2;
        TCheckBox *sampling4;
        TCheckBox *sampling3;
        TCheckBox *sampling5;
        TCheckBox *sampling6;
        TCheckBox *sampling7;
        TCheckBox *sampling8;
    TTabSheet *Main;
    TScrollBox *ScrollBox9;
    TPanel *Panel18;
    TImage *Image1;
    TNumLab *bout_dur0;
    TNumLab *syll_dur0;
    TNumLab *syllable_num0;
    TShape *thresh0;
    TImage *gainIc;
    TStaticText *StaticText10;
    TTrackBar *entropyThresh0;
    TStaticText *StaticText18;
    TTrackBar *DerivThresh0;
    TStaticText *StaticText21;
    TTrackBar *Brightness0;
    TStaticText *rec0;
    TEdit *ID_0;
    TStaticText *StaticText19;
    TStaticText *StaticText35;
    TMainMenu *MainMenu1;
    TMenuItem *File1;
    TMenuItem *new1;
    TMenuItem *close1;
    TMenuItem *Exit1;
    TMenuItem *Edit1;
    TMenuItem *copy1;
    TMenuItem *paste1;
    TPanel *Panel19;
    TPanel *Panel20;
    TPanel *Panel21;
    TPanel *Panel22;
    TPanel *Panel23;
    TPanel *Panel24;
    TPanel *Panel25;
    TStaticText *startup_state;
    TShape *busy;
    TBitBtn *start;
    TBitBtn *stop;
    TImage *Image2;
    TNumLab *bout_dur1;
    TNumLab *syll_dur1;
    TNumLab *syllable_num1;
    TShape *thresh1;
    TTrackBar *Brightness1;
    TStaticText *rec1;
    TTrackBar *entropyThresh1;
    TTrackBar *DerivThresh1;
    TEdit *ID_1;
    TStaticText *StaticText67;
    TImage *Image3;
    TNumLab *bout_dur2;
    TNumLab *syll_dur2;
    TNumLab *syllable_num2;
    TShape *thresh2;
    TTrackBar *Brightness2;
    TStaticText *rec2;
    TTrackBar *entropyThresh2;
    TTrackBar *DerivThresh2;
    TEdit *ID_2;
    TStaticText *StaticText66;
    TImage *Image4;
    TNumLab *bout_dur3;
    TNumLab *syll_dur3;
    TNumLab *syllable_num3;
    TShape *thresh3;
    TTrackBar *Brightness3;
    TStaticText *rec3;
    TTrackBar *entropyThresh3;
    TTrackBar *DerivThresh3;
    TEdit *ID_3;
    TStaticText *StaticText72;
    TImage *Image5;
    TNumLab *bout_dur4;
    TNumLab *syll_dur4;
    TNumLab *syllable_num4;
    TShape *thresh4;
    TTrackBar *Brightness4;
    TStaticText *rec4;
    TTrackBar *DerivThresh4;
    TTrackBar *entropyThresh4;
    TEdit *ID_4;
    TStaticText *StaticText73;
    TImage *Image6;
    TNumLab *bout_dur5;
    TNumLab *syll_dur5;
    TNumLab *syllable_num5;
    TShape *thresh5;
    TTrackBar *Brightness5;
    TStaticText *rec5;
    TTrackBar *entropyThresh5;
    TTrackBar *DerivThresh5;
    TEdit *ID_5;
    TStaticText *StaticText78;
    TImage *Image7;
    TNumLab *bout_dur6;
    TNumLab *syll_dur6;
    TNumLab *syllable_num6;
    TShape *thresh6;
    TTrackBar *Brightness6;
    TStaticText *rec6;
    TTrackBar *entropyThresh6;
    TTrackBar *DerivThresh6;
    TEdit *ID_6;
    TStaticText *StaticText79;
    TImage *Image8;
    TNumLab *bout_dur7;
    TNumLab *syll_dur7;
    TNumLab *syllable_num7;
    TShape *thresh7;
    TTrackBar *entropyThresh7;
    TTrackBar *DerivThresh7;
    TTrackBar *Brightness7;
    TStaticText *rec7;
    TEdit *ID_7;
    TStaticText *StaticText85;
    TBitBtn *Save_config;
    TBitBtn *BitBtn2;
    TLabel *deriv_ind0;
    TLabel *deriv_ind1;
    TLabel *deriv_ind2;
    TLabel *deriv_ind3;
    TLabel *deriv_ind4;
    TLabel *deriv_ind5;
    TLabel *deriv_ind6;
    TLabel *deriv_ind7;
    TLabel *ent_ind0;
    TLabel *ent_ind1;
    TLabel *ent_ind2;
    TLabel *ent_ind3;
    TLabel *ent_ind4;
    TLabel *ent_ind5;
    TLabel *ent_ind6;
    TLabel *ent_ind7;
    TImage *Image9;
    TImage *Image10;
    TImage *Image11;
    TImage *Image12;
    TImage *Image13;
    TImage *Image14;
    TImage *Image15;
    TBitBtn *show_segmentation_panel1;
    TBitBtn *show_segmentation_panel2;
    TBitBtn *show_segmentation_panel3;
    TBitBtn *show_segmentation_panel4;
    TBitBtn *show_segmentation_panel5;
    TBitBtn *show_segmentation_panel6;
    TBitBtn *show_segmentation_panel7;
    TBitBtn *show_segmentation_panel8;
    TStaticText *StaticText59;
    TCheckBox *create_bout_table;
    TLabel *Label10;
    TPanel *Panel26;
    TNumLab *advanceTb;
    TNumLab *windowTB;
    TStaticText *StaticText11;
    TTrackBar *advance;
    TStaticText *StaticText188;
    TStaticText *StaticText158;
    TTrackBar *data_window;
    TStaticText *StaticText157;
    TSpeedButton *PitchB;
    TSpeedButton *EntropyB;
    TSpeedButton *FMB;
    TSpeedButton *AM;
    TSpeedButton *PitchGoodnessB;
    TSpeedButton *DerivPowerB;
    TSpeedButton *PeakFrB;
	TBitBtn *recorder;
    TComboBox *segment_by;
    TCheckBox *smoothGraphCB;
    TCheckBox *adaptThreshCB;
    TRadioGroup *segmentation_flip;
    TNumLab *NumLab3;
    TTrackBar *hp_small;
    TTrackBar *hp_big;
    TNumLab *hp_small_num;
    TNumLab *hp_big_num;
    TPanel *secondary_segmentation_panel;
    TNumLab *secondaryThresholdTB;
    TComboBox *secondaryFeatureDD;
    TRadioGroup *secondaryComparisonRB;
    TRadioGroup *secondaryLogicRB;
    TCheckBox *secondaryFeatureCB;
    TTrackBar *min_stop_durTb;
    TTrackBar *bout_stop;
    TNumLab *min_stop_durN;
    TNumLab *bout_Stop_N;
    TPanel  *segment_panels;
	TComboBox *save0;
	TStaticText *StaticText2;
	TStaticText *StaticText3;
	TStaticText *StaticText4;
	TStaticText *StaticText22;
	TStaticText *StaticText24;
	TStaticText *StaticText25;
	TStaticText *StaticText26;
	TStaticText *StaticText27;
	TStaticText *StaticText28;
	TStaticText *StaticText29;
	TStaticText *StaticText30;
	TStaticText *StaticText31;
	TStaticText *StaticText38;
	TStaticText *StaticText41;
	TStaticText *StaticText44;
	TStaticText *StaticText47;
	TComboBox *save1;
	TComboBox *save2;
	TComboBox *save3;
	TComboBox *save4;
	TComboBox *save5;
	TComboBox *save6;
	TComboBox *save7;
	TStaticText *StaticText5;
	TStaticText *StaticText6;
	TStaticText *StaticText7;
	TStaticText *StaticText8;
	TStaticText *StaticText9;
	TStaticText *StaticText14;
	TStaticText *StaticText16;
	TStaticText *StaticText32;
	TStaticText *StaticText33;
	TStaticText *StaticText50;
	TStaticText *StaticText53;
	TStaticText *StaticText58;
	TLinkLabel *LinkLabel1;

          void __fastcall input0Click(TObject *Sender);
     void __fastcall Brightness0Change(TObject *Sender);
        void __fastcall entropyThresh0Change(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall ProcTreshTBChange(TObject *Sender);
        void __fastcall DerivThresh0Change(TObject *Sender);
        void __fastcall save1Click(TObject *Sender);
        void __fastcall PitchBClick(TObject *Sender);
        void __fastcall EntropyBClick(TObject *Sender);
        void __fastcall FMBClick(TObject *Sender);
        void __fastcall PitchGoodnessBClick(TObject *Sender);
        void __fastcall DerivPowerBClick(TObject *Sender);
        void __fastcall PeakFrBClick(TObject *Sender);
        void __fastcall startClick(TObject *Sender);
        void __fastcall output0Click(TObject *Sender);
        void __fastcall change_table0Click(TObject *Sender);
        void __fastcall stopClick(TObject *Sender);
        void __fastcall BitBtn2Click(TObject *Sender);
        void __fastcall folder_size_limitChange(TObject *Sender);
        void __fastcall chan0_inChange(TObject *Sender);
        void __fastcall chan0_outChange(TObject *Sender);
        void __fastcall new_table0Click(TObject *Sender);
        void __fastcall SyllMinDurTbChange(TObject *Sender);
        void __fastcall BoutMinDurTBChange(TObject *Sender);
        void __fastcall min_syllablesChange(TObject *Sender);
        void __fastcall AMClick(TObject *Sender);
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall slow_executionChange(TObject *Sender);
        void __fastcall folder_choiceClick(TObject *Sender);
        void __fastcall save_memClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall on_start_timerTimer(TObject *Sender);
        void __fastcall update_tablesClick(TObject *Sender);
        void __fastcall change_bird1Click(TObject *Sender);
        void __fastcall advanceChange(TObject *Sender);
        void __fastcall data_windowChange(TObject *Sender);
        void __fastcall create_bout_tableClick(TObject *Sender);
        void __fastcall BitBtn7Click(TObject *Sender);
            void __fastcall segment_byChange(TObject *Sender);
    void __fastcall segmentation_flipClick(TObject *Sender);
    void __fastcall smoothGraphCBClick(TObject *Sender);
    void __fastcall hp_smallChange(TObject *Sender);
    void __fastcall adaptThreshCBClick(TObject *Sender);
    void __fastcall hp_bigChange(TObject *Sender);
    void __fastcall secondaryFeatureCBClick(TObject *Sender);
    void __fastcall secondaryFeatureDDChange(TObject *Sender);
    void __fastcall secondaryComparisonRBClick(TObject *Sender);
    void __fastcall secondaryLogicRBClick(TObject *Sender);
    void __fastcall min_stop_durTbChange(TObject *Sender);
    void __fastcall bout_stopChange(TObject *Sender);
    void __fastcall saveClick(TObject *Sender);
        void __fastcall Save_configClick(TObject *Sender);
    void __fastcall show_segmentation_panel1Click(TObject *Sender);
	void __fastcall recorderClick(TObject *Sender);
	void __fastcall LinkLabel1LinkClick(TObject *Sender, const UnicodeString Link, TSysLinkType LinkType);


private:	// User declarations
          CZT_FeatureCalc            m_Calculator;
          AnsiString SoundFile, str, Age[8];
          void force_file_lists();
          void save_settings();
          void set_output_folders();
          void Lookfor_Files();
          void set_output_folder(int bird);
		  void save_data(int chan);
		  void update_vars();
		  void Retrieve_segmentation_to_options(int chan);
          TFileListBox *FileListBox[8];
          TLabel *file_ch[8];
          TStaticText *file_table[8];
          TEdit *chan_in[8], *chan_out[8], *ID_[8];
          TTrackBar *DerivThresh[8], *entropyThresh[8], *Brightness[8];
          TImage *image[8];
		  TStaticText *syllable_table[8], *ms_table[8], *rec[8];
          TComboBox *save[8];
          TCheckBox *sampling[8];
		  TNumLab *bout_dur[8], *syllable_dur[8], *syllable_num[8], *folder_size[8], *recnum[8], *ms_records[8], *age[8], *file_table_recs[8];
          TDate hatched[8];
		 // TChart *Chart[8];
          // float Yfactor;
         // void ReadTapers();
       //  CWavIterator wave0, wave1, wave2, wave3, wave4, wave5, wave6, wave7, wave8, wave9,wave10;
		  CWavIterator wave_files[11];
		  int active_frame;
        //  int Slaves;
          void plot_sonogram(int chan);
		  void check_tables();
          void update_file_lists();
          void edit(bool state),retrieveFeatures();
          bool Analyze_Files(int chan),  slave;
          bool IsBusy, equalizeIt, DrawIt, passed, skip_channel1, app_starting;
        //  Graphics::TBitmap *Sonogram1;
          int *Syll_types[8];
          int Syll_types0[6][100], Syll_types1[6][100],Syll_types2[6][100],Syll_types3[6][100];
          int Syll_types4[6][100], Syll_types5[6][100],Syll_types6[6][100],Syll_types7[6][100];
          float contrast[8];
		  int sIndex;
		  int pri_segmented_feature[8], pri_thresh_raw[8], sec_segmented_feature[8],
			  sec_thresh_raw[8], reverse_segmentation[8], sec_lt[8],smooth_feature[8],
			  adapt_threshold[8], sec_segmentation[8], sec_logic[8], HP_small[8], HP_big[8];
         // bool stutter[10000];

public:		// User declarations
        __fastcall Tsound_live(TComponent* Owner);
        void __fastcall CreateParams(Controls::TCreateParams &Params);
        void Calc(int chan);
		void save_frame_sf(TObject *Sender);
		//void segment_sf(TObject *Sender);

       // int fileNum1, fileNum2, fileNum3, fileNum4;//, ID1, ID2, ID3, ID4;
       // int derivThresh, entropyThresh, boutStopThresh, boutDurThresh, syllDurThresh;
};
void __fastcall Tsound_live::CreateParams(Controls::TCreateParams &Params)
{
  TForm::CreateParams(Params);
  Params.ExStyle   = Params.ExStyle | WS_EX_APPWINDOW;
  Params.WndParent = ParentWindow;
}
//---------------------------------------------------------------------------
extern PACKAGE Tsound_live *sound_live;
//---------------------------------------------------------------------------
#endif







































































