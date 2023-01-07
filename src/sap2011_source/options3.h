//--------------------------------------------------------------------------

#ifndef options3H
#define options3H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "CRGrid.hpp"
#include "SDL_NumLab.hpp"
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include "CSPIN.h"
#include "DBAccess.hpp"
#include "MemDS.hpp"
#include "MyAccess.hpp"
#include <Db.hpp>
#include <DBCtrls.hpp>
//---------------------------------------------------------------------------
class Toption : public TForm
{
__published:	// IDE-managed Components
        TPageControl *PageControl1;
        TTabSheet *settings;
        TCRDBGrid *CRDBGrid1;
        TBitBtn *update;
        TMemo *Memo7;
        TTabSheet *TabSheet1;
    TRadioGroup *format_old;
        TRadioGroup *extractFileIndex;
        TPanel *Panel1;
        TStaticText *StaticText103;
        TEdit *in_sound;
        TStaticText *StaticText2;
        TEdit *out_sound;
        TCheckBox *Serial_by_timeCb;
        TCheckBox *allow_overwrite;
        TBitBtn *test;
        TTabSheet *TabSheet2;
        TPanel *Panel4;
        TNumLab *advanceTb;
        TNumLab *spectrum_rangeN;
        TTrackBar *advance;
        TTrackBar *spectrum_rangeTb;
        TTabSheet *TabSheet3;
        TPanel *Panel3;
        TNumLab *max_freq_Tb;
        TNumLab *min_freq_Tb;
        TBevel *Bevel1;
        TTrackBar *max_freq;
        TTrackBar *min_freq;
        TStaticText *StaticText15;
        TStaticText *StaticText16;
        TMemo *Memo2;
        TMemo *Memo1;
        TTabSheet *TabSheet6;
        TPanel *Panel6;
        TNumLab *pitchOpLb;
        TNumLab *PitchDVLB;
        TNumLab *FMOpLb;
        TNumLab *PgoodOpLb;
        TNumLab *PgoodDVLB;
        TNumLab *FMDVLB;
        TNumLab *EntropyOpLb;
        TNumLab *EntropyDVLB;
        TBevel *Bevel5;
        TTrackBar *pitchTB;
        TTrackBar *PitchDVTB;
        TStaticText *StaticText28;
        TStaticText *StaticText29;
        TStaticText *StaticText30;
        TStaticText *StaticText31;
        TTrackBar *FMTB;
        TTrackBar *FMDVTB;
        TStaticText *StaticText32;
        TTrackBar *EntropyTB;
        TTrackBar *EntropyDVTB;
        TStaticText *StaticText33;
        TTrackBar *PgoodTB;
        TTrackBar *PgoodDVTB;
        TPanel *Panel8;
        TNumLab *medianPitch;
        TNumLab *deviationPitch;
        TNumLab *medianFM;
        TNumLab *medianPeakFr;
        TNumLab *deviationPeakFr;
        TNumLab *deviationFM;
        TNumLab *medianEntropy;
        TNumLab *deviationEntropy;
        TBevel *Bevel6;
        TNumLab *medianPgood;
        TNumLab *deviationPgood;
        TNumLab *medianVarPitch;
        TNumLab *deviationVarPitch;
        TNumLab *deviationVarEnt;
        TNumLab *medianVarEnt;
        TNumLab *medianVarFM;
        TNumLab *deviationVarFM;
        TNumLab *medianVarPeakFr;
        TNumLab *deviationVarPeakFr;
        TNumLab *medianVarPGood;
        TNumLab *deviationVarPGood;
        TTrackBar *TrackBar2;
        TTrackBar *TrackBar3;
        TStaticText *StaticText35;
        TStaticText *StaticText36;
        TStaticText *StaticText37;
        TStaticText *StaticText38;
        TTrackBar *TrackBar4;
        TTrackBar *TrackBar5;
        TStaticText *StaticText39;
        TTrackBar *TrackBar6;
        TTrackBar *TrackBar7;
        TStaticText *StaticText40;
        TTrackBar *TrackBar8;
        TTrackBar *TrackBar9;
        TTrackBar *TrackBar1;
        TTrackBar *TrackBar10;
        TTrackBar *TrackBar11;
        TTrackBar *TrackBar12;
        TTrackBar *TrackBar13;
        TTrackBar *TrackBar14;
        TTrackBar *ContTB;
        TTrackBar *ContDVTB;
        TStaticText *StaticText41;
        TStaticText *StaticText42;
        TStaticText *StaticText43;
        TStaticText *StaticText44;
        TTrackBar *TrackBar15;
        TTrackBar *TrackBar16;
        TTrackBar *TrackBar17;
        TTrackBar *TrackBar18;
        TStaticText *StaticText45;
        TStaticText *StaticText46;
        TMemo *Memo5;
        TTabSheet *TabSheet4;
        TPanel *Panel2;
        TNumLab *min_syllable_durN;
        TNumLab *min_stop_durN;
        TNumLab *min_peak;
        TBevel *Bevel4;
        TNumLab *bout_Stop_N;
        TStaticText *StaticText24;
        TTrackBar *min_syllable_durTb;
        TStaticText *StaticText23;
        TStaticText *StaticText26;
        TTrackBar *min_stop_durTb;
        TStaticText *StaticText27;
        TTrackBar *min_peak_freq;
        TStaticText *StaticText1;
        TStaticText *StaticText5;
        TRadioGroup *sound_detection_terms;
        TTrackBar *bout_stop;
        TStaticText *StaticText17;
        TStaticText *StaticText18;
        TTabSheet *TabSheet5;
        TPanel *Panel5;
        TLabel *Label1;
        TNumLab *SyllDurLow;
        TLabel *Label18;
        TNumLab *BoutDurLow;
        TNumLab *mean_syllable_num;
        TTrackBar *SyllMinDurTb;
        TStaticText *StaticText10;
        TTrackBar *BoutMinDurTB;
        TStaticText *StaticText83;
        TStaticText *StaticText6;
        TTrackBar *min_syllables;
        TTabSheet *TabSheet7;
        TPanel *Panel7;
        TNumLab *pitchWLb;
        TNumLab *FMWLB;
        TNumLab *entropyWLB;
        TNumLab *goodnessWLB;
        TNumLab *TimeWarpLB;
        TTrackBar *pitchWTB;
        TTrackBar *FMWTB;
        TTrackBar *entropyWTB;
        TTrackBar *goodnessWTB;
        TCheckBox *pitchWCb;
        TCheckBox *FMWCB;
        TCheckBox *entropyWCB;
        TCheckBox *goodnessWCB;
        TTrackBar *TimeWarpTB;
        TStaticText *StaticText34;
        TMemo *Memo6;
        TTrackBar *data_window;
        TNumLab *windowTB;
        TStaticText *StaticText25;
        TStaticText *StaticText48;
        TStaticText *StaticText49;
        TRadioGroup *pitch_averaging;
        TMemo *Memo8;
        TStaticText *StaticText51;
        TStaticText *StaticText52;
        TStaticText *StaticText53;
        TStaticText *StaticText3;
        TStaticText *StaticText54;
        TStaticText *StaticText11;
        TStaticText *StaticText13;
        TMemo *Memo9;
        TStaticText *StaticText47;
        TTrackBar *TrackBar20;
        TNumLab *medianDuration;
        TTrackBar *TrackBar19;
        TNumLab *deviationDuration;
        TCheckBox *boost_amp;
        TMemo *Memo4;
        TBitBtn *start;
        TStaticText *StaticText19;
        TTrackBar *AMTB;
        TNumLab *AMOpLb;
        TTrackBar *AMDVTB;
        TNumLab *AMDVLB;
        TCheckBox *AMWCB;
        TTrackBar *AMWTB;
        TNumLab *AMWLB;
        TStaticText *StaticText86;
        TStaticText *StaticText143;
        TNumLab *noise_tb;
        TTrackBar *Noise_ratio;
        TStaticText *StaticText142;
        TStaticText *StaticText141;
	TTrackBar *Amplitude_thresh;
        TStaticText *StaticText8;
        TNumLab *amptreshL;
        TStaticText *StaticText9;
        TNumLab *NumLab2;
        TNumLab *enttreshL;
        TStaticText *StaticText58;
        TStaticText *StaticText12;
	TTrackBar *Entropy_threshold;
        TMyTable *scale;
        TDBText *DBText1;
        TDataSource *DataSource1;
        TCSpinButton *CSpinButton1;
        TBitBtn *newScale;
        TDataSource *DataSource2;
        TTrackBar *TrackBar21;
        TStaticText *StaticText59;
        TNumLab *medianStop;
        TTrackBar *TrackBar22;
        TNumLab *deviationStop;
        TRadioGroup *channel;
        TPanel *Panel9;
        TTrackBar *problems_track;
        TNumLab *problems_num;
        TStaticText *StaticText62;
        TStaticText *StaticText63;
        TCheckBox *calc_continuity;
        TTrackBar *cont_t_medianTb;
        TNumLab *cont_t_medianVal;
        TTrackBar *cont_t_madTb;
        TNumLab *cont_t_madVal;
        TStaticText *StaticText64;
        TTrackBar *cont_f_medianTb;
        TNumLab *cont_f_medianVal;
        TTrackBar *cont_f_madTb;
        TNumLab *cont_f_madVal;
        TStaticText *StaticText65;
        TTrackBar *TrackBar27;
        TNumLab *median_slope;
        TTrackBar *TrackBar28;
        TNumLab *slope_madVal;
        TStaticText *StaticText66;
        TTrackBar *TrackBar23;
        TNumLab *mean_amplitude;
        TTrackBar *TrackBar24;
        TNumLab *deviation_amplitude;
        TStaticText *StaticText67;
        TTabSheet *TabSheet8;
        TPanel *Panel10;
        TStaticText *StaticText68;
        TStaticText *StaticText70;
        TTrackBar *s_min_fr_tb;
        TTrackBar *s_max_fr_tb;
        TNumLab *s_min_fr;
        TNumLab *s_max_fr;
        TStaticText *StaticText69;
        TStaticText *StaticText71;
        TStaticText *StaticText72;
        TTrackBar *amp_baseTb;
        TNumLab *amp_base;
        TStaticText *StaticText73;
        TRadioGroup *noise_detect;
        TBevel *Bevel3;
        TRadioGroup *pitch_method;
        TPanel *dynamic_pitch;
        TStaticText *StaticText50;
        TStaticText *StaticText60;
        TTrackBar *harmonic_pitch;
        TNumLab *harmonic_pitch_threshB;
        TStaticText *StaticText14;
        TNumLab *goodness_thresh;
        TTrackBar *goodness_thresh_TB;
        TStaticText *StaticText20;
        TStaticText *StaticText57;
        TTrackBar *entropy_threshold_TB;
        TNumLab *Entropy_thresh;
        TPanel *cepst_bounds;
        TStaticText *StaticText56;
        TTrackBar *upper_cepst;
        TNumLab *upper_cepstN;
        TStaticText *StaticText21;
        TStaticText *StaticText22;
        TNumLab *lower_cepstN;
        TTrackBar *lower_cepst;
        TStaticText *StaticText55;
        TStaticText *StaticText74;
        TRadioGroup *age_or_day;
        TStaticText *StaticText125;
        TStaticText *StaticText144;
        TTrackBar *frequency_range;
        TNumLab *frequency_range_val;
        TStaticText *StaticText126;
        TStaticText *StaticText75;
	TTrackBar *sldrBrightness;
	TStaticText *StaticText76;
    TPanel *segmentation_pannel;
    TNumLab *hp_big_num;
    TNumLab *hp_small_num;
    TTrackBar *HP_small;
    TTrackBar *HP_big;
    TStaticText *StaticText224;
    TStaticText *StaticText225;
    TSpeedButton *fine_segmentation;
    TRadioGroup *format;
    TMemo *Memo3;
    TStaticText *StaticText61;
    TTrackBar *contour_biasTB;
    TTrackBar *contour_thresh;
    TStaticText *StaticText4;
    TPanel *Panel11;
    TNumLab *state_transition_Num;
    TNumLab *missmatchesNum;
    TTrackBar *state_transition_TB;
    TTrackBar *missmatchesTb;
    TStaticText *StaticText214;
    TStaticText *StaticText216;
	TPanel *Panel12;
	TNumLab *minfr;
	TStaticText *StaticText79;
	TTrackBar *minfr_tb;
	TStaticText *StaticText80;
	TStaticText *StaticText81;

