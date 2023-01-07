// CZT_FeatureCalc.h: interface for the CZT_FeatureCalc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CZT_FEATURECALC_H__7A69A7DB_ABF6_43FF_BED8_F86A11D9603A__INCLUDED_)
#define AFX_CZT_FEATURECALC_H__7A69A7DB_ABF6_43FF_BED8_F86A11D9603A__INCLUDED_


#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <vector.h>
#include <float.h>
#include <DBTables.hpp>
#include "CWavIterator.h"
#include "records.h"
#include "CZT_FFT_Utils_Framework.h"
#include "CZT_FeatureImage.h"
#include "yinacf.h"

// Feature calc constants
//////////////////////////////////////////////////////////////////////
//#define   ztSamplesPerWindow       409
#define   ztFFTSIZE                1024
#define   ztFFT_CEPST              512
#define   rhythm_size              32768
//#define   ztFeatureAdvance         60

//#define   ztBufferRetract          ztSamplesPerWindow-ztFeatureAdvance   // do not change


struct Record{
        __int16 amplitude;//derivPw;
        __int16 am;
		__int16 peakFr;
		__int16 pitch;
        __int16 entropy;
        __int16 fm;
        __int16 pgood;
        __int16 mean_freq_amp;
        __int16 cepst0;
};

struct Features{
        //__int16 bird_ID;
        AnsiString bird_ID;
        double serial_number;
        __int8 month;
        __int8 day;
        __int8 hour;
        __int8 minute;
        __int8 second;
        int start;
        __int16 duration;
        __int16 min_pitch;
        __int16 min_principal_contour;
        __int16 min_FM;
        __int16 min_entropy;
        __int16 min_peakFr;
        __int16 min_slope;

        __int16 max_pitch;
        __int16 max_principal_contour;
        __int16 max_FM;
        __int16 max_entropy;
        __int16 max_peakFr;
        __int16 max_slope;

        __int16 max_amplitude;
        __int16 max_continuity_f;
        __int16 max_continuity_t;

        __int16 mean_pitch;
        __int16 mean_principal_contour;
        __int16 mean_FM;
        __int16 mean_entropy;
        __int16 mean_peakFr;
        __int16 mean_pGood;
        __int16 mean_amplitude;
        __int16 mean_am;
        __int16 mean_continuity_f;
        __int16 mean_continuity_t;
        __int16 mean_slope;

        __int16 var_pitch;
        __int16 var_FM;
        __int16 var_entropy;
        __int16 var_peakFr;
        __int16 var_pGood;
        __int16 var_amplitude;
        __int16 var_am;
   
        __int16 s1_amplitude;
        __int16 s1_entropy;
        __int16 s1_peak_frequency;
        __int16 s1_peak_power;
        __int16 s1_highpass_power;

        __int16 s2_amplitude;
        __int16 s2_entropy;
        __int16 s2_peak_frequency;
        __int16 s2_peak_power;
        __int16 s2_highpass_power;

        __int16 s3_amplitude;
        __int16 s3_entropy;
        __int16 s3_peak_frequency;
        __int16 s3_peak_power;
        __int16 s3_highpass_power;

        __int16 s4_amplitude;
        __int16 s4_entropy;
        __int16 s4_peak_frequency;
        __int16 s4_peak_power;
        __int16 s4_highpass_power;

        __int16 s5_amplitude;
        __int16 s5_entropy;
        __int16 s5_peak_frequency;
        __int16 s5_peak_power;
        __int16 s5_highpass_power;
};




struct Slave_Features{
        __int16 s_amplitude;
        __int8 s_peakFr;
        __int16 s_peakFr_power;
        __int8 s_entropy;
        __int16 s_highpass_power;
};




using namespace ztFFTAPI;
class CZT_MultiTaper
{
public:
     virtual ~CZT_MultiTaper();

     // Public util
     //////////////////////////////////////////////////////////////////////
     void Init(int nSize)
     {
         ReleaseTapers();
         m_nSize = nSize;
         m_pTaper1 = new float[nSize];
         m_pTaper2 = new float[nSize];
     };

     // Singleton Access
     //////////////////////////////////////////////////////////////////////
     static  CZT_MultiTaper& Instance();
     void ReleaseTapers();
     // Public data
     //////////////////////////////////////////////////////////////////////
     int            m_nSize;

     float*         m_pTaper1;
     float*         m_pTaper2;
     AnsiString str;


     private:
     // singleton constructor (private)
     CZT_MultiTaper();

};




