//---------------------------------------------------------------------------

#ifndef liveH
#define liveH
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
#include <TeeProcs.hpp>
#include "SDL_rchart.hpp"
#include "SDL_NumLab.hpp"
#include "SDL_rchart.hpp"
using namespace ztFFTAPI;
//---------------------------------------------------------------------------
class Tsal : public TForm
{
__published:	// IDE-managed Components
        TOpenDialog *OpenDialog1;
        TPageControl *PageControl1;
        TTabSheet *input_output;
        TTabSheet *TabSheet2;
        TBitBtn *input0;
        TTrackBar *entropyThresh1;
        TRadioGroup *image_stretch1;
        TScrollBox *ScrollBox1;
        TImage *Image1;
        TTrackBar *Brightness1;
        TTrackBar *DerivThresh1;
        TFileListBox *FileListBox1;
        TEdit *chan0_out;
        TFileListBox *FileListBox2;
        TFileListBox *FileListBox3;
        TFileListBox *FileListBox4;
        TLabel *file_ch0;
        TLabel *file_ch2;
        TLabel *Label16;
        TSpeedButton *PitchB;
        TSpeedButton *EntropyB;
        TSpeedButton *FMB;
        TSpeedButton *PeakFrB;
        TSpeedButton *PitchGoodnessB;
        TSpeedButton *DerivPowerB;
        TEdit *chan0_in;
        TBitBtn *start;
        TBitBtn *output0;
        TStaticText *rec1;
        TBitBtn *stop;
        TNumLab *max_syll_dur1;
        TNumLab *max_bout_dur1;
        TNumLab *syllable_number1;
        TStaticText *StaticText10;
        TStaticText *StaticText18;
        TStaticText *StaticText21;
        TTabSheet *options;
        TNumLab *folder_sizeN;
        TStaticText *sizeLb2;
        TTrackBar *folder_size_limit;
        TLabel *sizeLb;
        TLabel *Label6;
        TTrackBar *ProcTreshTB;
        TNumLab *ProcTreshN;
        TStaticText *StaticText12;
        TLabel *problem_filename;
        TMemo *Memo1;
        TMemo *Memo3;
        TPanel *pannel1;
        TPanel *pannel2;
        TPanel *pannel3;
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
        TMemo *Memo4;
        TNumLab *ent_ind1;
        TNumLab *deriv_ind1;
        TSpeedButton *AM;
        TBitBtn *BitBtn1;
        TTabSheet *TabSheet4;
        TChart *Chart1;
        TPointSeries *Series1;
        TPointSeries *Series2;
        TPointSeries *Series3;
        TPointSeries *Series4;
        TPointSeries *Series5;
        TPointSeries *Series6;
        TRadioGroup *folder_choice;
        TMemo *sizeH2;
        TTrackBar *gain1;
        TTrackBar *gain2;
        TTrackBar *gain3;
        TTrackBar *gain4;
        TTrackBar *gain5;
        TTrackBar *gain6;
        TTrackBar *gain7;
        TTrackBar *gain8;
        TTrackBar *gain9;
        TFileListBox *FileListBox5;
        TFileListBox *FileListBox6;
        TFileListBox *FileListBox7;
        TFileListBox *FileListBox8;
        TFileListBox *FileListBox9;
        TFileListBox *FileListBox10;
        TLabel *file_ch3;
        TLabel *file_ch4;
        TLabel *file_ch5;
        TLabel *file_ch6;
        TLabel *file_ch7;
        TLabel *file_ch8;
        TLabel *file_ch9;
        TLabel *file_ch10;
        TRChart *RChart1;
        TRChart *RChart2;
        TRChart *RChart3;
        TRChart *RChart4;
        TRChart *RChart5;
        TRChart *RChart6;
        TRChart *RChart7;
        TRChart *RChart8;
        TRChart *RChart9;
        TRChart *RChart10;
        TTrackBar *gain10;
        TLabel *file_ch1;
        TCheckBox *slave2_active;
        TCheckBox *slave3_active;
        TCheckBox *slave4_active;
        TCheckBox *slave5_active;
        TCheckBox *slave6_active;
        TCheckBox *slave7_active;
        TCheckBox *slave8_active;
        TCheckBox *slave9_active;
        TCheckBox *slave10_active;
        TStaticText *StaticText2;
        TStaticText *StaticText3;
        TStaticText *StaticText4;
        TStaticText *StaticText5;
        TStaticText *StaticText6;
        TStaticText *StaticText7;
        TStaticText *StaticText8;
        TStaticText *StaticText9;
        TStaticText *StaticText14;
        TStaticText *StaticText16;
        TCheckBox *slave1_active;
        TCheckBox *equalize1;
        TStaticText *StaticText62;
        TLabel *IDL1;
        TTimer *Timer1;
        TBitBtn *input5;
        TBitBtn *output5;
        TEdit *chan5_in;
        TEdit *chan5_out;
        TBitBtn *input6;
        TBitBtn *output6;
        TEdit *chan6_in;
        TEdit *chan6_out;
        TBitBtn *input8;
        TBitBtn *output8;
        TEdit *chan8_in;
        TEdit *chan8_out;
        TBitBtn *input9;
        TBitBtn *output9;
        TEdit *chan9_in;
        TEdit *chan9_out;
        TBitBtn *input10;
        TBitBtn *output10;
        TEdit *chan10_in;
        TEdit *chan10_out;
        TBitBtn *input1;
        TBitBtn *output1;
        TEdit *chan1_in;
        TEdit *chan1_out;
        TBitBtn *input2;
        TBitBtn *output2;
        TEdit *chan2_in;
        TEdit *chan2_out;
        TBitBtn *input3;
        TBitBtn *output3;
        TEdit *chan3_in;
        TEdit *chan3_out;
        TBitBtn *input4;
        TBitBtn *output4;
        TEdit *chan4_in;
        TEdit *chan4_out;
        TBitBtn *input7;
        TBitBtn *output7;
        TEdit *chan7_in;
        TEdit *chan7_out;
        TFileListBox *FileListBox0;
        TStaticText *ch1_Lb;
        TStaticText *ch2_Lb;
        TStaticText *ch3_Lb;
        TStaticText *ch4_Lb;
        TStaticText *ch5_Lb;
        TStaticText *ch6_Lb;
        TStaticText *ch7_Lb;
        TStaticText *ch8_Lb;
        TStaticText *ch9_Lb;
        TStaticText *ch10_Lb;
        TRadioGroup *slave_state1;
        TRadioGroup *slave_state2;
        TRadioGroup *slave_state3;
        TRadioGroup *slave_state4;
        TRadioGroup *slave_state5;
        TRadioGroup *slave_state6;
        TRadioGroup *slave_state7;
        TRadioGroup *slave_state8;
        TRadioGroup *slave_state9;
        TRadioGroup *slave_state10;
        TRadioGroup *save1;
        TLabel *Label2;
        TTrackBar *slow_execution;
        TBitBtn *help5;
        TStaticText *StaticText22;
        TStaticText *StaticText24;
        TStaticText *StaticText25;
        TStaticText *StaticText26;
        TStaticText *StaticText27;
        TStaticText *StaticText28;
        TStaticText *StaticText29;
        TStaticText *StaticText30;
        TStaticText *StaticText31;
        TStaticText *StaticText32;
        TStaticText *StaticText33;
        TBitBtn *BitBtn2;
        TStaticText *startup_state;
        TBitBtn *save_config;
        TTimer *start_timer;
        TStaticText *StaticText23;
        TEdit *ID_1;
        TStaticText *StaticText34;
        TEdit *bird_age;
        TBitBtn *features_output1;
        TBitBtn *warning1;
        TPanel *Panel1;
        TStaticText *file_table;
        TStaticText *StaticText117;
        TStaticText *StaticText56;
        TStaticText *ms_table1;
        TStaticText *syllable_table1;
        TStaticText *StaticText36;
        TNumLab *ms_recs0;
        TNumLab *recnum1;
        TStaticText *StaticText20;
        TNumLab *slaves_in_table;
        TNumLab *num_slaves;
        TStaticText *StaticText19;
        TPanel *Panel2;
        TBitBtn *new_table1;
        TBitBtn *table1;
        TStaticText *StaticText17;
        TNumLab *folder_size1;
        TSpeedButton *chang_bird1;
        TNumLab *file_table_recs0;
        TStaticText *StaticText1;
        TImage *Image9;
        TStaticText *StaticText11;
        TBitBtn *BitBtn37;
        TStaticText *saved_file1;
        TStaticText *saved_file2;
        TStaticText *saved_file3;
        TStaticText *saved_file4;
        TStaticText *saved_file5;
        TStaticText *saved_file6;
        TStaticText *saved_file7;
        TStaticText *saved_file8;
        TStaticText *saved_file0;
        TNumLab *recorder_chan;
       