         void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall min_freqChange(TObject *Sender);
        void __fastcall max_freqChange(TObject *Sender);
        void __fastcall advanceChange(TObject *Sender);
        void __fastcall spectrum_rangeTbChange(TObject *Sender);
        void __fastcall min_syllablesChange(TObject *Sender);
        void __fastcall min_peak_freqChange(TObject *Sender);
        void __fastcall BoutMinDurTBChange(TObject *Sender);
        void __fastcall SyllMinDurTbChange(TObject *Sender);
        void __fastcall upper_cepstChange(TObject *Sender);
        void __fastcall lower_cepstChange(TObject *Sender);
        void __fastcall pitch_averagingClick(TObject *Sender);
        void __fastcall harmonic_pitchChange(TObject *Sender);
        void __fastcall min_syllable_durTbChange(TObject *Sender);
        void __fastcall min_stop_durTbChange(TObject *Sender);
        void __fastcall Serial_by_timeCbClick(TObject *Sender);
        void __fastcall sound_detection_termsClick(TObject *Sender);
        void __fastcall bout_stopChange(TObject *Sender);
        void __fastcall TrackBar2Change(TObject *Sender);
        void __fastcall TrackBar4Change(TObject *Sender);
        void __fastcall TrackBar6Change(TObject *Sender);
        void __fastcall TrackBar8Change(TObject *Sender);
        void __fastcall TrackBar1Change(TObject *Sender);
        void __fastcall TrackBar5Change(TObject *Sender);
        void __fastcall TrackBar11Change(TObject *Sender);
        void __fastcall TrackBar14Change(TObject *Sender);
        void __fastcall ContTBChange(TObject *Sender);
        void __fastcall TrackBar15Change(TObject *Sender);
        void __fastcall TrackBar17Change(TObject *Sender);
        void __fastcall TrackBar20Change(TObject *Sender);
        void __fastcall TrackBar3Change(TObject *Sender);
        void __fastcall TrackBar7Change(TObject *Sender);
        void __fastcall TrackBar9Change(TObject *Sender);
        void __fastcall TrackBar10Change(TObject *Sender);
        void __fastcall TrackBar12Change(TObject *Sender);
        void __fastcall TrackBar13Change(TObject *Sender);
        void __fastcall ContDVTBChange(TObject *Sender);
        void __fastcall TrackBar16Change(TObject *Sender);
        void __fastcall TrackBar18Change(TObject *Sender);
        void __fastcall TrackBar19Change(TObject *Sender);
        void __fastcall allow_overwriteClick(TObject *Sender);
        void __fastcall TimeWarpTBChange(TObject *Sender);
        void __fastcall pitchWTBChange(TObject *Sender);
        void __fastcall FMWTBChange(TObject *Sender);
        void __fastcall entropyWTBChange(TObject *Sender);
        void __fastcall goodnessWTBChange(TObject *Sender);
        void __fastcall pitchWCbClick(TObject *Sender);
        void __fastcall FMWCBClick(TObject *Sender);
        void __fastcall entropyWCBClick(TObject *Sender);
        void __fastcall contWCBClick(TObject *Sender);
        void __fastcall updateClick(TObject *Sender);
        void __fastcall testClick(TObject *Sender);
        void __fastcall data_windowChange(TObject *Sender);
        void __fastcall goodness_thresh_TBChange(TObject *Sender);
        void __fastcall entropy_threshold_TBChange(TObject *Sender);
        void __fastcall pitchTBChange(TObject *Sender);
        void __fastcall FMTBChange(TObject *Sender);
        void __fastcall EntropyTBChange(TObject *Sender);
        void __fastcall PgoodTBChange(TObject *Sender);
        void __fastcall PitchDVTBChange(TObject *Sender);
        void __fastcall FMDVTBChange(TObject *Sender);
        void __fastcall EntropyDVTBChange(TObject *Sender);
        void __fastcall PgoodDVTBChange(TObject *Sender);
        void __fastcall boost_ampClick(TObject *Sender);
        void __fastcall startClick(TObject *Sender);
        void __fastcall AMTBChange(TObject *Sender);
        void __fastcall AMDVTBChange(TObject *Sender);
        void __fastcall AMWTBChange(TObject *Sender);
        void __fastcall Amplitude_threshChange(TObject *Sender);
        void __fastcall Entropy_thresholdChange(TObject *Sender);
        void __fastcall CSpinButton1DownClick(TObject *Sender);
        void __fastcall CSpinButton1UpClick(TObject *Sender);
        void __fastcall newScaleClick(TObject *Sender);
        void __fastcall TrackBar21Change(TObject *Sender);
        void __fastcall TrackBar22Change(TObject *Sender);
        void __fastcall problems_trackChange(TObject *Sender);
        void __fastcall calc_continuityClick(TObject *Sender);
        void __fastcall cont_t_medianTbChange(TObject *Sender);
		void __fastcall cont_t_madTbChange(TObject *Sender);
        void __fastcall cont_f_medianTbChange(TObject *Sender);
        void __fastcall cont_f_madTbChange(TObject *Sender);
        void __fastcall s_min_fr_tbChange(TObject *Sender);
        void __fastcall s_max_fr_tbChange(TObject *Sender);
        void __fastcall amp_baseTbChange(TObject *Sender);
        void __fastcall noise_directionClick(TObject *Sender);
        void __fastcall pitch_methodClick(TObject *Sender);
        void __fastcall age_or_dayClick(TObject *Sender);
        void __fastcall frequency_rangeChange(TObject *Sender);
    void __fastcall fine_segmentationClick(TObject *Sender);
    void __fastcall HP_bigChange(TObject *Sender);
    void __fastcall HP_smallChange(TObject *Sender);
	void __fastcall minfr_tbChange(TObject *Sender);




private:	// User declarations
AnsiString str;
float frequency_range_factor;
int monthToInt(AnsiString x);
void create_feature_scale();
void add_record_to_scale(AnsiString str);
void frequency_range_adjustments();

public:		// User declarations
        __fastcall Toption(TComponent* Owner);

