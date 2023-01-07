//--------------------------------------------------------------------------
#ifndef SongMeasuresH
#define SongMeasuresH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>

#include "stdlib.h"
#include "SDL_filesys.hpp"

#include "SDL_NumLab.hpp"
#include "SDL_Fourier.hpp"
#include "SDL_matrix.hpp"
#include <vcl\Clipbrd.hpp>
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
#include <fstream.h>
#include <stdio.h>
#include "SDL_RChart.hpp"
#include "SDL_Rot3D.hpp"
#include "CZT_FFT_Utils_Framework.h"
#include "CWavIterator.h"
#include "CZT_FeatureCalc.h"
#include <Db.hpp>
#include <DBGrids.hpp>
#include <DBTables.hpp>
#include <Grids.hpp>
#include <Graphics.hpp>
#include <FileCtrl.hpp>
#include <AppEvnts.hpp>
#include <MPlayer.hpp>
#include <Menus.hpp>
#include "cspin.h"
#include "AdvSmoothJogWheel.hpp"
#include "open_soundD.h"
#include "DBAccess.hpp"
#include "MyAccess.hpp"
#include <DBCtrls.hpp>
#include <ActnList.hpp>
#include <CheckLst.hpp>
#include "SDL_marksel.hpp"
#include "SDL_replist.hpp"
#include "segmentation.h"
#include <algorithm>
#include "SDL_marksel.hpp"
#include "Htmlhelp.h"
#include "SDL_ntabed.hpp"
#include "SDL_replist.hpp"
#include "preview.h"


#define Xdir         0
#define Ydir         1
#define Kdecending   1
#define Kacending   -1
#define Ksequence    0
#define Kvalue       1
#define Ksong1       0
#define Ksong2       1

//---------------------------------------------------------------------------