class CZT_FeatureCalc
{
public:
	CZT_FeatureCalc();
	virtual ~CZT_FeatureCalc();
        float* Calculate_rhythm(int range, int feature, int filter);

	// Utils
	//////////////////////////////////////////////////////////////////////
     bool                     save_ms_table(int slaves);
     void                     parse_sound(int current_rec);
     void                     parse_bouts();
	 bool                     render_image;
	 void                     Render_sonogram_images();
     int                      Calculate(CWavIterator* waves, int slaves);//(CWavIterator& wave);
     int  Calculate_light(CWavIterator* waves, int slaves, int begin, int end);//(CWavIterator& wave);
	 bool                     do_binary(AnsiString fileName);
     void                     decode_sound(Record &rec);
    // bool                     sound_detect();
     bool					  sound_detect_feature(int currRec);
     int                      msTable_to_vars(int i);
     void                     ResetContents();
	 void                     save_to_database(int bird);
	 void                     Add_Record();
	 void                     Add_Brain_Record();
	 CZT_FeatureImage&        GetImage(){return m_Image;};
	 CZT_FeatureImage&        GetSonogram(){return m_sonogram;};
	 CZT_FeatureImage&        GetContours(){return m_contours;};
	 void                     RenderImage(BOOL bRender){m_bRenderImage = bRender;};
     int                      save_and_reset();
     void                     note_dur(float thresh, int missmatches);
     bool                     custom_segmentation(int feature, int thresh);
     bool                     test_it(float* data, int position, int data_size, int dur_test, int tolerance);
	 bool                     init_viewer(CWavIterator& wave);
     bool                     viewer(CWavIterator& wave, int dur, bool fit_all);

     PCMPOS pcmPos;
    // void                     slave_wave(CWavIterator& wave, int slave_num);
    // float** xxx(int move, CWavIterator** wv);//CWavIterator& wave1, CWavIterator& wave2, CWavIterator& wave3);
     // The image we build (see the header of the class for more info)
	 CZT_FeatureImage          m_Image, m_sonogram, m_contours;
     int maxBoutDur, boutDur, maxSyllableDur, stopDur, syllable_number, bout_end, total_frames, Slaves;
     AnsiString newFile, file_names[10], raw_feature_file, current_file_name, comments;

     // public features
     //float                    sampling_rate;
     int                       m_file_index;
	 int                       slices; // how many slices of data contained in the sound (for delete[] of vars
     float                     m_Entropy;
     float                     m_PitchGoodness;
     double                    m_Pitch;
     float                     m_FM;
     float                     m_AM;
     float                     m_amplitude;
     float                     m_mfa;
  //   float                     m_cepst0;
     float                     m_PeakFreq;
     float                     m_continuity_f;
     float                     m_continuity_t;
     float                     m_principal_contour;
     float                     m_FMc;
     float       m_s1_amplitude,  m_s2_amplitude,  m_s3_amplitude, m_s4_amplitude, m_s5_amplitude
                , m_s1_entropy, m_s2_entropy, m_s3_entropy, m_s4_entropy, m_s5_entropy
                , m_s1_peak_frequency, m_s2_peak_frequency, m_s3_peak_frequency, m_s4_peak_frequency, m_s5_peak_frequency
                , m_s1_peak_power, m_s2_peak_power, m_s3_peak_power, m_s4_peak_power, m_s5_peak_power
                , m_s1_highpass_power, m_s2_highpass_power, m_s3_highpass_power, m_s4_highpass_power, m_s5_highpass_power;


     float brightness;
     long position;
	 float *pitch, *fm, *am, *entropy, *pitchGoodness, *peakFreq, *amplitude, *mfa, *pitchS, *fmS    //peakFreq is mean frequency
           , *entropyS, *pitchGoodnessS, *AMS, *wave_data, *continuity_t, *continuity_f, *principal_frequency, *slope, *FMc;
	 double *hp_big, *hp_diff;
	 float *hp_small;
     bool *signal, *bout_signal;//, recs_exsist;
     float amp_chans[10][1000];
     Slave_Features** slave_chans;
	 float mean_chan_amp[2][11];
	 int   chan_index;
	 int index;
	 bool warning;
	 int nFileTotal; // total samples in wave file
	 float* wdata; // wave data array for testing
	 int unsigned last_ms_record;
	 vector<Features> mem_recs;
	 Features features;
	 Slave_Features slave_features;
	 void do_hpfilt();
	 float Maximum;
	 float* m_alt_pitch_out;
	 //CWavIterator *waves;
private:// protected:
	 ofstream myfile;
	// Protected Utils
	//////////////////////////////////////////////////////////////////////
	 void                     ReadTapers();
	 void                     RenderViewerSlice();
	 void                     ZeroFFTs(), ZeroRhythm();
	 void                     RenderImageSlice(int itr), CalculateDAS();
	 bool                     good_sound();
	 void add_features_to_vars();
	 Slave_Features Calculate_amp(int mode, CWavIterator& wave, PCMPOS pcmPos, int start_at, int interval);
	 //float Calculate_rectified_amp(CWavIterator& wave, PCMPOS pcmPos, int advance, int retract, int start_at, int interval);
	 void Calculate_continuity(int time, int freq);