        void __fastcall CreateParams(Controls::TCreateParams &Params);
        void setScale();
        float adjustThreshold(int feature, float thresh, int frequency, bool pri);
		int ztSamplesPerWindow,squiz, baseline, module;
		int ms_from_midnight, chan;
        bool contour_pitch, dynamic_pitch_calculation, peak_frequency_pitch;
        double serial_number, hp_big, hp_small, hp_big1, hp_small1, hp_big2, hp_small2;
		float harmonic_pitch_thresh, syllable_duration_thresh, bout_duration_thresh
		, stop_duration_thresh, amplitude_thresh, entropy_thresh, syllCounter, minimum_syllables_per_files
        , noise_thresh_freq, min_entropy_freq, Advance, max_entropy_freq, upper_pitch_bound, frequency_scale, sampling_rate
        , lower_pitch_bound, pitch_goodness_thresh,pitch_entropy_thresh, max_peak_frequency, high_freq_rejection, noise_ratio;
	   //segmentation specific stuff
	   //float pri_thresh, sec_thresh;
       float frequency;
       float pri_thresh_raw, sec_thresh_raw, pri_thresh_raw1, sec_thresh_raw1, pri_thresh_raw2, sec_thresh_raw2;
	   int pri_segmented_feature, sec_segmented_feature, sec_lt, sec_logic;
       int pri_segmented_feature1, sec_segmented_feature1, sec_lt1, sec_logic1;
       int pri_segmented_feature2, sec_segmented_feature2, sec_lt2, sec_logic2;
	   bool smooth_feature, adapt_threshold, sec_segmentation, doNotSegment;
       bool smooth_feature2, adapt_threshold2, sec_segmentation2, doNotSegment2;
	   bool smooth_feature1, adapt_threshold1, sec_segmentation1, doNotSegment1;
	   bool cepstrum_pitch;
	   int calculateLocation;
	   int currentStartLocation, currentStartLocation2;
	   int pageNumber, pageNumber2;
	   int totalPages, totalPages2;
	   int minFreq;
	   bool paginationOn, paginationOn2, openFromSound1;
	   bool show_hpfilt, update_segmentation;
       // int ztSamplesPerWindow;   // default 409 samples
        int spectrum_range, min_syllable_duration, min_stop_duration, select_table;
        int slave_mode[10];  // this array keeps the mode for amp calculation of each slave channel: 0: avg, 1: rectified, 2: squer
        int File_index, problems_thresh, ms_recordes_added;
        int bird_age;
        TDateTime file_age;
        AnsiString syllable_table_name, wave_names[11], raw_feature_file,  raw_feature_folder, moveTo_sound_folders[12]
                   , problem_file, Message, File_to_delete, features_table;//, file_table; 

