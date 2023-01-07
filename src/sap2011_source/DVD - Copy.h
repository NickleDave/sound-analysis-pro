//---------------------------------------------------------------------------

#ifndef DVDH
#define DVDH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ImgList.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include "SDL_NumLab.hpp"
#include "SDL_rchart.hpp"
#include "SDL_plot3d.hpp"
#include <ExtDlgs.hpp>
#include <CheckLst.hpp>
#include "SDL_Rot3D.hpp"
#include "SDL_NumLab.hpp"
#include "SDL_plot3d.hpp"
#include "SDL_rchart.hpp"
#include "SDL_Rot3D.hpp"
//---------------------------------------------------------------------------
class TDVD_map : public TForm
{
__published:	// IDE-managed Components
        TTimer *Timer1;
        TRadioGroup *featureX;
        TRadioGroup *featureY;
        TTrackBar *Location;
        TPageControl *display;
        TTabSheet *bitmap;
        TImage *Image1;
        TTabSheet *graph;
        TRChart *chart;
        TTrackBar *xScale;
        TTrackBar *yScale;
        TRadioGroup *zoom;
        TTrackBar *min_duration;
        TStaticText *StaticText3;
        TColorDialog *ColorDialog1;
        TStaticText *date;
        TStaticText *filename;
        TTrackBar *max_duration;
        TStaticText *StaticText5;
        TNumLab *durationTresh;
        TTabSheet *TabSheet1;
        TPlot3D *dur_hist;
        TBitBtn *do_histogram;
        TRadioGroup *zoom1;
        TBitBtn *save_histo;
        TSavePictureDialog *SavePictureDialog1;
        TBitBtn *previous_hour;
        TBitBtn *next_hour;
        TBitBtn *next_morning;
        TBitBtn *previous_evening;
        TTrackBar *colors;
        TStaticText *StaticText10;
        TRadioGroup *autoscale;
        TEdit *min_val;
        TEdit *max_val;
        TStaticText *min_valT;
        TStaticText *max_valT;
        TCheckBox *trim_edges;
        TNumLab *max_durationT;
        TTrackBar *yOffset;
        TTrackBar *xOffset;
        TBitBtn *save_bitmap;
        TStaticText *StaticText11;
        TStaticText *StaticText15;
        TTabSheet *TabSheet2;
        TMemo *Memo1;
        TCheckBox *histdata;
        TBitBtn *BitBtn2;
        TShape *Shape1;
        TShape *Shape10;
        TShape *Shape3;
        TShape *Shape4;
        TShape *Shape5;
        TShape *Shape6;
        TShape *Shape7;
        TShape *Shape8;
        TShape *Shape2;
        TShape *Shape9;
        TCheckListBox *CheckListBox1;
        TCheckListBox *CheckListBox2;
        TCheckBox *calc_daily;
        TCheckBox *dailyhist;
        TTrackBar *histInt;
        TNumLab *histIntVal;
        TStaticText *StaticText17;
        TStaticText *intervTxt;
        TCheckBox *showGrid;
        TTrackBar *colorMin;
        TTrackBar *contrast3D;
        TStaticText *StaticText18;
        TBitBtn *options;
        TStaticText *table_name;
        TBitBtn *clear_map;
        TTabSheet *syll_3D;
        TRot3D *chart3D;
        TRadioGroup *featureZ;
        TCheckBox *auto_scale;
        TCheckBox *bound_box;
        TTabSheet *raw_features_2D;
        TRChart *raw_features_chart;
        TRadioGroup *rawFeatureX;
        TRadioGroup *rawFeatureY;
        TCheckBox *rotate;
        TMemo *Memo2;
        TStaticText *bird_name;
        TStaticText *model;
        TStaticText *current_age;
        TStaticText *StaticText20;
        TStaticText *StaticText21;
        TPanel *Panel1;
        TSpeedButton *cluster_mode;
        TSpeedButton *syntax_mode;
        TSpeedButton *synsong;
        TBitBtn *open_table;
        TPanel *Panel2;
        TStaticText *StaticText1;
        TStaticText *StaticText2;
        TStaticText *StaticText6;
        TStaticText *StaticText8;
        TStaticText *StaticText7;
        TStaticText *StaticText9;
        TTrackBar *bout_termination_dur;
        TNumLab *bout_termination_dur_val;
        TNumLab *bout_count_val;
        TTrackBar *bout_count;
        TNumLab *line_val;
        TTrackBar *line_width;
        TNumLab *slide_val;
        TStaticText *StaticText14;
        TStaticText *StaticText13;
        TTrackBar *slide;
        TTrackBar *interval;
        TNumLab *interval_val;
        TStaticText *StaticText12;
        TStaticText *StaticText4;
        TNumLab *speed_val;
        TTrackBar *speed;
        TPanel *Panel4;
        TCheckBox *jitter;
        TCheckBox *line;
        TCheckBox *sound;
        TRadioGroup *dot_size;
        TRadioGroup *color_scheme;
        TBitBtn *change_color;
        TTrackBar *time_progress;
        TCheckBox *morning_blink;
        TCheckBox *clear;
        TBitBtn *play;
        TBitBtn *stop;
        TSpeedButton *forword;
        TSpeedButton *backword;
        TStaticText *StaticText16;
        TStaticText *StaticText22;
        TStaticText *StaticText23;
        TStaticText *StaticText24;
        TStaticText *StaticText25;
        TStaticText *training;
        TStaticText *StaticText19;
        TBitBtn *new_bird;
        TBitBtn *BitBtn39;
        TBitBtn *help5;
        TBitBtn *BitBtn1;
        TBitBtn *BitBtn3;
        TBitBtn *BitBtn4;
        TBitBtn *BitBtn5;
        TRadioGroup *zoom_raw;
        TNumLab *time_cut;
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall speedChange(TObject *Sender);
        void __fastcall open_tableClick(TObject *Sender);
        void __fastcall featureXClick(TObject *Sender);
        void __fastcall featureYClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall xScaleChange(TObject *Sender);
        void __fastcall yScaleChange(TObject *Sender);
        void __fastcall intervalChange(TObject *Sender);
        void __fastcall LocationChange(TObject *Sender);
        void __fastcall LocationKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall LocationKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall LocationEnter(TObject *Sender);
        void __fastcall LocationExit(TObject *Sender);
        void __fastcall zoomClick(TObject *Sender);
        void __fastcall dot_sizeClick(TObject *Sender);
        void __fastcall line_widthChange(TObject *Sender);
        void __fastcall color_schemeClick(TObject *Sender);
        void __fastcall slideChange(TObject *Sender);
        void __fastcall cluster_modeClick(TObject *Sender);
        void __fastcall next_hourClick(TObject *Sender);
        void __fastcall next_morningClick(TObject *Sender);
        void __fastcall previous_eveningClick(TObject *Sender);
        void __fastcall previous_hourClick(TObject *Sender);
        void __fastcall do_histogramClick(TObject *Sender);
        void __fastcall zoom1Click(TObject *Sender);
        void __fastcall save_histoClick(TObject *Sender);
        void __fastcall colorsChange(TObject *Sender);
        void __fastcall autoscaleClick(TObject *Sender);
        void __fastcall min_durationChange(TObject *Sender);
        void __fastcall max_durationChange(TObject *Sender);
        void __fastcall clear_mapClick(TObject *Sender);
        void __fastcall save_bitmapClick(TObject *Sender);
        void __fastcall BitBtn2Click(TObject *Sender);
        void __fastcall histIntChange(TObject *Sender);
        void __fastcall dailyhistClick(TObject *Sender);
        void __fastcall showGridClick(TObject *Sender);
        void __fastcall colorMinChange(TObject *Sender);
        void __fastcall contrast3DChange(TObject *Sender);
        void __fastcall bout_termination_durChange(TObject *Sender);
        void __fastcall bout_countChange(TObject *Sender);
        void __fastcall optionsClick(TObject *Sender);
        void __fastcall auto_scaleClick(TObject *Sender);
        void __fastcall bound_boxClick(TObject *Sender);
        void __fastcall featureZClick(TObject *Sender);
        void __fastcall displayChange(TObject *Sender);
        void __fastcall rawFeatureXClick(TObject *Sender);
        void __fastcall playClick(TObject *Sender);
        void __fastcall stopClick(TObject *Sender);
        void __fastcall new_birdClick(TObject *Sender);
        void __fastcall BitBtn39Click(TObject *Sender);
        void __fastcall help5Click(TObject *Sender);
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall BitBtn3Click(TObject *Sender);
        void __fastcall BitBtn4Click(TObject *Sender);
        void __fastcall BitBtn5Click(TObject *Sender);
        void __fastcall zoom_rawClick(TObject *Sender);
private:	// User declarations
CZT_FeatureImage          DVD_Image;
__int8 DVDdata[256][256];
bool key_response;
bool table_type;
AnsiString xVar, yVar, zVar, str;

int jitter_raw;
float Xfactor, Yfactor, xfactor, yfactor,plot_data(bool three_D), find_feature(int feature);
short dot, days;
float **daily_means, **dur, **mean_pitch, **var_pitch, **mean_fm, **var_fm, **mean_ent, **var_ent, **mean_goodness, **var_goodness;
float find_raw_feature(int feature);
void plot_raw_features();
int buffer, add_data,residuals;
TColor color[20];
short clust, minute, hour, day, red, green, blue;
void calc();
TColor set_color();

public:		// User declarations
        __fastcall TDVD_map(TComponent* Owner);
        void __fastcall CreateParams(Controls::TCreateParams &Params);
        void drawImage();
};

void __fastcall TDVD_map::CreateParams(Controls::TCreateParams &Params)
{
  TForm::CreateParams(Params);
  Params.ExStyle   = Params.ExStyle | WS_EX_APPWINDOW;
  Params.WndParent = ParentWindow;
}
//---------------------------------------------------------------------------
extern PACKAGE TDVD_map *DVD_map;
//---------------------------------------------------------------------------
#endif