class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TSaveDialog *SaveDialog1;
        TSavePictureDialog *SaveGraph;
        TPrintDialog *PrintDialog1;
        TPrinterSetupDialog *PrinterSetupDialog1;
        TApplicationEvents *ApplicationEvents1;
        TMatrix *area;
        TMainMenu *MainMenu1;
        TMenuItem *File1;
        TMenuItem *Open1;
        TMenuItem *SaveAs1;
        TMenuItem *Exit1;
        TMenuItem *Edit2;
        TMenuItem *View1;
        TMenuItem *Similarity1;
        TMenuItem *Help1;
        TMenuItem *Help2;
        TMenuItem *AboutSoundAnalysisPro1;
        TMenuItem *Score1;
        TMenuItem *Reset1;
        TMenuItem *Savescore1;
        TMenuItem *Play3;
        TMenuItem *Sound11;
        TMenuItem *Sound21;
        TMenuItem *Close1;
        TMenuItem *Options1;
        TMenuItem *Generaloptions1;
        TMenuItem *Sounds121;
        TMenuItem *Features1;
        TMenuItem *Amplitude_m;
        TMenuItem *Pitch_m;
        TMenuItem *MeanFrequency_m;
        TMenuItem *Principalcontour_m;
        TMenuItem *Goodnessofpitch_m;
        TMenuItem *Frequencymodulation_m;
        TMenuItem *Amplitudemodulation_m;
        TMenuItem *Wienerentropy_m;
        TMenuItem *Durationofstate_m;
        TMenuItem *Continuityovertime_m;
        TMenuItem *Continuityoverspace_m;
        TMenuItem *Saverawfeatures1;
        TMenuItem *Save1;
        TMenuItem *ExploreScoreoptions1;
        TMenuItem *Similarityscorehelp1;
        TMenuItem *Exploringsoundshelp1;
        TMenuItem *Savingandexportingsounds1;
        TMenuItem *Featurespace1;
        TMenuItem *Visuallycomparingsounds1;
        TScrollBox *ScrollBox1;
        TSaveDialog *WaveSaveDialog;
        TOpenDialog *OpenDialog1;
	TMenuItem *ou1;
    TMenuItem *Hints1;
    TMenuItem *On1;
    TMenuItem *Off1;
    TPopupMenu *save_features;
    TMenuItem *save_features_at_pointer;
    TMenuItem *save_rawfeatures;
    TMenuItem *save_mean_features;
    TMenuItem *Playsound1;
    TMenuItem *Goaway1;
    TPageControl *display;
    TTabSheet *Song1;
    TNumLab *entropy_val;
    TNumLab *deriv_val;
    TLabel *Label5;
    TNumLab *contour_thresh_val;
    TImage *gainIc;
    TLabel *ent_label;
    TNumLab *windowTB;
    TNumLab *advanceTb;
    TNumLab *frequency_range_val;
    TNumLab *range_up_val;
    TNumLab *range_down_val;
    TGroupBox *analysis_view;
    TStaticText *StaticText104;
    TStaticText *StaticText105;
    TPageControl *data_control1;
    TTabSheet *clustering;
    TBitBtn *clusters;
    TBitBtn *cluster_table;
    TStaticText *StaticText80;
    TTrackBar *entropyTb;
    TTrackBar *derivTB;
    TTrackBar *sldrBrightness;
    TBitBtn *newB;
    TBitBtn *help5;
    TStaticText *StaticText134;
    TTrackBar *contour_thresh;
    TMediaPlayer *song_play;
    TStaticText *StaticText158;
    TTrackBar *data_window;
    TStaticText *StaticText157;
    TStaticText *StaticText106;
    TTrackBar *advance;
    TStaticText *StaticText188;
    TTrackBar *frequency_range;
    TStaticText *StaticText144;
    TStaticText *StaticText126;
    TRadioGroup *deriv_display;
    TScrollBox *ScrollBox3;
    TImage *Image1;
    TShape *range_down_shading;
    TShape *range_up_shading;
    TLabel *songname;
    TStaticText *y1;
    TStaticText *y2;
    TStaticText *y3;
    TStaticText *y4;
    TStaticText *StaticText245;
    TStaticText *y5;
    TBitBtn *update_display;
    TTrackBar *range_down;
    TTrackBar *range_up;
    TTabSheet *Song2;
    TNumLab *entropy_val2;
    TNumLab *contour_thresh_val2;
    TImage *Image5;
    TNumLab *deriv_val2;
    TLabel *Label10;
    TLabel *Label11;
    TNumLab *windowTB2;
    TNumLab *advanceTb2;
    TNumLab *frequency_range2_val;
    TLabel *ent_label2;
    TNumLab *range_up_val2;
    TNumLab *range_down_val2;
    TScrollBox *ScrollBox4;
    TImage *Image2;
    TLabel *SongName2a;
    TShape *range_down_shading2;
    TShape *range_up_shading2;
    TStaticText *y5b;
    TStaticText *y4b;
    TStaticText *y3b;
    TStaticText *y2b;
    TStaticText *y1b;
    TStaticText *StaticText238;
    TTrackBar *entropyTb2;
    TTrackBar *derivTB2;
    TTrackBar *sldrBrightness2;
    TBitBtn *new2B;
    TBitBtn *BitBtn25;
    TTrackBar *contour_thresh2;
    TBitBtn *BitBtn33;
    TStaticText *StaticText152;
    TMediaPlayer *play_song2;
    TStaticText *StaticText121;
    TTrackBar *data_window2;
    TStaticText *StaticText122;
    TStaticText *StaticText123;
    TTrackBar *advance2;
    TStaticText *StaticText124;
    TTrackBar *frequency_range2;
    TStaticText *StaticText145;
    TStaticText *StaticText159;
    TRadioGroup *deriv_display2;
    TTrackBar *range_down2;
    TTrackBar *range_up2;
    TBitBtn *update_display2;
    TTabSheet *Features_space;
    TLabel *warning;
    TRot3D *feature_space;
    TBitBtn *DrawB;
    TBitBtn *ClearB;
    TBitBtn *printB;
    TBitBtn *BitBtn1;
    TGroupBox *featuresBox2;
    TCheckBox *FMFS;
    TCheckBox *pitchFS;
    TCheckBox *entropyFS;
    TCheckBox *pgoodFS;
    TCheckBox *pitchDVFS;
    TCheckBox *FMDVFS;
    TCheckBox *entropyDVFS;
    TCheckBox *continuityDVFS;
    TCheckBox *pitchKSFS;
    TCheckBox *FMKSFS;
    TCheckBox *entropyKSFS;
    TCheckBox *contKSFS;
    TCheckBox *OverallDVFS;
    TCheckBox *overallKSFS;
    TCheckBox *informationFS;
    TCheckBox *periodFS;
    TCheckBox *pitchPrFS;
    TCheckBox *FMPrFS;
    TCheckBox *entropyPrFS;
    TCheckBox *contPrFS;
    TCheckBox *overallPrFS;
    TBitBtn *ResetLine;
    TGroupBox *ChoiseBox;
    TSpeedButton *DrawAll;
    TSpeedButton *GlobalPlotB;
    TRadioGroup *MouseAction;
    TScrollBox *ScrollBox6;
    TImage *Image8;
    TRadioGroup *SelectSound;
    TRadioGroup *FS_plot_options;
    TGroupBox *featuresBox1;
    TCheckBox *FS_pitch;
    TCheckBox *FS_principalCF;
    TCheckBox *FS_goodness;
    TCheckBox *FS_FM;
    TCheckBox *FS_slope;
    TCheckBox *FS_AM;
    TCheckBox *FS_entropy;
    TCheckBox *FS_continuity_t;
    TCheckBox *FS_continuity_fr;
    TCheckBox *FS_amp;
    TCheckBox *FS_meanFr;
    TTabSheet *Similarity;
    TScrollBox *ScrollBox5;
    TImage *Image4;
    TImage *Similarity_image;
    TImage *Image3;
    TPanel *Panel5;
    TSpeedButton *LocalScoreB;
    TSpeedButton *LocalPitchScoreB;
    TSpeedButton *LocalModulScoreB;
    TSpeedButton *LocalEntrScoreB;
    TSpeedButton *LocalContScoreB;
    TSpeedButton *GlobContScoreB;
    TSpeedButton *GlobEntrScoreB;
    TSpeedButton *GlobModuScoreB;
    TSpeedButton *GlobPitchScoreB;
    TSpeedButton *GlobalScoreB;
    TSpeedButton *LocalAMScoreB;
    TSpeedButton *GlobAMScoreB;
    TSpeedButton *OverallScoreB;
    TPageControl *similarity_control;
    TTabSheet *score_results;
    TPanel *ScorePannel;
    TNumLab *similarityN;
    TNumLab *accuracyN;
    TNumLab *sequentialN;
    TNumLab *scoreN;
    TLabel *songname1b;
    TLabel *songname2b;
    TStaticText *StaticText73;
    TCheckBox *accuracyCB;
    TCheckBox *sequentialCB;
    TCheckBox *similarityCB;
    TBitBtn *save_score;
    TBitBtn *delete_similarity_recs;
    TTabSheet *Combined;
    TScrollBox *show_song2;
    TImage *Image7;
    TStaticText *comp_sound2;
    TPanel *ks_panel;
    TNumLab *ks_pitch;
    TNumLab *ks_FM;
    TNumLab *ks_entropy;
    TNumLab *ks_Amplitude;
    TNumLab *ks_peak;
    TNumLab *ks_goodness;
    TNumLab *ks_all;
    TNumLab *syll_Score;
    TStaticText *StaticText11;
    TStaticText *StaticText34;
    TStaticText *StaticText66;
    TStaticText *StaticText67;
    TStaticText *StaticText71;
    TStaticText *StaticText77;
    TStaticText *StaticText78;
    TStaticText *StaticText108;
    TBitBtn *saveScoreSegm;
    TBitBtn *BitBtn16;
    TCheckBox *calc_KS;
    TBitBtn *BitBtn18;
    TScrollBox *ScrollBox7;
    TImage *Image6;
    TStaticText *comp_sound1;
    TTabSheet *simlilarity_batch;
    TSpeedButton *paste14;
    TSpeedButton *cut14;
    TSpeedButton *copy14;
    TBitBtn *add;
    TBitBtn *add_all;
    TListBox *List1;
    TStaticText *StaticText92;
    TBitBtn *DoBatch;
    TRadioGroup *MtimesN;
    TStaticText *StaticText93;
    TListBox *List2;
    TPanel *Panel8;
    TLabel *counter;
    TShape *st;
    TStaticText *StaticText94;
    TStaticText *StaticText95;
    TStaticText *StaticText96;
    TEdit *Edit1;
    TBitBtn *BitBtn2;
    TRadioGroup *data_type;
    TDriveComboBox *DriveComboBox1;
    TDirectoryListBox *DirectoryListBox1;
    TFileListBox *soundList;
    TMemo *batch_log;
    TBitBtn *breakIt;
    TPanel *target_score;
    TStaticText *querry_text;
    TEdit *selection;
    TStaticText *StaticText131;
    TEdit *folder;
    TStaticText *StaticText135;
    TBitBtn *BitBtn21;
    TStaticText *StaticText132;
    TEdit *to_ms;
    TStaticText *StaticText136;
    TStaticText *StaticText137;
    TStaticText *StaticText133;
    TEdit *iterations;
    TStaticText *StaticText138;
    TStaticText *StaticText139;
    TEdit *buffer;
    TStaticText *StaticText140;
    TBitBtn *batch_table2_open;
    TStaticText *StaticText146;
    TStaticText *table1L;
    TStaticText *StaticText148;
    TStaticText *table1_numL;
    TStaticText *batch_tablename2;
    TStaticText *table2L;
    TStaticText *batch_count_table2;
    TStaticText *table2_numL;
    TStaticText *batch_folder2;
    TEdit *folder2;
    TEdit *to_ms_start;
    TStaticText *StaticText149;
    TStaticText *StaticText151;
    TStaticText *StaticText153;
    TStaticText *StaticText154;
    TStaticText *StaticText155;
    TStaticText *StaticText156;
    TBitBtn *input2;
    TBitBtn *folder2_btn;
    TStaticText *StaticText115;
    TStaticText *StaticText116;
    TStaticText *StaticText160;
    TBitBtn *BitBtn22;
    TBitBtn *BitBtn26;
    TBitBtn *BitBtn27;
    TBitBtn *BitBtn28;
    TBitBtn *BitBtn29;
    TTabSheet *Rhythm;
    TSpeedButton *copy13;
    TSpeedButton *cut13;
    TSpeedButton *paste13;
    TRChart *rhythm_chart;
    TBitBtn *calc_rhythm;
    TStaticText *StaticText217;
    TStaticText *StaticText218;
    TMemo *rhythm_data;
    TRadioGroup *rhythm_range;
    TRadioGroup *rhythm_feature;
    TRadioGroup *rhythm_filter;
    TRadioGroup *zoom;
    TCheckBox *print_rhythm;
    TTabSheet *Viewer;
    TImage *Image10;
    TImage *Image11;
    TImage *Image12;
    TImage *Image13;
    TImage *Image14;
    TNumLab *advance_viewerTb;
    TNumLab *viewer_freq;
    TImage *Image15;
    TLabel *Label12;
    TNumLab *page;
    TBitBtn *open_viewer;
    TBitBtn *reset;
    TBitBtn *forward;
    TBitBtn *backwards;
    TStaticText *StaticText219;
    TTrackBar *advance_viewer;
    TStaticText *StaticText220;
    TStaticText *StaticText221;
    TTrackBar *viewer_range;
    TStaticText *StaticText222;
    TTrackBar *gain3;
    TStaticText *StaticText223;
    TStaticText *StaticText233;
    TTabSheet *options;
    TPanel *Panel13;
    TNumLab *min_peak;
    TNumLab *noise_tb;
    TNumLab *contour_bias;
    TNumLab *contour_bias2;
    TStaticText *StaticText28;
    TRadioGroup *noise_detect;
    TBitBtn *BitBtn23;
    TStaticText *StaticText141;
    TStaticText *StaticText142;
    TTrackBar *min_peak_freq;
    TTrackBar *noise_ratio;
    TStaticText *StaticText86;
    TStaticText *StaticText143;
    TCheckBox *adjust_pitch;
    TCheckBox *equalize;
    TCheckBox *calc_contours;
    TCheckBox *warnings;
    TRadioGroup *sound_detection_terms;
    TTrackBar *contour_biasTB;
    TTrackBar *contour_biasTB2;
    TBitBtn *BitBtn5;
    TBitBtn *matlab;
    TRadioGroup *pitch_method;
    TRadioGroup *age_or_day;
    TStaticText *StaticText125;
    TMemo *Memo1;
    TStaticText *StaticText89;
    TPanel *Panel2;
    TNumLab *state_transition_Num;
    TNumLab *missmatchesNum;
    TBitBtn *calcnote;
    TTrackBar *state_transition_TB;
    TStaticText *StaticText214;
    TStaticText *StaticText215;
    TStaticText *StaticText216;
    TTrackBar *missmatchesTb;
    TCheckBox *smoothy;
    TPopupMenu *draw_features;
    TMenuItem *Draw1;
    TPopupMenu *mouse_action;
    TMenuItem *Rotate1;
    TMenuItem *Pan1;
    TMenuItem *Zoom1;
    TColorBox *ColorBox;
    TColorDialog *ColorDialog;
    TMenuItem *Color1;
    TMenuItem *Goway1;
    TMarkSel *MarkSel;
    TMenuItem *gotoothersound1;
    TMenuItem *similarity2;
    TCheckBox *smoothi;
    TBitBtn *BitBtn31;
    TBitBtn *BitBtn37;
    TNumLab *contrast1;
    TNumLab *contrast2;
    TPageControl *features_display;
    TTabSheet *features_at_pointer;
    TTabSheet *display_means;
    TPanel *Panel6;
    TNumLab *pitchL;
    TNumLab *modulationL;
    TNumLab *PeakFreqL;
    TNumLab *entropyL;
    TNumLab *AmplitudeL;
    TNumLab *continuity_tN1;
    TNumLab *continuity_fN1;
    TNumLab *Principal_frN1;
    TNumLab *slopeN1;
    TNumLab *PgoodL;
    TNumLab *AML;
    TPanel *IntervalPanel;
    TNumLab *pitchAvgL;
    TNumLab *FMAvgL;
    TNumLab *peakFreqAvgL;
    TNumLab *beginL;
    TNumLab *endL;
    TNumLab *pgoodAvgL;
    TNumLab *pitchMinL;
    TNumLab *FMminL;
    TNumLab *peakFreqMinL;
    TNumLab *pgoodMinL;
    TNumLab *pitchMaxL;
    TNumLab *FMmaxL;
    TNumLab *peakFreqMaxL;
    TNumLab *pgoodMaxL;
    TNumLab *durationL;
    TNumLab *entAvgL;
    TNumLab *entMinL;
    TNumLab *entMaxL;
    TNumLab *pitchVarL;
    TNumLab *FMVarL;
    TNumLab *entVarL;
    TNumLab *pgoodVarL;
    TNumLab *peakFreqVarL;
    TNumLab *Pos_maxAmp1;
    TNumLab *Pos_maxFreq1;
    TNumLab *derivAvgL;
    TNumLab *derivMinL;
    TNumLab *derivMaxL;
    TNumLab *derivVarL;
    TNumLab *principalFreqAvgL;
    TNumLab *principalFreqMinL;
    TNumLab *principalFreqMaxL;
    TNumLab *principalFreqVarL;
    TNumLab *FMcAvgL;
    TNumLab *FMcMinL;
    TNumLab *FMcMaxL;
    TNumLab *FMcVarL;
    TNumLab *AMAvgL;
    TNumLab *AMMinL;
    TNumLab *AMMaxL;
    TNumLab *AMVarL;
    TNumLab *cont_tAvgL;
    TNumLab *cont_tMinL;
    TNumLab *cont_tMaxL;
    TNumLab *cont_tVarL;
    TNumLab *cont_fAvgL;
    TNumLab *cont_fMinL;
    TNumLab *cont_fMaxL;
    TNumLab *cont_fVarL;
    TStaticText *StaticText40;
    TStaticText *StaticText70;
    TStaticText *StaticText74;
    TStaticText *txt11;
    TStaticText *StaticText147;
    TBitBtn *help4;
    TNumLab *NumLab1;
    TNumLab *timeL;
    TReportListView *feature_at_pointer_list;
    TTabSheet *vector_display;
    TBitBtn *save_xml;
    TBitBtn *features_to_clipboard;
    TSpeedButton *DerivPowerB;
    TSpeedButton *PitchB;
    TSpeedButton *PowerB;
    TSpeedButton *principal_frB;
    TSpeedButton *PitchGoodnessB;
    TSpeedButton *FMB;
    TSpeedButton *AMB;
    TSpeedButton *EntropyB;
    TSpeedButton *continulity_tB;
    TSpeedButton *continuity_fB;
    TSpeedButton *persistantB;
	TLabel *Label6;
    TPanel *feature_buttons;
    TBitBtn *delete_feature_row;
    TReportListView *feature_vector_list;
    TBitBtn *feature_vector_to_clipboard;
    TBitBtn *feature_vector_XML;
    TBitBtn *delete_featurevector_rows;
    TBitBtn *Clear_featurelist;
    TBitBtn *delete_featurevector;
    TReportListView *feature_statistics_View;
    TStaticText *StaticText1;
    TStaticText *StaticText3;
    TStaticText *StaticText4;
    TStaticText *StaticText5;
    TBitBtn *AddBt;
    TRadioGroup *raw_features;
    TEdit *raw_features1;
    TBitBtn *add_raw_features;
    TSpeedButton *mfaB;
    TBitBtn *copy_stats;
    TBitBtn *clear_selection;
	TButton *goBackDisplay;
	TButton *advanceDisplay;
    TImage *preview_sono;
    TShape *preview_cursor;
    TTabSheet *display_opt;
    TCheckBox *auto_gain;
    TCheckBox *autosave_image;
    TBitBtn *cut;
    TSpeedButton *group;
    TLabel *Label8;
    TLabel *Label13;
    TShape *drag_cursor;