		//yinacf
	YinACF<float>yin;
	void initYin (float sampleRate, float minFreq);
	int getFundamentalFrequency(int n, float* inSamples, float* outFrequencies);

	 float*                    m_alt_pitch_in;

     void handle_wave_files(int bird);

     AnsiString set_destination(int index);
     AnsiString set_file_name(int index, AnsiString old_file_name);
     void move_the_wave(AnsiString old_name,AnsiString new_name);
     void Add_syll_to_features_array();
     void reset_syllable_vars();
	 int hp_filter(float *data, int nobs,double lambda, double *output, BOOL ret);
	 int hp_filter(float *data, int nobs,double lambda, float *output, BOOL ret);
    // void segment_sound(int recnum);
	// Data
	//////////////////////////////////////////////////////////////////////
     float*                    m_Taper1;
     float*                    m_Taper2;
     int                       m_nTaperSize;
     CFFT_R2C                  m_fft1;
     CFFT_R2C                  m_fft2;
     CFFT_R2C                  m_cepst;
     CFFT_R2C                  m_rhythm;
     BOOL                      m_bRenderImage;
	 bool                      sound_detected;
	 bool 					   updatedArray;
     time_t first, second;
     double syllDur;
     int recnum;
     // Feature Data
     //////////////////////////////////////////////////////////////////////
     float                     m_freq_deriv[512];// frequency derivatives
     float                     m_time_deriv[512];// time derivatives
     float                     m_spec_deriv[512];// spectral derivatives
     double                     m_powSpec[512];
     float                     m_rhythmSpec[5100];
     float                     m_freq_deriv_max; // max freq deriv
     float                     m_time_deriv_max; // max timer deriv
     float                     m_SumLog;
     float                     m_LogSum;
     float                     m_MaxPower;
     TMyTable *recTable;

     double noise_power, sum_pitch, sum_principal_contour, sum_pitch_simp, sum_entropy, sum_fm, sum_am, sum_peakFreq
     , sum_continuity_t,sum_continuity_f, sum_slope
         , sum_amplitude, SS_pitch, SS_entropy, SS_fm, SS_am, SS_peakFreq, SS_amplitude, sum_factors, tmp_sum

         , sum_s1_amp, sum_s2_amp, sum_s3_amp, sum_s4_amp, sum_s5_amp
         , sum_s1_ent, sum_s2_ent, sum_s3_ent, sum_s4_ent, sum_s5_ent
         , sum_s1_peak_frequency, sum_s2_peak_frequency, sum_s3_peak_frequency, sum_s4_peak_frequency, sum_s5_peak_frequency
         , sum_s1_peak_power, sum_s2_peak_power,sum_s3_peak_power, sum_s4_peak_power, sum_s5_peak_power
         , sum_s1_highpass_power, sum_s2_highpass_power, sum_s3_highpass_power, sum_s4_highpass_power, sum_s5_highpass_power

         ;
     double sum_pgood, SS_pgood, max_drv;
     float min_pitch, min_principal_contour, min_FM, min_entropy, min_peakFr, min_slope, max_pitch, max_principal_contour, max_FM, max_entropy
          , max_peakFr, max_slope, max_amplitude, max_continuity_f, max_continuity_t;
     int iterations;
     bool arrays_created;
     bool real_time;// do binary file mode indicator
     AnsiString str;
     void create_arrays(int size);
     void refine_segmentation();
     void feature_array_to_var(int i);
     bool **Borders;
	 int **borders;
     float **Deriv, **Sono;
	 bool slave_chan_exist;
	 bool deriv_created, continuity_created;
     //short *hp_big_s, *hp_small_s, *hp_diff_s;


};

#endif // !defined(AFX_CZT_FEATURECALC_H__7A69A7DB_ABF6_43FF_BED8_F86A11D9603A__INCLUDED_)