        //__int16 bird_ID;
        AnsiString bird_ID;
        int bitmapHeight, segmentation_feature, custom_segmentation_thresh;
        int slow_execution, year, month, day, minute, hour, second, msec, types[12][6]; // types keep syllable categories, see in ztfeaturecalc
        bool calculate_continuity, saveRawFile, saveRecFiles, delete_trash_file, doubleSamples,  show_start, simple_pitch_averaging ,check_overwrite
            , move_sound, serial_by_time, keep_file_name, entropy_and_power, entropy_or_power, halt, Draw_sonogram,save_image_mem
            , DVD, keep_original_rawfile_name, create_feature_arrays, tapers_done, DeleteFile_failed, boost_amplitude
			,detect_low_noise, save_all, refine_segmentation, brain_mode, quick_cluster, age_in_table, sono_display
			,range_x2, range_x05, slave_features, custom_parse, hp_amp, warning, reverse_segmentation, reverse_segmentation1, reverse_segmentation2;//, testing;

        void extract_file_attributes(AnsiString wave_name);
};

void __fastcall Toption::CreateParams(Controls::TCreateParams &Params)
{
  TForm::CreateParams(Params);
  Params.ExStyle   = Params.ExStyle | WS_EX_APPWINDOW;
  Params.WndParent = ParentWindow;
}
//---------------------------------------------------------------------------
extern PACKAGE Toption *option;
//---------------------------------------------------------------------------
#endif