//<<<<<<< .mine
    TButton *Preview;
    TImage *preview_sono2;
    TShape *preview_cursor2;
    TButton *Preview2;
    TPageControl *data_control2;
    TTabSheet *TabSheet9;
    TStaticText *StaticText205;
    TStaticText *bird_name2;
    TStaticText *StaticText206;
    TStaticText *age2;
    TStaticText *StaticText209;
    TStaticText *details2;
    TBitBtn *new_bird2;
    TStaticText *StaticText229;
    TMemo *file_comments2;
    TTabSheet *TabSheet2;
    TBitBtn *cluster2;
    TBitBtn *BitBtn12;
    TPageControl *features_display2;
    TTabSheet *features_at_pointer2;
    TTabSheet *vector_display2;
    TTabSheet *display_means2;
    TPanel *Panel7;
    TSpeedButton *Pitch2B;
    TSpeedButton *Power2B;
    TSpeedButton *Entropy2B;
    TSpeedButton *FM2B;
    TSpeedButton *PitchGoodnessB2;
    TSpeedButton *AM2B;
    TSpeedButton *DerivPower2B;
    TSpeedButton *principal_frB2;
    TSpeedButton *persistantB2;
    TSpeedButton *continuity_fB2;
    TSpeedButton *continulity_tB2;
    TBitBtn *BitBtn35;
    TSpeedButton *mfaB2;
    TLabel *Label14;
    TReportListView *feature_at_pointer_list2;
    TNumLab *time2L;
    TNumLab *frequency2L;
    TStaticText *StaticText25;
    TStaticText *StaticText23;
    TNumLab *Amplitude2L;
    TNumLab *pitch2L;
    TNumLab *PeakFreq2L;
    TNumLab *Principal_frN2;
    TNumLab *PgoodL2;
    TNumLab *modulation2L;
    TNumLab *AM2L;
    TNumLab *entropy2L;
    TNumLab *continuity_tN2;
    TNumLab *continuity_fN2;
    TNumLab *slopeN2;
	TBitBtn *features_to_clipboard2;
    TBitBtn *BitBtn8;
    TBitBtn *BitBtn9;
    TBitBtn *BitBtn10;
    TBitBtn *BitBtn11;
    TBitBtn *BitBtn13;
    TBitBtn *feature_vector_to_clipboard2;
    TBitBtn *BitBtn41;
    TBitBtn *delete_vector_row2;
    TBitBtn *delete_featurevector2;
    TStaticText *StaticText7;
    TStaticText *StaticText9;
    TNumLab *duration2L;
    TNumLab *deriv2AvgL;
    TNumLab *pitch2AvgL;
    TNumLab *peakFreq2AvgL;
    TNumLab *principalFreq2AvgL;
    TNumLab *pgood2AvgL;
    TNumLab *FM2AvgL;
    TNumLab *AM2AvgL;
    TNumLab *ent2AvgL;
    TNumLab *cont_t2AvgL;
    TNumLab *cont_f2AvgL;
    TNumLab *deriv2MinL;
    TNumLab *deriv2MaxL;
    TNumLab *deriv2VarL;
    TNumLab *pitch2MinL;
    TNumLab *pitch2MaxL;
    TNumLab *pitch2VarL;
    TNumLab *peakFreq2MinL;
    TNumLab *peakFreq2MaxL;
    TNumLab *peakFreq2VarL;
    TNumLab *principalFreq2MinL;
    TNumLab *principalFreq2MaxL;
    TNumLab *principalFreq2VarL;
    TNumLab *pgood2MinL;
    TNumLab *pgood2MaxL;
    TNumLab *pgood2VarL;
    TNumLab *FM2minL;
    TNumLab *FM2maxL;
    TNumLab *FM2VarL;
    TNumLab *AM2MinL;
    TNumLab *AM2MaxL;
    TNumLab *AM2VarL;
    TNumLab *ent2MinL;
    TNumLab *ent2MaxL;
    TNumLab *ent2VarL;
    TNumLab *cont_t2MinL;
    TNumLab *cont_t2MaxL;
    TNumLab *cont_t2VarL;
    TNumLab *cont_f2MinL;
    TNumLab *cont_f2MaxL;
    TNumLab *cont_f2VarL;
    TNumLab *begin2L;
    TNumLab *end2L;
    TNumLab *FMc2AvgL;
    TNumLab *FMc2MinL;
    TNumLab *FMc2MaxL;
    TNumLab *FMc2VarL;
    TNumLab *NumLab32;
    TNumLab *NumLab37;
    TReportListView *feature_statistics_View2;
    TStaticText *StaticText10;
    TStaticText *StaticText12;
    TStaticText *StaticText13;
    TStaticText *StaticText14;
    TStaticText *StaticText15;
    TLabel *Label9;
    TLabel *Label15;
    TBitBtn *AddBt2;
    TRadioGroup *raw_features_2;
    TEdit *raw_features2;
    TBitBtn *add_raw_features2;
    TSpeedButton *paste11;
    TSpeedButton *feature_vector_cut2;
    TTabSheet *TabSheet1;
    TCheckBox *autosave_image2;
    TCheckBox *CheckBox3;
    TRadioGroup *switch_stats_diplay;
    TBitBtn *cut_featurestatview;
    TBitBtn *save_stats;
    TBitBtn *Clear_featurestatview;
    TBitBtn *feature_vector_cut;
	TRadioGroup *switch_stats_diplay2;
    TBitBtn *copy_stats2;
    TBitBtn *BitBtn19;
    TBitBtn *save_stats2;
    TBitBtn *Clear_featurestatview2;
    TButton *goBackDisplay2;
    TButton *advanceDisplay2;
    TRadioGroup *pagination2;
	TShape *drag_cursor2;
	TTabSheet *Segmentation;
	TTabSheet *animal;
	TMemo *file_comments;
	TBitBtn *save_comments;
	TStaticText *StaticText226;
	TStaticText *StaticText204;
	TStaticText *StaticText201;
	TStaticText *StaticText200;
	TBitBtn *new_bird;
	TBitBtn *newScale;
	TComboBox *templates;
	TStaticText *StaticText17;
	TStaticText *age;
	TStaticText *bird_name;
	TStaticText *details;
    TFrame1 *Frame11;
    TRadioGroup *CalcMode;
    TTabSheet *Segmentation2;
    TFrame1 *Frame12;
    TRadioGroup *CalcMode2;
    TReportListView *feature_vector_list2;
	TEdit *comments;
	TRadioGroup *pagination;
	TRadioGroup *stretch;
	TImage *preview_scale;
	TTabSheet *TabSheet3;
	TBitBtn *new_table;
	TBitBtn *openTable;
	TBitBtn *view_recs;
	TBitBtn *ToExcel;
	TBitBtn *delete_all;
	TBitBtn *remove_rec;
	TStaticText *table_name;
	TStaticText *RecordsNum;
	TBitBtn *addBtn_alt;
	TNumLab *frequencyL;
	TMemo *Memo2;
	TLabel *Label16;
	TLabel *Label17;
	TImage *preview_scale2;
	TBitBtn *save_comments2;
	TSpeedButton *group2;
	TRadioGroup *stretch2;
	TTabSheet *TabSheet4;
	TBitBtn *new_table2;
	TBitBtn *openTable2;
	TBitBtn *view_recs2;
	TBitBtn *ToExcel2;
	TBitBtn *delete_all2;
	TStaticText *table_name2;
	TBitBtn *BitBtn48;
	TStaticText *RecordsNum2;
	TBitBtn *Addrec2_alt;
	TLabel *Label18;
	TLabel *Label19;
	TReportListView *comparison_view;
	TReportListView *similarity_batch_view;
	TPanel *Panel9;
	TNumLab *ThreshNum;
	TNumLab *IntervalNum;
	TNumLab *SectionTab;
	TNumLab *TimeWarpLB;
	TLabel *Label3;
	TLabel *Label7;
	TNumLab *match_tolerance_level;
	TStaticText *msTxt;
	TStaticText *msText2;
	TTrackBar *IntervalTB;
	TTrackBar *SimilaritySection;
	TStaticText *SectionTxt;
	TStaticText *textInt;
	TStaticText *textThresh;
	TTrackBar *ThreshlodTB;
	TTrackBar *TimeWarpTB;
	TStaticText *StaticText82;
	TTrackBar *match_tolerance;
	TStaticText *StaticText27;
	TBitBtn *ZoomIB;
	TBitBtn *ZoomOB;
	TCheckBox *gui;
	TTrackBar *sim_contrast;
	TCheckBox *smooth;
	TBitBtn *ScoreB;
	TSpeedButton *diagonal;
	TSpeedButton *sections;
	TSpeedButton *conservative;
	TSpeedButton *liberal;
	TCheckBox *save_sections;
	TCheckBox *save_matrix;
	TBitBtn *SaveScore;
	TSpeedButton *copy12;
	TSpeedButton *cut12;
	TBitBtn *delete_similarity;
	TEdit *similarity_comments;
	TBitBtn *delete_row_stats;
	TBitBtn *BitBtn4;
	TBitBtn *delete_sim_row;
	TBitBtn *update_multisegment;
	TBitBtn *update_segment2;
	TSpeedButton *batch_copy;
	TSpeedButton *batch_cut;
	TBitBtn *batch_deleterow;
	TBitBtn *batch_delete;
	TPanel *Panel11;
	TNumLab *ThreshNum3;
	TNumLab *ThreshNum2;
	TNumLab *meanEuclideanDist;
	TNumLab *median_period;
	TNumLab *random_period;
	TCheckBox *period_estimate;
	TBitBtn *BitBtn6;
	TTrackBar *TBperThresh;
	TStaticText *StaticText103;
	TTrackBar *TBperiod;
	TStaticText *StaticText97;
	TBitBtn *save_period;
	TBitBtn *calc_period;
	TStaticText *StaticText88;
	TStaticText *StaticText76;
	TPanel *per_sound2;
	TNumLab *meanEuclideanDist2;
	TNumLab *median_period2;
	TNumLab *random_period2;
	TNumLab *NumLab4;
	TNumLab *NumLab5;
	TStaticText *StaticText118;
	TStaticText *StaticText120;
	TCheckBox *CheckBox1;
	TBitBtn *BitBtn15;
	TBitBtn *save_period2;
	TBitBtn *calc_period2;
	TTrackBar *TrackBar2;
	TStaticText *StaticText117;
	TTrackBar *TrackBar1;
	TStaticText *StaticText114;
	TSpeedButton *copy_comparison;
	TSpeedButton *cut_comparison;
	TBitBtn *delete_row_comparison;
	TBitBtn *delete_comparison;
	TCheckBox *align;
	TMemo *Memo4;
	TEdit *comparison_comments;
	TBitBtn *save_sim;
	TBitBtn *save_comparison;
	TBitBtn *save_batch;
	TBitBtn *open_table_alt;
	TMemo *Memo5;
	TEdit *comments2;
	TCheckBox *boost;
	TButton *Button1;
	TPanel *Panel12;
	TNumLab *minfr;
	TStaticText *StaticText79;
	TTrackBar *minfr_tb;
	TStaticText *StaticText2;
	TStaticText *StaticText81;
	TRadioGroup *cepst_pitch;
	TRadioGroup *cepstrum_pitch2;
	TLinkLabel *LinkLabel1;
	TMenuItem *Savethisimage1;
	TRadioGroup *CalcModeAlt;
	TLabel *Label1;
	TRadioGroup *CalcModeAlt2;
	TCheckBox *auto_create_tables;
	TBitBtn *SaveB;
	TBitBtn *SaveB2;
	TCheckBox *auto_create_tables2;
	TMemo *Memo3;
	TLabel *Label2;
	TLabel *Label4;
	TLabel *Label20;
	TStaticText *StaticText33;
	TStaticText *StaticText6;
	TPanel *score_panel;
	TReportListView *similarity_view;
	TBitBtn *expand;
	TBitBtn *play1;
	TBitBtn *play2;

        void __fastcall EntropyBClick(TObject *Sender);
		void __fastcall FMBClick(TObject *Sender);
		void __fastcall Image2MouseMove(TObject *Sender, TShiftState Shift,
		  int X, int Y);
        void __fastcall PitchBClick(TObject *Sender);
        void __fastcall PowerBClick(TObject *Sender);
        void __fastcall DerivPowerBClick(TObject *Sender);
        void __fastcall ScoreBClick(TObject *Sender);
        void __fastcall GlobEntrScoreBClick(TObject *Sender);
        void __fastcall GlobContScoreBClick(TObject *Sender);
        void __fastcall GlobModuScoreBClick(TObject *Sender);
        void __fastcall GlobPitchScoreBClick(TObject *Sender);
        void __fastcall LocalPitchScoreBClick(TObject *Sender);
        void __fastcall LocalModulScoreBClick(TObject *Sender);
        void __fastcall LocalEntrScoreBClick(TObject *Sender);
        void __fastcall LocalContScoreBClick(TObject *Sender);
        void __fastcall OverallScoreBClick(TObject *Sender);
        void __fastcall GlobalScoreBClick(TObject *Sender);
        void __fastcall LocalScoreBClick(TObject *Sender);
        void __fastcall ZoomOBClick(TObject *Sender);
        void __fastcall ZoomIBClick(TObject *Sender);
        void __fastcall Image1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall Image2MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall SaveScoreClick(TObject *Sender);
        void __fastcall ThreshlodTBChange(TObject *Sender);
        void __fastcall IntervalTBChange(TObject *Sender);
		void __fastcall newBClick(TObject *Sender);
		void __fastcall PreviewClick(TObject *Sender);
        void __fastcall new2BClick(TObject *Sender);
        void __fastcall SaveRecBtClick(TObject *Sender);
        void __fastcall AddBtClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall RecordMemo2Click(TObject *Sender);
        void __fastcall secondaryCo(TObject *Sender);
        void __fastcall SaveIntervalsBClick(TObject *Sender);
        void __fastcall SaveB2Click(TObject *Sender);
        void __fastcall DrawBClick(TObject *Sender);
        void __fastcall ClearBClick(TObject *Sender);
        void __fastcall printBClick(TObject *Sender);
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall SimilaritySectionChange(TObject *Sender);
        void __fastcall displayChange(TObject *Sender);
        void __fastcall TBperThreshChange(TObject *Sender);
        void __fastcall optionsClick(TObject *Sender);
        void __fastcall DrawAllClick(TObject *Sender);
        void __fastcall GlobalPlotBClick(TObject *Sender);
        void __fastcall MouseActionClick(TObject *Sender);
        void __fastcall PitchGoodnessBClick(TObject *Sender);
        void __fastcall SelectSoundClick(TObject *Sender);
        void __fastcall derivTBChange(TObject *Sender);
        void __fastcall sldrBrightnessChange(TObject *Sender);
        void __fastcall entropyTbChange(TObject *Sender);
        void __fastcall stretchClick(TObject *Sender);
        void __fastcall SaveBClick(TObject *Sender);
        void __fastcall sldrBrightness2Change(TObject *Sender);
        void __fastcall show_song2MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall ScrollBox4MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall ScrollBox4MouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall ScrollBox4DblClick(TObject *Sender);
        void __fastcall show_song2MouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall calc_periodClick(TObject *Sender);
        void __fastcall SimilarityEnter(TObject *Sender);
        void __fastcall CalcModeClick(TObject *Sender);
        void __fastcall ToExcelClick(TObject *Sender);
        void __fastcall delete_allClick(TObject *Sender);
        void __fastcall remove_recClick(TObject *Sender);
        void __fastcall view_recsClick(TObject *Sender);
        void __fastcall openTableClick(TObject *Sender);
        void __fastcall save_scoreClick(TObject *Sender);
        void __fastcall matlabClick(TObject *Sender);
        void __fastcall new_tableClick(TObject *Sender);
        void __fastcall calc_contoursClick(TObject *Sender);
        void __fastcall CombinedEnter(TObject *Sender);
        void __fastcall guiClick(TObject *Sender);
        void __fastcall BitBtn2Click(TObject *Sender);
        void __fastcall addClick(TObject *Sender);
        void __fastcall add_allClick(TObject *Sender);
        void __fastcall List1KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall List1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall List2KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall List2MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall data_typeClick(TObject *Sender);
        void __fastcall Edit1Click(TObject *Sender);
        void __fastcall DoBatchClick(TObject *Sender);
        void __fastcall breakItClick(TObject *Sender);
        void __fastcall help5Click(TObject *Sender);
        void __fastcall data_windowChange(TObject *Sender);
        void __fastcall advanceChange(TObject *Sender);
        void __fastcall TimeWarpTBChange(TObject *Sender);
        void __fastcall min_peak_freqChange(TObject *Sender);
        void __fastcall saveScoreSegmClick(TObject *Sender);
        void __fastcall GlobAMScoreBClick(TObject *Sender);
        void __fastcall LocalAMScoreBClick(TObject *Sender);
        void __fastcall AMBClick(TObject *Sender);
        void __fastcall sound_detection_termsClick(TObject *Sender);
        void __fastcall conservativeClick(TObject *Sender);
        void __fastcall liberalClick(TObject *Sender);
        void __fastcall MtimesNClick(TObject *Sender);
        void __fastcall BitBtn22Click(TObject *Sender);
        void __fastcall BitBtn23Click(TObject *Sender);
        void __fastcall noise_detectClick(TObject *Sender);
        void __fastcall noise_ratioChange(TObject *Sender);
        void __fastcall CalcMode2Click(TObject *Sender);
        void __fastcall diagonalClick(TObject *Sender);
        void __fastcall batch_table2_openClick(TObject *Sender);
        void __fastcall raw_features2Change(TObject *Sender);
        void __fastcall input2Click(TObject *Sender);
        void __fastcall folder2_btnClick(TObject *Sender);
        void __fastcall save_periodClick(TObject *Sender);
        void __fastcall save_period2Click(TObject *Sender);
        void __fastcall calc_period2Click(TObject *Sender);
        void __fastcall match_toleranceChange(TObject *Sender);
        void __fastcall ZoomOBMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall ZoomIBMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall BitBtn29Click(TObject *Sender);
        void __fastcall refineClick(TObject *Sender);
        void __fastcall raw_featuresClick(TObject *Sender);
        void __fastcall raw_features_2Click(TObject *Sender);
        void __fastcall adjust_pitchClick(TObject *Sender);
        void __fastcall mfaBClick(TObject *Sender);
        void __fastcall principal_frBClick(TObject *Sender);
        void __fastcall contour_threshChange(TObject *Sender);
        void __fastcall contour_biasTBChange(TObject *Sender);
        void __fastcall persistantBClick(TObject *Sender);
        void __fastcall continulity_tBClick(TObject *Sender);
        void __fastcall continuity_fBClick(TObject *Sender);
        void __fastcall mfaB2Click(TObject *Sender);
        void __fastcall continulity_tB2Click(TObject *Sender);
        void __fastcall contour_thresh2Change(TObject *Sender);
        void __fastcall contour_biasTB2Change(TObject *Sender);
        void __fastcall BitBtn31Click(TObject *Sender);
        void __fastcall ApplicationEvents1Idle(TObject *Sender,
          bool &Done);
        void __fastcall autosave_imageClick(TObject *Sender);
        void __fastcall autosave_image2Click(TObject *Sender);
        void __fastcall delete_similarity_recsClick(TObject *Sender);
        void __fastcall clustersClick(TObject *Sender);
        void __fastcall cluster_tableClick(TObject *Sender);
        void __fastcall pitch_methodClick(TObject *Sender);
        void __fastcall age_or_dayClick(TObject *Sender);
        void __fastcall frequency_rangeChange(TObject *Sender);
        void __fastcall frequency_range2Change(TObject *Sender);
        void __fastcall new_birdClick(TObject *Sender);
        void __fastcall calcnoteClick(TObject *Sender);
        void __fastcall state_transition_TBChange(TObject *Sender);
        void __fastcall missmatchesTbChange(TObject *Sender);
        void __fastcall BitBtn37Click(TObject *Sender);
        void __fastcall smoothyClick(TObject *Sender);
        void __fastcall calc_rhythmClick(TObject *Sender);
        void __fastcall zoomClick(TObject *Sender);
        void __fastcall open_viewerClick(TObject *Sender);
        void __fastcall forwardClick(TObject *Sender);
        void __fastcall backwardsClick(TObject *Sender);
        void __fastcall advance_viewerChange(TObject *Sender);
        void __fastcall gain3Change(TObject *Sender);
        void __fastcall viewer_rangeChange(TObject *Sender);
        void __fastcall add_raw_featuresClick(TObject *Sender);
        void __fastcall play_song2PostClick(TObject *Sender,
          TMPBtnType Button);
          void __fastcall play_song2Click(TObject *Sender, TMPBtnType Button,
    bool &DoDefault);
        void __fastcall play1Click(TObject *Sender);
        void __fastcall play2Click(TObject *Sender);
        void __fastcall Open1Click(TObject *Sender);
        void __fastcall SaveAs1Click(TObject *Sender);
        void __fastcall Play3Click(TObject *Sender);
        void __fastcall Exit1Click(TObject *Sender);
        void __fastcall Close1Click(TObject *Sender);
        void __fastcall Sounds121Click(TObject *Sender);
        void __fastcall Sound11Click(TObject *Sender);
        void __fastcall Sound21Click(TObject *Sender);
        void __fastcall paste11Click(TObject *Sender);
        void __fastcall copy13Click(TObject *Sender);
        void __fastcall cut13Click(TObject *Sender);
        void __fastcall paste13Click(TObject *Sender);
        void __fastcall copy14Click(TObject *Sender);
        void __fastcall cut14Click(TObject *Sender);
        void __fastcall paste14Click(TObject *Sender);
        //void __fastcall fine_segmentationClick(TObject *Sender);
        void __fastcall save_commentsClick(TObject *Sender);
        void __fastcall update_displayClick(TObject *Sender);
        void __fastcall Saverawfeatures1Click(TObject *Sender);
	void __fastcall deriv_displayClick(TObject *Sender);
	void __fastcall Pitch_mClick(TObject *Sender);
	void __fastcall Amplitude_mClick(TObject *Sender);
	void __fastcall MeanFrequency_mClick(TObject *Sender);
	void __fastcall Principalcontour_mClick(TObject *Sender);
	void __fastcall ou1Click(TObject *Sender);
	void __fastcall Save1Click(TObject *Sender);
	void __fastcall range_downChange(TObject *Sender);
	//void __fastcall locationChange(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall range_upChange(TObject *Sender);
	void __fastcall newScaleClick(TObject *Sender);
    void __fastcall auto_gainClick(TObject *Sender);
    void __fastcall songnameClick(TObject *Sender);
    void __fastcall SongName2aClick(TObject *Sender);
    void __fastcall update_display2Click(TObject *Sender);
    void __fastcall range_down2Change(TObject *Sender);
    void __fastcall range_up2Change(TObject *Sender);
	void __fastcall save_rawfeaturesClick(TObject *Sender);
    void __fastcall save_mean_featuresClick(TObject *Sender);
    void __fastcall save_features_at_pointerClick(TObject *Sender);
    void __fastcall Playsound1Click(TObject *Sender);
    void __fastcall Goaway1Click(TObject *Sender);
    void __fastcall song_playClick(TObject *Sender, TMPBtnType Button, bool &DoDefault);
    void __fastcall Draw1Click(TObject *Sender);
    void __fastcall Rotate1Click(TObject *Sender);
    void __fastcall Pan1Click(TObject *Sender);
    void __fastcall Zoom1Click(TObject *Sender);
    void __fastcall Color1Click(TObject *Sender);
    void __fastcall Goway1Click(TObject *Sender);
    void __fastcall gotoothersound1Click(TObject *Sender);
    void __fastcall similarity2Click(TObject *Sender);
	void __fastcall save_featuresPopup(TObject *Sender);
	void __fastcall Image1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y);
	void __fastcall preview_sonoMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y);
    void __fastcall save_xmlClick(TObject *Sender);
    void __fastcall features_to_clipboardClick(TObject *Sender);
    void __fastcall delete_feature_rowClick(TObject *Sender);
    void __fastcall feature_at_pointer_listSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
    void __fastcall feature_vector_listSelectCell(TObject *Sender, int ACol, int ARow,
          bool &CanSelect);
    void __fastcall features_displayChange(TObject *Sender);
    void __fastcall feature_vector_to_clipboardClick(TObject *Sender);
    void __fastcall feature_vector_XMLClick(TObject *Sender);
    void __fastcall delete_featurevector_rowsClick(TObject *Sender);
    void __fastcall Clear_featurelistClick(TObject *Sender);
    void __fastcall delete_featurevectorClick(TObject *Sender);
    void __fastcall feature_statistics_ViewSelectCell(TObject *Sender, int ACol,
		  int ARow, bool &CanSelect);
	void __fastcall clear_selectionClick(TObject *Sender);
	//void __fastcall locationEChange(TObject *Sender);
	void __fastcall advanceDisplayClick(TObject *Sender);
	void __fastcall goBackDisplayClick(TObject *Sender);
	void __fastcall preview_sonoMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
    void __fastcall cutClick(TObject *Sender);
	void __fastcall paginationClick(TObject *Sender);
    void __fastcall switch_stats_diplayClick(TObject *Sender);
    void __fastcall copy_statsClick(TObject *Sender);
    void __fastcall Clear_featurestatviewClick(TObject *Sender);
    void __fastcall cut_featurestatviewClick(TObject *Sender);
    void __fastcall templatesChange(TObject *Sender);
    void __fastcall feature_vector_cutClick(TObject *Sender);
    void __fastcall feature_vector_cut2Click(TObject *Sender);
    void __fastcall save_statsClick(TObject *Sender);
    void __fastcall preview_sono2MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall Image2MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
    void __fastcall Clear_featurestatview2Click(TObject *Sender);
    void __fastcall entropy_valBeforeDisplay(TObject *Sender, UnicodeString &Text);
    void __fastcall deriv_valBeforeDisplay(TObject *Sender, UnicodeString &Text);
    void __fastcall preview_cursorMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall preview_cursorMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall delete_row_statsClick(TObject *Sender);
	void __fastcall update_multisegmentClick(TObject *Sender);
	void __fastcall update_segment2Click(TObject *Sender);
	void __fastcall copy12Click(TObject *Sender);
	void __fastcall cut12Click(TObject *Sender);
	void __fastcall delete_sim_rowClick(TObject *Sender);
	void __fastcall delete_similarityClick(TObject *Sender);
	void __fastcall copy_comparisonClick(TObject *Sender);
	void __fastcall delete_comparisonClick(TObject *Sender);
	void __fastcall cut_comparisonClick(TObject *Sender);
	void __fastcall delete_row_comparisonClick(TObject *Sender);
	void __fastcall batch_copyClick(TObject *Sender);
	void __fastcall batch_cutClick(TObject *Sender);
	void __fastcall batch_deleterowClick(TObject *Sender);
	void __fastcall batch_deleteClick(TObject *Sender);
	void __fastcall boostClick(TObject *Sender);
	void __fastcall Goodnessofpitch_mClick(TObject *Sender);
	void __fastcall Frequencymodulation_mClick(TObject *Sender);
	void __fastcall Amplitudemodulation_mClick(TObject *Sender);
	void __fastcall Wienerentropy_mClick(TObject *Sender);
	void __fastcall Durationofstate_mClick(TObject *Sender);
	void __fastcall Continuityovertime_mClick(TObject *Sender);
	void __fastcall Continuityoverspace_mClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall minfr_tbChange(TObject *Sender);
	void __fastcall cepst_pitchClick(TObject *Sender);
	void __fastcall cepstrum_pitch2Click(TObject *Sender);
	void __fastcall LinkLabel1LinkClick(TObject *Sender, const UnicodeString Link, TSysLinkType LinkType);
	void __fastcall Savethisimage1Click(TObject *Sender);
	void __fastcall CalcModeAltClick(TObject *Sender);
	void __fastcall CalcModeAlt2Click(TObject *Sender);
	void __fastcall expandClick(TObject *Sender);



 








