//--------------------------------------------------------------------------

#ifndef clusterH
#define clusterH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <CheckLst.hpp>
#include <ExtCtrls.hpp>
#include "DBAccess.hpp"
#include "MemDS.hpp"
#include "MyAccess.hpp"
#include "SDL_NumLab.hpp"
#include "SDL_rchart.hpp"


#include "SDL_NumLab.hpp"
#include <vector.h>
#include <hash_set.h>
#include <hash_map.h>
#include <list.h>
#include <Mask.hpp>
#include "SDL_NumLab.hpp"
#include "SDL_rchart.hpp"
#include "DBAccess.hpp"
#include "MemDS.hpp"
#include "MyAccess.hpp"
#include "SDL_matrix.hpp"
#include "ClusterScreen.h"


//---------------------------------------------------------------------------
class TclusterIt : public TForm
{
__published:	// IDE-managed Components
	TScrollBox *ScrollBox1;
	TSpeedButton *repeat;
	TBitBtn *Start;
	TProgressBar *prog;
	TBitBtn *open_table;
	TTrackBar *Location;
	TBitBtn *ClearAll;
	TProgressBar *prog2;
	TBitBtn *backwards;
	TBitBtn *forward;
	TRadioGroup *featureY;
	TRadioGroup *featureX;
	TPageControl *PageControl2;
	TTabSheet *TabSheet1;
	TNumLab *intervalN;
	TCheckListBox *features;
	TStaticText *StaticText27;
	TRadioGroup *RadioGroup1;
	TStaticText *StaticText1;
	TTrackBar *intervalSize;
	TTabSheet *TabSheet5;
	TNumLab *durationTresh;
	TNumLab *maxDurationThresh;
	TNumLab *durThresh;
	TNumLab *pitchThresh;
	TNumLab *fmThresh;
	TNumLab *entropyThresh;
	TNumLab *tolerance;
	TNumLab *GoodnessThresh;
	TStaticText *StaticText2;
	TTrackBar *thresh;
	TStaticText *StaticText32;
	TTrackBar *TrackBar21;
	TStaticText *StaticText25;
	TTrackBar *TrackBar1;
	TTrackBar *TrackBar18;
	TStaticText *StaticText29;
	TStaticText *StaticText30;
	TTrackBar *TrackBar19;
	TTrackBar *TrackBar20;
	TStaticText *StaticText31;
	TTrackBar *toleranceTb;
	TStaticText *StaticText8;
	TStaticText *StaticText9;
	TStaticText *StaticText10;
	TStaticText *StaticText11;
	TStaticText *StaticText12;
	TStaticText *StaticText13;
	TStaticText *StaticText14;
	TStaticText *StaticText15;
	TTrackBar *TrackBar2;
	TStaticText *StaticText17;
	TStaticText *StaticText18;
	TTabSheet *TabSheet6;
	TEdit *quary;
	TBitBtn *BitBtn15;
	TBitBtn *noFilter;
	TStaticText *wait;
	TBitBtn *BitBtn2;
	TStaticText *Stage;
	TStaticText *toStage;
	TBitBtn *restart;
	TBitBtn *reset;
	TBitBtn *redo;
	TRadioGroup *zoom1;
	TBitBtn *BitBtn1;
	TSaveDialog *SaveDialog1;
	TTimer *Timer1;
	TMyCommand *MyCommand1;
	TMyQuery *MyQuery1;
	TTrackBar *TrackBar3;
	TStaticText *StaticText3;
	TNumLab *meanFrThresh;
	TStaticText *StaticText19;
	TPageControl *PageControl1;
	TTabSheet *TabSheet3;
	TShape *Shape1;
	TShape *Shape2;
	TShape *Shape3;
	TShape *Shape4;
	TShape *Shape5;
	TShape *Shape6;
	TShape *Shape7;
	TShape *Shape8;
	TShape *Shape9;
	TShape *Shape10;
	TLabel *clust1;
	TLabel *clust2;
	TLabel *clust3;
	TLabel *clust4;
	TLabel *clust5;
	TLabel *clust6;
	TLabel *clust7;
	TLabel *clust8;
	TLabel *clust9;
	TLabel *clust10;
	TLabel *residuals;
	TLabel *left_over;
	TLabel *counter;
	TLabel *clust_num;
	TShape *Shape11;
	TLabel *clust11;
	TShape *Shape12;
	TLabel *clust12;
	TShape *Shape13;
	TLabel *clust13;
	TShape *Shape14;
	TLabel *clust14;
	TShape *Shape15;
	TLabel *clust15;
	TRChart *chart2;
	TBitBtn *clear_graph;
	TBitBtn *save_graph;
	TStaticText *StaticText21;
	TTrackBar *showClust;
	TRadioGroup *ChosenCluster;
	TStaticText *StaticText33;
	TStaticText *StaticText34;
	TStaticText *StaticText22;
	TStaticText *development;
	TBitBtn *help5;
	TBitBtn *help6;
	TStaticText *StaticText16;
	TMaskEdit *clusterName;
	TCheckBox *warnings;
	TTabSheet *TabSheet2;
	TRChart *chart;
	TBitBtn *BitBtn5;
	TBitBtn *saveGraph;
	TTabSheet *TabSheet4;
	TRChart *chart3;
	TBitBtn *BitBtn6;
	TBitBtn *BitBtn14;
	TRadioGroup *size;
	TBitBtn *movie;
	TButton *Button2;
	TTrackBar *slide;
	TStaticText *StaticText6;
	TTrackBar *speed;
	TStaticText *StaticText5;
	TTabSheet *TabSheet7;
	TNumLab *recs1;
	TNumLab *done1;
	TEdit *select_it;
	TBitBtn *select_quarry;
	TBitBtn *cluster_it;
	TBitBtn *BitBtn3;
	TBitBtn *BitBtn8;
	TMemo *Memo1;
	TCheckBox *showData;
	TEdit *cluster_name;
	TBitBtn *help7;
	TPanel *Panel1;
	TCheckBox *writePremit;
	TCheckBox *overwriteClusters;
	TStaticText *StaticText91;
	TStaticText *table_name;
	TStaticText *RecordsNum;
	TStaticText *StaticText7;
	TCheckBox *analyze_all;
	TBitBtn *BitBtn16;
	TPanel *Panel2;
	TNumLab *TreshDist;
	TNumLab *actual;
	TNumLab *maxClust;
	TNumLab *vectorSize;
	TBitBtn *move_back;
	TBitBtn *all_data_included;
	TTrackBar *Iterations;
	TTrackBar *EuclideanTB;
	TStaticText *StaticText35;
	TStaticText *StaticText20;
	TStaticText *StaticText28;
	TBitBtn *help1;
	TBitBtn *help4;
	TStaticText *StaticText4;
	TMemo *Memo2;
	TBitBtn *BitBtn10;
	TBitBtn *saveClusters;
	TBitBtn *new_table;
	TRadioGroup *dot_size;
	TLinkLabel *LinkLabel1;
        void __fastcall exitClick(TObject *Sender);
        void __fastcall StartClick(TObject *Sender);
		void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall intervalSizeChange(TObject *Sender);
        void __fastcall threshChange(TObject *Sender);
		void __fastcall open_tableClick(TObject *Sender);
        void __fastcall BitBtn5Click(TObject *Sender);
        void __fastcall clear_graphClick(TObject *Sender);
        void __fastcall LocationChange(TObject *Sender);
        void __fastcall saveGraphClick(TObject *Sender);
        void __fastcall BitBtn6Click(TObject *Sender);
        void __fastcall ClearAllClick(TObject *Sender);
        void __fastcall IterationsChange(TObject *Sender);
        void __fastcall redoClick(TObject *Sender);
        void __fastcall RadioGroup1Click(TObject *Sender);
        void __fastcall TrackBar1Change(TObject *Sender);
        void __fastcall saveClustersClick(TObject *Sender);
        void __fastcall save_graphClick(TObject *Sender);
        void __fastcall restartClick(TObject *Sender);
        void __fastcall BitBtn10Click(TObject *Sender);
        void __fastcall resetClick(TObject *Sender);
        void __fastcall ChosenClusterClick(TObject *Sender);
        void __fastcall backwardsClick(TObject *Sender);
        void __fastcall forwardClick(TObject *Sender);
        void __fastcall move_backClick(TObject *Sender);
        void __fastcall all_data_includedClick(TObject *Sender);
        void __fastcall BitBtn14Click(TObject *Sender);
        void __fastcall BitBtn15Click(TObject *Sender);
        void __fastcall noFilterClick(TObject *Sender);
        void __fastcall sizeClick(TObject *Sender);
        void __fastcall TrackBar18Change(TObject *Sender);
        void __fastcall TrackBar19Change(TObject *Sender);
        void __fastcall TrackBar20Change(TObject *Sender);
        void __fastcall TrackBar21Change(TObject *Sender);
        void __fastcall zoomClick(TObject *Sender);
        void __fastcall zoom1Click(TObject *Sender);
        void __fastcall MyIdleHandler(TObject *Sender, bool &Done);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall featureYClick(TObject *Sender);
        void __fastcall BitBtn2Click(TObject *Sender);
        void __fastcall movieClick(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall showClustChange(TObject *Sender);
        void __fastcall EuclideanTBChange(TObject *Sender);
        void __fastcall help1Click(TObject *Sender);
        void __fastcall help7Click(TObject *Sender);
        void __fastcall help4Click(TObject *Sender);
        void __fastcall help5Click(TObject *Sender);
        void __fastcall help6Click(TObject *Sender);
        void __fastcall speedChange(TObject *Sender);
        void __fastcall select_quarryClick(TObject *Sender);
        void __fastcall cluster_itClick(TObject *Sender);
		void __fastcall BitBtn3Click(TObject *Sender);
		void __fastcall BitBtn8Click(TObject *Sender);
		void __fastcall toleranceTbChange(TObject *Sender);
		void __fastcall TrackBar2Change(TObject *Sender);
		void __fastcall BitBtn16Click(TObject *Sender);
		void __fastcall analyze_allClick(TObject *Sender);
		void __fastcall dot_sizeClick(TObject *Sender);
	void __fastcall writePremitClick(TObject *Sender);
	void __fastcall overwriteClustersClick(TObject *Sender);
	void __fastcall featuresClick(TObject *Sender);
	void __fastcall clusterNameFooChange(TObject *Sender);
	void __fastcall quaryChange(TObject *Sender);
	void __fastcall clusterNameChange(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall TrackBar3Change(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall LinkLabel1LinkClick(TObject *Sender, const UnicodeString Link, TSysLinkType LinkType);


private:	// User declarations


// functions:


void computeDistance(), cluster(), sortTheVector(), copyToVector(float distance, int i, int j);
void Cluster_track(bool do_change);
bool GetData();
float plot_raw_data();
void get_ready();
int bias;
float Find_Feature(int index, bool axis);

// constants and variables:
AnsiString str;
const char *fileName;
bool includeIt[20], first, problems, stayThere, DVD_play;
int onset, ChosenOne, boundIndex;
float PreviousPitch[16], PreviousFm[16], PreviousDur[16],PreviousEnt[16],PreviousGood[16],PreviousMFR[16];
float RefDur,RefPitch, RefFM , RefEnt, RefGood, RefMFR;
float **interval;
//float **avgFeature;
//float **avgRefFeature;
short dot;
//hash_multiset<int> clustSet;
hash_map<int, int> clustLocation;
vector<int> clust;
vector<list<int> > masterCluster;
//list<int> clust;
TColor color[20];
ClusterScreen cs;
public:	 	// User declarations

  __fastcall TclusterIt(TComponent* Owner);
  void __fastcall CreateParams(Controls::TCreateParams &Params);

  void insertScreenSetting(String settingName, String settingValue);
  bool screenSettingExists(String settingName);
  void createScreenSettingTable();
  bool table_exists(AnsiString table_name);
  void showTables();
  void getScreenSetting(String settingName);
  void resetDefaultScreenSetting();

};
void __fastcall TclusterIt::CreateParams(Controls::TCreateParams &Params)
{
  TForm::CreateParams(Params);
  Params.ExStyle   = Params.ExStyle | WS_EX_APPWINDOW;
  Params.WndParent = ParentWindow;
}
//---------------------------------------------------------------------------
extern PACKAGE TclusterIt *clusterIt;
//---------------------------------------------------------------------------
#endif