          void __fastcall input0Click(TObject *Sender);
     void __fastcall Brightness1Change(TObject *Sender);
        void __fastcall entropyThresh1Change(TObject *Sender);
        void __fastcall image_stretch1Click(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall ProcTreshTBChange(TObject *Sender);
        void __fastcall DerivThresh1Change(TObject *Sender);
        void __fastcall save1Click(TObject *Sender);
        void __fastcall PitchBClick(TObject *Sender);
        void __fastcall EntropyBClick(TObject *Sender);
        void __fastcall FMBClick(TObject *Sender);
        void __fastcall PitchGoodnessBClick(TObject *Sender);
        void __fastcall DerivPowerBClick(TObject *Sender);
        void __fastcall PeakFrBClick(TObject *Sender);
        void __fastcall startClick(TObject *Sender);
        void __fastcall output0Click(TObject *Sender);
        void __fastcall features_output1Click(TObject *Sender);
        void __fastcall table1Click(TObject *Sender);
        void __fastcall stopClick(TObject *Sender);
        void __fastcall BitBtn2Click(TObject *Sender);
        void __fastcall folder_size_limitChange(TObject *Sender);
        void __fastcall chan0_inChange(TObject *Sender);
        void __fastcall ID_1Change(TObject *Sender);
        void __fastcall chan0_outChange(TObject *Sender);
        void __fastcall raw_features1Change(TObject *Sender);
        void __fastcall new_table1Click(TObject *Sender);
        void __fastcall SyllMinDurTbChange(TObject *Sender);
        void __fastcall BoutMinDurTBChange(TObject *Sender);
        void __fastcall min_syllablesChange(TObject *Sender);
        void __fastcall AMClick(TObject *Sender);
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall slow_executionChange(TObject *Sender);
        void __fastcall help5Click(TObject *Sender);
        void __fastcall folder_choiceClick(TObject *Sender);
        void __fastcall slave1_activeClick(TObject *Sender);
        void __fastcall gain1Change(TObject *Sender);
        void __fastcall warning1Click(TObject *Sender);
        void __fastcall slave_state1Click(TObject *Sender);
        void __fastcall start_timerTimer(TObject *Sender);
        void __fastcall chang_bird1Click(TObject *Sender);
        void __fastcall save_configClick(TObject *Sender);
        void __fastcall recorder_chanChange(TObject *Sender);
        void __fastcall BitBtn37Click(TObject *Sender);

private:	// User declarations
          CZT_FeatureCalc            m_Calculator;
          AnsiString SoundFile, str, Age1;
          void Reset_wavefiles(int num_slaves);
          void Delete_wavefiles(int num_slaves);
          void force_file_lists();
          void save_settings();
          void set_output_folders();
          // float Yfactor;
         // void ReadTapers();
       //  CWavIterator wave0, wave1, wave2, wave3, wave4, wave5, wave6, wave7, wave8, wave9,wave10;
          CWavIterator wave_files[11];
          int count_slaves();
          int Slaves;
          void update_file_lists();
          void edit(bool state),retrieveFeatures();
          bool Analyze_Files(),  slave;
          bool IsBusy,active1,active2,active3,active4,equalizeIt,DrawIt, passed,skip_channel1;
          Graphics::TBitmap *Sonogram1;
          int Syll_types1[6][100], Syll_types2[6][100],Syll_types3[6][100],Syll_types4[6][100];
public:		// User declarations
        __fastcall Tsal(TComponent* Owner);
        void __fastcall CreateParams(Controls::TCreateParams &Params);
        bool Calc(int num_slaves);

        int fileNum1, fileNum2, fileNum3, fileNum4;//, ID1, ID2, ID3, ID4;
       // int derivThresh, entropyThresh, boutStopThresh, boutDurThresh, syllDurThresh;
};

void __fastcall Tsal::CreateParams(Controls::TCreateParams &Params)
{
  TForm::CreateParams(Params);
  Params.ExStyle   = Params.ExStyle | WS_EX_APPWINDOW;
  Params.WndParent = ParentWindow;
}
//---------------------------------------------------------------------------
extern PACKAGE Tsal *sal;
//---------------------------------------------------------------------------
#endif