private:	// User declarations
        // functions
	  AnsiString SoundFile, song1_name, song2_name;
	  bool doIncrement;
	  int feature_at_pointer_cursor, feature_at_pointer2_cursor, feature_vector_cursor,
		  feature_vector2_cursor, similarity_cursor, comparison_cursor, similarity_batch_cursor; // for the feature_at_pointer_list
	 // float valid_cmp[50];
	  int mouse_down_X, mouse_down_X2;
	  int numSamples;
	  bool update_score_sounds, similarity_matrix_exists, advance_page, draged, done, first_snd;
	  Graphics::TBitmap *Sonogram;
	  CZT_FeatureCalc            m_Calculator1, m_Calculator2;
	  void Calc(AnsiString fileName, TImage *Image);
	  void Calc_light(AnsiString fileName, TImage *Image, int begin);
      void Segment_Sound(int begin, int end, bool sound);
      void Calc_Diagonal_Score();
	  void set_tables(bool bird);
      void update_popup(int flag);
	  void recalc(bool snd), update_scale();
	  void recalc_light(bool snd);
      float set_brightness(bool autobright, int slider_pos, bool deriv_display);
	 // void __fastcall MyIdleHandler(TObject *Sender, bool &Done);
      bool Try(int direction);
      TColor ColorMap(int Col), ColorMap2(int Col),ColorCode(float val, float AvgEng);
      int syll_score(float syll_dist), glob_score(float dist);
	  int Redundent(int X,int Y,bool up);
      void draw_score_sounds(), recalc_Similarity(), calc_Linearity(), calc_the_distances();
      void set_all_bitmaps(), scan_the_lines(int line),set_local_score(float LocalMeasures, int song1, int song2);
      void TopLeft(), Compute_Similarity_Sections(),Trim_Similarity_Sections()
         , DeleteFinalData1(),DeleteFinalData2()
         , Calc_Distance(),DeleteScoreData(),SimilarityScore(),  SetBitmap( Graphics::TBitmap *BM , int W, int H, TColor color)
		 ,CreateVerticalBM(), calc_range(), Update_Display(bool bird);
      void Update_Display_light(bool bird);
	  void hide_all_features(CZT_FeatureCalc *cal), save_syllables(bool song);
	  void Analyze_song1(AnsiString SongName, bool render), Analyze_song2(AnsiString SongName, bool render);
	  void Analyze_song1_light(AnsiString SongName, bool render, int start);
	  void note_dur();
      void Fill_feature_headers();
	  void set_scale(bool image);
	  void set_analysis_settings(bool bird);
      void set_feature_col_highlight(int col, bool the_song);
      void deselect_rows();

      float KSs(float *v1, float *v2, int start1, int end1, int start2, int end2);
      bool right(),GetSong1();
      int best();
      //int hp_filter(float *data, int nobs,double lambda, double *output, BOOL ret);
      float KS(TVector *v1, TVector *v2), calc_thresh(int pos);
      void show_outlines(TImage* image, int onset, int offset);
	  void update_template();
	  void save_data(), retrieve_data(AnsiString name), retrieve_data2(AnsiString name);
	  // variables
      enum {Up,Down,Left,Right,UpLeft,UpRight,DownLeft,DownRight};
      POINT cursor;
      //TMatrix *area;
      TVector *KSpitch1, *KSpitch2, *KSModul1 , *KSModul2 , *KSentropy1 , *KSentropy2 , *KScont1 , *KScont2;

      AnsiString string, MainStr1, MainStr2,str;
      bool TwoSongs, *Signal1, *Signal2, SecondRound, ready, ready2, GUI, ActiveList, dobatch
           ,clicked,Active ,interval1, interval2, memo1Clear, memo2Clear,ResetTheLine;
	  char c[80];
	  bool **Borders, parameters_altered, active_view, preview_state, preview_state2;
	  float  **Local,**Glob, threshold, THRESH;
      int **Sim, **Section, ind1, ind2;
      int   n, Show1, Show2, SongScore ,OnsetSong1, OffsetSong1, OnsetSong2, OffsetSong2,silence1,silence2
            ,state1,state2, imitated, inspired, TutorNotImitatedPitch, *ScoreCurve, Xtrim, Ytrim
			, SyllDur, SilenceDur, sim_iter1, sim_iter2,target_syllable_num, the_iteration, table_iterate,table_buffer,
			onset_1, offset_1, onset_2, offset_2, menu_outline, feature_statistics_cursor, feature_statistics2_cursor;
	  long dataCounter1,dataCounter2, DataPointer, margin;
	  double  rx, ry, rz;
      float   *Entropy1, *FM1, *AM1, *VarPitch, *VarFM, *VarFMc, *VarAM, *VarEnt, *VarAmplitude, *VarPGood, *VarPeakFr, *VarPrincipalFr, *VarCont_t, *VarCont_f,
			  *Amplitude1, *mfa1, *Pitch1, *PitchGoodness1, *FMc1, *FMc2, *dt, *df, *PeakFreq1, AvgAmplitude,
              *EntropyS1, *FMS1, *AMS1, *PitchGoodnessS1, *PitchS1, *Data2,  **Contours2,
              *Entropy2, *FM2, *AM2, *Pitch2, *PitchGoodness2, *PeakFreq2, *Amplitude2, *mfa2,
              *principal_frequency1, *continuity_t1, *continuity_f1, *principal_frequency2, *continuity_t2, *continuity_f2,

              *EntropyS2, *FMS2, *AMS2, *PitchGoodnessS2, *PitchS2,TutorImitatedPitch, PupilImitatedPitch,
               TutorImitatedEntropy, PupilImitatedEntropy, TutorImitatedFM,PupilImitatedFM,
               TutorNotImitatedEntropy, TutorNotImitatedFM,treshold, entFr, Local_Thresh,
               TutorImitatedCont, PupilImitatedCont, sdModul, sdPitch, sdEntropy, sdCont, table_ms_duration,  table_ms_start
               , sdModul2, sdPitch2, sdEntropy2, sdCont2, KSval[5], PeriodVal,InfoVal, Cf[10], Cfl[10],
                AvgPitch1, AvgFM1, AvgAM1, AvgEnt1, AvgAmplitude1, AvgPGood1, AvgPeakFr1, AvgPrincipalFr1, AvgFMc1, AvgCont_t1,  AvgCont_f1,
                MinPitch1, MinFM1,  MinAM1, MinEnt1, MinAmplitude1, MinPGood1, MinPeakFr1, MinPrincipalFr1, MinFMc1, MinCont_t1, MinCont_f1,
                MaxPitch1, MaxFM1, MaxAM1, MaxEnt1, MaxAmplitude1, MaxPGood1, MaxPeakFr1, MaxPrincipalFr1, MaxFMc1, MaxCont_t1,  MaxCont_f1,
                VarPitch1, VarFM1, VarAM1, VarEnt1, VarAmplitude1, VarPGood1, VarPeakFr1, VarPrincipalFr1, VarFMc1, VarCont_t1,  VarCont_f1,

                AvgPitch2, AvgFM2, AvgAM2, AvgEnt2, AvgAmplitude2, AvgPGood2, AvgPeakFr2, AvgPrincipalFr2, AvgFMc2, AvgCont_t2,  AvgCont_f2,
                MinPitch2, MinFM2, MinAM2, MinEnt2, MinAmplitude2, MinPGood2, MinPeakFr2, MinPrincipalFr2, MinFMc2, MinCont_t2, MinCont_f2,
                MaxPitch2, MaxFM2, MaxAM2, MaxEnt2, MaxAmplitude2, MaxPGood2, MaxPeakFr2, MaxPrincipalFr2, MaxFMc2, MaxCont_t2,  MaxCont_f2,
                VarPitch2, VarFM2, VarAM2, VarEnt2, VarAmplitude2, VarPGood2, VarPeakFr2, VarPrincipalFr2, VarFMc2, VarCont_t2,  VarCont_f2,
                position_maxFreq1, position_maxAmp1, position_maxFreq2, position_maxAmp2;

      COLORREF *LocalMeasuresPtr, *GlobalSlopePtr, *AllMeasuresPtr, *GlobalEntropyPtr,*LocalPitchPtr
            ,*LocalEntropyPtr,*LocalSlopePtr,*GlobalPitchPtr,*GlobalMeasuresPtr
            ,*LocalContPtr,*GlobalContPtr,*LocalAMPtr,*GlobalAMPtr;

			CWavIterator wave;
			bool doNotSegment;
			int calc_advance1, calc_advance2; // these vars keep memory of advance window while calculating


			 float foo;
			//static float foo = advanceTb->Value*100;
			int beginLocation, endLocation;
			bool showAdvance, showBack, showAdvance2, showBack2;
			ofstream myfile;
