//---------------------------------------------------------------------------

#ifndef recordsH
#define recordsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
#include "DBAccess.hpp"
#include "MemDS.hpp"
#include "MyAccess.hpp"
#include <Db.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include "CRGrid.hpp"
#include <ExtCtrls.hpp>
#include "SDL_NumLab.hpp"
#include <CheckLst.hpp>
#include <DBCtrls.hpp>
#include <DBTables.hpp>
#include <FileCtrl.hpp>
#include "MyDump.hpp"
#include "SDL_NumLab.hpp"
#include "UCustomFlexCelReport.hpp"
#include "UExcelAdapter.hpp"
#include "UFlexCelReport.hpp"
#include "XLSAdapter.hpp"
#include "UFlexCelGrid.hpp"
#include "UFlexCelImport.hpp"
#include "UFlexCelPreview.hpp"
#include "DADump.hpp"
#include "segmentation.h"
#include "config.h"
#include "MyDacVcl.hpp"
#include <fstream>

//---------------------------------------------------------------------------
class TDataForm : public TForm
{
__published:	// IDE-managed Components
        TOpenDialog *OpenDialog1;
        TMyTable *Records;
        TDataSource *DataSource1;
        TMyTable *Records1;
        TMyTable *settings;
        TMyQuery *MyQuery1;
        TMyCommand *MyCommand1;
        TMyTable *similarity;
        TMyQuery *MyQuery2;
        TPageControl *datapages;
        TTabSheet *animals_table;
        TSpeedButton *allow_changes;
        TBitBtn *update_table;
        TCRDBGrid *animals_grid;
    TPanel *edit_box;
        TDateTimePicker *hatched;
        TBitBtn *set_time1;
        TPanel *Panel5;
        TLabel *Label11;
        TNumLab *procedure1_age;
        TLabel *Label12;
        TEdit *procedure1;
        TDateTimePicker *procedure1_schedule;
        TDateTimePicker *procedure1_time;
        TBitBtn *set_time3;
        TPanel *Panel6;
        TLabel *Label13;
        TLabel *Label14;
        TNumLab *procedure2_age;
        TEdit *procedure2;
        TDateTimePicker *procedure2_schedule;
        TDateTimePicker *procedure2_time;
        TBitBtn *set_time4;
        TPanel *Panel7;
        TLabel *Label15;
        TNumLab *procedure3_age;
        TLabel *Label16;
        TEdit *procedure3;
        TDateTimePicker *procedure3_time;
        TDateTimePicker *procedure3_schedule;
        TBitBtn *set_time5;
    TTabSheet *notebook;
        TLabel *date_Label;
        TPanel *animal_pannel;
        TDBText *bird_name;
        TDBText *DBText1;
        TDBText *DBText3;
        TDBText *DBText4;
        TDBText *DBText5;
        TDBText *DBText6;
        TNumLab *age_lb;
        TStaticText *StaticText6;
        TStaticText *StaticText7;
        TStaticText *StaticText8;
        TStaticText *StaticText9;
        TStaticText *StaticText10;
        TStaticText *StaticText11;
        TStaticText *StaticText12;
        TDBNavigator *DBNavigator1;
        TStaticText *StaticText13;
        TPanel *action_pannel;
        TDBText *action_typeLb;
        TLabel *Label17;
        TStaticText *StaticText14;
        TStaticText *StaticText25;
        TDBNavigator *DBNavigator3;
        TStaticText *StaticText27;
        TEdit *comments;
        TEdit *resource_location;
        TBitBtn *Add_experiment_record;
        TDBNavigator *DBNavigator4;
        TStaticText *StaticText36;
        TStaticText *StaticText37;
        TEdit *file1;
        TCRDBGrid *action_animal_grid;
        TBitBtn *BitBtn8;
        TBitBtn *BitBtn10;
        TBitBtn *BitBtn11;
        TTabSheet *action_table;
        TStaticText *StaticText29;
        TEdit *action_comments;
        TStaticText *StaticText31;
        TEdit *attribute3_val;
        TEdit *attribute2_val;
        TStaticText *StaticText40;
        TEdit *attribute1_val;
        TEdit *action_type;
        TBitBtn *add_action;
        TDBNavigator *DBNavigator5;
        TStaticText *StaticText43;
        TCRDBGrid *actions_grid;
        TStaticText *StaticText44;
        TEdit *attribute1_name;
        TTabSheet *matlab;
        TNumLab *dataCount;
        TNumLab *start;
        TStaticText *StaticText45;
        TCheckListBox *CheckListBox1;
        TBitBtn *BitBtn2;
        TMemo *Memo2;
        TBitBtn *convert;
        TStaticText *StaticText1;
        TTrackBar *dataCountTb;
        TStaticText *StaticText5;
        TTrackBar *startTb;
        TBitBtn *forward;
        TStaticText *StaticText4;
        TRadioGroup *steps;
        TStaticText *StaticText3;
        TStaticText *StaticText2;
        TBitBtn *open_table;
        TTabSheet *data_table;
        TNumLab *recordsNum;
        TBitBtn *CountRecs;
        TCRDBGrid *CRDBGrid1;
        TTabSheet *wave;
        TLabel *Label1;
        TNumLab *progress;
        TLabel *Label3;
        TLabel *Label5;
        TLabel *Label4;
        TLabel *Label2;
        TEdit *slaves;
        TBitBtn *warning1;
        TStaticText *StaticText19;
        TBitBtn *New_table_raw;
        TBitBtn *Add_days;
        TFileListBox *FileListBox2;
        TFileListBox *FileListBox1;
        TDirectoryListBox *DirectoryListBox1;
        TDriveComboBox *DriveComboBox1;
        TDirectoryListBox *DirectoryListBox2;
        TBitBtn *dailyfolders;
        TEdit *Edit1;
        TDriveComboBox *DriveComboBox2;
        TProgressBar *ProgressBar1;
        TMyTable *experiments;
        TMyTable *birds;
        TDataSource *DataSource3;
        TDataSource *DataSource2;
        TTimer *Timer1;
        TDataSource *DataSource4;
        TStaticText *StaticText46;
        TLabel *Label6;
        TStaticText *StaticText28;
        TStaticText *StaticText32;
        TSpeedButton *allow_action_changes;
        TEdit *file2;
        TStaticText *StaticText47;
        TStaticText *StaticText50;
        TEdit *attribute3_name;
        TStaticText *StaticText15;
        TStaticText *StaticText16;
        TStaticText *StaticText17;
        TStaticText *StaticText54;
        TDBText *DBText7;
        TDBText *DBText8;
        TMyQuery *MyQuery3;
        TStaticText *StaticText18;
        TEdit *attribute2_name;
        TMyTable *actions;
        TStaticText *StaticText20;
        TEdit *attribute4_name;
        TEdit *attribute4_val;
        TStaticText *StaticText30;
        TBitBtn *update_action;
        TDBText *DBText2;
        TDBText *DBText9;
        TDBText *DBText10;
        TDBText *DBText11;
        TSpeedButton *allow_action_animal_changes;
        TBitBtn *update_experiment_record;
        TStaticText *StaticText26;
        TDBText *action_ID;
        TButton *Button2;
        TLabel *Label7;
        TBitBtn *BitBtn32;
        TLabel *table_name;
    TXLSAdapter *XLSAdapter1;
    TFlexCelReport *SongReport;
    TSaveDialog *SaveDialog1;
    TMyTable *Song1;
    TTabSheet *TabSheet1;
    TMyDump *dump;
    TBitBtn *animals;
    TBitBtn *view_data_btn;
    TBitBtn *export_btn;
    TMemo *Memo1;
    TMemo *Memo3;
    TMemo *Memo5;
    TTabSheet *Export;
    TPanel *Panel1;
    TBitBtn *ToExel;
    TBitBtn *SimToExcel;
    TStaticText *StaticText42;
    TStaticText *StaticText48;
    TPanel *Panel2;
    TStaticText *StaticText51;
    TBitBtn *update_display;
    TLabel *Label8;
    TNumLab *age;
    TLabel *Label9;
    TNumLab *training;
    TDateTimePicker *trained;
    TLabel *Label19;
    TDateTimePicker *train_time;
    TBitBtn *set_time2;
    TEdit *name;
    TLabel *Label10;
    TEdit *weight;
    TLabel *Label21;
    TEdit *song;
    TLabel *Label22;
    TEdit *audio;
    TLabel *Label23;
    TComboBox *gender;
    TEdit *cage;
    TLabel *Label24;
    TEdit *song_quota;
    TLabel *Label25;
    TEdit *photoperiod;
    TLabel *Label26;
    TEdit *attributes;
    TLabel *Label27;
    TEdit *bird_comments;
    TLabel *Label20;
    TBitBtn *backup_btn;
    TBitBtn *create_table_btn;
    TMemo *Memo6;
    TMemo *Memo7;
    TPageControl *manage_tables;
    TTabSheet *backup;
    TBitBtn *backup_table;
    TBitBtn *restore_table;
    TBitBtn *backup_all;
    TBitBtn *restore_all;
    TProgressBar *backup_progress;
    TProgressBar *restore_progress;
    TTabSheet *view_tables;
    TBitBtn *openTable;
    TBitBtn *open_raw;
    TBitBtn *open_FILE_table;
    TBitBtn *open_Similarity;
    TBitBtn *open_settings;
    TTabSheet *create_tables;
    TBitBtn *new_table;
    TBitBtn *BitBtn9;
    TBitBtn *new_file_tableB;
    TBitBtn *new_settings;
    TBitBtn *empty_table;
    TBitBtn *delTable;
    TBitBtn *export_data;
    TBitBtn *new_brain_table;
    TButton *Button1;
    TBitBtn *BitBtn5;
    TBitBtn *BitBtn4;
    TLabel *backup_error;
    TMemo *Memo4;
    TMemo *Memo8;
    TMemo *Memo9;
    TMemo *Memo10;
    TMemo *Memo11;
    TLabel *Label18;
    TStaticText *StaticText21;
	TMyConnectDialog *MyConnectDialog1;

