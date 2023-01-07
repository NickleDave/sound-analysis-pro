// ---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#include <fstream.h>
#pragma hdrstop
#include "SongMeasures.h"
#include "tapers.h"
#include "SDL_vector.hpp"
#include "start.h"
#include "CZT_FeatureCalc.h"
#include "CZT_FeatureImage.h"
#include "CZT_PcmEq.h"
#include "options3.h"
#include "tapers.h"
#include "records.h"
#include "animals.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SDL_NumLab"
#pragma link "SDL_rchart"
#pragma link "cspin"
#pragma link "AdvSmoothJogWheel"
#pragma link "DBAccess"
#pragma link "MyAccess"
#pragma link "SDL_marksel"
#pragma link "SDL_ntabed"
#pragma link "SDL_replist"
#pragma resource "*.dfm"
#define ShowDeriv       0
#define ShowContours    1
#define ShowSonogram    2
#define Xs              1
#define Ys              2
#define Xe              3
#define Ye              4
#define Val             5
#define ValidComp       6
#define ksong1          0
#define ksong2          1
#define kList1  0
#define kList2  1
#define k22             1
#define k44             2
#define DEFAULT_MAX    1000000
#define auto_outline  1
#define auto_segment  2
#define fit_screen  0

TForm1 *Form1;
// ---------------------------------------------------------------------------

/* This method set initial defalut parameters and start the dialog box for selecting sounds */
__fastcall TForm1::TForm1(TComponent* Owner) : TForm(Owner) {
	feature_at_pointer_cursor = 1; // for the feature_at_pointer_list
	feature_at_pointer2_cursor = 1;
	feature_vector_cursor = 0;
	feature_vector2_cursor = 0;
	feature_statistics_cursor = 1;
	feature_statistics2_cursor = 1;
	similarity_cursor=1;
	similarity_batch_cursor=1;
	comparison_cursor=1;
	Fill_feature_headers();
	option->cepstrum_pitch=true;
    option->module=0;
	option->bitmapHeight = 250;
    option->squiz = 1; // we will show the derivaitve and sonogram images using maximum definition
    option->save_image_mem = false; // do not use the save memory options that compromize image quality
    option->save_all = true; // we will consider every file the same, as if it contains good data
    option->format->ItemIndex = 1; // get serial number and time from file age...
    option->saveRawFile = false;
    option->keep_original_rawfile_name = true;
    option->create_feature_arrays = true; // CZT_featureCalc will generate arrays of feature values
	option->Draw_sonogram = false; // Draw derivatives only is the default
    option->select_table = 0; // The results will be saved in table 0, which is called Recs0.
	option->chan = 0;
	no_pagination_action=false;
    DataForm->MyConnection1->Database = "SAP";
    similarity_matrix_exists = false;
	// DataForm->settings->TableName="settings";
    DataForm->settings->Active = true;
    // DataForm->Change_Database("SAP");
    // option->features_database="SAP";
    THRESH = 1.08;
    Local_Thresh = 0.32; // MADs, which is p=0.05
    parameters_altered = false;
    // WindowState=wsMaximized;
    update_score_sounds = true;
    option->refine_segmentation = false;
    tmpSong1 = false;
    tmpSong2 = false;
    FinalSong1 = false;
    FinalSong2 = false;
    dobatch = false;
    TwoSongs = false;
    ready = false;
    clicked = false;
    score = false;
    Show1 = ShowDeriv;
    Show2 = ShowDeriv; // set the default spectral image to the derivative presentation
    Zoom = 0; // set display zoom of score table to scale 1
    interval1 = false;
	interval2 = false;
	batchAlt = false;
	preview_state = 1;
	preview_state2 = 1;

    n = 1;
    state1 = 0;
	state2 = 0;
	calc_advance1=calc_advance2=0;
    memo1Clear = false;
    memo2Clear = false;
    DirectoryListBox1->Directory = "c:\\sap";
    savgol(Cf, 5, 2, 2, 0, 2);
    savgol(Cfl, 7, 3, 3, 0, 2); // compute smoothing fuctions:
    try {
        DataForm->Records->Active = true;
        DataForm->CountRecsClick(this);
        DataForm->file_table_name = "file_table";
        derivTB->Position = DataForm->Retrieve(1, "deriv_thresh").ToInt();
        //deriv_val->Value = (125 - derivTB->Position) / 1.25;
        int pri_feature = DataForm->Retrieve(1, "pri_segmented_feature").ToInt();
        deriv_val->Value = option->adjustThreshold(pri_feature, derivTB->Position, option->frequency, 1);
        entropyTb->Position = DataForm->Retrieve(1, "entropy_thresh").ToInt();
        int sec_feature = DataForm->Retrieve(1, "sec_segmented_feature").ToInt();
        //entropy_val->Value = (float) - entropyTb->Position / 50;
        entropy_val->Value = option->adjustThreshold(sec_feature, entropyTb->Position, option->frequency, 0);

        derivTB2->Position = DataForm->Retrieve(2, "deriv_thresh").ToInt(); //derivTB->Position;
        //deriv_val2->Value = (125 - derivTB2->Position);
        pri_feature = DataForm->Retrieve(2, "pri_segmented_feature").ToInt();
        deriv_val2->Value = option->adjustThreshold(pri_feature, derivTB2->Position, option->frequency, 1);
        entropyTb2->Position = DataForm->Retrieve(2, "entropy_thresh").ToInt(); //entropyTb->Position;
        //entropy_val2->Value = (float) - entropyTb2->Position / 50;
        sec_feature = DataForm->Retrieve(2, "sec_segmented_feature").ToInt();
        entropy_val2->Value = option->adjustThreshold(sec_feature, entropyTb2->Position, option->frequency, 0);

		sldrBrightness->Position = DataForm->Retrieve(1, "display_contrast")
        .ToInt();
        sldrBrightness2->Position = DataForm->Retrieve(2, "display_contrast")
        .ToInt();
        auto_gain->Checked = true;

        data_window->Position = DataForm->Retrieve(1, "FFT_window").ToInt();
        data_window2->Position = DataForm->Retrieve(2, "FFT_window").ToInt();
		advance->Position = DataForm->Retrieve(1, "advance_window").ToInt();
		advance2->Position = DataForm->Retrieve(2, "advance_window").ToInt();
        frequency_range->Position = DataForm->Retrieve(1, "frequency_range")
		.ToInt();
		SaveGraph->InitialDir=DataForm->Retrieve(1,"sonogram_folder");

        option->hp_amp = false;
		Frame11->hp_big->Visible = false;
        Frame11->hp_big_num->Visible = false;
        Frame11->StaticText50->Visible = false;
        Frame11->hp_small->Visible = false;
        Frame11->hp_small_num->Visible = false;
        Frame11->StaticText49->Visible = false;
        Frame11->secondary_segmentation_panel->Visible = true;
        Frame11->secondaryLogicRB->Visible = false;
        Frame11->secondaryFeatureDD->Visible = false;
        Frame11->secondaryComparisonRB->Visible = false;
		Frame11->secondaryThresholdTB->Visible = false;
		Frame11->save->Visible=false;

		Frame12->hp_big->Visible = false;
		Frame12->hp_big_num->Visible = false;
		Frame12->StaticText50->Visible = false;
		Frame12->hp_small->Visible = false;
		Frame12->hp_small_num->Visible = false;
		Frame12->StaticText49->Visible = false;
		Frame12->secondary_segmentation_panel->Visible = true;
		Frame12->secondaryLogicRB->Visible = false;
		Frame12->secondaryFeatureDD->Visible = false;
		Frame12->secondaryComparisonRB->Visible = false;
		Frame12->secondaryThresholdTB->Visible = false;
		Frame12->save->Visible=false;

		entropyTb->Visible = false;
		DerivPowerB->Caption = "Amp";
		entropy_val->Visible = false;
		ent_label->Visible = false;

        entropyTb2->Visible = false;
        entropy_val2->Visible = false;
        ent_label2->Visible = false;

        updateSegmentation = false;
        updateSegmentation2 = false;
        RecordsNum->Caption = DataForm->recNum; // Records->RecordCount;
        table_name->Caption = DataForm->Records->TableName;
		option->syllable_table_name = table_name->Caption;
		sldrBrightnessChange(this);
	}
	catch(...) {
        Application->MessageBox(
            L"could not open the settings table, default settings will be applied"
            , NULL, MB_OK);
        DataForm->Create_table("recs_n0");
        DataForm->Records1->TableName = "recs_n0";
        option->syllable_table_name = "recs_n0";
        DataForm->file_table_name = "file_table";
    }
    Sonogram = new Graphics::TBitmap();
    derivTB->Brush->Color = clYellow;
    entropyTb->Brush->Color = clRed;
    derivTB2->Brush->Color = clYellow;
    entropyTb2->Brush->Color = clRed;


	m_Calculator1.m_Image.m_nMaxSlices = 100000;
	m_Calculator1.m_sonogram.m_nMaxSlices = 100000;
	/*
	m_Calculator1.GetImage().GetPitch().bRender = false;
	m_Calculator1.GetImage().GetEntropy().bRender = false;
	m_Calculator1.GetImage().GetFM().bRender = false;
	m_Calculator1.GetImage().GetPitchGoodness().bRender = false;
	m_Calculator1.GetImage().GetPeakFreq().bRender = false;
	m_Calculator1.GetImage().Gethp_small().bRender = false;
	m_Calculator1.GetImage().Gethp_big().bRender = false;
	m_Calculator1.GetImage().Gethp_diff().bRender = false;
	*/
	m_Calculator2.m_Image.m_nMaxSlices = 100000;
	m_Calculator2.m_sonogram.m_nMaxSlices = 100000;
	/*
	m_Calculator2.GetImage().GetPitch().bRender = false;
	m_Calculator2.GetImage().GetEntropy().bRender = false;
	m_Calculator2.GetImage().GetFM().bRender = false;
	m_Calculator2.GetImage().GetPitchGoodness().bRender = false;
	m_Calculator2.GetImage().GetPeakFreq().bRender = false;
	m_Calculator2.GetImage().Gethp_small().bRender = false;
	m_Calculator2.GetImage().Gethp_big().bRender = false;
	m_Calculator2.GetImage().Gethp_diff().bRender = false;
	*/

	GUI = true;
	score = false;
	active_view = false;
	Similarity->TabVisible = false;
	Combined->TabVisible = false;
	show_song2->Visible = false;
    per_sound2->Visible = false;

    OnsetSong1 = 1;
    OnsetSong2 = 1;
	OffsetSong1 = Iterations1 - 1;
    OffsetSong2 = Iterations2 - 1;
	doIncrement = true;

    Image10->Canvas->Rectangle(0, 0, Image10->Width, Image1->Height);
    Image11->Canvas->Rectangle(0, 0, Image10->Width, Image1->Height);
    Image12->Canvas->Rectangle(0, 0, Image10->Width, Image1->Height);
    Image13->Canvas->Rectangle(0, 0, Image10->Width, Image1->Height);
    Image14->Canvas->Rectangle(0, 0, Image10->Width, Image1->Height);
    //Form1->WindowState = wsMaximized;
	EntropyB->Down = false;
	Entropy2B->Down = false;
	DerivPowerB->Down = false;
	mfaB->Down = false;
	//DerivPowerBClick(this);
	mfaBClick(this);
   // SaveGraph->InitialDir = "C:\\sap";
	update_template(); // different features values in feature scale table (zebra finch, canary, etc).


	similarity_view->Header[1] = "Sound 1";
	similarity_view->Header[2] = "Sound 2";
	similarity_view->Header[3] = "%Similarity";
	similarity_view->Header[4] = "Accuracy";
	similarity_view->Header[5] = "%Sequential";
	similarity_view->Header[6] = "Pitch diff";
	similarity_view->Header[7] = "FM diff";
	similarity_view->Header[8] = "Entropy diff";
	similarity_view->Header[9] = "Goodness diff";
	similarity_view->Header[10] = "AM diff";
	similarity_view->Header[11] = "From(1)";
	similarity_view->Header[12] = "To(1)";
	similarity_view->Header[13] = "From(2)";
	similarity_view->Header[14] = "To(2)";
	similarity_view->Header[15] = "Comments";

	similarity_batch_view->Header[1] = "Sound 1";
	similarity_batch_view->Header[2] = "Sound 2";
	similarity_batch_view->Header[3] = "%Similarity";
	similarity_batch_view->Header[4] = "Accuracy";
	similarity_batch_view->Header[5] = "%Sequential";
	similarity_batch_view->Header[6] = "Pitch diff";
	similarity_batch_view->Header[7] = "FM diff";
	similarity_batch_view->Header[8] = "Entropy diff";
	similarity_batch_view->Header[9] = "Goodness diff";
	similarity_batch_view->Header[10] = "AM diff";
	similarity_batch_view->Header[11] = "From(1)";
	similarity_batch_view->Header[12] = "To(1)";
	similarity_batch_view->Header[13] = "From(2)";
	similarity_batch_view->Header[14] = "To(2)";
	similarity_batch_view->Header[15] = "Comments";

	comparison_view->Header[1] = "Sound 1";
	comparison_view->Header[2] = "Sound 2";
	comparison_view->Header[3] = "Distribution score";
	comparison_view->Header[4] = "KS all";
	comparison_view->Header[5] = "KS Amplitude";
	comparison_view->Header[6] = "KS pitch";
	comparison_view->Header[7] = "KS FM";
	comparison_view->Header[8] = "KS entropy";
	comparison_view->Header[9] = "KS goodness";
	comparison_view->Header[10] = "KS mean Fr.";
	comparison_view->Header[11] = "From(1)";
	comparison_view->Header[12] = "To(1)";
	comparison_view->Header[13] = "From(2)";
	comparison_view->Header[14] = "To(2)";
	comparison_view->Header[15] = "Comments";
}


// ---------------------------------------------------------------------------

void TForm1::update_template() {
	int cnt = DataForm->Count_Table_Recs(option->scale);
    templates->Clear();
    option->scale->First();
    for (int i = 0; i < cnt; i++) {
        str = option->scale->FieldValues["domain"];
        templates->Items->Add(str);
        option->scale->Next();
    }
}

// ---------------------------------------------------------------------------

/* This function is called when the user has outlined begining and end of a sound.
It calculate average value, minimum value, maximum values and variance values of features within
an interval. It is used both for Sound1 or Sound2. The output is displayed in the 'features across interval'
pannel. Note the it also set Onset and Offset values for parts of the sounds the will be analyzed later on.
For example, only the outlined parts will be used for similarity measurments. */
bool TForm1::CalculateIntervalMeasures(int start, int end)

{
    TNumLab *beginLb, *endLb, *durationLb, *pitchAvgLb, *pitchMinLb,
    *pitchMaxLb, *pitchVarLb, *FMAvgLb, *FMminLb, *FMmaxLb, *FMVarLb,
    *FMcAvgLb, *FMcminLb, *FMcmaxLb, *FMcVarLb, *AMAvgLb, *AMminLb, *AMmaxLb,
    *AMVarLb, *entAvgLb, *entMinLb, *entMaxLb, *entVarLb, *derivAvgLb,
    *derivMinLb, *derivMaxLb, *derivVarLb, *peakFreqAvgLb, *peakFreqMinLb,
    *peakFreqMaxLb, *peakFreqVarLb, *principalFreqAvgLb, *principalFreqMinLb,
    *principalFreqMaxLb, *principalFreqVarLb, *pgoodAvgLb, *pgoodMinLb,
    *pgoodMaxLb, *pgoodVarLb, *cont_tAvgLb, *cont_tMinLb, *cont_tMaxLb,
    *cont_tVarLb, *cont_fAvgLb, *cont_fMinLb, *cont_fMaxLb, *cont_fVarLb;

    bool *signal;
	int *ind; // T *ptr;  ptr = &t1;
	int currentStartLoc;
	 bool pagination_On;

    float *pitch, *fm, *fmc, *am, *entropy, *Amplitude, *Pgood, *PeakFr,
    *PrincipalFr, *Cont_t, *Cont_f;

    float *AvgPitch, Adj_AvgPitch, *AvgFM, *AvgFMc, *AvgAM, *AvgEnt,
    *AvgAmplitude, *AvgPGood, *AvgPeakFr, *AvgPrincipalFr, *AvgCont_t,
    *AvgCont_f, *MinPitch, *MinFM, *MinFMc, *MinAM, *MinEnt, *MinAmplitude,
	*MinPGood, *MinPeakFr, *MinPrincipalFr, *MinCont_t, *MinCont_f, *MaxPitch,
    *MaxFM, *MaxFMc, *MaxAM, *MaxEnt, *MaxAmplitude, *MaxPGood, *MaxPeakFr,
    *MaxPrincipalFr, *MaxCont_t, *MaxCont_f, *position_maxFreq,
    *position_maxAmp;


    /*int adj = X * tot_pages / preview_sonogram->Width;
	*currentStartLoc = adj;
    *pageNum = (*currentStartLoc) / 1000;   */
    if (sound == 0) // Sound 1
    {
		currentStartLoc = option->currentStartLocation;
		pagination_On = option->paginationOn;

		signal = Signal1;
        ind = &ind1;

		beginLb = beginL;
		endLb = endL;
		durationLb = durationL;
		pitchAvgLb = pitchAvgL;
        pitchMinLb = pitchMinL;
        pitchMaxLb = pitchMaxL;
        pitchVarLb = pitchVarL;
        FMAvgLb = FMAvgL;
        FMminLb = FMminL;
        FMmaxLb = FMmaxL;
        FMVarLb = FMVarL;
        FMcAvgLb = FMcAvgL;
        FMcminLb = FMcMinL;
        FMcmaxLb = FMcMaxL;
        FMcVarLb = FMcVarL;
        AMAvgLb = AMAvgL;
        AMminLb = AMMinL;
        AMmaxLb = AMMaxL;
        AMVarLb = AMVarL;
        entAvgLb = entAvgL;
        entMinLb = entMinL;
        entMaxLb = entMaxL;
        entVarLb = entVarL;
        derivAvgLb = derivAvgL;
        derivMinLb = derivMinL;
        derivMaxLb = derivMaxL;
        derivVarLb = derivVarL;
        peakFreqAvgLb = peakFreqAvgL;
        peakFreqMinLb = peakFreqMinL;
        peakFreqMaxLb = peakFreqMaxL;
        peakFreqVarLb = peakFreqVarL;
        principalFreqAvgLb = principalFreqAvgL;
        principalFreqMinLb = principalFreqMinL;
        principalFreqMaxLb = principalFreqMaxL;
        principalFreqVarLb = principalFreqVarL;
        pgoodAvgLb = pgoodAvgL;
        pgoodMinLb = pgoodMinL;
        pgoodMaxLb = pgoodMaxL;
        pgoodVarLb = pgoodVarL;
        cont_tAvgLb = cont_tAvgL;
        cont_tMinLb = cont_tMinL;
        cont_tMaxLb = cont_tMaxL;
        cont_tVarLb = cont_tVarL;
        cont_fAvgLb = cont_fAvgL;
        cont_fMinLb = cont_fMinL;
        cont_fMaxLb = cont_fMaxL;
        cont_fVarLb = cont_fVarL;

        pitch = Pitch1;
        fm = FM1;
        fmc = FMc1;
        am = AM1;
        entropy = Entropy1;
        Amplitude = Amplitude1;
        Pgood = PitchGoodness1;
        PeakFr = PeakFreq1;
        PrincipalFr = principal_frequency1;
        Cont_t = continuity_t1;
        Cont_f = continuity_f1;

        AvgPitch = &AvgPitch1;
        AvgFM = &AvgFM1;
        AvgFMc = &AvgFMc1;
        AvgAM = &AvgAM1;
        AvgEnt = &AvgEnt1;
        AvgAmplitude = &AvgAmplitude1;
        AvgPGood = &AvgPGood1;
        AvgPeakFr = &AvgPeakFr1;
        AvgPrincipalFr = &AvgPrincipalFr1;
        AvgCont_t = &AvgCont_t1;
        AvgCont_f = &AvgCont_f1;
        MinPitch = &MinPitch1;
        MinFM = &MinFM1;
        MinFMc = &MinFMc1;
        MinAM = &MinAM1;
        MinEnt = &MinEnt1;
        MinAmplitude = &MinAmplitude1;
        MinPGood = &MinPGood1;
        MinPeakFr = &MinPeakFr1;
        MinPrincipalFr = &MinPrincipalFr1;
        MinCont_t = &MinCont_t1;
        MinCont_f = &MinCont_f1;
        MaxPitch = &MaxPitch1;
        MaxFM = &MaxFM1;
        MaxFMc = &MaxFMc1;
        MaxAM = &MaxAM1;
        MaxEnt = &MaxEnt1;
        MaxAmplitude = &MaxAmplitude1;
        MaxPGood = &MaxPGood1;
        MaxPeakFr = &MaxPeakFr1;
        MaxPrincipalFr = &MaxPrincipalFr1;
        MaxCont_t = &MaxCont_t1;
        MaxCont_f = &MaxCont_f1;
        VarPitch = &VarPitch1;
        VarFM = &VarFM1;
        VarFMc = &VarFMc1;
        VarAM = &VarAM1;
        VarEnt = &VarEnt1;
        VarAmplitude = &VarAmplitude1;
        VarPGood = &VarPGood1;
        VarPeakFr = &VarPeakFr1;
        VarPrincipalFr = &VarPrincipalFr1;
        VarCont_t = &VarCont_t1;
        VarCont_f = &VarCont_f1;
        position_maxFreq = &position_maxFreq1;
        position_maxAmp = &position_maxAmp1;
    }
    else // Sound 2
    {
		currentStartLoc = option->currentStartLocation;
		pagination_On = option->paginationOn;

		signal = Signal2;
        ind = &ind2;

        beginLb = begin2L;
        endLb = end2L;
        durationLb = duration2L;
        pitchAvgLb = pitch2AvgL;
        pitchMinLb = pitch2MinL;
        pitchMaxLb = pitch2MaxL;
        pitchVarLb = pitch2VarL;
        FMAvgLb = FM2AvgL;
        FMminLb = FM2minL;
        FMmaxLb = FM2maxL;
        FMVarLb = FM2VarL;
        FMcAvgLb = FMc2AvgL;
        FMcminLb = FMc2MinL;
        FMcmaxLb = FMc2MaxL;
        FMcVarLb = FMc2VarL;
        AMAvgLb = AM2AvgL;
        AMminLb = AM2MinL;
        AMmaxLb = AM2MaxL;
        AMVarLb = AM2VarL;
        entAvgLb = ent2AvgL;
        entMinLb = ent2MinL;
        entMaxLb = ent2MaxL;
        entVarLb = ent2VarL;
        derivAvgLb = deriv2AvgL;
        derivMinLb = deriv2MinL;
        derivMaxLb = deriv2MaxL;
        derivVarLb = deriv2VarL;
        peakFreqAvgLb = peakFreq2AvgL;
        peakFreqMinLb = peakFreq2MinL;
        peakFreqMaxLb = peakFreq2MaxL;
        peakFreqVarLb = peakFreq2VarL;
        principalFreqAvgLb = principalFreq2AvgL;
        principalFreqMinLb = principalFreq2MinL;
        principalFreqMaxLb = principalFreq2MaxL;
        principalFreqVarLb = principalFreq2VarL;
        pgoodAvgLb = pgood2AvgL;
        pgoodMinLb = pgood2MinL;
        pgoodMaxLb = pgood2MaxL;
        pgoodVarLb = pgood2VarL;
        cont_tAvgLb = cont_t2AvgL;
        cont_tMinLb = cont_t2MinL;
        cont_tMaxLb = cont_t2MaxL;
        cont_tVarLb = cont_t2VarL;
        cont_fAvgLb = cont_f2AvgL;
        cont_fMinLb = cont_f2MinL;
        cont_fMaxLb = cont_f2MaxL;
        cont_fVarLb = cont_f2VarL;

        pitch = Pitch2;
        fm = FM2;
        fmc = FMc2;
        am = AM2;
        entropy = Entropy2;
        Amplitude = Amplitude2;
        Pgood = PitchGoodness2;
        PeakFr = PeakFreq2;
        PrincipalFr = principal_frequency2;
        Cont_t = continuity_t2;
        Cont_f = continuity_f2;

        AvgPitch = &AvgPitch2;
        AvgFM = &AvgFM2;
        AvgFMc = &AvgFMc2;
        AvgAM = &AvgAM2;
        AvgEnt = &AvgEnt2;
        AvgAmplitude = &AvgAmplitude2;
        AvgPGood = &AvgPGood2;
        AvgPeakFr = &AvgPeakFr2;
        AvgPrincipalFr = &AvgPrincipalFr2;
        AvgCont_t = &AvgCont_t2;
        AvgCont_f = &AvgCont_f2;
        MinPitch = &MinPitch2;
        MinFM = &MinFM2;
        MinFMc = &MinFMc2;
        MinAM = &MinAM2;
        MinEnt = &MinEnt2;
        MinAmplitude = &MinAmplitude2;
        MinPGood = &MinPGood2;
        MinPeakFr = &MinPeakFr2;
        MinPrincipalFr = &MinPrincipalFr2;
        MinCont_t = &MinCont_t2;
        MinCont_f = &MinCont_f2;
        MaxPitch = &MaxPitch2;
        MaxFM = &MaxFM2;
        MaxFMc = &MaxFMc2;
        MaxAM = &MaxAM2;
        MaxEnt = &MaxEnt2;
        MaxAmplitude = &MaxAmplitude2;
        MaxPGood = &MaxPGood2;
        MaxPeakFr = &MaxPeakFr2;
        MaxPrincipalFr = &MaxPrincipalFr2;
        MaxCont_t = &MaxCont_t2;
        MaxCont_f = &MaxCont_f2;
        VarPitch = &VarPitch2;
        VarFM = &VarFM2;
        VarFMc = &VarFMc2;
        VarAM = &VarAM2;
        VarEnt = &VarEnt2;
        VarAmplitude = &VarAmplitude2;
        VarPGood = &VarPGood2;
        VarPeakFr = &VarPeakFr2;
        VarPrincipalFr = &VarPrincipalFr2;
        VarCont_t = &VarCont_t2;
        VarCont_f = &VarCont_f2;
        position_maxFreq = &position_maxFreq2;
        position_maxAmp = &position_maxAmp2;
    }

    int j, fmc_index = 1;
    *ind = 0;
    *AvgPitch = 0;
    Adj_AvgPitch = 0;
    *AvgFM = 0;
    *AvgFMc = 0;
    *AvgAM = 0;
    *AvgEnt = 0;
    *AvgAmplitude = 0;
    *AvgPGood = 0;
    *AvgPeakFr = 0;
    *AvgPrincipalFr = 0;
    *AvgCont_t = 0;
    *AvgCont_f = 0;
    *MinPitch = 100000.0;
    *MinFM = 100000.0;
    *MinFMc = 100000.0;
    *MinAM = 100000.0;
    *MinEnt = 0;
    *MinAmplitude = 100000.0;
    *MinPGood = 100000.0;
    *MinPeakFr = 100000;
    *MinPrincipalFr = 100000;
    *MinCont_t = 100000;
    *MinCont_f = 100000;
    *MaxPitch = 0;
    *MaxFM = 0;
    *MaxFMc = 0;
    *MaxAM = 0;
    *MaxEnt = -10000.0;
    *MaxAmplitude = 0;
    *MaxPGood = 0;
    *MaxPeakFr = 0;
    *MaxPrincipalFr = 0;
    *MaxCont_t = 0;
    *MaxCont_f = 0;
    *VarPitch = 0;
    *VarFM = 0;
    *VarFMc = 0;
    *VarAM = 0;
    *VarEnt = 0;
    *VarAmplitude = 0;
    *VarPGood = 0;
    *VarPeakFr = 0;
    *VarPrincipalFr = 0;
    *VarCont_t = 0;
    *VarCont_f = 0;


    for (int i = min(start, end); i < max(start, end); i++)
        if (signal[i]) {

            *AvgPitch += pitch[i];
            *AvgFM += fm[i];
            if (fmc[i] > -999) {
                *AvgFMc += fmc[i];
                fmc_index++;
            }
            *AvgAM += am[i];
            *AvgEnt += entropy[i];
            *AvgAmplitude += Amplitude[i];
            *AvgPGood += Pgood[i];
            *AvgPeakFr += PeakFr[i];
            *AvgPrincipalFr += PrincipalFr[i];
            *AvgCont_t += Cont_t[i];
            *AvgCont_f += Cont_f[i];

            *VarPitch += pitch[i] * pitch[i];
            *VarFM += fm[i] * fm[i];
            if (fmc[i] > -999)
                * VarFMc += fmc[i] * fmc[i];
            *VarAM += am[i] * am[i];
            *VarEnt += entropy[i] * entropy[i];
            *VarAmplitude += Amplitude[i] * Amplitude[i];
            *VarPGood += Pgood[i] * Pgood[i];
            *VarPeakFr += PeakFr[i] * PeakFr[i];
            *VarPrincipalFr += PrincipalFr[i] * PrincipalFr[i];
            *VarCont_t += Cont_t[i] * Cont_t[i];
            *VarCont_f += Cont_f[i] * Cont_f[i];

            if (adjust_pitch->Checked)
                Adj_AvgPitch += (Pgood[i] * pitch[i]);
            if (*MaxFM < fm[i])
                * MaxFM = fm[i];
            if (*MaxFMc < fmc[i])
                * MaxFMc = fmc[i];
            if (*MaxAM < am[i])
                * MaxAM = am[i];
            if (*MaxPitch < pitch[i])
                * MaxPitch = pitch[i];
            if (*MaxEnt < entropy[i])
                * MaxEnt = entropy[i];
            if (*MaxAmplitude < Amplitude[i]) {
                *MaxAmplitude = Amplitude[i];
                *position_maxAmp = i;
            }
            if (*MaxPeakFr < PeakFr[i]) {
                *MaxPeakFr = PeakFr[i];
                *position_maxFreq = i;
            }
            if (*MaxPrincipalFr < PrincipalFr[i]) {
                *MaxPrincipalFr = PrincipalFr[i];
            }
            if (*MaxPGood < Pgood[i])
                * MaxPGood = Pgood[i];
            if (*MaxCont_t < Cont_t[i])
                * MaxCont_t = Cont_t[i];

            if (*MaxCont_f < Cont_f[i])
                * MaxCont_f = Cont_f[i];

            if (*MinFM > fm[i])
                * MinFM = fm[i];
            if (fmc[i] > -999 && *MinFMc > fmc[i])
                * MinFMc = fmc[i];
            if (*MinAM > am[i])
                * MinAM = am[i];
            if (*MinPitch > pitch[i])
                * MinPitch = pitch[i];
            if (*MinEnt > entropy[i])
                * MinEnt = entropy[i];
            if (*MinAmplitude > Amplitude[i])
                * MinAmplitude = Amplitude[i];
            if (*MinPeakFr > PeakFr[i])
                * MinPeakFr = PeakFr[i];
            if (*MinPrincipalFr > PrincipalFr[i]) {
                *MinPrincipalFr = PrincipalFr[i];
            }
            if (*MinPGood > Pgood[i])
                * MinPGood = Pgood[i];
            if (*MinCont_t > Cont_t[i])
                * MinCont_t = Cont_t[i];
            if (*MinCont_f > Cont_f[i])
                * MinCont_f = Cont_f[i];

            (*ind)++;
        }

    if (!(*ind)) {
        Application->MessageBox(
            L"No signal identified -- please try again. If you do see sound in the outlined area, try changing the derivative power (yellow slide bar)", NULL, MB_OK);
        return(false);
    }
    else {
        *VarPitch = (*VarPitch - (1.0 / (*ind)) * (*AvgPitch) * (*AvgPitch)) /
            (*ind - 1);
        *VarFM = (*VarFM - (1.0 / (*ind)) * (*AvgFM) * (*AvgFM)) / (*ind - 1);
        *VarFMc = (*VarFMc - (1.0 / (*ind)) * (*AvgFMc) * (*AvgFMc)) /
            (*ind - 1);
        *VarAM = (*VarAM - (1.0 / (*ind)) * (*AvgAM) * (*AvgAM)) / (*ind - 1);
        *VarEnt = (*VarEnt - (1.0 / (*ind)) * (*AvgEnt) * (*AvgEnt)) /
            (*ind - 1);
        *VarAmplitude = (*VarAmplitude - (1.0 / (*ind)) * (*AvgAmplitude) *
            (*AvgAmplitude)) / (*ind - 1);
        *VarPGood = (*VarPGood - (1.0 / (*ind)) * (*AvgPGood) * (*AvgPGood)) /
            (*ind - 1);
        *VarPeakFr = (*VarPeakFr - (1.0 / (*ind)) * (*AvgPeakFr) * (*AvgPeakFr)
            ) / (*ind - 1);
        *VarPrincipalFr = (*VarPrincipalFr - (1.0 / (*ind)) * (*AvgPrincipalFr)
            * (*AvgPrincipalFr)) / (*ind - 1);
        *VarCont_t = (*VarCont_t - (1.0 / (*ind)) * (*AvgCont_t) * (*AvgCont_t)
            ) / (*ind - 1);
        *VarCont_f = (*VarCont_f - (1.0 / (*ind)) * (*AvgCont_f) * (*AvgCont_f)
            ) / (*ind - 1);

        if (adjust_pitch->Checked)
            * AvgPitch = (Adj_AvgPitch) / (*AvgPGood);
        // *AvgPitch/=(*AvgPGood);
        else
            *AvgPitch /= *ind;
        *AvgFM /= *ind;
        *AvgFMc *= option->frequency_scale;
        *AvgFMc /= (float)fmc_index;
        *AvgAM /= *ind;
        *AvgEnt /= *ind;
        *AvgAmplitude /= *ind;
        *AvgPGood /= *ind;
        *AvgPeakFr /= *ind;
        *AvgPrincipalFr /= *ind;
        *AvgCont_t /= *ind;
        *AvgCont_f /= *ind;
        *MinFMc *= option->frequency_scale;
        *MaxFMc *= option->frequency_scale;
        *VarFMc *= 1849.0;
        *AvgPrincipalFr *= option->frequency_scale;
        *MinPrincipalFr *= option->frequency_scale;
        *MaxPrincipalFr *= option->frequency_scale;
		*VarPrincipalFr *= 1849;

	   if(pagination_On){
	   beginLb->Value = start * option->advanceTb->Value + ((float)currentStartLoc - 1000.0) * option->advanceTb->Value;
	   endLb->Value = end * option->advanceTb->Value + ((float)currentStartLoc - 1000.0) * option->advanceTb->Value;
	   }
	   else {
	   beginLb->Value = start * option->advanceTb->Value;
	   endLb->Value = end * option->advanceTb->Value;
	   }

		// 1.4;//Slide*rangeIndex*750/DigiRate;

		// Slide*rangeIndex*750/DigiRate;
        durationLb->Value = endLb->Value - beginLb->Value;

        *position_maxAmp = (*position_maxAmp - start) / float(end - start);
        *position_maxFreq = (*position_maxFreq - start) / float(end - start);
        Pos_maxAmp1->Value = *position_maxAmp;
        Pos_maxFreq1->Value = *position_maxFreq;

        peakFreqAvgLb->Value = (*AvgPeakFr);
        peakFreqMinLb->Value = (*MinPeakFr);
        peakFreqMaxLb->Value = (*MaxPeakFr);
        peakFreqVarLb->Value = (*VarPeakFr);

        principalFreqAvgLb->Value = (*AvgPrincipalFr);
        principalFreqMinLb->Value = (*MinPrincipalFr);
        principalFreqMaxLb->Value = (*MaxPrincipalFr);
        if (*VarPrincipalFr > 0)
            principalFreqVarLb->Value = sqrt(*VarPrincipalFr);
        else
            principalFreqVarLb->Value = -999;

        pitchAvgLb->Value = (*AvgPitch);
        pitchMinLb->Value = (*MinPitch);
        pitchMaxLb->Value = (*MaxPitch);
        if (*VarPitch > 0)
            pitchVarLb->Value = sqrt(*VarPitch);
        else
            pitchVarLb->Value = -999;

        FMAvgLb->Value = (*AvgFM);
        FMminLb->Value = (*MinFM);
        FMmaxLb->Value = (*MaxFM);
        if (*VarFM > 0)
            FMVarLb->Value = sqrt(*VarFM);
        else
            FMVarLb->Value = -999;

        FMcAvgLb->Value = (*AvgFMc);
        FMcminLb->Value = (*MinFMc);
        FMcmaxLb->Value = (*MaxFMc);
        if (*VarFMc > 0)
            FMcVarLb->Value = sqrt(*VarFMc);
        else
            FMcVarLb->Value = -999;

        AMAvgLb->Value = (*AvgAM);
        AMminLb->Value = (*MinAM);
        AMmaxLb->Value = (*MaxAM);
        if (*VarAM > 0)
            AMVarLb->Value = sqrt(*VarAM);
        else
            AMVarLb->Value = -999;

        entAvgLb->Value = (*AvgEnt);
        entMinLb->Value = (*MinEnt);
        entMaxLb->Value = (*MaxEnt);
        if (*VarEnt > 0)
            entVarLb->Value = sqrt(*VarEnt);
        else
            entVarLb->Value = -999;

        derivAvgLb->Value = *AvgAmplitude;
        derivMinLb->Value = *MinAmplitude;
        derivMaxLb->Value = *MaxAmplitude;
        if (*VarAmplitude > 0)
            derivVarLb->Value = sqrt(*VarAmplitude);
        else
            derivVarLb->Value = -999;

        pgoodAvgLb->Value = (*AvgPGood);
        pgoodMinLb->Value = (*MinPGood);
        pgoodMaxLb->Value = (*MaxPGood);
        if (*VarPGood > 0)
            pgoodVarLb->Value = sqrt(*VarPGood);
        else
            pgoodVarLb->Value = -999;

        cont_tAvgLb->Value = (*AvgCont_t);
        cont_tMinLb->Value = (*MinCont_t);
        cont_tMaxLb->Value = (*MaxCont_t);
        if (*VarCont_t > 0)
            cont_tVarLb->Value = sqrt(*VarCont_t);
        else
            cont_tVarLb->Value = -999;

        cont_fAvgLb->Value = option->frequency_scale * (*AvgCont_f);
        cont_fMinLb->Value = option->frequency_scale * (*MinCont_f);
        cont_fMaxLb->Value = option->frequency_scale * (*MaxCont_f);
        if (*VarCont_f > 0)
            cont_fVarLb->Value = option->frequency_scale * sqrt(*VarCont_f);
        else
            cont_fVarLb->Value = -999;

    }
    if (sound)
        ind1 = *ind;
    else
        ind2 = *ind;
    return(true);
}

// --------------------------------------------------------------------------------------

void TForm1::calc_meanPeriod_and_meanEuclideanDist(bool song)
/* Mean  period is defined as the mean time interval between two similar rendtions of a sound
A sound is represented by the curves of features (pitch, fm...) within a time window of 70ms.
The similarity between two sounds is estimated by the Euclidean distance across feature curves.
This function first start with a randomly chosen interval, keep progressing forward in time until it
reaches an interval that has sufficiently different features, and then keep moving forward looking
for a match. That is, we first make sure we steped out of the neighborhood of some feature distribution
and only then looking for a similar sound. */

{
    int onset, offset;
    float *pitchS, *entropyS, *FMS, *goodnessS; // , *AMS;
    bool *signal;
    TNumLab *med_per, *rnd_per, *distance;

    if (song == ksong1) {
        onset = OnsetSong1;
        offset = OffsetSong1;
        pitchS = PitchS1;
        entropyS = EntropyS1;
        FMS = FMS1;
        goodnessS = PitchGoodnessS1;
        signal = Signal1;
        med_per = median_period;
        rnd_per = random_period;
        distance = meanEuclideanDist;
    }
    else {
        onset = OnsetSong2;
        offset = OffsetSong2;
        pitchS = PitchS2;
        entropyS = EntropyS2;
        FMS = FMS2;
        goodnessS = PitchGoodnessS2;
        signal = Signal2;
        med_per = median_period2;
        rnd_per = random_period2;
        distance = meanEuclideanDist2;
    }

    // 1. calc  mean Euclidean distance: how different sound windows are from each other?
    float nn = 1, dst = 0;
    for (int i = onset; i < offset; i++)
        for (int j = i + 1; j < offset; j++)
            if (signal[i] && signal[j]) {
                nn++;
                dst += pow(pitchS[i] - pitchS[j], 2) + pow
                    (entropyS[i] - entropyS[j], 2) + pow(FMS[i] - FMS[j], 2)
                    + pow(goodnessS[i] - goodnessS[j], 2);
            }
    distance->Value = dst / (nn * 4);

    if (period_estimate->Checked) {
        // 2. calc mean period of repetition (based on n=40 random starting points)
        float similarity_reject_thresh = calc_thresh(TBperiod->Position);
        float similarity_accept_thresh = calc_thresh(TBperThresh->Position);
        int ref, k = 1, loops = 0, silenceDur, silenceStart,
        interval = offset - onset;
        if (interval < 1000) {
            Application->MessageBox(
                L"Period calculation requires at least 1s of outlined sound, it is recomendeed to have at least 3s outlined to obtain a stable estimate", NULL, MB_OK);
            return;
        }
        TVector *per, *randPer;
        // per->SetVecSize(41);
        // randPer->Create(41);
        // per= new TIntVector(41);  randPer= new TIntVector(41);
        do { // do 40 times until k=40 random choise of location
            int tmp = 0, pointer, binPt;
            do {
                tmp++;
                ref = onset + random(interval / 1.5);
            }
            while (tmp < 1000 && !signal[ref]);
            // we found a reference signal. We will now move forward until we have something different:
            pointer = ref + 13; // this is the first independent window (with 12.5 frames +- per interval of 70ms judged)
            bool found = false;
            float diff;
            ThreshlodTBChange(this);
            do {
                pointer++;
                diff = 0;
                int counter = 0;
                for (int j = -25; j < 25; j++)
                    if (signal[ref + j] && signal[pointer + j]) {
                        counter++;
                        diff += pow(pitchS[ref + j] - pitchS[pointer + j], 2)
                            + pow(FMS[ref + j] - FMS[pointer + j], 2) + pow
                            (entropyS[ref + j] - entropyS[pointer + j], 2)
                            + pow(goodnessS[ref + j] - goodnessS[pointer + j],
                            2);
                    }
                if (counter > 8)
                    diff /= (counter * 4); // only obtain estimates for 8 valid comparisons
                else
                    diff = DEFAULT_MAX;
                if (diff > similarity_reject_thresh)
                    found = true; // we found a disimilar sound
            }
            while (!found && pointer < offset - 25);

            if (found == true) // that's it, we are out of the range of this sound, we can now look for the next match:
            {
                found = false;

                do { // repeat the same procedure but look for match < instead >
                    pointer++;
                    float diff = 0;
                    int counter = 0;
                    for (int j = -25; j < 25; j++) {
                        counter++;
                        diff += pow(pitchS[ref + j] - pitchS[pointer + j], 2)
                            + pow(FMS[ref + j] - FMS[pointer + j], 2) + pow
                            (entropyS[ref + j] - entropyS[pointer + j], 2)
                            + pow(goodnessS[ref + j] - goodnessS[pointer + j],
                            2);
                    }
                    if (counter > 8)
                        diff /= (counter * 4); // only obtain estimates for 8 valid comparisons
                    else
                        diff = DEFAULT_MAX;
                    if (diff < similarity_accept_thresh)
                        found = true; // note that the sign is reversed: this is a similar sound
                }
                while (!found && pointer < offset - 25);
                if (found) {
                    per->Elem[k] = pointer - ref;
                    k++;
                }
                else
                    loops++;
            } // end if found is true
            /* */
            // now do the same procedure for a random match:
            bool foundRand = false;
            int randPtr, randRef = 0, randLoops = 0;
            if (found)
                do {
                    randLoops++;
                    randPtr = onset + random(interval / 1.5);
                    randRef = randPtr;
                    do {
                        randPtr++;
                        float diff = 0;
                        int counter = 0;
                        for (int j = -25; j < 25; j++) {
                            counter++;
                            diff += pow(pitchS[ref + j] - pitchS[randPtr + j],
                                2) + pow(FMS[ref + j] - FMS[randPtr + j], 2)
                                + pow
                                (entropyS[ref + j] - entropyS[randPtr + j], 2)
                                + pow
                                (goodnessS[ref + j] - goodnessS[randPtr + j],
                                2);

                        }
                        if (counter > 8)
                            diff /= (counter * 4); // only obtain estimates for 8 valid comparisons
                        else
                            diff = DEFAULT_MAX;
                        if (diff < similarity_accept_thresh)
                            foundRand = true;
                    }
                    while
                        (!foundRand && randPtr < offset - 25 && randLoops <
                        50); // end for selected random
                }
                while (!foundRand && randLoops < 50); // found a match
            randPer->Elem[k] = randPtr - randRef;
        }
		while (k < 40 && loops < 100); // end for k

		if (loops < 98) // procedure worked fine...
		{ // the period and random period are medians
			per->SortElems(true, 1, 40);
			med_per->Value = per->Elem[20] * option->advanceTb->Value;
			randPer->SortElems(true, 1, 40);
			rnd_per->Value = randPer->Elem[20] * option->advanceTb->Value * 2;
		}
		else {
			med_per->Value = -1;
			rnd_per->Value = -1;
		}
		per->Free();
		randPer->Free();

	}
}

// ---------------------------------------------------------------------------

void TForm1::SimilarityScore() {
	score = true;
	// ScoreB->Enabled=false;
	sim_iter1 = OffsetSong1 - OnsetSong1;
	sim_iter2 = OffsetSong2 - OnsetSong2;

	calc_the_distances(); // calculate a matrix of Euclidean distances across features upon overalping intervals
	if (diagonal->Down)
		Calc_Diagonal_Score();
	else {
		Compute_Similarity_Sections(); // this function find boundaries and local scores of similarity sections
		Trim_Similarity_Sections(); // this funtion trim similarity section using hirarchial approach
		recalc_Similarity(); // now we calculate the final similarity of each section
		calc_Linearity(); // finaly, examine how similairty sections are organized sequentially
		CalculateResults();
	}
	for (int i = 0; i < sim_iter1 + margin; i++) {
		delete[]Glob[i];
		delete[]Sim[i];
	}
	delete[]Sim;
	delete[]Glob;

   // view_score->Visible = true;
	save_score->Visible = true;
	if (GUI)
		Similarity_image->Picture->Graphic = CombinedBM;
	ZoomIB->Visible = true;
	ZoomOB->Visible = true;
	SaveScore->Enabled = true;
	// ScorePannel->Visible=true;
	Beep();
}

/* This function compute Euclidean distances across overalpping intervals
We do initiation of GUI state and declartion of bitmap pointers and then
comes the nice part: we use vast overalpping between intervals, e.g., from
fft window 1 to 60 in two songs. There is lots of calculations here, not very efficient... */
void TForm1::calc_the_distances() {

	float **Local_pitch, **Local_entropy, **Local_fm, **Local_goodness,
	**Local_am;
	float **Global_pitch, **Global_entropy, **Global_fm, **Global_goodness,
	**Global_am;
	float pitchW, entropyW, FMW, AMW, goodnessW, allW; // the weights

	float pitch_dist, entropy_dist, fm_dist, pgood_dist, am_dist;
	int interval = IntervalTB->Position * 2, sc;
	margin = 1 + interval / 2;
	float similarity_threshold;
	if (liberal->Down)
		similarity_threshold = Local_Thresh;
	else
		similarity_threshold = THRESH;

	if (GUI || liberal->Down) // here we set the GUI variables
	{
		Similarity_image->Width = sim_iter1;
		Similarity_image->Height = sim_iter2;
		TRect rct; // next we set pointers
		Local_pitch = new float*[sim_iter1 + margin];
		Local_entropy = new float*[sim_iter1 + margin];
		Local_fm = new float*[sim_iter1 + margin];
		Local_goodness = new float*[sim_iter1 + margin];
		Local_am = new float*[sim_iter1 + margin];

		Global_pitch = new float*[sim_iter1 + margin];
		Global_entropy = new float*[sim_iter1 + margin];
		Global_fm = new float*[sim_iter1 + margin];
		Global_goodness = new float*[sim_iter1 + margin];
		Global_am = new float*[sim_iter1 + margin];

		for (int i = 0; i < sim_iter1 + margin; i++) {
			Local_pitch[i] = new float[sim_iter2 + margin];
			Local_entropy[i] = new float[sim_iter2 + margin];
			Local_fm[i] = new float[sim_iter2 + margin];
			Local_goodness[i] = new float[sim_iter2 + margin];
			Local_am[i] = new float[sim_iter2 + margin];

			Global_pitch[i] = new float[sim_iter2 + margin];
			Global_entropy[i] = new float[sim_iter2 + margin];
			Global_fm[i] = new float[sim_iter2 + margin];
			Global_goodness[i] = new float[sim_iter2 + margin];
			Global_am[i] = new float[sim_iter2 + margin];
		}
		for (int i = 0; i < sim_iter1 + margin; i++)
			for (int j = 0; j < sim_iter2 + margin; j++)
				Local_pitch[i][j] = Local_entropy[i][j] = Local_fm[i][j]
					= Local_am[i][j] = Local_goodness[i][j] = Global_pitch[i]
					[j] = Global_entropy[i][j] = Global_fm[i][j] = Global_am[i]
					[j] = Global_goodness[i][j] = 0;

	}
	// core function:

	// 1. set the weights for each feature:
	pitchW = option->pitchWCb->Checked * option->pitchWLb->Value;
	FMW = option->FMWCB->Checked * option->FMWLB->Value;
	AMW = option->AMWCB->Checked * option->AMWLB->Value;
	entropyW = option->entropyWCB->Checked * option->entropyWLB->Value;
	goodnessW = option->goodnessWCB->Checked * 1;
	option->goodnessWLB->Value;
	allW = pitchW + FMW + entropyW + goodnessW + AMW;

	// 2. set scorring array
	Sim = new int*[sim_iter1 + margin]; // this is the overall similarity matrix in p-value units of 0-100%
	Glob = new float*[sim_iter1 + margin]; // this is the overall similarity matrix in units of the actual scales Euclidean distance
	Local = new float*[sim_iter1 + margin];
	// this is the local similarity matrix
	for (int i = 0; i < sim_iter1 + margin; i++) {
		Sim[i] = new int[sim_iter2 + margin];
		Glob[i] = new float[sim_iter2 + margin];
		Local[i] = new float[sim_iter2 + margin];
	}
	for (int i = 0; i < sim_iter1 + margin; i++)
		for (int j = 0; j < sim_iter2 + margin; j++) {
			Sim[i][j] = Glob[i][j] = Local[i][j] = 0;
		} // just to be sure...

	// 3. calculate local distances across all combination of FFT windows, seperately for each feature
	// note that we are using scaled features values , e.g., PitchS1..., those are also smoothed by default:
	for (int song1 = 1; song1 < sim_iter1; song1++)
		for (int song2 = 1; song2 < sim_iter2; song2++) {
			pitch_dist = pow
				(PitchS1[OnsetSong1 + song1] - PitchS2[OnsetSong2 + song2], 2);
			entropy_dist = pow(EntropyS1[OnsetSong1 + song1] - EntropyS2
				[OnsetSong2 + song2], 2);
			fm_dist = pow(FMS1[OnsetSong1 + song1] - FMS2[OnsetSong2 + song2],
				2);
			pgood_dist = pow(PitchGoodnessS1[OnsetSong1 + song1]
				- PitchGoodnessS2[OnsetSong2 + song2], 2);
			am_dist = pow(AMS1[OnsetSong1 + song1] - AMS2[OnsetSong2 + song2],
				2);

			// we now add the squared feature distances.
			Local[song1][song2] =
				(pitch_dist * pitchW + entropy_dist * entropyW +
				fm_dist * FMW + pgood_dist * goodnessW + am_dist * AMW)
				/ allW;
			set_local_score(Local[song1][song2], song1, song2);
			// set p value score of Local Similarity array Sim[]
			if (GUI) // We stort the results of each feature only for GUI purposes:
			{
				Local_pitch[song1][song2] = pitch_dist;
				Local_entropy[song1][song2] = entropy_dist;
				Local_fm[song1][song2] = fm_dist;
				Local_goodness[song1][song2] = pgood_dist; // am_dist;
				Local_am[song1][song2] = am_dist;
			}

		}

	// 4. calc global distances, across larger windows. That is, we examine distances between curves of feautres:

	for (int song1 = 1; song1 < sim_iter1; song1++)
		for (int song2 = 1; song2 < sim_iter2; song2++) {
			if (GUI) {
				Global_pitch[song1][song2] = 0;
				Global_entropy[song1][song2] = 0;
				Global_fm[song1][song2] = 0;
				Global_goodness[song1][song2] = 0;
				Global_am[song1][song2] = 0;
			}

			Glob[song1][song2] = 0;
			float Index = 0;
			float global_pitchAvg1 = 0, global_pitchAvg2 = 0,
			global_entropyAvg1 = 0, global_entropyAvg2 = 0,
			global_FMAvg1 = 0, global_FMAvg2 = 0, global_goodnessAvg1 = 0,
			global_goodnessAvg2 = 0, global_am1 = 0, global_am2 = 0;

			// compute distances for one window into Glog, all the rest is GUI stuff
			for (int window = -margin; window <= margin; window++)
				if (song1 + window > 0 && song2 + window > 0
					// prevent underflow
					&& OnsetSong1 + song1 + window < Iterations1 &&
					OnsetSong2 + song2 + window < Iterations2
					// prevent overflow
					&& Signal1[OnsetSong1 + song1 + window] && Signal2
                    [OnsetSong2 + song2 + window]) // there are sounds there
                { // we only compare if there is sound in the two corresponding windows
                    Index++;

                    if (liberal->Down) // score by mean values
                    {
                        global_pitchAvg1 += PitchS1
                            [OnsetSong1 + song1 + window];
                        global_pitchAvg2 += PitchS2
                            [OnsetSong2 + song2 + window];
                        global_entropyAvg1 += EntropyS1
                            [OnsetSong1 + song1 + window];
                        global_entropyAvg2 += EntropyS2
                            [OnsetSong2 + song2 + window];
                        global_FMAvg1 += FMS1[OnsetSong1 + song1 + window];
                        global_FMAvg2 += FMS2[OnsetSong2 + song2 + window];
                        global_goodnessAvg1 += PitchGoodnessS1
                            [OnsetSong1 + song1 + window];
                        global_goodnessAvg2 += PitchGoodnessS2
                            [OnsetSong2 + song2 + window];
                        global_am1 += AMS1[OnsetSong1 + song1 + window];
                        global_am2 += AMS2[OnsetSong2 + song2 + window];
                    }

                    else // score by comparing feature time series
                    {
                        Glob[song1][song2] += Local[song1 + window]
                            [song2 + window];
                        if (GUI) // save arrays for bitmaps:
                        {
                            Global_pitch[song1][song2] += Local_pitch
                                [song1 + window][song2 + window];
                            Global_entropy[song1][song2] += Local_entropy
                                [song1 + window][song2 + window];
                            Global_fm[song1][song2] += Local_fm[song1 + window]
                                [song2 + window];
                            Global_goodness[song1][song2] += Local_goodness
                                [song1 + window][song2 + window];
                            Global_am[song1][song2] += Local_am[song1 + window]
                                [song2 + window]; // already scaled!
                        }
                    }

                } // end for

            if (Index < 8)
                Glob[song1][song2] = 0; // NOT ENOUGH DATA TO JUDGE DISTANCE...
            else if (Signal1[OnsetSong1 + song1] && Signal2[OnsetSong2 + song2]
                ) {
                if (liberal->Down) {
                    Global_pitch[song1][song2] = pow
                        ((global_pitchAvg1 - global_pitchAvg2) / Index, 2);
                    Global_entropy[song1][song2] = pow
                        ((global_entropyAvg1 - global_entropyAvg2) / Index, 2);
                    Global_fm[song1][song2] = pow
                        ((global_FMAvg1 - global_FMAvg2) / Index,
                        2);
                    Global_goodness[song1][song2] = pow
                        ((global_goodnessAvg1 - global_goodnessAvg2) / Index,
                        2);
                    Global_am[song1][song2] = pow
                        ((global_am1 - global_am2) / Index, 2);

                    Glob[song1][song2] = Global_pitch[song1][song2]
                        * pitchW + Global_entropy[song1][song2]
                        * entropyW + Global_fm[song1][song2]
                        * FMW + Global_goodness[song1][song2]
                        * goodnessW + Global_am[song1][song2] * AMW;

                    Glob[song1][song2] /= allW;

                }
                else
                    Glob[song1][song2] /= max(Index, (float)1);
                // now we have the mean distance
                // Memo2->Lines->Add(Glob[song1][song2]);
                if (diagonal->Down)
                    Glob[song1][song2] = glob_score(Glob[song1][song2]);
                // turn glob into global p value estimate
                else if (Glob[song1][song2] > similarity_threshold)
                    Glob[song1][song2] = 0; // here we impose a 5% cutoff threshold
                else if (liberal->Down)
                    Glob[song1][song2] = max(255 - (300 * Glob[song1][song2]),
                    (float)0);
                else
                    Glob[song1][song2] = max(255 - (60 * Glob[song1][song2]),
                    (float)0);

                // this is for GUI display, not important...
                if (GUI && !(liberal->Down)) {
                    Global_pitch[song1][song2] /= (Index - 1.0);
                    Global_entropy[song1][song2] /= (Index - 1.0);
                    Global_fm[song1][song2] /= (Index - 1.0);
                    Global_goodness[song1][song2] /= (Index - 1.0);
                    Global_am[song1][song2] /= (Index - 1.0);
                    // Memo2->Lines->Add(string);
                }
            }
            else {
                Glob[song1][song2] = 0;
                Sim[song1][song2] = 0;
            }
        }

    if (save_matrix->Checked) {
        TStringList* simi = new TStringList();
        for (int song1 = 1; song1 < sim_iter1; song1++) {
            str = "";
            for (int song2 = 1; song2 < sim_iter2; song2++) {
                str += Glob[song1][song2];
                str += ",";
            }
            simi->Add(str);
        }
        SaveDialog1->Execute();
        simi->SaveToFile(SaveDialog1->FileName);
    }

    if (GUI) {

        // we now calculate the partial Euclidean distances (averages and minimum per collumn)
        float mPitch_local, mFM_local, mEntropy_local, mGoodness_local,
        mAM_local, mPitch_global, mFM_global, mEntropy_global,
        mGoodness_global, mAM_global, xPitch_local, xFM_local, xEntropy_local,
        xGoodness_local, xAM_local, xPitch_global, xFM_global, xEntropy_global,
        xGoodness_global, xAM_global, mnPitch_local, mnFM_local,
        mnEntropy_local, mnGoodness_local, mnAM_local,
        mnPitch_global, mnFM_global, mnEntropy_global, mnGoodness_global,
        mnAM_global;
        int overallCount = 0;
        mPitch_local = mFM_local = mEntropy_local = mGoodness_local =
            mAM_local = mPitch_global = mFM_global = mEntropy_global =
            mGoodness_global = mAM_global = 0;
        mnPitch_local = mnFM_local = mnEntropy_local = mnGoodness_local =
            mnAM_local = mnPitch_global = mnFM_global = mnEntropy_global =
            mnGoodness_global = mnAM_global = 0;
        xPitch_local = xFM_local = xEntropy_local = xGoodness_local =
            xAM_local = xPitch_global = xFM_global = xEntropy_global =
            xGoodness_global = xAM_global = 9999;

        for (int song1 = 1; song1 < sim_iter1; song1++) {
            for (int song2 = 1; song2 < sim_iter2; song2++) {
                overallCount++;
                mPitch_local += Local_pitch[song1][song2];
                if (xPitch_local > Local_pitch[song1][song2])
                    xPitch_local = Local_pitch[song1][song2];
                mFM_local += Local_fm[song1][song2];
                if (xFM_local > Local_fm[song1][song2])
                    xFM_local = Local_fm[song1][song2];
                mEntropy_local += Local_entropy[song1][song2];
                if (xEntropy_local > Local_entropy[song1][song2])
                    xEntropy_local = Local_entropy[song1][song2];
                mGoodness_local += Local_goodness[song1][song2];
                if (xGoodness_local > Local_goodness[song1][song2])
                    xGoodness_local = Local_goodness[song1][song2];
                mAM_local += Local_am[song1][song2];
                if (xAM_local > Local_am[song1][song2])
                    xAM_local = Local_am[song1][song2];
                mPitch_global += Global_pitch[song1][song2];
                if (xPitch_global > Global_pitch[song1][song2])
                    xPitch_global = Global_pitch[song1][song2];
                mFM_global += Global_fm[song1][song2];
                if (xFM_global > Global_fm[song1][song2])
                    xFM_global = Global_fm[song1][song2];
                mEntropy_global += Global_entropy[song1][song2];
                if (xEntropy_global > Global_entropy[song1][song2])
                    xEntropy_global = Global_entropy[song1][song2];
                mGoodness_global += Global_goodness[song1][song2];
                if (xGoodness_global > Global_goodness[song1][song2])
                    xGoodness_global = Global_goodness[song1][song2];
                mAM_global += Global_am[song1][song2];
                if (xAM_global > Global_am[song1][song2])
                    xAM_global = Global_am[song1][song2];
            }
            mnPitch_local += xPitch_local;
            mnFM_local += xFM_local;
            mnEntropy_local += xEntropy_local;
            mnGoodness_local += xGoodness_local;
            mnAM_local += xAM_local;
            mnPitch_global += xPitch_global;
            mnFM_global += xFM_global;
            mnEntropy_global += xEntropy_global;
            mnGoodness_global += xGoodness_global;
			mnAM_global += xAM_global;
			xPitch_local = xFM_local = xEntropy_local = xGoodness_local =
				xAM_local = xPitch_global = xFM_global = xEntropy_global =
				xGoodness_global = xAM_global = 9999;
		}

		TReportListView *sim_view;
		int sim_cursor;
		if(display->ActivePage==Similarity){sim_view=similarity_view; sim_cursor=similarity_cursor; }
		else { sim_view=similarity_batch_view; sim_cursor=similarity_batch_cursor; }

		mPitch_local /= overallCount;
		str= mPitch_local;
		str=str.SubString(1,5);
		sim_view->Elem[6][sim_cursor] = str;

		mFM_local /= overallCount;
		str =  mFM_local;
		str=str.SubString(1,5);
		sim_view->Elem[7][sim_cursor] = str;

		mEntropy_local /= overallCount;
		str = mEntropy_local;
		str=str.SubString(1,5);
		sim_view->Elem[8][sim_cursor] = str;

		mGoodness_local /= overallCount;
		str = mGoodness_local;
		str=str.SubString(1,5);
		sim_view->Elem[9][sim_cursor] = str;

		mAM_local /= overallCount;
		str = mAM_local;
		str=str.SubString(1,5);
		sim_view->Elem[10][sim_cursor] = str;

		// global distances (currently not reported)
		mPitch_global /= overallCount;
		mFM_global /= overallCount;
		mEntropy_global /= overallCount;
		mGoodness_global /= overallCount;
		mAM_global /= overallCount;

		//"mean(min column distances)", currently not reported
		mnPitch_local /= sim_iter1;
		mnFM_local /= sim_iter1;
		mnEntropy_local /= sim_iter1;
		mnGoodness_local /= sim_iter1;
		mnAM_local /= sim_iter1;

		// "long-scale values:", currently not reported
		mnPitch_global /= sim_iter1;
		mnFM_global /= sim_iter1;
		mnEntropy_global /= sim_iter1;
		mnGoodness_global /= sim_iter1;
		mnAM_global /= sim_iter1;

		short cl;
		set_all_bitmaps();
		for (int song2 = 1; song2 < sim_iter2; song2++) {
			scan_the_lines(song2);
			for (int song1 = 1; song1 < sim_iter1; song1++) {
				bool snd = Signal1[OnsetSong1 + song1] * Signal2
					[OnsetSong2 + song2];
				int cl = Glob[song1][song2];
				GlobalMeasuresPtr[song1] = RGB(cl, cl, cl);
				AllMeasuresPtr[song1] = RGB(cl, cl, cl);
				cl = 2.55 * Sim[song1][song2];
				if (Signal1[OnsetSong1 + song1] && Signal2[OnsetSong2 + song2])
					LocalMeasuresPtr[song1] = RGB(cl, cl, cl);
				else
					LocalMeasuresPtr[song1] = RGB(0, 0, 0); // clBlack;

				cl = 255 - min(sim_contrast->Position * Global_pitch[song1]
					[song2], (float)255);
				GlobalPitchPtr[song1] = RGB(snd * cl, snd * cl, snd * cl);
				cl = 255 - min
					(sim_contrast->Position * Global_fm[song1][song2],
					(float)255);
				GlobalSlopePtr[song1] = RGB(snd * cl, snd * cl, snd * cl);
				cl = 255 - min(sim_contrast->Position * Global_entropy[song1]
					[song2], (float)255);
				GlobalEntropyPtr[song1] = RGB(snd * cl, snd * cl, snd * cl);
				cl = 255 - min(sim_contrast->Position * Global_goodness[song1]
					[song2], (float)255);
				GlobalContPtr[song1] = RGB(snd * cl, snd * cl, snd * cl);
				cl = 255 - min
					(sim_contrast->Position * Global_am[song1][song2],
					(float)255);
				GlobalAMPtr[song1] = RGB(snd * cl, snd * cl, snd * cl);

				cl = 255 - min(2 * sim_contrast->Position * Local_pitch[song1]
					[song2], (float)255);
				LocalPitchPtr[song1] = RGB(snd * cl, snd * cl, snd * cl);
				cl = 255 - min(2 * sim_contrast->Position * Local_entropy[song1]
					[song2], (float)255);
				LocalEntropyPtr[song1] = RGB(snd * cl, snd * cl, snd * cl);
				cl = 255 - min(2 * sim_contrast->Position * Local_fm[song1]
					[song2], (float)255);
				LocalSlopePtr[song1] = RGB(snd * cl, snd * cl, snd * cl);
				cl = 255 - min
					(2 * sim_contrast->Position * Local_goodness[song1]
					[song2], (float)255);
				LocalContPtr[song1] = RGB(snd * cl, snd * cl, snd * cl);
				cl = 255 - min(2 * sim_contrast->Position * Local_am[song1]
					[song2], (float)255);
				LocalAMPtr[song1] = RGB(snd * cl, snd * cl, snd * cl);
			}
		}
		for (int i = 0; i < sim_iter1 + margin; i++) {
			delete[]Local_pitch[i];
			delete[]Local_entropy[i];
			delete[]Local_fm[i];
			delete[]Local_goodness[i];
			delete[]Local_am[i];
			delete[]Global_pitch[i];
			delete[]Global_entropy[i];
			delete[]Global_fm[i];
			delete[]Global_goodness[i];
			delete[]Global_am[i];
		}
		delete[]Local_pitch;
		delete[]Local_entropy;
		delete[]Local_fm;
		delete[]Local_goodness;
		delete[]Local_am;
		delete[]Global_pitch;
		delete[]Global_entropy;
		delete[]Global_fm;
		delete[]Global_goodness;
		delete[]Global_am;
	} // end if GUI

	for (int i = 0; i < sim_iter1 + margin; i++)
		delete[]Local[i];
	delete[]Local;

	ScoreB->Caption = "Reset";
	ScoreB->Enabled = true;
}

// ---------------------------------------------------------------------------

void TForm1::Calc_Diagonal_Score() {
	float index = 1;
	float score = 0, glob_score;
	TColor color;
	int window = SectionTab->Value, max_score, max_glob_score, winwin = 0;
	int songdur = min(sim_iter1, sim_iter2);
	for (int song1 = 1; song1 < songdur; song1++) {
		max_score = 0;
        max_glob_score = 0;
        for (int i = -window; i < window; i++)
            if (song1 + i > 0 // prevent underflow
                && OnsetSong1 + song1 < Iterations1 && OnsetSong2 + song1 + i <
                Iterations2 // prevent overflow
                && OnsetSong2 + song1 + i > 0 // prevent overflow
                && Signal1[OnsetSong1 + song1] && Signal2
                [OnsetSong2 + song1 + i]) // there are sounds there

            {
                if (Sim[song1][song1 + i] > max_score) {
                    max_score = Sim[song1][song1 + i];
                    winwin = i;
                }
                if (Glob[song1][song1 + i] > max_glob_score)
                    max_glob_score = Glob[song1][song1 + i];
            }

        if (Signal1[OnsetSong1 + song1] && Signal2[OnsetSong2 + song1 + winwin]
            ) {
            if (max_score > 100 || max_score < 0)
                max_score = 0;
            if (max_glob_score > 100 || max_glob_score < 0)
                max_glob_score = 0;
            score += max_score;
            glob_score += max_glob_score;
            if (GUI) {
                color = ColorMap(max_score);
                CombinedBM->Canvas->Brush->Color = color; // this will transfer the range to the functions range 0-256
                CombinedBM->Canvas->FillRect(Rect(song1 - 1,
                        song1 + winwin - 1, song1 + 2, song1 + winwin + 2));
            }
            index++;
        }
    }
    score /= index;
    glob_score /= index;

    similarityN->Value = glob_score;
    accuracyN->Value = score;
    float dur1 = OffsetSong1 - OnsetSong1;
    float dur2 = OffsetSong2 - OnsetSong2;
    if (!dur1)
        dur1 = 1;
    sequentialN->Value = 100 * min(dur1, dur2) / max(dur1, dur2);
	scoreN->Value = glob_score * score * sequentialN->Value / 10000.0;

}

// ---------------------------------------------------------------------------

void TForm1::Compute_Similarity_Sections() {

    // Problem: you need to optimize Borders size and have it sensitive to outlining boundaries of the song
    // and so you need to set a routine for onset and offset correspond to and  Glob position adjustment.
    char str[80];
    // area = new TMatrix(6,500);
    // area->NrOfRows=6;
    // area->NrOfColumns=500;
    area->Fill(0); // this is the matrix of similarity sections, 500 sections is a constraint
    SignalToScore = 0;
    for (int i = OnsetSong1; i <= OffsetSong1; i++)
        if (Signal1[i])
            SignalToScore++; // singal vs silence

    // 1.	declare a Boolean matrix of all border points (section boundaries).
    Borders = new bool*[sim_iter1]; // 50+dataCounter1/Slide];
    for (int j = 0; j < sim_iter1; j++) // 50+dataCounter1/Slide; j++)
        Borders[j] = new bool[sim_iter2]; // 50+dataCounter2/Slide];
    // define boundaries of borders as zero:
    for (int i = 0; i < sim_iter1; i++)
        for (int j = 0; j < 4; j++) {
            Glob[i][j] = 0;
        } // the first 3 frames are defined as disimilar
    for (int i = 0; i < sim_iter2; i++)
        for (int j = 0; j < 4; j++) {
            Glob[j][i] = 0;
        } // the first 3 frames are defined as disimilar

    for (int i = 0; i < sim_iter1; i++)
        for (int j = 0; j < sim_iter2; j++)
            Borders[i][j] = false;

    // 2.	Declare an array (vector called section?) for temporary storage of of border group
    Section = new int*[sim_iter1 * sim_iter2]; // a similarity section is unlikely to be bigger...
    for (int j = 0; j < sim_iter1 * sim_iter2; j++)
        Section[j] = new int[2];

    // 3. find border points
    if (GUI)
        GlobalBM->Canvas->Brush->Color = clAqua;
    for (int i = 1; i < sim_iter1 - 1; i++)
        for (int j = 1; j < sim_iter2 - 1; j++)
            if (!Glob[i][j] && (Glob[i + 1][j] || Glob[i][j + 1] || Glob[i - 1]
                    [j] || Glob[i][j - 1])) {
                Borders[i][j] = true;
                int sX = i;
                int sY = j;
                if (GUI)
                    GlobalBM->Canvas->FillRect(Rect(sX - 1, sY - 1, sX, sY));
            }
            else
                Borders[i][j] = false;

    // 4. find neighborhoolds of border points: by adding to Section and find neighbor for each point...
    bool BorderExist = true;
    do {
        int x = 0, y; // x and y are the cursor
        bool flag = true; // we now look thoughtout the matrix to the first border point
        for (int i = 1; i < sim_iter1 - 1 && flag; i++) // find the first border point in the matrix Border(i j) and put the cursor there
            for (int j = 1; j < sim_iter2 - 1; j++)
                if (Borders[i][j]) {
                    x = i;
                    y = j;
                    flag = false;
                    break;
                }
        if (!x) {
            BorderExist = false;
            break;
        } // We did not detect any border points, since x did not change from 0 -- break out
        else
            Borders[x][y] = false; // after detection, we erase this border point from Borders(i j) but the cursor is still there
        // we will next place our first member (founder) in the Section(1) matrix
        int index = 0;
        Section[index][0] = x;
        Section[index][1] = y; // we are now generating a matrix Section, that includes all border points of a section
        bool NeighborExist = true; // we filled it with the first memeber (the cursor) and we will now look for neigbors for each one
        // of those members. Namely, each time we add a member, we will include him in any judgement of membership test, as shown below
        do {
            bool flag = false;
            for (int i = 0; i <= index; i++) // for every point in the matrix find a neigbour and add to matrix
            { // Note that index is the number of members in a section, so we now compare our candidate to all of them
                // index increases dynamically inside the loop, each time we find a new member. i is the location of this new member

                x = Section[i][0];
                y = Section[i][1]; // place the cursor on the current member.
                if (x < 1 || y < 1 || x > sim_iter1 || y > sim_iter2)
                    break;

                // we now look for neighbors around our member, and if found one -- add him to the list. Note that this loop
                // keeps rolling as lonng as there is any neighbor to any one of our members. We keep eliminating members
                // from Borders, but keep them in Section and use them as refferences -- until we cannot continue any longer
                if (Borders[x - 1][y - 1]) {
                    Borders[x - 1][y - 1] = false;
                    index++;
                    Section[index][0] = x - 1;
                    Section[index][1] = y - 1;
                    flag = true;
                    break;
                }
                if (Borders[x][y - 1]) {
                    Borders[x][y - 1] = false;
                    index++;
                    Section[index][0] = x;
                    Section[index][1] = y - 1;
                    flag = true;
                    break;
                }
                if (Borders[x + 1][y - 1]) {
                    Borders[x + 1][y - 1] = false;
                    index++;
                    Section[index][0] = x + 1;
                    Section[index][1] = y - 1;
                    flag = true;
                    break;
                }
                if (Borders[x - 1][y]) {
                    Borders[x - 1][y] = false;
                    index++;
                    Section[index][0] = x - 1;
                    Section[index][1] = y;
                    flag = true;
                    break;
                }
                if (Borders[x + 1][y]) {
                    Borders[x + 1][y] = false;
                    index++;
                    Section[index][0] = x + 1;
                    Section[index][1] = y;
                    flag = true;
                    break;
                }
                if (Borders[x - 1][y + 1]) {
                    Borders[x - 1][y + 1] = false;
                    index++;
                    Section[index][0] = x - 1;
                    Section[index][1] = y + 1;
                    flag = true;
                    break;
                }
                if (Borders[x][y + 1]) {
                    Borders[x][y + 1] = false;
                    index++;
                    Section[index][0] = x;
                    Section[index][1] = y + 1;
                    flag = true;
                    break;
                }
                if (Borders[x + 1][y + 1]) {
                    Borders[x + 1][y + 1] = false;
                    index++;
                    Section[index][0] = x + 1;
                    Section[index][1] = y + 1;
                    flag = true;
                    break;
                }
            }
            if (!flag)
                NeighborExist = false;
        }
        while (NeighborExist); // This ends the process for one similarity section

        // we next calculate features of this similairty section:

        // 1. Find the top-left and bottom-right extremes of the section
        int BeginX = DEFAULT_MAX, BeginY = DEFAULT_MAX, EndX = 0, EndY = 0;
        for (int i = 0; i < index; i++) {
            if (BeginX > Section[i][0])
                BeginX = Section[i][0];
            if (EndX < Section[i][0])
                EndX = Section[i][0];
            if (BeginY > Section[i][1])
                BeginY = Section[i][1];
            if (EndY < Section[i][1])
                EndY = Section[i][1];
        }

        // 2. calculate raw score:
        int durationX = EndX - BeginX, durationY = EndY - BeginY,
        valid_comparisons;
        if (BeginX < DEFAULT_MAX && durationX && durationY) {
            float slope;
            int end_position_X, end_position_Y, sX, sY, eX, eY;
            int offsetX, offsetY, advanceX, advanceY, scoreN, bestScoreN = 0;
            int delta = option->TimeWarpLB->Value * durationX / 2;
            // this is the spil we have, in terms of section frames
            if (!delta)
                delta = 1;
            // We will look for the best 45% diagonal by searching for best scores, first by offseting the starting point
            // of the tutor song and then by offsetting the starting point of the pupil song.
            // Offseting the tutor song:
            for (offsetX = 0; offsetX < durationX / 2; offsetX++)
            // we offset X and Y as much as 50% of the duration of
                for (offsetY = 0; offsetY < durationY / 2; offsetY++)
                // the similarity section
                    for (int range = -delta; range <= delta; range++)
                    // for each offset combo (of X and Y), we try variaty of slopes,
                    { // as much as time-warp tolerance allow
                        valid_comparisons = 0;
                        scoreN = 0;
                        slope = (float)(durationX - (2 * range)) / (float)
                            (durationX);
                        // and here is the calculation for each of the competing 'sections':
                        for (advanceX = 0; advanceX < durationX; advanceX++)
                        // for every frame in tutors section
                        {
                            advanceY = advanceX * slope;
                            if
                                (BeginX + offsetX + advanceX <= EndX &&
                                BeginY + offsetY + advanceY <= EndY) {
                                scoreN = scoreN + Sim
                                    [BeginX + offsetX + advanceX]
                                    [BeginY + offsetY + advanceY];
                                valid_comparisons++;
                                end_position_X = advanceX;
                                end_position_Y = advanceY;
                            }
                        }
                        if (scoreN > bestScoreN) {
                            bestScoreN = scoreN;
                            /* area->Elem[Xs][n]=sX=BeginX+offsetX;
                            area->Elem[Ys][n]=sY=BeginY+offsetY;
                            area->Elem[Xe][n]=eX=area->Elem[Xs][n]+end_position_X;
                            area->Elem[Ye][n]=eY=area->Elem[Ys][n]+end_position_Y;
                             */
                            sX = BeginX + offsetX;
                            sY = BeginY + offsetY;
                            eX = sX + end_position_X;
                            eY = sY + end_position_Y;

                            area->Elem[Xs][n] = sX;
                            area->Elem[Ys][n] = sY;
                            area->Elem[Xe][n] = eX;
                            area->Elem[Ye][n] = eY;
                            area->Elem[Val][n] = scoreN;
                            area->Elem[ValidComp][n] = valid_comparisons;
                            // comparisons;
                        }
                    }
            if (GUI) {
                // string=area->Elem[Xs][n]; string+=";"; string+=area->Elem[Ys][n]; string+=";"; string+=area->Elem[Val][n]; Memo1->Lines->Add(string);
                GlobalBM->Canvas->Brush->Color = clBlue;
                GlobalBM->Canvas->FrameRect(Rect(sX, sY, eX, eY));
            }

            if (bestScoreN < 100 * SimilaritySection->Position)
            // insuficient similarity, area rejected
            {
                area->Elem[Val][n] = 0;
                area->Elem[ValidComp][n] = 0;
                if (GUI) // low similarity rejection is shown by a green X
                {
                    GlobalBM->Canvas->Pen->Color = clGreen;
                    GlobalBM->Canvas->MoveTo(sX, sY);
                    GlobalBM->Canvas->LineTo(eX, eY); // first diagonal
                    GlobalBM->Canvas->MoveTo(eX, sY);
                    GlobalBM->Canvas->LineTo(sX, eY); // second diagonal
                }
            }
            n++;
        } // end if(BeginX<100000)

    }
    while (BorderExist); // this ends the computation for all similarity sections

    // 5. free the memory!!
    for (int i = 0; i < sim_iter1; i++)
        delete[]Borders[i];
    delete[]Borders;

    for (int i = 0; i < sim_iter1 * sim_iter2; i++)
        delete[]Section[i];
    delete[]Section;

}

//////////////////////////////////////////////////////////////////////////////

void TForm1::Trim_Similarity_Sections() {
    float old_duration, new_duration;
    // now trim weeker similarities
    for (int j = 1; j < n; j++) {
        area->SortRows(5, false, 1, 1, 6, n); // sort Area's 1...n from large to small values
        // Memo1->Lines->Add("===============================");
        // for(int d=1;d<=n;d++)Memo1->Lines->Add(area->Elem[Val][d]);

        area->Elem[Val][1] = area->Elem[Val][1]*-1; // this will ensure we will not consider it again
        float XsInf_ref, XeInf_ref, YsInf_ref, YeInf_ref;
        // those are just refferences for GUI
        for (int i = 2; i <= n; i++)
            if (area->Elem[Val][i] > 0) {
                float XsSup = area->Elem[Xs][1];
                float XeSup = area->Elem[Xe][1];
                float YsSup = area->Elem[Ys][1];
                float YeSup = area->Elem[Ye][1];

                float XsInf = XsInf_ref = area->Elem[Xs][i];
                float XeInf = XeInf_ref = area->Elem[Xe][i];
                float YsInf = YsInf_ref = area->Elem[Ys][i];
                float YeInf = YeInf_ref = area->Elem[Ye][i];

                if (XeInf > XsSup && XsInf < XeSup)
                // there is an overlap on the X
                {
                    if ((XsInf < XsSup && XeInf > XeSup) ||
                        (XsInf > XsSup && XeInf < XeSup)) {
                        // kill this section
                        area->Elem[Val][i] = 0;
                        if (GUI) {
                            GlobalBM->Canvas->Pen->Color = clWhite;
                            GlobalBM->Canvas->MoveTo(XsInf_ref, YsInf_ref);
                            GlobalBM->Canvas->LineTo(XeInf_ref, YeInf_ref);
                            // first diagonal
                            GlobalBM->Canvas->MoveTo(XeInf_ref, YsInf_ref);
                            GlobalBM->Canvas->LineTo(XsInf_ref, YeInf_ref);
                            // second diagonal
                        }
                    }

                    else if (XsInf > XsSup) {
                        area->Elem[Xs][i] = XeSup;
                        area->Elem[Ys][i] = YsInf +
                            ((YeInf - YsInf) * (XeSup - XsInf) /
                            (XeInf - XsInf)); // correct for Y
                        old_duration = XeInf - XsInf;
                        XsInf = XeSup;
                        YsInf = area->Elem[Ys][i];
                        new_duration = XeInf - XsInf;
                        area->Elem[Val][i] = area->Elem[Val][i] *
                            (new_duration / old_duration);
                    }

                    else {
                        area->Elem[Xe][i] = XsSup;
                        area->Elem[Ye][i] = YsInf +
                            ((YeInf - YsInf) * (XsSup - XsInf) /
                            (XeInf - XsInf)); // correct for Y
                        old_duration = XeInf - XsInf;
                        XeInf = XsSup;
                        YeInf = area->Elem[Ye][i];
                        new_duration = XeInf - XsInf;
                        area->Elem[Val][i] = area->Elem[Val][i] *
                            (new_duration / old_duration);
                    }
                }
                // if not val==0 do the same for the Y axis and correct for all X's

                if (area->Elem[Val][i] && YeInf > YsSup && YsInf < YeSup)
                // there is an overlap on the Y
                {
                    if ((YsInf < YsSup && YeInf > YeSup) ||
                        (YsInf > YsSup && YeInf < YeSup)) {
                        // kill this section
                        area->Elem[Val][i] = 0;
                        if (GUI) {
                            GlobalBM->Canvas->Pen->Color = clWhite;
                            GlobalBM->Canvas->MoveTo(XsInf_ref, YsInf_ref);
                            GlobalBM->Canvas->LineTo(XeInf_ref, YeInf_ref);
                            // first diagonal
                            GlobalBM->Canvas->MoveTo(XeInf_ref, YsInf_ref);
                            GlobalBM->Canvas->LineTo(XsInf_ref, YeInf_ref);
                            // second diagonal
                        }
                    }
                    else if (YsInf > YsSup) {
                        old_duration = area->Elem[Ye][i] - area->Elem[Ys][i];
                        area->Elem[Ys][i] = YeSup;
                        new_duration = area->Elem[Ye][i] - area->Elem[Ys][i];
                        area->Elem[Xs][i] = XsInf +
                            ((XeInf - XsInf) * (YeSup - YsInf) /
                            (YeInf - YsInf)); // correct for X
                        area->Elem[Val][i] = area->Elem[Val][i] *
                            (new_duration / old_duration);
                    }
                    else {
                        old_duration = area->Elem[Ye][i] - area->Elem[Ys][i];
                        area->Elem[Ye][i] = YsSup;
                        new_duration = area->Elem[Ye][i] - area->Elem[Ys][i];
                        area->Elem[Xe][i] = XsInf +
                            ((XeInf - XsInf) * (YsSup - YsInf) /
                            (YeInf - YsInf)); // correct for X
                        area->Elem[Val][i] = area->Elem[Val][i] *
                            (new_duration / old_duration);
                    }
                }
                // eliminate areas that are less than 5ms long
                if (area->Elem[Xe][i] - area->Elem[Xs][i] < 3 || area->Elem[Ye]
                    [i] - area->Elem[Ys][i] < 3) { // kill this section
                    area->Elem[Val][i] = 0;
                    if (GUI) {
                        GlobalBM->Canvas->Pen->Color = clWhite;
                        GlobalBM->Canvas->MoveTo(XsInf_ref, YsInf_ref);
                        GlobalBM->Canvas->LineTo(XeInf_ref, YeInf_ref);
                        // first diagonal
                        GlobalBM->Canvas->MoveTo(XeInf_ref, YsInf_ref);
                        GlobalBM->Canvas->LineTo(XsInf_ref, YeInf_ref);
                        // second diagonal
                    }
                }
            } // if>0

    }
    for (int i = 0; i <= n; i++)
        area->Elem[Val][i] = area->Elem[Val][i]*-1;
    // this get the score sign positive again
    area->SortRows(5, false, 1, 1, 6, n);
    // Memo1->Lines->Add("++++++++++++++++++++++++++++++++++++");

}

void TForm1::recalc_Similarity() {
    if (GUI) {
        CombinedBM->Canvas->Font->Color = clRed;
        CombinedBM->Canvas->Font->Size = 10;
    }

    TutorImitatedEntropy = 0;
    PupilImitatedEntropy = 0;
    TutorImitatedFM = 0;
    PupilImitatedFM = 0;
    TutorImitatedPitch = 0;
    PupilImitatedPitch = 0;
    TutorImitatedCont = 0;
    PupilImitatedCont = 0;

    char str[80];
    scrNum = 0;
    scrVal = 0;

    int Xpoint, Ypoint;
    TColor color;
    for (int i = 0; i < n; i++)
        if (area->Elem[Val][i]) // for all similarity sections
        { // this procedure record the raw & final similarity values...
            if (GUI) {
                GlobalBM->Canvas->Brush->Color = clRed;
                GlobalBM->Canvas->FrameRect(Rect(area->Elem[Xs][i],
                        area->Elem[Ys][i], area->Elem[Xe][i],
                        area->Elem[Ye][i]));
            }

            int value = 0, validComparisons = 0;
            int BeginX = area->Elem[Xs][i], EndX = area->Elem[Xe][i],
            BeginY = area->Elem[Ys][i], EndY = area->Elem[Ye][i];
            float Xi = EndX - BeginX, Yi = EndY - BeginY,
            slope2 = Yi / max(Xi, (float)1);

            for (int Xj = BeginX; Xj <= EndX; Xj++) {
                int the_score, BestScore = 0; // here we use an approach of finding best scores first, and slope next...
                for (int Yj = BeginY; Yj <= EndY; Yj++) {
                    the_score = Glob[Xj][Yj];
                    if (the_score > BestScore)
                        BestScore = the_score;
                }
                int y_place = BeginY + slope2 * (Xj - BeginX);
                BestScore = Sim[Xj][y_place];
                if (BestScore) {
                    validComparisons++;
                    value += BestScore;
                    TutorImitatedEntropy += Entropy1[Xj];
                    PupilImitatedEntropy += Entropy2[y_place];
                    TutorImitatedPitch += Pitch1[Xj];
                    PupilImitatedPitch += Pitch2[y_place];
                    TutorImitatedFM += FM1[Xj];
                    PupilImitatedFM += FM2[y_place];
                    TutorImitatedCont += PitchGoodness1[Xj];
                    PupilImitatedCont += PitchGoodness2[y_place];
                    if (GUI)
                        color = ColorMap(BestScore);
                }

                else if (GUI)
                    color = clWhite; // no sound!
                if (GUI) {
                    CombinedBM->Canvas->Brush->Color = color; // this will transfer the range to the functions range 0-256
                    CombinedBM->Canvas->FillRect(Rect(Xj - 2, y_place - 2,
                            Xj + 2, y_place + 2));
                }

            } // end for Xj

            area->Elem[ValidComp][i] = validComparisons;
            area->Elem[Val][i] = value;
			if (area->Elem[Val][i] < 100 * SimilaritySection->Position) {
				area->Elem[Val][i] = 0;
				if (GUI) // indicate final stage low similarity rejection by a yellow X
				{
					GlobalBM->Canvas->Pen->Color = clYellow;
					GlobalBM->Canvas->MoveTo(area->Elem[Xs][i],
						area->Elem[Ys][i]);
					GlobalBM->Canvas->LineTo(area->Elem[Xe][i],
						area->Elem[Ye][i]); // first diagonal
					GlobalBM->Canvas->MoveTo(area->Elem[Xe][i],
						area->Elem[Ys][i]);
					GlobalBM->Canvas->LineTo(area->Elem[Xs][i],
						area->Elem[Ye][i]); // second diagonal
					// same for the overall bitmap:
					CombinedBM->Canvas->Pen->Color = clYellow;
					CombinedBM->Canvas->MoveTo(area->Elem[Xs][i],
						area->Elem[Ys][i]);
					CombinedBM->Canvas->LineTo(area->Elem[Xe][i],
						area->Elem[Ye][i]); // first diagonal
					CombinedBM->Canvas->MoveTo(area->Elem[Xe][i],
						area->Elem[Ys][i]);
					CombinedBM->Canvas->LineTo(area->Elem[Xs][i],
						area->Elem[Ye][i]); // second diagonal
				}
			}
			scrVal = scrVal + area->Elem[Val][i];
			scrNum = scrNum + area->Elem[ValidComp][i];

		} // end of for, if(area->Elem[Val][i])
	int signum = 0;
	for (int i = 0; i < sim_iter1; i++)
		if (Signal1[OnsetSong1 + i])
			signum++;
	int sectionDur, overallSimilarityDur = 0;
	double overallAccuracy = 0;
	if (GUI) {
		Image3->Canvas->Brush->Color = clWhite;
		Image3->Canvas->FillRect(Rect(0, 130, Image3->Width, 155));
		Image4->Canvas->Brush->Color = clWhite;
		Image4->Canvas->FillRect(Rect(130, 0, 145, Image4->Height));
	}
	DataForm->similarity->Last();
	int Comparisons = 0;
	int recnum = DataForm->similarity->FieldByName("recnum")->AsInteger;
	recnum++;
	for (int i = 1; i <= n; i++) {
		AnsiString str = "";
		if (area->Elem[Val][i]) {
			sectionDur = area->Elem[Xe][i] - area->Elem[Xs][i] - 1;
			if (!sectionDur)
				sectionDur = 1;
			overallSimilarityDur += sectionDur + 1;
			overallAccuracy += area->Elem[Val][i];
			Comparisons += area->Elem[ValidComp][i];
			str = (int)(area->Elem[Val][i] / area->Elem[ValidComp][i]);
			str += "%";
			Image3->Canvas->TextOut(area->Elem[Xs][i] + 3, 140, str);

            if (save_sections->Checked && similarity_matrix_exists) {
                float vll = area->Elem[Val][i];
                float comps = area->Elem[ValidComp][i];
                float acur = vll / comps;
                DataForm->Insert_similarity_section(recnum, area->Elem[Xs][i],
                    area->Elem[Xe][i], area->Elem[Ys][i], area->Elem[Ye][i],
                    acur);
            }
        }
    }

    float nm = scrNum;
    if (!nm)
        nm = 1;
    TutorImitatedEntropy /= nm;
    PupilImitatedEntropy /= nm;
    TutorImitatedFM /= nm;
    PupilImitatedFM /= nm;
    TutorImitatedPitch /= nm / 44.1;
    PupilImitatedPitch /= nm / 44.1;
    TutorImitatedCont /= nm;
    PupilImitatedCont /= nm;
    if (GUI) {
        GlobalBM->Canvas->Pen->Color = clRed;
        Image3->Canvas->Pen->Width = 2;
        Image4->Canvas->Pen->Width = 2;
        CombinedBM->Canvas->Pen->Width = 2;
    }

    if (GUI) {
        int simV, accurV;
        for (int i = n - 1; i >= 0; i--) {
            simV = area->Elem[Val][i];

            if (simV) {
                color = clYellow;
                CombinedBM->Canvas->Brush->Style = bsClear;
                LocPitchBM->Canvas->Brush->Style = bsClear;
                GlobPitchBM->Canvas->Brush->Style = bsClear;
                LocModBM->Canvas->Brush->Style = bsClear;
                GlobModBM->Canvas->Brush->Style = bsClear;
                GlobEntBM->Canvas->Brush->Style = bsClear;
                LocEntBM->Canvas->Brush->Style = bsClear;
                LocContBM->Canvas->Brush->Style = bsClear;
                GlobContBM->Canvas->Brush->Style = bsClear;
                LocAMBM->Canvas->Brush->Style = bsClear;
                GlobAMBM->Canvas->Brush->Style = bsClear;

                CombinedBM->Canvas->Pen->Color = color;
                LocPitchBM->Canvas->Pen->Color = color;
                GlobPitchBM->Canvas->Pen->Color = color;
                LocModBM->Canvas->Pen->Color = color;
                GlobModBM->Canvas->Pen->Color = color;
                GlobEntBM->Canvas->Pen->Color = color;
                LocEntBM->Canvas->Pen->Color = color;
                LocContBM->Canvas->Pen->Color = color;
                GlobContBM->Canvas->Pen->Color = color;
                LocAMBM->Canvas->Pen->Color = color;
                GlobAMBM->Canvas->Pen->Color = color;

                TPoint points[7];
                points[0] = Point((int)area->Elem[Xs][i], 0);
                points[1] = Point((int)area->Elem[Xs][i],
                    (int)area->Elem[Ys][i]);
                points[2] = Point(0, (int)area->Elem[Ys][i]);
                points[3] = Point(0, (int)area->Elem[Ye][i]);
                points[4] = Point((int)area->Elem[Xe][i],
                    (int)area->Elem[Ye][i]);
                points[5] = Point((int)area->Elem[Xe][i], 0);
                points[6] = Point((int)area->Elem[Xs][i], 0);

                CombinedBM->Canvas->Polygon(points, 6);
                LocPitchBM->Canvas->Polygon(points, 6);
                GlobPitchBM->Canvas->Polygon(points, 6);
                LocModBM->Canvas->Polygon(points, 6);
                GlobModBM->Canvas->Polygon(points, 6);
                GlobEntBM->Canvas->Polygon(points, 6);
                LocEntBM->Canvas->Polygon(points, 6);
                LocContBM->Canvas->Polygon(points, 6);
                GlobContBM->Canvas->Polygon(points, 6);
                LocAMBM->Canvas->Polygon(points, 6);
                GlobAMBM->Canvas->Polygon(points, 6);

                Image3->Canvas->Pen->Color = color;
                Image4->Canvas->Pen->Color = color;
                Image3->Canvas->MoveTo((int)area->Elem[Xs][i], 0);
                Image3->Canvas->LineTo((int)area->Elem[Xs][i], 240);
                Image3->Canvas->MoveTo((int)area->Elem[Xe][i], 0);
                Image3->Canvas->LineTo((int)area->Elem[Xe][i], 240);
                Image4->Canvas->MoveTo(0, (int)area->Elem[Ys][i]);
                Image4->Canvas->LineTo(240, (int)area->Elem[Ys][i]);
                Image4->Canvas->MoveTo(0, (int)area->Elem[Ye][i]);
                Image4->Canvas->LineTo(240, (int)area->Elem[Ye][i]);

            }
        }
    } // end if GUI
    similarityN->Value = (int)(100.0 * (float)overallSimilarityDur / (float)max
        (signum, 1));
    if (similarityN->Value)
        accuracyN->Value = overallAccuracy / (float)max(Comparisons, 1);
    else
        accuracyN->Value = -1;
}

//////////////////////////////////////////////////////////////////////////////////////////

void TForm1::calc_Linearity() {
    int i = 1;
    area->SortRows(Xs, true, 1, 1, 5, n); // sort the similarity sections by the X axis
    for (; ; ) {
        if (area->Elem[Val][i] || i == n)
            break;
        else
            i++;
    }
    int first = i, second; // we found the first section with non-zero score
    i++;
    nonLinear = 0;
    do {
        for (; ; ) {
            if (area->Elem[Val][i] || i >= n)
                break;
            else
                i++;
        }
        second = i; // we found the next non-zero section

        if (area->Elem[Val][first] && area->Elem[Val][second]
            // just to make sure...
            && (area->Elem[Ys][first] > 5 + area->Elem[Ys][second]
                // the first start after the second on Y axis (sorted by X)
                || fabs(area->Elem[Xe][first] - area->Elem[Xs][second])
                > match_tolerance_level->Value // astop within a song is unlikely to be more than 80 frames (100ms)
                || fabs(area->Elem[Ye][first] - area->Elem[Ys][second])
                > match_tolerance_level->Value))
            nonLinear += min(area->Elem[Xe][first] - area->Elem[Xs][first],
            area->Elem[Xe][second] - area->Elem[Xs][second]);
        // the smaller of the two areas is considered out of order
        first = second; // recursion
        i++;
    }
    while (i < n);

    nonLinear *= 2; // to change the bound to allow more than 50% non-linearity
    nonLinear /= OffsetSong1 - OnsetSong1; // turn to ratio
    nonLinear = 100 * (1 - nonLinear); // this is the precentage of non-linearity
    if (similarityN->Value)
        sequentialN->Value = nonLinear;
    else
        sequentialN->Value = -1;
    float Fscore;
    if (similarityCB->Checked)
        Fscore = similarityN->Value;
    else
        Fscore = 100;
    if (accuracyCB->Checked)
        Fscore *= accuracyN->Value / 100; // else Fscore*=100;
    if (sequentialCB->Checked)
        Fscore = Fscore * (sequentialN->Value / 100.0) + 0.5 * Fscore *
            (1.0 - (sequentialN->Value / 100.0));

    // Fscore*=sequentialN->Value; else Fscore*=100;
    scoreN->Value = Fscore;
}

// ****************************************************************************************************************************

void TForm1::CalculateResults() {
    /*
    char str[20];
    int duration1=(float)(sim_iter1+1)*(float)(Slide/44.1);
    int duration2=(float)(sim_iter2+1)*(float)(Slide/44.1);
    int signal1=duration1-((float)silence1*(float)(Slide/44.1));
    int signal2=duration2-(float)(silence2*(float)(Slide/44.1));
    float Imit=100*(float)scrNum/(float)SignalToScore;//(OffsetSong1-OnsetSong1-silence1);
    float Accur=(float)(scrVal)/float(max(scrNum,1));
    if(!similarityCB->Checked)Imit=100;
	if(!accuracyCB->Checked)Accur=100;
	if(!sequentialCB->Checked)nonLinear=100; */
}

// ****************************************************************************************************************************

void __fastcall TForm1::Image2MouseMove(TObject *Sender, TShiftState Shift,
	int X, int Y) {
	bool *show_adv, *pagination_On, *show_Back;
	int *currentStartLoc;
	int *pageNum;
	TShape* dragCursor;
	TImage *image;
	TPageControl *features_disp;
	TNumLab *advance_fft;
	TScrollBox *scroll;
	int mouse_x;
	draged = false;

	// shift from selecting points to selecting intervals by dragging the mouse
	if (Sender == Image1) {
		image=Image1;
		scroll=ScrollBox3;
		pagination_On = &option->paginationOn;
		dragCursor = drag_cursor;
		features_disp = features_display;
		mouse_x = mouse_down_X;
	}
	else {
		dragCursor = drag_cursor2;
		image=Image2;
		scroll=ScrollBox4;
		pagination_On = &option->paginationOn2;
		features_disp = features_display2;
		mouse_x = mouse_down_X2;
	}
	if (Shift.Contains(ssLeft) && ((Sender == Image1 && state1 == 1) ||
			(Sender == Image2 && state2 == 1)) && X > mouse_x + 1) {
		// && state2==1
		state2;
		if(!(*pagination_On)) dragCursor->Left = X + 37 - scroll->HorzScrollBar->Position;
		else dragCursor->Left = X + 37;//43;
		dragCursor->Height=image->Height;
		dragCursor->Visible = true;
		draged = true;
		if (features_disp->TabIndex<2)
			features_disp->TabIndex = 2;
	}

	if (ready)
		try {
		TNumLab *time, *freq, *ent, *mod, *am, *pf, *pitch, *amp, *Pgood,
		*Principal_frN, *continuity_tN, *continuity_fN, *slopeN;
		TButton *prev, *advanceDisp, *goBackDisp;
		float *Pitch, *Entropy, *FM, *FMc, *AM, *Amplitude, *PeakFreq,
		*Pgoodness, *principal_freq, *cont_t, *cont_f;
		int itr;
		if (Sender == Image1 && stretch->ItemIndex) {
			show_adv = &showAdvance;
			pageNum = &option->pageNumber;
			currentStartLoc = &option->currentStartLocation;
			pagination_On = &option->paginationOn;
			show_Back = &showBack;
			prev = Preview;
			advance_fft=advanceTb;
			advanceDisp = advanceDisplay;
			goBackDisp = goBackDisplay;
			time = timeL;
			freq = frequencyL;
			ent = entropyL;
			mod = modulationL;
			pf = PeakFreqL;
			am = AML;
			pitch = pitchL;
			amp = AmplitudeL;
			Pgood = PgoodL;
			slopeN = slopeN1;
			Principal_frN = Principal_frN1;
			continuity_tN = continuity_tN1;
			continuity_fN = continuity_fN1;
			itr = Iterations1;
			Pitch = Pitch1;
			Entropy = Entropy1;
			FM = FM1;
			FMc = FMc1;
			AM = AM1;
            Amplitude = Amplitude1;
            PeakFreq = PeakFreq1;
            Pgoodness = PitchGoodness1;
            principal_freq = principal_frequency1;
            cont_t = continuity_t1;
            cont_f = continuity_f1;
        }
        else if (stretch2->ItemIndex) {
            show_adv = &showAdvance2;
            pageNum = &option->pageNumber2;
            show_Back = &showBack2;
            currentStartLoc = &option->currentStartLocation2;
            pagination_On = &option->paginationOn2;
            prev = Preview2;
			advance_fft=advanceTb2;
			advanceDisp = advanceDisplay2;
			goBackDisp = goBackDisplay2;
			time = time2L;
			freq = frequency2L;
			ent = entropy2L;
			mod = modulation2L;
			pf = PeakFreq2L;
			am = AM2L;
			pitch = pitch2L;
			amp = Amplitude2L;
			Pgood = PgoodL2;
			slopeN = slopeN2;
			Principal_frN = Principal_frN2;
			continuity_tN = continuity_tN2;
			continuity_fN = continuity_fN2;
			itr = Iterations2;
			Pitch = Pitch2;
			Entropy = Entropy2;
			FM = FM2;
			FMc = FMc2;
			AM = AM2;
			Amplitude = Amplitude2;
			PeakFreq = PeakFreq2;
			Pgoodness = PitchGoodness2;
			principal_freq = principal_frequency2;
			cont_t = continuity_t2;
			cont_f = continuity_f2;
		}
		else
			return;

		// need to generalize this part for sound two as well
		if (X < 50 && (*show_Back) && (*pagination_On) && (*currentStartLoc)
			> 1000)
			goBackDisp->Visible = true;
		else
			goBackDisp->Visible = false;
		if (X > 950 && (*show_adv) && (*pagination_On))
			advanceDisp->Visible = true;
		else
			advanceDisp->Visible = false;



		// end part to generalize
		if (*pagination_On) {
		   time->Value = X * advance_fft->Value + ((float) * currentStartLoc - 1000.0) * advance_fft->Value;
		   prev->Visible = true;
		}
		else  {
			time->Value = X * advance_fft->Value;
			prev->Visible = false;
		}

		// Slide*rangeIndex*750/DigiRate;
		freq->Value = option->sampling_rate * (250 - Y) / 1024;

		// (rangeIndex*1000);
		if (option->range_x2)
			freq->Value *= 2;
		else if (option->range_x05)
			freq->Value /= 2;
		if (X > 5 && X < itr - 5) {
			ent->Value = Entropy[X];
			mod->Value = FM[X];
			am->Value = AM[X];
			pf->Value = PeakFreq[X];
			///(1000*rangeIndex);
			pitch->Value = Pitch[X]; // DigiRate*Pitch[X]/(1000*rangeIndex);
			amp->Value = Amplitude[X];
			Pgood->Value = Pgoodness[X]; // log10(Pgoodness[X]+1)*10-20;//
			Principal_frN->Value = principal_freq[X] * option->frequency_scale;
			continuity_tN->Value = cont_t[X];
			continuity_fN->Value = option->frequency_scale * cont_f[X];
			if (FMc[X] > -999)
				slopeN->Value = FMc[X]; // option->frequency_scale*(principal_freq[X+3]-principal_freq[X-3]);
		}
		}
	catch(Exception & exception) {
		Crash();
	}
}

// ---------------------------------------------------------------------------

void __fastcall TForm1::Image1MouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y) {
	static int start, end;
	if (!ready)
		return;
	POINT cursorPos;
	if (Button == mbRight &&
		(display->ActivePage == Song1 || display->ActivePage == Song2)) {
		GetCursorPos(&cursorPos);
		if (state1 == 2)
			update_popup(0);
		else
			update_popup(1);
		save_features->Popup(cursorPos.x, 150);
		return;
	}
	mouse_down_X = X;

	// Do features at pointer
	if (features_display->TabIndex==0 && display->ActivePage==Song1) {

		switch(deriv_display->ItemIndex)
		{
		  case 0: m_Calculator1.GetImage().DrawAllSlices(Image1); break;
		  case 1: m_Calculator1.GetContours().DrawAllSlices(Image1); break;
		  case 2: m_Calculator1.GetSonogram().DrawAllSlices(Image1); break;
		}
		Image1->Canvas->Pen->Width = 1;
		Image1->Canvas->Pen->Color = clRed;
		Image1->Canvas->MoveTo(X, 0);
		Image1->Canvas->LineTo(X, 250);
		save_features_at_pointerClick(this);
		state1 = 1;
		onset_1 = OnsetSong1 = X;
		return;
	}

	/* {
	if (display->ActivePage == Song1 || display->ActivePage == Song2)
	return;
	else {
	draw_features->Popup(cursorPos.x, 150);
	} // in case of segmented comparisons?
	} */
	if (stretch->ItemIndex == fit_screen) {
		MessageDlgPos(
			"Sorry, you cannot outline sounds in fit-screen mode. Please change the 'draw interval' mode to unscaled ", mtInformation, TMsgDlgButtons() << mbOK, 0, 140, 162);
		return;
	}

	Image1->Canvas->Pen->Width = 1;

	switch(state1) {
	case 0: // KSpage->Visible=false;
		interval1 = false;
		if (CalcMode->ItemIndex == auto_outline) {
			do {
				X--;
			}
			while (X > 2 && Signal1[X]);
			if (X > 4)
				X -= 3;
		}
		onset_1 = OnsetSong1 = X;
		Image1->Canvas->Pen->Color = clRed;
		start = X;
		Image1->Canvas->MoveTo(X, 0);
		Image1->Canvas->LineTo(X, 250);
		state1 = 1;
		if(option->paginationOn)song_play->StartPos = (option->currentStartLocation+OnsetSong1-1000)* advanceTb->Value;
		else song_play->StartPos = OnsetSong1 * advanceTb->Value;
		if (CalcMode->ItemIndex != auto_outline)
			break;
	case 1:

		// if user reversed beginning and end, reset and get out of there:
		if(CalcMode->ItemIndex != auto_outline && X<OnsetSong1+1){
			drag_cursor->Visible = false;
			state1 = 0;
			interval1 = false;
			switch(deriv_display->ItemIndex)
			{
			  case 0: m_Calculator1.GetImage().DrawAllSlices(Image1); break;
			  case 1: m_Calculator1.GetContours().DrawAllSlices(Image1); break;
			  case 2: m_Calculator1.GetSonogram().DrawAllSlices(Image1); break;
			}
			set_scale(0);
			OnsetSong1 = 1;
			OffsetSong1 = Iterations1 - 1;
			return;
		}



		update_score_sounds = true;
		if (CalcMode->ItemIndex == auto_outline) {
			X += 4;
			do {
				X++;
			}
			while (X < Iterations1 && Signal1[X]);
			if (X < Iterations1 + 4)
				X += 3;
		}
		offset_1 = OffsetSong1 = X;
		Image1->Canvas->Pen->Color = clBlue;
		end = X;
		Image1->Canvas->MoveTo(X, 0);
		Image1->Canvas->LineTo(X, 250);
		state1 = 2;
		sound = 0;
		done = CalculateIntervalMeasures(start, end);
		if(option->paginationOn)song_play->EndPos = (option->currentStartLocation+OffsetSong1-1000)* advanceTb->Value;
		else song_play->EndPos = OffsetSong1 * advanceTb->Value;
		//song_play->EndPos = OffsetSong1 * advanceTb->Value;
		if (!done) {
			start = 0;
			break;
		}
	  //	if (CalcMode->ItemIndex == auto_segment) save_syllables(ksong1);
		interval1 = true;
		//ks_panel->Visible = false;
		GetCursorPos(&cursorPos);
		if (display->ActivePage == Song1 || display->ActivePage == Song2) {
			// update_popup(0);
			// save_features->Popup(cursorPos.x, 150);
		}
		else if (display->ActivePage == Features_space) {
			Image8->Canvas->Pen->Color = clBlue;
			Image8->Canvas->MoveTo(X, 0);
			Image8->Canvas->LineTo(X, 240);
			update_popup(0);
			draw_features->Popup(cursorPos.x, 150);

		}
		// if(CalcMode->ItemIndex) done=CalculateNoteMeasures(start,end);
		break;
	case 2: // KSpage->Visible=false;
		drag_cursor->Visible = false;
		state1 = 0;
		if (done) {
			KSpitch1->Free();
			KSModul1->Free();
			KSentropy1->Free();
			KScont1->Free();
		}
		interval1 = false;
	   // option->amplitude_thresh = (125 - derivTB->Position);
		option->entropy_thresh = (float)(-entropyTb->Position) / 50.0;
		switch(deriv_display->ItemIndex)
		{
		  case 0: m_Calculator1.GetImage().DrawAllSlices(Image1); break;
		  case 1: m_Calculator1.GetContours().DrawAllSlices(Image1); break;
		  case 2: m_Calculator1.GetSonogram().DrawAllSlices(Image1); break;
		}
		set_scale(0);
		OnsetSong1 = 1;
		OffsetSong1 = Iterations1 - 1;
	}
	Image6->AutoSize=true;
	Image6->Picture = Image1->Picture;
	Image6->AutoSize=false;
	Image6->Stretch=true;
	Image6->Height=175;
	Image8->Picture = Image1->Picture;
}

// ---------------------------------------------------------------------------

void __fastcall TForm1::Image2MouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y) {
	static int start, end, done;
	POINT cursorPos;
	if (!ready2)
		return;
	if (Button == mbRight)
		return;
	if (stretch2->ItemIndex == fit_screen) {
		MessageDlgPos(
			"Sorry, you cannot outline sounds in fit-screen mode. Please change the 'draw interval' mode to unscaled ", mtInformation, TMsgDlgButtons() << mbOK, 0, 140, 162);
		return;
	}
	mouse_down_X2 = X;

	if (features_display2->TabIndex==0 && display->ActivePage==Song2) {

		switch(deriv_display2->ItemIndex)
		{
		  case 0: m_Calculator2.GetImage().DrawAllSlices(Image2); break;
		  case 1: m_Calculator2.GetContours().DrawAllSlices(Image2); break;
		  case 2: m_Calculator2.GetSonogram().DrawAllSlices(Image2); break;
		}
		set_scale(1);
		Image2->Canvas->Pen->Width = 1;
		Image2->Canvas->Pen->Color = clRed;
		Image2->Canvas->MoveTo(X, 0);
		Image2->Canvas->LineTo(X, 250);
		save_features_at_pointerClick(this);
		state2 = 1;
		OnsetSong2 = X;
		return;
	}

	Image2->Canvas->Pen->Width = 1;
	switch(state2) {
	case 0: // KSpage->Visible=false;
		interval2 = false;
		if (CalcMode->ItemIndex == auto_outline) {
			do {
				X--;
			}
			while (X > 2 && Signal2[X]);
			if (X > 4)
				X -= 3;
		}
		OnsetSong2 = X;
		Image2->Canvas->Pen->Color = clRed;
		start = X;
		Image2->Canvas->MoveTo(X, 0);
		Image2->Canvas->LineTo(X, 240);
		state2++;
		play_song2->StartPos = OnsetSong2 * advanceTb2->Value;
		if (CalcMode->ItemIndex != auto_outline)
			break;
	case 1:

		// if user reversed beginning and end, reset and get out of there:
		if(CalcMode->ItemIndex != auto_outline && X<OnsetSong2+1){
			drag_cursor2->Visible = false;
			state2 = 0;
			interval2 = false;
			switch(deriv_display2->ItemIndex)
			{
			  case 0: m_Calculator2.GetImage().DrawAllSlices(Image2); break;
			  case 1: m_Calculator2.GetContours().DrawAllSlices(Image2); break;
			  case 2: m_Calculator2.GetSonogram().DrawAllSlices(Image2); break;
			}
			set_scale(1);
			OnsetSong2 = 1;
			OffsetSong2 = Iterations2 - 1;
			return;
		}


	   //	if(X<OnsetSong2+2)return;
		// if(interval2==true)KSpage->Visible=true;
		update_score_sounds = true;
		if (CalcMode->ItemIndex == auto_outline) {
			X += 4;
			do {
				X++;
			}
			while (X < Iterations2 && Signal2[X]);
			if (X < Iterations2 + 4)
				X += 3;
		}
		OffsetSong2 = X;
		Image2->Canvas->Pen->Color = clBlue;
		end = X;
		Image2->Canvas->MoveTo(X, 0);
		Image2->Canvas->LineTo(X, 240);
		state2++;
		sound = 1;
		done = CalculateIntervalMeasures(start, end);
		play_song2->EndPos = OffsetSong2 * advanceTb2->Value;
		if (!done) {
			start = 0;
			break;
		}
		interval2 = true;
		if (CalcMode2->ItemIndex == auto_segment)
			save_syllables(ksong2);
		if (calc_KS->Checked) {
			float syll_distance = pow(durationL->Value - duration2L->Value, 2)
				/ pow(option->deviationDuration->Value, 2) + pow
				(pitchAvgL->Value - pitch2AvgL->Value, 2) / pow
				(option->deviationPitch->Value, 2) + pow
				(pitchVarL->Value - pitch2VarL->Value, 2) / pow
				(option->deviationVarPitch->Value, 2) + pow
				(FMAvgL->Value - FM2AvgL->Value, 2) / pow
				(option->deviationFM->Value, 2) + pow
				(FMVarL->Value - FM2VarL->Value, 2) / pow
				(option->deviationVarFM->Value, 2) + pow
				(entAvgL->Value - ent2AvgL->Value, 2) / pow
				(option->deviationEntropy->Value, 2) + pow
				(entVarL->Value - ent2VarL->Value, 2) / pow
				(option->deviationVarEnt->Value, 2) + pow
				(pgoodAvgL->Value - pgood2AvgL->Value, 2) / pow
				(option->deviationPgood->Value, 2) + pow
				(pgoodVarL->Value - pgood2VarL->Value, 2) / pow
				(option->deviationVarPGood->Value, 2) + pow
				(peakFreqAvgL->Value - peakFreq2AvgL->Value, 2) / pow
				(option->deviationPeakFr->Value, 2) + pow
				(peakFreqVarL->Value - peakFreq2VarL->Value, 2) / pow
				(option->deviationVarPeakFr->Value, 2);
			syll_distance /= 11;
			syll_Score->Value = syll_score(syll_distance);

			ks_pitch->Value = KSs(Pitch1, Pitch2, OnsetSong1, OffsetSong1,
				OnsetSong2, OffsetSong2);
			ks_entropy->Value = KSs(Entropy1, Entropy2, OnsetSong1,
				OffsetSong1, OnsetSong2, OffsetSong2);
			ks_FM->Value = KSs(FM1, FM2, OnsetSong1, OffsetSong1, OnsetSong2,
				OffsetSong2);
			ks_Amplitude->Value = KSs(Amplitude1, Amplitude2, OnsetSong1,
				OffsetSong1, OnsetSong2, OffsetSong2);
			ks_peak->Value = KSs(PeakFreq1, PeakFreq2, OnsetSong1, OffsetSong1,
				OnsetSong2, OffsetSong2);
			ks_goodness->Value = KSs(PitchGoodness1, PitchGoodness2,
				OnsetSong1, OffsetSong1, OnsetSong2, OffsetSong2);
			ks_all->Value =
				(ks_pitch->Value + ks_entropy->Value + ks_FM->Value +
				ks_Amplitude->Value + ks_peak->Value + ks_goodness->Value)
				/ 6.0;

		// this move the values to the ReportView of segmented comparisons:
		str = songname->Caption;
		comparison_view->Elem[1][comparison_cursor] = str;
		str = SongName2a->Caption;
		comparison_view->Elem[2][comparison_cursor] = str;
		str=syll_Score->Value;
		str=str.SubString(1,5);
		comparison_view->Elem[3][comparison_cursor] = str;
		str=ks_all->Value;
		str=str.SubString(1,5);
		comparison_view->Elem[4][comparison_cursor] = str;
		str=ks_Amplitude->Value;
		str=str.SubString(1,5);
		comparison_view->Elem[5][comparison_cursor] = str;
		str=ks_pitch->Value;
		str=str.SubString(1,5);
		comparison_view->Elem[6][comparison_cursor] = str;
		str=ks_FM->Value;
		str=str.SubString(1,5);
		comparison_view->Elem[7][comparison_cursor] = str;
		str=ks_entropy->Value;
		str=str.SubString(1,5);
		comparison_view->Elem[8][comparison_cursor] = str;
		str=ks_goodness->Value;
		str=str.SubString(1,5);
		comparison_view->Elem[9][comparison_cursor] = str;
		str=ks_peak->Value;
		str=str.SubString(1,5);
		comparison_view->Elem[10][comparison_cursor] = str;
		str=OnsetSong1*option->Advance;
		str=str.SubString(1,5);
		comparison_view->Elem[11][comparison_cursor] = str;
		str=OffsetSong1*option->Advance;
		str=str.SubString(1,5);
		comparison_view->Elem[12][comparison_cursor] = str;
		str=OnsetSong2*option->Advance;
		str=str.SubString(1,5);
		comparison_view->Elem[13][comparison_cursor] = str;
		str=OffsetSong2*option->Advance;
		str=str.SubString(1,5);
		comparison_view->Elem[14][comparison_cursor] = str;
		str=comparison_comments->Text;
		comparison_view->Elem[15][comparison_cursor] = str;
		comparison_cursor++;
		comparison_view->AddRow();


		}
		if (align->Checked)
			Image7->Left = Image6->Left + OnsetSong1 - OnsetSong2;
		GetCursorPos(&cursorPos);
		if (display->ActivePage == Song1 || display->ActivePage == Song2) {
			// update_popup(0);
			// save_features->Popup(cursorPos.x, 150);
		}
        else if (display->ActivePage == Features_space) {
            Image8->Canvas->Pen->Color = clBlue;
            Image8->Canvas->MoveTo(X, 0);
            Image8->Canvas->LineTo(X, 240);
            draw_features->Popup(cursorPos.x, 150);
        }

        // if(CalcMode->ItemIndex) done=CalculateNoteMeasures(start,end);
        break;
    case 2: // KSpage->Visible=false;
        drag_cursor2->Visible = false;
        state2 = 0;
        if (done) {
            KSpitch2->Free();
            KSModul2->Free();
            KSentropy2->Free();
            KScont2->Free();
        }
        interval2 = false;
	   // option->amplitude_thresh = (125 - derivTB2->Position);
        option->entropy_thresh = (float)(-entropyTb2->Position) / 50.0;
		switch(deriv_display2->ItemIndex)
		{
		  case 0: m_Calculator2.GetImage().DrawAllSlices(Image2); break;
		  case 1: m_Calculator2.GetContours().DrawAllSlices(Image2); break;
		  case 2: m_Calculator2.GetSonogram().DrawAllSlices(Image2); break;
		}
        set_scale(1);
		OnsetSong2 = 1;
		OffsetSong2 = Iterations2 - 1;
    }

	Image7->AutoSize=true;
	Image7->Picture = Image2->Picture;
	Image7->AutoSize=false;
	Image7->Stretch=true;
	Image7->Height=175;
	Image8->Picture = Image2->Picture;
}

// ---------------------------------------------------------------------------

void __fastcall TForm1::MouseActionClick(TObject *Sender) {
    switch(MouseAction->ItemIndex) {
    case 0:
        feature_space->MouseAction = maRotate;
        break;
    case 1:
        feature_space->MouseAction = (TMouseActMode)4;
        break; // should be 4...
    case 2:
        feature_space->MouseAction = maZoom;
        break;
    }
}

// ---------------------------------------------------------------------------

TColor TForm1::ColorMap(int score) {
    TColor color;
    if (score > 95)
        color = clRed;
    else if (score > 85)
        color = clYellow;
    else if (score > 75)
        color = clLime;
    else if (score > 65)
        color = clGreen;
    else if (score >= 50)
        color = clOlive;
    else if (score >= 35)
        color = clBlue;
    else
        color = clGray;
    return(color);
}

TColor TForm1::ColorMap2(int Col) {
    TColor cl;
    if (Col == -1)
        cl = clWhite;
    else if (Col < 100)
        cl = (TColor)RGB(0, 0, 0); // clBlack;
    else if (Col < 170)
        cl = clBlue;
    else if (Col < 230)
        cl = clYellow;
    else
        cl = clRed;
    return(cl);
}

TColor TForm1::ColorCode(float val, float AvgEng) {
    int gray, red, yellow;
    TColor color;
    gray = (int)(120 * val / log(max(exp(1) + AvgEng - 20, 1.1)));
    if (gray > 255)
        gray = 255;
    red = (int)(95 * val / log(max((float(exp(1) + AvgEng - 20)), (float)1.1)));
    if (red > 255)
        red = 255;
    yellow = (int)(110 * val / log(max(exp(1) + AvgEng - 20, 1.1)));
    if (yellow > 255)
        yellow = 255;
    if (red > 160)
        color = (TColor)(RGB(0, 0, red));
    else if (yellow > 130)
        color = (TColor)(RGB(0, yellow, yellow));
    else
        color = (TColor)(RGB(gray, gray, gray));
    return(color);
}

////////////////////////////////////////////////////////////////////////////////////////////

void __fastcall TForm1::ScoreBClick(TObject *Sender) {
	DataForm->similarity->Active = true;
	Continuous = false;
	if (ScoreB->Caption == "Reset") {
		// ScorePannel->Visible=false;
		Image3->Width -= Zoom;
		Image4->Height -= Zoom;
		Similarity_image->Width -= Zoom;
		Similarity_image->Height -= Zoom;
		Zoom = 0;
		// ScorePannel->Visible=false;
		ScoreB->Caption = "Score";
		if (score)
			DeleteScoreData();
		return;
	}
	similarity_matrix_exists = true;
	Screen->Cursor = crHourGlass;
	SimilarityScore(); // this function is the entire procedure...
	//if (autosave_similarity->Checked) save_scoreClick(this);

	// ScorePannel->Visible=true;
	// area->Free();
	similarity_matrix_exists = false;
	Screen->Cursor = crDefault;
	//similarity_control->SelectNextPage(1);

	TReportListView *sim_view;
	int sim_cursor;
	if(display->ActivePage==Similarity){ sim_view=similarity_view;  sim_cursor=similarity_cursor; }
	else {sim_view=similarity_batch_view; sim_cursor=similarity_batch_cursor; }

	str = songname->Caption;
	sim_view->Elem[1][sim_cursor] = str;
	str = SongName2a->Caption;
	sim_view->Elem[2][sim_cursor] = str;
	str = similarityN->Value;
	str=str.SubString(1,5);
	sim_view->Elem[3][sim_cursor] = str;
	str = accuracyN->Value;
	str=str.SubString(1,5);
	sim_view->Elem[4][sim_cursor] = str;
	str = sequentialN->Value;
	str=str.SubString(1,5);
	sim_view->Elem[5][sim_cursor] = str;
	str=OnsetSong1*option->Advance;
	str=str.SubString(1,5);
	sim_view->Elem[11][sim_cursor] = str;
	str=OffsetSong1*option->Advance;
	str=str.SubString(1,5);
	sim_view->Elem[12][sim_cursor] = str;
	str=OnsetSong2*option->Advance;
	str=str.SubString(1,5);
	sim_view->Elem[13][sim_cursor] = str;
	str=OffsetSong2*option->Advance;
	str=str.SubString(1,5);
	sim_view->Elem[14][sim_cursor] = str;
	str=similarity_comments->Text;
	sim_view->Elem[15][sim_cursor] = str;
	sim_view->AddRow();
	if(display->ActivePage==Similarity)similarity_cursor++;
	else similarity_batch_cursor++;
}

// ---------------------------------------------------------------------------

void __fastcall TForm1::GlobEntrScoreBClick(TObject *Sender) {
	Similarity_image->Picture->Graphic = GlobEntBM;
}

// ---------------------------------------------------------------------------

void __fastcall TForm1::GlobContScoreBClick(TObject *Sender) {
	Similarity_image->Picture->Graphic = GlobContBM;
}

// ---------------------------------------------------------------------------

void __fastcall TForm1::GlobModuScoreBClick(TObject *Sender) {
	Similarity_image->Picture->Graphic = GlobModBM;
}

// ---------------------------------------------------------------------------

void __fastcall TForm1::GlobPitchScoreBClick(TObject *Sender) {
    Similarity_image->Picture->Graphic = GlobPitchBM;
}

// ---------------------------------------------------------------------------

void __fastcall TForm1::LocalPitchScoreBClick(TObject *Sender) {
    Similarity_image->Picture->Graphic = LocPitchBM;
}

// ---------------------------------------------------------------------------

void __fastcall TForm1::LocalModulScoreBClick(TObject *Sender) {
    Similarity_image->Picture->Graphic = LocModBM;
}

// ---------------------------------------------------------------------------

void __fastcall TForm1::LocalEntrScoreBClick(TObject *Sender) {
    Similarity_image->Picture->Graphic = LocEntBM;
}

// ---------------------------------------------------------------------------

void __fastcall TForm1::LocalContScoreBClick(TObject *Sender) {
    Similarity_image->Picture->Graphic = LocContBM;
}

// ---------------------------------------------------------------------------

void __fastcall TForm1::OverallScoreBClick(TObject *Sender) {
    Similarity_image->Picture->Graphic = CombinedBM;
}

// ---------------------------------------------------------------------------

void __fastcall TForm1::GlobalScoreBClick(TObject *Sender) {
    Similarity_image->Picture->Graphic = GlobalBM;
}

// ---------------------------------------------------------------------------

void __fastcall TForm1::LocalScoreBClick(TObject *Sender) {
    Similarity_image->Picture->Graphic = LocAllBM;
}

// ---------------------------------------------------------------------------

void __fastcall TForm1::ZoomOBClick(TObject *Sender) // zoom out
{
    Zoom -= 10;
    Image3->AutoSize = false;
    Image3->Stretch = true;
    Image4->AutoSize = false;
    Image4->Stretch = true;
    Similarity_image->AutoSize = false;
    Similarity_image->Stretch = true;
    Image3->Width -= 10;
    Image4->Height -= 10;
    Similarity_image->Width -= 10;
    Similarity_image->Height -= 10;
}

// ---------------------------------------------------------------------------

void __fastcall TForm1::ZoomIBClick(TObject *Sender) {
    Zoom += 10;
    Image3->AutoSize = false;
    Image3->Stretch = true;
    Image4->AutoSize = false;
    Image4->Stretch = true;
    Similarity_image->AutoSize = false;
    Similarity_image->Stretch = true;
    Image3->Width += 10;
    Image4->Height += 10;
    Similarity_image->Width += 10;
    Similarity_image->Height += 10;
}

// ---------------------------------------------------------------------------

void __fastcall TForm1::SaveScoreClick(TObject *Sender) {
   // SaveGraph->InitialDir = "C:\\Sound Analysis";
    // SaveGraph->DefaultExt = "BMP";
    Beep();
    SaveGraph->Title = "choose a name for similarity matrix (+song1, +song2)";
    if (SaveGraph->Execute())
        Image3->Picture->SaveToFile(SaveGraph->FileName);
    Similarity_image->Picture->SaveToFile(SaveGraph->FileName);
    Image3->Picture->SaveToFile("song1.emf");
    Image4->Picture->SaveToFile("song2.emf");
    Beep();

}

// ---------------------------------------------------------------------------

void __fastcall TForm1::ThreshlodTBChange(TObject *Sender) {
    TTrackBar *thr;
    TNumLab *numthr;
    if (Sender == ThreshlodTB) {
        thr = ThreshlodTB;
        numthr = ThreshNum;
    }
    else if (Sender == TBperiod) {
        thr = TBperiod;
        numthr = ThreshNum2;
    }
    else {
        thr = TBperThresh;
        numthr = ThreshNum3;
    }

    switch(thr->Position) {
    case 0:
        numthr->Value = 0.01;
        THRESH = 0.79;
        Local_Thresh = 0.14;
        break;
    case 1:
        numthr->Value = 0.02;
        THRESH = 0.92;
        Local_Thresh = 0.23;
        break;
    case 2:
        numthr->Value = 0.05;
        THRESH = 1.08;
        Local_Thresh = 0.32;
        break;
    case 3:
        numthr->Value = 0.10;
        THRESH = 1.3;
        Local_Thresh = 0.5;
        break;
    case 4:
        numthr->Value = 0.15;
        THRESH = 1.5;
        Local_Thresh = 0.7;
        break;
    case 5:
        numthr->Value = 0.20;
        THRESH = 1.67;
        Local_Thresh = 0.8;
        break;
    case 6:
        numthr->Value = 0.25;
        THRESH = 1.82;
        Local_Thresh = 0.98;
        break;
    case 7:
        numthr->Value = 0.30;
        THRESH = 2;
        Local_Thresh = 1.2;
        break;
    case 8:
        numthr->Value = 0.35;
        THRESH = 2.2;
        Local_Thresh = 1.3;
        break;
    case 9:
        numthr->Value = 0.40;
        THRESH = 2.33;
        Local_Thresh = 1.45;
        break;
    case 10:
        numthr->Value = 0.50;
        THRESH = 2.6;
        Local_Thresh = 1.75;
        break;
    case 11:
        numthr->Value = 0.60;
        THRESH = 3;
        Local_Thresh = 2.25;
        break;
    case 12:
        numthr->Value = 0.70;
        THRESH = 3.5;
        Local_Thresh = 3;
        break;
    case 13:
        numthr->Value = 0.80;
        THRESH = 4.7;
        Local_Thresh = 4.1;
        break;
    case 14:
        numthr->Value = 0.90;
        THRESH = 6.2;
        Local_Thresh = 5.2;
        break;
    case 15:
        numthr->Value = 1;
        THRESH = 1000;
        Local_Thresh = 1000;
        break; // accept everything...
    }
}

// ---------------------------------------------------------------------------

float TForm1::calc_thresh(int pos) {
    switch(pos) // that's the position of the slider, see function above.
    {
    case 0:
        return(0.48);
    case 1:
        return(0.56);
    case 2:
        return(0.66);
    case 3:
        return(0.84);
    case 4:
        return(0.95);
    case 5:
        return(1.04);
    case 6:
        return(1.12);
    case 7:
        return(1.17);
    case 8:
        return(1.29);
    case 9:
        return(1.38);
    case 10:
        return(1.62);
    case 11:
        return(2.05);
    case 12:
        return(2.62);
    case 13:
        return(3.4);
    case 14:
        return(4.3);
    case 15:
        return(1000);
    }
    return(-1); // should never get here!
}

void __fastcall TForm1::IntervalTBChange(TObject *Sender) {
    static bool firstone = true;
    if (firstone) {
        firstone = false;
        AnsiString str =
            "This version of Sound Analysis does not adjust p-values when interval size is manipulated.";
        str +=
            "Consequently, contrast of score increases with interval size and will diverge from the 70ms estimate.";
        str += "We will post an adjustment table as soon as we can.";
        if (warnings->Checked)
            MessageDlg(str, mtWarning, TMsgDlgButtons() << mbOK, 0);
    }
    IntervalNum->Value = IntervalTB->Position * 2 * option->advanceTb->Value;
}

// ---------------------------------------------------------------------------

void __fastcall TForm1::newBClick(TObject *Sender) {
	static bool firsttime_open = true;

	if (firsttime_open) {
		str = DataForm->Retrieve(1, "input_folder");
		if (DirectoryExists(str))
			open_sound->DirectoryListBox1->Directory = str;
		firsttime_open = false;
	}

	else{
	   if(feature_at_pointer_cursor>1 || feature_statistics_cursor>1 ){
		   save_data();
		   Clear_featurelistClick(this);
		   delete_featurevectorClick(this);
		   Clear_featurestatviewClick(this);
		}
	}

	GUI = true;
	parameters_altered = false;
    // if(calc_contours->Checked){option->Draw_sonogram=true;  }
    // OpenDialog1->DefaultExt="*.wav";
	song_play->Close();
    play1->Enabled = false;
    play_song2->Close();
    play2->Enabled = false;
    // OpenDialog1->Filter="sound files|*.wav";
	m_Calculator1.render_image = true;
    // openwav->Visible=true;
    // if(OpenDialog1->Execute())
	open_sound->frequency_range->Position = frequency_range->Position;
	open_sound->data_window->Position = data_window->Position;
	open_sound->advance->Position = advance->Position;
	open_sound->paginate_tb->Position=DataForm->Retrieve(1,"pagination_thresh").ToInt();

	showBack = false;
	option->openFromSound1 = true;

    if (open_sound->ShowModal() == mrOk) {

		if(pagination->ItemIndex != option->paginationOn)
		{
			no_pagination_action=true;
			pagination->ItemIndex != option->paginationOn;
		}

		option->currentStartLocation = 0;
		preview_sono->Picture = open_sound->Image1->Picture;
		// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
		if (open_sound->FileName == play_song2->FileName) {
			// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
			play_song2->Close();
			// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
			play2->Enabled = false;
			// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
		}
		// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
		// put more debugging statements here.
		if (DataForm->retrieve_animal(ExtractFileName(open_sound->FileName))) {
			// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
			TDateTime hatched = DataForm->MyQuery1->FieldValues["hatched"];
			bird_name->Caption = DataForm->MyQuery1->FieldValues["bird_name"];
			details->Caption = DataForm->MyQuery1->FieldValues["details"];
			str = "syll_" + bird_name->Caption;
			DataForm->Settings(1, "syllable_table", str); // save name of current syll table in settings...???
			if (auto_create_tables->Checked)set_tables(0);
			TDateTime file_age;
			file_age = file_age.FileDateToDateTime(FileAge(open_sound->FileName));
			int the_age = file_age - hatched;
			if (the_age < 0 || the_age > 99999) {
				age->Caption = "-";
			}
			else {
			age->Caption = the_age;

			// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
			}
		}
        else {
            // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
            bird_name->Caption = "-";
            // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
            age->Caption = "-";
            // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
            // model->Caption="-";
            // training_age->Caption="-";
            details->Caption = "-";
            // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
        }
        // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
		if (bird_name->Caption != "_") {
            option->bird_ID = bird_name->Caption;
            // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
        }
        else
            option->bird_ID = "bird";
        // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
        Form1->Repaint();
        // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
        Screen->Cursor = crHourGlass;
        // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
		m_Calculator1.syllable_number = 0;
		// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
		m_Calculator1.mem_recs.erase(m_Calculator1.mem_recs.begin(),
            m_Calculator1.mem_recs.end());
        // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
        option->currentStartLocation = 0;
        // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
        option->pageNumber = 0;
        // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
        if (option->paginationOn) {
            Analyze_song1_light(open_sound->FileName, 1,
                option->currentStartLocation);
            // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
        }
        else {
            Analyze_song1(open_sound->FileName, 1);
            // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
        }
        // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
        // update the frequency range values
        int i = pow(2, 4 - frequency_range->Position);
        // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
        frequency_range_val->Value = option->sampling_rate / i;
        // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
        windowTB->Value = 1000.0 * (float)data_window->Position / (float)
            option->sampling_rate;
        // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
        advanceTb->Value = 1000.0 * (float)advance->Position / (float)
            option->sampling_rate;
        // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
        update_scale();
        // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
        update_display->Visible = false;
        // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
        Screen->Cursor = crDefault;
        // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
        float bright = set_brightness(auto_gain->Checked,
            sldrBrightness->Position, deriv_display->ItemIndex ? 0 : 1);
        // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
		m_Calculator1.GetImage().SetBrightness(bright);
		// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
		// if(deriv_display->ItemIndex==2)m_Calculator1.GetSonogram().DrawAllSlices(Image1);
		// else
        if (!option->paginationOn) {
		 switch(deriv_display->ItemIndex)
		{
		  case 0: m_Calculator1.GetImage().DrawAllSlices(Image1); break;
		  case 1: m_Calculator1.GetContours().DrawAllSlices(Image1); break;
		  case 2: m_Calculator1.GetSonogram().DrawAllSlices(Image1); break;
		}

			// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
		}
		else{
			switch(deriv_display->ItemIndex)
			{
			  case 0: m_Calculator1.GetImage().DrawSlices(0,option->currentStartLocation, Image1); break;
			  case 1: m_Calculator1.GetContours().DrawSlices(0,option->currentStartLocation, Image1); break;
			  case 2: m_Calculator1.GetSonogram().DrawSlices(0,option->currentStartLocation, Image1); break;
			}
			// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
		}
		set_scale(0);
        // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
        if (raw_features->ItemIndex)
			m_Calculator1.save_ms_table(0);
        // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
        str = ExtractFilePath(open_sound->FileName);
        // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
        DataForm->Settings(1, "input_folder", str);
        // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
        song_play->FileName = open_sound->FileName;
        // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
        file_comments->Lines->Text = DataForm->Retrieve_comments
            (songname->Caption);
		// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
		try{song_play->Open();
		}catch(...){int d=0;}
		// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
		play1->Enabled = true;
		play2->Enabled = true;
		// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
        set_scale(0);
        // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
        range_down_shading->Width = Image1->Width;
        // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
        range_up_shading->Width = Image1->Width;
        // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
        calc_rhythm->Enabled = true;
        // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
        updateSegmentation = true;
		//derivTBChange(this);
        // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
    }
    analysis_view->Visible = true;
    // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
    feature_buttons->Visible = true;
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	// int jj=song_play->TrackLength[1];
	//data_control1->TabIndex = 0;
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	pagination->Visible=true;
	if(option->paginationOn)
	{
	   ScrollBox3->HorzScrollBar->Visible=false;
	   Preview->Visible=true;
	   preview_state=true;
	   PreviewClick(this);
	}
	else
	{
	   ScrollBox3->HorzScrollBar->Visible=true;
	   Preview->Visible=false;
	   preview_state=false;
	   PreviewClick(this);
	}
	retrieve_data(songname->Caption);
	ScrollBox3->HorzScrollBar->Position=0;
}

void __fastcall TForm1::PreviewClick(TObject *Sender) {
	bool *st, *pagination_On;
	int *currentStartLoc, *totPages;
	TTrackBar* amp_db;
	TTrackBar* ent_db;
	TImage *image, *preview_sonogram, *prev_scale;
	TShape *prev_cursor;
	if (Sender == Preview || display->ActivePage == Song1) {
		amp_db = derivTB;
		ent_db = entropyTb;
		image = Image1;
		prev_scale=preview_scale;
		totPages = &option->totalPages;
		pagination_On = &option->paginationOn;
		preview_sonogram = preview_sono;
		prev_cursor = preview_cursor;
		st = &preview_state;
		currentStartLoc = &option->currentStartLocation;
	}
	else {
		amp_db = derivTB2;
		ent_db = entropyTb2;
		image = Image2;
		prev_scale=preview_scale2;
		totPages = &option->totalPages2;
		pagination_On = &option->paginationOn2;
		preview_sonogram = preview_sono2;
		prev_cursor = preview_cursor2;
		st = &preview_state2;
		currentStartLoc = &option->currentStartLocation2;
	}

	// preview sonogram  = true;
	if (*st) {
		// 225;
		image->AutoSize = false;
		image->Height = 175;
		image->Stretch = true;
		amp_db->Height = 165;
		ent_db->Height = 165;

		preview_sonogram->Visible = true;
		prev_cursor->Visible = true;
		*st = false;

		if (*totPages > 0 && (*pagination_On)) {
			prev_cursor->Left = preview_sonogram->Width *
				(*currentStartLoc - 1000) / (*totPages) + 36;
			prev_cursor->Width = 1000 * preview_sonogram->Width / (*totPages);

			if(prev_cursor->Width<5)prev_cursor->Width=5;

		}
		else {
			prev_cursor->Left = 36;
			prev_cursor->Width = preview_sonogram->Width;
			if(prev_cursor->Width<5)prev_cursor->Width=5;
		}

		prev_scale->Visible=true;
		prev_scale->Canvas->Brush->Color=clWhite;
		prev_scale->Canvas->Pen->Color=clWhite;
		prev_scale->Canvas->Rectangle(0,0,649,31);
		prev_scale->Canvas->Pen->Color=Graphics::clBlack;
		prev_scale->Canvas->Font->Size = 9;
		int song_dur=(float)(*totPages)*advanceTb->Value;
		for (int i = 0; i < 4; i++) {
		float x=(i+1)*song_dur/5000.0;
		str=x;
		str=str.SubString(1,3);
		prev_scale->Canvas->TextOut(i*200, 3, str);
	}

	}

	else {
		amp_db->Height = 235;
		ent_db->Height = 235;
		image->Height = 225;
		image->AutoSize = true;
		image->Stretch = false;
		prev_scale->Visible=false;
		preview_sonogram->Visible = false;
		prev_cursor->Visible = false;
		*st = true;
	}
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




void TForm1::Analyze_song1(AnsiString SongName, bool render) {
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	if (score)
		DeleteScoreData();
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	DataForm->Records->Active = true;
	update_score_sounds = true;
	option->halt = false;
   // option->amplitude_thresh = (125 - derivTB->Position);
	option->entropy_thresh = (float)(-entropyTb->Position) / 50.0;
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	songname->Caption = ExtractFileName(SongName);
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	songname1b->Caption = songname->Caption;
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	song1_name = SongName;
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	if (ready)
		DeleteFinalData1();
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	m_Calculator1.RenderImage(render);
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	option->wave_names[0] = SongName;
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	m_Calculator1.newFile = SongName;
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	try {
		option->extract_file_attributes(SongName);
	}
	catch(Exception & exception) {
		option->format->ItemIndex = 0;
		option->extract_file_attributes(SongName);
		option->format->ItemIndex = 1;
	}


	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	Calc(SongName, Image1);
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	// we now get the feature values for each window from the feature file...
	Iterations1 = m_Calculator1.total_frames;
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	OnsetSong1 = 1;
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	OffsetSong1 = Iterations1 - 1;
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	Signal1 = m_Calculator1.signal;
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	Amplitude1 = m_Calculator1.amplitude;
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	mfa1 = m_Calculator1.mfa;
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	Pitch1 = m_Calculator1.pitch;
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	Entropy1 = m_Calculator1.entropy;
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	FM1 = m_Calculator1.fm;
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	AM1 = m_Calculator1.am;
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	PeakFreq1 = m_Calculator1.peakFreq;
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	PitchGoodness1 = m_Calculator1.pitchGoodness;
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	continuity_t1 = m_Calculator1.continuity_t;
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	continuity_f1 = m_Calculator1.continuity_f;
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	principal_frequency1 = m_Calculator1.principal_frequency;
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	FMc1 = m_Calculator1.FMc;
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	PitchS1 = m_Calculator1.pitchS;
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	FMS1 = m_Calculator1.fmS;
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	AMS1 = m_Calculator1.AMS;
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	EntropyS1 = m_Calculator1.entropyS;
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	PitchGoodnessS1 = m_Calculator1.pitchGoodnessS;
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();

	//Segment_Sound(0, Iterations1, ksong1);
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	if (warnings->Checked) {
		int detected = 0;
		// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
		for (int i = 0; i < Iterations1; i++)
			detected += Signal1[i];
		// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
		// if (detected < 20)
		// MessageDlg("No or little sound was detected. Try changing the derivative power (yellow slide bar) threshold. If you do not want to see any more warnings, uncheck the 'show warning' button.", mtWarning, TMsgDlgButtons() << mbOK, 0);
	}
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	if (smooth->Checked)
		for (int i = 4; i < Iterations1 - 4; i++) {
			FMS1[i] = Cfl[1] * FMS1[i] + Cfl[2] * FMS1[i - 1] + Cfl[3] * FMS1
				[i - 2] + Cfl[4] * FMS1[i - 3] + Cfl[5] * FMS1[i + 1] + Cfl[6]
				* FMS1[i + 2] + Cfl[7] * FMS1[i + 3];
			// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
			PitchGoodnessS1[i] = Cfl[1] * PitchGoodnessS1[i] + Cfl[2]
				* PitchGoodnessS1[i - 1] + Cfl[3] * PitchGoodnessS1[i - 2]
				+ Cfl[4] * PitchGoodnessS1[i - 3] + Cfl[5] * PitchGoodnessS1
				[i + 1] + Cfl[6] * PitchGoodnessS1[i + 2] + Cfl[7]
				* PitchGoodnessS1[i + 3];
			// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
			PitchS1[i] = Cfl[1] * PitchS1[i] + Cfl[2] * PitchS1[i - 1] + Cfl[3]
				* PitchS1[i - 2] + Cfl[4] * PitchS1[i - 3] + Cfl[5] * PitchS1
				[i + 1] + Cfl[6] * PitchS1[i + 2] + Cfl[7] * PitchS1[i + 3];
			// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
			AMS1[i] = Cfl[1] * AMS1[i] + Cfl[2] * AMS1[i - 1] + Cfl[3] * AMS1
				[i - 2] + Cfl[4] * AMS1[i - 3] + Cfl[5] * AMS1[i + 1] + Cfl[6]
				* AMS1[i + 2] + Cfl[7] * AMS1[i + 3];
			// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
		}
	ready = true;
}





void TForm1::Analyze_song1_light(AnsiString SongName, bool render, int start) {
	if (score)
		DeleteScoreData();
	DataForm->Records->Active = true;
	update_score_sounds = true;
	option->halt = false;
   // option->amplitude_thresh = (125 - derivTB->Position);
	option->entropy_thresh = (float)(-entropyTb->Position) / 50.0;
	songname->Caption = ExtractFileName(SongName);
	songname1b->Caption = songname->Caption;
	song1_name = SongName;
	if (ready)
		DeleteFinalData1();
	m_Calculator1.RenderImage(render);
	option->wave_names[0] = SongName;
	m_Calculator1.newFile = SongName;
	try {
		option->extract_file_attributes(SongName);
	}
	catch(Exception & exception) {
		option->format->ItemIndex = 0;
		option->extract_file_attributes(SongName);
		option->format->ItemIndex = 1;
	}

	Calc_light(SongName, Image1, start);

	// we now get the feature values for each window from the feature file...
	Iterations1 = m_Calculator1.total_frames;
	OnsetSong1 = 1;
	OffsetSong1 = Iterations1 - 1;
	Signal1 = m_Calculator1.signal;
	Amplitude1 = m_Calculator1.amplitude;
	mfa1 = m_Calculator1.mfa;
	Pitch1 = m_Calculator1.pitch;
	Entropy1 = m_Calculator1.entropy;
	FM1 = m_Calculator1.fm;
	AM1 = m_Calculator1.am;
	PeakFreq1 = m_Calculator1.peakFreq;
	PitchGoodness1 = m_Calculator1.pitchGoodness;
	continuity_t1 = m_Calculator1.continuity_t;
	continuity_f1 = m_Calculator1.continuity_f;
	principal_frequency1 = m_Calculator1.principal_frequency;
	FMc1 = m_Calculator1.FMc;

	PitchS1 = m_Calculator1.pitchS;
	FMS1 = m_Calculator1.fmS;
	AMS1 = m_Calculator1.AMS;
	EntropyS1 = m_Calculator1.entropyS;
	PitchGoodnessS1 = m_Calculator1.pitchGoodnessS;

	//Segment_Sound(0, Iterations1, ksong1);
	if (warnings->Checked) {
		int detected = 0;
		for (int i = 0; i < Iterations1; i++)
			detected += Signal1[i];
		// if (detected < 20)
		// MessageDlg("No or little sound was detected. Try changing the derivative power (yellow slide bar) threshold. If you do not want to see any more warnings, uncheck the 'show warning' button.", mtWarning, TMsgDlgButtons() << mbOK, 0);
	}

	if (smooth->Checked)
		for (int i = 4; i < Iterations1 - 4; i++) {
			FMS1[i] = Cfl[1] * FMS1[i] + Cfl[2] * FMS1[i - 1] + Cfl[3] * FMS1
				[i - 2] + Cfl[4] * FMS1[i - 3] + Cfl[5] * FMS1[i + 1] + Cfl[6]
				* FMS1[i + 2] + Cfl[7] * FMS1[i + 3];

			PitchGoodnessS1[i] = Cfl[1] * PitchGoodnessS1[i] + Cfl[2]
				* PitchGoodnessS1[i - 1] + Cfl[3] * PitchGoodnessS1[i - 2]
				+ Cfl[4] * PitchGoodnessS1[i - 3] + Cfl[5] * PitchGoodnessS1
				[i + 1] + Cfl[6] * PitchGoodnessS1[i + 2] + Cfl[7]
				* PitchGoodnessS1[i + 3];

			PitchS1[i] = Cfl[1] * PitchS1[i] + Cfl[2] * PitchS1[i - 1] + Cfl[3]
				* PitchS1[i - 2] + Cfl[4] * PitchS1[i - 3] + Cfl[5] * PitchS1
				[i + 1] + Cfl[6] * PitchS1[i + 2] + Cfl[7] * PitchS1[i + 3];

			AMS1[i] = Cfl[1] * AMS1[i] + Cfl[2] * AMS1[i - 1] + Cfl[3] * AMS1
				[i - 2] + Cfl[4] * AMS1[i - 3] + Cfl[5] * AMS1[i + 1] + Cfl[6]
				* AMS1[i + 2] + Cfl[7] * AMS1[i + 3];

		}
	ready = true;
}

void TForm1::Calc(AnsiString fileName, TImage *Image) {
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	bool img;
	TRadioGroup *pagi;
	TRadioGroup *disp;
	CZT_FeatureCalc *calci;
	if (Image == Image1) {
		img = 0;
		pagi = pagination;
		calci = &m_Calculator1;
		calc_advance1=advance->Position;
		disp=deriv_display;
		option->pri_segmented_feature = option->pri_segmented_feature1;
		option->pri_thresh_raw = option->pri_thresh_raw1;
		option->reverse_segmentation =  option->reverse_segmentation1;
		option->smooth_feature = option->smooth_feature1;
		option->adapt_threshold = option->adapt_threshold1;
		option->sec_segmentation = option->sec_segmentation1;
		option->sec_segmented_feature = option->sec_segmented_feature1;
		option->sec_thresh_raw =  option->sec_thresh_raw1;
		option->sec_lt = option->sec_lt1;
		option->sec_logic =  option->sec_logic1;
		option->hp_small = option->hp_small1;
		option->hp_big = option->hp_big1;
	}
	else {
		img = 1;
		pagi = pagination2;
		calci = &m_Calculator2;
		calc_advance2=advance2->Position;
		disp=deriv_display2;
		option->pri_segmented_feature = option->pri_segmented_feature2;
		option->pri_thresh_raw = option->pri_thresh_raw2;
		option->reverse_segmentation =  option->reverse_segmentation2;
		option->smooth_feature = option->smooth_feature2;
		option->adapt_threshold = option->adapt_threshold2;
		option->sec_segmentation = option->sec_segmentation2;
		option->sec_segmented_feature = option->sec_segmented_feature2;
		option->sec_thresh_raw =  option->sec_thresh_raw2;
		option->sec_lt = option->sec_lt2;
		option->sec_logic =  option->sec_logic2;
		option->hp_small = option->hp_small2;
		option->hp_big = option->hp_big2;
	}
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	// CWavIterator wave;
	CWavIterator wave[1];
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	// wv[0]=wave;
	if (option->halt)
		return;
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	// calci->ResetContents();
	CZT_PcmEq equalizer;
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	if (wave[0].OpenFile(fileName.c_str()) == pcmpos_error)
		Application->MessageBox(L"Error: could not open wave file", NULL,
		MB_OK); // != pcmpos_error)
	else {
		/* if(wave.m_WaveHeader.nChannels==2)
		{
		wave.ResetContents();
		wave_split(fileName.c_str());
		if(wave.OpenFile(split.c_str())== pcmpos_error)return;
		} */
		// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
		numSamples = wave[0].GetTotalSamples();
		//int foo= numSamples / option->Advance;
		// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
		int dur = numSamples / wave[0].m_WaveHeader.nSamplesPerSec;
		// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();

		if (dur < 1 && pagi->Visible && !dobatch) {
			pagi->Controls[1]->Enabled = false;
			pagi->ItemIndex = 0;
		}
		else if(pagi->Visible && !dobatch){
			pagi->Controls[1]->Enabled = true;
		}

		// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
		pagi->ItemIndex = 0;

		SoundFile = fileName;
		// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
		if (equalize->Checked)
			wave[0].UseFilter(equalizer);
		// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
		if (calci->Calculate(wave, 0) && GUI) {
			// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
			calci->render_image = true;
			// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
			switch(disp->ItemIndex)
			{
			  case 0: calci->GetImage().DrawAllSlices(Image); break;
			  case 1: calci->GetContours().DrawAllSlices(Image); break;
			  case 2: calci->GetSonogram().DrawAllSlices(Image); break;
			}


			// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
			Sonogram->Assign(Image1->Picture->Bitmap);
			// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
			set_scale(img);
			// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
			ScrollBox1->HorzScrollBar->Position=0;//(ScrollBox1->HorzScrollBar->Range
			// *calci->bout_end / calci->total_frames);
		}
	}
}

// ---------------------------------------------------------------------------

void TForm1::Calc_light(AnsiString fileName, TImage *Image, int begin) {
	bool img;
	bool *show_adv, *show_Back;
	int* currentStartLoc;
	int *pageNum, *totPages;
	TRadioGroup *pagi;
	TRadioGroup *disp;
	CZT_FeatureCalc *calci;
	if (Image == Image1) {
		img = 0;
		calci = &m_Calculator1;
		calc_advance1=advance->Position;
		show_adv = &showAdvance;
		show_Back = &showBack;
		totPages = &option->totalPages;
		pageNum = &option->pageNumber;
		currentStartLoc = &option->currentStartLocation;
		pagi = pagination;
		disp=deriv_display;

		option->pri_segmented_feature = option->pri_segmented_feature1;
		option->pri_thresh_raw = option->pri_thresh_raw1;
		option->reverse_segmentation =  option->reverse_segmentation1;
		option->smooth_feature = option->smooth_feature1;
		option->adapt_threshold = option->adapt_threshold1;
		option->sec_segmentation = option->sec_segmentation1;
		option->sec_segmented_feature = option->sec_segmented_feature1;
		option->sec_thresh_raw =  option->sec_thresh_raw1;
		option->sec_lt = option->sec_lt1;
		option->sec_logic =  option->sec_logic1;
		option->hp_small = option->hp_small1;
		option->hp_big = option->hp_big1;
	}
	else {
		img = 1;
		calci = &m_Calculator2;
		calc_advance2=advance2->Position;
		show_adv = &showAdvance2;
		show_Back = &showBack2;
		totPages = &option->totalPages2;
		pageNum = &option->pageNumber2;
		currentStartLoc = &option->currentStartLocation2;
		pagi = pagination2;
		disp=deriv_display2;

		option->pri_segmented_feature = option->pri_segmented_feature2;
		option->pri_thresh_raw = option->pri_thresh_raw2;
		option->reverse_segmentation =  option->reverse_segmentation2;
		option->smooth_feature = option->smooth_feature2;
		option->adapt_threshold = option->adapt_threshold2;
		option->sec_segmentation = option->sec_segmentation2;
		option->sec_segmented_feature = option->sec_segmented_feature2;
		option->sec_thresh_raw =  option->sec_thresh_raw2;
		option->sec_lt = option->sec_lt2;
		option->sec_logic =  option->sec_logic2;
		option->hp_small = option->hp_small2;
		option->hp_big = option->hp_big2;
	}
	// int foo = option->calculateLocation;
	// CWavIterator wave;
	CWavIterator wave[1];
	// wv[0]=wave;
	if (option->halt)
		return;
	// calci->ResetContents();
	CZT_PcmEq equalizer;
	if (wave[0].OpenFile(fileName.c_str()) == pcmpos_error)
		Application->MessageBox(L"Error: could not open wave file", NULL,
		MB_OK); // != pcmpos_error)
	else {
		/* if (option->pageNumber > 1) {
		showBack = true;
		} else {
		showBack = false;
		} */
		*show_adv = true;
		beginLocation = begin;
		// wave.ResetContents();
		int foo = wave[0].GetTotalSamples() / option->Advance;
		*totPages = foo;
		int dur = wave[0].GetTotalSamples()
			/ wave[0].m_WaveHeader.nSamplesPerSec;
		if (dur < 1 && pagi->Visible && !dobatch) {
			pagi->Controls[1]->Enabled = false;
			pagi->ItemIndex = 0;
		}
		else if(pagi->Visible && !dobatch) {
			pagi->Controls[1]->Enabled = true;
			pagi->ItemIndex = 1;
		}

		if (foo > *currentStartLoc + 1) {
			if (doIncrement)
				(*pageNum)++;
			*show_Back = true;
		}
		//if (foo > *currentStartLoc + 1000) {
		if (foo > *currentStartLoc + 1) {
			if (doIncrement)
				*currentStartLoc += 1000;
			*show_Back = true;
			if (foo < *currentStartLoc ) {
				 *show_adv = false;
			}
			// option->pageNumber++;
		} //else {
			//if (doIncrement)
			 //   *currentStartLoc = foo - 1;
			//*show_adv = false;
		//}

		endLocation = *currentStartLoc;
		if (!doIncrement) {
			beginLocation -= 1000;
		}

		SoundFile = fileName;
		if (equalize->Checked)
			wave[0].UseFilter(equalizer);
		if (calci->Calculate_light(wave, 0, beginLocation,
				endLocation * (option->Advance)) && GUI) {
			// if (calci->Calculate(wave, 0) && GUI) {
			calci->render_image = true;
			switch(disp->ItemIndex)
			{
			  case 0: calci->GetImage().DrawSlices(0, 1000 - 1, Image); break;
			  case 1: calci->GetContours().DrawSlices(0, 1000 - 1, Image); break;
			  case 2: calci->GetSonogram().DrawSlices(0, 1000 - 1, Image); break;
            }

			Sonogram->Assign(Image->Picture->Bitmap);
			 int page = (*pageNum)/1000 - 1;(img);
		}
	}
	/* if (*pageNum > 1) {
	 *show_Back = true;
	} else {
	 *show_Back = false;
	} */
	if (*currentStartLoc > 1) {
		*show_Back = true;
    }
    else {
        *show_Back = false;
    }

}

// ---------------------------------------------------------------------------

void __fastcall TForm1::new2BClick(TObject *Sender) {
	GUI = true;
	// if(calc_contours->Checked) { option->Draw_sonogram=true; Sono2B->Enabled=true; }
	// else Sono2B->Enabled=false;
	// OpenDialog1->DefaultExt="*.wav";
	// OpenDialog1->Filter="sound files|*.wav";
	m_Calculator2.render_image = true;
	song_play->Close();
	play1->Enabled = false;
	play_song2->Close();
	play2->Enabled = false;

	open_sound->frequency_range->Position = frequency_range2->Position;
	open_sound->data_window->Position = data_window2->Position;
	open_sound->advance->Position = advance2->Position;
	option->openFromSound1 = false;

	if (open_sound->ShowModal() == mrOk) // Execute())
	{
		// delta option->paginationOn2=option->paginationOn;
		if(pagination2->ItemIndex != option->paginationOn2)
		{
			no_pagination_action=true;
			pagination2->ItemIndex != option->paginationOn2;
		}

		//pagination2->ItemIndex = option->paginationOn2;
		option->currentStartLocation2 = 0;
		preview_sono2->Picture = open_sound->Image1->Picture;
		if (DataForm->retrieve_animal(ExtractFileName(open_sound->FileName2))) {
			TDateTime hatched = DataForm->MyQuery1->FieldValues["hatched"];
			TDateTime file_age;
			file_age = file_age.FileDateToDateTime
				(FileAge(open_sound->FileName2));
			int the_age = file_age - hatched;
			if (the_age < 0 || the_age > 99999) {
				bird_name2->Caption = "-";
				age2->Caption = "-";
				details2->Caption = "-";
			}
			else {
				bird_name2->Caption = DataForm->MyQuery1->FieldValues
					["bird_name"];
				age2->Caption = the_age;
				TDateTime trn = DataForm->MyQuery1->FieldValues["training"];
				details2->Caption = DataForm->MyQuery1->FieldValues["details"];
				str = "syll_" + bird_name2->Caption;
				DataForm->Settings(2, "syllable_table", str);
				if (auto_create_tables2->Checked)
					set_tables(1);
			}
		}
		else {
			bird_name2->Caption = "-";
			age2->Caption = "-";
			details2->Caption = "-";
		}
		if (bird_name2->Caption != "_")
			option->bird_ID = bird_name2->Caption;
		else
			option->bird_ID = "bird";
		Form1->Repaint();
		Screen->Cursor = crHourGlass;
		m_Calculator2.syllable_number = 0;
		m_Calculator2.mem_recs.erase(m_Calculator2.mem_recs.begin(),
			m_Calculator2.mem_recs.end());

		option->currentStartLocation2 = 0;
		option->pageNumber2 = 0;
		// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();

		Analyze_song2(open_sound->FileName2, 1);

		// update the frequency range values
		int i = pow(2, 4 - frequency_range->Position);
		frequency_range2_val->Value = option->sampling_rate / i;
		windowTB2->Value = 1000.0 * (float)data_window->Position / (float)
			option->sampling_rate;
		advanceTb2->Value = 1000.0 * (float)advance->Position / (float)
			option->sampling_rate;
		update_scale();

		update_display2->Visible = false;

		hide_all_features(&m_Calculator2); // GUI, do not show feature curves
		play_song2->FileName = open_sound->FileName2;
		play_song2->Open();
		play2->Enabled = true;
		ready2 = true;
		Screen->Cursor = crDefault;
		str = ExtractFilePath(open_sound->FileName2);
		DataForm->Settings(2, "input_folder", str);
		if (open_sound->FileName2 != song_play->FileName)
		  song_play->Open();
		play1->Enabled = true;
		play2->Enabled = true;
		updateSegmentation2 = true;
	   //	derivTBChange(this);
		// DerivPowerBClick(this);
		if(option->paginationOn2)
		{
		   ScrollBox4->HorzScrollBar->Visible=false;
		   Preview2->Visible=true;
		   preview_state2=true;
		   PreviewClick(this);
		}
		else
		{
		   ScrollBox4->HorzScrollBar->Visible=true;
		   Preview2->Visible=false;
		   preview_state2=false;
		   PreviewClick(this);
		}
	}
	retrieve_data2(SongName2a->Caption);
}

////////////////////////////////////////////////////////////////////////////////

void TForm1::Analyze_song2(AnsiString SongName, bool render) {
	if (score)
		DeleteScoreData();
	DataForm->Records->Active = true;
	update_score_sounds = true;
	option->halt = false;
   // option->amplitude_thresh = (125 - derivTB2->Position);
	option->entropy_thresh = (float)(-entropyTb2->Position) / 50.0;
	song2_name = SongName;
	SongName2a->Caption = ExtractFileName(SongName);
	songname2b->Caption = SongName2a->Caption;
	Similarity->TabVisible = true;
	Combined->TabVisible = true;
	show_song2->Visible = true;
	try {
		per_sound2->Visible = true;
	}
	catch(Exception & exception) {
		option->format->ItemIndex = 0;
		option->extract_file_attributes(SongName);
		option->format->ItemIndex = 1;
	}

	// Combined->TabVisible=true;
	if (ready2)
		DeleteFinalData2();
	m_Calculator2.RenderImage(render);
	option->wave_names[0] = SongName;
	m_Calculator2.newFile = SongName;
	option->extract_file_attributes(SongName);


	if (option->paginationOn2) {
		Calc_light(SongName, Image2, option->currentStartLocation2);
		// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	}
	else {
		Calc(SongName, Image2);
		// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	}

	// Calc(SongName, Image2);
	// Calc_light(SongName, Image2, option->currentStartLocation2);
	// we now get the feature values for each window from the feature file...
	Iterations2 = m_Calculator2.total_frames;
	OnsetSong2 = 1;
	OffsetSong2 = Iterations2 - 1;
	Signal2 = m_Calculator2.signal;
	Amplitude2 = m_Calculator2.amplitude;
	mfa2 = m_Calculator2.mfa;
	Pitch2 = m_Calculator2.pitch;
	Entropy2 = m_Calculator2.entropy;
	FM2 = m_Calculator2.fm;
	AM2 = m_Calculator2.am;
	PeakFreq2 = m_Calculator2.peakFreq;
	PitchGoodness2 = m_Calculator2.pitchGoodness;
	FMc2 = m_Calculator2.FMc;
	PitchS2 = m_Calculator2.pitchS;
	FMS2 = m_Calculator2.fmS;
	AMS2 = m_Calculator2.AMS;
	EntropyS2 = m_Calculator2.entropyS;
	PitchGoodnessS2 = m_Calculator2.pitchGoodnessS;
	principal_frequency2 = m_Calculator2.principal_frequency;
	continuity_t2 = m_Calculator2.continuity_t;
	continuity_f2 = m_Calculator2.continuity_f;
	FMc2 = m_Calculator2.FMc;

//	Segment_Sound(0, Iterations2, ksong2);
	if (warnings->Checked) {
		int detected = 0;
		for (int i = 0; i < Iterations2; i++)
			detected += Signal2[i];
		/* if (detected < 20)
		MessageDlg(
		"No or few sound where detected. Try changing the derivative power (yellow slide bar) threshold. If you do not want to see any more warnings, uncheck the 'show warning' button.", mtWarning, TMsgDlgButtons() << mbOK, 0);
		 */ }

	if (smooth->Checked)
		for (int i = 4; i < Iterations2 - 4; i++) {
			FMS2[i] = Cfl[1] * FMS2[i] + Cfl[2] * FMS2[i - 1] + Cfl[3] * FMS2
				[i - 2] + Cfl[4] * FMS2[i - 3] + Cfl[5] * FMS2[i + 1] + Cfl[6]
				* FMS2[i + 2] + Cfl[7] * FMS2[i + 3];

			PitchGoodnessS2[i] = Cfl[1] * PitchGoodnessS2[i] + Cfl[2]
				* PitchGoodnessS2[i - 1] + Cfl[3] * PitchGoodnessS2[i - 2]
				+ Cfl[4] * PitchGoodnessS2[i - 3] + Cfl[5] * PitchGoodnessS2
				[i + 1] + Cfl[6] * PitchGoodnessS2[i + 2] + Cfl[7]
				* PitchGoodnessS2[i + 3];

			PitchS2[i] = Cfl[1] * PitchS2[i] + Cfl[2] * PitchS2[i - 1] + Cfl[3]
				* PitchS2[i - 2] + Cfl[4] * PitchS2[i - 3] + Cfl[5] * PitchS2
				[i + 1] + Cfl[6] * PitchS2[i + 2] + Cfl[7] * PitchS2[i + 3];

			AMS2[i] = Cfl[1] * AMS2[i] + Cfl[2] * AMS2[i - 1] + Cfl[3] * AMS2
				[i - 2] + Cfl[4] * AMS2[i - 3] + Cfl[5] * AMS2[i + 1] + Cfl[6]
				* AMS2[i + 2] + Cfl[7] * AMS2[i + 3];
		}

}

// ---------------------------------------------------------------------------

void __fastcall TForm1::SaveRecBtClick(TObject *Sender) {
	TStringList *strings;
    strings = new TStringList;
    SaveDialog1->InitialDir = "c:\\sap";
    SaveDialog1->DefaultExt = "TXT";
    Beep();
    SaveDialog1->Title = "Please choose a name for records file";
    if (SaveDialog1->Execute()) {
        Beep();
        // strings->Add("interval;");
        // strings->AddStrings(FinalRecsM->Lines);
        /* strings->Add("mean values; pitch; entropy; FM; continuity ");
        strings->AddStrings(featuresPage->Lines);
        strings->Add("diversity;");
        strings->AddStrings(diversityPage->Lines); */
        // strings->Add("period;");
		// strings->AddStrings(perPage->Lines);
		// strings->Add("KS statistic;");
		// strings->AddStrings(KSpage->Lines);
		// strings->Add("comments;");
		// strings->AddStrings(commentsPage->Lines);
		// strings->SaveToFile(SaveDialog1->FileName);
	}
	delete strings;
}

// ---------------------------------------------------------------------------

void __fastcall TForm1::AddBtClick(TObject *Sender) {
	static int serial_num = 0;
	CZT_FeatureCalc *calci;
	TEdit *memo;
	TLabel *song_name;
	TStaticText *id;
	TReportListView *feature_statistics_V;
	bool the_song;
	int *feature_statistics_curs;
	int *start;

	TNumLab *begin, *end, *amplitude, *durationLb, *pitchAvgLb, *FMAvgLb,
	*entAvgLb, *FMcAvg, *peakFreqAvgLb, *pgoodAvgLb, *meanAM,
	*principalFreqAvg, *cont_fAvg, *cont_tAvg, *amplitudeMin,
	*pitchMin, *principalFreqMin, *pgoodnessMin, *FMmin, *AMmin, *entMin,
	*peakFreqMin, *FMcMin, *cont_fMin, *cont_tMin, *amplitudeMax, *pitchMax,
	*principalFreqMax, *pgoodnessMax, *FMmax, *AMmax, *entMax, *peakFreqMax,
	*FMcMax, *cont_tMax, *cont_fMax, *varamplitude, *varpitch, *varpeakFreq,
	*varFM, *varAM, *varentropy, *varprincipalFreq, *varpgoodness, *varmeanFr,
	*varcontf, *varcontt;

	TStaticText *recNum;

	if (display->ActivePage==Song1){
		if (!interval1) {
			Application->MessageBox(L"Please Outline the sound and try again",
				NULL, MB_OK);
			return;
		}

		if (CalcMode->ItemIndex == auto_segment){
		   save_syllables(0);
		   return;
		}

		calci = &m_Calculator1;
		the_song=0;
		feature_statistics_V = feature_statistics_View;
		feature_statistics_curs = &feature_statistics_cursor;
		start = &option->currentStartLocation;
		memo = comments;
		song_name = songname;
		id = bird_name;
		recNum = RecordsNum;

		durationLb = durationL;
		begin = beginL;
		end = endL;

		amplitude = derivAvgL;
		amplitudeMin = derivMinL;
		amplitudeMax = derivMaxL;
		varamplitude = derivVarL;

		pitchAvgLb = pitchAvgL;
		pitchMin = pitchMinL;
		pitchMax = pitchMaxL;
		varpitch = pitchVarL;

		FMAvgLb = FMAvgL;
		FMmin = FMminL;
		FMmax = FMmaxL;
		varFM = FMVarL;

		meanAM = AMAvgL;
		AMmin = AMMinL;
		AMmax = AMMaxL;
		varAM = AMVarL;

		entAvgLb = entAvgL;
		entMin = entMinL;
		entMax = entMaxL;
		varentropy = entVarL;

		peakFreqAvgLb = peakFreqAvgL;
		peakFreqMin = peakFreqMinL;
		peakFreqMax = peakFreqMaxL;
		varpeakFreq = peakFreqVarL;

		pgoodAvgLb = pgoodAvgL;
		pgoodnessMin = pgoodMinL;
		pgoodnessMax = pgoodMaxL;
		varpgoodness = pgoodVarL;

		principalFreqAvg = principalFreqAvgL;
		principalFreqMin = principalFreqMinL;
		principalFreqMax = principalFreqMaxL;
		varprincipalFreq = principalFreqVarL;

		cont_fAvg = cont_fAvgL;
		cont_fMin = cont_fMinL;
		cont_fMax = cont_fMaxL;
		varcontf = cont_fVarL;

		cont_tAvg = cont_tAvgL;
		cont_tMin = cont_tMinL;
		cont_tMax = cont_tMaxL;
		varcontt = cont_tVarL;

		FMcAvg = FMcAvgL;
		FMcMin = FMcMinL;
		FMcMax = FMcMaxL;

		option->syllable_table_name = table_name->Caption;

	}
	else {
		if (!interval2) {
			Application->MessageBox(L"Please Outline the sound and try again",
				NULL, MB_OK);
			return;
		}

		if (CalcMode2->ItemIndex == auto_segment){
		   save_syllables(1);
		   return;
		}

		calci = &m_Calculator2;
		the_song=1;
		 feature_statistics_V = feature_statistics_View2;
		feature_statistics_curs = &feature_statistics2_cursor;
		start = &option->currentStartLocation2;
		memo = comments;
		song_name = SongName2a;
		id = bird_name2;
		recNum = RecordsNum2;

		durationLb = duration2L;
		begin = begin2L;
		end = end2L;

		amplitude = deriv2AvgL;
		amplitudeMin = deriv2MinL;
		amplitudeMax = deriv2MaxL;
		varamplitude = deriv2VarL;

		pitchAvgLb = pitch2AvgL;
		pitchMin = pitch2MinL;
		pitchMax = pitch2MaxL;
		varpitch = pitch2VarL;

		FMAvgLb = FM2AvgL;
		FMmin = FM2minL;
		FMmax = FM2maxL;
		varFM = FM2VarL;

		meanAM = AM2AvgL;
		AMmin = AM2MinL;
		AMmax = AM2MaxL;
		varAM = AM2VarL;

		entAvgLb = ent2AvgL;
		entMin = ent2MinL;
		entMax = ent2MaxL;
		varentropy = ent2VarL;

		peakFreqAvgLb = peakFreq2AvgL;
		peakFreqMin = peakFreq2MinL;
		peakFreqMax = peakFreq2MaxL;
		varpeakFreq = peakFreq2VarL;

		pgoodAvgLb = pgood2AvgL;
		pgoodnessMin = pgood2MinL;
		pgoodnessMax = pgood2MaxL;
		varpgoodness = pgood2VarL;

		principalFreqAvg = principalFreq2AvgL;
		principalFreqMin = principalFreq2MinL;
		principalFreqMax = principalFreq2MaxL;
		varprincipalFreq = principalFreq2VarL;

		cont_fAvg = cont_f2AvgL;
		cont_fMin = cont_f2MinL;
		cont_fMax = cont_f2MaxL;
		varcontf = cont_f2VarL;

		cont_tAvg = cont_t2AvgL;
		cont_tMin = cont_t2MinL;
		cont_tMax = cont_t2MaxL;
		varcontt = cont_t2VarL;

		FMcAvg = FMc2AvgL;
		FMcMin = FMc2MinL;
		FMcMax = FMc2MaxL;

		option->syllable_table_name = table_name2->Caption;
	}


	if( (!the_song && features_display->TabIndex==2) || (the_song && features_display2->TabIndex==2))
	// add data to data view report:
   {
	// Avg values
	str = durationLb->Value;
	feature_statistics_V->Elem[1][*feature_statistics_curs] = str.SubString(1,
		6);
	str = amplitude->Value;
	feature_statistics_V->Elem[2][*feature_statistics_curs] = str.SubString(1,
		6);
	str = pitchAvgLb->Value;
	feature_statistics_V->Elem[3][*feature_statistics_curs] = str.SubString(1,
		6);
	str = peakFreqAvgLb->Value;
	feature_statistics_V->Elem[4][*feature_statistics_curs] = str.SubString(1,
		6);
	str = principalFreqAvg->Value;
	feature_statistics_V->Elem[5][*feature_statistics_curs] = str.SubString(1,
		6);
	str = pgoodAvgLb->Value;
	feature_statistics_V->Elem[6][*feature_statistics_curs] = str.SubString(1,
		6);
	str = FMAvgLb->Value;
	feature_statistics_V->Elem[7][*feature_statistics_curs] = str.SubString(1,
		6);
	str = meanAM->Value;
	feature_statistics_V->Elem[8][*feature_statistics_curs] = str.SubString(1,
		6);
	str = entAvgLb->Value;
	feature_statistics_V->Elem[9][*feature_statistics_curs] = str.SubString(1,
		6);
	str = cont_tAvg->Value;
	feature_statistics_V->Elem[10][*feature_statistics_curs] = str.SubString(1,
		6);
	str = cont_fAvg->Value;
	feature_statistics_V->Elem[11][*feature_statistics_curs] = str.SubString(1,
		6);
	feature_statistics_V->Elem[12][*feature_statistics_curs] = (AnsiString)
		comments->Text;

	// Min values


	str = begin->Value;
	feature_statistics_V->Elem[13][*feature_statistics_curs] = str.SubString(1,
		7); // hidden
	str = amplitudeMin->Value;
	feature_statistics_V->Elem[14][*feature_statistics_curs] = str.SubString(1,
		6);
	str = pitchMin->Value;
	feature_statistics_V->Elem[15][*feature_statistics_curs] = str.SubString(1,6);
	str = peakFreqMin->Value;
	feature_statistics_V->Elem[16][*feature_statistics_curs] = str.SubString(1,
		6);
	str = principalFreqMin->Value;
	feature_statistics_V->Elem[17][*feature_statistics_curs] = str.SubString(1,
		6);
	str = pgoodnessMin->Value;
	feature_statistics_V->Elem[18][*feature_statistics_curs] = str.SubString(1,
		6);
	str = FMmin->Value;
	feature_statistics_V->Elem[19][*feature_statistics_curs] = str.SubString(1,
		6);
	str = AMmin->Value;
	feature_statistics_V->Elem[20][*feature_statistics_curs] = str.SubString(1,
		6);
	str = entMin->Value;
	feature_statistics_V->Elem[21][*feature_statistics_curs] = str.SubString(1,
		6);
	str = cont_tMin->Value;
	feature_statistics_V->Elem[22][*feature_statistics_curs] = str.SubString(1,
		6);
	str = cont_fMin->Value;
	feature_statistics_V->Elem[23][*feature_statistics_curs] = str.SubString(1,
		6);
	feature_statistics_V->Elem[24][*feature_statistics_curs] = (AnsiString)
		comments->Text;

	// Max values
	str = end->Value;
	feature_statistics_V->Elem[25][*feature_statistics_curs] = str.SubString(1,
		7); // hidden
	str = amplitudeMax->Value;
	feature_statistics_V->Elem[26][*feature_statistics_curs] = str.SubString(1,
		6);
	str = pitchMax->Value;
	feature_statistics_V->Elem[27][*feature_statistics_curs] = str.SubString(1,
		6);
	str = peakFreqMax->Value;
	feature_statistics_V->Elem[28][*feature_statistics_curs] = str.SubString(1,
		6);
	str = principalFreqMax->Value;
	feature_statistics_V->Elem[29][*feature_statistics_curs] = str.SubString(1,
		6);
	str = pgoodnessMax->Value;
	feature_statistics_V->Elem[30][*feature_statistics_curs] = str.SubString(1,
		6);
	str = FMmax->Value;
	feature_statistics_V->Elem[31][*feature_statistics_curs] = str.SubString(1,
		6);
	str = AMmax->Value;
	feature_statistics_V->Elem[32][*feature_statistics_curs] = str.SubString(1,
		6);
	str = entMax->Value;
	feature_statistics_V->Elem[33][*feature_statistics_curs] = str.SubString(1,
		6);
	str = cont_tMax->Value;
	feature_statistics_V->Elem[34][*feature_statistics_curs] = str.SubString(1,
		6);
	str = cont_fMax->Value;
	feature_statistics_V->Elem[35][*feature_statistics_curs] = str.SubString(1,
		6);
	feature_statistics_V->Elem[36][*feature_statistics_curs] = (AnsiString)
		comments->Text;

	// STD values:
	str = "";
	feature_statistics_V->Elem[37][*feature_statistics_curs] = str.SubString(1,
		6); // hidden
	str = varamplitude->Value;
	feature_statistics_V->Elem[38][*feature_statistics_curs] = str.SubString(1,
		6);
	str = varpitch->Value;
	feature_statistics_V->Elem[39][*feature_statistics_curs] = str.SubString(1,
		6);
	str = varpeakFreq->Value;
	feature_statistics_V->Elem[40][*feature_statistics_curs] = str.SubString(1,
		6);
	str = varprincipalFreq->Value;
	feature_statistics_V->Elem[41][*feature_statistics_curs] = str.SubString(1,
		6);
	str = varpgoodness->Value;
	feature_statistics_V->Elem[42][*feature_statistics_curs] = str.SubString(1,
		6);
	str = varFM->Value;
	feature_statistics_V->Elem[43][*feature_statistics_curs] = str.SubString(1,
		6);
	str = varAM->Value;
	feature_statistics_V->Elem[44][*feature_statistics_curs] = str.SubString(1,
		6);
	str = varentropy->Value;
	feature_statistics_V->Elem[45][*feature_statistics_curs] = str.SubString(1,
		6);
	str = varcontt->Value;
	feature_statistics_V->Elem[46][*feature_statistics_curs] = str.SubString(1,
		6);
	str = varcontf->Value;
	feature_statistics_V->Elem[47][*feature_statistics_curs] = str.SubString(1,
		6);
	feature_statistics_V->Elem[48][*feature_statistics_curs] = (AnsiString)
		comments->Text;
	str = AnsiString(*start-1000);
	feature_statistics_V->Elem[49][*feature_statistics_curs] = str; //.SubString(1,6);  //hidden
   // finish:
	(*feature_statistics_curs)++;
	feature_statistics_V->AddRow();
	feature_statistics_V->TopRow = feature_statistics_V->NrOfRows - 5;

   }

   else {	// add to database:

	calci->features.bird_ID = id->Caption; // .ToInt();
	calci->features.serial_number = serial_num;
	serial_num++;
	calci->features.start = begin->Value;
	calci->features.duration = durationLb->Value / option->advanceTb->Value;
	// 0ffset the multiplication by 1.4 in zt_calc

	calci->features.mean_amplitude = 10 * amplitude->Value;
	calci->features.mean_pitch = pitchAvgLb->Value;
	calci->features.mean_FM = 10 * FMAvgLb->Value;
	calci->features.mean_entropy = 100 * entAvgLb->Value;
	calci->features.mean_pGood = 10 * pgoodAvgLb->Value;
	calci->features.mean_peakFr = peakFreqAvgLb->Value;
	calci->features.mean_am = 10000 * meanAM->Value;
	calci->features.mean_slope = FMcAvg->Value;
	calci->features.mean_principal_contour = principalFreqAvg->Value;
	calci->features.mean_continuity_f = 100 * cont_fAvg->Value;
	calci->features.mean_continuity_t = 100 * cont_tAvg->Value;

	calci->features.min_pitch = pitchMin->Value;
	calci->features.min_principal_contour = principalFreqMin->Value;
	calci->features.min_FM = 10 * FMmin->Value;
	calci->features.min_entropy = 100 * entMin->Value;
	calci->features.min_peakFr = peakFreqMin->Value;
	calci->features.min_slope = 100 * FMcMin->Value;

	calci->features.max_pitch = pitchMax->Value;
	calci->features.max_principal_contour = principalFreqMax->Value;
	calci->features.max_FM = 10 * FMmax->Value;
	calci->features.max_entropy = 100 * entMax->Value;
	calci->features.max_peakFr = peakFreqMax->Value;
	calci->features.max_slope = 100 * FMcMax->Value;
	calci->features.max_amplitude = amplitudeMax->Value;
	calci->features.max_continuity_f = 100 * cont_fMax->Value;
	calci->features.max_continuity_t = 100 * cont_tMax->Value;

	calci->features.var_pitch = 0.001 * (*VarPitch);
	calci->features.var_FM = *VarFM;
	calci->features.var_entropy = 100 * (*VarEnt);
	calci->features.var_pGood = 0.01 * (*VarPGood);
	calci->features.var_am = 1000 * Pos_maxAmp1->Value; // 0;//(*VarAM);
	calci->features.var_peakFr = 1000 * Pos_maxFreq1->Value;
	// 0.001*(*VarPeakFr);
	calci->current_file_name = song_name->Caption;
	calci->comments = memo->Text;

	calci->Add_Record();
	recNum->Caption = DataForm->count_records(option->syllable_table_name);
   }
	// DataForm->Records->Append();
	// DataForm->CountRecsClick(this);
	// recNum->Caption=DataForm->recNum;
	// Beep();
}

// ---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action) {
	song_play->Close();
	play_song2->Close();
	if (score)
		DeleteScoreData();
	if (FinalSong1)
		DeleteFinalData1();
	if (FinalSong2)
		DeleteFinalData2();
	StartForm->Visible = true;
	StartForm->Repaint();
	// Sleep(50);
	// StartForm->Repaint();
	// Sleep(50);
	bool xx=option->calculate_continuity;
	option->calculate_continuity=false;
	delete open_sound;
	option->calculate_continuity=xx;
	StartForm->Refresh();
	DataForm->Save_frame_params(Frame11, 1);
	DataForm->Save_frame_params(Frame12, 2);
	DataForm->Settings(1,"sonogram_folder", SaveGraph->InitialDir);
	DataForm->Settings(1,"tabs", features_display->TabIndex);
	DataForm->Settings(2,"tabs", features_display2->TabIndex);
	DataForm->Settings(1,"tabs2", data_control1->TabIndex);
	DataForm->Settings(2,"tabs2", data_control2->TabIndex);
	if(feature_at_pointer_cursor>1 || feature_statistics_cursor>1 ) save_data();
}

// ---------------------------------------------------------------------------

void __fastcall TForm1::RecordMemo2Click(TObject *Sender) {
    // RecordMemo2->Lines->Clear();
    // memo2Clear = true;
}

// ---------------------------------------------------------------------------

void __fastcall TForm1::secondaryCo(TObject *Sender) {
    SaveDialog1->InitialDir = "c:\\Sound Analysis";
    SaveDialog1->DefaultExt = "TXT";
    Beep();
    SaveDialog1->Title = "Please choose a name for records file";
    if (SaveDialog1->Execute()) {
        Beep();
        // Memo1->Lines->SaveToFile(SaveDialog1->FileName);
    }
}

// ---------------------------------------------------------------------------

void __fastcall TForm1::SaveIntervalsBClick(TObject *Sender) {
    SaveDialog1->InitialDir = "c:\\Sound Analysis";
    SaveDialog1->DefaultExt = "TXT";
    Beep();
    SaveDialog1->Title = "Please choose a name for records file";
    if (SaveDialog1->Execute()) {
        Beep();
        // Memo2->Lines->SaveToFile(SaveDialog1->FileName);
    }
}

// ---------------------------------------------------------------------------

void __fastcall TForm1::SaveB2Click(TObject *Sender) {
	Active = ksong2;
	SaveSonogram();
}

// ---------------------------------------------------------------------------

void __fastcall TForm1::DrawBClick(TObject *Sender) {
    AnsiString axis_names[3];

    float **FS_vars;
    float *Amplitude, *Pitch, *Entropy, *FM, *AM, *PeakFreq, *Pgoodness,
    *principal_freq, *principal_CF, *cont_t, *cont_f;
    int marker, onset, offset;
    if (!SelectSound->ItemIndex) // song1
    {
        onset = OnsetSong1;
        offset = OffsetSong1;
        Amplitude = Amplitude1;
        Pitch = PitchS1;
        Entropy = EntropyS1;
        FM = FMS1;
        AM = AMS1;
        PeakFreq = PeakFreq1;
        Pgoodness = PitchGoodnessS1;
        principal_freq = principal_frequency1;
        cont_t = continuity_t1;
        cont_f = continuity_f1;
    }
    else // song2
    {
        onset = OnsetSong2;
        offset = OffsetSong2;
        Amplitude = Amplitude2;
        Pitch = PitchS2;
        Entropy = EntropyS2;
        FM = FMS2;
        AM = AMS2;
        PeakFreq = PeakFreq2;
        Pgoodness = PitchGoodnessS2;
        principal_freq = principal_frequency2;
        cont_t = continuity_t2;
        cont_f = continuity_f2;
    }

    int interval = abs(offset - onset);
    // memory allocation
    FS_vars = new float*[3];
    for (int i = 0; i < 3; i++)
        FS_vars[i] = new float[interval];

    featuresBox1->Visible = false;
    featuresBox2->Visible = false;
    ChoiseBox->Visible = false;
    marker = MarkSel->Selected;
    feature_space->ColorData = ColorBox->Selected;

    if (DrawAll->Down == true) // draw ms features
    {
        // first set pointer addresses for the three 3D vars
        int axis = 0;

        if (FS_amp->Checked && axis < 3) {
            for (int i = 0; i < interval; i++)
                FS_vars[axis][i] = Amplitude[onset + i];
            axis_names[axis] = "Amplitude";
            axis++;
        }
        if (FS_pitch->Checked && axis < 3) {
            for (int i = 0; i < interval; i++)
                FS_vars[axis][i] = Pitch[onset + i];
            axis_names[axis] = "Pitch";
            axis++;
        }
        if (FS_meanFr->Checked && axis < 3) {
            for (int i = 0; i < interval; i++)
                FS_vars[axis][i] = PeakFreq[onset + i];
            axis_names[axis] = "Mean Frequency";
            axis++;
        }
        if (FS_principalCF->Checked && axis < 3) {
            for (int i = 0; i < interval; i++)
                FS_vars[axis][i] = principal_freq[onset + i];
            axis_names[axis] = "Principal Contour Frequency";
            axis++;
        }
        if (FS_goodness->Checked && axis < 3) {
            for (int i = 0; i < interval; i++)
                FS_vars[axis][i] = Pgoodness[onset + i];
            axis_names[axis] = "Goodness of pitch";
            axis++;
        }
        if (FS_FM->Checked && axis < 3) {
            for (int i = 0; i < interval; i++)
                FS_vars[axis][i] = FM[onset + i];
            axis_names[axis] = "FM";
            axis++;
        }
        if (FS_slope->Checked && axis < 3) {
            for (int i = 0; i < interval; i++)
                FS_vars[axis][i] = FM[onset + i];
            axis_names[axis] = "Slope";
            axis++;
        }
        if (FS_AM->Checked && axis < 3) {
            for (int i = 0; i < interval; i++)
                FS_vars[axis][i] = AM[onset + i];
            axis_names[axis] = "AM";
            axis++;
        }
        if (FS_entropy->Checked && axis < 3) {
            for (int i = 0; i < interval; i++)
                FS_vars[axis][i] = Entropy[onset + i];
            axis_names[axis] = "Wiener entropy";
            axis++;
        }
        if (FS_continuity_t->Checked && axis < 3) {
            for (int i = 0; i < interval; i++)
                FS_vars[axis][i] = cont_t[onset + i];
            axis_names[axis] = "Continuity over time";
            axis++;
        }
        if (FS_continuity_fr->Checked && axis < 3) {
            for (int i = 0; i < interval; i++)
                FS_vars[axis][i] = cont_f[onset + i];
            axis_names[axis] = "Continuity over frequency";
            axis++;
        }

        if (axis < 2) {
            MessageDlg(
                "you selected less than 3 variables, please check 3 and try again"
                , mtError, TMsgDlgButtons() << mbOK, 0);
            return;
        }

        feature_space->AxNameX = axis_names[0];
        feature_space->AxNameY = axis_names[1];
        feature_space->AxNameZ = axis_names[2];

        for (int i = 0; i < interval; i++) {
            if (FS_plot_options->ItemIndex > 0)
                feature_space->MarkAt(FS_vars[0][i], FS_vars[1][i],
                FS_vars[2][i], marker);
            if (FS_plot_options->ItemIndex == 0 ||
                FS_plot_options->ItemIndex == 2) {
                if (i == onset)
                    feature_space->Moveto(FS_vars[0][i], FS_vars[1][i],
                    FS_vars[2][i]);
                else
                    feature_space->Drawto(FS_vars[0][i], FS_vars[1][i],
                    FS_vars[2][i]);
            }
        }

    } // end if DrawAll

    else if (!interval1 && !interval2)
        Application->MessageBox(L"Please Outline a sound and try again", NULL,
        MB_OK);
    else {
        // calc_meanPeriod_and_meanEuclideanDist(ksong1);
        float plot[3];
        AnsiString gLable[3];
        int i = 0;
        do {
            // mean features
            if (pitchFS->Checked == true) {
                gLable[i] = "Mean Pitch";
                plot[i++] = pitchAvgL->Value;
            }
            if (FMFS->Checked == true) {
                gLable[i] = "Mean FM";
                plot[i++] = FMAvgL->Value;
            }
            if (entropyFS->Checked == true) {
                gLable[i] = "Mean Entropy";
                plot[i++] = entAvgL->Value;
            }
            if (pgoodFS->Checked == true) {
                gLable[i] = "Mean Pitch goodness";
                plot[i++] = pgoodAvgL->Value;
            }
            // feature diversity
            if (pitchDVFS->Checked == true) {
                gLable[i] = "Pitch diversity";
                plot[i++] = sdPitch;
            }
            if (FMDVFS->Checked == true) {
                gLable[i] = "FM diversity";
                plot[i++] = sdEntropy;
            }
            if (entropyDVFS->Checked == true) {
                gLable[i] = "Entropy diversity";
                plot[i++] = sdModul;
            }
            if (continuityDVFS->Checked == true) {
                gLable[i] = "Continuity diversity";
                plot[i++] = sdCont;
            }
            if (OverallDVFS->Checked == true) {
                gLable[i] = "Overall diversity";
                plot[i++] = sdPitch / PitchDV + sdEntropy / EntropyDV +
                    sdModul / FMDV + sdCont / ContDV;
            }

            // periodicity
            if (pitchPrFS->Checked == true) {
                gLable[i] = "Pitch periodicity";
                plot[i++] = FeaturePeriodicity[0];
            }
            if (entropyPrFS->Checked == true) {
                gLable[i] = "Entropy periodicity";
                plot[i++] = FeaturePeriodicity[1];
            }
            if (FMPrFS->Checked == true) {
                gLable[i] = "FM periodicity";
                plot[i++] = FeaturePeriodicity[2];
            }
            if (contPrFS->Checked == true) {
                gLable[i] = "Continuity periodicity";
                plot[i++] = FeaturePeriodicity[3];
            }
            if (overallPrFS->Checked == true) {
                gLable[i] = "Overall periodicity";
                plot[i++] = (FeaturePeriodicity[0] + FeaturePeriodicity[1]
                    + FeaturePeriodicity[2] + FeaturePeriodicity[3]) / 4;
            }
            // KS statistic
            if (pitchKSFS->Checked == true) {
                gLable[i] = "Pitch KS";
                plot[i++] = KSval[0];
            }
            if (entropyKSFS->Checked == true) {
                gLable[i] = "Entropy KS";
                plot[i++] = KSval[1];
            }
            if (FMKSFS->Checked == true) {
                gLable[i] = "FM KS";
                plot[i++] = KSval[2];
            }
            if (contKSFS->Checked == true) {
                gLable[i] = "Continuity KS";
                plot[i++] = KSval[3];
            }
            if (overallKSFS->Checked == true) {
                gLable[i] = "Overall KS";
                plot[i++] = KSval[4];
            }
            // Period
            if (periodFS->Checked == true) {
                gLable[i] = "Period";
                plot[i++] = PeriodVal;
            }
            // information
            if (informationFS->Checked == true) {
                gLable[i] = "Information";
                plot[i++] = InfoVal;
            }
            else
                i = 4;
        }
        while (i < 3);
        feature_space->AxNameX = gLable[0];
        feature_space->AxNameY = gLable[1];
        feature_space->AxNameZ = gLable[2];

        rx = 10 * plot[0];
        ry = 10 * plot[1];
        rz = 10 * plot[2];
        if (FS_plot_options->ItemIndex > 0)
            feature_space->MarkAt(rx, ry, rz, marker);
        if (FS_plot_options->ItemIndex == 0 || FS_plot_options->ItemIndex == 2)
        {
            if (ResetTheLine == true) {
                ResetTheLine = false;
                feature_space->Moveto(rx, ry, rz);
            }
            else
                feature_space->Drawto(rx, ry, rz);

        }
    }

    feature_space->MakeVisible();
    for (int i = 0; i < 3; i++)
        delete[]FS_vars[i];
    delete[]FS_vars;

}
// ---------------------------------------------------------------------------

void __fastcall TForm1::ClearBClick(TObject *Sender) {
    ChoiseBox->Visible = true;
    if (DrawAll->Down) {
        featuresBox1->Visible = true;
        featuresBox2->Visible = false;
    }
    else {
        featuresBox1->Visible = false;
        featuresBox2->Visible = true;
    }
    feature_space->Clear();
    feature_space->ViewAngleX += 5;
    feature_space->ViewAngleX -= 5;
    feature_space->MakeVisible();
    feature_space->Clear();
    feature_space->ViewAngleX += 5;
    feature_space->ViewAngleX -= 5;
    feature_space->MakeVisible();
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::printBClick(TObject *Sender) {
    // feature_space->Printit(1,false);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn1Click(TObject *Sender) {
   // SaveGraph->InitialDir = "C:\\Sound Analysis";
    SaveGraph->Title = "choose a name for 3D features image";
    if (SaveGraph->Execute())
        feature_space->CopyToBMP(SaveGraph->FileName, 0);
    // feature_space->CopyToBMP ("c:\\SongScore\\Bitmap");
    Beep();
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::SimilaritySectionChange(TObject *Sender) {
    SectionTab->Value = SimilaritySection->Position * option->advanceTb->Value;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::displayChange(TObject *Sender) {
    // if(display->ActivePage==Song1)option->hp_amp=fine_segmentation->Down;
    // else if(display->ActivePage==Song2)option->hp_amp=fine_segmentation2->Down;

    if (display->ActivePage == Song2) {

        first_snd = false;
        if (!ready2)
            new2BClick(this);
    }

    if (display->ActivePage == Combined) {
		Image6->AutoSize=true;
		Image6->Picture = Image1->Picture;
		Image6->AutoSize=false;
		Image6->Stretch=true;
		Image6->Height=175;

		Image7->AutoSize=true;
		Image7->Picture = Image2->Picture;
		Image7->AutoSize=false;
		Image7->Stretch=true;
		Image7->Height=175;
    }

    if (display->ActivePage == Similarity) {
        similarity_control->ActivePageIndex = 0;
        if (update_score_sounds)
            draw_score_sounds();
        update_score_sounds = false;
    }

    if (display->ActivePage == Features_space) {
        if (SelectSound->ItemIndex)
            Image8->Picture = Image2->Picture;
        else
            Image8->Picture = Image1->Picture;
    }
}

// ---------------------------------------------------------------------------
float TForm1::KS(TVector *v1, TVector *v2) {
    int j1 = 1, j2 = 1;
    float ks = 0, d1, d2, dt, en1, en2, fn1 = 0.0, fn2 = 0.0;
    en1 = ind1;
    en2 = ind2;
    while (j1 <= ind1 && j2 <= ind2) { // If we are not done...
        if ((d1 = v1->Elem[j1]) <= (d2 = v2->Elem[j2]))
            fn1 = j1++ / en1; // Next step is in data1.
        if (d2 <= d1)
            fn2 = j2++ / en2; // Next step is in data2.
        if ((dt = fabs(fn2 - fn1)) > ks)
            ks = dt;
    }
    return(ks);
}

float TForm1::KSs(float *data1, float *data2, int start1, int end1, int start2,
    int end2) {
    int size1 = 0, size2 = 0, j1 = 0, j2 = 0;
    double ks = 0, d1, d2, dt, fn1 = 0.0, fn2 = 0.0, duration1 = end1 - start1,
    duration2 = end2 - start2;

    vector<float>v1;
    for (int i = 0; i < duration1; i++)
        if (Signal1[start1 + i]) {
            v1.push_back(data1[start1 + i]);
            size1++;
        }
    sort(v1.begin(), v1.end());

    vector<float>v2;
    for (int i = 0; i < duration2; i++)
        if (Signal2[start2 + i]) {
            v2.push_back(data2[start2 + i]);
            size2++;
        }
    sort(v2.begin(), v2.end());

    if (size1 < 5 || size2 < 5) {
		//ks_panel->Visible = false;
        return(0);
    }

    while (j1 < size1 && j2 < size2) { // If we are not done...
        if ((d1 = v1[j1]) <= (d2 = v2[j2]))
            fn1 = j1++ / (double)max(size1, 1); // Next step is in data1.
        if (d2 <= d1)
            fn2 = j2++ / (double)max(size2, 1); // Next step is in data2.
        if ((dt = fabs(fn2 - fn1)) > ks)
            ks = dt;
    }
    v1.erase(v1.begin(), v1.end());
    v2.erase(v2.begin(), v2.end());
    return(ks);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::TBperThreshChange(TObject *Sender) {
    /* float x=0.06*Form5->TBperThresh->Position;
    Form5->simL->Value=0.0018*pow(x,3)-0.2342*pow(x,2)+10.583*x-69.21;
     */
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::optionsClick(TObject *Sender) {
    option->Visible = true;
    option->BringToFront();
    option->show_start = false;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::DrawAllClick(TObject *Sender) {
    featuresBox1->Visible = true;
    featuresBox2->Visible = false;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::GlobalPlotBClick(TObject *Sender) {
    featuresBox1->Visible = false;
    featuresBox2->Visible = true;
    // CalcMode->ItemIndex=2;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::PitchGoodnessBClick(TObject *Sender) {

    TSpeedButton *goodness_btn, *group_btns;
    TImage *image;
    TRadioGroup *sono_display;
    CZT_FeatureCalc *calci;
    bool the_song, paginate;
    bool smooth_feature;
    int onset, offset;
    if (Sender == PitchGoodnessB || display->ActivePage == Song1) {
        goodness_btn = PitchGoodnessB;
        the_song = Ksong1;
        calci = &m_Calculator1;
		group_btns = group;
		paginate = option->paginationOn;
		smooth_feature = option->smooth_feature1;
		image = Image1;
		sono_display = deriv_display;
		onset = OnsetSong1;
		offset = OffsetSong1;
	}
	else if (Sender == PitchGoodnessB2 || display->ActivePage == Song2) {
		goodness_btn = PitchGoodnessB2;
		the_song = Ksong2;
		calci = &m_Calculator2;
		group_btns = group;
		paginate = option->paginationOn2;
		smooth_feature = option->smooth_feature2;
		image = Image2;
		sono_display = deriv_display2;
		onset = OnsetSong2;
		offset = OffsetSong2;
	}

	if (goodness_btn->Down)
		set_feature_col_highlight(6, the_song);
	else
		set_feature_col_highlight(0, the_song);
	if (!group_btns->Down)
		hide_all_features(calci);


	if (sono_display->ItemIndex == 0)
	{
		calci->GetImage().GetPitchGoodness().bRender = goodness_btn->Down;
		if (smooth_feature)	calci->GetImage().Gethp_small().bRender = goodness_btn->Down;
		if (!paginate)calci->GetImage().DrawAllSlices(image);
		else calci->GetImage().DrawSlices(0, 1000 - 1, image);
	}
	else if (sono_display->ItemIndex == 1)
	{
		calci->GetContours().GetPitchGoodness().bRender = goodness_btn->Down;
		if (smooth_feature)	calci->GetContours().Gethp_small().bRender = goodness_btn->Down;
		if (!paginate)calci->GetContours().DrawAllSlices(image);
		else calci->GetContours().DrawSlices(0, 1000 - 1, image);
	}
	else if (sono_display->ItemIndex == 2)
	{
		calci->GetSonogram().GetPitchGoodness().bRender = goodness_btn->Down;
		if (smooth_feature)	calci->GetSonogram().Gethp_small().bRender = goodness_btn->Down;
		if (!paginate)calci->GetSonogram().DrawAllSlices(image);
		else calci->GetSonogram().DrawSlices(0, 1000 - 1, image);
	}
	set_scale(the_song);
	show_outlines(image, onset, offset);
	option->doNotSegment = true;

}
// ---------------------------------------------------------------------------

void __fastcall TForm1::SelectSoundClick(TObject *Sender) {
    // for feature space selection of song
    if (SelectSound->ItemIndex) {
        Image8->Picture = Image2->Picture;
        Image8->OnMouseDown = Image2MouseDown;
    }
    else {
        Image8->Picture = Image1->Picture;
        Image8->OnMouseDown = Image1MouseDown;
	}
}

void __fastcall TForm1::derivTBChange(TObject *Sender) {
	TFrame1* frame;
	TSpeedButton *derivPowB;
	TTrackBar *deriv_TB, *ent_TB;
	TNumLab *frequency_val;
	TImage *image;
	TNumLab *deriv_vl;
	CZT_FeatureCalc *calci;
	TLabel *label;
	TLabel *label2;
	TRadioGroup *displaySnd;

	bool song;
	bool *pagination_On;
	bool *adapt_threshold;
	bool *smooth_graph;
	bool *sec_segmentation;
	bool *reverse_segmentation;
	float *pri_thresh_raw;
	int *pri_segmented_feature;
	int *sec_segmented_feature;
	parameters_altered = true;
	if(CalcMode->ItemIndex == 2){
		update_multisegment->Enabled=true;
		update_segment2->Enabled=true;
		Addrec2_alt->Enabled=false;
		addBtn_alt->Enabled=false;
	}
	int iter, onset, offset;
	/*doNotSegment really means not to show feature graph/smoothed graph/primary and secondary threshold
	 (anything associated with segmentation screen)  If you want to turn off showing of all these
	 segmentation related staff, you un-select button and turn it off.  Then as you go forward or
	 backward in the file, the graphs are not shown (segmentation is still there).  */
	if (Sender == derivTB || updateSegmentation || first_snd) {
		option->pri_segmented_feature = option->pri_segmented_feature1;
		option->pri_thresh_raw = option->pri_thresh_raw1;
		option->reverse_segmentation =  option->reverse_segmentation1;
		option->smooth_feature = option->smooth_feature1;
		option->adapt_threshold = option->adapt_threshold1;
		option->sec_segmentation = option->sec_segmentation1;
		option->sec_segmented_feature = option->sec_segmented_feature1;
		option->sec_thresh_raw =  option->sec_thresh_raw1;
		option->sec_lt = option->sec_lt1;
		option->sec_logic =  option->sec_logic1;
		option->hp_small = option->hp_small1;
		option->hp_big = option->hp_big1;

		frame = Frame11;
		song = 0;
		derivPowB = DerivPowerB;
		deriv_TB = derivTB;
		calci = &m_Calculator1;
		pagination_On = &option->paginationOn;
		adapt_threshold = &option->adapt_threshold1;
		smooth_graph = &option->smooth_feature1;
		sec_segmentation = &option->sec_segmentation1;
		reverse_segmentation = &option->reverse_segmentation1;
		pri_thresh_raw = &option->pri_thresh_raw1;
		pri_segmented_feature = &option->pri_segmented_feature1;
		sec_segmented_feature = &option->sec_segmented_feature1;
		iter = Iterations1;
		image = Image1;
		deriv_vl = deriv_val;
		onset = OnsetSong1;
		offset = OffsetSong1;
		label = Label5;
		label2 = ent_label;
		frequency_val = frequency_range_val;
		ent_TB = entropyTb;
		displaySnd=deriv_display;
		DataForm->Settings(1, "deriv_thresh", deriv_TB->Position);
		if (*sec_segmentation)
		   DataForm->Settings(1, "entropy_thresh", ent_TB->Position);

	} else if (Sender == derivTB2 || updateSegmentation2 || !first_snd) {
		option->pri_segmented_feature = option->pri_segmented_feature2;
		option->pri_thresh_raw = option->pri_thresh_raw2;
		option->reverse_segmentation =  option->reverse_segmentation2;
		option->smooth_feature = option->smooth_feature2;
		option->adapt_threshold = option->adapt_threshold2;
		option->sec_segmentation = option->sec_segmentation2;
		option->sec_segmented_feature = option->sec_segmented_feature2;
		option->sec_thresh_raw =  option->sec_thresh_raw2;
		option->sec_lt = option->sec_lt2;
		option->sec_logic =  option->sec_logic2;
		option->hp_small = option->hp_small2;
		option->hp_big = option->hp_big2;

		frame = Frame12;
		song = 1;
		derivPowB = DerivPower2B;
		deriv_TB = derivTB2;
		calci = &m_Calculator2;
		pagination_On = &option->paginationOn2;
		adapt_threshold = &option->adapt_threshold2;
		smooth_graph = &option->smooth_feature2;
		sec_segmentation = &option->sec_segmentation2;
		reverse_segmentation = &option->reverse_segmentation2;
		pri_thresh_raw = &option->pri_thresh_raw2;
		pri_segmented_feature = &option->pri_segmented_feature2;
		sec_segmented_feature = &option->sec_segmented_feature2;
		iter = Iterations2;
		image = Image2;
		deriv_vl = deriv_val2;
		onset = OnsetSong2;
		offset = OffsetSong2;
		label = Label11;
		label2 = ent_label2;
		frequency_val = frequency_range2_val;
		ent_TB = entropyTb2;
		displaySnd=deriv_display2;
		DataForm->Settings(2, "deriv_thresh", deriv_TB->Position);
		if (*sec_segmentation)
		   DataForm->Settings(2, "entropy_thresh", ent_TB->Position);

	}
	hide_all_features(calci);

	int pos = 20 + deriv_TB->Position * 9 / 5;
	int feature = frame->segment_by->ItemIndex;

	switch(feature) {
	case 0:
		label->Caption = "dB";
		option->Amplitude_thresh->Position = deriv_TB->Position; // we have the same slider in options screen and we make it the same
		derivPowB->Down = true;
		calci->GetImage().GetDerivPow().bRender = true;
		break;
	case 1:
		// ft=m_Pitch;
		label->Caption = "Hz";
		calci->GetImage().GetPitch().bRender = true;
		break;
	case 2:
		// ft=m_mfa;
		label->Caption = "Hz";
		PowerB->Down = true;
		calci->GetImage().GetPeakFreq().bRender = true;
		break;
	case 3:
		label->Caption = "";
		PitchGoodnessB->Down = true;
		calci->GetImage().GetPitchGoodness().bRender = true;
		break;
	case 4:
		label->Caption = "deg";
		FMB->Down = true;
		calci->GetImage().GetFM().bRender = true;

		break;
	case 5:
		label->Caption = "ms";
		AMB->Down = true;
		calci->GetImage().GetAM().bRender = true;
		break;
	case 6:
		label->Caption = "";
		EntropyB->Down = true;
		calci->GetImage().GetEntropy().bRender = true;
		break;
	case 7:
		label->Caption = "ms";
		continulity_tB->Down = true;
		calci->GetImage().Getcontinuity_t().bRender = true;
		break;
	case 8:
		label->Caption = "Hz";
		continuity_fB->Down = true;
		calci->GetImage().Getcontinuity_f().bRender = true;
		break;
	default:
		label->Caption = "dB";
		option->Amplitude_thresh->Position = deriv_TB->Position; // we have the same slider in options screen and we make it the same
		derivPowB->Down = true;
		calci->GetImage().GetDerivPow().bRender = true;
	}


	if (*sec_segmentation) {
		ent_label->Visible = true;
		ent_label2->Visible = true;
		int featureSec = frame->secondaryFeatureDD->ItemIndex;

		switch(featureSec) {
		case 0:
			label2->Caption = "dB";
			DerivPowerB->Down = true;
			calci->GetImage().GetDerivPow().bRender = true;
			break;
		case 1:
			label2->Caption = "Hz";
			PitchB->Down = true;
			calci->GetImage().GetPitch().bRender = true;
			break;
		case 2:
            label2->Caption = "Hz";
			PowerB->Down = true;
			calci->GetImage().GetPeakFreq().bRender = true;
			break;
		case 3:
            label2->Caption = "";
			PitchGoodnessB->Down = true;
			calci->GetImage().GetPitchGoodness().bRender = true;
			break;
		case 4:
			label2->Caption = "deg";
			FMB->Down = true;
			calci->GetImage().GetFM().bRender = true;
			break;
		case 5:
            label2->Caption = "ms";
			AMB->Down = true;
			calci->GetImage().GetAM().bRender = true;
			break;
		case 6:
            label2->Caption = "";
			EntropyB->Down = true;
			calci->GetImage().GetEntropy().bRender = true;
			break;
		case 7:
			label2->Caption = "ms";
			continulity_tB->Down = true;
			calci->GetImage().Getcontinuity_t().bRender = true;
			break;
		case 8:
            label2->Caption = "Hz";
			continuity_fB->Down = true;
			calci->GetImage().Getcontinuity_f().bRender = true;
			break;
		default:
            label2->Caption = "dB";
			DerivPowerB->Down = true;
			calci->GetImage().GetDerivPow().bRender = true;
		}
	} else {
	  ent_label->Visible = false;
      ent_label2->Visible = false;
    }

	*pri_segmented_feature = feature;

	// Changed
	option->frequency = frequency_val->Value;
	*pri_thresh_raw = deriv_TB->Position;
	deriv_vl->Value = option->adjustThreshold(feature, deriv_TB->Position, frequency_val->Value, 1);
	*reverse_segmentation = frame->segmentation_flip->ItemIndex;
	option->doNotSegment = false;
	frame->NumLab3->Value = deriv_vl->Value;

	if (*smooth_graph || *adapt_threshold) {
		calci->do_hpfilt();
		calci->Render_sonogram_images();
		if (*adapt_threshold) switch(displaySnd->ItemIndex) {
			case 0: calci->GetImage().Gethp_big().bRender = true; break;
			case 1: calci->GetContours().Gethp_big().bRender = true; break;
			case 2: calci->GetSonogram().Gethp_big().bRender = true; break;
		}
		if (*smooth_graph) switch(displaySnd->ItemIndex) {
			case 0: calci->GetImage().Gethp_small().bRender = true; break;
			case 1: calci->GetContours().Gethp_small().bRender = true; break;
			case 2: calci->GetSonogram().Gethp_small().bRender = true; break;
		}
	}

  	Segment_Sound(0, iter, song); // we now update the signal accordingly-

	switch(displaySnd->ItemIndex) {
    case 0: calci->GetImage().DrawAllSlices(image); break;
				case 1: calci->GetContours().DrawAllSlices(image); break;
				case 2: calci->GetSonogram().DrawAllSlices(image); break;
	}


	set_scale(song);
	if (!(*adapt_threshold)) {
		image->Canvas->Pen->Color = clYellow;
		image->Canvas->MoveTo(0, pos);
		image->Canvas->LineTo(image->Width, pos);
		//show_outlines(image, onset, offset);
	}
	if (*sec_segmentation) {
		image->Canvas->Pen->Color = clAqua; // (TColor)RGB(0, 0, 0);
		int posSec = 20 + ent_TB->Position * 9 / 5;
		image->Canvas->MoveTo(0, posSec);
		image->Canvas->LineTo(image->Width, posSec);
		//show_outlines(image, onset, offset);
	}
    //show_outlines(image, onset, offset);
	updateSegmentation = false;
	updateSegmentation2 = false;
}

// ---------------------------------------------------------------------------

void __fastcall TForm1::sldrBrightnessChange(TObject *Sender) {
	if(deriv_display->ItemIndex==1)return;
	auto_gain->Checked = false;
	contrast1->Value = sldrBrightness->Position / 100;
	float bright = set_brightness(auto_gain->Checked, sldrBrightness->Position,
		deriv_display->ItemIndex ? 0 : 1);
	if(deriv_display->ItemIndex==2)
	{
	m_Calculator1.GetSonogram().SetBrightness(bright);
	m_Calculator1.GetSonogram().DrawAllSlices(Image1);
	}
	else if(deriv_display->ItemIndex==0)
	{
	m_Calculator1.GetImage().SetBrightness(bright);
	m_Calculator1.GetImage().DrawAllSlices(Image1);
	}
	set_scale(0);
    option->sldrBrightness->Position = sldrBrightness->Position;
    show_outlines(Image1, OnsetSong1, OffsetSong1);
	DataForm->Settings(1, "display_contrast", sldrBrightness->Position);
}

// ---------------------------------------------------------------------------

float TForm1::set_brightness(bool autobright, int slider_pos,
    bool deriv_display) {
    float brightness;
    if (autobright) // case -- auto brightmess
    {
        if (deriv_display)
            brightness = m_Calculator1.Maximum / 100000000000.0;
        else
            brightness = m_Calculator1.Maximum / 50000000000.0;
        if (brightness < 1)
            brightness = 1;
    }
    else // case -- brightness by slider:
    {
        if (deriv_display)
            brightness = exp((float)slider_pos / 1500.0);
        else
            brightness = exp(((float)slider_pos + 3000.0) / 2000.0);
    }
    return(brightness);
}

// ---------------------------------------------------------------------------

void __fastcall TForm1::entropyTbChange(TObject *Sender) {
     TFrame1 *frame;
     TTrackBar *ent_TB;
     float *sec_thresh_raw;
     TNumLab *frequency_val;
     TNumLab *ent_vl;
     TLabel *label;
     TImage *image;
     bool *update_sec, *update_seg;

    if (Sender == entropyTb || updateSegmentationSec || first_snd) {
		update_sec = &updateSegmentationSec;
		update_seg = &updateSegmentation;
		frame = Frame11;
		ent_TB = entropyTb;
		ent_vl = entropy_val;
		frequency_val = frequency_range_val;
		sec_thresh_raw = &option->sec_thresh_raw1;
        image = Image1;
	} else if (Sender == entropyTb2 || updateSegmentationSec2 || !first_snd) {
		update_sec = &updateSegmentationSec2;
		update_seg = &updateSegmentation2;
		frame = Frame12;
		ent_TB = entropyTb2;
		ent_vl = entropy_val2;
		frequency_val = frequency_range2_val;
		sec_thresh_raw = &option->sec_thresh_raw2;
        image = Image2;
	}

    option->sec_segmented_feature = frame->secondaryFeatureDD->ItemIndex;
	int feature = option->sec_segmented_feature;
	option->doNotSegment = false;

    if (Sender == entropyTb || updateSegmentationSec || first_snd)
       option->sec_segmented_feature1 = option->sec_segmented_feature;
    else
        option->sec_segmented_feature2 = option->sec_segmented_feature;

	*sec_thresh_raw = ent_TB->Position;
	option->frequency = frequency_val->Value;
	ent_vl->Value = option->adjustThreshold(feature, ent_TB->Position, option->frequency, 0);
	frame->secondaryThresholdTB->Value = ent_vl->Value;


	*update_sec = false;
	*update_seg = true;
	derivTBChange(this);
}
// ---------------------------------------------------------------------------

void TForm1::show_outlines(TImage* image, int onset, int offset) {
	if (onset < 2)
		return;
	image->Canvas->Pen->Color = clRed;
	image->Canvas->MoveTo(onset, 0);
	image->Canvas->LineTo(onset, 240);
	image->Canvas->Pen->Color = clBlue;
	image->Canvas->MoveTo(offset, 0);
	image->Canvas->LineTo(offset, 240);
}

void __fastcall TForm1::stretchClick(TObject *Sender) {
    TImage *image;
    TRadioGroup *fit;
    if (Sender == stretch) {
        image = Image1;
        fit = stretch;
    }
    else {
        image = Image2;
        fit = stretch2;
    }
    if (fit->ItemIndex) {
        range_up_shading->Width = 1045;
        range_down_shading->Width = 1045;
	   // x_axis->Width = 1045;
        image->AutoSize = true;
        image->Stretch = false;
    }
    else {
	  //  x_axis->Visible = true;
        image->AutoSize = false;
        image->Stretch = true;
        image->Width = ScrollBox1->Width - 10;
    }
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::SaveBClick(TObject *Sender) {
    Active = ksong1;
    SaveSonogram(); // in the other form...
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::sldrBrightness2Change(TObject *Sender) {

    if(deriv_display2->ItemIndex==1)return;
	float bright = exp((float)sldrBrightness2->Position / 1500);
    contrast2->Value = sldrBrightness2->Position / 100;

	if(deriv_display2->ItemIndex==2)
	{
	  m_Calculator2.GetSonogram().SetBrightness(bright);
	  m_Calculator2.GetSonogram().DrawAllSlices(Image2);
	}
	else if(deriv_display2->ItemIndex==0)
	{
	  m_Calculator2.GetImage().SetBrightness(bright);
	  m_Calculator2.GetImage().DrawAllSlices(Image2);
	}
	//m_Calculator2.GetImage().SetBrightness(bright);
	// float(sldrBrightness->Position/39.0625));
	//m_Calculator2.GetImage().DrawAllSlices(Image2);
    show_outlines(Image2, OnsetSong2, OffsetSong2);
    //DataForm->Settings(2, "display_contrast", sldrBrightness2->Position);
}

// -------------------------------------------------------------------------

void TForm1::hide_all_features(CZT_FeatureCalc *cal) {
	cal->GetImage().GetPitch().bRender = false;
	cal->GetImage().GetEntropy().bRender = false;
	cal->GetImage().GetFM().bRender = false;
	cal->GetImage().GetAM().bRender = false;
	cal->GetImage().GetDerivPow().bRender = false;
	cal->GetImage().Getmfa().bRender = false;
	cal->GetImage().GetPitchGoodness().bRender = false;
	cal->GetImage().GetPeakFreq().bRender = false;
	cal->GetImage().Getcepst0().bRender = false;
	cal->GetImage().Getcontinuity_t().bRender = false;
	cal->GetImage().Getcontinuity_f().bRender = false;
	cal->GetImage().Getprincipal_frequency().bRender = false;
	cal->GetImage().Getslope().bRender = false;
	cal->GetImage().Gethp_small().bRender = false;
	cal->GetImage().Gethp_big().bRender = false;
	cal->GetImage().Gethp_diff().bRender = false;
	// cal->GetImage().ResetContents();

	cal->GetSonogram().GetPitch().bRender = false;
	cal->GetSonogram().GetEntropy().bRender = false;
	cal->GetSonogram().GetFM().bRender = false;
	cal->GetSonogram().GetAM().bRender = false;
	cal->GetSonogram().GetDerivPow().bRender = false;
	cal->GetSonogram().Getmfa().bRender = false;
	cal->GetSonogram().GetPitchGoodness().bRender = false;
	cal->GetSonogram().GetPeakFreq().bRender = false;
	cal->GetSonogram().Getcepst0().bRender = false;
	cal->GetSonogram().Getcontinuity_t().bRender = false;
	cal->GetSonogram().Getcontinuity_f().bRender = false;
	cal->GetSonogram().Getprincipal_frequency().bRender = false;
	cal->GetSonogram().Getslope().bRender = false;
	cal->GetSonogram().Gethp_small().bRender = false;
	cal->GetSonogram().Gethp_big().bRender = false;
	cal->GetSonogram().Gethp_diff().bRender = false;


	cal->GetContours().GetPitch().bRender = false;
	cal->GetContours().GetEntropy().bRender = false;
	cal->GetContours().GetFM().bRender = false;
	cal->GetContours().GetAM().bRender = false;
	cal->GetContours().GetDerivPow().bRender = false;
	cal->GetContours().Getmfa().bRender = false;
	cal->GetContours().GetPitchGoodness().bRender = false;
	cal->GetContours().GetPeakFreq().bRender = false;
	cal->GetContours().Getcepst0().bRender = false;
	cal->GetContours().Getcontinuity_t().bRender = false;
	cal->GetContours().Getcontinuity_f().bRender = false;
	cal->GetContours().Getprincipal_frequency().bRender = false;
	cal->GetContours().Getslope().bRender = false;
	cal->GetContours().Gethp_small().bRender = false;
	cal->GetContours().Gethp_big().bRender = false;
	cal->GetContours().Gethp_diff().bRender = false;


}

void __fastcall TForm1::show_song2MouseDown(TObject *Sender,
	TMouseButton Button, TShiftState Shift, int X, int Y) {
	Image7->Left = X;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::ScrollBox4MouseDown(TObject *Sender,
    TMouseButton Button, TShiftState Shift, int X, int Y) {
    Image6->Left = X;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::ScrollBox4MouseMove(TObject *Sender, TShiftState Shift,
    int X, int Y) {
    if (smoothi->Checked)
		Image6->Left = X;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::ScrollBox4DblClick(TObject *Sender) {
    if (smoothi->Checked)
        smoothi->Checked = false;
    else
        smoothi->Checked = true;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::show_song2MouseMove(TObject *Sender, TShiftState Shift,
    int X, int Y) {
    if (smoothi->Checked)
		Image7->Left = X;
}
// ---------------------------------------------------------------------------

void TForm1::Crash() {
    static int toler = 0;
    toler++;
    if (toler > 10) {
        Application->MessageBox(L"Sorry, an error has occurred.", NULL, MB_OK);
        Application->Terminate();
    }
}

// ---------------------------------------------------------------------------

void TForm1::DeleteFinalData1() {
    try {
        /* delete[] Signal1; delete[] DerivPow1;
        delete[] Pitch1;  delete[] FM1;  delete[] Entropy1;  delete[] PitchGoodness1;
        delete[] PitchS1; delete[] FMS1; delete[] EntropyS1; delete[] PitchGoodnessS1;
        delete[] PeakFreq1;delete[] Power1; */ delete Sonogram1;
        delete DerivBM1;
        delete ContoursBM1;
        FinalSong1 = false;
    }
    catch(Exception & exception) {
        Crash();
    }
}

// ---------------------------------------------------------------------------
void TForm1::DeleteFinalData2() {
    try {
        delete Sonogram2;
        delete DerivBM2;
        FinalSong2 = false;
    }
    catch(Exception & exception) {
        Crash();
    }
}

// ---------------------------------------------------------------------------

void TForm1::DeleteScoreData() {
    try {
        if (GUI) {
            delete LocAllBM;
            delete GlobModBM;
            delete CombinedBM;
            delete GlobEntBM;
            delete LocPitchBM;
            delete LocEntBM, delete LocModBM;
            delete GlobPitchBM;
            delete GlobalBM;
            delete LocContBM;
            delete GlobContBM;
            delete LocAMBM;
            delete GlobAMBM;
        }

        imitated = 0;
        inspired = 0;
        silence1 = 0;
        silence2 = 0;
        scrVal = 0;
        scrNum = 0;
        n = 1;
        score = false;
    }
    catch(Exception & exception) {
        Crash();
    }
}

// ---------------------------------------------------------------------------

void TForm1::SetBitmap(Graphics::TBitmap *BM, int W, int H, TColor color) {
    BM->Width = W;
    BM->Height = H;
    BM->PixelFormat = (Graphics::TPixelFormat)7;
    BM->Canvas->Brush->Color = color;
    BM->Canvas->FillRect(Rect(0, 0, W, H));
}

// ---------------------------------------------------------------------------

void __fastcall TForm1::EntropyBClick(TObject *Sender) {
    TSpeedButton *entropy_btn, *group_btns;
    TImage *image;
    TRadioGroup *sono_display;
    CZT_FeatureCalc *calci;
    bool the_song, paginate, smooth_feature;
    int onset, offset;
    if (Sender == EntropyB || display->ActivePage == Song1) {
        entropy_btn = EntropyB;
        the_song = Ksong1;
        calci = &m_Calculator1;
        group_btns = group;
        paginate = option->paginationOn;
        smooth_feature = option->smooth_feature1;
        image = Image1;
        sono_display = deriv_display;
        onset = OnsetSong1;
        offset = OffsetSong1;
    }
    else if (Sender == Entropy2B || display->ActivePage == Song2) {
        entropy_btn = Entropy2B;
        the_song = Ksong2;
        calci = &m_Calculator2;
        group_btns = group2;
        paginate = option->paginationOn2;
        smooth_feature = option->smooth_feature2;
        image = Image2;
        sono_display = deriv_display2;
        onset = OnsetSong2;
        offset = OffsetSong2;
    }

    if (entropy_btn->Down)
        set_feature_col_highlight(9, the_song);
    else
        set_feature_col_highlight(0, the_song);
    if (!group_btns->Down)
        hide_all_features(calci);

	if (sono_display->ItemIndex == 0)
	{
		calci->GetImage().GetEntropy().bRender = entropy_btn->Down;
		if (smooth_feature)calci->GetImage().Gethp_small().bRender = EntropyB->Down;
		if (!paginate)calci->GetImage().DrawAllSlices(image);
		else calci->GetImage().DrawSlices(0, 1000 - 1, image);
	}
	else if (sono_display->ItemIndex == 1)
	{
		calci->GetContours().GetEntropy().bRender = entropy_btn->Down;
		if (smooth_feature)calci->GetContours().Gethp_small().bRender = EntropyB->Down;
		if (!paginate)calci->GetContours().DrawAllSlices(image);
		else calci->GetContours().DrawSlices(0, 1000 - 1, image);
	}
	else if (sono_display->ItemIndex == 2)
	{
		calci->GetSonogram().GetEntropy().bRender = entropy_btn->Down;
		if (smooth_feature)calci->GetSonogram().Gethp_small().bRender = EntropyB->Down;
		if (!paginate)calci->GetSonogram().DrawAllSlices(image);
		else calci->GetSonogram().DrawSlices(0, 1000 - 1, image);
	}
	set_scale(the_song);
	show_outlines(image, onset, offset);
    // show_outlines(Image1, OnsetSong1, OffsetSong1);
    option->doNotSegment = true;
}

// ---------------------------------------------------------------------------

void __fastcall TForm1::PitchBClick(TObject *Sender) {
    TSpeedButton *pitch_btn, *group_btns;
    TImage *image;
    TRadioGroup *sono_display;
    CZT_FeatureCalc *calci;
    bool the_song, paginate, smooth_feature;
    int onset, offset;
    if (Sender == PitchB || display->ActivePage == Song1) {
        pitch_btn = PitchB;
        the_song = Ksong1;
        calci = &m_Calculator1;
        group_btns = group;
        paginate = option->paginationOn;
        smooth_feature = option->smooth_feature1;
        image = Image1;
        sono_display = deriv_display;
        onset = OnsetSong1;
        offset = OffsetSong1;
    }
    else if (Sender == Pitch2B || display->ActivePage == Song2) {
        pitch_btn = Pitch2B;
        the_song = Ksong2;
        calci = &m_Calculator2;
        group_btns = group2;
        paginate = option->paginationOn2;
        smooth_feature = option->smooth_feature2;
        image = Image2;
        sono_display = deriv_display2;
        onset = OnsetSong2;
        offset = OffsetSong2;
    }

    if (pitch_btn->Down)
        set_feature_col_highlight(3, the_song);
    else
        set_feature_col_highlight(0, the_song);
    if (!group_btns->Down)
        hide_all_features(calci);


	if (sono_display->ItemIndex == 0)
	{
		calci->GetImage().GetPitch().bRender = pitch_btn->Down;
		if (smooth_feature) calci->GetImage().Gethp_small().bRender = pitch_btn->Down;
		if (!paginate) calci->GetImage().DrawAllSlices(image);
		else calci->GetImage().DrawSlices(0, 1000 - 1, image);
	}

	else if (sono_display->ItemIndex == 1)
	{
		calci->GetContours().GetPitch().bRender = pitch_btn->Down;
		if (smooth_feature) calci->GetContours().Gethp_small().bRender = pitch_btn->Down;
		if (!paginate) calci->GetContours().DrawAllSlices(image);
		else calci->GetContours().DrawSlices(0, 1000 - 1, image);
	}

	else if (sono_display->ItemIndex == 2)
	{
		calci->GetSonogram().GetPitch().bRender = pitch_btn->Down;
		if (smooth_feature) calci->GetSonogram().Gethp_small().bRender = pitch_btn->Down;
		if (!paginate) calci->GetSonogram().DrawAllSlices(image);
		else calci->GetSonogram().DrawSlices(0, 1000 - 1, image);
	}
	set_scale(the_song);
	show_outlines(image, onset, offset);
	option->doNotSegment = true;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::FMBClick(TObject *Sender) {
	TSpeedButton *fm_btn, *group_btns;
    TImage *image;
    TRadioGroup *sono_display;
    CZT_FeatureCalc *calci;
    bool the_song, paginate, smooth_feature;
    int onset, offset;
    if (Sender == FMB || display->ActivePage == Song1) {
        fm_btn = FMB;
        the_song = Ksong1;
        calci = &m_Calculator1;
        group_btns = group;
        paginate = option->paginationOn;
        smooth_feature = option->smooth_feature1;
        image = Image1;
        sono_display = deriv_display;
        onset = OnsetSong1;
        offset = OffsetSong1;
    }
    else if (Sender == FM2B || display->ActivePage == Song2) {
        fm_btn = FM2B;
        the_song = Ksong2;
        calci = &m_Calculator2;
        group_btns = group2;
        paginate = option->paginationOn2;
        smooth_feature = option->smooth_feature2;
        image = Image2;
        sono_display = deriv_display2;
        onset = OnsetSong2;
        offset = OffsetSong2;
    }

    if (fm_btn->Down)
        set_feature_col_highlight(7, the_song);
    else
        set_feature_col_highlight(0, the_song);
    if (!group_btns->Down)
		hide_all_features(calci);

	if (sono_display->ItemIndex == 0)
	{
		calci->GetImage().GetFM().bRender = fm_btn->Down;
		if (smooth_feature) calci->GetImage().Gethp_small().bRender = fm_btn->Down;
		if (!paginate) calci->GetImage().DrawAllSlices(image);
		else calci->GetImage().DrawSlices(0, 1000 - 1, image);
	}
	else if (sono_display->ItemIndex == 1)
	{
		calci->GetContours().GetFM().bRender = fm_btn->Down;
		if (smooth_feature) calci->GetContours().Gethp_small().bRender = fm_btn->Down;
		if (!paginate) calci->GetContours().DrawAllSlices(image);
		else calci->GetContours().DrawSlices(0, 1000 - 1, image);
	}
	else if (sono_display->ItemIndex == 2)
	{
		calci->GetSonogram().GetFM().bRender = fm_btn->Down;
		if (smooth_feature) calci->GetSonogram().Gethp_small().bRender = fm_btn->Down;
		if (!paginate) calci->GetSonogram().DrawAllSlices(image);
		else calci->GetSonogram().DrawSlices(0, 1000 - 1, image);
	}
	set_scale(0);
	show_outlines(image, onset, offset);
	option->doNotSegment = true;
}

void __fastcall TForm1::PowerBClick(TObject *Sender) {
	TSpeedButton *meanFr_btn, *group_btns;
	TImage *image;
	TRadioGroup *sono_display;
	CZT_FeatureCalc *calci;
	bool the_song, paginate, smooth_feature;
	int onset, offset;
	if (Sender == PowerB || display->ActivePage == Song1) {
		meanFr_btn = PowerB;
		the_song = Ksong1;
		calci = &m_Calculator1;
		group_btns = group;
		paginate = option->paginationOn;
		smooth_feature = option->smooth_feature1;
		image = Image1;
		sono_display = deriv_display;
		onset = OnsetSong1;
		offset = OffsetSong1;
	}
	else if (Sender == Power2B || display->ActivePage == Song2) {
		meanFr_btn = Power2B;
		the_song = Ksong2;
		calci = &m_Calculator2;
		group_btns = group;
		paginate = option->paginationOn2;
		smooth_feature = option->smooth_feature2;
		image = Image2;
		sono_display = deriv_display2;
		onset = OnsetSong2;
		offset = OffsetSong2;
	}

	if (meanFr_btn->Down)
		set_feature_col_highlight(4, the_song);
	else
		set_feature_col_highlight(0, the_song);
	if (!group_btns->Down)
		hide_all_features(calci);


	if (sono_display->ItemIndex == 0)
	{
		calci->GetImage().GetPeakFreq().bRender = meanFr_btn->Down;
		if (smooth_feature)	calci->GetImage().Gethp_small().bRender = meanFr_btn->Down;
		if (!paginate)calci->GetImage().DrawAllSlices(image);
		else calci->GetImage().DrawSlices(0, 1000 - 1, image);
	}

	else if (sono_display->ItemIndex == 1)
	{
		calci->GetContours().GetPeakFreq().bRender = meanFr_btn->Down;
		if (smooth_feature)	calci->GetContours().Gethp_small().bRender = meanFr_btn->Down;
		if (!paginate)calci->GetContours().DrawAllSlices(image);
		else calci->GetContours().DrawSlices(0, 1000 - 1, image);
	}

	else if (sono_display->ItemIndex == 2)
	{
		calci->GetSonogram().GetPeakFreq().bRender = meanFr_btn->Down;
		if (smooth_feature)	calci->GetSonogram().Gethp_small().bRender = meanFr_btn->Down;
		if (!paginate)calci->GetSonogram().DrawAllSlices(image);
		else calci->GetSonogram().DrawSlices(0, 1000 - 1, image);
	}

	set_scale(the_song);
	show_outlines(image, onset, offset);
	option->doNotSegment = true;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::DerivPowerBClick(TObject *Sender) {

	TSpeedButton *amp_btn, *group_btns;
	TImage *image;
	TRadioGroup *sono_display;
	CZT_FeatureCalc *calci;
	bool the_song, paginate, smooth_feature;
	int onset, offset;
	if (Sender == DerivPowerB || display->ActivePage == Song1) {
		amp_btn = DerivPowerB;
		the_song = Ksong1;
		calci = &m_Calculator1;
		group_btns = group;
		paginate = option->paginationOn;
		smooth_feature = option->smooth_feature1;
		image = Image1;
		sono_display = deriv_display;
		onset = OnsetSong1;
		offset = OffsetSong1;
	}
	else if (Sender == DerivPower2B || display->ActivePage == Song2) {
		amp_btn = DerivPower2B;
		the_song = Ksong2;
		calci = &m_Calculator2;
		group_btns = group2;
		paginate = option->paginationOn2;
		smooth_feature = option->smooth_feature2;
		image = Image2;
		sono_display = deriv_display2;
		onset = OnsetSong2;
		offset = OffsetSong2;
	}
	if (amp_btn->Down)
		set_feature_col_highlight(2, the_song);
	else
		set_feature_col_highlight(0, the_song);
	if (!group_btns->Down)
		hide_all_features(calci);

	if (sono_display->ItemIndex == 0)
	{
		calci->GetImage().GetDerivPow().bRender = amp_btn->Down;
		if (smooth_feature) calci->GetImage().Gethp_small().bRender = amp_btn->Down;
		if (!paginate) calci->GetImage().DrawAllSlices(image);
		else calci->GetImage().DrawSlices(0, 1000 - 1, image);
	}

	else if (sono_display->ItemIndex == 1)
	{
		calci->GetContours().GetDerivPow().bRender = amp_btn->Down;
		if (smooth_feature) calci->GetContours().Gethp_small().bRender = amp_btn->Down;
		if (!paginate) calci->GetContours().DrawAllSlices(image);
		else calci->GetContours().DrawSlices(0, 1000 - 1, image);
	}

	else if (sono_display->ItemIndex == 2)
	{
		calci->GetSonogram().GetDerivPow().bRender = amp_btn->Down;
		if (smooth_feature) calci->GetSonogram().Gethp_small().bRender = amp_btn->Down;
		if (!paginate) calci->GetSonogram().DrawAllSlices(image);
		else calci->GetSonogram().DrawSlices(0, 1000 - 1, image);
	}
	set_scale(the_song);
    option->doNotSegment = true;
	show_outlines(image, onset, offset);

}
// ---------------------------------------------------------------------------

void TForm1::SaveFeatures(bool scale) {
    SaveDialog1->InitialDir = "c:\\Sound Analysis";
    SaveDialog1->DefaultExt = "TXT";
    SaveDialog1->Title = "Please choose a name for features file";
    if (SaveDialog1->Execute()) {
        // strcpy(c,SaveDialog1->FileName.c_str());
        strcpy(c, (char*)SaveDialog1->FileName.c_str());

        ofstream out(c);
        out <<
            "pitch ; modulation ; entropy ; continuity ; power ; peak frequency ; signal" << endl;
        for (int i = 7; i < Iterations1 - 7; i++) {
            if (Active == ksong1) {
                if (scale)
                    out << PitchS1[i] << ";" << FMS1[i] << ";" << EntropyS1[i]
                        << ";" << PitchGoodnessS1[i] << ";" << Amplitude1[i]
                        << ";" << 44.1 * PeakFreq1[i] << ";" << Signal1[i]
                        << endl;
                else
                    out << 44.1 * Pitch1[i] << ";" << FM1[i] << ";" << Entropy1
                        [i] << ";" << Amplitude1[i] << ";" << 44.1 * PeakFreq1
                        [i] << ";" << Signal1[i] << endl;
            }
            else {
                if (scale)
                    out << PitchS2[i] << ";" << FMS2[i] << ";" << EntropyS2[i]
                        << ";" << PitchGoodnessS2[i] << ";" << Amplitude2[i]
                        << ";" << 44.1 * PeakFreq2[i] << ";" << Signal2[i]
                        << endl;
                else
                    out << 44.1 * Pitch2[i] << ";" << FM2[i] << ";" << Entropy2
                        [i] << ";" << Amplitude2[i] << ";" << 44.1 * PeakFreq2
                        [i] << ";" << Signal2[i] << endl;
            }
        }
        Beep();
    }
}

void TForm1::SaveSonogram() {
    SaveGraph->DefaultExt = "bmp";
    // Beep();
    if (autosave_image->Checked) {
		str = "C:\\sap\\";
        if (Active == ksong1) {
            str += songname->Caption.SubString(1,
                songname->Caption.Length() - 4);
            str += ".bmp";
            Image1->Picture->SaveToFile(str);
        }
        else {
            str += SongName2a->Caption.SubString(1,
                SongName2a->Caption.Length() - 4);
            str += ".bmp"; ;
			Image2->Picture->SaveToFile(str);
        }
        return;
    }

    SaveGraph->Title = "Please choose a name for song immage file";
    if (SaveGraph->Execute()) {
		if (Active == ksong1)
            Image1->Picture->SaveToFile(SaveGraph->FileName);
        else
			Image2->Picture->SaveToFile(SaveGraph->FileName);
	}

	SaveGraph->InitialDir=ExtractFileDir(SaveGraph->FileName);
}


/////////////////////////////////////////////////////



void __fastcall TForm1::calc_periodClick(TObject *Sender) {
    Screen->Cursor = crHourGlass;
	if (Sender == calc_period)
		calc_meanPeriod_and_meanEuclideanDist(ksong1);
	else
		calc_meanPeriod_and_meanEuclideanDist(ksong1);
	Screen->Cursor = crDefault;
}
// ---------------------------------------------------------------------------

void TForm1::draw_score_sounds() {

	Image3->AutoSize = true;
	Image3->Stretch = false;
	Image4->AutoSize = true;
	Image4->Stretch = false;
	Similarity_image->AutoSize = true;
	Similarity_image->Stretch = false;
	Zoom = 0;

	Image3->Canvas->Brush->Color = clWhite;
	Image3->Canvas->FillRect(Rect(0, 130, Image3->Width, 155));
	Image4->Canvas->Brush->Color = clWhite;
	Image4->Canvas->FillRect(Rect(130, 0, 145, Image4->Height));

	Image3->Picture->Bitmap->Height = Image1->Picture->Bitmap->Height / 2 + 20;
	Image3->Picture->Bitmap->Width = OffsetSong1 - OnsetSong1;
	// Image1->Picture->Bitmap->Width;
	// for(int w=0;w<Image1->Picture->Bitmap->Width; w++)
	for (int w = OnsetSong1; w <= OffsetSong1; w++)
		for (int h = 0; h < Image1->Picture->Bitmap->Height / 2; h++)
			Image3->Canvas->Pixels[w - OnsetSong1][h] = Image1->Canvas->Pixels
				[w][h * 2];
	Image3->Invalidate();

	int image2H = Image2->Picture->Bitmap->Height;
	// int image2W=Image2->Picture->Bitmap->Width;
	Image4->Picture->Bitmap->Height = OffsetSong2 - OnsetSong2; // image2W;
	Image4->Picture->Bitmap->Width = image2H / 2 + 30;
	image2H /= 2; // this will scale the higth down to half

	// for(int w=0;w<image2W; w++)
	for (int w = OnsetSong2; w < OffsetSong2; w++)
		for (int h = 0; h < image2H; h++)
			Image4->Canvas->Pixels[h][w - OnsetSong2] = Image2->Canvas->Pixels
				[w][(image2H - h) * 2];
	Image4->Invalidate();
}

void __fastcall TForm1::SimilarityEnter(TObject *Sender) {
	if (update_score_sounds)
		draw_score_sounds();
	update_score_sounds = false;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::CalcModeClick(TObject *Sender) {

	CalcMode2->ItemIndex = CalcMode->ItemIndex;
	CalcModeAlt->ItemIndex = CalcMode->ItemIndex;
	CalcModeAlt2->ItemIndex = CalcMode->ItemIndex;
	if(CalcMode->ItemIndex==1 && features_display->TabIndex==0) features_display->TabIndex=2;
	/*
	AnsiString str =
        "Warning: You had changed thresholds: you must reload the sound prior to outlining, otherwize your";
    str +=
        "changes wont register to the database and segmentation in the data wont match your decision";
	if (parameters_altered && CalcMode->ItemIndex == 2 && warnings->Checked)
		MessageDlg(str, mtWarning, TMsgDlgButtons() << mbOK, 0);
		*/
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::ToExcelClick(TObject *Sender) {
    DataForm->ToExelClick(this);
}
// ---------------------------------------------------------------------------

void TForm1::set_all_bitmaps() {
    // We declare the bitmap objects
    LocAllBM = new Graphics::TBitmap();
    SetBitmap(LocAllBM, sim_iter1, sim_iter2, clWhite);
    GlobModBM = new Graphics::TBitmap();
    SetBitmap(GlobModBM, sim_iter1, sim_iter2, clWhite);
    CombinedBM = new Graphics::TBitmap();
    SetBitmap(CombinedBM, sim_iter1, sim_iter2, clWhite);
    GlobEntBM = new Graphics::TBitmap();
    SetBitmap(GlobEntBM, sim_iter1, sim_iter2, clWhite);
    LocPitchBM = new Graphics::TBitmap();
    SetBitmap(LocPitchBM, sim_iter1, sim_iter2, clWhite);
    LocEntBM = new Graphics::TBitmap();
    SetBitmap(LocEntBM, sim_iter1, sim_iter2, clWhite);
    LocModBM = new Graphics::TBitmap();
    SetBitmap(LocModBM, sim_iter1, sim_iter2, clWhite);
    LocAMBM = new Graphics::TBitmap();
    SetBitmap(LocAMBM, sim_iter1, sim_iter2, clWhite);
    GlobPitchBM = new Graphics::TBitmap();
    SetBitmap(GlobPitchBM, sim_iter1, sim_iter2, clWhite);
    GlobalBM = new Graphics::TBitmap();
    SetBitmap(GlobalBM, sim_iter1, sim_iter2, clWhite);
    LocContBM = new Graphics::TBitmap();
    SetBitmap(LocContBM, sim_iter1, sim_iter2, clWhite);
    GlobContBM = new Graphics::TBitmap();
    SetBitmap(GlobContBM, sim_iter1, sim_iter2, clWhite);
    GlobAMBM = new Graphics::TBitmap();
    SetBitmap(GlobAMBM, sim_iter1, sim_iter2, clWhite);

}

void TForm1::scan_the_lines(int line) {
    LocalMeasuresPtr = (COLORREF*)LocAllBM->ScanLine[line];
    LocalPitchPtr = (COLORREF*)LocPitchBM->ScanLine[line];
    LocalEntropyPtr = (COLORREF*)LocEntBM->ScanLine[line];
    LocalSlopePtr = (COLORREF*)LocModBM->ScanLine[line];
    LocalContPtr = (COLORREF*)LocContBM->ScanLine[line];
    LocalAMPtr = (COLORREF*)LocAMBM->ScanLine[line];
    GlobalMeasuresPtr = (COLORREF*)GlobalBM->ScanLine[line];
    GlobalPitchPtr = (COLORREF*)GlobPitchBM->ScanLine[line];
    GlobalEntropyPtr = (COLORREF*)GlobEntBM->ScanLine[line];
    GlobalSlopePtr = (COLORREF*)GlobModBM->ScanLine[line];
    GlobalContPtr = (COLORREF*)GlobContBM->ScanLine[line];
    GlobalAMPtr = (COLORREF*)GlobAMBM->ScanLine[line];
    AllMeasuresPtr = (COLORREF*)CombinedBM->ScanLine[line];
}

void TForm1::set_local_score(float LocalMeasures, int song1, int song2) {
    if (LocalMeasures < 0.03)
        Sim[song1][song2] = 100;
    else if (LocalMeasures < 0.065)
        Sim[song1][song2] = 99;
    else if (LocalMeasures < 0.095)
        Sim[song1][song2] = 98;
    else if (LocalMeasures < 0.125)
        Sim[song1][song2] = 97;
    else if (LocalMeasures < 0.17)
        Sim[song1][song2] = 95;
    else if (LocalMeasures < 0.23)
        Sim[song1][song2] = 93;
    else if (LocalMeasures < 0.32)
        Sim[song1][song2] = 90;
    else if (LocalMeasures < 0.44)
        Sim[song1][song2] = 85;
    else if (LocalMeasures < 0.55)
        Sim[song1][song2] = 80;
    else if (LocalMeasures < 0.66)
        Sim[song1][song2] = 75;
    else if (LocalMeasures < 0.72)
        Sim[song1][song2] = 70;
    else if (LocalMeasures < 1)
        Sim[song1][song2] = 60;
    else if (LocalMeasures < 1.38)
        Sim[song1][song2] = 50;
    else if (LocalMeasures < 1.65)
        Sim[song1][song2] = 40;
    else if (LocalMeasures < 2.7)
        Sim[song1][song2] = 30;
    else if (LocalMeasures < 3.8)
        Sim[song1][song2] = 20;
    else if (LocalMeasures < 5.5)
        Sim[song1][song2] = 10;
    else
        Sim[song1][song2] = 0;
}

void __fastcall TForm1::delete_allClick(TObject *Sender) {
    if (warnings->Checked)
        if (MessageDlg("Are you sure? All records will be deleted!",
                mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
        {
            DataForm->Records->Active = false;
            DataForm->Records->EmptyTable();
            DataForm->Records->Active = true;
            DataForm->CountRecsClick(this);
            RecordsNum->Caption = DataForm->recNum;
            RecordsNum2->Caption = DataForm->recNum;
        }
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::remove_recClick(TObject *Sender) {
    DataForm->Records->Active = true;
    DataForm->Records->Last();
    DataForm->Records->Delete();
    DataForm->CountRecsClick(this);
    RecordsNum->Caption = DataForm->recNum;
    RecordsNum2->Caption = DataForm->recNum;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::view_recsClick(TObject *Sender) {
    DataForm->Records->Active = false;
    DataForm->Records->Active = true; // this get the table data refreshed, dont ask why...
    DataForm->DataSource1->DataSet = DataForm->Records;
    DataForm->DataSource1->Enabled = true;
    DataForm->Visible = true;
    DataForm->BringToFront();
    DataForm->show_start = false;
    DataForm->datapages->ActivePageIndex = 3;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::openTableClick(TObject *Sender) {
    DataForm->TheTable = DataForm->Records;
    DataForm->openTableClick(this);
    if (!DataForm->table_exists(DataForm->TheTable->TableName))
        return;
	if (Sender == openTable || display->ActivePage==Song1) {
        table_name->Caption = DataForm->TheTable->TableName;
        RecordsNum->Caption = DataForm->recNum;
        table1L->Caption = table_name->Caption;
        table1_numL->Caption = RecordsNum->Caption;
        DataForm->Settings(1, "syllable_table", table_name->Caption);
    }
    else {
        table_name2->Caption = DataForm->TheTable->TableName;
        RecordsNum2->Caption = DataForm->recNum;
        table2L->Caption = table_name2->Caption;
        table2_numL->Caption = RecordsNum2->Caption;
        DataForm->Settings(2, "syllable_table", table_name2->Caption);
    }
    option->syllable_table_name = DataForm->Records->TableName;
    DataForm->fix_syll_table_fields(option->syllable_table_name);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::save_scoreClick(TObject *Sender) {

    DataForm->similarity->Append();
    DataForm->similarity->FieldValues["song1"] = songname->Caption;
    DataForm->similarity->FieldValues["song2"] = SongName2a->Caption;
    DataForm->similarity->FieldValues["score"] = scoreN->Value;
    DataForm->similarity->FieldValues["similarity"] = similarityN->Value;
    DataForm->similarity->FieldValues["accuracy"] = accuracyN->Value;
    DataForm->similarity->FieldValues["sequential_match"] = sequentialN->Value;
    DataForm->similarity->FieldValues["start1"] = beginL->Value;
    DataForm->similarity->FieldValues["start2"] = begin2L->Value;
    DataForm->similarity->FieldValues["end1"] = endL->Value;
    DataForm->similarity->FieldValues["end2"] = end2L->Value;
    DataForm->similarity->Append();
    Beep();
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::matlabClick(TObject *Sender) {

    DataForm->Visible = true;
    DataForm->BringToFront();
    DataForm->show_start = false;
    DataForm->datapages->ActivePageIndex = 1;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::new_tableClick(TObject *Sender) {
    DataForm->new_tableClick(this);
    table_name->Caption = DataForm->TheTable->TableName;
    RecordsNum->Caption = DataForm->recNum;
    table_name2->Caption = table_name->Caption;
    RecordsNum2->Caption = RecordsNum->Caption;
	DataForm->Settings(1, "syllable_table", DataForm->Records->TableName);
    option->syllable_table_name = DataForm->Records->TableName;
}
// ---------------------------------------------------------------------------

void TForm1::save_syllables(bool song) {
    CZT_FeatureCalc *calci;
    TEdit *memo;
    TLabel *the_song;
    AnsiString animal_ID;
    int onset, offset, tempnum = DataForm->recNum;
    if (song == ksong1) {
        animal_ID = bird_name->Caption;
        calci = &m_Calculator1;
        memo = comments;
        the_song = songname;
        onset = OnsetSong1;
        offset = OffsetSong1;
    }
    else {
        animal_ID = bird_name2->Caption;
        calci = &m_Calculator2;
        memo = comments2;
        the_song = SongName2a;
        onset = OnsetSong2;
        offset = OffsetSong2;
    }
   // calci->current_file_name = memo->Text;
    calci->current_file_name = the_song->Caption;
    if (animal_ID == "")
        animal_ID = "NA";
    for (int i = 0; i < calci->syllable_number; i++) {
        option->syllCounter++;
        calci->features = calci->mem_recs[i];
        calci->features.bird_ID = animal_ID;
        if (calci->features.start > onset && calci->features.start < offset)
            calci->Add_Record();
    }
    DataForm->CountRecsClick(this);
    RecordsNum->Caption = DataForm->recNum;
    if (DataForm->recNum == tempnum) {

        AnsiString str =
            "Warning, Sound Analysis failed to update the table.  If syllables were detected (light blue underline apears bellow each syllable in the display) ";
        str +=
            " than you must reload the table: click 'Change table' and open the table again.";
        MessageDlg(str, mtError, TMsgDlgButtons() << mbOK, 0);
    }
    RecordsNum2->Caption = RecordsNum->Caption; // update recs display
    Beep();
}

void __fastcall TForm1::calc_contoursClick(TObject *Sender) {
    if (calc_contours->Checked)
        MessageDlg("please open the sound again to enable contours display",
        mtInformation, TMsgDlgButtons() << mbOK, 0);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::CombinedEnter(TObject *Sender) {
    calc_KS->Checked = true;
    comp_sound1->Caption = songname->Caption;
    comp_sound2->Caption = SongName2a->Caption;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::guiClick(TObject *Sender) {
    if (gui->Checked)
        GUI = true;
    else
        GUI = false;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn2Click(TObject *Sender) {
    soundList->Directory = Edit1->Text;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::addClick(TObject *Sender) {
    TListBox *list;
    if (ActiveList)
        list = List2;
    else
        list = List1;

    for (int i = 0; i < soundList->Items->Count; i++)
		if (soundList->Selected[i]){
			str=soundList->Directory;
			str+="\\";
			str+=soundList->Items->Strings[i];
			list->Items->Add(str);
		}
    // FileName);//soundList->Items->Strings[i]);//
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::add_allClick(TObject *Sender) {
    // if(ActiveList==kList1) List1->Items=soundList->Items;
    // else List2->Items=soundList->Items;
    for (int i = 0; i < soundList->Items->Count; i++) {
        soundList->Selected[i] = true;
    }
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::List1KeyDown(TObject *Sender, WORD &Key,
    TShiftState Shift) {
    if (Key == 46 || Key == 110)
        List1->Items->Delete(List1->ItemIndex);

}
// ---------------------------------------------------------------------------

void __fastcall TForm1::List1MouseDown(TObject *Sender, TMouseButton Button,
    TShiftState Shift, int X, int Y) {
    if (ActiveList == kList2) {
        List1->Color = clYellow;
        List2->Color = clWhite;
        ActiveList = kList1;
    }
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::List2KeyDown(TObject *Sender, WORD &Key,
    TShiftState Shift) {
    if (Key == 46 || Key == 110)
        List2->Items->Delete(List2->ItemIndex);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::List2MouseDown(TObject *Sender, TMouseButton Button,
    TShiftState Shift, int X, int Y) {
    if (ActiveList == kList1) {
        List2->Color = clYellow;
        List1->Color = clWhite;
        ActiveList = kList2;
    }
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::data_typeClick(TObject *Sender) {
    if (data_type->ItemIndex) {
        soundList->Mask = "*.dat";
        option->format->ItemIndex = 0;
    }
    else {
        soundList->Mask = "*.wav";
        option->format->ItemIndex = 1;
    }
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Edit1Click(TObject *Sender) {
    Edit1->Clear();
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::DoBatchClick(TObject *Sender) {
	option->paginationOn=option->paginationOn2=false;
	if (ScoreB->Caption == "Reset")
        ScoreBClick(this); // make sure the Score is in 'score' state;

    if (MtimesN->ItemIndex == 2) // this is the syllable target batch mode
    {
        the_iteration = 0;
        DataForm->Select_data(selection->Text);
        target_syllable_num = DataForm->MyQuery1->RecordCount;
        DataForm->MyQuery1->First();
        table_ms_duration = to_ms->Text.ToDouble();
        table_ms_start = to_ms_start->Text.ToDouble();
        table_iterate = iterations->Text.ToInt();
        table_buffer = buffer->Text.ToInt();
        display->ActivePage = Similarity;
    }

    else if (MtimesN->ItemIndex == 3) // this is the Table1 x Table2 mode
    {
        DataForm->Records->First();
        DataForm->Records1->First();
        table_ms_duration = to_ms->Text.ToDouble();
        table_ms_start = to_ms_start->Text.ToDouble();
        table_iterate = iterations->Text.ToInt();
        table_buffer = buffer->Text.ToInt();
        display->ActivePage = Similarity;
    }

    else {
        // gui->Checked=false;  GUI=false;
        m_Calculator1.render_image = false;
        m_Calculator2.render_image = false;
        List1->ItemIndex = -1;
        List2->ItemIndex = -1;
        RecordsNum->Caption = DataForm->Records->RecordCount;
    }

    option->halt = false;
    DataForm->Records->Active = true;
	dobatch = true;
}

// ---------------------------------------------------------------------------
// void __fastcall TForm1::MyIdleHandler(TObject *Sender, bool &Done)
// {

// }

void __fastcall TForm1::breakItClick(TObject *Sender) {
    dobatch = false;
}
// ---------------------------------------------------------------------------

int TForm1::syll_score(float syll_dist) {
    if (syll_dist < 0.05)
        return(100);
    else if (syll_dist < 0.15)
        return(99);
    else if (syll_dist < 0.3)
        return(98);
    else if (syll_dist < 0.418)
        return(97);
    else if (syll_dist < 0.545)
        return(96);
    else if (syll_dist < 0.61)
        return(95);
    else if (syll_dist < 0.71)
        return(94); // manipulated up to here
    else if (syll_dist < 0.85)
        return(93);
    else if (syll_dist < 1)
        return(90);
    else if (syll_dist < 1.25)
        return(85);
    else if (syll_dist < 1.36)
        return(80);
    else if (syll_dist < 1.64)
        return(75);
    else if (syll_dist < 1.86)
        return(70);
    else if (syll_dist < 2.36)
        return(60);
    else if (syll_dist < 2.72)
        return(50);
    else if (syll_dist < 3.36)
        return(40);
    else if (syll_dist < 4.09)
        return(30);
    else if (syll_dist < 5.45)
        return(20);
    else if (syll_dist < 7.1)
        return(10);
    else
        return(0);
}

// ------------------------------------

int TForm1::glob_score(float dist) {
    if (dist < 0.48)
        return(100);
    else if (dist < 0.56)
        return(98);
    else if (dist < 0.66)
        return(95);
    else if (dist < 0.84)
        return(90); // manipulated up to here
    else if (dist < 0.85)
        return(93);
    else if (dist < 0.95)
        return(85);
    else if (dist < 1.04)
        return(80);
    else if (dist < 1.12)
        return(75);
    else if (dist < 1.17)
        return(70);
    else if (dist < 1.29)
        return(65);
    else if (dist < 1.38)
        return(60);
    else if (dist < 1.62)
        return(50);
    else if (dist < 2.05)
        return(40);
    else if (dist < 2.62)
        return(30);
    else if (dist < 3.4)
        return(20);
    else if (dist < 4.3)
        return(10);
    else if (dist < 6)
		return(5);
	else
		return(0);
}

// ---------------------------------------------------------------------------

void __fastcall TForm1::data_windowChange(TObject *Sender) {
	TTrackBar *FFTwindowTB;
	TNumLab *FFTwindowN;
	bool snd;

	if (Sender == data_window) {
		snd = 0;
		FFTwindowTB = data_window;
		FFTwindowN = windowTB;
	}
	else {
		snd = 1;
		FFTwindowTB = data_window2;
		FFTwindowN = windowTB2;
	}

	FFTwindowN->Value = 1000.0 * (float)FFTwindowTB->Position / (float)
		option->sampling_rate;

	// FFTwindowN->Value=(float)FFTwindowTB->Position/44.1;
	data_window->Position = FFTwindowTB->Position;
	data_window2->Position = FFTwindowTB->Position;
	windowTB->Value = FFTwindowN->Value;
	windowTB2->Value = FFTwindowN->Value;
	option->data_window->Position = FFTwindowTB->Position;
	option->data_windowChange(this);
	// DataForm->Settings(1, "FFT_window", data_window->Position);
	// DataForm->Settings(2, "FFT_window", data_window2->Position);
	recalc(snd);
	// derivTBChange(this);

}
// ---------------------------------------------------------------------------

void __fastcall TForm1::advanceChange(TObject *Sender) {
	TTrackBar *FFTadvanceTB;
	TShape *preview_curs;
	TNumLab *FFTadvanceN;
	bool snd;
	int calc_adv, adv;
	int *totPages;

	if (Sender == advance) {
		snd = 0;
		calc_adv=calc_advance1;
		adv=advance->Position;
		totPages = &option->totalPages;
		FFTadvanceTB = advance;
		FFTadvanceN = advanceTb;
		preview_curs = preview_cursor;
	}
	else {
		snd = 1;
		calc_adv=calc_advance2;
		adv=advance2->Position;
		totPages = &option->totalPages2;
		FFTadvanceTB = advance2;
		FFTadvanceN = advanceTb2;
		preview_curs = preview_cursor2;
	}

	if(!(*totPages))preview_curs->Width = FFTadvanceN->Value * 100;
	else {
	  float foo=(float)adv/(float)calc_adv;
	  preview_curs->Width = foo*1000.0 * (float)preview_sono->Width / (float)max((*totPages),1);
	}

	// FFTadvanceN->Value=float(FFTadvanceTB->Position)/44.1;
	FFTadvanceN->Value = 1000.0 * (float)FFTadvanceTB->Position / (float)
		option->sampling_rate;
	advance->Position = FFTadvanceTB->Position;
	advance2->Position = FFTadvanceTB->Position;
	advanceTb->Value = FFTadvanceN->Value;
	advanceTb2->Value = FFTadvanceN->Value;
	option->advance->Position = FFTadvanceTB->Position;
	option->advanceChange(this);
	// DataForm->Settings(1, "advance_window", advance->Position);
	// DataForm->Settings(2, "advance_window", advance2->Position);
	recalc(snd);
	// derivTBChange(this);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::TimeWarpTBChange(TObject *Sender) {
	TimeWarpLB->Value = (float)TimeWarpTB->Position / 100;
	option->TimeWarpTB->Position = TimeWarpTB->Position;
	option->TimeWarpTBChange(this);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::min_peak_freqChange(TObject *Sender) {
    option->noise_thresh_freq = min_peak_freq->Position / 86;
    min_peak->Value = min_peak_freq->Position;
    update_display->Visible = true;
    update_display2->Visible = true;
}
// ---------------------------------------------------------------------------


void __fastcall TForm1::saveScoreSegmClick(TObject *Sender) {
    DataForm->similarity->Active = true;
    DataForm->similarity->Append();
    DataForm->similarity->FieldValues["song1"] = songname->Caption;
    DataForm->similarity->FieldValues["song2"] = SongName2a->Caption;
	DataForm->similarity->FieldValues["similarity"] = syll_Score->Value;
    DataForm->similarity->FieldValues["Kolmogorov_Smirnov"] = ks_all->Value;
    DataForm->similarity->FieldValues["sequential_match"] = sequentialN->Value;
	DataForm->similarity->Append();
}
// ---------------------------------------------------------------------------




void __fastcall TForm1::GlobAMScoreBClick(TObject *Sender) {
    Similarity_image->Picture->Graphic = GlobAMBM;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::LocalAMScoreBClick(TObject *Sender) {
    Similarity_image->Picture->Graphic = LocAMBM;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::AMBClick(TObject *Sender) {
	TSpeedButton *am_btn, *group_btns;
    TImage *image;
    TRadioGroup *sono_display;
    CZT_FeatureCalc *calci;
    bool the_song, paginate, smooth_feature;
    int onset, offset;
    if (Sender == AMB || display->ActivePage == Song1) {
        am_btn = AMB;
        the_song = Ksong1;
        calci = &m_Calculator1;
        group_btns = group;
        paginate = option->paginationOn;
        smooth_feature = option->smooth_feature1;
        image = Image1;
        sono_display = deriv_display;
        onset = OnsetSong1;
        offset = OffsetSong1;
    }
    else if (Sender == AM2B || display->ActivePage == Song2) {
        am_btn = AM2B;
        the_song = Ksong2;
        calci = &m_Calculator2;
        group_btns = group2;
        paginate = option->paginationOn2;
        smooth_feature = option->smooth_feature2;
        image = Image2;
		sono_display = deriv_display2;
        onset = OnsetSong2;
        offset = OffsetSong2;
    }

    if (am_btn->Down)
        set_feature_col_highlight(8, the_song);
    else
        set_feature_col_highlight(0, the_song);
    if (!group_btns->Down)
        hide_all_features(calci);

	if (sono_display->ItemIndex == 0)
	{
		calci->GetImage().GetAM().bRender = am_btn->Down;
		if (smooth_feature) calci->GetImage().Gethp_small().bRender = am_btn->Down;
		if (!option->paginationOn)calci->GetImage().DrawAllSlices(image);
		else calci->GetImage().DrawSlices(0, 1000 - 1, image);
	}
	else if (sono_display->ItemIndex == 1)
	{
		calci->GetContours().GetAM().bRender = am_btn->Down;
		if (smooth_feature) calci->GetContours().Gethp_small().bRender = am_btn->Down;
		if (!option->paginationOn)calci->GetContours().DrawAllSlices(image);
		else calci->GetContours().DrawSlices(0, 1000 - 1, image);
	}
	else if (sono_display->ItemIndex == 2)
	{
		calci->GetSonogram().GetAM().bRender = am_btn->Down;
		if (smooth_feature) calci->GetSonogram().Gethp_small().bRender = am_btn->Down;
		if (!option->paginationOn)calci->GetSonogram().DrawAllSlices(image);
		else calci->GetSonogram().DrawSlices(0, 1000 - 1, image);
	}
	set_scale(the_song);
	show_outlines(image, onset, offset);
    // show_outlines(Image1, OnsetSong1, OffsetSong1);
    option->doNotSegment = true;
}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------

void __fastcall TForm1::sound_detection_termsClick(TObject *Sender) {
    if (sound_detection_terms->ItemIndex) {
        option->entropy_and_power = false;
        option->entropy_or_power = true;
    }
    else {
        option->entropy_and_power = true;
        option->entropy_or_power = false;
    }
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::conservativeClick(TObject *Sender) {
    gui->Enabled = true;
    // ThreshlodTB->Position=4;
    // ThreshNum->Value=0.15;
    draw_score_sounds();
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::liberalClick(TObject *Sender) {
	// ThreshlodTB->Position=4;
    // ThreshNum->Value=0.15;
    draw_score_sounds();
    gui->Checked = true;
    GUI = true;
    gui->Enabled = false;
}
// ---------------------------------------------------------------------------


void __fastcall TForm1::MtimesNClick(TObject *Sender) {
    if (MtimesN->ItemIndex == 2 || MtimesN->ItemIndex == 3) {
        target_score->Visible = true;
        List1->Visible = false;
        List2->Visible = false;
    }
    else {
        target_score->Visible = false;
        List1->Visible = true;
        List2->Visible = true;
    }
    if (MtimesN->ItemIndex == 2) {
        batch_tablename2->Visible = false;
        table2L->Visible = false;
        batch_count_table2->Visible = false;
        batch_table2_open->Visible = false;
        selection->Visible = true;
        querry_text->Visible = true;
        batch_folder2->Visible = false;
        folder2->Visible = false;
        folder2_btn->Visible = false;
    }
    else if (MtimesN->ItemIndex == 3) {
        batch_tablename2->Visible = true;
        table2L->Visible = true;
        batch_count_table2->Visible = true;
        batch_table2_open->Visible = true;
        selection->Visible = false;
        querry_text->Visible = false;
        batch_folder2->Visible = true;
        folder2->Visible = true;
        folder2_btn->Visible = true;
    }
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn22Click(TObject *Sender) {
    DataForm->MyQuery1->Next();
    dobatch = true;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn23Click(TObject *Sender) {
    string =
        "This setting exclude sounds according to the distribution of their power spectrum \r";
    string +=
        " For example, if the amount of power below 500Hz is more than 50% of the total power\r";
    string +=
        "Sound Analysis will reject that sound. You can use this filter to exclude high frequencies \r";
    string +=
        " by selecting 'above' and you can also change the rejection ratio.  \r";
    string += "\r";
    string +=
        " NOTE:  You must re-open the sound for the new settings to take effect.";
    MessageDlgPos(string, mtInformation, TMsgDlgButtons() << mbOK, 0, 140, 162);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::noise_detectClick(TObject *Sender) {
    if (noise_detect->ItemIndex)
        option->detect_low_noise = true;
    else
        option->detect_low_noise = false;
    update_display->Visible = true;
    update_display2->Visible = true;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::noise_ratioChange(TObject *Sender) {
    option->noise_ratio = (float)noise_ratio->Position / 100;
    noise_tb->Value = noise_ratio->Position;
    update_display->Visible = true;
    update_display2->Visible = true;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::CalcMode2Click(TObject *Sender) {
	CalcMode->ItemIndex = CalcMode2->ItemIndex;
	CalcModeAlt->ItemIndex = CalcMode2->ItemIndex;
	CalcModeAlt2->ItemIndex = CalcMode2->ItemIndex;
}

// ---------------------------------------------------------------------------

// void TForm1::Segment_Sound(int iter, bool sound) {
void TForm1::Segment_Sound(int startIter, int endIter, bool sound) {
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	CZT_FeatureCalc *calci;
	bool *signal;

	int stop_duration = 0;
	float *peakFr, *entropy, *amplitude, *mfa, *am;
	// float *pitch, *pitchGoodness, *fm, *am, *continuity_t, *continuity_f;
	// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	if (sound == ksong1) {
		// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
		calci = &m_Calculator1;
		peakFr = PeakFreq1;
		entropy = Entropy1;
		amplitude = Amplitude1;
		mfa = mfa1;
		signal = Signal1;
		am = AM1;
		// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	}
	else {
		// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
		calci = &m_Calculator2;
		peakFr = PeakFreq2;
		entropy = Entropy2;
		amplitude = Amplitude2;
		mfa = mfa2;
		signal = Signal2;
		// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	}

	for (int i = startIter; i < endIter; i++) {
		// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
		if (calci->sound_detect_feature(i)) {
			// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
			signal[i] = true;
			stop_duration = 0;
			// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
		}
		else {
			// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
			if (stop_duration <= option->min_stop_duration)
				signal[i] = true;
			else
				signal[i] = false;
			stop_duration++;
			// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
		}

	}
	// calci->parse_bouts();
	// option->amplitude_thresh=amp_thresh;   elena
}

void __fastcall TForm1::diagonalClick(TObject *Sender) {
 	draw_score_sounds();
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::batch_table2_openClick(TObject *Sender) {
	DataForm->TheTable = DataForm->Records1;
	DataForm->openTableClick(this);
    table2L->Caption = DataForm->TheTable->TableName;
    table2_numL->Caption = DataForm->recNum;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::raw_features2Change(TObject *Sender) {
    raw_features1->Text = raw_features2->Text;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::input2Click(TObject *Sender) {
    AnsiString str;
    OpenDialog1->FileName = "not relevant";
    if (OpenDialog1->Execute()) {
        str = ExtractFileDir(OpenDialog1->FileName);
        str += "\\";
        folder->Text = str;
    }
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::folder2_btnClick(TObject *Sender) {
    AnsiString str;
    OpenDialog1->FileName = "not relevant";
    if (OpenDialog1->Execute()) {
        str = ExtractFileDir(OpenDialog1->FileName);
        str += "\\";
        folder2->Text = str;
    }
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::save_periodClick(TObject *Sender) {
    DataForm->similarity->Active = true;
    DataForm->similarity->Append();
    DataForm->similarity->FieldValues["song1"] = songname->Caption;
    DataForm->similarity->FieldValues["start1"] = beginL->Value;
    DataForm->similarity->FieldValues["similarity"] = meanEuclideanDist->Value;
    DataForm->similarity->FieldValues["period"] = median_period->Value;
    DataForm->similarity->FieldValues["random_period"] = random_period->Value;
    DataForm->similarity->Append();
    Beep();
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::save_period2Click(TObject *Sender) {
    DataForm->similarity->Active = true;
    DataForm->similarity->Append();
    DataForm->similarity->FieldValues["song2"] = SongName2a->Caption;
    DataForm->similarity->FieldValues["start2"] = begin2L->Value;
    DataForm->similarity->FieldValues["similarity"] = meanEuclideanDist2->Value;
    DataForm->similarity->FieldValues["period"] = median_period2->Value;
    DataForm->similarity->FieldValues["random_period"] = random_period2->Value;
    DataForm->similarity->Append();
    Beep();
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::calc_period2Click(TObject *Sender) {
    Screen->Cursor = crHourGlass;
    if (Sender == calc_period)
        calc_meanPeriod_and_meanEuclideanDist(ksong2);
    else
        calc_meanPeriod_and_meanEuclideanDist(ksong2);
    Screen->Cursor = crDefault;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::match_toleranceChange(TObject *Sender) {
    match_tolerance_level->Value = match_tolerance->Position;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::ZoomOBMouseDown(TObject *Sender, TMouseButton Button,
    TShiftState Shift, int X, int Y) {
    ZoomOBClick(this);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::ZoomIBMouseDown(TObject *Sender, TMouseButton Button,
    TShiftState Shift, int X, int Y) {
    ZoomIBClick(this);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn29Click(TObject *Sender) {
    string = "There are four batch modes: \r \r";
    string +=
        "Score by pairs select two ordered set A, B, of sound files, and compare A1 against B1,";
    string +=
        "A2 against B2 and so forth. The number of files in A and B must be the same. \r\r";
    string +=
        "M x N comparisons performs all possible comparisons between the sets A and B.";
    string += "The two sets do not have to be of equal length.\r\r";
    string +=
        "Target clusters This mode reads the syllable table, find sound of specific";
    string +=
        "type and compare them to a reference sound. For example, it can be used to compare";
    string +=
        "sounds that belong to one cluster to a target sound (e.g., model song syllable). SA+";
    string +=
        "reads the table, detect a syllable, find the file that contains it. Outline only this";
    string += "syllable and compare it to a target syllable.\r\r";
    string +=
        "Table1 x Table2 compares all the syllables in one table to all the syllables";
    string +=
        "in the other table. SA+ reads the tables, find the file that contains each syllable,";
    string +=
        "and score similarity for all pairs of syllables across the two tables. For example,";
    string +=
        "if table 1 contains several calls of one animal, and table 2 contains several calls";
    string +=
        "from another animals, this mode will compare calls across the two animals.";
    MessageDlgPos(string, mtInformation, TMsgDlgButtons() << mbOK, 0, 100, 100);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::refineClick(TObject *Sender) {
    // if(refine->Checked)option->refine_segmentation=true;
    // else option->refine_segmentation=false;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::raw_featuresClick(TObject *Sender) {
    switch(raw_features->ItemIndex) {
    case 0:
        option->saveRawFile = false;
        raw_features1->ReadOnly = false;
        raw_features1->Color = clWhite;
        break;
    case 1: // save raw features into a table
        option->saveRawFile = true;
        // raw_features1->Text = InputBox("Sound Analysis Pro", "raw features table:", "Type feature table name, starting with a letter");
        raw_features1->ReadOnly = true;
        raw_features1->Color = clYellow;
        m_Calculator1.last_ms_record = 0;
        raw_features1->Text = DataForm->new_raw_table(raw_features1->Text, 0);
        option->features_table = raw_features1->Text;
        DataForm->file_table_name = "file_table_";
        DataForm->file_table_name += raw_features1->Text;
        DataForm->Create_File_Table();
        break;

    }
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::raw_features_2Click(TObject *Sender) {
    raw_features->ItemIndex = raw_features_2->ItemIndex;
    raw_featuresClick(this);
    raw_features2->Text = raw_features1->Text;
}
// ---------------------------------------------------------------------------


void __fastcall TForm1::adjust_pitchClick(TObject *Sender) {
    if (adjust_pitch->Checked)
        option->pitch_averaging->ItemIndex = 1;
    else
        option->pitch_averaging->ItemIndex = 0;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::mfaBClick(TObject *Sender) {
	if (!group->Down)
		hide_all_features(&m_Calculator1);
	switch(deriv_display->ItemIndex)
	{
		case 0:
		m_Calculator1.GetImage().Getmfa().bRender = mfaB->Down;
		m_Calculator1.GetImage().DrawAllSlices(Image1);
		set_scale(0);
		break;

		case 1:
		m_Calculator1.GetContours().Getmfa().bRender = mfaB->Down;
		m_Calculator1.GetContours().DrawAllSlices(Image1);
		set_scale(0);
		break;

		case 2:
		m_Calculator1.GetSonogram().Getmfa().bRender = mfaB->Down;
		m_Calculator1.GetSonogram().DrawAllSlices(Image1);
		set_scale(0);
		break;
	}
	show_outlines(Image1, OnsetSong1, OffsetSong1);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::principal_frBClick(TObject *Sender) {
	TSpeedButton *contour_btn, *group_btns;
    TImage *image;
    TRadioGroup *sono_display;
    CZT_FeatureCalc *calci;
    bool the_song, paginate;
    int onset, offset;
    if (Sender == principal_frB || display->ActivePage == Song1) {
        contour_btn = principal_frB;
        the_song = Ksong1;
        calci = &m_Calculator1;
        group_btns = group;
        paginate = option->paginationOn;
        image = Image1;
        sono_display = deriv_display;
        onset = OnsetSong1;
        offset = OffsetSong1;
    }
    else if (Sender == principal_frB2 || display->ActivePage == Song2) {
        contour_btn = principal_frB2;
        the_song = Ksong2;
        calci = &m_Calculator2;
        group_btns = group2;
        paginate = option->paginationOn2;
        image = Image2;
        sono_display = deriv_display2;
        onset = OnsetSong2;
        offset = OffsetSong2;
    }

    if (contour_btn->Down)
        set_feature_col_highlight(5, the_song);
    else
        set_feature_col_highlight(0, the_song);
    if (!group_btns->Down)
        hide_all_features(calci);


	if (sono_display->ItemIndex == 0)
	{
		calci->GetImage().Getprincipal_frequency().bRender = contour_btn->Down;
		if (!paginate) calci->GetImage().DrawAllSlices(image);
		else calci->GetImage().DrawSlices(0, 1000 - 1, image);
	}

	else if (sono_display->ItemIndex == 1)
	{
		calci->GetContours().Getprincipal_frequency().bRender = contour_btn->Down;
		if (!paginate) calci->GetContours().DrawAllSlices(image);
		else calci->GetContours().DrawSlices(0, 1000 - 1, image);
	}

	else if (sono_display->ItemIndex == 2)
	{
		calci->GetSonogram().Getprincipal_frequency().bRender = contour_btn->Down;
		if (!paginate) calci->GetSonogram().DrawAllSlices(image);
		else calci->GetSonogram().DrawSlices(0, 1000 - 1, image);
	}
	set_scale(the_song);
	show_outlines(image, onset, offset);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::contour_threshChange(TObject *Sender) {
    option->contour_thresh->Position = contour_thresh->Position;
    contour_thresh_val->Value = option->contour_thresh->Position;
    contour_bias->Value = 200 - option->contour_biasTB->Position;
    recalc(0);
    // newBClick(this);

}
// ---------------------------------------------------------------------------

void __fastcall TForm1::contour_biasTBChange(TObject *Sender) {
    option->contour_biasTB->Position = contour_biasTB->Position;
    contour_thresh_val->Value = option->contour_thresh->Position;
    contour_bias->Value = 200 - option->contour_biasTB->Position;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::persistantBClick(TObject *Sender)
// persistant contours...
{
    TSpeedButton *persistant_btn, *group_btns;
    TImage *image;
    TRadioGroup *sono_display;
    CZT_FeatureCalc *calci;
    bool the_song, paginate;
    int onset, offset;
    if (Sender == continulity_tB || display->ActivePage == Song1) {
        persistant_btn = persistantB;
        the_song = Ksong1;
        calci = &m_Calculator1;
        group_btns = group;
        paginate = option->paginationOn;
        image = Image1;
        sono_display = deriv_display;
        onset = OnsetSong1;
        offset = OffsetSong1;
    }
    else if (Sender == continulity_tB2 || display->ActivePage == Song2) {
        persistant_btn = persistantB2;
        the_song = Ksong2;
        calci = &m_Calculator2;
        group_btns = group2;
        paginate = option->paginationOn2;
        image = Image2;
        sono_display = deriv_display2;
        onset = OnsetSong2;
        offset = OffsetSong2;
    }
    if (persistant_btn->Down)
        set_feature_col_highlight(12, the_song);
    else
        set_feature_col_highlight(0, the_song);
    if (!group_btns->Down)
        hide_all_features(calci);

	if (sono_display->ItemIndex == 0)
	{
		calci->GetImage().Getslope().bRender = persistant_btn->Down;
		if (!paginate)calci->GetImage().DrawAllSlices(image);
		else calci->GetImage().DrawSlices(0, 1000 - 1, image);
	}
	else if (sono_display->ItemIndex == 1)
	{
		calci->GetContours().Getslope().bRender = persistant_btn->Down;
		if (!paginate)calci->GetContours().DrawAllSlices(image);
		else calci->GetContours().DrawSlices(0, 1000 - 1, image);
	}
	else if (sono_display->ItemIndex == 2)
	{
		calci->GetSonogram().Getslope().bRender = persistant_btn->Down;
		if (!paginate)calci->GetSonogram().DrawAllSlices(image);
		else calci->GetSonogram().DrawSlices(0, 1000 - 1, image);
	}
	set_scale(the_song);
    show_outlines(image, onset, offset);

}
// ---------------------------------------------------------------------------

void __fastcall TForm1::continulity_tBClick(TObject *Sender) {
    TSpeedButton *contt_btn, *group_btns;
    TImage *image;
    TRadioGroup *sono_display;
    CZT_FeatureCalc *calci;
    bool the_song, paginate, smooth_feature;
    int onset, offset;
    if (Sender == continulity_tB || display->ActivePage == Song1) {
        contt_btn = continulity_tB;
        the_song = Ksong1;
        calci = &m_Calculator1;
        group_btns = group;
        paginate = option->paginationOn;
        smooth_feature = option->smooth_feature1;
        image = Image1;
        sono_display = deriv_display;
        onset = OnsetSong1;
        offset = OffsetSong1;
    }
    else if (Sender == continulity_tB2 || display->ActivePage == Song2) {
        contt_btn = continulity_tB2;
        the_song = Ksong2;
        calci = &m_Calculator2;
        group_btns = group2;
        paginate = option->paginationOn2;
        smooth_feature = option->smooth_feature2;
        image = Image2;
        sono_display = deriv_display2;
        onset = OnsetSong2;
        offset = OffsetSong2;
    }

    if (contt_btn->Down)
        set_feature_col_highlight(10, the_song);
    else
        set_feature_col_highlight(0, the_song);
    if (!group_btns->Down)
        hide_all_features(calci);

	if (sono_display->ItemIndex == 0)
	{
		calci->GetImage().Getcontinuity_t().bRender = contt_btn->Down;
		if (smooth_feature) calci->GetImage().Gethp_small().bRender = contt_btn->Down;
		if (!paginate)calci->GetImage().DrawAllSlices(image);
		else calci->GetImage().DrawSlices(0, 1000 - 1, Image1);
	}
	else if (sono_display->ItemIndex == 1)
	{
		calci->GetContours().Getcontinuity_t().bRender = contt_btn->Down;
		if (smooth_feature) calci->GetContours().Gethp_small().bRender = contt_btn->Down;
		if (!paginate)calci->GetContours().DrawAllSlices(image);
		else calci->GetContours().DrawSlices(0, 1000 - 1, Image1);
	}
	else if (sono_display->ItemIndex == 2)
	{
		calci->GetSonogram().Getcontinuity_t().bRender = contt_btn->Down;
		if (smooth_feature) calci->GetSonogram().Gethp_small().bRender = contt_btn->Down;
		if (!paginate)calci->GetSonogram().DrawAllSlices(image);
		else calci->GetSonogram().DrawSlices(0, 1000 - 1, Image1);
	}
	set_scale(the_song);
	show_outlines(image, onset, offset);
    // show_outlines(Image1, OnsetSong1, OffsetSong1);
    option->doNotSegment = true;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::continuity_fBClick(TObject *Sender) {
    TSpeedButton *contf_btn, *group_btns;
    TImage *image;
    TRadioGroup *sono_display;
    CZT_FeatureCalc *calci;
    bool the_song, paginate, smooth_feature;
    int onset, offset;
    if (Sender == continulity_tB || display->ActivePage == Song1) {
        contf_btn = continuity_fB;
        the_song = Ksong1;
        calci = &m_Calculator1;
        group_btns = group;
        paginate = option->paginationOn;
        smooth_feature = option->smooth_feature1;
        image = Image1;
        sono_display = deriv_display;
        onset = OnsetSong1;
        offset = OffsetSong1;
    }
    else if (Sender == continulity_tB2 || display->ActivePage == Song2) {
        contf_btn = continuity_fB2;
        the_song = Ksong2;
        calci = &m_Calculator2;
        group_btns = group2;
        paginate = option->paginationOn2;
        smooth_feature = option->smooth_feature2;
        image = Image2;
        sono_display = deriv_display2;
        onset = OnsetSong2;
        offset = OffsetSong2;
    }

    if (contf_btn->Down)
        set_feature_col_highlight(11, the_song);
    else
        set_feature_col_highlight(0, the_song);
    if (!group_btns->Down)
        hide_all_features(calci);

	if (sono_display->ItemIndex == 0)
	{
		calci->GetImage().Getcontinuity_f().bRender = contf_btn->Down;
		if (smooth_feature)calci->GetImage().Gethp_small().bRender = contf_btn->Down;
		if (!paginate) calci->GetImage().DrawAllSlices(image);
		else calci->GetImage().DrawSlices(0, 1000 - 1, image);
	}
	else if (sono_display->ItemIndex == 1)
	{
		calci->GetContours().Getcontinuity_f().bRender = contf_btn->Down;
		if (smooth_feature)calci->GetContours().Gethp_small().bRender = contf_btn->Down;
		if (!paginate) calci->GetContours().DrawAllSlices(image);
		else calci->GetContours().DrawSlices(0, 1000 - 1, image);
	}
	else if (sono_display->ItemIndex == 2)
	{
		calci->GetSonogram().Getcontinuity_f().bRender = contf_btn->Down;
		if (smooth_feature)calci->GetSonogram().Gethp_small().bRender = contf_btn->Down;
		if (!paginate) calci->GetSonogram().DrawAllSlices(image);
		else calci->GetSonogram().DrawSlices(0, 1000 - 1, image);
	}
    set_scale(the_song);
    show_outlines(image, onset, offset);
    option->doNotSegment = true;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::mfaB2Click(TObject *Sender) {
	if (!group2->Down)
		hide_all_features(&m_Calculator2);

	switch(deriv_display2->ItemIndex)
	{
		case 0:
		m_Calculator2.GetImage().Getmfa().bRender = mfaB2->Down;
		m_Calculator2.GetImage().DrawAllSlices(Image2);
		set_scale(1);
		break;

		case 1:
		m_Calculator2.GetContours().Getmfa().bRender = mfaB2->Down;
		m_Calculator2.GetContours().DrawAllSlices(Image2);
		set_scale(1);
		break;

		case 2:
		m_Calculator2.GetSonogram().Getmfa().bRender = mfaB2->Down;
		m_Calculator2.GetSonogram().DrawAllSlices(Image2);
		set_scale(1);
		break;
	}
	show_outlines(Image2, OnsetSong2, OffsetSong2);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::continulity_tB2Click(TObject *Sender) {
	if (!group2->Down)
		hide_all_features(&m_Calculator2);

    switch(deriv_display2->ItemIndex)
	{
		case 0:
		m_Calculator2.GetImage().Getcontinuity_t().bRender = continulity_tB2->Down;
		m_Calculator2.GetImage().DrawAllSlices(Image2);
		set_scale(1);
		break;

		case 1:
		m_Calculator2.GetContours().Getcontinuity_t().bRender = continulity_tB2->Down;
		m_Calculator2.GetContours().DrawAllSlices(Image2);
		set_scale(1);
		break;

		case 2:
		m_Calculator2.GetSonogram().Getcontinuity_t().bRender = continulity_tB2->Down;
		m_Calculator2.GetSonogram().DrawAllSlices(Image2);
		set_scale(1);
		break;
	}
	show_outlines(Image2, OnsetSong2, OffsetSong2);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::contour_thresh2Change(TObject *Sender) {
    option->contour_thresh->Position = contour_thresh2->Position;
    contour_thresh_val2->Value = option->contour_thresh->Position;
    contour_bias2->Value = 200 - option->contour_biasTB->Position;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::contour_biasTB2Change(TObject *Sender) {
    option->contour_biasTB->Position = contour_biasTB2->Position;
    contour_thresh_val2->Value = option->contour_thresh->Position;
    contour_bias2->Value = 200 - option->contour_biasTB->Position;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn31Click(TObject *Sender) {
    string =
        "Segmented similarity has both advantages and disadvantages compared to the non-segmented version. Shortly:";
    string +=
        "1. Segmented similarity is less powerful It provides a gross similarity estimate.\r";
    string +=
        "2. Score is only valid for durations within natural syllable units.\r";
    string += "3. The order of sounds within the syllable does not matter.\r";
    string +=
        " -------------------------------------------------------------.\r";
    string += "Instructions:\r";
    string += " 1. Outline a syllable in sound 1. \r ";
    string += "2. Outline a syllable in sound 2.\r";
    string += "3. Selected syllables are now scored && aligned.\r";
    string += "4. Save the score if so desired.\r";

    MessageDlgPos(string, mtInformation, TMsgDlgButtons() << mbOK, 0, 140, 162);
}
// ---------------------------------------------------------------------------


void __fastcall TForm1::ApplicationEvents1Idle(TObject *Sender, bool &Done) {

    static AnsiString oldFile = "nothing", oldFile1 = "nothing";
    AnsiString FileName1, FileName2;
    try {
        if (dobatch) {
            st->Brush->Color = clRed;

            // this is the pairwise comparisons mode
            if (!MtimesN->ItemIndex && List1->ItemIndex + 1 <=
                List1->Items->Count) {
                Analyze_song1(List1->Items->Strings[List1->ItemIndex], 0);
                Analyze_song2(List2->Items->Strings[List2->ItemIndex], 0);
                ScoreBClick(this);
                string = List1->Items->Strings[List1->ItemIndex];
                string += " : ";
                string += List2->Items->Strings[List2->ItemIndex];
                string += " : ";
                string += scoreN->Value;
				batch_log->Lines->Add(string);
                save_scoreClick(this);
                ScoreBClick(this); // reset
                if (List1->ItemIndex + 1 == List1->Items->Count ||
                    List2->ItemIndex + 1 == List2->Items->Count)
                    dobatch = false;
                else {
                    List1->ItemIndex++;
                    List2->ItemIndex++;
                }

            }

            // this is the MxN comparisons mode
            else if (MtimesN->ItemIndex == 1 &&
                (List1->ItemIndex < List1->Items->Count - 1 ||
                    List2->ItemIndex < List2->Items->Count - 1)) {
                if (List2->ItemIndex == -1) {
                    List1->ItemIndex = 0;
                    List2->ItemIndex = 0;
                }
                else if (List1->ItemIndex == List1->Items->Count - 1) {
                    List1->ItemIndex = 0;
                    List2->ItemIndex++;

                }
                else
                    List1->ItemIndex++;

                Analyze_song1(List1->Items->Strings[List1->ItemIndex], 0);
                Analyze_song2(List2->Items->Strings[List2->ItemIndex], 0);
                ScoreBClick(this);
                save_scoreClick(this);
                string = List1->Items->Strings[List1->ItemIndex];
                string += " : ";
                string += List2->Items->Strings[List2->ItemIndex];
                string += " : ";
				string += scoreN->Value;
                batch_log->Lines->Add(string);
                ScoreBClick(this); // reset
            }

            // this is the table syllable target mode
            else if (MtimesN->ItemIndex == 2 && the_iteration <
                table_iterate && the_iteration < target_syllable_num) {
                the_iteration++;
                AnsiString str = folder->Text;
                str += DataForm->MyQuery1->FieldValues["file_name"];
                m_Calculator2.render_image = true;
                if (str != oldFile) {
                    if (!FileExists(str)) {
                        dobatch = false;
                        MessageDlg(
                            "could not find the file -- please change destination folder and click 'resume'", mtWarning, TMsgDlgButtons() << mbOK, 0);
                    }
                    else
                        Analyze_song2(str, 1);
                }
                if (dobatch) {
                    oldFile = str;
                    DerivPowerBClick(this);
                    hide_all_features(&m_Calculator2);
                    // GUI, do not show feature curves
                    ready2 = true;
                    OnsetSong2 = (float)DataForm->MyQuery1->FieldValues
                        ["start_on"] / table_ms_start - table_buffer;
                    OffsetSong2 = OnsetSong2 + (float)
                        DataForm->MyQuery1->FieldValues["duration"]
                        / table_ms_duration + table_buffer;
                    begin2L->Value = OnsetSong2;
                    draw_score_sounds();
                    ScoreBClick(this);
                    save_scoreClick(this);
                    ScoreBClick(this); // reset
                    DataForm->MyQuery1->Next();
                }
            }

            // this is the table1 x table2 mode
            else if (MtimesN->ItemIndex == 3 &&
                (DataForm->Records->RecNo < DataForm->Records->RecordCount ||
                    DataForm->Records1->RecNo < DataForm->Records1->RecordCount)
                ) {

                CalcMode->ItemIndex = 0;
                CalcModeClick(this);
                FileName1 = folder->Text;
                FileName1 += DataForm->Records->FieldValues["file_name"];
                FileName2 = folder->Text;
                FileName2 += DataForm->Records1->FieldValues["file_name"];
                m_Calculator1.render_image = true;
                m_Calculator2.render_image = true;

                if (FileName1 != oldFile) {
                    if (!FileExists(FileName1)) {
                        dobatch = false;
                        MessageDlg(
                            "could not find the file -- please change destination folder and click 'resume'", mtWarning, TMsgDlgButtons() << mbOK, 0);
                    }
                    else
                        Analyze_song1(FileName1, 1);
                }

                if (FileName2 != oldFile1) {
                    if (!FileExists(FileName2)) {
                        dobatch = false;
                        MessageDlg(
                            "could not find the file -- please change destination folder and click 'resume'", mtWarning, TMsgDlgButtons() << mbOK, 0);
                    }
                    else
                        Analyze_song2(FileName2, 1);
                }

                if (dobatch) {
                    oldFile = FileName1;
                    oldFile1 = FileName2;
                    // bug?	DerivPowerBClick(this);
                    // DerivPowerBClick(this);
                    hide_all_features(&m_Calculator1);
                    // GUI, do not show feature curves
                    hide_all_features(&m_Calculator2);
                    // GUI, do not show feature curves
                    ready = true;
                    ready2 = true;

                    OnsetSong1 = (float)DataForm->Records->FieldValues
                        ["start_on"] / table_ms_start - table_buffer;
                    OffsetSong1 = OnsetSong1 + (float)
                        DataForm->Records->FieldValues["duration"]
                        / table_ms_duration + table_buffer;
                    OnsetSong2 = (float)DataForm->Records1->FieldValues
                        ["start_on"] / table_ms_start - table_buffer;
                    OffsetSong2 = OnsetSong2 + (float)
                        DataForm->Records1->FieldValues["duration"]
                        / table_ms_duration + table_buffer;
                    beginL->Value = OnsetSong1;
                    begin2L->Value = OnsetSong2;
					draw_score_sounds();
                    ScoreBClick(this);
                    save_scoreClick(this);
                    ScoreBClick(this); // reset

                    if
                        (DataForm->Records->RecNo <
                        DataForm->Records->RecordCount)
                        DataForm->Records->Next();
                    else if
                        (DataForm->Records1->RecNo <
                        DataForm->Records1->RecordCount) {
                        DataForm->Records->First();
                        DataForm->Records1->Next();
                    }
                    else
                        dobatch = false;
                }
            }

            else
                dobatch = false;

        }
        else
            st->Brush->Color = clGreen;
    }
    catch(...) {
        // doit=false;
		dobatch = false;
        MessageDlg("an exception has occured, could not finish the batch",
            mtWarning, TMsgDlgButtons() << mbOK, 0);

    }
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::autosave_imageClick(TObject *Sender) {
    autosave_image2->Checked = autosave_image->Checked;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::autosave_image2Click(TObject *Sender) {
    autosave_image->Checked = autosave_image2->Checked;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::delete_similarity_recsClick(TObject *Sender) {
    if (warnings->Checked)
        if (MessageDlg("Are you sure? All similarity records will be deleted!",
                mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0) == mrYes)
        {
            DataForm->similarity->Active = false;
            DataForm->similarity->EmptyTable();
            DataForm->similarity->Active = true;
            Beep();
        }
}
// ---------------------------------------------------------------------------


void __fastcall TForm1::clustersClick(TObject *Sender) {
	TImage *image;
    TLabel *name;
    TNumLab *advance;
	if (Sender == clusters || display->ActivePage==Song1) {
        image = Image1;
        name = songname;
        advance = advanceTb;
    }
    else {
        image = Image2;
        name = SongName2a;
        advance = advanceTb2;
    }
    str = "select start_on, duration, cluster from ";
    str += option->syllable_table_name;
    str += " where file_name like'";
    str += name->Caption;
    str += "';";
    DataForm->MyQuery1->SQL->Clear();
    DataForm->MyQuery1->SQL->Add(str);
    DataForm->MyQuery1->Execute();
    int found = DataForm->MyQuery1->RecordCount;
    if (!found)
        MessageDlg(
        "Could not find syllable table for this file. If you already made one click 'change table' otherwise, click 'new table'. Note that for clustering, it is recomended to have at least 50 syllables in your table", mtWarning, TMsgDlgButtons() << mbOK, 0);
    else {
        int clust_found = 0;
        for (int i = 1; i <= found; i++) {
            int clust = DataForm->MyQuery1->FieldValues["cluster"];
            if (clust) {
                clust_found++;
                int clust_start = DataForm->MyQuery1->FieldValues["start_on"];
                int clust_duration = DataForm->MyQuery1->FieldValues
                    ["duration"];
                clust_start = (float)clust_start / advance->Value;
                clust_duration = (float)clust_duration / advance->Value;
                TColor clust_color;
                switch(clust) {
				case 1:
                    clust_color = clRed;
                    break;
                case 2:
                    clust_color = clBlue;
                    break;
                case 3:
					clust_color = clLime;
                    break;
                case 4:
					clust_color = clYellow;
                    break;
                case 5:
                    clust_color = (TColor)RGB(0, 0, 0);
                    break;
                case 6:
                    clust_color = clFuchsia;
                    break;
                case 7:
					clust_color = clOlive;
                    break;
                case 8:
					clust_color = clWhite;
                    break;
                case 9:
                    clust_color = clGreen;
                    break;
				case 10:
					clust_color = clAqua;
					break;
				case 11:
					clust_color = TColor(RGB(180,250,250));
					break;
				case 12:
					clust_color = TColor(RGB(250,180,250));
					break;
				case 13:
					clust_color = TColor(RGB(250,250,180));
					break;
				case 14:
					clust_color = TColor(RGB(180,180,250));
					break;
				case 15:
					clust_color = TColor(RGB(180,250,180));
					break;
				case 16:
					clust_color = TColor(RGB(100,100,100));
					break;
                default:
                    clust_color = clGray;
                    break;
                }

                image->Canvas->Brush->Color = clust_color;
                image->Canvas->FillRect(Rect(clust_start, 10,
                        clust_start + clust_duration, 15));
            } // end if clust
            DataForm->MyQuery1->Next();
        } // end for
        if (!clust_found) {
            MessageDlg(
                "I found syllable records, but I could not find clusters for this file."
                , mtWarning, TMsgDlgButtons() << mbOK, 0);
            // here we open the clustering module
            // StartForm->clustersClick(this);
        }
    }

}
// ---------------------------------------------------------------------------

void __fastcall TForm1::cluster_tableClick(TObject *Sender) {
	option->quick_cluster = true;
	StartForm->clustersClick(this);
	Form1->BringToFront();
	clustersClick(this);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::pitch_methodClick(TObject *Sender) {
    option->pitch_method->ItemIndex = pitch_method->ItemIndex;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::age_or_dayClick(TObject *Sender) {
    if (age_or_day->ItemIndex)
        option->age_in_table = false;
    else
        option->age_in_table = true;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::frequency_rangeChange(TObject *Sender) {
    bool snd;
    if (Sender == frequency_range)
        snd = 0;
    else
        snd = 1;
    option->frequency_range->Position = frequency_range->Position;
    option->frequency_rangeChange(this);

    int i = pow(2, 4 - frequency_range->Position);
    frequency_range_val->Value = option->sampling_rate / i;
    frequency_range2->Position = frequency_range->Position;
    frequency_range2_val->Value = frequency_range_val->Value;

    switch(frequency_range->Position) {
    case 1: // show quater of the range of FFT
        // frequency_range_val->Value=5000;
        option->range_x2 = false;
        option->range_x05 = true;
        update_scale();
        break;

    case 2: // show half of the range of FFT
        // frequency_range_val->Value=10000;
        option->range_x2 = false;
        option->range_x05 = false;
        update_scale();
        break;

    case 3: // show all the range of FFT (well, almost)
        // frequency_range_val->Value=20000;
        option->range_x2 = true;
        option->range_x05 = false;
        update_scale();
        break;

    }
     option->frequency = frequency_range_val->Value;
    // DataForm->Settings(1, "frequency_range", frequency_range->Position);
    recalc(snd);
    // derivTBChange(this);
}

/*
void __fastcall TForm1::locationChange(TObject *Sender) {
bool snd;
if (Sender == locationTB)
snd = 0;
else
snd = 1;
// option->frequency_range->Position = frequency_range->Position;
// option->frequency_rangeChange(this);
locationE->Text = locationTB->Position;
option->calculateLocation = atoi(locationE->Text.t_str());
// Location->Min=intervalSize->Position;
// Iterations->Max=intervalSize->Position/10;
// DataForm->Settings(1, "frequency_range", frequency_range->Position);
recalc(snd);

}
 */
void TForm1::recalc(bool snd) {
//OFER????
	if (FileExists(open_sound->FileName2) && snd ) {
        //if (snd)
            update_display2->Visible = true;
    } else if (FileExists(open_sound->FileName) && !snd ) {
            update_display->Visible = true;
    }
}
// ---------------------------------------------------------------------------

void TForm1::recalc_light(bool snd) {
    if (FileExists(open_sound->FileName)) {
        Update_Display_light(snd);
    }
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::frequency_range2Change(TObject *Sender) {
    frequency_range->Position = frequency_range2->Position;
    int i = pow(2, 4 - frequency_range->Position);
    frequency_range2_val->Value = option->sampling_rate / i;
    option->frequency = frequency_range2_val->Value;
    // frequency_range2_val->Value=frequency_range_val->Value;
    frequency_rangeChange(this);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::new_birdClick(TObject *Sender) {
    TStaticText *b_name, *b_age, *b_model, *b_training, *b_details;

    if (Sender == new_bird) {
        b_name = bird_name;
        b_age = age;
        // b_model=model;
        // b_training=training_age;
        b_details = details;
    }

    else {
        b_name = bird_name2;
        b_age = age2;
        b_details = details2;
    }
    option->bird_ID = b_name->Caption;
    animals_form->datapages->ActivePageIndex = 0;
    if (animals_form->ShowModal() == mrOk) {
        b_name->Caption = animals_form->name->Text;
        b_age->Caption = animals_form->age->Value;
        b_details->Caption = animals_form->bird_comments->Text;
    }
    if (animals_form->new_bird)
        animals_form->Insert_bird_record();
	animals_form->Animals_to_settings(1);
    if (Sender == new_bird)
        set_tables(0);
    else
        set_tables(1);
}
// ---------------------------------------------------------------------------

void TForm1::set_tables(bool bird) {
    bool success = true;
    // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
    table_name->Caption = DataForm->Retrieve(1, "syllable_table");
    // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
    if (!DataForm->table_exists(table_name->Caption)) {
        // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
        try {
            DataForm->Create_table(table_name->Caption);
        }
        catch(...) {
            success = false;
            RecordsNum->Caption = "NA";
        }
        // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
    }
    if (success) {
        if (!bird) // bird 1
        {
            // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
            RecordsNum->Caption = DataForm->count_records(table_name->Caption);
            // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
            DataForm->TheTable->TableName = table_name->Caption;
            // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
            option->syllable_table_name = table_name->Caption;
            // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
            raw_features1->Text = "raw_" + bird_name->Caption;
            // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
            option->features_table = raw_features1->Text;
            // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
            DataForm->file_table_name = "file_table_" + bird_name->Caption;
        }
        else // bird 2
        {
            // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
            RecordsNum2->Caption = DataForm->count_records
                (table_name2->Caption);
            // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
            DataForm->TheTable->TableName = table_name2->Caption;
            // myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
            option->syllable_table_name = table_name2->Caption;
			// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
			raw_features2->Text = "raw_" + bird_name2->Caption;
			// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
			option->features_table = raw_features2->Text;
			// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
			DataForm->file_table_name = "file_table_" + bird_name2->Caption;
			// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
		}
		// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
		DataForm->Song1->TableName = DataForm->Records->TableName;
		// myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; //myfile.flush();
	}
}

void TForm1::set_scale(bool image) {
	TImage* Image;
	TBevel* Axis;
	int itr;
	int *pageNum;
	int page, currentStartLoc;
	float adv;
	int *location;
	bool *pagination_On;
	float fft_step;
	TImage *preview_sonogram;


	if (!image) {
		Image = Image1;
	   // Axis = x_axis;
		itr = Iterations1;
		currentStartLoc=option->currentStartLocation;
		adv=advanceTb->Value;
		pagination_On = &option->paginationOn;
		fft_step = advanceTb->Value;
		location = &option->currentStartLocation;
		pageNum = &option->pageNumber;
		preview_sonogram = preview_sono;
	}
	else {
		Image = Image2;
	//    Axis = x_axis2;
		itr = Iterations2;
		adv=advanceTb2->Value;
		currentStartLoc=option->currentStartLocation2;
		pagination_On = &option->paginationOn2;
		fft_step = advanceTb2->Value;
		location = &option->currentStartLocation2;
		pageNum = &option->pageNumber2;
		preview_sonogram = preview_sono2;
	}

   // Axis->Width = itr;
	Image->Picture->Bitmap->Height = 280;
	Image->Canvas->Font->Size = 9;
	//int tick_num = itr / 200;
	//if (tick_num == 0) {
	int tick_num;
	if (*pagination_On)	tick_num = 5;
	else tick_num = Image->Width/200;
	//}
	int tick_val = 200.0 * (float)fft_step;
	if (preview_sonogram->Visible ) {
	   page = (*location)/1000 - 1;
	} else {
	  page = (*pageNum) - 1;
	}
	if (page < 1) {
		page = 0;
	}

	int start_ms, end_ms, diff_ms;
	for (int i = 0; i <= tick_num; i++) {
		// str = (int)((float)tick_val * ((float)((option->pageNumber-1)*tick_num)+(float)i));
		////myfile<<"tick_num: "<<tick_num<<" tick_val: "<<tick_val<<" i: "<<i<< "page num: "<<option->pageNumber-1<<"\n";

		if (*pagination_On) {
			//str = (int)((float)tick_val * ((float)page * tick_num + (float)i));
			start_ms = ((float) currentStartLoc - 1000.0) * adv;
			end_ms = 1000 * adv + ((float) currentStartLoc - 1000.0) * adv;
			diff_ms=(end_ms-start_ms)/5;
			str=start_ms+i*diff_ms;

		}
		else {
			str = (int)((float)tick_val * (float)i);
		}
		Image->Canvas->TextOut(200 * i, 251, "        ");
		Image->Canvas->TextOut(200 * i, 251, str);
	}

}

void TForm1::note_dur() {
	int *note_durs;
	int ref, counter;
	float diff;
	float similarity_reject_thresh = state_transition_Num->Value;
	// calc_thresh(state_transition_TB->Position);
	note_durs = new int[Iterations1];
	for (int i = 10; i < Iterations1; i++)
		note_durs[i] = 0;

    for (int i = 10; i < Iterations1 - 10; i++) {
        if (Signal1[i]) {
            // go forwards until hit threshold of dis-similarity
            ref = 0;
            counter = 0;
            int missmatches = 0;
            do {
                ref++;
                counter++;
                diff = pow(PitchS1[i] - PitchS1[i + ref], 2) + pow
                    (FMS1[i] - FMS1[i + ref], 2) + pow
                    (EntropyS1[i] - EntropyS1[i + ref], 2) + pow
                    (PitchGoodnessS1[i] - PitchGoodnessS1[i + ref], 2);
                if (diff > similarity_reject_thresh)
                    missmatches++;
                else
                    missmatches = 0;
            }
            while (Signal1[i + ref] && missmatches < missmatchesTb->Position &&
                i + ref < Iterations1);
            note_durs[i] += ref;

            // go backwords until hit threshold of dis-similarity
            ref = 0;
            counter = 0;
            missmatches = 0;
            do {
                ref--;
                counter++;
                diff = pow(PitchS1[i] - PitchS1[i + ref], 2) + pow
                    (FMS1[i] - FMS1[i + ref], 2) + pow
                    (EntropyS1[i] - EntropyS1[i + ref], 2) + pow
                    (PitchGoodnessS1[i] - PitchGoodnessS1[i + ref], 2);
                if (diff > similarity_reject_thresh)
                    missmatches++;
                else
                    missmatches = 0;
            }
            while (Signal1[i + ref] && missmatches < missmatchesTb->Position &&
                i + ref > 1);
            note_durs[i] -= ref; // ref is now negative, so we actually add it...
        }
        Memo1->Lines->Add(note_durs[i]);
    }
    delete[]note_durs;
}

void __fastcall TForm1::calcnoteClick(TObject *Sender) {
    note_dur();
    display->ActivePage = options;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::state_transition_TBChange(TObject *Sender) {
    // float x=calc_thresh(float(state_transition_TB->Position)/10);
    state_transition_Num->Value = (float)state_transition_TB->Position / 10.0;
    // x;
    option->state_transition_Num->Value = state_transition_Num->Value;
    recalc(0);
    recalc(1);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::missmatchesTbChange(TObject *Sender) {
    missmatchesNum->Value = missmatchesTb->Position;
    option->missmatchesTb->Position = missmatchesTb->Position;
    recalc(0);
    recalc(1);
}
// ----------------------------------------------------------------------------

void __fastcall TForm1::BitBtn37Click(TObject *Sender) {
    Application->HelpContext(900);
}
// ---------------------------------------------------------------------------


void __fastcall TForm1::help5Click(TObject *Sender) {
    Application->HelpContext(310);
}


void __fastcall TForm1::smoothyClick(TObject *Sender) {
    if (smoothy->Checked)
        smooth->Checked = true;
    else
        smooth->Checked = false;
    recalc(0);
    recalc(1);
    // MessageDlg("This change will take effect only the next time you open a sound",mtWarning, TMsgDlgButtons() << mbOK, 0);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::calc_rhythmClick(TObject *Sender) {
    int range, samp;
    switch(rhythm_range->ItemIndex) {
    case 0:
        range = 200;
        samp = 2;
        break;
    case 1:
        range = 500;
        samp = 5;
        break;
    case 2:
        range = 5000;
        samp = 50;
        break;
    }

    float* r = m_Calculator1.Calculate_rhythm(rhythm_range->ItemIndex,
        rhythm_feature->ItemIndex, rhythm_filter->ItemIndex);

    rhythm_chart->ClearGraf();
    rhythm_chart->DataColor = clRed;
    rhythm_chart->MoveTo(0, 0);
    float Xr, maxY = 0;
    str = "";

    for (int i = 1; i < range; i++) // we now process those records:
    {
        Xr = (float)i * 0.030517578 / advanceTb->Value;
        rhythm_chart->DrawTo(Xr, r[i]);
        if (maxY < r[i])
            maxY = r[i];
        if (!(i % (2 * samp))) {
            float a = 0;
            for (int j = 0; j < (2 * samp); j++)
                a += r[i];
            str += a / (2 * samp);
            str += " , ";
        }
    }
    if (print_rhythm->Checked)
        rhythm_data->Lines->Add(str);
    rhythm_chart->Scale1X->RangeHigh = Xr;
    rhythm_chart->Scale1Y->RangeHigh = maxY;
    rhythm_chart->Scale1X->RangeLow = 0;
    rhythm_chart->Scale1Y->RangeLow = 0;

    rhythm_chart->ShowGraf();
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::zoomClick(TObject *Sender) {
    switch(zoom->ItemIndex) {
    case 0:
        rhythm_chart->MouseAction = maZoomDrag;
        break;
    case 1:
        rhythm_chart->MouseAction = maPan;
        break;
    case 2:
        rhythm_chart->MouseAction = maZoomWind;
        break;
    }
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::open_viewerClick(TObject *Sender) {
    forward->Enabled = true;
    backwards->Enabled = false;
    advance_page = true;
    Image10->Canvas->Rectangle(0, 0, Image10->Width, Image1->Height);
    Image11->Canvas->Rectangle(0, 0, Image10->Width, Image1->Height);
    Image12->Canvas->Rectangle(0, 0, Image10->Width, Image1->Height);
    Image13->Canvas->Rectangle(0, 0, Image10->Width, Image1->Height);
    Image14->Canvas->Rectangle(0, 0, Image10->Width, Image1->Height);
    wave.ResetContents();
    m_Calculator1.brightness = -0.00000001 * gain3->Position;
    if (open_sound->ShowModal() == mrOk) // ->Execute())
    {
        active_view = true;
        GUI = true;
        m_Calculator1.newFile = open_sound->FileName;
        // CWavIterator wave;
        CZT_PcmEq equalizer;
        wave.UseFilter(equalizer);
        m_Calculator1.RenderImage(1);
        m_Calculator1.render_image = true;
        str = open_sound->FileName;
        if (wave.OpenFile(str.c_str()) == pcmpos_error)
            Application->MessageBox(L"Error: could not open wave file", NULL,
            MB_OK); // != pcmpos_error)
        else {
            page->Value = 1;
            m_Calculator1.init_viewer(wave);
            if (!m_Calculator1.viewer(wave, 1000, 0)) {
                forward->Enabled = false;
                m_Calculator1.GetImage().DrawViewerSlices(Image10);
                return;
            }
            else
                m_Calculator1.GetImage().DrawViewerSlices(Image10);

            if (!m_Calculator1.viewer(wave, 1000, 0)) {
                forward->Enabled = false;
                m_Calculator1.GetImage().DrawViewerSlices(Image11);
                return;
            }
            else
                m_Calculator1.GetImage().DrawViewerSlices(Image11);

            if (!m_Calculator1.viewer(wave, 1000, 0)) {
                forward->Enabled = false;
                m_Calculator1.GetImage().DrawViewerSlices(Image12);
                return;
            }
            else
                m_Calculator1.GetImage().DrawViewerSlices(Image12);

            if (!m_Calculator1.viewer(wave, 1000, 0)) {
                forward->Enabled = false;
                m_Calculator1.GetImage().DrawViewerSlices(Image13);
                return;
            }
            else
                m_Calculator1.GetImage().DrawViewerSlices(Image13);

            if (!m_Calculator1.viewer(wave, 1000, 0)) {
                forward->Enabled = false;
                m_Calculator1.GetImage().DrawViewerSlices(Image14);
                return;
            }
            else
                m_Calculator1.GetImage().DrawViewerSlices(Image14);

            // m_Calculator.init_viewer(wave);
            /*
            if(calci->Calculate(wave,0) && GUI)
            {
            calci->render_image=true;
            calci->GetImage().DrawAllSlices(Image);
            Sonogram->Assign(Image1->Picture->Bitmap);
            } */
        }
    }

    // at some point need to waves[0].ResetContents();
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::forwardClick(TObject *Sender) {
    backwards->Enabled = true;
    Image10->Canvas->Rectangle(0, 0, Image10->Width, Image1->Height);
    Image11->Canvas->Rectangle(0, 0, Image10->Width, Image1->Height);
    Image12->Canvas->Rectangle(0, 0, Image10->Width, Image1->Height);
    Image13->Canvas->Rectangle(0, 0, Image10->Width, Image1->Height);
    Image14->Canvas->Rectangle(0, 0, Image10->Width, Image1->Height);
    if (m_Calculator1.position > m_Calculator1.nFileTotal +
        (option->ztSamplesPerWindow * 2))
        return;
    // page->Value++;
    CZT_PcmEq equalizer;
    wave.UseFilter(equalizer);
    if (m_Calculator1.viewer(wave, 1000, 0)) {
        if (advance_page)
            page->Value++;
        m_Calculator1.GetImage().DrawViewerSlices(Image10);
    }
    else {
        m_Calculator1.GetImage().DrawViewerSlices(Image10);
        forward->Enabled = false;
        return;
    }
    if (m_Calculator1.viewer(wave, 1000, 0))
        m_Calculator1.GetImage().DrawViewerSlices(Image11);
    else {
        m_Calculator1.GetImage().DrawViewerSlices(Image11);
        forward->Enabled = false;
        return;
    }
    if (m_Calculator1.viewer(wave, 1000, 0))
        m_Calculator1.GetImage().DrawViewerSlices(Image12);
    else {
        m_Calculator1.GetImage().DrawViewerSlices(Image12);
        forward->Enabled = false;
        return;
    }
    if (m_Calculator1.viewer(wave, 1000, 0))
        m_Calculator1.GetImage().DrawViewerSlices(Image13);
    else {
        m_Calculator1.GetImage().DrawViewerSlices(Image13);
        forward->Enabled = false;
        return;
    }
    m_Calculator1.viewer(wave, 1000, 0);
    m_Calculator1.GetImage().DrawViewerSlices(Image14);
    advance_page = true;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::backwardsClick(TObject *Sender) {
    forward->Enabled = true;
    if (page->Value == 1)
        return;
    if (m_Calculator1.position >= 10000 * option->Advance) {
        m_Calculator1.pcmPos -= (10000 * option->Advance);
        m_Calculator1.position -= (10000 * option->Advance);
        page->Value--;
        if (page->Value == 1)
            backwards->Enabled = false;
    }
    else if (m_Calculator1.position > 0) {
        m_Calculator1.pcmPos -= m_Calculator1.position;
        m_Calculator1.position = 0;
        page->Value--;
        if (page->Value == 1)
            backwards->Enabled = false;
    }
    else
        return;
    // return;
    CZT_PcmEq equalizer;

    // else page->Value--;
    wave.UseFilter(equalizer);
    m_Calculator1.viewer(wave, 1000, 0);
    m_Calculator1.GetImage().DrawViewerSlices(Image10);
    m_Calculator1.viewer(wave, 1000, 0);
    m_Calculator1.GetImage().DrawViewerSlices(Image11);
    m_Calculator1.viewer(wave, 1000, 0);
    m_Calculator1.GetImage().DrawViewerSlices(Image12);
    m_Calculator1.viewer(wave, 1000, 0);
    m_Calculator1.GetImage().DrawViewerSlices(Image13);
    m_Calculator1.viewer(wave, 1000, 0);
    m_Calculator1.GetImage().DrawViewerSlices(Image14);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::advance_viewerChange(TObject *Sender) {
    advance->Position = advance_viewer->Position;
    advance_viewerTb->Value = advanceTb->Value;
    if (active_view) {
        m_Calculator1.pcmPos -= (5000 * option->Advance);
        m_Calculator1.position -= (5000 * option->Advance);
        forwardClick(this);
    }
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::gain3Change(TObject *Sender) {
    // float bright=exp((float)gain3->Position/1500);
    // m_Calculator1.GetImage().SetBrightness(bright);
    m_Calculator1.brightness = -0.00000001 * gain3->Position;
    if (active_view) {
        if (m_Calculator1.position >= 5000 * option->Advance) {
            m_Calculator1.pcmPos -= (5000 * option->Advance);
            m_Calculator1.position -= (5000 * option->Advance);
        }
        else {
            m_Calculator1.pcmPos -= m_Calculator1.position;
            m_Calculator1.position = 0;
        }
        advance_page = false;
        forwardClick(this);
    }
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::viewer_rangeChange(TObject *Sender) {
    frequency_range->Position = viewer_range->Position;
    viewer_freq->Value = frequency_range_val->Value;
    if (active_view) {
        m_Calculator1.pcmPos -= (5000 * option->Advance);
        m_Calculator1.position -= (5000 * option->Advance);
        forwardClick(this);
    }
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::add_raw_featuresClick(TObject *Sender) {
	AnsiString str1, str2;
    str = "";
    int dur;
    float tm;
    int *start;
	int *state;
	int *feature_vector_curs;
	int onset,offset;
	float *Amplitude, *Pitch, *PeakFreq, *principal_frequency, *PitchGoodness,
	*FM, *AM, *Entropy, *continuity_t, *continuity_f, *FMc;
	TReportListView *feature_vector_l;

    if (Sender == add_raw_features || display->ActivePage == Song1) {
       start = &option->currentStartLocation;
	   feature_vector_l = feature_vector_list;
	   feature_vector_curs=&feature_vector_cursor;
	   state = &state1;
	   onset=OnsetSong1; offset= OffsetSong1;
	   Amplitude=Amplitude1; Pitch=Pitch1; PeakFreq=PeakFreq1;
	   principal_frequency=principal_frequency1;  PitchGoodness=PitchGoodness1;
	   FM=FM1; AM=AM1; Entropy=Entropy1; continuity_t=continuity_t1;
	   continuity_f=continuity_f1; FMc=FMc1;
	} else {
	   start = &option->currentStartLocation2;
	   feature_vector_l = feature_vector_list2;
	   feature_vector_curs=&feature_vector2_cursor;
	   state = &state2;
	   onset=OnsetSong2; offset= OffsetSong2;
	   Amplitude=Amplitude2; Pitch=Pitch2; PeakFreq=PeakFreq2;
	   principal_frequency=principal_frequency2; PitchGoodness=PitchGoodness2;
	   FM=FM2; AM=AM2; Entropy=Entropy2; continuity_t=continuity_t2;
	   continuity_f=continuity_f2; FMc=FMc2;
	}

	if (*state == 2) {
		feature_vector_l->SuppressPaint = true;
		dur = offset - onset;
		tm = onset * option->advanceTb->Value; // timeL->Value-dur;
		for (int i = 1; i < dur; i++) {
			tm += advanceTb->Value;
			str = tm;
			str = str.SubString(1, 6);
			feature_vector_l->Elem[1][*feature_vector_curs+i] = str;
			str = Amplitude[onset + i];
			str = str.SubString(1, 6);
			feature_vector_l->Elem[2][*feature_vector_curs+i] = str;
			str = Pitch[onset + i];
			str = str.SubString(1, 6);
			feature_vector_l->Elem[3][*feature_vector_curs+i] = str;
			str = PeakFreq[onset + i];
			str = str.SubString(1, 6);
			feature_vector_l->Elem[4][*feature_vector_curs+i] = str;
			str = principal_frequency[onset + i];
			str = str.SubString(1, 6);
			feature_vector_l->Elem[5][*feature_vector_curs+i] = str;
			str = PitchGoodness[onset + i];
			str = str.SubString(1, 6);
			feature_vector_l->Elem[6][*feature_vector_curs+i] = str;
			str = FM[onset + i];
			str = str.SubString(1, 6);
			feature_vector_l->Elem[7][*feature_vector_curs+i] = str;
			str = AM[onset + i];
			str = str.SubString(1, 6);
			feature_vector_l->Elem[8][*feature_vector_curs+i] = str;
			str = Entropy[onset + i];
			str = str.SubString(1, 6);
			feature_vector_l->Elem[9][*feature_vector_curs+i] = str;
			str = continuity_t[onset + i];
			str = str.SubString(1, 6);
			feature_vector_l->Elem[10][*feature_vector_curs+i] = str;
			str = continuity_f[onset + i];
			str = str.SubString(1, 6);
			feature_vector_l->Elem[11][*feature_vector_curs+i] = str;
			str = FMc[onset + i];
			str = str.SubString(1, 6);
			feature_vector_l->Elem[12][*feature_vector_curs+i] = str;
			str = AnsiString(*start-1000);
			feature_vector_l->Elem[13][*feature_vector_curs+i] = str; //hidden
			feature_vector_l->AddRow();

		}
		feature_vector_l->SuppressPaint = false;
		*feature_vector_curs+=(dur-1);
	}
	else
		MessageDlg(
		"You need to outline an interval first, use mouse to click in two location on sonogram image", mtError, TMsgDlgButtons() << mbOK, 0);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::play_song2PostClick(TObject *Sender, TMPBtnType Button)
{
    //play_song2->StartPos = OnsetSong2 * advanceTb2->Value;
    //play_song2->EndPos = OffsetSong2 * advanceTb2->Value;
     play_song2->StartPos = (OnsetSong2+option->currentStartLocation2-1000) * advanceTb2->Value;
    //if (OnsetSong1+option->currentStartLocation < OffsetSong1) {
    play_song2->EndPos = (OnsetSong2+option->currentStartLocation2) * advanceTb2->Value;

}
// ---------------------------------------------------------------------------

void __fastcall TForm1::play_song2Click(TObject *Sender, TMPBtnType Button,
    bool &DoDefault)
{
     play_song2->StartPos = (OnsetSong2+option->currentStartLocation2-1000) * advanceTb2->Value;
     play_song2->EndPos = (OnsetSong2+option->currentStartLocation2) * advanceTb2->Value;

}
// ---------------------------------------------------------------------------

void __fastcall TForm1::play1Click(TObject *Sender) {
    song_play->Play();
	song_play->StartPos = OnsetSong1 * advanceTb->Value;
    song_play->EndPos = OffsetSong1 * advanceTb->Value;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::play2Click(TObject *Sender) {
    play_song2->Play();
    play_song2->StartPos = OnsetSong2 * advanceTb2->Value;
    play_song2->EndPos = OffsetSong2 * advanceTb2->Value;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Open1Click(TObject *Sender) {
    if (display->ActivePage == Song1)
        newBClick(this);
    else if (display->ActivePage == Song2)
        new2BClick(this);
    else {
        display->ActivePage = Song1;
        newBClick(this);
    }
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::SaveAs1Click(TObject *Sender) {
    if (display->ActivePage == Song1) {
        WaveSaveDialog->FileName = song_play->FileName;
        if (!WaveSaveDialog->Execute())
            return;
        // InputBox("Save Sound As", "Type new sound name, e.g., song.wav:", "");
        str = song_play->FileName;
        song_play->Close();
        RenameFile(str, WaveSaveDialog->FileName);
        song_play->FileName = WaveSaveDialog->FileName;
        song_play->Open();
    }

    else {
        WaveSaveDialog->FileName = play_song2->FileName;
        if (!WaveSaveDialog->Execute())
            return;
        // InputBox("Save Sound As", "Type new sound name, e.g., song.wav:", "");
        str = play_song2->FileName;
        play_song2->Close();
        RenameFile(str, WaveSaveDialog->FileName);
        play_song2->FileName = WaveSaveDialog->FileName;
        play_song2->Open();
    }
    // else if(display->ActivePage==Song2) {}
    // else {display->ActivePage=Song1; }
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Play3Click(TObject *Sender) {
    if (display->ActivePage == Song1)
        song_play->Play();
    else if (display->ActivePage == Song2)
        play_song2->Play();
    else
        song_play->Play();
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Exit1Click(TObject *Sender) {
    Application->Terminate();
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Close1Click(TObject *Sender) {
    Form1->Close();
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Sounds121Click(TObject *Sender) {
    display->ActivePage = Combined;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Sound11Click(TObject *Sender) {
    display->ActivePage = Song1;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Sound21Click(TObject *Sender) {
    display->ActivePage = Song2;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::paste11Click(TObject *Sender) {
    // raw_features_page2->PasteFromClipboard();
}
// ---------------------------------------------------------------------------


void __fastcall TForm1::copy13Click(TObject *Sender) {
    rhythm_data->SelectAll();
    rhythm_data->CopyToClipboard();
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::cut13Click(TObject *Sender) {
    rhythm_data->SelectAll();
    rhythm_data->CutToClipboard();
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::paste13Click(TObject *Sender) {
    rhythm_data->PasteFromClipboard();
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::copy14Click(TObject *Sender) {
    batch_log->SelectAll();
    batch_log->CopyToClipboard();
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::cut14Click(TObject *Sender) {
    batch_log->SelectAll();
    batch_log->CutToClipboard();
}

// ---------------------------------------------------------------------------

void __fastcall TForm1::paste14Click(TObject *Sender) {
	batch_log->PasteFromClipboard();
}
// ---------------------------------------------------------------------------

/*
void __fastcall TForm1::fine_segmentationClick(TObject *Sender)
{
option->fine_segmentation->Down=fine_segmentation->Down;
if(fine_segmentation->Down)
{
entropyTb->Visible=true;
entropy_val->Visible=true;
ent_label->Visible=true;
DerivPowerB->Caption="diff";
mfaB->Caption="Amp";
segmentation_pannel->Visible=true;
option->hp_amp=true;
}
else
{
entropyTb->Visible=false;
entropy_val->Visible=false;
ent_label->Visible=false;
DerivPowerB->Caption="Amp";
mfaB->Caption="MF Amp";
segmentation_pannel->Visible=false;
option->hp_amp=false;
}
recalc(0);
DataForm->Settings(1, "fine_segmentation", (int)fine_segmentation->Down);
}
 */

// ---------------------------------------------------------------------------

void __fastcall TForm1::save_commentsClick(TObject *Sender) {
	if (Sender == save_comments)
		DataForm->insert_comment((AnsiString)bird_name->Caption,
		(AnsiString)songname->Caption, (AnsiString)file_comments->Lines->Text);
	else
		DataForm->insert_comment((AnsiString)bird_name2->Caption,
		(AnsiString)SongName2a->Caption,
		(AnsiString)file_comments2->Lines->Text);

}


// ---------------------------------------------------------------------------


void __fastcall TForm1::update_displayClick(TObject *Sender) {
	Update_Display(0);
}


// ---------------------------------------------------------------------------



void __fastcall TForm1::Saverawfeatures1Click(TObject *Sender) {

	features_display->ActivePage = vector_display;
    // data_control1->ActivePage=raw_features_display;
    if (interval1)
        add_raw_featuresClick(this);
    else
        MessageDlg(
        "Before you can save raw features you must outline the sound interval. Click on the sonogram in two different locations and then try again", mtError, TMsgDlgButtons() << mbOK, 0);
}


// ---------------------------------------------------------------------------
/*


void __fastcall TForm1::deriv_displayClick(TObject *Sender) {
CZT_FeatureCalc *calci;
TImage *the_image;
TTrackBar *Brightness;
TRadioGroup *the_display;
bool brd;

if (Sender == deriv_display) {
the_image = Image1;
calci = &m_Calculator1;
Brightness = sldrBrightness;
the_display = deriv_display;
brd = 0;
}

else {
the_image = Image2;
calci = &m_Calculator2;
Brightness = sldrBrightness2;
the_display = deriv_display2;
brd = 1;
}

<<<<<<< .mine
	float bright = set_brightness(auto_gain->Checked, Brightness->Position,	1);//the_display->ItemIndex ? 0 : 1);
	//calci->GetImage().SetBrightness(bright);

	if (the_display->ItemIndex == 0) {
	//option->sono_display = false;
	calci->GetImage().SetBrightness(bright);
	calci->GetImage().DrawAllSlices(the_image);
	}
	else if (the_display->ItemIndex == 1) {
	calci->GetContours().SetBrightness(bright);
	calci->GetContours().DrawAllSlices(the_image);
	}
=======
float bright = set_brightness(auto_gain->Checked, Brightness->Position,
the_display->ItemIndex ? 0 : 1);
calci->GetImage().SetBrightness(bright);
if (the_display->ItemIndex == 0) {

//calci->GetImage().DrawAllSlices(the_image);
// if(option->sono_display==true)
{
option->sono_display = false;
Update_Display(brd);
// update_displayClick(this); //recalc();
}
}
else if (the_display->ItemIndex == 1)
calci->GetSonogram().DrawAllSlices(the_image); // show contours

else if (the_display->ItemIndex == 2) // sonogram
{
//calci->GetImage().DrawAllSlices(the_image);
// if(option->sono_display==false)
{
option->sono_display = true;
Update_Display(brd);
>>>>>>> .r111
	else if (the_display->ItemIndex == 2) {
	//option->sono_display = true;
	calci->GetSonogram().SetBrightness(bright);
	calci->GetSonogram().DrawAllSlices(the_image);
	}
}

}
}

// ---------------------------------------------------------------------------
 */


void __fastcall TForm1::deriv_displayClick(TObject *Sender) {
    CZT_FeatureCalc *calci;
    TImage *the_image;
    TTrackBar *Brightness;
    TRadioGroup *the_display;
    bool brd;

    if (Sender == deriv_display) {
        the_image = Image1;
        calci = &m_Calculator1;
        Brightness = sldrBrightness;
        the_display = deriv_display;
		brd = 0;
    }

    else {
        the_image = Image2;
        calci = &m_Calculator2;
        Brightness = sldrBrightness2;
        the_display = deriv_display2;
        brd = 1;
    }

    float bright = set_brightness(auto_gain->Checked, Brightness->Position, 1);
    // the_display->ItemIndex ? 0 : 1);
    // calci->GetImage().SetBrightness(bright);

    if (the_display->ItemIndex == 0) {
        // option->sono_display = false;
        calci->GetImage().SetBrightness(bright);
        calci->GetImage().DrawAllSlices(the_image);
    }
    else if (the_display->ItemIndex == 1) {
        calci->GetContours().SetBrightness(bright);
        calci->GetContours().DrawAllSlices(the_image);
    }
    else if (the_display->ItemIndex == 2) {
        // option->sono_display = true;
        calci->GetSonogram().SetBrightness(bright);
        calci->GetSonogram().DrawAllSlices(the_image);
	}
	set_scale(brd);
}

// ---------------------------------------------------------------------------


void TForm1::calc_range() {
	int i = range_down_val->Value / option->frequency_scale;
	if (frequency_range->Position == 1)
		option->min_freq->Position = i * 2;
	if (frequency_range->Position == 2)
		option->min_freq->Position = i;
	if (frequency_range->Position == 3)
		option->min_freq->Position = i / 2;
	option->min_freqChange(this);

	i = range_up_val->Value / option->frequency_scale;
	if (frequency_range->Position == 1)
		option->max_freq->Position = i * 2;
	if (frequency_range->Position == 2)
		option->max_freq->Position = i;
	if (frequency_range->Position == 3)
		option->max_freq->Position = i / 2;
	option->max_freqChange(this);

	range_down_shading->Width = Image1->Width;
	range_up_shading->Width = Image1->Width;
	range_down_shading2->Width = Image2->Width;
	range_up_shading2->Width = Image2->Width;
}

void __fastcall TForm1::Pitch_mClick(TObject *Sender) {
	PitchB->Down = true;
	PitchBClick(this);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Amplitude_mClick(TObject *Sender) {
    DerivPowerB->Down = true;
    DerivPowerBClick(this);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::MeanFrequency_mClick(TObject *Sender) {
    PowerB->Down = true;
    PowerBClick(this);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Principalcontour_mClick(TObject *Sender) {
	principal_frB->Down = true;
	principal_frBClick(this);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::ou1Click(TObject *Sender) {
	if (!onset_1 || !offset_1) {
        MessageDlg(
            "First outline the song manually by clicking it in two different locations. Then use this menu command to repeat the same outline in a different sound.", mtError, TMsgDlgButtons() << mbOK, 0);
        return;
    }
    Image1->Canvas->Pen->Color = clRed;
    Image1->Canvas->MoveTo(onset_1, 0);
    Image1->Canvas->LineTo(onset_1, 240);
    song_play->StartPos = onset_1 * advanceTb->Value;
    Image1->Canvas->Pen->Color = clBlue;
    Image1->Canvas->MoveTo(offset_1, 0);
    Image1->Canvas->LineTo(offset_1, 240);
	sound = 0;
    CalculateIntervalMeasures(onset_1, offset_1);
    song_play->EndPos = offset_1 * advanceTb->Value;
    interval1 = true;
    OnsetSong1 = onset_1;
    OffsetSong1 = offset_1;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Save1Click(TObject *Sender) {
	if (display->ActivePage == Song1)
        menu_outline = 1;
    else
        menu_outline = 2;
    features_display->ActivePage = display_means;
    // data_control1->ActivePage=mean_features_display;
    if (interval1)
        AddBtClick(this);
    else
        MessageDlg(
        "Before you can save syllable features you must outline the sound interval. Click on the sonogram in two different locations and then try again", mtError, TMsgDlgButtons() << mbOK, 0);

}
// ---------------------------------------------------------------------------

void __fastcall TForm1::range_downChange(TObject *Sender) {
	if(!preview_state)PreviewClick(this);
	range_down_shading->Visible = true;
	range_down_shading->Top = 25 + (float)(range_down->Position) / 45;
	range_down_shading->Height = 250 - range_down_shading->Top;

    range_down_val->Value = 0.9 * (1.0 - (float)range_down->Position / 10000.0)
		* option->sampling_rate / pow(2, 4 - frequency_range->Position);

    range_down_shading->Visible = true;

	//range_down2->Position = range_down->Position;
	range_down_shading2->Visible = true;
	//range_down_val2->Value = range_down_val->Value;
	range_down_shading2->Top = range_down_shading->Top;
	range_down_shading2->Height = range_down_shading->Height;
	update_display2->Visible = true;

	update_display->Visible = true;
	calc_range();
    if (range_down->Position == 10000)
        range_down_shading->Visible = false;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::FormShow(TObject *Sender) {
	Application->CreateForm(__classid(Topen_sound), &open_sound);
    Fill_feature_headers();
    DataForm->Retrieve_frame_params(Frame11, 1);
	DataForm->Retrieve_frame_params(Frame12, 2);

	features_display->TabIndex=DataForm->Retrieve(1,"tabs").ToInt();
	features_display2->TabIndex=DataForm->Retrieve(2,"tabs").ToInt();
	data_control1->TabIndex=DataForm->Retrieve(1,"tabs2").ToInt();
	data_control2->TabIndex=DataForm->Retrieve(2,"tabs2").ToInt();

   str="c:\\sap\\data\\similarity.xml";
   if(FileExists(str)) similarity_view->LoadFromXMLFile(str,"features_list");
   similarity_cursor=similarity_view->NrOfRows;

   str="c:\\sap\\data\\similarity_batch.xml";
   if(FileExists(str)) similarity_batch_view->LoadFromXMLFile(str,"features_list");
   similarity_batch_cursor=similarity_batch_view->NrOfRows;

   str="c:\\sap\\data\\comparison.xml";
   if(FileExists(str)) comparison_view->LoadFromXMLFile(str,"features_list");
   comparison_cursor=comparison_view->NrOfRows;

}
// ---------------------------------------------------------------------------

void __fastcall TForm1::range_upChange(TObject *Sender) {
	range_up_shading->Visible = true;
    range_up_shading->Height = 10 + (float)(range_up->Position) / 45.0;
    range_up_val->Value = 0.9 * (1.0 - (float)range_up->Position / 10000.0)
        * option->sampling_rate / pow(2, 4 - frequency_range->Position);
    // range_up_val->Value=10000-range_up->Position;
    // if(frequency_range->Position==3)range_up_val->Value=range_up_val->Value*2;
    // else if(frequency_range->Position==1)range_up_val->Value=range_up_val->Value/2;
    calc_range();
	range_up_shading2->Visible = true;
	//range_up2->Position = range_up->Position;
	//range_up_val2->Value = range_up_val->Value;
	range_up_shading2->Top = range_up_shading->Top;
	range_up_shading2->Height = range_up_shading->Height;
	update_display->Visible = true;
	update_display2->Visible = true;
	if (range_up->Position == 0)
		range_up_shading->Visible = false;
}
// ---------------------------------------------------------------------------

void TForm1::set_analysis_settings(bool bird) {
    if (!bird) {
        derivTB->Position = option->scale->FieldValues["deriv_thresh"];
        entropyTb->Position = option->scale->FieldValues["entropy_thresh"];
        sldrBrightness->Position = option->scale->FieldValues
            ["display_contrast"];
        frequency_range->Position = option->scale->FieldValues
            ["frequency_range"];
        data_window->Position = option->scale->FieldValues["FFT_window"];
        advance->Position = option->scale->FieldValues["advance_window"];
        Frame11->hp_big->Position = option->scale->FieldValues["hp_big"];
        Frame11->hp_small->Position = option->scale->FieldValues["hp_small"];
		// pagination->ItemIndex = option->scale->FieldValues["pagination"];
        Frame11->hp_bigChange(this);
        Frame11->hp_smallChange(this);
    }
    else {
        derivTB2->Position = option->scale->FieldValues["deriv_thresh"];
        entropyTb2->Position = option->scale->FieldValues["entropy_thresh"];
        sldrBrightness2->Position = option->scale->FieldValues
            ["display_contrast"];
        frequency_range2->Position = option->scale->FieldValues
            ["frequency_range"];
        data_window2->Position = option->scale->FieldValues["FFT_window"];
        advance2->Position = option->scale->FieldValues["advance_window"];
        Frame12->hp_big->Position = option->scale->FieldValues["hp_big"];
        Frame12->hp_small->Position = option->scale->FieldValues["hp_small"];
        Frame12->hp_bigChange(this);
        Frame12->hp_smallChange(this);
    }
}

void TForm1::update_scale() {
    TStaticText* y[5];
    TStaticText* yb[5];
    float x;
    y[0] = y1;
    y[1] = y2;
    y[2] = y3;
    y[3] = y4;
    y[4] = y5;
    yb[0] = y1b;
    yb[1] = y2b;
    yb[2] = y3b;
    yb[3] = y4b;
    yb[4] = y5b;

    if (frequency_range->Position == 1)
        for (int i = 0; i < 5; i++) {
            x = (float)(i + 1) * (float)option->sampling_rate / 44100.0;
            str = x;
            str = str.SubString(1, 3);
            str += "kHz";
            y[i]->Caption = str;
            yb[i]->Caption = str;
        }

    else if (frequency_range->Position == 2)
        for (int i = 0; i < 5; i++) {
            x = (float)(i + 1) * 2.0 * (float)option->sampling_rate / 44100.0;
            str = x;
            str = str.SubString(1, 3);
            str += "kHz";
            y[i]->Caption = str;
            yb[i]->Caption = str;
        }

    else if (frequency_range->Position == 3)
        for (int i = 0; i < 5; i++) {
            x = (float)(i + 1) * 4.0 * (float)option->sampling_rate / 44100.0;
            str = x;
            str = str.SubString(1, 3);
            str += "kHz";
            y[i]->Caption = str;
            yb[i]->Caption = str;

        }
    /*
    if(scale==1)
    {
    y1->Caption="1kHz";
    y2->Caption="2kHz";
    y3->Caption="3kHz";
    y4->Caption="4kHz";
    y5->Caption="5kHz";
    }
    else if(scale==2)
    {
    y1->Caption="2kHz";
    y2->Caption="4kHz";
    y3->Caption="6kHz";
    y4->Caption="8kHz";
    y5->Caption="10kHz";
    }
    else if(scale==3)
    {
    y1->Caption="4kHz";
    y2->Caption="8kHz";
    y3->Caption="12kHz";
    y4->Caption="16kHz";
    y5->Caption="20kHz";
    }
     */
}

void __fastcall TForm1::newScaleClick(TObject *Sender) {
    option->newScaleClick(this);
    update_template();
    templates->ItemIndex = templates->Items->Count;
    // option->scale->Last();
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::auto_gainClick(TObject *Sender) {
    /*
    if(auto_gain->Checked)
    {
    sldrBrightness->Visible=false;
    gainL->Visible=false;
    gainIc->Visible=false;
    }
    else
    {
    sldrBrightness->Visible=true;
    gainL->Visible=true;
    gainIc->Visible=true;
    } */
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::songnameClick(TObject *Sender) {
    if (songname->Transparent)
        songname->Transparent = false;
    else
        songname->Transparent = true;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::SongName2aClick(TObject *Sender) {
    if (SongName2a->Transparent)
        SongName2a->Transparent = false;
    else
        SongName2a->Transparent = true;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::update_display2Click(TObject *Sender) {
	Update_Display(1);
   /* Screen->Cursor = crHourGlass;
	play_song2->Close();
	Analyze_song2(song2_name, 1);
	play_song2->Open();
	Screen->Cursor = crDefault;
	m_Calculator2.GetImage().DrawAllSlices(Image2);
	set_scale(1);
	update_display2->Visible = false;  */
}

// ---------------------------------------------------------------------------

void TForm1::Update_Display(bool bird) {
	TRadioGroup *disp;
	TMediaPlayer *playsong;
	CZT_FeatureCalc *calci;
	TImage *img;
	int* currentStartLoc;
	bool *pagination_On;

	if (!bird) // bird 1
	{
		disp=deriv_display;
		playsong = song_play;
		calci = &m_Calculator1;
		pagination_On = &option->paginationOn;
		img = Image1;
		currentStartLoc = &option->currentStartLocation;
	}
	else // bird 2
	{
		disp=deriv_display2;
		playsong = play_song2;
		calci = &m_Calculator2;
		pagination_On = &option->paginationOn2;
		img = Image2;
		currentStartLoc = &option->currentStartLocation2;
	}

  //	doIncrement = false;
	Screen->Cursor = crHourGlass;
   	playsong->Close();
	if (!bird) {
		// Analyze_song1(song1_name, 1);
		if (*pagination_On) {
			Analyze_song1_light(song1_name, 1, *currentStartLoc);
		}
		else {
			Analyze_song1(song1_name, 1);
		}
	}
	else
		Analyze_song2(song2_name, 1);
	playsong->Open();
	Screen->Cursor = crDefault;
	if (*pagination_On) {
		switch(disp->ItemIndex)
		{
			case 0: calci->GetImage().DrawSlices(0, 1000 - 1, img); break;
			case 1: calci->GetContours().DrawSlices(0, 1000 - 1, img); break;
			case 2: calci->GetSonogram().DrawSlices(0, 1000 - 1, img); break;
		}

	}
	else {
		switch(disp->ItemIndex)
		{
		   case 0: calci->GetImage().DrawAllSlices(img); break;
		   case 1: calci->GetContours().DrawAllSlices(img); break;
		   case 2: calci->GetSonogram().DrawAllSlices(img); break;
		}

	}

	set_scale(bird);

	if (!bird)
		update_display->Visible = false;
	else
		update_display2->Visible = false;
	derivTBChange(this);

}

// ---------------------------------------------------------------------------

void TForm1::Update_Display_light(bool bird) {
	TRadioGroup *disp;
	TMediaPlayer *playsong;
	CZT_FeatureCalc *calci;
	TImage *img;

	if (!bird) // bird 1
	{
		playsong = song_play;
		calci = &m_Calculator1;
		img = Image1;
		disp=deriv_display;
	}
	else // bird 2
	{
		playsong = play_song2;
		calci = &m_Calculator2;
		img = Image2;
		disp=deriv_display2;
	}

	Screen->Cursor = crHourGlass;
	playsong->Close();
	if (!bird)
		Analyze_song1_light(song1_name, 1, 0);
	/* Need to write for the other screen
	else
	Analyze_song2(song2_name, 1); */
	playsong->Open();
	Screen->Cursor = crDefault;

	switch(disp->ItemIndex)
	{
	  case 0: calci->GetImage().DrawAllSlices(img); break;
	  case 1: calci->GetContours().DrawAllSlices(img); break;
	  case 2: calci->GetSonogram().DrawAllSlices(img); break;
	}

	set_scale(bird);
	/* if (!bird)
	update_display->Visible = false;
	else
	update_display2->Visible = false;
	 */
}







void __fastcall TForm1::range_down2Change(TObject *Sender) {
   /*
	range_down_shading2->Visible = true;
	range_down_shading2->Top = 15 + (float)(range_down2->Position) / 45;
	range_down_shading2->Height = 240 - range_down_shading2->Top;
	range_down_val2->Value = 0.9 *
		(1.0 - (float)range_down2->Position / 10000.0)
		* option->sampling_rate / pow(2, 4 - frequency_range2->Position);
	// range_down_val2->Value=10000-range_down2->Position;//option->frequency_scale*(float)range_down_shading->Height+450;
	// if(frequency_range->Position==3)range_down_val2->Value=range_down_val2->Value*2;
	// else if(frequency_range->Position==1)range_down_val2->Value=range_down_val2->Value/2;
	range_down_shading->Visible = true;
    range_down->Position = range_down2->Position;
    range_down_val->Value = range_down_val2->Value;
	range_down_shading->Top = range_down_shading2->Top;
	range_down_shading->Height = range_down_shading2->Height;
    update_display->Visible = true;
    update_display2->Visible = true;
    calc_range();
    if (range_down2->Position == 10000)
        range_down_shading2->Visible = false;
        */
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::range_up2Change(TObject *Sender) {
/*
    range_up_shading2->Visible = true;
    range_up_shading2->Height = 10 + (float)(range_up2->Position) / 45.0;
    range_up_val2->Value = 0.9 * (1.0 - (float)range_up2->Position / 10000.0)
        * option->sampling_rate / pow(2, 4 - frequency_range2->Position);

    // range_up_val2->Value=10000-range_up2->Position;
    // if(frequency_range->Position==3)range_up_val2->Value=range_up_val2->Value*2;
    // else if(frequency_range->Position==1)range_up_val2->Value=range_up_val2->Value/2;
    range_up->Position = range_up2->Position;
    range_up_shading->Visible = true;
    range_up_val->Value = range_up_val2->Value;
    range_up_shading->Top = range_up_shading2->Top;
    range_up_shading->Height = range_up_shading2->Height;
    update_display->Visible = true;
    update_display2->Visible = true;
    calc_range();
    if (range_up2->Position == 0)
        range_up_shading2->Visible = false;
   */
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::save_rawfeaturesClick(TObject *Sender) {

    if (display->ActivePage == Song1)
        features_display->ActivePage = vector_display;
    else
        features_display2->ActivePage = vector_display2;
    add_raw_featuresClick(this);

}
// ---------------------------------------------------------------------------

void __fastcall TForm1::save_mean_featuresClick(TObject *Sender) {

    if (display->ActivePage == Song1) {
        features_display->ActivePage = display_means;
        // data_control1->ActivePage=mean_features_display;
        menu_outline = 1;
		AddBtClick(this);
	}
	else {
		features_display2->ActivePage = display_means2;
		menu_outline = 2;
		AddBtClick(this);
	}

}


// ---------------------------------------------------------------------------




void __fastcall TForm1::save_features_at_pointerClick(TObject *Sender) {
	TReportListView* save_at_pointer;
	TTabSheet* features_at_point;
	TPageControl* features_disp;
	TNumLab *Ftime, *Famp, *Fpitch, *Fmeanfr, *Fcontour, *Fgoodness, *Ffm,
	*Fam, *Fentropy, *Fcontt, *Fcontf, *Fdas;
	int *cursor;

	if (display->ActivePage == Song1) {
		save_at_pointer = feature_at_pointer_list;
		features_at_point = features_at_pointer;
		cursor = &feature_at_pointer_cursor;
		features_disp = features_display;
		Ftime = timeL;
		Famp = AmplitudeL;
		Fpitch = pitchL;
		Fmeanfr = PeakFreqL;
		Fcontour = Principal_frN1;
		Fgoodness = PgoodL;
		Ffm = modulationL;
		Fam = AML;
		Fentropy = entropyL;
		Fcontt = continuity_tN1;
		Fcontf = continuity_fN1;
		Fdas = slopeN1;
	}
	else {
		save_at_pointer = feature_at_pointer_list2;
		features_at_point = features_at_pointer2;
		cursor = &feature_at_pointer2_cursor;
		features_disp = features_display2;
		Ftime = time2L;
		Famp = Amplitude2L;
		Fpitch = pitch2L;
		Fmeanfr = PeakFreq2L;
		Fcontour = Principal_frN2;
		Fgoodness = PgoodL2;
		Ffm = modulation2L;
		Fam = AM2L;
		Fentropy = entropy2L;
		Fcontt = continuity_tN2;
		Fcontf = continuity_fN2;
		Fdas = slopeN2;
	}

	features_disp->ActivePage = features_at_point;
	save_at_pointer->SuppressPaint = true;
	save_at_pointer->AddRow();
	str = Ftime->Value; // +((float)option->currentStartLocation-1000.0)*advanceTb->Value
	str = str.SubString(1, 7);
	save_at_pointer->Elem[1][*cursor] = str;
	str = Famp->Value;
	str = str.SubString(1, 6);
	save_at_pointer->Elem[2][*cursor] = str;
	str = Fpitch->Value;
	str = str.SubString(1, 6);
	save_at_pointer->Elem[3][*cursor] = str;
	str = Fmeanfr->Value;
	str = str.SubString(1, 6);
	save_at_pointer->Elem[4][*cursor] = str;
	str = Fcontour->Value;
	str = str.SubString(1, 6);
	save_at_pointer->Elem[5][*cursor] = str;
	str = Fgoodness->Value;
	str = str.SubString(1, 6);
	save_at_pointer->Elem[6][*cursor] = str;
	str = Ffm->Value;
	str = str.SubString(1, 6);
	save_at_pointer->Elem[7][*cursor] = str;
	str = Fam->Value;
	str = str.SubString(1, 6);
	save_at_pointer->Elem[8][*cursor] = str;
	str = Fentropy->Value;
	str = str.SubString(1, 6);
	save_at_pointer->Elem[9][*cursor] = str;
	str = Fcontt->Value;
	str = str.SubString(1, 6);
	save_at_pointer->Elem[10][*cursor] = str;
	str = Fcontf->Value;
	str = str.SubString(1, 6);
	save_at_pointer->Elem[11][*cursor] = str;
	str = Fdas->Value;
	str = str.SubString(1, 6);
	save_at_pointer->Elem[12][*cursor] = str;
	save_at_pointer->Elem[13][*cursor] = (AnsiString)comments->Text;
	save_at_pointer->SuppressPaint = false;
	if (save_at_pointer->NrOfRows > 8)
		save_at_pointer->TopRow = save_at_pointer->NrOfRows - 8;
	(*cursor)++;
	set_scale(0);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Playsound1Click(TObject *Sender) {
	if (display->ActivePage == Song1) {
		song_play->StartPos = OnsetSong1 * advanceTb->Value;
        song_play->EndPos = OffsetSong1 * advanceTb->Value;
        song_play->Play();
    }
    else {
        play_song2->StartPos = OnsetSong2 * advanceTb2->Value;
        play_song2->EndPos = OffsetSong2 * advanceTb2->Value;
        play_song2->Play();
    }
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Goaway1Click(TObject *Sender) {
    keybd_event(VK_ESCAPE, 0, 0, 0);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::song_playClick(TObject *Sender, TMPBtnType Button,
    bool &DoDefault)

{
   /*
	if(pagination->ItemIndex){  // page by page:
	  if(OffsetSong1<OnsetSong1 || OnsetSong1<2){
	  song_play->StartPos = (OnsetSong1+option->currentStartLocation-1000) * advanceTb->Value;
	  song_play->EndPos = (OnsetSong1+option->currentStartLocation) * advanceTb->Value;
	  }
	  else{
	   song_play->StartPos = (option->currentStartLocation+OnsetSong1)* advanceTb->Value;
	   song_play->EndPos = (option->currentStartLocation+OffsetSong1)* advanceTb->Value;//song_play->StartPos+1000;
	  }

	}
	else {  // entire sound
	if(OffsetSong1>OnsetSong1 && OnsetSong1>2){
	  song_play->StartPos = OnsetSong1 * advanceTb->Value;
	  song_play->EndPos =  OffsetSong1 * advanceTb->Value;
	  }
	  else{
	   song_play->StartPos = 0;
	   song_play->EndPos = Iterations1* advanceTb->Value;
	  }
	}
  */

}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Draw1Click(TObject *Sender) {
    DrawBClick(this);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Rotate1Click(TObject *Sender) {
    feature_space->MouseAction = maRotate;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Pan1Click(TObject *Sender) {
    feature_space->MouseAction = maPan;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Zoom1Click(TObject *Sender) {
    feature_space->MouseAction = maZoom;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Color1Click(TObject *Sender) {
    if (ColorDialog->Execute()) {
        ColorBox->Selected = ColorDialog->Color;
        DrawBClick(this);
    }
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Goway1Click(TObject *Sender) {
    keybd_event(VK_ESCAPE, 0, 0, 0);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::gotoothersound1Click(TObject *Sender) {
    if (display->ActivePage == Song1)
        display->ActivePage = Song2;
    else
        display->ActivePage = Song1;
    displayChange(this);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::similarity2Click(TObject *Sender) {
    display->ActivePage = Similarity;
   // similarity_control->ActivePage = score_parameters;
    displayChange(this);
}
// ---------------------------------------------------------------------------


void TForm1::update_popup(int flag) {
    if (ready2)
        similarity2->Enabled = true;
    else
        similarity2->Enabled = false;
    switch(flag) {
    case 0: // interval 1
        save_features_at_pointer->Enabled = false;
        save_rawfeatures->Enabled = true;
        save_mean_features->Enabled = true;

        break;

    case 1: // features at pointer
        save_features_at_pointer->Enabled = true;
        save_rawfeatures->Enabled = false;
        save_mean_features->Enabled = false;
    }
}

void __fastcall TForm1::save_featuresPopup(TObject *Sender) {
    if (display->ActivePage == Song1 && state1 == 0)
        update_popup(1); // popup for feature at pointer flag
    if (display->ActivePage == Song2 && state2 == 0)
        update_popup(1); // popup for features at pointer flag
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Image1MouseUp(TObject *Sender, TMouseButton Button,
    TShiftState Shift, int X, int Y) {
	if (draged && state1 == 1) {
        offset_1 = OffsetSong1 = X;
        state1 = 2;
        sound = 0;
        done = CalculateIntervalMeasures(OnsetSong1, X);
		if(option->paginationOn)song_play->EndPos = (option->currentStartLocation+OffsetSong1-1000)* advanceTb->Value;
		else song_play->EndPos = OffsetSong1 * advanceTb->Value;
        //song_play->EndPos = OffsetSong1 * advanceTb->Value;
        if (CalcMode->ItemIndex == auto_segment)
            save_syllables(ksong1);
        interval1 = true;
       // ks_panel->Visible = false;
		menu_outline = 1;
		update_score_sounds=true;
        AddBtClick(this);
    } /*

    int adj = X*option->totalPages/preview_sono->Width;
    //if (adj > 0) {
    option->currentStartLocation = adj;
    option->pageNumber = option->currentStartLocation/1000;
    //}  else {
    //	option->currentStartLocation = 0;
    //	option->pageNumber = 0;
    //}

    Analyze_song1_light(song1_name, 1, option->currentStartLocation);
    option->pageNumber--;
    if (option->pageNumber < 1) {
    option->pageNumber =1;
    showBack = false;
    }

    if (!option->doNotSegment) {
    if (option->sec_segmentation == true) {
    updateSegmentationSec = true;
    entropyTbChange(this);
    }
	updateSegmentation=true;
	derivTBChange(this);
	} */
}

// ---------------------------------------------------------------------------

void __fastcall TForm1::preview_sonoMouseUp(TObject *Sender,
	TMouseButton Button, TShiftState Shift, int X, int Y) {
	int *currentStartLoc, *pageNum;
	bool *show_Back, *update_seg, *update_sec, prev_state;
	int tot_pages;
	TImage* preview_sonogram;
	TImage* image;

	if (Sender == preview_sono || display->ActivePage == Song1) {
		currentStartLoc = &option->currentStartLocation;
		image=Image1;
		preview_sonogram = preview_sono;
		prev_state=preview_state;
		tot_pages = option->totalPages;
		pageNum = &option->pageNumber;
		show_Back = &showBack;
		first_snd = true;
		update_sec = &updateSegmentationSec;
		update_seg = &updateSegmentation;
	} else {
		currentStartLoc = &option->currentStartLocation2;
		image=Image2;
		preview_sonogram = preview_sono2;
		prev_state=preview_state2;
		tot_pages = option->totalPages2;
		pageNum = &option->pageNumber2;
		show_Back = &showBack2;
		first_snd = false;
		update_sec = &updateSegmentationSec2;
		update_seg = &updateSegmentation2;
	}

	/* if(draged && state1==1)
	{
	offset_1 = OffsetSong1 = X;
	state1=2;
	sound = 0;
	done=CalculateIntervalMeasures(OnsetSong1, X);
	song_play->EndPos = OffsetSong1 * advanceTb->Value;
	song_play->EndPos = OffsetSong1 * advanceTb->Value;
	if (CalcMode->ItemIndex == auto_segment) save_syllables(ksong1);
	interval1 = true;
	ks_panel->Visible = false;
	menu_outline= 1; AddBtClick(this);
	} */

	int adj = X * tot_pages / preview_sonogram->Width;
	// if (adj > 0) {
	*currentStartLoc = adj;
	*pageNum = (*currentStartLoc) / 1000;
	// }  else {
	// option->currentStartLocation = 0;
	// option->pageNumber = 0;
	// }

	if (Sender == preview_sono || display->ActivePage == Song1)
		Analyze_song1_light(song1_name, 1, *currentStartLoc);
	else
		Analyze_song2(song2_name, 1);

	(*pageNum)--;
	if (*pageNum < 1) {
		*pageNum = 1;
	}
	if (*currentStartLoc < 1001) {
		*show_Back = false;
	}
	else {
		*show_Back = true;
	}
	if (!option->doNotSegment) {
		if (option->sec_segmentation == true) {
			*update_sec = true;
			entropyTbChange(this);
		}
		*update_seg = true;
		derivTBChange(this);
	}
	else set_scale(!first_snd);
	// cursor beyond song range
 //	if(preview_cursor->Left+preview_cursor->Width>1000)
	{
		image->AutoSize = true;
		image->Stretch = false;
		image->AutoSize = false;
		if(!prev_state){
			image->Height = 175;
			image->Stretch = true;
		}
		else {
			image->AutoSize = false;
			image->Stretch = true;
		}
	}
}

// ---------------------------------------------------------------------------

void TForm1::retrieve_data(AnsiString name)
{
   str=name;
   str=str.SubString(1,str.Length()-4);
   str+=str+"_pointer";
   str+=".xml";
   str="c:\\sap\\data\\"+str;
   if(FileExists(str)) feature_at_pointer_list->LoadFromXMLFile(str,"features_at_pointer");
   feature_at_pointer_cursor=feature_at_pointer_list->NrOfRows;

   str=name;
   str=str.SubString(1,str.Length()-4);
   str+=str+"_vector";
   str+=".xml";
   str="c:\\sap\\data\\"+str;
   if(FileExists(str)) feature_vector_list->LoadFromXMLFile(str,"features_vector");
   feature_vector_cursor=feature_vector_list->NrOfRows-1;

   str=name;
   str=str.SubString(1,str.Length()-4);
   str+=str+"_syll";
   str+=".xml";
   str="c:\\sap\\data\\"+str;
   if(FileExists(str)) feature_statistics_View->LoadFromXMLFile(str,"features_list");
   feature_statistics_cursor=feature_statistics_View->NrOfRows;

}

void TForm1::retrieve_data2(AnsiString name)
{
   str=name;
   str=str.SubString(1,str.Length()-4);
   str+=str+"_pointer";
   str+=".xml";
   str="c:\\sap\\data\\"+str;
   if(FileExists(str)) feature_at_pointer_list2->LoadFromXMLFile(str,"features_at_pointer");
   feature_at_pointer2_cursor=feature_at_pointer_list2->NrOfRows;

   str=name;
   str=str.SubString(1,str.Length()-4);
   str+=str+"_vector";
   str+=".xml";
   str="c:\\sap\\data\\"+str;
   if(FileExists(str)) feature_vector_list2->LoadFromXMLFile(str,"features_vector");
   feature_vector2_cursor=feature_vector_list2->NrOfRows-1;

   str=name;
   str=str.SubString(1,str.Length()-4);
   str+=str+"_syll";
   str+=".xml";
   str="c:\\sap\\data\\"+str;
   if(FileExists(str)) feature_statistics_View2->LoadFromXMLFile(str,"features_list");
   feature_statistics2_cursor=feature_statistics_View2->NrOfRows;

}


///////////////////////////////////////////////////////////////////////////////////////



void TForm1::save_data()
{

	if(display->ActivePage==Song1){
		str=songname->Caption;
		str=str.SubString(1,str.Length()-4);
		str+=str+"_pointer";
		str+=".xml";
		str="c:\\sap\\data\\"+str;
		feature_at_pointer_list->SaveAsXMLFile(str,"features_at_pointer");

		str=songname->Caption;
		str=str.SubString(1,str.Length()-4);
		str+=str+"_vector";
		str+=".xml";
		str="c:\\sap\\data\\"+str;
		feature_vector_list->SaveAsXMLFile(str,"features_vector");

		str=songname->Caption;
		str=str.SubString(1,str.Length()-4);
		str+=str+"_syll";
		str+=".xml";
		str="c:\\sap\\data\\"+str;
		feature_statistics_View->SaveAsXMLFile(str,"features_list");
	}else{
	str=SongName2a->Caption;
	str=str.SubString(1,str.Length()-4);
	str+=str+"_pointer";
	str+=".xml";
	str="c:\\sap\\data\\"+str;
	feature_at_pointer_list2->SaveAsXMLFile(str,"features_at_pointer");

	str=SongName2a->Caption;
	str=str.SubString(1,str.Length()-4);
	str+=str+"_vector";
	str+=".xml";
	str="c:\\sap\\data\\"+str;
	feature_vector_list2->SaveAsXMLFile(str,"features_vector");

	str=SongName2a->Caption;
	str=str.SubString(1,str.Length()-4);
	str+=str+"_syll";
	str+=".xml";
	str="c:\\sap\\data\\"+str;
	feature_statistics_View2->SaveAsXMLFile(str,"features_list");
	}

	if(similarity_view->NrOfRows>1){
	   str="c:\\sap\\data\\similarity.xml";
	   similarity_view->SaveAsXMLFile(str,"features_list");
	}

	if(similarity_batch_view->NrOfRows>1){
	   str="c:\\sap\\data\\similarity_batch.xml";
	   similarity_batch_view->SaveAsXMLFile(str,"features_list");
	}

	if(comparison_view->NrOfRows>1){
	   str="c:\\sap\\data\\comparison.xml";
	   comparison_view->SaveAsXMLFile(str,"features_list");
	}
}


///////////////////////////////////////////////////////////////////////////////////////////



void __fastcall TForm1::save_xmlClick(TObject *Sender) {
	save_data();
   /*
	str = SaveDialog1->Filter;
	SaveDialog1->Filter = "XML Files|*.xml";
	if (SaveDialog1->Execute()) {
		feature_at_pointer_list->SaveAsXMLFile
			(StripExtension(SaveDialog1->FileName) + ".xml",
			"features_at_pointer");
	}
	SaveDialog1->Filter = str;
	*/
}


// ---------------------------------------------------------------------------



void __fastcall TForm1::features_to_clipboardClick(TObject *Sender) {
   if(Sender==features_to_clipboard || display->ActivePage == Song1)
		feature_at_pointer_list->CopyToClipBoard(1, 1, 13, feature_at_pointer_list->NrOfRows, 1, 1);
		else 	feature_at_pointer_list2->CopyToClipBoard(1, 1, 13, feature_at_pointer_list2->NrOfRows, 1, 1);

	// , er; AddHeader, UseTabs: boolean);

}
// ---------------------------------------------------------------------------

void __fastcall TForm1::delete_feature_rowClick(TObject *Sender) {
	bool fnd = false;
	if(display->ActivePage==Song1)
	{
		for (int i = 1; i <= feature_at_pointer_list->NrOfRows; i++)
				if (feature_at_pointer_list->RowSelected[i]) {
					feature_at_pointer_list->RemoveRow(i);
					feature_at_pointer_cursor--;
					fnd = true;
				}
		}else{
			for (int i = 1; i <= feature_at_pointer_list2->NrOfRows; i++)
				if (feature_at_pointer_list2->RowSelected[i]) {
					feature_at_pointer_list2->RemoveRow(i);
					feature_at_pointer2_cursor--;
					fnd = true;
				 }
		}
	if (!fnd)
		MessageDlg(
		"No row selected. Please click on a row to highlight it and try again",
		mtError, TMsgDlgButtons() << mbOK, 0);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::feature_at_pointer_listSelectCell(TObject *Sender,
	int ACol, int ARow, bool &CanSelect) {
	TReportListView *feature_at_pointer_l;
	CZT_FeatureCalc *calci;
	TRadioGroup *deriv_disp;
    TNumLab *time, *freq, *ent, *mod, *am, *pf, *pitch, *amp, *Pgood,
		*Principal_frN, *continuity_tN, *continuity_fN, *slopeN;
	TImage *image;
	TScrollBox *scroll;
	TShape *prev_cursor;
	int left_adjusted_outline, tot_pages;
	bool left_edge=false;
	bool pagination_on;
	bool song;

	if(display->ActivePage==Song1){
	   feature_at_pointer_l=feature_at_pointer_list;
	   pagination_on=option->paginationOn;
	   deriv_disp=deriv_display;
	   calci = &m_Calculator1;
	   image=Image1;
	   scroll=ScrollBox3;
	   tot_pages=option->totalPages;
	   prev_cursor=preview_cursor;
	   song=0;

		time = timeL;
		freq = frequencyL;
		ent = entropyL;
		mod = modulationL;
		pf = PeakFreqL;
		am = AML;
		pitch = pitchL;
		amp = AmplitudeL;
		Pgood = PgoodL;
		slopeN = slopeN1;
		Principal_frN = Principal_frN1;
		continuity_tN = continuity_tN1;
		continuity_fN = continuity_fN1;
	}else{
	   feature_at_pointer_l=feature_at_pointer_list2;
	   pagination_on=option->paginationOn2;
	   deriv_disp=deriv_display2;
	   calci = &m_Calculator2;
	   image=Image2;
	   scroll=ScrollBox4;
	   tot_pages=option->totalPages2;
	   prev_cursor=preview_cursor2;
	   song=1;

		time = time2L;
		freq = frequency2L;
		ent = entropy2L;
		mod = modulation2L;
		pf = PeakFreq2L;
		am = AM2L;
		pitch = pitch2L;
		amp = Amplitude2L;
		Pgood = PgoodL2;
		slopeN = slopeN2;
		Principal_frN = Principal_frN2;
		continuity_tN = continuity_tN2;
		continuity_fN = continuity_fN2;
	}

	ARow = ARow + 1;
	if (ARow == feature_at_pointer_l->NrOfRows)
		return;
	str = feature_at_pointer_l->Elem[1][ARow]; // time
	if(str=="")return;
	float X = str.ToDouble() / option->advanceTb->Value; // slice number

	// this takes care of the full file mode
if(!pagination_on)
	{
	  switch(deriv_disp->ItemIndex)
	  {
			case 0: calci->GetImage().DrawAllSlices(image); break;
			case 1: calci->GetContours().DrawAllSlices(image); break;
			case 2: calci->GetSonogram().DrawAllSlices(image); break;
	  }
	  image->Canvas->Pen->Width = 1;
	  image->Canvas->Pen->Color = clRed;
	  image->Canvas->MoveTo(X, 0);
	  image->Canvas->LineTo(X, 240);
	  //int xx=Image1->Width;
	  scroll->HorzScrollBar->Position=max(0, X-500);
	  set_scale(song);
	}
	else // for pagination
	{
		int coord = (X-500.0) * (float)preview_sono->Width / (float)tot_pages;
		prev_cursor->Left = coord+30;
		coord = (X-500.0);// * (float)preview_sono->Width / (float)tot_pages;
		if(coord<1){left_adjusted_outline=X; coord=1; left_edge=true;
		}else left_edge=false;

		//TMouseButton Btn;
		//TShiftState Sh;
		//preview_sonoMouseUp(this, Btn, Sh, coord, 0);



	if (display->ActivePage == Song1) {
		option->currentStartLocation=coord;
		option->pageNumber=coord/1000;
		Analyze_song1_light(song1_name, 1, coord);
	}
	else
	{
	   option->currentStartLocation2=coord;
	   option->pageNumber2=coord/1000;
	   Analyze_song2(song2_name, 1);
	}


		switch(deriv_disp->ItemIndex)
		{
			case 0: calci->GetImage().DrawAllSlices(image); break;
			case 1: calci->GetContours().DrawAllSlices(image); break;
			case 2: calci->GetSonogram().DrawAllSlices(image); break;
		}

		image->Canvas->Pen->Width = 1;
		image->Canvas->Pen->Color = clRed;
		if(left_edge)
		{
		   image->Canvas->MoveTo(left_adjusted_outline, 0);
		   image->Canvas->LineTo(left_adjusted_outline, 240);
		}
		else
		{
		  image->Canvas->MoveTo(510, 0);
		  image->Canvas->LineTo(510, 240);
		}
		set_scale(song);
	}
	// update the numlabs:
	str = feature_at_pointer_l->Elem[1][ARow];
	time->Value = str.ToDouble();
	str = feature_at_pointer_l->Elem[2][ARow];
	amp->Value = str.ToDouble();
	str = feature_at_pointer_l->Elem[3][ARow];
	pitch->Value = str.ToDouble();
	str = feature_at_pointer_l->Elem[4][ARow];
	pf->Value = str.ToDouble();
	str = feature_at_pointer_l->Elem[5][ARow];
	Principal_frN->Value = str.ToDouble();
	str = feature_at_pointer_l->Elem[6][ARow];
	Pgood->Value = str.ToDouble();
	str = feature_at_pointer_l->Elem[7][ARow];
	mod->Value = str.ToDouble();
	str = feature_at_pointer_l->Elem[8][ARow];
	am->Value = str.ToDouble();
	str = feature_at_pointer_l->Elem[9][ARow];
	ent->Value = str.ToDouble();
	str = feature_at_pointer_l->Elem[10][ARow];
	continuity_tN->Value = str.ToDouble();
	str = feature_at_pointer_l->Elem[11][ARow];
	continuity_fN->Value = str.ToDouble();
	str = feature_at_pointer_l->Elem[12][ARow];
	slopeN->Value = str.ToDouble();

}
// ---------------------------------------------------------------------------

// highlight a selected column of feature values in song 1 or song 2
void TForm1::set_feature_col_highlight(int col, bool the_song) {
    TReportListView *feature_at_pointer_lst, *feature_vector_lst,
    *feature_statistics_Vw;

    if (the_song == Ksong1) {
        feature_at_pointer_lst = feature_at_pointer_list;
        feature_vector_lst = feature_vector_list;
        feature_statistics_Vw = feature_statistics_View;
    }
    else {
        feature_at_pointer_lst = feature_at_pointer_list2;
        feature_vector_lst = feature_vector_list2;
        feature_statistics_Vw = feature_statistics_View2;
    }

    if (feature_at_pointer_lst->NrOfRows > 1) {
        feature_at_pointer_lst->SuppressPaint = true;
        for (int i = 1; i <= feature_at_pointer_lst->NrOfColumns; i++)
            feature_at_pointer_lst->ColumnSelected[i] = 0;
        if (!col) {
            feature_at_pointer_lst->SuppressPaint = false;
            // option->doNotSegment = true;
        }
        else {
            feature_at_pointer_lst->ColumnSelected[col] = 1;
            feature_at_pointer_lst->SuppressPaint = false;
            // option->doNotSegment = false;
        }

    }

	if (feature_vector_lst->NrOfRows > 1) {
        feature_vector_lst->SuppressPaint = true;
        for (int i = 1; i <= feature_vector_lst->NrOfColumns; i++)
            feature_vector_lst->ColumnSelected[i] = 0;
        if (!col)
            feature_vector_lst->SuppressPaint = false;
        else {
            feature_vector_lst->ColumnSelected[col] = 1;
            feature_vector_lst->SuppressPaint = false;
        }
    }

    if (feature_statistics_Vw->NrOfRows > 1) {
        feature_statistics_Vw->SuppressPaint = true;
        for (int i = 1; i <= feature_statistics_Vw->NrOfColumns; i++)
            feature_statistics_Vw->ColumnSelected[i] = 0;
        if (!col)
            feature_statistics_Vw->SuppressPaint = false;
        else {
            feature_statistics_Vw->ColumnSelected[col] = 1;
            feature_statistics_Vw->SuppressPaint = false;
        }
    }
}

// ---------------------------------------------------------------------------

void __fastcall TForm1::feature_vector_listSelectCell(TObject *Sender,
	int ACol, int ARow, bool &CanSelect) {
	TReportListView *feature_vector_l;
	TImage *image;
	CZT_FeatureCalc *calci;
	bool pagi;

	if (Sender == feature_vector_list) {
		feature_vector_l = feature_vector_list;
		calci = &m_Calculator1;
		image = Image1;
		sound = 0;
		pagi=option->paginationOn;
	} else {
		feature_vector_l = feature_vector_list2;
		calci = &m_Calculator2;
		image = Image2;
		sound = 1;
		pagi=option->paginationOn2;
	}
	//ARow = ARow + 1;
	if (ARow > feature_vector_l->NrOfRows-2)
		return;

	str = feature_vector_l->Elem[13][ARow+1];
	if (sound==0 && pagi) {
	   option->currentStartLocation = str.ToInt();
	   Analyze_song1_light(song1_name, 1, option->currentStartLocation);
	} else if (sound==1 && pagi) {
	   option->currentStartLocation2 = str.ToInt();
	   Analyze_song2(song2_name, 1);
	}

	str = feature_vector_l->Elem[1][ARow+1];
	if(str=="")return;
	float X = str.ToDouble() / option->advanceTb->Value;
	switch(deriv_display->ItemIndex)
	{
		case 0: calci->GetImage().DrawAllSlices(image);break;
		case 1: calci->GetContours().DrawAllSlices(image);break;
		case 2: calci->GetSonogram().DrawAllSlices(image);break;
	}
	image->Canvas->Pen->Width = 1;
	image->Canvas->Pen->Color = clRed;
	image->Canvas->MoveTo(X, 0);
	image->Canvas->LineTo(X, 240);
	set_scale(sound);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::features_displayChange(TObject *Sender) {
	CZT_FeatureCalc *calci;
	TImage *image;
	TPageControl *features_disp;
	TSpeedButton *das;
	TEdit *Comments;
	TShape *drag_curs;
	TRadioGroup *disp;
	bool prev_st;
	bool paginate;
	bool snd;

	if (Sender == features_display) {
		calci = &m_Calculator1;
		image = Image1;
		snd=0;
		paginate=paginate = option->paginationOn;
		prev_st=preview_state;
		disp=deriv_display;
		features_disp = features_display;
		das = persistantB;
		Comments = comments;
		drag_curs = drag_cursor;
	}
	else {
		calci = &m_Calculator2;
		image = Image2;
		snd=1;
		paginate=paginate = option->paginationOn2;
		prev_st=preview_state2;
		disp=deriv_display2;
		features_disp = features_display2;
		das = persistantB2;
		Comments = comments2;
		drag_curs = drag_cursor2;
	}

	switch(disp->ItemIndex)
	{
	  case 0: calci->GetImage().DrawAllSlices(image); break;
	  case 1: calci->GetContours().DrawAllSlices(image); break;
	  case 2: calci->GetSonogram().DrawAllSlices(image); break;
	}
	set_scale(snd);

	if (features_disp->TabIndex == 2) // display statistics
	{
		das->Visible = false;
		Comments->Left = 668; // 724;
		Fill_feature_headers();
	}
	else if (features_disp->TabIndex == 0){
		drag_curs->Visible = false;
		Comments->Left = 724;
		das->Visible = true;
	}

	else {
		das->Visible = true;
		Comments->Left = 724;
	}
	/*
	if(paginate && !prev_st){
		image->AutoSize = false;
		image->Height = 175;
		image->Stretch = true;
	}
	*/
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::feature_vector_to_clipboardClick(TObject *Sender) {
	TReportListView *vector_list;
	if (Sender == feature_vector_to_clipboard || display->ActivePage==Song1)
		vector_list = feature_vector_list;
	else
        vector_list = feature_vector_list2;
    vector_list->CopyToClipBoard(1, 1, 12, vector_list->NrOfRows, 1, 1);

}
// ---------------------------------------------------------------------------

void __fastcall TForm1::feature_vector_XMLClick(TObject *Sender) {
    str = SaveDialog1->Filter;
    SaveDialog1->Filter = "XML Files|*.xml";
    if (SaveDialog1->Execute()) {
        feature_vector_list->SaveAsXMLFile
            (StripExtension(SaveDialog1->FileName) + ".xml",
            "features_vector");
    }
    SaveDialog1->Filter = str;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::delete_featurevector_rowsClick(TObject *Sender) {
    TReportListView *vector_list;
    if (Sender == delete_featurevector_rows)
        vector_list = feature_vector_list;
    else
        vector_list = feature_vector_list2;
    bool fnd = false;
    for (int i = 1; i <= vector_list->NrOfRows; i++)
        if (vector_list->RowSelected[i]) {
            vector_list->RemoveRow(i);
            fnd = true;
        }
    if (!fnd)
        MessageDlg(
        "No row selected. Please click on a row to highlight it and try again",
        mtError, TMsgDlgButtons() << mbOK, 0);

}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Clear_featurelistClick(TObject *Sender) {
	if(display->ActivePage==Song1){
	feature_at_pointer_list->Clear();
	feature_at_pointer_cursor = 1;
	feature_at_pointer_list->NrOfRows = 1;
	}else{
		feature_at_pointer_list2->Clear();
		feature_at_pointer2_cursor = 1;
		feature_at_pointer_list2->NrOfRows = 1;
    }
	Fill_feature_headers();
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::delete_featurevectorClick(TObject *Sender) {
	TReportListView *vector_list;
	int *feature_vector_curs;
	if (Sender == delete_featurevector || display->ActivePage == Song1){
		vector_list = feature_vector_list;
		feature_vector_curs= &feature_vector_cursor;
	}
	else {
		vector_list = feature_vector_list2;
		feature_vector_curs=&feature_vector2_cursor;
	}
    vector_list->Clear();
	vector_list->NrOfRows = 1;
	*feature_vector_curs = 0;
    Fill_feature_headers();

}
// ---------------------------------------------------------------------------

void TForm1::Fill_feature_headers() {
    feature_at_pointer_list->Header[1] = "Time";
    feature_at_pointer_list->Header[2] = "Amplitude";
    feature_at_pointer_list->Header[3] = "Pitch";
    feature_at_pointer_list->Header[4] = "Mean_frequency";
	feature_at_pointer_list->Header[5] = "Peak_frequency";
    feature_at_pointer_list->Header[6] = "Goodness_of_pitch";
    feature_at_pointer_list->Header[7] = "FM";
    feature_at_pointer_list->Header[8] = "AM";
    feature_at_pointer_list->Header[9] = "Wiener_entropy";
    feature_at_pointer_list->Header[10] = "Continuity_time";
    feature_at_pointer_list->Header[11] = "Continuity_freq";
    feature_at_pointer_list->Header[12] = "Duration_of_state";

    feature_vector_list->Header[1] = "Time";
    feature_vector_list->Header[2] = "Amplitude";
    feature_vector_list->Header[3] = "Pitch";
    feature_vector_list->Header[4] = "Mean_frequency";
	feature_vector_list->Header[5] = "Peak_frequency";
    feature_vector_list->Header[6] = "Goodness_of_pitch";
    feature_vector_list->Header[7] = "FM";
    feature_vector_list->Header[8] = "AM";
    feature_vector_list->Header[9] = "Wiener_entropy";
    feature_vector_list->Header[10] = "Continuity_time";
    feature_vector_list->Header[11] = "Continuity_freq";
    feature_vector_list->Header[12] = "Duration_of_state";
    feature_vector_list->Header[13] = "currStartLocation";

    feature_statistics_View->Header[1] = "Duration";
    feature_statistics_View->Header[2] = "Amplitude";
    feature_statistics_View->Header[3] = "Pitch";
    feature_statistics_View->Header[4] = "Mean freq";
	feature_statistics_View->Header[5] = "Peak_freq";
	feature_statistics_View->Header[6] = "Goodness";
    feature_statistics_View->Header[7] = "FM";
    feature_statistics_View->Header[8] = "AM";
    feature_statistics_View->Header[9] = "Entropy";
    feature_statistics_View->Header[10] = "Cont(t)";
    feature_statistics_View->Header[11] = "Cont(f)";
    feature_statistics_View->Header[12] = "comments";

    feature_statistics_View->Header[13] = "start";
    feature_statistics_View->Header[14] = "Min(Amp)";
    feature_statistics_View->Header[15] = "Min(Pitch)";
    feature_statistics_View->Header[16] = "Min(freq)";
	feature_statistics_View->Header[17] = "Min(Peak)";
    feature_statistics_View->Header[18] = "Min(Good)";
    feature_statistics_View->Header[19] = "Min(FM)";
    feature_statistics_View->Header[20] = "Min(AM)";
    feature_statistics_View->Header[21] = "Min(Ent)";
    feature_statistics_View->Header[22] = "Min(Cont(t))";
    feature_statistics_View->Header[23] = "Min(Cont(f))";
    feature_statistics_View->Header[24] = "comments";

    feature_statistics_View->Header[25] = "end";
    feature_statistics_View->Header[26] = "Max(Amp)";
    feature_statistics_View->Header[27] = "Max(Pitch)";
    feature_statistics_View->Header[28] = "Max(freq)";
	feature_statistics_View->Header[29] = "Max(Peak)";
    feature_statistics_View->Header[30] = "Max(Good)";
    feature_statistics_View->Header[31] = "Max(FM)";
    feature_statistics_View->Header[32] = "Max(AM)";
    feature_statistics_View->Header[33] = "Max(Ent)";
    feature_statistics_View->Header[34] = "Max(Cont(t))";
    feature_statistics_View->Header[35] = "Max(Cont(f))";
    feature_statistics_View->Header[36] = "comments";

    feature_statistics_View->Header[37] = " ";
    feature_statistics_View->Header[38] = "STD(Amp)";
    feature_statistics_View->Header[39] = "STD(Pitch)";
    feature_statistics_View->Header[40] = "STD(freq)";
	feature_statistics_View->Header[41] = "STD(Peak)";
    feature_statistics_View->Header[42] = "STD(Good)";
	feature_statistics_View->Header[43] = "STD(FM)";
    feature_statistics_View->Header[44] = "STD(AM)";
    feature_statistics_View->Header[45] = "STD(Ent)";
    feature_statistics_View->Header[46] = "STD(Cont(t))";
    feature_statistics_View->Header[47] = "STD(Cont(f))";
    feature_statistics_View->Header[48] = "comments";
    feature_statistics_View->Header[49] = "currStartLocation";

    feature_at_pointer_list2->Header[1] = "Time";
    feature_at_pointer_list2->Header[2] = "Amplitude";
    feature_at_pointer_list2->Header[3] = "Pitch";
    feature_at_pointer_list2->Header[4] = "Mean_frequency";
	feature_at_pointer_list2->Header[5] = "Peak_frequency";
    feature_at_pointer_list2->Header[6] = "Goodness_of_pitch";
    feature_at_pointer_list2->Header[7] = "FM";
    feature_at_pointer_list2->Header[8] = "AM";
    feature_at_pointer_list2->Header[9] = "Wiener_entropy";
    feature_at_pointer_list2->Header[10] = "Continuity_time";
    feature_at_pointer_list2->Header[11] = "Continuity_freq";
    feature_at_pointer_list2->Header[12] = "Duration_of_state";

    feature_vector_list2->Header[1] = "Time";
    feature_vector_list2->Header[2] = "Amplitude";
    feature_vector_list2->Header[3] = "Pitch";
    feature_vector_list2->Header[4] = "Mean_frequency";
	feature_vector_list2->Header[5] = "Peak_frequency";
    feature_vector_list2->Header[6] = "Goodness_of_pitch";
    feature_vector_list2->Header[7] = "FM";
    feature_vector_list2->Header[8] = "AM";
    feature_vector_list2->Header[9] = "Wiener_entropy";
    feature_vector_list2->Header[10] = "Continuity_time";
    feature_vector_list2->Header[11] = "Continuity_freq";
    feature_vector_list2->Header[12] = "Duration_of_state";
    feature_vector_list2->Header[13] = "currStartLocation";

    feature_statistics_View2->Header[1] = "Duration";
    feature_statistics_View2->Header[2] = "Amplitude";
    feature_statistics_View2->Header[3] = "Pitch";
    feature_statistics_View2->Header[4] = "Mean freq";
	feature_statistics_View2->Header[5] = "Peak_freq";
    feature_statistics_View2->Header[6] = "Goodness";
    feature_statistics_View2->Header[7] = "FM";
	feature_statistics_View2->Header[8] = "AM";
	feature_statistics_View2->Header[9] = "Entropy";
	feature_statistics_View2->Header[10] = "Cont(t)";
	feature_statistics_View2->Header[11] = "Cont(f)";
	feature_statistics_View2->Header[12] = "comments";

	feature_statistics_View2->Header[13] = "start";
	feature_statistics_View2->Header[14] = "Min(Amp)";
	feature_statistics_View2->Header[15] = "Min(Pitch)";
	feature_statistics_View2->Header[16] = "Min(freq)";
	feature_statistics_View2->Header[17] = "Min(peak)";
	feature_statistics_View2->Header[18] = "Min(Good)";
	feature_statistics_View2->Header[19] = "Min(FM)";
	feature_statistics_View2->Header[20] = "Min(AM)";
	feature_statistics_View2->Header[21] = "Min(Ent)";
	feature_statistics_View2->Header[22] = "Min(Cont(t))";
	feature_statistics_View2->Header[23] = "Min(Cont(f))";
	feature_statistics_View2->Header[24] = "comments";

	feature_statistics_View2->Header[25] = "end";
	feature_statistics_View2->Header[26] = "Max(Amp)";
	feature_statistics_View2->Header[27] = "Max(Pitch)";
	feature_statistics_View2->Header[28] = "Max(peak)";
	feature_statistics_View2->Header[29] = "Max(Cont)";
	feature_statistics_View2->Header[30] = "Max(Good)";
	feature_statistics_View2->Header[31] = "Max(FM)";
	feature_statistics_View2->Header[32] = "Max(AM)";
	feature_statistics_View2->Header[33] = "Max(Ent)";
	feature_statistics_View2->Header[34] = "Max(Cont(t))";
	feature_statistics_View2->Header[35] = "Max(Cont(f))";
	feature_statistics_View2->Header[36] = "comments";

	feature_statistics_View2->Header[37] = " ";
	feature_statistics_View2->Header[38] = "STD(Amp)";
	feature_statistics_View2->Header[39] = "STD(Pitch)";
	feature_statistics_View2->Header[40] = "STD(freq)";
	feature_statistics_View2->Header[41] = "STD(peak)";
	feature_statistics_View2->Header[42] = "STD(Good)";
	feature_statistics_View2->Header[43] = "STD(FM)";
	feature_statistics_View2->Header[44] = "STD(AM)";
	feature_statistics_View2->Header[45] = "STD(Ent)";
	feature_statistics_View2->Header[46] = "STD(Cont(t))";
	feature_statistics_View2->Header[47] = "STD(Cont(f))";
	feature_statistics_View2->Header[48] = "comments";
	feature_statistics_View2->Header[49] = "currStartLocation";
}




///////////////////////////////////////////////////////////////////////////////////////////







void __fastcall TForm1::feature_statistics_ViewSelectCell(TObject *Sender,
	int ACol, int ARow, bool &CanSelect) {
	// ARow=ARow+1;
	TReportListView *feature_statistics_V;
	TShape *drag_curs;
	TImage *image;
	TRadioGroup *disp;
	CZT_FeatureCalc *calci;
	bool paginate;

	if (Sender == feature_statistics_View) {
		feature_statistics_V = feature_statistics_View;
		drag_curs = drag_cursor;
		disp=deriv_display;
		calci = &m_Calculator1;
		image = Image1;
		paginate = option->paginationOn;
		sound = 0;
	}

	else {
		feature_statistics_V = feature_statistics_View2;
		drag_curs = drag_cursor2;
		disp=deriv_display2;
		calci = &m_Calculator2;
		image = Image2;
		paginate = option->paginationOn2;
		sound = 1;

	}

	if (ARow == feature_statistics_V->NrOfRows)
		return;
	str = feature_statistics_V->Elem[49][ARow];    // starting of pagination (option->currentStartLocation)...
	if (Sender == feature_statistics_View && option->paginationOn) {
	   option->currentStartLocation = str.ToInt();
	   Analyze_song1_light(song1_name, 1, option->currentStartLocation);
	} else if (Sender == feature_statistics_View2 && option->paginationOn2) {
	   option->currentStartLocation2 = str.ToInt();
	   Analyze_song2(song2_name, 1);
	}
	drag_curs->Visible = false;
	bool left_edge=false;


	// find time slices on start X and end Y
	str = feature_statistics_V->Elem[13][ARow]; // start on (time)
	float X = str.ToDouble() / option->advanceTb->Value; // slice number (song wide)
	str = feature_statistics_V->Elem[25][ARow]; // end on (time)
	float Y = str.ToDouble() / option->advanceTb->Value;

	if(paginate)
	{
		// find the starting point (coord) for the preview window
		int coord = (X-300.0) * (float)preview_sono->Width / (float)option->totalPages;
		if(coord<1){ coord=1; left_edge=true; }
		preview_cursor->Left = coord+30;
		coord = (X-300.0);
		//TMouseButton Btn;
		//	TShiftState Sh;
		//	preview_cursor->Left = coord;
		// plot the sonogram based on preview window
		//	preview_sonoMouseUp(this, Btn, Sh, coord, 0);


	if (display->ActivePage == Song1) {
		option->currentStartLocation=coord;
		option->pageNumber=coord/1000;
		Analyze_song1_light(song1_name, 1, coord);
	}
	else
	{
	   option->currentStartLocation2=coord;
	   option->pageNumber2=coord/1000;
	   Analyze_song2(song2_name, 1);
	}



		switch(disp->ItemIndex)
		{
		  case 0: calci->GetImage().DrawAllSlices(image); break;
		  case 1: calci->GetContours().DrawAllSlices(image); break;
		  case 2: calci->GetSonogram().DrawAllSlices(image); break;
		}
		set_scale(sound);

		CalculateIntervalMeasures(300,300+Y-X);
											/*  */
		image->Canvas->Pen->Width = 1;
		image->Canvas->Pen->Color = clRed;
		if(!left_edge) // normal case, not dealing with an edge
		{
		  image->Canvas->MoveTo(300, 0);
		  image->Canvas->LineTo(300, 240);
		  image->Canvas->Pen->Color = clBlue;
		  image->Canvas->MoveTo(300+Y-X, 0);
		  image->Canvas->LineTo(300+Y-X, 240);
		}
		else
		{
		  image->Canvas->MoveTo(X, 0);
		  image->Canvas->LineTo(X, 240);
		  image->Canvas->Pen->Color = clBlue;
		  image->Canvas->MoveTo(Y, 0);
		  image->Canvas->LineTo(Y, 240);
		}
	}

	else{

			switch(disp->ItemIndex)
			{
			  case 0: calci->GetImage().DrawAllSlices(image); break;
			  case 1: calci->GetContours().DrawAllSlices(image); break;
			  case 2: calci->GetSonogram().DrawAllSlices(image); break;
			}
			image->Canvas->Pen->Width = 1;
			image->Canvas->Pen->Color = clRed;
			image->Canvas->MoveTo(X, 0);
			image->Canvas->LineTo(X, 240);
			image->Canvas->Pen->Color = clBlue;
			image->Canvas->MoveTo(Y, 0);
			image->Canvas->LineTo(Y, 240);
			if(!paginate || Y<1000)	CalculateIntervalMeasures(X, Y);
			set_scale(sound);
			if(sound==0 && !paginate)ScrollBox3->HorzScrollBar->Position=max(0, X-500);
            if(sound==1 && !paginate)ScrollBox4->HorzScrollBar->Position=max(0, X-500);
		}
}



// ---------------------------------------------------------------------------



void TForm1::deselect_rows() {
 if(display->ActivePage==Song1)
 {
	for (int i = 1; i <= feature_at_pointer_list->NrOfRows; i++)
		feature_at_pointer_list->RowSelected[i] = false;
	for (int i = 1; i <= feature_vector_list->NrOfRows; i++)
		feature_vector_list->RowSelected[i] = false;
	for (int i = 1; i <= feature_statistics_View->NrOfRows; i++)
		feature_statistics_View->RowSelected[i] = false;
 }else {
	 for (int i = 1; i <= feature_at_pointer_list2->NrOfRows; i++)
		feature_at_pointer_list2->RowSelected[i] = false;
	for (int i = 1; i <= feature_vector_list2->NrOfRows; i++)
		feature_vector_list2->RowSelected[i] = false;
	for (int i = 1; i <= feature_statistics_View2->NrOfRows; i++)
		feature_statistics_View2->RowSelected[i] = false;
 }

}

void __fastcall TForm1::clear_selectionClick(TObject *Sender) {
	deselect_rows();
}

// ---------------------------------------------------------------------------

void __fastcall TForm1::advanceDisplayClick(TObject *Sender) {
	// int theEnd = wave.GetTotalSamples(); //m_Calculator1.features.duration;
	// if (option->currentStartLocation+advanceTb->Value*750 > theEnd) {
	// option->pageNumber++;
	bool *show_adv, *pagination_On, *update_sec, *update_seg, prev_state;
	TShape *prev_cursor;
	TImage *prev_sono, *image;
	TRadioGroup *disp;
	TSpeedButton *amp_btn, *pitch_btn, *meanFr_btn, *goodness_btn, *FM_btn,
	*AM_btn, *entropy_btn, *contt_btn, *contf_btn, *das_btn, *princCont_btn;
	TButton *adv_disp;
	CZT_FeatureCalc *calci;
	int tot_pages, *current_location;
	if (Sender == advanceDisplay) {
		current_location = &option->currentStartLocation;
		tot_pages = option->totalPages;
		disp=deriv_display;
		prev_cursor = preview_cursor;
		prev_state=preview_state;
		prev_sono = preview_sono;
		amp_btn = DerivPowerB;
		pitch_btn = PitchB;
		meanFr_btn = PowerB;
		goodness_btn = PitchGoodnessB;
		FM_btn = FMB;
		AM_btn = AMB;
		entropy_btn = EntropyB;
		contt_btn = continulity_tB;
		contf_btn = continuity_fB;
		das_btn = persistantB;
		princCont_btn = principal_frB;
		calci = &m_Calculator1;
		image = Image1;
		adv_disp = advanceDisplay;
		first_snd = true;
		show_adv = &showAdvance;
		pagination_On = &option->paginationOn;
		update_sec = &updateSegmentationSec;
		update_seg = &updateSegmentation;
		Analyze_song1_light(song1_name, 1, option->currentStartLocation);
	}
	else {
		current_location = &option->currentStartLocation2;
		tot_pages = option->totalPages2;
		prev_cursor = preview_cursor2;
		disp=deriv_display2;
		prev_sono = preview_sono2;
		prev_state=preview_state2;
		amp_btn = DerivPower2B;
		pitch_btn = Pitch2B;
		meanFr_btn = Power2B;
		goodness_btn = PitchGoodnessB2;
		FM_btn = FM2B;
		AM_btn = AM2B;
		entropy_btn = Entropy2B;
		contt_btn = continulity_tB2;
		contf_btn = continuity_fB2;
		das_btn = persistantB2;
		princCont_btn = principal_frB2;
		calci = &m_Calculator2;
		image = Image2;
		adv_disp = advanceDisplay2;
		first_snd = false;
		show_adv = &showAdvance2;
		pagination_On = &option->paginationOn2;
		update_sec = &updateSegmentationSec2;
		update_seg = &updateSegmentation2;
		Analyze_song2(song2_name, 1);
	}
	doIncrement = true;
	// set_scale(0);
	// Sleep(50);

	if (tot_pages > 0) {
		prev_cursor->Left = prev_sono->Width * (*current_location - 1000)
			/ tot_pages + 36;
		prev_cursor->Width = 1000 * prev_sono->Width / tot_pages;
		if(prev_cursor->Width<5)prev_cursor->Width=5;
	}
   /*
	if (amp_btn->Down || pitch_btn->Down || meanFr_btn->Down ||
		goodness_btn->Down || FM_btn->Down || AM_btn->Down ||
		entropy_btn->Down || contt_btn->Down || contf_btn->Down ||
		das_btn->Down || princCont_btn->Down) {
		option->doNotSegment = false;
	}    */
	if (!option->doNotSegment) {
		if (option->sec_segmentation == true) {
			*update_sec = true;
			entropyTbChange(this);
		}
		*update_seg = true;
		derivTBChange(this);

	}
	else {
		Segment_Sound(0, 1000, !first_snd); // the logic here happens to be opposite with !first_sound=ksong1...
		if (!(*pagination_On)){
			switch(disp->ItemIndex)
			{
				case 0: calci->GetImage().DrawAllSlices(image); break;
				case 1: calci->GetContours().DrawAllSlices(image); break;
				case 2: calci->GetSonogram().DrawAllSlices(image); break;
			}
		}
		else{
			switch(disp->ItemIndex)
			{
				case 0: calci->GetImage().DrawSlices(0, 1000 - 1, image); break;
				case 1: calci->GetContours().DrawSlices(0, 1000 - 1, image); break;
				case 2: calci->GetSonogram().DrawSlices(0, 1000 - 1, image); break;
			}
		}
		if (first_snd) {
			set_scale(0);
		}
		else {
			set_scale(1);
		}
		// set_scale(!first_snd);  // here too the logic is inverse...
	}

	image->AutoSize = true;
	image->Stretch = false;
	image->AutoSize = false;
	if(!prev_state){
		image->Height = 175;
		image->Stretch = true;
	}
	else {
		image->AutoSize = false;
		image->Stretch = true;
	}

	if (!*show_adv)
		adv_disp->Visible = false;

}
// ---------------------------------------------------------------------------

void __fastcall TForm1::goBackDisplayClick(TObject *Sender) {
	//int begin = 0;
	int *currentStartLoc, *pageNum, *totalPages;
	bool *pagination_On, *show_Back, prev_state;
	TSpeedButton *amp_btn, *pitch_btn, *meanFr_btn, *goodness_btn, *FM_btn,
	*AM_btn, *entropy_btn, *contt_btn, *contf_btn, *das_btn, *princCont_btn;
	TButton *goBackDisp;
	TShape *preview_curs;
	TRadioGroup *disp;
	TImage *preview_sonogram, *image;
	CZT_FeatureCalc *calci;
	bool *update_sec, *update_seg;
	if (Sender == goBackDisplay) {
		currentStartLoc = &option->currentStartLocation;
		pageNum = &option->pageNumber;
		first_snd = true;
		totalPages = &option->totalPages;
		pagination_On = &option->paginationOn;
		show_Back = &showBack;
		preview_curs = preview_cursor;
		preview_sonogram = preview_sono;
		prev_state=preview_state;
		amp_btn = DerivPowerB;
		pitch_btn = PitchB;
		meanFr_btn = PowerB;
		goodness_btn = PitchGoodnessB;
		FM_btn = FMB;
		AM_btn = AMB;
		entropy_btn = EntropyB;
		contt_btn = continulity_tB;
		contf_btn = continuity_fB;
		das_btn = persistantB;
		princCont_btn = principal_frB;
		image = Image1;
		disp=deriv_display;
		calci = &m_Calculator1;
		goBackDisp = goBackDisplay;
		update_sec = &updateSegmentationSec;
		update_seg = &updateSegmentation;
	}
	else {
		currentStartLoc = &option->currentStartLocation2;
		pageNum = &option->pageNumber2;
		pagination_On = &option->paginationOn2;
		first_snd = false;
		totalPages = &option->totalPages2;
		show_Back = &showBack2;
		preview_curs = preview_cursor2;
		preview_sonogram = preview_sono2;
		prev_state=preview_state2;
		amp_btn = DerivPower2B;
		pitch_btn = Pitch2B;
		meanFr_btn = Power2B;
		goodness_btn = PitchGoodnessB2;
		FM_btn = FM2B;
		AM_btn = AM2B;
		entropy_btn = Entropy2B;
		contt_btn = continulity_tB2;
		contf_btn = continuity_fB2;
		das_btn = persistantB2;
		princCont_btn = principal_frB2;
		image = Image2;
		disp=deriv_display2;
		calci = &m_Calculator2;
		goBackDisp = goBackDisplay2;
		update_sec = &updateSegmentationSec2;
		update_seg = &updateSegmentation2;
	}
	doIncrement = true;

	if (*currentStartLoc > 2000) {
		*currentStartLoc -= 2000;
	} else {
		*currentStartLoc = 0;
	}
	//begin = *currentStartLoc;
	if (Sender == goBackDisplay)
		Analyze_song1_light(song1_name, 1, *currentStartLoc);
	else
		Analyze_song2(song2_name, 1);

	(*pageNum) -= 2;

	if (*pageNum < 1) {
		*pageNum = 1;
		// *show_Back = false;
	}
	if (*currentStartLoc < 1001) {
		*show_Back = false;
	}
	else {
		*show_Back = true;
	}
	if (first_snd) {
		set_scale(0);
	}
	else {
		set_scale(1);
	}

	// set_scale(!first_snd);

	if (*totalPages > 0) {
		preview_curs->Left = preview_sonogram->Width *
			(*currentStartLoc - 1000) / (*totalPages) + 36;
		preview_curs->Width = 1000 * preview_sonogram->Width / (*totalPages);
	}
 /*
	if (amp_btn->Down || pitch_btn->Down || meanFr_btn->Down ||
		goodness_btn->Down || FM_btn->Down || AM_btn->Down ||
		entropy_btn->Down || contt_btn->Down || contf_btn->Down ||
		das_btn->Down || princCont_btn->Down) {
		option->doNotSegment = false;
	}
   */
	if (!option->doNotSegment) {
		if (option->sec_segmentation == true) {
			*update_sec = true;
			entropyTbChange(this);
		}
		*update_seg = true;
		derivTBChange(this);

	}
	else {
		Segment_Sound(0, 1000, 0);
		if (!(*pagination_On)) {
			switch(disp->ItemIndex)
			{
				case 0: calci->GetImage().DrawAllSlices(image); break;
				case 1: calci->GetContours().DrawAllSlices(image); break;
				case 2: calci->GetSonogram().DrawAllSlices(image); break;
			}
		}
		else {
			switch(disp->ItemIndex)
			{
				case 0: calci->GetImage().DrawSlices(0, 1000 - 1, image); break;
				case 1: calci->GetContours().DrawSlices(0, 1000 - 1, image); break;
				case 2: calci->GetSonogram().DrawSlices(0, 1000 - 1, image); break;
			}
		}
		if (first_snd) {
			set_scale(0);
		}
		else {
			set_scale(1);
		}
		// set_scale(!first_snd);
	}

	image->AutoSize = true;
	image->Stretch = false;
	image->AutoSize = false;
	if(!prev_state){
		image->Height = 175;
		image->Stretch = true;
	}
	else {
		image->AutoSize = false;
		image->Stretch = true;
	}

	if (!(*show_Back))
		goBackDisp->Visible = false;

}
// ---------------------------------------------------------------------------

void __fastcall TForm1::preview_sonoMouseDown(TObject *Sender,
	TMouseButton Button, TShiftState Shift, int X, int Y) {
	if(display->ActivePage==Song1)preview_cursor->Left = X + 36;
	else preview_cursor2->Left = X + 36;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::cutClick(TObject *Sender) {
	features_to_clipboardClick(this);
	Clear_featurelistClick(this);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::paginationClick(TObject *Sender) {
	int *currentStartLoc;
	bool *show_adv, *pagination_On, *show_Back;
	TImage *image;
	TRadioGroup *page;
	if(no_pagination_action==true){no_pagination_action=false; return;}
	if (dobatch)return;
	Screen->Cursor = crHourGlass;
	if (Sender == pagination) {
		image = Image1;
		page = pagination;
		show_adv = &showAdvance;
		show_Back = &showBack;
		currentStartLoc = &option->currentStartLocation;
		pagination_On = &option->paginationOn;
	}
	else {
		image = Image2;
		page = pagination2;
		show_adv = &showAdvance2;
		show_Back = &showBack2;
		currentStartLoc = &option->currentStartLocation2;
		pagination_On = &option->paginationOn2;
	}

	if (!page->ItemIndex) {
		*pagination_On = false;
		*show_Back = false;
		*show_adv = false;
		if (Sender == pagination)
			Analyze_song1(open_sound->FileName, 1);
		else
			Analyze_song2(open_sound->FileName2, 1);
		// !!!!! open_sound is going to work unreliably -- we need a songname record for 1 & 2
	}

	else {
		*pagination_On = true;
		*show_Back = true;
		*show_adv = true;
		if (Sender == pagination)
			Analyze_song1_light(open_sound->FileName, 1, *currentStartLoc);
		else
			Analyze_song2(open_sound->FileName2, 1);
	}


	if (Sender == pagination) {
		if(option->paginationOn)
		{
		   ScrollBox3->HorzScrollBar->Visible=false;
		   Preview->Visible=true;
		   preview_state=true;
		   PreviewClick(this);
		}
		else
		{
		   ScrollBox3->HorzScrollBar->Visible=true;
		   Preview->Visible=false;
		   preview_state=false;
		   PreviewClick(this);
		}
	}
	else {
		if(option->paginationOn2)
		{
		   ScrollBox4->HorzScrollBar->Visible=false;
		   Preview2->Visible=true;
		   preview_state2=true;
		   PreviewClick(this);
		}
		else
		{
		   ScrollBox4->HorzScrollBar->Visible=true;
		   Preview2->Visible=false;
		   preview_state2=false;
		   PreviewClick(this);
		}
	}
	Screen->Cursor = crDefault;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::switch_stats_diplayClick(TObject *Sender) {

	if(Sender==switch_stats_diplay)
	{
		switch(switch_stats_diplay->ItemIndex) {
		case 0:
			feature_statistics_View->LeftCol = 1;
			break;
		case 1:
			feature_statistics_View->LeftCol = 13;
			break;
		case 2:
			feature_statistics_View->LeftCol = 25;
			break;
		case 3:
			feature_statistics_View->LeftCol = 37;
			break;
		}
	}
	else
	{
	   switch(switch_stats_diplay2->ItemIndex) {
		case 0:
			feature_statistics_View2->LeftCol = 1;
			break;
		case 1:
			feature_statistics_View2->LeftCol = 13;
			break;
		case 2:
			feature_statistics_View2->LeftCol = 25;
			break;
		case 3:
			feature_statistics_View2->LeftCol = 37;
			break;
		}
    }
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::copy_statsClick(TObject *Sender) {
    TReportListView *stat_view;
    if (Sender == copy_stats)
        stat_view = feature_statistics_View;
    else
        stat_view = feature_statistics_View2;
    stat_view->CopyToClipBoard(1, 1, 49, stat_view->NrOfRows, 1, 1);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Clear_featurestatviewClick(TObject *Sender) {

    feature_statistics_View->Clear();
	feature_statistics_cursor = 1;
	feature_statistics_View->NrOfRows = 1;
	feature_statistics_View->NrOfColumns = 50;
    Fill_feature_headers();
    //Fill_feature_headers();
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::cut_featurestatviewClick(TObject *Sender) {
    copy_statsClick(this);
    Clear_featurestatviewClick(this);

}
// ---------------------------------------------------------------------------

void __fastcall TForm1::templatesChange(TObject *Sender) {
    if (templates->Items->Count == 0) {
        Application->MessageBox(
            L"There are no saved settings. Click 'Save SAP settings' to save the current setting", NULL, MB_OK);
        return;
    }
    option->scale->First();
    if (templates->ItemIndex)
        for (int i = 0; i < templates->ItemIndex; i++)
            option->scale->Next();
    option->setScale();
    if (display->ActivePage == Song1)
        set_analysis_settings(0);
    else
        set_analysis_settings(1);

}
// ---------------------------------------------------------------------------

void __fastcall TForm1::feature_vector_cutClick(TObject *Sender) {
    feature_vector_to_clipboardClick(this);
    delete_featurevectorClick(this);
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::feature_vector_cut2Click(TObject *Sender) {
    feature_vector_list2->CopyToClipBoard(1, 1, 12,
        feature_vector_list2->NrOfRows, 1, 1);
    feature_vector_list2->Clear();
    feature_vector_list2->NrOfRows = 1;
    Fill_feature_headers();
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::save_statsClick(TObject *Sender) {
    TReportListView *stat_view;
    if (Sender == save_stats)
		stat_view = feature_statistics_View;
    else
        stat_view = feature_statistics_View2;
    str = SaveDialog1->Filter;
    SaveDialog1->Filter = "XML Files|*.xml";
    if (SaveDialog1->Execute()) {
        stat_view->SaveAsXMLFile(StripExtension(SaveDialog1->FileName)
            + ".xml", "features_vector");
    }
    SaveDialog1->Filter = str;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::preview_sono2MouseDown(TObject *Sender,
    TMouseButton Button, TShiftState Shift, int X, int Y) {
    preview_cursor2->Left = X + 36;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::Image2MouseUp(TObject *Sender, TMouseButton Button,
    TShiftState Shift, int X, int Y) {
	if (draged && state2 == 1) {
		offset_2 = OffsetSong2 = X;
        state2 = 2;
        sound = 1;
		done = CalculateIntervalMeasures(OnsetSong2, X);
        play_song2->EndPos = OffsetSong2 * advanceTb->Value;
        if (CalcMode->ItemIndex == auto_segment)
            save_syllables(ksong2);
        interval2 = true;
	   // ks_panel->Visible = false;
		menu_outline = 0;
		update_score_sounds=true;
        AddBtClick(this);
	}
}


// ---------------------------------------------------------------------------


void __fastcall TForm1::Clear_featurestatview2Click(TObject *Sender) {
	feature_statistics_View2->Clear();
    feature_statistics2_cursor = 1;
	feature_statistics_View2->NrOfRows = 1;
	feature_statistics_View2->NrOfColumns = 50;
	Fill_feature_headers();
    //Fill_feature_headers();
}
// ---------------------------------------------------------------------------

void TForm1::min_stop_durTbChange_sf(TObject *Sender) {
    if (option->stop_duration_thresh > 0) {
        return;
    }
}

// ---------------------------------------------------------------------------

void TForm1::min_stop_durTbChange_sf2(TObject *Sender) {
    if (option->stop_duration_thresh > 0) {
        return;
    }
}

// ---------------------------------------------------------------------------

void TForm1::bout_stopChange_sf(TObject *Sender) {
    if (option->bout_duration_thresh > 0) {
        return;
    }
}
// ---------------------------------------------------------------------------

void TForm1::bout_stopChange_sf2(TObject *Sender) {
    if (option->bout_duration_thresh > 0) {
        return;
    }
}
// ---------------------------------------------------------------------------

void TForm1::ComboBox1Change_sf(TObject *Sender) {
    first_snd = true;
    updateSegmentation = true;
    derivTBChange(this);
}

// ---------------------------------------------------------------------------

void TForm1::ComboBox1Change_sf2(TObject *Sender) {
    first_snd = false;
    updateSegmentation2 = true;
    derivTBChange(this);
}

// ---------------------------------------------------------------------------
void TForm1::secondaryFeatureDDChange_sf(TObject *Sender) {
    first_snd = true;
    updateSegmentationSec = true;
    entropyTbChange(this);
}

// ---------------------------------------------------------------------------
void TForm1::secondaryFeatureDDChange_sf2(TObject *Sender) {
    first_snd = false;
    updateSegmentationSec2 = true;
    entropyTbChange(this);
}

// ---------------------------------------------------------------------------
void TForm1::secondaryComparisonRBClick_sf(TObject *Sender) {
    first_snd = true;
    updateSegmentationSec = true;
    entropyTbChange(this);
}

// ---------------------------------------------------------------------------
void TForm1::secondaryComparisonRBClick_sf2(TObject *Sender) {
    first_snd = false;
    updateSegmentationSec2 = true;
    entropyTbChange(this);
}

// ---------------------------------------------------------------------------
void TForm1::segmentation_flipClick_sf(TObject *Sender) {
    first_snd = true;
    updateSegmentation = true;
    derivTBChange(this);
}

// ---------------------------------------------------------------------------
void TForm1::segmentation_flipClick_sf2(TObject *Sender) {
    first_snd = false;
    updateSegmentation2 = true;
    derivTBChange(this);
}

// ---------------------------------------------------------------------------
void TForm1::secondaryLogicRBClick_sf(TObject *Sender) {
    first_snd = true;
    updateSegmentationSec = true;
    entropyTbChange(this);
}

// ---------------------------------------------------------------------------
void TForm1::secondaryLogicRBClick_sf2(TObject *Sender) {
    first_snd = false;
    updateSegmentationSec2 = true;
    entropyTbChange(this);
}
// ---------------------------------------------------------------------------

void TForm1::secondaryFeatureCBClick_sf(TObject *Sender) {
    first_snd = true;
    if (option->sec_segmentation1) {
        entropyTb->Visible = true;
        entropy_val->Visible = true;
    }
    else {
        entropyTb->Visible = false;
        entropy_val->Visible = false;
        hide_all_features(&m_Calculator1);
    }
    option->sec_segmentation = option->sec_segmentation1;
    //entropyTb->Position = 125;   Should retrieve from DB!!!
    Frame11->secondaryThresholdTB->Value = entropy_val->Value;
    updateSegmentationSec = true;
    entropyTbChange(this);
    //m_Calculator1.do_hpfilt(); // recalc_light(0);

}
// ---------------------------------------------------------------------------

void TForm1::secondaryFeatureCBClick_sf2(TObject *Sender) {
    first_snd = false;
    if (option->sec_segmentation2) {
        entropyTb2->Visible = true;
        entropy_val2->Visible = true;
    }
    else {
        entropyTb2->Visible = false;
        entropy_val2->Visible = false;
        hide_all_features(&m_Calculator2);
    }
    option->sec_segmentation = option->sec_segmentation2;
    //entropyTb2->Position = 125;
    Frame12->secondaryThresholdTB->Value = entropy_val2->Value;
    updateSegmentationSec2 = true;
    entropyTbChange(this);
    //m_Calculator2.do_hpfilt(); // recalc_light(0);

}
// ---------------------------------------------------------------------------

void TForm1::smoothGraphClick_sf(TObject *Sender) {
    if (option->smooth_feature) {
        //m_Calculator1.do_hpfilt();
        // hp_smallChange_sf(this);
        first_snd = true;
        updateSegmentation = true;
        derivTBChange(this);
    }

}
// ---------------------------------------------------------------------------

void TForm1::smoothGraphClick_sf2(TObject *Sender) {
    if (option->smooth_feature2) {
        //m_Calculator2.do_hpfilt();
       first_snd = false;
       updateSegmentation2 = true;
       derivTBChange(this);
    }

}
// ---------------------------------------------------------------------------

void TForm1::adaptThresholdCBClick_sf(TObject *Sender) {
    if (option->adapt_threshold) {
        //m_Calculator1.do_hpfilt();
        first_snd = true;
        updateSegmentation = true;
        derivTBChange(this);
    }
}
// ---------------------------------------------------------------------------

void TForm1::adaptThresholdCBClick_sf2(TObject *Sender) {
    if (option->adapt_threshold2) {
        //m_Calculator2.do_hpfilt();
        first_snd = false;
       updateSegmentation2 = true;
       derivTBChange(this);
    }
}
// ---------------------------------------------------------------------------

void TForm1::hp_smallChange_sf(TObject *Sender) {
    if (display->ActivePage == Song1) {
        // DataForm->Settings(1, "hp_small", Frame11->hp_small_num->Value);
        //m_Calculator1.do_hpfilt(); // recalc_light(0);
        /*if (!option->paginationOn) {
            m_Calculator1.GetImage().DrawAllSlices(Image1);
        }
        else {
            m_Calculator1.GetImage().DrawSlices(0, 1000 - 1, Image1);
        }*/
        first_snd = true;
		updateSegmentation = true;
		derivTBChange(this);
        //set_scale(0);
    }

    else if (display->ActivePage == Song2) {
		// DataForm->Settings(2, "hp_small", Frame12->hp_small_num->Value);
        /*m_Calculator2.do_hpfilt(); // recalc_light(0);
        if (!option->paginationOn2) {
            m_Calculator2.GetImage().DrawAllSlices(Image2);
        }
        else {
            m_Calculator2.GetImage().DrawSlices(0, 1000 - 1, Image2);
        }*/
        updateSegmentation2 = true;
        first_snd = false;
        derivTBChange(this);
       // set_scale(1);
    }

}
// ---------------------------------------------------------------------------

void TForm1::hp_bigChange_sf(TObject *Sender) {
    float x;

    if (display->ActivePage == Song1) {
        /*if (!option->paginationOn) {
            m_Calculator1.GetImage().DrawAllSlices(Image1);
        }
        else {
            m_Calculator1.GetImage().DrawSlices(0, 1000 - 1, Image1);
        }
        m_Calculator1.do_hpfilt(); // recalc_light(0);*/
        updateSegmentation = true;
        first_snd = true;
        derivTBChange(this);
        //set_scale(0);
    }
    else if (display->ActivePage == Song2) {
       /* if (!option->paginationOn2) {
			m_Calculator2.GetImage().DrawAllSlices(Image2);
        }
        else {
            m_Calculator2.GetImage().DrawSlices(0, 1000 - 1, Image2);
        }
		m_Calculator2.do_hpfilt(); // recalc_light(0);    */
        updateSegmentation2 = true;
        first_snd = false; // true;
        derivTBChange(this);
		//set_scale(1);
    }

}
// ---------------------------------------------------------------------------

void __fastcall TForm1::entropy_valBeforeDisplay(TObject *Sender,
	UnicodeString &Text)

{
	if (entropy_val->Value > 999)
		entropy_val->Precision = 0;
	else
		entropy_val->Precision = 1;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::deriv_valBeforeDisplay(TObject *Sender,
	UnicodeString &Text)

{
	if (deriv_val->Value > 999)
		deriv_val->Precision = 0;
	else
		deriv_val->Precision = 1;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::preview_cursorMouseDown(TObject *Sender,
	TMouseButton Button, TShiftState Shift, int X, int Y) {
	if(display->ActivePage==Song1) preview_cursor->Left = preview_cursor->Left + X;
	else preview_cursor2->Left = preview_cursor2->Left + X;
}
// ---------------------------------------------------------------------------

void __fastcall TForm1::preview_cursorMouseUp(TObject *Sender,
    TMouseButton Button, TShiftState Shift, int X, int Y) {
	TMouseButton Btn;
    TShiftState Sh;
	preview_sonoMouseUp(this, Btn, Sh, preview_cursor->Left - 35, Y);

}
// ---------------------------------------------------------------------------






void __fastcall TForm1::delete_row_statsClick(TObject *Sender)
{
  bool fnd = false;
	if(display->ActivePage==Song1)
	{
		for (int i = 1; i <= feature_statistics_View->NrOfRows; i++)
				if (feature_statistics_View->RowSelected[i]) {
					feature_statistics_View->RemoveRow(i);
					feature_statistics_cursor--;
					fnd = true;
				}
		}else{
			for (int i = 1; i <= feature_statistics_View2->NrOfRows; i++)
				if (feature_statistics_View2->RowSelected[i]) {
					feature_statistics_View2->RemoveRow(i);
					feature_statistics2_cursor--;
					fnd = true;
				 }
		}
	if (!fnd)
		MessageDlg(
		"No row selected. Please click on a row to highlight it and try again",
		mtError, TMsgDlgButtons() << mbOK, 0);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::update_multisegmentClick(TObject *Sender)
{
  Update_Display(0);
  addBtn_alt->Enabled=true;
  update_multisegment->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::update_segment2Click(TObject *Sender)
{
  Update_Display(1);
  Addrec2_alt->Enabled=true;
  update_segment2->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::copy12Click(TObject *Sender)
{
  similarity_view->CopyToClipBoard(1, 1, 15, similarity_view->NrOfRows, 1, 1);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::cut12Click(TObject *Sender)
{
   copy12Click(this);
   delete_similarityClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::delete_sim_rowClick(TObject *Sender)
{
	bool fnd=false;
	for (int i = 1; i <= similarity_view->NrOfRows; i++)
		if (similarity_view->RowSelected[i]) {
			similarity_view->RemoveRow(i);
			similarity_cursor--;
			fnd = true;
		}
	if (!fnd) MessageDlg("No row selected. Please click on a row to highlight it and try again", mtError, TMsgDlgButtons() << mbOK, 0);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::delete_similarityClick(TObject *Sender)
{
	similarity_view->Clear();
	similarity_view->NrOfRows = 1;
	similarity_cursor = 1;
	similarity_view->Header[1] = "Sound 1";
	similarity_view->Header[2] = "Sound 2";
	similarity_view->Header[3] = "%Similarity";
	similarity_view->Header[4] = "Accuracy";
	similarity_view->Header[5] = "%Sequential";
	similarity_view->Header[6] = "Pitch diff";
	similarity_view->Header[7] = "FM diff";
	similarity_view->Header[8] = "Entropy diff";
	similarity_view->Header[9] = "Goodness diff";
	similarity_view->Header[10] = "AM diff";
	similarity_view->Header[11] = "From(1)";
	similarity_view->Header[12] = "To(1)";
	similarity_view->Header[13] = "From(2)";
	similarity_view->Header[14] = "To(2)";
	similarity_view->Header[15] = "Comments";

}
//---------------------------------------------------------------------------

void __fastcall TForm1::copy_comparisonClick(TObject *Sender)
{
  comparison_view->CopyToClipBoard(1, 1, 15, similarity_view->NrOfRows, 1, 1);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::delete_comparisonClick(TObject *Sender)
{
	comparison_view->Clear();
	comparison_view->NrOfRows=1;
	comparison_cursor = 1;
	comparison_view->Header[1] = "Sound 1";
	comparison_view->Header[2] = "Sound 2";
	comparison_view->Header[3] = "Distribution score";
	comparison_view->Header[4] = "KS all";
	comparison_view->Header[5] = "KS Amplitude";
	comparison_view->Header[6] = "KS pitch";
	comparison_view->Header[7] = "KS FM";
	comparison_view->Header[8] = "KS entropy";
	comparison_view->Header[9] = "KS goodness";
	comparison_view->Header[10] = "KS mean Fr.";
	comparison_view->Header[11] = "From(1)";
	comparison_view->Header[12] = "To(1)";
	comparison_view->Header[13] = "From(2)";
	comparison_view->Header[14] = "To(2)";
	comparison_view->Header[15] = "Comments";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::cut_comparisonClick(TObject *Sender)
{
   copy_comparisonClick(this);
   delete_comparisonClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::delete_row_comparisonClick(TObject *Sender)
{
bool fnd=false;
	for (int i = 1; i <= comparison_view->NrOfRows; i++)
		if (comparison_view->RowSelected[i]) {
			comparison_view->RemoveRow(i);
			comparison_cursor--;
			fnd = true;
		}
	if (!fnd) MessageDlg("No row selected. Please click on a row to highlight it and try again", mtError, TMsgDlgButtons() << mbOK, 0);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::batch_copyClick(TObject *Sender)
{
  similarity_batch_view->CopyToClipBoard(1, 1, 15, similarity_batch_view->NrOfRows, 1, 1);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::batch_cutClick(TObject *Sender)
{
   batch_copyClick(this);
   batch_deleteClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::batch_deleterowClick(TObject *Sender)
{
bool fnd=false;
	for (int i = 1; i <= similarity_batch_view->NrOfRows; i++)
		if (similarity_batch_view->RowSelected[i]) {
			similarity_batch_view->RemoveRow(i);
			similarity_batch_cursor--;
			fnd = true;
		}
	if (!fnd) MessageDlg("No row selected. Please click on a row to highlight it and try again", mtError, TMsgDlgButtons() << mbOK, 0);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::batch_deleteClick(TObject *Sender)
{
	similarity_batch_view->Clear();
	similarity_batch_view->NrOfRows=1;
	similarity_batch_cursor = 1;
	similarity_batch_view->Header[1] = "Sound 1";
	similarity_batch_view->Header[2] = "Sound 2";
	similarity_batch_view->Header[3] = "%Similarity";
	similarity_batch_view->Header[4] = "Accuracy";
	similarity_batch_view->Header[5] = "%Sequential";
	similarity_batch_view->Header[6] = "Pitch diff";
	similarity_batch_view->Header[7] = "FM diff";
	similarity_batch_view->Header[8] = "Entropy diff";
	similarity_batch_view->Header[9] = "Goodness diff";
	similarity_batch_view->Header[10] = "AM diff";
	similarity_batch_view->Header[11] = "From(1)";
	similarity_batch_view->Header[12] = "To(1)";
	similarity_batch_view->Header[13] = "From(2)";
	similarity_batch_view->Header[14] = "To(2)";
	similarity_batch_view->Header[15] = "Comments";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::boostClick(TObject *Sender)
{
  option->boost_amp->Checked=boost->Checked;
  option->boost_ampClick(this);
  Screen->Cursor = crHourGlass;
  update_displayClick(this);
  Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------






void __fastcall TForm1::Goodnessofpitch_mClick(TObject *Sender)
{
	PitchGoodnessB->Down = true;
	PitchGoodnessBClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Frequencymodulation_mClick(TObject *Sender)
{
    FMB->Down = true;
	FMBClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Amplitudemodulation_mClick(TObject *Sender)
{
   AMB->Down = true;
	AMBClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Wienerentropy_mClick(TObject *Sender)
{
principal_frB->Down = true;
	principal_frBClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Durationofstate_mClick(TObject *Sender)
{
	principal_frB->Down = true;
	principal_frBClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Continuityovertime_mClick(TObject *Sender)
{
principal_frB->Down = true;
	principal_frBClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Continuityoverspace_mClick(TObject *Sender)
{
  principal_frB->Down = true;
	principal_frBClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
  //numSamples =
 // int numSamples = wave.GetTotalSamples();
  for(int i=0;i<numSamples;i++) if(!(i%20))
  {
   str=m_Calculator1.m_alt_pitch_out[i];
   Memo1->Lines->Add(str);
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::minfr_tbChange(TObject *Sender)
{
  minfr->Value=minfr_tb->Position;
  option->minFreq=minfr_tb->Position;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::cepst_pitchClick(TObject *Sender)
{
  if(cepst_pitch->ItemIndex){
	PitchB->Caption="Fund. freq.";
	option->cepstrum_pitch=false;
	option->pitch_method->ItemIndex=3;
	}
  else {
	PitchB->Caption="Pitch";
	option->cepstrum_pitch=true;
	option->pitch_method->ItemIndex=2;
	}
  if(pagination->Visible) update_display->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::cepstrum_pitch2Click(TObject *Sender)
{
   if(cepst_pitch->ItemIndex){ Pitch2B->Caption="Fund. freq."; option->cepstrum_pitch=false;}
  else { Pitch2B->Caption="Pitch"; option->cepstrum_pitch=true;}
  if(pagination2->Visible) update_display2->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LinkLabel1LinkClick(TObject *Sender, const UnicodeString Link,
          TSysLinkType LinkType)
{
  AnsiString link=Link;
  ShellExecute(0, "open", link.c_str(), 0, 0, 1);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Savethisimage1Click(TObject *Sender)
{
  if(display->TabIndex==0)Active = ksong1;
  else Active = ksong2;
  SaveSonogram();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CalcModeAltClick(TObject *Sender)
{
  CalcMode->ItemIndex = CalcModeAlt->ItemIndex;
  CalcMode2->ItemIndex = CalcModeAlt->ItemIndex;
  CalcModeAlt2->ItemIndex = CalcModeAlt->ItemIndex;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CalcModeAlt2Click(TObject *Sender)
{
  CalcMode->ItemIndex = CalcModeAlt2->ItemIndex;
  CalcMode2->ItemIndex = CalcModeAlt2->ItemIndex;
  CalcModeAlt->ItemIndex = CalcModeAlt2->ItemIndex;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::expandClick(TObject *Sender)
{
  if(score_panel->Width==445) score_panel->Width=1230;
  else score_panel->Width=445;
  if(expand->Caption==">>")expand->Caption="<<";
  else expand->Caption=">>";
}
//---------------------------------------------------------------------------