public:		// User declarations

        __fastcall TForm1(TComponent* Owner);
        void __fastcall CreateParams(Controls::TCreateParams &Params);
        Graphics::TBitmap *Sonogram1, *DerivBM1, *ContoursBM1,*Sonogram2, *DerivBM2
                , *Sonogram2v, *Deriv2vBM,*LocAllBM
                , *GlobModBM, *CombinedBM, *GlobEntBM, *LocPitchBM, *LocEntBM, *LocModBM
                , *GlobPitchBM, *GlobalBM, *LocContBM, *GlobContBM, *LocAMBM, *GlobAMBM;
		float nonLinear, Zoom, BandWidth,PitchAvg,PitchDV,FMDV,FMAvg,EntropyDV,ContDV,EntropyAvg,ContAvg;
        void  SaveFeatures(bool scale), SaveSonogram(),CalculateResults(),calc_meanPeriod_and_meanEuclideanDist(bool song);
		void Crash();
		bool  no_pagination_action, Waiting1, Waiting2, batch,  score, Continuous,dolphin, Loon, bat, Dove , tmpSong1, tmpSong2, FinalSong1, FinalSong2
			 , CalculateIntervalMeasures(int start, int end), CalculateNoteMeasures(int start, int end);
        int  sound, WindowSize, Slide, scrNum, scrVal, pitchHarmon, Iterations1, Iterations2, SignalToScore;
        float FeaturePeriodicity[4], rangeIndex, DigiRate, VarRawAmplitude;
        ofstream *out;

		bool SALoldFile, doneCalc, doneEqualizing, batchAlt;
		bool updateSegmentation, updateSegmentation2, updateSegmentationSec, updateSegmentationSec2;


	// segmentation methods
	void secondaryFeatureDDChange_sf(TObject *Sender);
	void ComboBox1Change_sf(TObject *Sender);
    void segmentation_flipClick_sf(TObject *Sender);
	void secondaryComparisonRBClick_sf(TObject *Sender);
	void secondaryLogicRBClick_sf(TObject *Sender);
	void secondaryFeatureCBClick_sf(TObject *Sender);
	void adaptThresholdCBClick_sf(TObject *Sender);
	void smoothGraphClick_sf(TObject *Sender);
	void hp_smallChange_sf(TObject *Sender);
	void hp_bigChange_sf(TObject *Sender);
	void min_stop_durTbChange_sf(TObject *Sender);
	void bout_stopChange_sf(TObject *Sender);

    void secondaryFeatureDDChange_sf2(TObject *Sender);
	void ComboBox1Change_sf2(TObject *Sender);
    void segmentation_flipClick_sf2(TObject *Sender);
	void secondaryComparisonRBClick_sf2(TObject *Sender);
	void secondaryLogicRBClick_sf2(TObject *Sender);
	void secondaryFeatureCBClick_sf2(TObject *Sender);
	void adaptThresholdCBClick_sf2(TObject *Sender);
	void smoothGraphClick_sf2(TObject *Sender);
	void hp_smallChange_sf2(TObject *Sender);
	void hp_bigChange_sf2(TObject *Sender);
	void min_stop_durTbChange_sf2(TObject *Sender);
	void bout_stopChange_sf2(TObject *Sender);

};
void __fastcall TForm1::CreateParams(Controls::TCreateParams &Params)
{
  TForm::CreateParams(Params);
  Params.ExStyle   = Params.ExStyle | WS_EX_APPWINDOW;
  Params.WndParent = ParentWindow;
}
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