    TMyConnection *MyConnection1;
	TMemo *Memo12;
	TLinkLabel *LinkLabel1;
	TMemo *Memo13;

        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall openTableClick(TObject *Sender);
        void __fastcall ToExelClick(TObject *Sender);
        void __fastcall CountRecsClick(TObject *Sender);
        void __fastcall empty_tableClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall convertClick(TObject *Sender);
        void __fastcall open_tableClick(TObject *Sender);
        void __fastcall dataCountTbChange(TObject *Sender);
        void __fastcall startTbChange(TObject *Sender);
        void __fastcall BitBtn2Click(TObject *Sender);
        void __fastcall forwardClick(TObject *Sender);
        void __fastcall stepsClick(TObject *Sender);
        void __fastcall new_tableClick(TObject *Sender);
        void __fastcall SimToExcelClick(TObject *Sender);
        void __fastcall Add_daysClick(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall New_table_rawClick(TObject *Sender);
        void __fastcall open_SimilarityClick(TObject *Sender);
        void __fastcall DirectoryListBox1Change(TObject *Sender);
        void __fastcall FileListBox1Click(TObject *Sender);
        void __fastcall FileListBox1Change(TObject *Sender);
        void __fastcall dailyfoldersClick(TObject *Sender);
        void __fastcall new_brain_tableClick(TObject *Sender);
        void __fastcall delTableClick(TObject *Sender);
        void __fastcall new_file_tableBClick(TObject *Sender);
        void __fastcall open_FILE_tableClick(TObject *Sender);
        void __fastcall hatchedClick(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall DBNavigator1Click(TObject *Sender,
          TNavigateBtn Button);
        void __fastcall procedure1_scheduleChange(TObject *Sender);
        void __fastcall trainedClick(TObject *Sender);
        void __fastcall procedure2_scheduleClick(TObject *Sender);
        void __fastcall Add_experiment_recordClick(TObject *Sender);
        void __fastcall update_tableClick(TObject *Sender);
        void __fastcall add_actionClick(TObject *Sender);
        void __fastcall procedure3_scheduleClick(TObject *Sender);
        void __fastcall animals_gridCellClick(TColumn *Column);
        void __fastcall allow_changesClick(TObject *Sender);
        void __fastcall set_time2Click(TObject *Sender);
        void __fastcall set_time1Click(TObject *Sender);
        void __fastcall set_time3Click(TObject *Sender);
        void __fastcall set_time4Click(TObject *Sender);
        void __fastcall set_time5Click(TObject *Sender);
        void __fastcall procedure1_timeChange(TObject *Sender);
        void __fastcall procedure2_timeChange(TObject *Sender);
        void __fastcall procedure3_timeChange(TObject *Sender);
        void __fastcall train_timeChange(TObject *Sender);
        void __fastcall animals_gridDrawColumnCell(TObject *Sender,
          const TRect &Rect, int DataCol, TColumn *Column,
          TGridDrawState State);
        void __fastcall animals_gridTitleClick(TColumn *Column);
        void __fastcall allow_action_changesClick(TObject *Sender);
        void __fastcall actions_gridCellClick(TColumn *Column);
        void __fastcall actions_gridDrawColumnCell(TObject *Sender,
          const TRect &Rect, int DataCol, TColumn *Column,
          TGridDrawState State);
        void __fastcall update_actionClick(TObject *Sender);
        void __fastcall allow_action_animal_changesClick(TObject *Sender);
        void __fastcall action_animal_gridCellClick(TColumn *Column);
        void __fastcall action_animal_gridDrawColumnCell(TObject *Sender,
          const TRect &Rect, int DataCol, TColumn *Column,
          TGridDrawState State);
        void __fastcall new_settingsClick(TObject *Sender);
        void __fastcall open_settingsClick(TObject *Sender);
        void __fastcall BitBtn32Click(TObject *Sender);
    void __fastcall backup_tableClick(TObject *Sender);
    void __fastcall restore_tableClick(TObject *Sender);
    void __fastcall BitBtn4Click(TObject *Sender);
    void __fastcall BitBtn5Click(TObject *Sender);
    void __fastcall open_rawClick(TObject *Sender);
    void __fastcall dumpBackupProgress(TObject *Sender, UnicodeString ObjectName,
          int ObjectNum, int ObjectCount, int Percent);
    void __fastcall dumpRestoreProgress(TObject *Sender, int Percent);
    void __fastcall dumpError(TObject *Sender, Exception *E, UnicodeString SQL, TErrorAction &Action);
    void __fastcall backup_allClick(TObject *Sender);
    void __fastcall restore_allClick(TObject *Sender);
    void __fastcall animalsClick(TObject *Sender);
    void __fastcall view_data_btnClick(TObject *Sender);
    void __fastcall export_btnClick(TObject *Sender);
    void __fastcall data_tableEnter(TObject *Sender);
    void __fastcall data_tableExit(TObject *Sender);
    void __fastcall update_displayClick(TObject *Sender);
    void __fastcall export_dataClick(TObject *Sender);
    void __fastcall backup_btnClick(TObject *Sender);
    void __fastcall create_table_btnClick(TObject *Sender);
	void __fastcall nameChange(TObject *Sender);

private:	// User declarations
        AnsiString str, str2;
        bool show_table, checked[30];
        bool allow_update;
        int advance;
        void bird_update();
        void actions_update();
        void animal_actions_update();
		void Create_similarity_sections();
		ofstream myfile;


       // MYSQL_RES *result;

public:		// User declarations
        void Retrieve_segmentation_to_options(int chan);
		void Retrieve_frame_params(TFrame1 *frame, int chan);
        void Save_frame_params(TFrame1 *frame, int chan);
        __fastcall TDataForm(TComponent* Owner);
        void __fastcall CreateParams(Controls::TCreateParams &Params);
        TMyTable *TheTable;
        int chans_exist(AnsiString table_name, int num_slaves);
        bool Create_File_Table();
        bool fix_syll_table_fields(AnsiString table_name);
        int Count_Table_Recs(TMyTable* table);
        int file_indexes[10];
        int recNum, count_records(AnsiString name);
        int File_to_table(AnsiString bird_name, AnsiString SoundFile, double file_age, int bird_age, int chan);
        int find_age(int bird);
        void Settings(int bird, AnsiString feature, AnsiString value);
        void Create_table(AnsiString table_name);
        void Create_animals();
        bool retrieve_animal(AnsiString name);
        void Fetch_data(int channel, int location, int min_duration, int max_duration,  int amount);
        void Select_data(AnsiString quary);
        void Select_data_freeStyle(AnsiString quary);
        bool brain_format(AnsiString table_name);
        AnsiString Retrieve(int bird, AnsiString feature);
        int Retrieve_int(int bird, AnsiString feature);
        AnsiString file_table_name, settings_table_name;
        void ChangeTable(AnsiString name, AnsiString feature, AnsiString val, int location);
        void ChangeTable_freestyle(AnsiString command);
        bool show_start;
        void showTables(), showDatabases(), Change_Database(AnsiString name);
        bool table_exists(AnsiString table_name);
        bool database_exists(AnsiString database_name);
        AnsiString new_raw_table(AnsiString name, int num_slaves);
        AnsiString GetFileName(AnsiString table_name, int index);
        AnsiString open_Raw_Table();
      //  void new_raw_brain_table(AnsiString name);
        int count_files();
        void Create_Brain_syllable_table(AnsiString table_name);
        void create_settings(AnsiString settings_table);
        void create_database(AnsiString name);
        void create_similarity(AnsiString similarity_table);
        void Create_events(), Create_actions();
        void create_durations();
        void Create_bouts();
        void insert_bout(AnsiString bird, int bout_dur, AnsiString newFile);
        void DeleteTable(AnsiString table_name);
        void initiate_handler(AnsiString database, AnsiString table);
        void get_recs(AnsiString table, int amount);
        double GetFile_serial(AnsiString table_name, int index);
        int update_file_table_index(int chan);
        void insert_dur(int i);
        void insert_comment(AnsiString bird_name, AnsiString file_name, AnsiString comments);
        AnsiString Retrieve_comments(AnsiString file_name);
        void Insert_similarity_section(int num,float Xs, float Xe, float Ys, float Ye, float accuracy);
        void Insert_syllable_record(
                    AnsiString table_name,
                    double serial_number,
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
                );
                
                void Insert_syllable_brain_record(
                    AnsiString table_name,
                    double serial_number,
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
        );
};
void __fastcall TDataForm::CreateParams(Controls::TCreateParams &Params)
{
  TForm::CreateParams(Params);
  Params.ExStyle   = Params.ExStyle | WS_EX_APPWINDOW;
  Params.WndParent = ParentWindow;
}
//---------------------------------------------------------------------------
extern PACKAGE TDataForm *DataForm;
//---------------------------------------------------------------------------
#endif
