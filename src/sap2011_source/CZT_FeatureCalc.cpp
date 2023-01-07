// CZT_FeatureCalc.cpp: implementation of the CZT_FeatureCalc class.
//                                   R
//////////////////////////////////////////////////////////////////////
#include "CZT_FeatureCalc.h"
#include <time.h>
#include <limits.h>
#include <math.h>
#include "options3.h"
#include <dir.h>
#include <Filectrl.hpp>
#include <fstream> // for ifstream, ofstream, ios_base
#include <iostream>// for cin, cout, endl
#include <algorithm>
#include "tapers.h"
#define DEFAULT_MAX 999999
#define Contour_length 0
#define Contour_height 5
#define X_start 1
#define X_end 2
#define Y_start 3
#define Y_end 4
#define average 1
#define rectified 2
#define squered 3


using std::ios_base;
using std::cout;
using std::cin;
using std::endl;


 struct Contour
  {
     int time;
     short freq;
     int contour_num;
	 Contour( int ti, short fr, int num) : time(ti), freq(fr), contour_num(num) {};
	 Contour() : time(0), freq(0), contour_num(0){};
  };
  bool operator<(const Contour &x, const Contour &y)
  { return x.contour_num < y.contour_num; }

//////////////////////////////////////////////////////////////////////
// CZT_MultiTaper class
//////////////////////////////////////////////////////////////////////

CZT_MultiTaper::CZT_MultiTaper()
{
	m_nSize=0;
	m_pTaper1=NULL;
	m_pTaper2=NULL;

}



CZT_MultiTaper::~CZT_MultiTaper()
{
	 ReleaseTapers();

}


void CZT_MultiTaper::ReleaseTapers()
{
  if(m_pTaper1) delete [] m_pTaper1;
  if(m_pTaper2) delete [] m_pTaper2;
  m_nSize=0;
  m_pTaper1=NULL;
  m_pTaper2=NULL;
}


CZT_MultiTaper& CZT_MultiTaper::Instance()
{
  static CZT_MultiTaper mgr;
  return mgr;
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


// The caller is expected to call initYin() during program
// initialization, or whenever sample rate of minimum frequency
// requirements change.
//
// this implementation uses tmax=1/minFreq seconds

void CZT_FeatureCalc::initYin (float sampleRate, float minFreq) {

	unsigned w, tmax;
	w = (unsigned)ceil(sampleRate/minFreq);
	tmax = w;
	yin.build(w, tmax);
}


// extract frequency estimates from the signal in inSamples, and save
// in outFrequencies

int CZT_FeatureCalc::getFundamentalFrequency(int n, float* inSamples, float* outFrequencies)
{
    int i;
    for (i = 0; i < n; ++i)
		try{outFrequencies[i] = yin.tick(inSamples[i]);
		}catch(Exception & exception) {
		  if(i>0)outFrequencies[i] =outFrequencies[i-1];
		  else outFrequencies[i] =0;
		}
        return 0;
}




CZT_FeatureCalc::CZT_FeatureCalc()
{
	 m_fft1.Initialize(ztFFTSIZE);
	 m_fft2.Initialize(ztFFTSIZE);
	 m_cepst.Initialize(ztFFT_CEPST);
	 m_rhythm.Initialize(rhythm_size);
	 m_bRenderImage = FALSE;
	 mem_recs.reserve(100);
	 iterations=0;
	 deriv_created=false;
	 continuity_created=false;
	 arrays_created=false;
	 real_time=true; // default -- do it the simple way...
	 wave_data=new float[option->ztSamplesPerWindow];
	 for(int i=0;i<2;i++)for(int j=0;j<11;j++) mean_chan_amp[i][j]=0;
	 chan_index=1;
	 slave_chan_exist=false;
	 last_ms_record=0;
	 comments="";

 /*

  myfile.open ("c:\\sap\\log_featurecalc.txt");
  if (!myfile) {
	return ;
  } */

}


CZT_FeatureCalc::~CZT_FeatureCalc()
{

// clean up:
   if(deriv_created)
   {
	   for(int i=0;i<slices+2;i++) delete[] Deriv[i];
	   delete[] Deriv;
	   for(int i=0;i<slices+2;i++) delete[] Sono[i];
	   delete[] Sono;
	   deriv_created=false;
   }

   if(continuity_created)
  {
	  for (int i=0; i<recnum; i++)
	  {
			delete[] Borders[i];
			delete[] borders[i];
	  }
	  delete[] Borders;
	  delete[] borders;
	  continuity_created=false;
  }
   mem_recs.erase(mem_recs.begin(), mem_recs.end());
   delete[] wave_data;
   ResetContents();

}




void CZT_FeatureCalc::ResetContents()
{
	 ZeroFFTs();
	 ZeroRhythm();

	 m_Image.ResetContents();
	 m_sonogram.ResetContents();
	 m_contours.ResetContents();

	if(deriv_created)
   {
	   for(int i=0;i<slices+2;i++) delete[] Deriv[i];
	   delete[] Deriv;
	   for(int i=0;i<slices+2;i++) delete[] Sono[i];
	   delete[] Sono;
	   deriv_created=false;
   }

   if(continuity_created)
  {
	  for (int i=0; i<recnum; i++)
	  {
			delete[] Borders[i];
			delete[] borders[i];
	  }
	  delete[] Borders;
	  delete[] borders;
	  continuity_created=false;
  }

   if(arrays_created)
   {
	arrays_created=false;
	delete[] pitch;
	delete[] principal_frequency;
	delete[] fm;
	delete[] am;
	delete[] entropy;
	delete[] pitchGoodness;
	delete[] peakFreq;
	delete[] amplitude;
	delete[] mfa;
   //	delete[] cepst0;
	delete[] pitchS;
	delete[] fmS;
	delete[] entropyS;
	delete[] pitchGoodnessS;
	delete[] AMS;
	delete[] signal;
	delete[] bout_signal;
	delete[] hp_small;
	delete[] hp_big;
	delete[] hp_diff;
	
	if(option->calculate_continuity)
	{
		delete[] continuity_t;
		delete[] continuity_f;
	   //	delete[] principal_frequency;
		delete[] slope;
		delete[] FMc;
	}
	if(Slaves && slave_chan_exist)
	 {
		 slave_chan_exist=false;
		 for(int i=0;i<slices;i++)
				delete[] slave_chans[i];
		 delete[] slave_chans;
	 }
   // delete[] CZT_FeatImgSlice;

   }
   iterations=0;
}




inline void CZT_FeatureCalc::ZeroFFTs()
{
	 memset(m_fft1.m_pInput,0,sizeof(float)*m_fft1.m_nIn);  // zero out fft input....
	 // m_fft2 gets a copy of fft1 so no need to mess with it
}

inline void CZT_FeatureCalc::ZeroRhythm()
{
	 memset(m_rhythm.m_pInput,0,sizeof(float)*m_rhythm.m_nIn);  // zero out fft input....

}

//////////////////////////////////////////////////////////////////////
// Calculate the features and send them to the parssing function
//////////////////////////////////////////////////////////////////////


Slave_Features CZT_FeatureCalc::Calculate_amp(int mode, CWavIterator& wave, PCMPOS pcmPos, int start_at, int interval)
{
  float x, s_amplitude=0;
 // static int rep=0;
  wave.ReadSamples(pcmPos, wave_data, option->ztSamplesPerWindow);

 // first calculate the amplitude of the wave form (average, rectified or squered)
 switch(mode)
 {
    case average:
    for(int i=1;i<interval;i++) s_amplitude+=wave_data[i+start_at];
    break;

    case rectified:
    for(int i=1;i<interval;i++) s_amplitude+=fabs(wave_data[i+start_at]);
    break;

    case squered:
    for(int i=1;i<interval;i++) s_amplitude+=(wave_data[i+start_at]*wave_data[i+start_at]);
    s_amplitude=10*sqrt(s_amplitude);   ///=100; // avoid overflaw of large sq. numbers
    break;
 }
 s_amplitude/=(float)interval; // now compute spectral features (high pass, according to user choices)
 m_fft1.m_pInput[0]=0;
 for(int i=1; i<option->ztSamplesPerWindow; i++)
        m_fft1.m_pInput[i]= (wave_data[i]-wave_data[i-1]) * m_Taper1[i];
 for(int i=option->ztSamplesPerWindow;i<1024; i++) m_fft1.m_pInput[i]=0;

 m_fft1.Run();

 float s_SumLog, s_LogSum, s_peakFr, s_Entropy, s_peakFrPower, s_highpass_power;
 s_SumLog=s_LogSum=s_peakFr=s_Entropy=s_peakFrPower=0;
 for(int i=option->s_min_fr_tb->Position;i<option->s_max_fr_tb->Position;i++) // 12-181 freq range *43 == 500-8000Hz
 {
   m_powSpec[i]=(m_fft1.m_pOutput[i][ztReal]*m_fft1.m_pOutput[i][ztReal])+(m_fft1.m_pOutput[i][ztImaginary]*m_fft1.m_pOutput[i][ztImaginary]);
   if(s_peakFrPower<m_powSpec[i]) { s_peakFr=i; s_peakFrPower=m_powSpec[i];}
   if(m_powSpec[i])s_SumLog+=log(m_powSpec[i]);
   s_LogSum+=m_powSpec[i];
 }
 int dv=option->s_max_fr_tb->Position-option->s_min_fr_tb->Position;
 s_highpass_power=s_LogSum/dv; // note that s_Logsum is sum, not log yet!
 if(s_LogSum)
 {
    s_LogSum=log(s_LogSum/dv);
    s_Entropy=(s_SumLog/dv)-s_LogSum;
 }
 Slave_Features sf;
 sf.s_amplitude=s_amplitude/10;
 if(s_highpass_power<1)s_highpass_power=1;
 sf.s_highpass_power=1000*log(s_highpass_power);
 sf.s_entropy=s_Entropy*10;
 sf.s_peakFr=s_peakFr-100;
 if(s_peakFrPower<1)s_peakFrPower=1;
 sf.s_peakFr_power=1000*log(s_peakFrPower);
 return(sf);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////


void CZT_FeatureCalc::ReadTapers()
{
     int samples=option->ztSamplesPerWindow;
     if(option->tapers_done) CZT_MultiTaper::Instance().ReleaseTapers();
     CZT_MultiTaper::Instance().Init(samples);
     float* pTape1 =CZT_MultiTaper::Instance().m_pTaper1;
     float* pTape2 =CZT_MultiTaper::Instance().m_pTaper2;
     float* TapersOut=new float[100+samples*4];
     proTapers(samples, 4, 1.5, TapersOut);
     
     if(TapersOut[2]<0) for(int i=0; i<samples; i++) TapersOut[i]*=-1;
     if(TapersOut[samples+2]<0) for(int i=0; i<samples; i++) TapersOut[samples+i]*=-1;
     if(TapersOut[2*samples+2]<0) for(int i=0; i<samples; i++) TapersOut[2*samples+i]*=-1;
     if(TapersOut[3*samples+2]<0) for(int i=0; i<samples; i++) TapersOut[3*samples+i]*=-1;
         /*  */
     for(int i=0;i<samples;i++)
     {
       pTape1[i]=TapersOut[i];
       pTape2[i]=TapersOut[i+samples];
     }
}




float* CZT_FeatureCalc::Calculate_rhythm(int range, int feature, int filter)
{
     int rhythm_range;
     float val, val2;
     int tmp=option->ztSamplesPerWindow;
     option->ztSamplesPerWindow=recnum; // lenght of vector
     ReadTapers();  // calculate the tapers filter
     m_Taper1=CZT_MultiTaper::Instance().m_pTaper1;  // this is a pointer to the tapers array

     switch(range)
     {
                case 0: rhythm_range=200; break;
                case 1: rhythm_range=500; break;
                case 2: rhythm_range=5000; break;
     }

     ZeroRhythm();

     for(int i=0; i<rhythm_size; i++)
     {
                if(i<recnum-1)
                {
                        switch(feature)
                        {
                                case 0: val=amplitude[i]; val2=amplitude[i+1]; break;
								case 1: val=pitch[i]; val2=pitch[i+1]; break;
                                case 2: val=pitchGoodness[i]; val2=pitchGoodness[i+1]; break;
                                case 3: val=peakFreq[i]; val2=peakFreq[i+1]; break;
                                case 4: val=principal_frequency[i]; val2=principal_frequency[i+1]; break;
                                case 5: val=fm[i]; val2=fm[i+1]; break;
                                case 6: val=100.0*am[i]; val2=100.0*am[i+1]; break;
                                case 7: val=10.0*entropy[i]; val2=10.0*entropy[i+1]; break;
                                case 8: val=continuity_f[i]; val2=continuity_f[i+1]; break;
                                case 9: val=continuity_t[i]; val2=continuity_t[i+1]; break;
                        }
						if(filter)m_rhythm.m_pInput[i]=m_Taper1[i]*(val2-val);//(10*amplitude[i+1])-(10*amplitude[i]);
                        else m_rhythm.m_pInput[i]=m_Taper1[i]*val;
                }

                else m_rhythm.m_pInput[i]=0;
        }
        
        val=0;
        if(!filter)
        {
                for(int i=0;i<recnum; i++) val+=m_rhythm.m_pInput[i];
                val/=recnum; // mean val
                for(int i=0;i<recnum; i++)
                {
                  m_rhythm.m_pInput[i]-=val;
                }
        }
        try{ m_rhythm.Run(); }
         catch ( ... ) { return(0); }
       //float x=m_rhythm.m_pOutput[i][ztReal] * m_rhythm.m_pOutput[i][ztReal] +  m_rhythm.m_pOutput[i][ztImaginary] * m_rhythm.m_pOutput[i][ztImaginary];
       for(int i=0;i<rhythm_range;i++) //  option->spectrum_range ... 256 out of the whole 512, which is up to 11kHz
       {
               m_rhythmSpec[i] = m_rhythm.m_pOutput[i][ztReal] * m_rhythm.m_pOutput[i][ztReal] +  m_rhythm.m_pOutput[i][ztImaginary] * m_rhythm.m_pOutput[i][ztImaginary];
               // find peak frequency
               if(m_MaxPower<m_powSpec[i])
               {
                    m_MaxPower=m_powSpec[i];
                    m_PeakFreq=i;
               }

               // calculate Wiener entropy, amplitude and mean frequency upon this frequency range
               if(i>=option->min_entropy_freq && i<option->max_entropy_freq && m_powSpec[i])
               {
				 m_amplitude+=m_powSpec[i];
                 m_SumLog+=log(m_powSpec[i]+1);//log(m_powSpec[i]);
                 m_LogSum+=m_powSpec[i];
               }

       }  // end for all frequencies in a frame.
		m_amplitude=log10(m_amplitude+1)*10-option->baseline;   // get the units to Db
        //rec.amplitude=m_amplitude*10;
        // compute Wiener entropy
        if(m_LogSum)
         {
		   // m_LogSum=log(m_LogSum/entropy_range);
		  //  m_Entropy=(m_SumLog/entropy_range)-m_LogSum;
		 }
		 //else  m_Entropy=0;// rec.power=0;
		// RenderImageSlice(itr);
	 option->ztSamplesPerWindow=tmp;
	 ReadTapers();
	 return m_rhythmSpec;

}




// This is the the leading function of featurecalc. The input arguments are an array of wave fiels
// and the number of slave channels. The first channel, namely, wave[0] is assumed to be a sound file
// and for this channel, which is obligatory, all calculations are performed on the spectral domain
// including MT FFT and calcualtion of all song features. For the slaves channels, this function onlyh
// send data to other functions, e.g., calc_Rect_amp, which are doing very simple calcuations on the waveform.
// such as computing mean rectified amplitude. All the data are written across channels to a text file,
// which is then copied into a mySQL table. For rexample,. the table 1ms table should include all the
// song features per one ms: namelly, amp, pitch, FM... and for the other slave files, each one will
// get a singel field in the table, say for rect amp or amp.
// To keep things in synchrony, everything is calculated at once: all the wave files bave pointers that
// progresses using the same steps. Only for the slaves, which are processed by external functions, the
// pointer has to be moved forward explicitly.
// From the pointer, the functions are calculating features upon a window, write a line to the ext file
// and then we move the pointers and do the next window, until the end of the wave file.
// all files are assumed to be exactly of the same lenght. In partucular, shorter salve files might
// result in an access violation error.

int CZT_FeatureCalc::Calculate(CWavIterator* waves, int slaves)//& wave)//CWavIterator& wave)
{
	 Maximum=0;
	 option->problem_file=0;
	 int amp_index=0;
	 int save_mem=0, slice_num=0;
	 Slaves=slaves;
	 index=0; // for the slave channel 1 -- temporary...
	 ResetContents();  // Clean up any garbage from a previous run...
	 int data_samples=option->ztSamplesPerWindow;
	 syllable_number=0;  Record rec;
	 float entropy_range=option->max_entropy_freq-option->min_entropy_freq;
	 Slaves=slaves;
	 int start_at;
	 if(slaves) start_at=data_samples/2-22; // this will give us a ms start position, starting in the middle of the wave epoch
	 option->syllCounter=0;
	 // init our tapers from the singleton
	 //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
	 m_Taper1=CZT_MultiTaper::Instance().m_pTaper1;
	 //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
	 m_Taper2=CZT_MultiTaper::Instance().m_pTaper2;
	 //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
	 m_nTaperSize==CZT_MultiTaper::Instance().m_nSize;
	 //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
	 // m_nTaperSize, Taper Size = number of samples to read (with overlap)
	 // Data
	 int i, nTotal=0;
	 float fReal1,fReal2,fImag1,fImag2;
	 float fTmp, gravity_center, gc_base, log_power;
	 double cFM,sFM, min_drv=0; //  max_power=0, min_power=0,
	 max_drv=0;
	 float x,y;
	 float fTemp;
	//maxBoutDur=0; boutDur=0;
	maxSyllableDur=0; syllDur=0; stopDur=1000;

	// Get Starting positiong

   // waves_to_starting_position(&wave); // this function will getheadposition() for all waves
	PCMPOS pcmPos[11];
	//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();

	for(int i=0;i<=slaves;i++)
		pcmPos[i]=waves[i].GetHeadPosition();

	//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
	option->sampling_rate=waves[0].m_WaveHeader.nSamplesPerSec;

	//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
	option->frequency_scale=(option->sampling_rate/2)/512;

	//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
	// if focus on
	if(option->range_x05) waves[0].ReadSamples(pcmPos[0], m_fft1.m_pInput, 2*option->ztSamplesPerWindow);
	else waves[0].ReadSamples(pcmPos[0], m_fft1.m_pInput, option->ztSamplesPerWindow);

	//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();

	int nFileTotal  =    waves[0].GetTotalSamples();
	recnum=0;

	//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();

	int advance=option->Advance, retract;//ztFeatureAdvance;  60

	//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();

	if(option->doubleSamples) advance=option->Advance/2; // if sample rate is 22050Hz

	//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();

	slices=nFileTotal/advance;

	//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();

	if(!slices)return(false); // wave file contains no data

	//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();

	if(option->create_feature_arrays) create_arrays(slices);

	if(!option->cepstrum_pitch)
	{

	  // new fundamental frequency pitch--------------------------------------------------
	  //int numSamples =  waves[0].GetTotalSamples();
	  int sampling_rate=waves[0].m_WaveHeader.nSamplesPerSec;
	  long startPos = pcmPos[0];
	  initYin(sampling_rate,option->minFreq);//option->sampling_rate,  22);
	  m_alt_pitch_in = new float[nFileTotal];
	  m_alt_pitch_out = new float[nFileTotal];
//	  m_alt_pitch_in = (float*) malloc (sizeof(float)*nFileTotal);
//	  m_alt_pitch_out = (float*) malloc (sizeof(float)*nFileTotal);
	  waves[0].ReadSamples(startPos,m_alt_pitch_in, nFileTotal);
	  getFundamentalFrequency(nFileTotal, m_alt_pitch_in, m_alt_pitch_out);
	  // that's it, now move to pitch array
	  int div=nFileTotal/slices;
	  // constrain the pitch range
	  int maxpitch;
	  if(option->range_x2 == false && option->range_x05 == false) maxpitch=sampling_rate/4;
	  else if(option->range_x2 == false && option->range_x05 == true) maxpitch=sampling_rate/8;
	  else maxpitch=sampling_rate/2;
	  // save pitch to array:
	  float pp;
	  for(int i=0;i<slices;i++){
		   pp=(float)sampling_rate * m_alt_pitch_out[div*i];
		   if(pp<10 || pp>maxpitch)pitch[i]=0;
		   else pitch[i]=pp;
	   }
			// free the memory
		   delete[] m_alt_pitch_in;
		   delete[] m_alt_pitch_out;
	  //-----------------------------------------------------------------------------------
	}


	//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();

	if(option->range_x05)retract=2*data_samples-advance;
	else retract=data_samples-advance;

	//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
	m_Image.signal_r=signal;
	m_Image.bout_r=bout_signal;
  /*
	m_Image.hp_small_r=hp_small;
	m_Image.hp_big_r=hp_big;
	m_Image.hp_diff_r=hp_diff;
  */

	m_sonogram.signal_r=signal;
	m_sonogram.bout_r=bout_signal;
  /*	m_sonogram.hp_small_r=hp_small;
	m_sonogram.hp_big_r=hp_big;
	m_sonogram.hp_diff_r=hp_diff;
  */
	m_contours.signal_r=signal;
	m_contours.bout_r=bout_signal;
  /*	m_contours.hp_small_r=hp_small;
	m_contours.hp_big_r=hp_big;
	m_contours.hp_diff_r=hp_diff;
   */
	//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();

	if(slaves)
	{
	  //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
	  slave_chans=new Slave_Features*[slices];
	  //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
	  for(int i=0;i<slices;i++) slave_chans[i]=new Slave_Features[slaves+1];
	  //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
	  slave_chan_exist=true;
	  //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
	}

	//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
	reset_syllable_vars();
	//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
	int addit=0;
	// for continuity we declare an array for the derivatives
	deriv_created=true;
	Deriv= new float*[slices+2];

	for(int i=0;i<slices+2;i++)Deriv[i]=new float[256];
	//Deriv= new float*[slices];
	//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
	//for(int i=0;i<slices;i++)Deriv[i]=new float[256];
	//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
	Sono= new float*[slices+2];

	for(int i=0;i<slices+2;i++)Sono[i]=new float[256];
	//Sono= new float*[slices];
	//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
	//for(int i=0;i<slices;i++)Sono[i]=new float[256];
	//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
	//debug//myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; myfile.flush();

	while(nTotal<nFileTotal-(option->ztSamplesPerWindow+option->Advance))
	{
		 //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
		 if(option->slow_execution && !(recnum%100)) Sleep(option->slow_execution);
		 nTotal += advance;//ztFeatureAdvance; // Advance ztFeatureAdvance samples at a time
		 recnum++;
		 // Copy input data to fft2...
		 //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
		 memcpy(m_fft2.m_pInput,m_fft1.m_pInput,sizeof(float)*m_fft1.m_nIn);
		//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
		if(option->doubleSamples) // Interpulate to half the frequency range
		 {
			  for(i=data_samples-1; i>=0; i--) m_fft1.m_pInput[i]= m_fft1.m_pInput[i/2];
			  for(i=1;i<data_samples-2;i++) if(i%2)m_fft1.m_pInput[i]= m_fft1.m_pInput[i-1]/2+m_fft1.m_pInput[i+1]/2;
			  for(i=0;i<data_samples;i++)  m_fft2.m_pInput[i]= m_fft1.m_pInput[i];
		 }

		 if(option->range_x2) // use every other sample to double the frequency range
		 {
			//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
			for(i=data_samples-1; i>=0; i--) m_fft1.m_pInput[i]= m_fft1.m_pInput[i/2];
			for(i=1;i<data_samples-2;i++) if(i%2)m_fft1.m_pInput[i]= m_fft1.m_pInput[i-1]/2+m_fft1.m_pInput[i+1]/2;
			for(i=0;i<data_samples;i++)  m_fft2.m_pInput[i]= m_fft1.m_pInput[i];
		 }
		 // do taper multiplies
		 for(i=0;i<data_samples;i++)
		 {
			//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
			 m_fft1.m_pInput[i] *=m_Taper1[i];
			//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
			 m_fft2.m_pInput[i] *=m_Taper2[i];
			//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
			   if(option->boost_amplitude)
			   {
				  //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
				  m_fft1.m_pInput[i] *=5;
				  //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
				  m_fft2.m_pInput[i] *=5;
				  //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
			   }
		 }
		 // Run em...
		 //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
		 try{
			 //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
			 m_fft1.Run();
			//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
			 }
		 catch ( ... ) { return(false); }

		 try{
			 //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
			 m_fft2.Run();
			 }
		 catch ( ... ) { return(false); }

		 // Tapers Are Done.
		 //////////////////////////////////////////////////////////////////////

		 // we now caclulate cepstrum for pitch and goodness of pitch estimates

		 // Zero these out..
		m_time_deriv_max=m_freq_deriv_max=m_amplitude=m_MaxPower=m_SumLog=m_LogSum=0;
		 gc_base=gravity_center=m_AM=noise_power=0;
		 for(i=0;i<option->spectrum_range;i++) //256 out of the whole 512, which is up to 11kHz
		 {
				//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
			   //fReal[x] = fImage[x] - references output of fft1
			   // real2,imag2 - reference fft2
			   fReal1 =m_fft1.m_pOutput[i][ztReal];
			   //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
			   fReal2 =m_fft2.m_pOutput[i][ztReal];
			   //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
			   fImag1 =m_fft1.m_pOutput[i][ztImaginary];
			   //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
			   fImag2 =m_fft2.m_pOutput[i][ztImaginary];
			   //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
			   // Calculate power spectrum..
			  // m_powSpec[i] = fReal1*fReal1+fImag1*fImag1;
				m_powSpec[i] = fReal1*fReal1+fReal2*fReal2+fImag1*fImag1+fImag2*fImag2;
			   //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
			   // m_power+=m_powSpec[i];
			   // calculate time derivatives
			   m_time_deriv[i]=-fReal1*fReal2-fImag1*fImag2;
			   //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
			   // calcumate frequency derivatives
			   m_freq_deriv[i]=fImag1*fReal2-fReal1*fImag2;
			   //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
			   Sono[recnum][i]=1+3000.0*log(0.1+(fReal1*fReal1)+(fImag1*fImag1));
			   //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
			   // calculate Wiener entropy, amplitude and mean frequency upon this frequency range
			   if(i>=option->min_entropy_freq && i<option->max_entropy_freq && m_powSpec[i])
			   {
				 //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
				 // find maximal values of time and frequency derivatives of a windows for FM.
				 fTmp=m_time_deriv[i]*m_time_deriv[i];
				 //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
				 if(fTmp>m_time_deriv_max) m_time_deriv_max=fTmp;
				 //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
				 fTmp=m_freq_deriv[i]*m_freq_deriv[i];
				 //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
				 if(fTmp>m_freq_deriv_max) m_freq_deriv_max=fTmp;
				 //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
				 // find peak frequency
				 if(m_MaxPower<m_powSpec[i])
				 {
				 //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
					m_MaxPower=m_powSpec[i];
					//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
					m_PeakFreq=i;
					//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
				 }

				 if(Maximum<m_powSpec[i])Maximum=m_powSpec[i];
				 //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
				  // calculate power in the noise-detector frequency range
				 if(   ( option->detect_low_noise && i<option->noise_thresh_freq)   // for low freq noise
				  //|| (!option->detect_low_noise && i>option->noise_thresh_freq)  // for high freq noise
				   )
				 {
				   noise_power+=m_powSpec[i];
				 }

				 m_amplitude+=m_powSpec[i];
				 log_power=m_freq_deriv[i]*m_freq_deriv[i]+m_time_deriv[i]*m_time_deriv[i];
				 m_SumLog+=log(m_powSpec[i]);//log(m_powSpec[i]);
				 gravity_center+=i*log_power;
				 gc_base+=log_power;
				 m_LogSum+=m_powSpec[i];
				 m_AM+=m_time_deriv[i];
				 }

		 }  // end for all frequencies in a frame.
		 principal_frequency[recnum]=m_PeakFreq;
		 gravity_center/=max(gc_base,(float)1); // for calculating mean frequency
		 if(!m_amplitude)m_amplitude=1;
		 m_AM/=m_amplitude;
		 if(m_AM)rec.am=m_AM*100;
		 noise_power/=max(m_amplitude,(float)1);
		 m_amplitude=log10(m_amplitude+1)*10-option->baseline;   // get the units to Db
		// int raw_amp=m_amplitude;
		 if(noise_power>option->noise_ratio)m_amplitude=0;  //  filter out the noisy sounds
		 rec.amplitude=m_amplitude*10;
		 int mean_fr_int=gravity_center;
		 double mfa=0;
		 for(i=-5;i<6;i++)
		 {
		   if(mean_fr_int>4 && mean_fr_int<200)
				mfa+=m_powSpec[mean_fr_int+i];   // we are not interested in frequecny below 4=150Hz
		 }
		 mfa=log10(mfa+1)*10-option->baseline;
		 rec.mean_freq_amp=mfa*10;
		 // we now compute the features of this fft window:
		 //////////////////////////////////////////////////////////////////////
		 // compute Wiener entropy
		 if(m_LogSum)
		 {
			m_LogSum=log(m_LogSum/entropy_range);
			m_Entropy=(m_SumLog/entropy_range)-m_LogSum;
		}
		 else  m_Entropy=0;// rec.power=0;


		 // compute FM
		 if(m_freq_deriv_max && (m_time_deriv_max != 0))// div by 0 caught
			   m_FM=atan(m_time_deriv_max/m_freq_deriv_max);// Possible div by 0? not anymore...
		 else
			   m_FM=0;

		 // Compute SPECTRAL DERIVATIVES
		 //////////////////////////////////////////////////////////////////////
		 //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
		cFM=cos(m_FM);
		//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
		sFM=sin(m_FM);
		//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
		// Calc pitch     // this 256 will give use sufficient range assuming 512 freq of 22050Hz
		memset(m_cepst.m_pInput,0,sizeof(float)*m_cepst.m_nIn);// zero out fft.
		//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();

		for(i=7;i<255;i++)
		{
		  m_spec_deriv[i]=( m_time_deriv[i]*sFM+m_freq_deriv[i]*cFM);
		  fTemp =m_powSpec[i];//*m_Taper1[i];
		  if(fTemp != 0 && i>=option->min_entropy_freq && i<option->max_entropy_freq) m_cepst.m_pInput[i]=m_spec_deriv[i]/fTemp;
		  else m_cepst.m_pInput[i]=0;
		}
		//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
		m_cepst.Run(); // run it
		//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
		m_PitchGoodness = 1.0f;
		m_Pitch=1.0f;
		//  m_cepst0=m_cepst.m_pOutput[0][ztReal]*m_cepst.m_pOutput[0][ztReal]
	  //            +m_cepst.m_pOutput[0][ztImaginary]*m_cepst.m_pOutput[0][ztImaginary];
		//rec.cepst0=m_cepst0/10;
		//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();

		for(i=option->upper_pitch_bound;i<option->lower_pitch_bound;i++)
		{
		 //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
		  x=m_cepst.m_pOutput[i][ztReal];
		 //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
		  y=m_cepst.m_pOutput[i][ztImaginary];
		 //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
		 x=x*x+y*y;
		 if(x>m_PitchGoodness)
		  {
			m_PitchGoodness=x;
			m_Pitch=i;
		  }
	   }
	   //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
	   if(option->cepstrum_pitch) m_Pitch=option->frequency_scale*(float)ztFFT_CEPST/m_Pitch; // divistion by ztFFT_CEPST transform from period to frequency units (those are not Hz yet!)
	   else m_Pitch=pitch[recnum];
	   //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
	   m_FM=57.2957795*m_FM;   // change units of FM to degrees
	  // rec.amplitude=mfa*(90.0-m_FM)/5;
	   if(option->cepstrum_pitch && option->peak_frequency_pitch)m_Pitch=option->frequency_scale*m_PeakFreq;
		m_PeakFreq=option->frequency_scale*gravity_center;

	   // now to calculation of contours:
	   for(i=0;i<256;i++)
	   {
			  Deriv[recnum][i]=m_spec_deriv[i];
			  if(max_drv<m_spec_deriv[i])max_drv=m_spec_deriv[i];
			  if(min_drv>m_spec_deriv[i])min_drv=m_spec_deriv[i];
		}


  // that is it: we have calculated all the features that can be calculsted in a single slice (excluding continuity measures)


	   // next, encode the features into the rec structure:
	   double tmp;
	   rec.pitch=m_Pitch;//-120;    // in the raw data, pitch is always based on cepstrum
	 //  rec.cepst0=rec.pitch;  // this is what goes to the raw feature table!
	   rec.entropy= m_Entropy*100;
	   rec.fm=m_FM*10;
	   rec.pgood=m_PitchGoodness*10;
	   rec.peakFr=m_PeakFreq;//-120;
	   //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
	   decode_sound(rec); // recover the m_features out of the recs, reduntant but healthy...
	   // new strategy: we place all our features in the vars of the arrays (e.g., pitch[]...)
	   //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
	   add_features_to_vars();  // add the features to arrays pitch[]..., this is now obligatory to calculate the continuity, etc.
	   //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();

	   if(slaves)// we have to handle the presure, or brain data in separate wave files!
		  {
			 //float x;
			 for(int i=0;i<slaves;i++)
			 {
			  //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
			   switch(option->slave_mode[i])
			   {
				  case average: slave_features=Calculate_amp(average, waves[i+1], pcmPos[i+1], start_at, 44); break;
				  case rectified: slave_features=Calculate_amp(rectified, waves[i+1], pcmPos[i+1], start_at, 44); break;
				  case squered: slave_features=Calculate_amp(squered, waves[i+1], pcmPos[i+1], start_at, 44); break;
			   }
			   //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
			   slave_chans[recnum][i]=slave_features; // slave_chans[recnum][i]=x;
			   pcmPos[i+1]+=advance;
			   //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
			   if( !(index%4) && amp_index<1000)
			   {
				 amp_chans[i][amp_index]=slave_features.s_amplitude;
				 if(i==0)amp_index++;
				 }
			 }
			 index++; chan_index++;
			 //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
		  }
	   // Get ready for next window
	   memset(m_fft1.m_pInput,0,sizeof(float)*m_fft1.m_nIn);  // zero out fft input...
	   pcmPos[0]-=retract; // Back up a little bit...(for over-lap). We can Assume there is always at least  ztBufferRetract number
	  if(advance==44)
	   {
		 if(addit==9) { for(int i=0;i<=slaves;i++) pcmPos[i]++; addit=0; }
		 else addit++;
	   }
	   //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
	   if(option->range_x05)
		{
		  float tmpdata[2000];
		  //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
		  waves[0].ReadSamples(pcmPos[0],tmpdata,data_samples*2);
		  //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
		  for(i=0;i<data_samples;i++) m_fft1.m_pInput[i]= tmpdata[i*2];
		}
		else waves[0].ReadSamples(pcmPos[0],m_fft1.m_pInput,data_samples);
	   //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
	   if(pcmPos[0]<0) 	break;
	   //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
	 }// end of reading loop





   //debug//myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; myfile.flush();
	 ////////////////////////////////////////////////////////////////////////////////////////////////////////
   features.bird_ID=option->bird_ID;
   features.serial_number=option->serial_number;
   //option->problem_file=1;
   max_drv/=100;// max_power/=100;
   if(!max_drv)max_drv=1;
   for(int itr=1;itr<recnum;itr++)for(int fr=1;fr<256;fr++)
						Deriv[itr][fr]/=max_drv;

   if(option->calculate_continuity)
   {
	 try
	 {
	 //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
	 continuity_created=true;
	 Calculate_continuity(recnum, 250);
	 //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
	 }
	 catch(...)
	 {
		option->problem_file=1;
	 }
   }

   //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
   CalculateDAS();
   //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
   if(option->age_in_table)
	{
	  features.day=option->bird_age;
	  features.month=-1;
	}
   else
   {
	 features.month=option->month;
	 features.day=option->day;
   }

   features.hour=option->hour;
   features.minute=option->minute;
   features.second=option->second;


   for(int i=1;i<recnum;i++)feature_array_to_var(i);
   if(option->smooth_feature || option->adapt_threshold)do_hpfilt();
   for(int i=1;i<recnum;i++)parse_sound(i);
  /*
   {
		//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
		feature_array_to_var(i);
		//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
		parse_sound(i);
		//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
   }
   */

   //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
   parse_bouts();
   //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
   if(syllDur && !maxSyllableDur)maxSyllableDur=syllDur;
   if(maxSyllableDur && !syllable_number)
   {
		//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
		Add_syll_to_features_array();//syllable_number=1;
		//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
		if(!maxBoutDur)maxBoutDur=maxSyllableDur;
   }

   Render_sonogram_images();
  /*
  for(int itr=1; itr<recnum; itr++)
  {
	   save_mem++;
	   //save_mem=0;
	   if(save_mem>4)save_mem=0;
	   if(render_image && (!option->save_image_mem || (!save_mem && slice_num<633) ))
	   {
		 slice_num++;
		 RenderImageSlice(itr);
		// if(option->Draw_sonogram)RenderSonogramSlice();
	   }
	}
	*/

  if(maxBoutDur<boutDur){ maxBoutDur=boutDur; bout_end=recnum;}
  total_frames=recnum;
  //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();

  waves[0].ResetContents();
  //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();

  if(slaves>0) for(int i=1;i<=slaves;i++) waves[i].ResetContents();
  bool return_val;
  //debug//myfile <<"At file "<<__FILE__<<" At line "<<__LINE__<<"\n"; myfile.flush();
  //option->problem_file=2;
  //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
  if(option->save_all || good_sound()) return(true);
  else return(false);
}











int CZT_FeatureCalc::Calculate_light(CWavIterator* waves, int slaves, int begin, int end)//& wave)//CWavIterator& wave)
{
	int save_mem=0, slice_num=0;
	arrays_created = false;
	 ResetContents();  // Clean up any garbage from a previous run...

	 Maximum=0;
	 option->problem_file=0;
	 int amp_index=0;

	 Slaves=slaves;
	 index=0; // for the slave channel 1 -- temporary...
	 int startLocation = option->calculateLocation;
	 int data_samples=option->ztSamplesPerWindow;
	 syllable_number=0;  Record rec;
	 float entropy_range=option->max_entropy_freq-option->min_entropy_freq;
	 Slaves=slaves;
	 int start_at;
	 if(slaves) start_at=data_samples/2-22; // this will give us a ms start position, starting in the middle of the wave epoch
	 option->syllCounter=0;
	 // init our tapers from the singleton
	 m_Taper1=CZT_MultiTaper::Instance().m_pTaper1;
	 m_Taper2=CZT_MultiTaper::Instance().m_pTaper2;
	 m_nTaperSize==CZT_MultiTaper::Instance().m_nSize;
	 // m_nTaperSize, Taper Size = number of samples to read (with overlap)

	 // Data
	 int i=0, nTotal = begin*option->Advance;
	 float fReal1,fReal2,fImag1,fImag2;
	 float fTmp, gravity_center, gc_base, log_power;
	 double cFM,sFM, min_drv=0; //  max_power=0, min_power=0,
	 max_drv=0;
	 float x,y;
	 float fTemp;
	//maxBoutDur=0; boutDur=0;
	maxSyllableDur=0; syllDur=0; stopDur=1000;

	// Get Starting positiong

   // waves_to_starting_position(&wave); // this function will getheadposition() for all waves
	PCMPOS pcmPos[11];
	for(int i=0;i<=slaves;i++)
		pcmPos[i]=waves[i].GetHeadPosition();


	option->sampling_rate=waves[0].m_WaveHeader.nSamplesPerSec;
	option->frequency_scale=(option->sampling_rate/2)/512;

	pcmPos[0]+=(long)begin*option->Advance;
	// if focus on
	if(option->range_x05) waves[0].ReadSamples(pcmPos[0], m_fft1.m_pInput, 2*option->ztSamplesPerWindow);
	else waves[0].ReadSamples(pcmPos[0], m_fft1.m_pInput, option->ztSamplesPerWindow);
	int nFileTotal  = option->ztSamplesPerWindow+1000*option->Advance;//   waves[0].GetTotalSamples();
	recnum=0;
	int advance=option->Advance, retract;//ztFeatureAdvance;  60
	if(option->doubleSamples) advance=option->Advance/2; // if sample rate is 22050Hz
	slices=1000;//end/advance; //nFileTotal/advance;
	if(!slices)return(false); // wave file contains no data
	if(option->create_feature_arrays) create_arrays(slices);
	if(option->range_x05)retract=2*data_samples-advance;
	else retract=data_samples-advance;


	if(!option->cepstrum_pitch)
	{

	  // new fundamental frequency pitch--------------------------------------------------
	  //int numSamples =  waves[0].GetTotalSamples();
	  int sampling_rate=waves[0].m_WaveHeader.nSamplesPerSec;
	  long startPos = pcmPos[0];
	  initYin(sampling_rate,option->minFreq);//option->sampling_rate,  22);
	   m_alt_pitch_in = new float[nFileTotal];
	  m_alt_pitch_out = new float[nFileTotal];
	  //m_alt_pitch_in = (float*) malloc (sizeof(float)*nFileTotal);
	  //m_alt_pitch_out = (float*) malloc (sizeof(float)*nFileTotal);
	  waves[0].ReadSamples(startPos,m_alt_pitch_in, nFileTotal);
	  getFundamentalFrequency(nFileTotal, m_alt_pitch_in, m_alt_pitch_out);
	  // that's it, now move to pitch array
	  int div=nFileTotal/1000;

	  int maxpitch;
	  if(option->range_x2 == false && option->range_x05 == false) maxpitch=sampling_rate/4;
	  else if(option->range_x2 == false && option->range_x05 == true) maxpitch=sampling_rate/8;
	  else maxpitch=sampling_rate/2;
	  // save pitch to array:
	  float pp;
	  for(int i=0;i<1000;i++){
		   pp=(float)sampling_rate * m_alt_pitch_out[div*i];
		   if(pp<10 || pp>maxpitch)pitch[i]=0;
		   else pitch[i]=pp;
	   }
      // free the memory
		   delete[] m_alt_pitch_in;
		   delete[] m_alt_pitch_out;
	  //-----------------------------------------------------------------------------------
	}


	m_Image.signal_r=signal;
	m_Image.bout_r=bout_signal;
   /*
	m_Image.hp_small_r=hp_small;
	m_Image.hp_big_r=hp_big;
	m_Image.hp_diff_r=hp_diff;
   */
	m_sonogram.signal_r=signal;
	m_sonogram.bout_r=bout_signal;
	/*
	m_sonogram.hp_small_r=hp_small;
	m_sonogram.hp_big_r=hp_big;
	m_sonogram.hp_diff_r=hp_diff;
	*/
	m_contours.signal_r=signal;
	m_contours.bout_r=bout_signal;
	/*
	m_contours.hp_small_r=hp_small;
	m_contours.hp_big_r=hp_big;
	m_contours.hp_diff_r=hp_diff;
	*/


	if(slaves)
	{
	  slave_chans=new Slave_Features*[slices];
	  for(int i=0;i<slices;i++) slave_chans[i]=new Slave_Features[slaves+1];
	  slave_chan_exist=true;
	}

	reset_syllable_vars();
	int addit=0;
	// for continuity we declare an array for the derivatives
	deriv_created=true;
	Deriv= new float*[slices+2];
	for(int i=0;i<slices+2;i++)Deriv[i]=new float[256];

	Sono= new float*[slices+2];
	for(int i=0;i<slices+2;i++)Sono[i]=new float[256];

	while ( nTotal <= end )//nFileTotal-(option->ztSamplesPerWindow+option->Advance))
	{
		 if(option->slow_execution && !(recnum%100)) Sleep(option->slow_execution);
		 nTotal += advance;//ztFeatureAdvance; // Advance ztFeatureAdvance samples at a time
		 recnum++;
		 // Copy input data to fft2...
		 memcpy(m_fft2.m_pInput,m_fft1.m_pInput,sizeof(float)*m_fft1.m_nIn);

		 if(option->doubleSamples) // Interpulate to half the frequency range
		 {
			  for(i=data_samples-1; i>=0; i--) m_fft1.m_pInput[i]= m_fft1.m_pInput[i/2];
			  for(i=1;i<data_samples-2;i++) if(i%2)m_fft1.m_pInput[i]= m_fft1.m_pInput[i-1]/2+m_fft1.m_pInput[i+1]/2;
			  for(i=0;i<data_samples;i++)  m_fft2.m_pInput[i]= m_fft1.m_pInput[i];

		 }

		 if(option->range_x2) // use every other sample to double the frequency range
		 {
			  for(i=data_samples-1; i>=0; i--) m_fft1.m_pInput[i]= m_fft1.m_pInput[i/2];
			  for(i=1;i<data_samples-2;i++) if(i%2)m_fft1.m_pInput[i]= m_fft1.m_pInput[i-1]/2+m_fft1.m_pInput[i+1]/2;
			  for(i=0;i<data_samples;i++)  m_fft2.m_pInput[i]= m_fft1.m_pInput[i];
		 }


		 // do taper multiplies
		 for(i=0;i<data_samples;i++)
		 {
			   m_fft1.m_pInput[i] *=m_Taper1[i];
			   m_fft2.m_pInput[i] *=m_Taper2[i];
			   if(option->boost_amplitude)
			   {
				  m_fft1.m_pInput[i] *=5;
				  m_fft2.m_pInput[i] *=5;

			   }
		 }
		 // Run em...

		 try{ m_fft1.Run(); }
		 catch ( ... ) { return(false); }


		 try{ m_fft2.Run(); }
		 catch ( ... ) { return(false); }

		 // Tapers Are Done.
		 //////////////////////////////////////////////////////////////////////

		 // we now caclulate cepstrum for pitch and goodness of pitch estimates

		 // Zero these out..
		 m_time_deriv_max=m_freq_deriv_max=m_amplitude=m_MaxPower=m_SumLog=m_LogSum=0;

		 gc_base=gravity_center=m_AM=noise_power=0;
		 for(i=0;i<option->spectrum_range;i++) //256 out of the whole 512, which is up to 11kHz
		 {

			   //fReal[x] = fImage[x] - references output of fft1
			   // real2,imag2 - reference fft2
			   fReal1 =m_fft1.m_pOutput[i][ztReal];
			   fReal2 =m_fft2.m_pOutput[i][ztReal];
			   fImag1 =m_fft1.m_pOutput[i][ztImaginary];
			   fImag2 =m_fft2.m_pOutput[i][ztImaginary];

			   // Calculate power spectrum..
			   m_powSpec[i] = fReal1*fReal1+fReal2*fReal2+fImag1*fImag1+fImag2*fImag2;
			   // m_power+=m_powSpec[i];
			   // calculate time derivatives
			   m_time_deriv[i]=-fReal1*fReal2-fImag1*fImag2;
			   // calcumate frequency derivatives
			   m_freq_deriv[i]=fImag1*fReal2-fReal1*fImag2;
			   Sono[recnum][i]=1+3000.0*log(0.1+(fReal1*fReal1)+(fImag1*fImag1));

			   // calculate Wiener entropy, amplitude and mean frequency upon this frequency range
			   if(i>=option->min_entropy_freq && i<option->max_entropy_freq && m_powSpec[i])
			   {
				 // find maximal values of time and frequency derivatives of a windows for FM.
				 fTmp=m_time_deriv[i]*m_time_deriv[i];
				 if(fTmp>m_time_deriv_max) m_time_deriv_max=fTmp;
				 fTmp=m_freq_deriv[i]*m_freq_deriv[i];
				 if(fTmp>m_freq_deriv_max) m_freq_deriv_max=fTmp;

				  // find peak frequency
				 if(m_MaxPower<m_powSpec[i])
				 {
					m_MaxPower=m_powSpec[i];
					m_PeakFreq=i;
				 }

				 if(Maximum<m_powSpec[i])Maximum=m_powSpec[i];

				  // calculate power in the noise-detector frequency range
				 if(   ( option->detect_low_noise && i<option->noise_thresh_freq)   // for low freq noise
				  //|| (!option->detect_low_noise && i>option->noise_thresh_freq)  // for high freq noise
				   )
				 {
				   noise_power+=m_powSpec[i];
				 }

				 m_amplitude+=m_powSpec[i];
				 log_power=m_freq_deriv[i]*m_freq_deriv[i]+m_time_deriv[i]*m_time_deriv[i];
				 m_SumLog+=log(m_powSpec[i]);//log(m_powSpec[i]);
				 gravity_center+=i*log_power;
				 gc_base+=log_power;
				 m_LogSum+=m_powSpec[i];
				 m_AM+=m_time_deriv[i];

			   }

		 }  // end for all frequencies in a frame.
         principal_frequency[recnum]=m_PeakFreq;
		 gravity_center/=max(gc_base,(float)1); // for calculating mean frequency
		 if(!m_amplitude)m_amplitude=1;
		 m_AM/=m_amplitude;
		 if(m_AM)rec.am=m_AM*100;

		 noise_power/=max(m_amplitude,(float)1);
		 m_amplitude=log10(m_amplitude+1)*10-option->baseline;   // get the units to dB
		// int raw_amp=m_amplitude;
		 if(noise_power>option->noise_ratio)m_amplitude=0;  //  filter out the noisy sounds
		 rec.amplitude=m_amplitude*10;

		 int mean_fr_int=gravity_center;
		 double mfa=0;
		 for(i=-5;i<6;i++)
		 {
		   if(mean_fr_int>4 && mean_fr_int<200)
				mfa+=m_powSpec[mean_fr_int+i];   // we are not interested in frequecny below 4=150Hz
		 }
		 mfa=log10(mfa+1)*10-option->baseline;
		 rec.mean_freq_amp=mfa*10;
         ////myfile<<recnum<<" processing mean_freq: "<<rec.mean_freq_amp<<"\n";
		 // we now compute the features of this fft window:
		 //////////////////////////////////////////////////////////////////////
		 // compute Wiener entropy
		 if(m_LogSum)
		 {
			m_LogSum=log(m_LogSum/entropy_range);
			m_Entropy=(m_SumLog/entropy_range)-m_LogSum;
		 }
		 else  m_Entropy=0;// rec.power=0;


		 // compute FM
		 if(m_freq_deriv_max && (m_time_deriv_max != 0))// div by 0 caught
			   m_FM=atan(m_time_deriv_max/m_freq_deriv_max);// Possible div by 0? not anymore...
		 else
			   m_FM=0;

		 // Compute SPECTRAL DERIVATIVES
		 //////////////////////////////////////////////////////////////////////

		cFM=cos(m_FM);
		sFM=sin(m_FM);

		// Calc pitch     // this 256 will give use sufficient range assuming 512 freq of 22050Hz
		memset(m_cepst.m_pInput,0,sizeof(float)*m_cepst.m_nIn);// zero out fft.
		for(i=7;i<255;i++)
		{
		  m_spec_deriv[i]=( m_time_deriv[i]*sFM+m_freq_deriv[i]*cFM);
		  fTemp =m_powSpec[i];//*m_Taper1[i];
		  if(fTemp != 0 && i>=option->min_entropy_freq && i<option->max_entropy_freq) m_cepst.m_pInput[i]=m_spec_deriv[i]/fTemp;
		  else m_cepst.m_pInput[i]=0;
		}
		m_cepst.Run(); // run it
		m_PitchGoodness = 1.0f;
		m_Pitch=1.0f;
	  //  m_cepst0=m_cepst.m_pOutput[0][ztReal]*m_cepst.m_pOutput[0][ztReal]
	  //            +m_cepst.m_pOutput[0][ztImaginary]*m_cepst.m_pOutput[0][ztImaginary];
		//rec.cepst0=m_cepst0/10;
		for(i=option->upper_pitch_bound;i<option->lower_pitch_bound;i++)
		{
		  x=m_cepst.m_pOutput[i][ztReal];
		  y=m_cepst.m_pOutput[i][ztImaginary];
		  x=x*x+y*y;
		  if(x>m_PitchGoodness)
		  {
				  m_PitchGoodness=x;
				  m_Pitch=i;
		  }
	   }
	   if(option->cepstrum_pitch) m_Pitch=option->frequency_scale*(float)ztFFT_CEPST/m_Pitch; // divistion by ztFFT_CEPST transform from period to frequency units (those are not Hz yet!)
	   else m_Pitch=pitch[recnum];
	   m_FM=57.2957795*m_FM;   // change units of FM to degrees
	  // rec.amplitude=mfa*(90.0-m_FM)/5;
	   if(option->cepstrum_pitch && option->peak_frequency_pitch)m_Pitch=option->frequency_scale*m_PeakFreq;

	   m_PeakFreq=option->frequency_scale*gravity_center;


	   // now to calculation of contours:
	   for(i=0;i<256;i++)
	   {

			  Deriv[recnum][i]=m_spec_deriv[i];
			  if(max_drv<m_spec_deriv[i])max_drv=m_spec_deriv[i];
			  if(min_drv>m_spec_deriv[i])min_drv=m_spec_deriv[i];
	   }


  // that is it: we have calculated all the features that can be calculsted in a single slice (excluding continuity measures)


	   // next, encode the features into the rec structure:
	   double tmp;
	   rec.pitch=m_Pitch;//-120;    // in the raw data, pitch is always based on cepstrum
	   //rec.cepst0=rec.pitch;  // this is what goes to the raw feature table!
	   rec.entropy= m_Entropy*100;
	   rec.fm=m_FM*10;
	   rec.pgood=m_PitchGoodness*10;
	   rec.peakFr=m_PeakFreq;//-120;
	   decode_sound(rec); // recover the m_features out of the recs, reduntant but healthy...
	   // new strategy: we place all our features in the vars of the arrays (e.g., pitch[]...)

	   add_features_to_vars();  // add the features to arrays pitch[]..., this is now obligatory to calculate the continuity, etc.
	   if(slaves)// we have to handle the presure, or brain data in separate wave files!
		  {
			 //float x;
			 for(int i=0;i<slaves;i++)
			 {
			   switch(option->slave_mode[i])
			   {
				  case average: slave_features=Calculate_amp(average, waves[i+1], pcmPos[i+1], start_at, 44); break;
				  case rectified: slave_features=Calculate_amp(rectified, waves[i+1], pcmPos[i+1], start_at, 44); break;
				  case squered: slave_features=Calculate_amp(squered, waves[i+1], pcmPos[i+1], start_at, 44); break;
			   }
			   slave_chans[recnum][i]=slave_features; // slave_chans[recnum][i]=x;
			  // mean_chan_amp[0][i]+=slave_features.s_amplitude; //  mean_chan_amp[0][i]+=x;
			  // mean_chan_amp[1][i]+=slave_chans[recnum][i];
			   pcmPos[i+1]+=advance;
			   if( !(index%4) && amp_index<1000)
			   {
				 amp_chans[i][amp_index]=slave_features.s_amplitude;
				 if(i==0)amp_index++;
			   }
			 }
			 index++; chan_index++;
		  }
		
	   // Get ready for next window
	   memset(m_fft1.m_pInput,0,sizeof(float)*m_fft1.m_nIn);  // zero out fft input...
	   pcmPos[0]-=retract; // Back up a little bit...(for over-lap). We can Assume there is always at least  ztBufferRetract number
	   if(advance==44)
	   {
		 if(addit==9)
		 {
		   for(int i=0;i<=slaves;i++)
		   pcmPos[i]++; addit=0;

		 }
		 else addit++;
	   }

	   if(option->range_x05)
		{
		   float tmpdata[2000];
		   waves[0].ReadSamples(pcmPos[0],tmpdata,data_samples*2);
		   for(i=0;i<data_samples;i++) m_fft1.m_pInput[i]= tmpdata[i*2];
	   }
	   else waves[0].ReadSamples(pcmPos[0],m_fft1.m_pInput,data_samples);
	   if(pcmPos[0]<0)
						break;
	 }// end of reading loop

	 ////////////////////////////////////////////////////////////////////////////////////////////////////////
   features.bird_ID=option->bird_ID;
   features.serial_number=option->serial_number;
   //option->problem_file=1;
   max_drv/=100;// max_power/=100;
   if(!max_drv)max_drv=1;
   for(int itr=1;itr<recnum;itr++)for(int fr=1;fr<256;fr++)
						Deriv[itr][fr]/=max_drv;

   if(option->calculate_continuity)
   {
	 try
	 {
		Calculate_continuity(recnum, 250);
		continuity_created=true;
	 }
	 catch(...)
	 {
		option->problem_file=1;
	 }
   }
   CalculateDAS();
   //do_hpfilt();

   if(option->age_in_table)
	{
	  features.day=option->bird_age;
	  features.month=-1;
	}
   else
   {
	 features.month=option->month;
	 features.day=option->day;
   }

   features.hour=option->hour;
   features.minute=option->minute;
   features.second=option->second;

   for(int i=1;i<recnum;i++)feature_array_to_var(i);
   if(option->smooth_feature || option->adapt_threshold)do_hpfilt();
   for(int i=1;i<recnum;i++)parse_sound(i);
   /*
   for(int i=1;i<recnum;i++) //for (int itr = begin; itr < begin+recnum; itr++)
   {
		feature_array_to_var(i);
		parse_sound(i);
   }     */
   parse_bouts();
   if(syllDur && !maxSyllableDur)maxSyllableDur=syllDur;
   if(maxSyllableDur && !syllable_number)
   {
		Add_syll_to_features_array();//syllable_number=1;
		if(!maxBoutDur)maxBoutDur=maxSyllableDur;
   }
   Render_sonogram_images();

   /*
   for(int itr=1; itr<recnum; itr++)
   //for (int itr = begin; itr < begin+recnum; itr++)
   {
	   save_mem++;
	   //save_mem=0;
	   if(save_mem>4)save_mem=0;
	   if(render_image && (!option->save_image_mem || (!save_mem && slice_num<633) ))
	   {
		 slice_num++;
		 RenderImageSlice(itr);
		// if(option->Draw_sonogram)RenderSonogramSlice();
	   }
	}
	*/

   // render contours
   /*
   if(render_image && !option->save_image_mem)
   {
	  for(int itr=1; itr<recnum; itr++) RenderSonogramSlice(itr);
   }
   */
  // clean up:
   //for(int i=0;i<slices;i++) delete[] Deriv[i];
   //delete[] Deriv;

   /*for(int i=0;i<slices+2;i++) delete[] Sono[i];
   delete[] Sono;*/

 /* if(option->calculate_continuity)
  {
	  for (int i=0; i<recnum; i++)
	  {
			delete[] Borders[i];
			delete[] borders[i];
	  }
	  delete[] Borders;
	  delete[] borders;
  } */
  if(maxBoutDur<boutDur){ maxBoutDur=boutDur; bout_end=recnum;}
  total_frames=recnum;

  waves[0].ResetContents();
  if(slaves>0) for(int i=1;i<=slaves;i++) waves[i].ResetContents();
  bool return_val;
  //option->problem_file=2;
  if(option->save_all || good_sound()) return(true);
  else return(false);
}
















void CZT_FeatureCalc::reset_syllable_vars()
{
    syllDur=0; sum_pitch=0; sum_pitch_simp=0; sum_principal_contour=0; sum_entropy=0; sum_fm=0; sum_pgood=0; sum_amplitude=0;
    sum_continuity_t=0; sum_continuity_f=0; sum_slope=0; sum_am=0; sum_peakFreq=0;
    SS_pitch=0; SS_entropy=0; SS_fm=0; SS_pgood=0; SS_am=0; SS_peakFreq=0;  SS_amplitude=0;
    min_pitch=99999; min_principal_contour=99999; min_FM=99999; min_entropy=99999; min_peakFr=99999; min_slope=99999;
    max_pitch=0; max_principal_contour=0; max_FM=0; max_entropy=-99999; max_peakFr=0;
    max_slope=-99999; max_amplitude=0; max_continuity_f=0; max_continuity_t=0;

    if(option->slave_features)
    {
        sum_s1_amp=0; sum_s2_amp=0; sum_s3_amp=0; sum_s4_amp=0; sum_s5_amp=0;
        sum_s1_ent=0; sum_s2_ent=0; sum_s3_ent=0; sum_s4_ent=0; sum_s5_ent=0;
        sum_s1_peak_frequency=0; sum_s2_peak_frequency=0; sum_s3_peak_frequency=0; sum_s4_peak_frequency=0; sum_s5_peak_frequency=0;
        sum_s1_peak_power=0; sum_s2_peak_power=0; sum_s3_peak_power=0; sum_s4_peak_power=0; sum_s5_peak_power=0;
        sum_s1_highpass_power=0; sum_s2_highpass_power=0; sum_s3_highpass_power=0; sum_s4_highpass_power=0; sum_s5_highpass_power=0;
    }
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
void CZT_FeatureCalc::save_ms_interval(int start, int end, AnsiString raw_features_table)
{
int time=recnum;
   DeleteFile("c:\\sap\\features_out.txt");
   FILE *stream;
   stream = fopen("c:\\sap\\features_out.txt", "w+");
   //setvbuf(stream, NULL, _IOLBF, 512);
   if((good_sound() || option->save_all) && option->saveRawFile)
   {
      //serial_number instead of file_age
      int file_index=DataForm->File_to_table(option->bird_ID, newFile, option->serial_number, option->bird_age, option->chan);  // insert a record into the file table
      int unsigned time_stamp=option->ms_from_midnight*10;  // the units of time are now 0.1ms
      int advance_window=0.5+(float)option->Advance/4.4;
      if(advance_window<1)advance_window=1;
      if(advance_window>100)advance_window=100;

      if(option->format->ItemIndex!=2)// there is a risk of overlapping times
      {
        if(last_ms_record>time_stamp) // there is an overlapping time!
        {
           if(last_ms_record-time_stamp<9000)time_stamp=last_ms_record;
        }
      }

      for(int i=start;i<=end;i++)
      {
        time_stamp+=advance_window;
        fprintf(stream,"%d \t %d \t %.0f \t %.0f \t %.0f \t %.0f \t %.0f \t %.0f \t  %.0f \t %.0f \t %.0f \t %.0f \t  %.0f \t %.0f \n"
        , time_stamp, file_index, amplitude[i], mfa[i], cepst0[i], peakFreq[i], 10*fm[i], 100*am[i], pitchGoodness[i]
        , 100*entropy[i], principal_frequency[i]-120, slope[i]-120,  100*continuity_t[i], 100*continuity_f[i]);
      }

      last_ms_record=time_stamp;
      fclose(stream);

      str="LOAD DATA INFILE 'c:/sap/features_out.txt' into table ";
      str+=raw_features_table;
      DataForm->MyCommand1->SQL->Clear();
      DataForm->MyCommand1->SQL->Add(str);
      DataForm->MyCommand1->Execute(1);
      option->ms_recordes_added=time;     
}

  */
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////




bool CZT_FeatureCalc::save_ms_table(int slaves)  // save features of every ms into a mySQL ms table
{
  // static int unsigned last_ms_record=0;
   int time=recnum;
   DeleteFile("c:\\sap\\features_out.txt");
   FILE *stream;
   stream = fopen("c:\\sap\\features_out.txt", "w+");
   //setvbuf(stream, NULL, _IOLBF, 512);
   if((good_sound() || option->save_all) && option->saveRawFile)
   {
	  //serial_number instead of file_age



	  //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();

      int file_index=DataForm->File_to_table(option->bird_ID, newFile, option->serial_number, option->bird_age, option->chan);  // insert a record into the file table



	  //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();

	  int unsigned time_stamp=option->ms_from_midnight*10;  // the units of time are now 0.1ms



	  //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();

	  int advance_window=0.5+(float)option->Advance/((float)option->sampling_rate/10000.0);   //4.4;



	  //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();

	  if(advance_window<1)advance_window=1;
      if(advance_window>100)advance_window=100;

      if(last_ms_record>time_stamp) // there is an overlapping time!
      {
       if(last_ms_record-time_stamp<9000)time_stamp=last_ms_record;
	  }

      for(int i=1;i<=time;i++)
	  {
        time_stamp+=advance_window;
		fprintf(stream,"%d \t %d \t %.0f \t %.0f \t %.0f \t %.0f \t %.0f \t %.0f \t  %.0f \t %.0f \t %.0f \t %.0f \t  %.0f \t %.0f \n"
		, time_stamp, file_index, amplitude[i], mfa[i], pitch[i], peakFreq[i], 10*fm[i], 100*am[i], pitchGoodness[i]
		, 100*entropy[i], option->frequency_scale*principal_frequency[i], slope[i],  100*continuity_t[i], 100*continuity_f[i]);
	  }

      /*
      double tmp1=(double)option->file_age;
      double tmp2=abs(option->file_age);
      tmp1=tmp1-tmp2;
      tmp1*=86400000;
      int unsigned time_stamp=tmp1;      //float time_stamp=(option->hour*10000)+(option->minute*100)+(option->second);// the file age
      if(last_ms_record>time_stamp)
      {
        if(last_ms_record-last_ms_record<9000)time_stamp=last_ms_record;
      }


   switch(Slaves)
   {
	case 0:



	//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();

	  for(int i=1;i<=time;i++)
	  {
		time_stamp+=advance_window;
		fprintf(stream,"%d \t %d \t %.0f \t %.0f \t %.0f \t %.0f \t %.0f \t %.0f \t  %.0f \t %.0f \t %.0f \t %.0f \t  %.0f \t %.0f \n"
		, time_stamp, file_index, amplitude[i], mfa[i], pitch[i], peakFreq[i], 10*fm[i], 100*am[i], pitchGoodness[i]
		, 100*entropy[i], principal_frequency[i], slope[i],  100*continuity_t[i], 100*continuity_f[i]);
	  }
	  break;

	case 1:



	//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();

	  for(int i=1;i<=time;i++)
	  {
		time_stamp+=advance_window;
		fprintf(stream,"%d \t %d \t %.0f \t %.0f \t %.0f \t %.0f \t %.0f \t %.0f \t  %.0f \t %.0f \t %.0f \t %.0f \t  %.0f \t %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \n"
		, time_stamp, file_index, amplitude[i], mfa[i], pitch[i], peakFreq[i], 10*fm[i], 100*am[i], pitchGoodness[i]
		, 100*entropy[i], principal_frequency[i]-120, slope[i]-120,  100*continuity_t[i], 100*continuity_f[i]
		, (float)slave_chans[i][0].s_amplitude, (float)slave_chans[i][0].s_entropy, (float)slave_chans[i][0].s_peakFr+100.0
		, (float)slave_chans[i][0].s_peakFr_power , (float)slave_chans[i][0].s_highpass_power
		);
	  }
	  break;

   case 2:



   //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();

	  for(int i=1;i<=time;i++)
	  {
		time_stamp+=advance_window;
		fprintf(stream,"%d \t %d \t %.0f \t %.0f \t %.0f \t %.0f \t %.0f \t %.0f \t  %.0f \t %.0f \t %.0f \t %.0f \t  %.0f \t %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \n"
		, time_stamp, file_index, amplitude[i], mfa[i], pitch[i], peakFreq[i], 10*fm[i], 100*am[i], pitchGoodness[i]
		, 100*entropy[i], principal_frequency[i]-120, slope[i]-120,  100*continuity_t[i], 100*continuity_f[i]
		, (float)slave_chans[i][0].s_amplitude, (float)slave_chans[i][0].s_entropy, (float)slave_chans[i][0].s_peakFr+100.0
		, (float)slave_chans[i][0].s_peakFr_power , (float)slave_chans[i][0].s_highpass_power
		, (float)slave_chans[i][1].s_amplitude, (float)slave_chans[i][1].s_entropy, (float)slave_chans[i][1].s_peakFr+100.0
		, (float)slave_chans[i][1].s_peakFr_power , (float)slave_chans[i][1].s_highpass_power
		);
	  }
	  break;

   case 3:



   //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();

	  for(int i=1;i<=time;i++)
	  {
		time_stamp+=advance_window;
		fprintf(stream,"%d \t %d \t %.0f \t %.0f \t %.0f \t %.0f \t %.0f \t %.0f \t  %.0f \t %.0f \t %.0f \t %.0f \t  %.0f \t %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \n"
		, time_stamp, file_index, amplitude[i], mfa[i], pitch[i], peakFreq[i], 10*fm[i], 100*am[i], pitchGoodness[i]
		, 100*entropy[i], principal_frequency[i]-120, slope[i]-120,  100*continuity_t[i], 100*continuity_f[i]
		, (float)slave_chans[i][0].s_amplitude, (float)slave_chans[i][0].s_entropy, (float)slave_chans[i][0].s_peakFr+100.0
		, (float)slave_chans[i][0].s_peakFr_power , (float)slave_chans[i][0].s_highpass_power
		, (float)slave_chans[i][1].s_amplitude, (float)slave_chans[i][1].s_entropy, (float)slave_chans[i][1].s_peakFr+100.0
		, (float)slave_chans[i][1].s_peakFr_power , (float)slave_chans[i][1].s_highpass_power
		, (float)slave_chans[i][2].s_amplitude, (float)slave_chans[i][2].s_entropy, (float)slave_chans[i][2].s_peakFr+100.0
		, (float)slave_chans[i][2].s_peakFr_power , (float)slave_chans[i][2].s_highpass_power
		);
	  }
	  break;

   case 4:



   //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();

	  for(int i=1;i<=time;i++)
	  {
		time_stamp+=advance_window;
		fprintf(stream,"%d \t %d \t %.0f \t %.0f \t %.0f \t %.0f \t %.0f \t %.0f \t  %.0f \t %.0f \t %.0f \t %.0f \t  %.0f \t %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \n"
		, time_stamp, file_index, amplitude[i], mfa[i], pitch[i], peakFreq[i], 10*fm[i], 100*am[i], pitchGoodness[i]
		, 100*entropy[i], principal_frequency[i]-120, slope[i]-120,  100*continuity_t[i], 100*continuity_f[i]
		, (float)slave_chans[i][0].s_amplitude, (float)slave_chans[i][0].s_entropy, (float)slave_chans[i][0].s_peakFr+100.0
		, (float)slave_chans[i][0].s_peakFr_power , (float)slave_chans[i][0].s_highpass_power
		, (float)slave_chans[i][1].s_amplitude, (float)slave_chans[i][1].s_entropy, (float)slave_chans[i][1].s_peakFr+100.0
		, (float)slave_chans[i][1].s_peakFr_power , (float)slave_chans[i][1].s_highpass_power
		, (float)slave_chans[i][2].s_amplitude, (float)slave_chans[i][2].s_entropy, (float)slave_chans[i][2].s_peakFr+100.0
		, (float)slave_chans[i][2].s_peakFr_power , (float)slave_chans[i][2].s_highpass_power
		, (float)slave_chans[i][3].s_amplitude, (float)slave_chans[i][3].s_entropy, (float)slave_chans[i][3].s_peakFr+100.0
		, (float)slave_chans[i][3].s_peakFr_power , (float)slave_chans[i][3].s_highpass_power
		);
	  }
	  break;

   case 5:



   //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();

	  for(int i=1;i<=time;i++)
	  {
		time_stamp+=advance_window;
		fprintf(stream,"%d \t %d \t %.0f \t %.0f \t %.0f \t %.0f \t %.0f \t %.0f \t  %.0f \t %.0f \t %.0f \t %.0f \t  %.0f \t %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \t  %.0f \n"
		, time_stamp, file_index, amplitude[i], mfa[i], pitch[i], peakFreq[i], 10*fm[i], 100*am[i], pitchGoodness[i]
		, 100*entropy[i], principal_frequency[i]-120, slope[i]-120,  100*continuity_t[i], 100*continuity_f[i]
		, (float)slave_chans[i][0].s_amplitude, (float)slave_chans[i][0].s_entropy, (float)slave_chans[i][0].s_peakFr+100.0
		, (float)slave_chans[i][0].s_peakFr_power , (float)slave_chans[i][0].s_highpass_power
		, (float)slave_chans[i][1].s_amplitude, (float)slave_chans[i][1].s_entropy, (float)slave_chans[i][1].s_peakFr+100.0
		, (float)slave_chans[i][1].s_peakFr_power , (float)slave_chans[i][1].s_highpass_power
		, (float)slave_chans[i][2].s_amplitude, (float)slave_chans[i][2].s_entropy, (float)slave_chans[i][2].s_peakFr+100.0
		, (float)slave_chans[i][2].s_peakFr_power , (float)slave_chans[i][2].s_highpass_power
		, (float)slave_chans[i][3].s_amplitude, (float)slave_chans[i][3].s_entropy, (float)slave_chans[i][3].s_peakFr+100.0
		, (float)slave_chans[i][3].s_peakFr_power , (float)slave_chans[i][3].s_highpass_power
		, (float)slave_chans[i][4].s_amplitude, (float)slave_chans[i][4].s_entropy, (float)slave_chans[i][4].s_peakFr+100.0
		, (float)slave_chans[i][4].s_peakFr_power , (float)slave_chans[i][4].s_highpass_power
		);
	  }
	  break;
  }// end switch

  */

	  //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();

	  last_ms_record=time_stamp;
	  fclose(stream);

	  str="LOAD DATA INFILE 'c:/sap/features_out.txt' into table ";
	  str+=option->features_table;



	  //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();

	  DataForm->MyCommand1->SQL->Clear();



	  //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();

	  DataForm->MyCommand1->SQL->Add(str);



	  //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();

	  DataForm->MyCommand1->Execute(1);



	  //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();

	  //inout.close();

	//  DeleteFile("c:\\sap\\features_out.txt");
	  //DataForm->Change_Database("SAP");
	  option->ms_recordes_added=time;
      return(true);
   }
   // we should look into this else, it might never materialize since the calling file (live) will decide if to call this based on calcualte return value....
   else
   {



		//myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();

	   DeleteFile(raw_feature_file); // delete the raw feature file



	   //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();

	   if(option->delete_trash_file)DeleteFile(option->wave_names[0]);



	   //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();

	   mem_recs.erase(mem_recs.begin(), mem_recs.end());



	   //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();

    //   recs_exsist=false;
       option->ms_recordes_added=0;
       return(false);// both sound and feature files are not useful and deleted
    }

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 Continuity and principal contour features are calculated after the whole sound file is processed with derivative spectra in place
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CZT_FeatureCalc::Calculate_continuity(int time, int freq)
{
    static int old_principal_num=0;
	Borders=new bool*[time];
	borders=new int*[time];
	for(int i=0;i<time;i++)
	{
		Borders[i]=new bool[256];
		borders[i]=new int[256];
	}
	for(int i=0;i<time;i++)for(int j=0;j<256;j++){Borders[i][j]=false;  borders[i][j]=false;}

	float ctr, ctr_base=0.01*option->contour_thresh->Position;//-0.1;
   // double treshold=0.001*option->contour_biasTB->Position;
	// we now continue with the contours!!!

 try{

   for(int itr=3;itr<time-3;itr++)
   {
	   //int ind=1;
		// here we find the contours, zero crossings
	 //for(int fr=10;fr<170;fr++)
	 for(int fr=option->min_entropy_freq; fr<min(240,option->max_entropy_freq); fr++)
	 {
	   ctr=ctr_base*fabs(entropy[itr])/max(fabs(fr-(peakFreq[itr]/option->frequency_scale)),(float)1);
	   if(
		   (
			   (Deriv[itr][fr]*Deriv[itr+1][fr]<0 && Deriv[itr-1][fr]*Deriv[itr+2][fr]<0

				 && Deriv[itr][fr]>ctr && Deriv[itr+1][fr]  <-ctr)

			|| (Deriv[itr][fr]*Deriv[itr][fr+1]<0 && Deriv[itr][fr-1]*Deriv[itr][fr+2]<0

				 && Deriv[itr][fr]>ctr && Deriv[itr][fr+1]  <-ctr)

			|| (Deriv[itr][fr]*Deriv[itr+1][fr+1]<0 && Deriv[itr-1][fr-1]*Deriv[itr+2][fr+2]<0

				 && Deriv[itr][fr]>ctr && Deriv[itr+1][fr+1]<-ctr )

			|| (Deriv[itr][fr]*Deriv[itr+1][fr-1]<0 && Deriv[itr-1][fr+1]*Deriv[itr+2][fr-2]<0

				&& Deriv[itr][fr]>ctr && Deriv[itr+1][fr-1]<-ctr )

			)// && FinalFFT[itr][fr]>treshold//(fabs( fr-(peakFreq[itr]/option->frequency_scale) )/fabs(entropy[itr])+1)*FinalFFT[itr][fr]>treshold*amplitude[itr]
		 )

		{
		   Borders[itr][fr]=true;

		}  // end if

	  //  Contours[itr][50]=ind;  //  Picks[][50] is the number of picks
	  //  if(Contours[itr][50]>1 && PeakFreq[itr]>7) Signal[itr]=1;
	 }  // end  for fr
   }  // end for itr


 }
  catch(...)
  {Application->MessageBox(L"Continuity calculation error", NULL, MB_OK);}
//option->problem_file=2;}


 //  int cp=0;
   for(int itr=0;itr<time;itr++)
   {
		for(int fr=0;fr<255;fr++)
		{
				borders[itr][fr]=Borders[itr][fr];
			  //	if(!cp && Borders[itr][fr])cp=fr;//cp+=Borders[itr][fr];
		}
	  //	if(option->contour_pitch)pitch[itr]=44.1*cp;
	 //	cp=0;
   }


   // for(int itr=0;itr<time;itr++) for(int fr=0;fr<200;fr++) borders[itr][fr]=Borders[itr][fr];
   AnsiString var_name,  str;
  // bool BorderExist=true;
  // int contour_start=0; // a nx2 matrix to hold the points of the contour we deteced
   Contour cnt;
   vector<Contour> contours;

   contours.reserve(time*10);
   int **Contour_features;

  /*
	 Countour holds ALL the contours data:
	 time, freq, contour number
	 502  33  10
	 503  33  10
	 503  34  10
	 2343 12  11
	 .....
  */

 // int max_index=time*50;
  //Contour=new short*[max_index]; // a similarity Contour is unlikely to be larger than this, I am trying to avoid the usage of vector container here...

  continuity_t=new float[time+10]; //continuity feature
  continuity_f=new float[time+10]; //continuity feature
 // principal_frequency=new float[time+10];
  slope=new float[time+10];
  FMc=new float[time+10];
  int *principal_num=new int[time];

  for(int i=0;i<time+10;i++)
  {
	continuity_t[i]=0;
	continuity_f[i]=0;
   //	principal_frequency[i]=0;
	slope[i]=0;
	FMc[i]=0;
  }
  Contour_features=new int*[time*10+50]; // Contour_features of contour
  for (int i=0; i<time*10+50; i++) Contour_features[i]= new int[6];

  for(int i=0; i<time*10+50; i++)
	for(int j=0;j<6;j++)
	   Contour_features[i][j]=0;

  int contour_num=0, contour_points=0;



  try
 {

  for(int i=1;i<time;i++)
  {
	 //str=i;    str+=":";
	 for(int j=4;j<freq-4;j++)
	 {
		if(borders[i][j])
		{
				if(borders[i-1][j])
						{ borders[i][j]=borders[i-1][j]; cnt.time=i; cnt.freq=j; cnt.contour_num=borders[i-1][j]; contours.push_back(cnt); }
				else if(borders[i-1][j+1])
						{borders[i][j]=borders[i-1][j+1]; cnt.time=i; cnt.freq=j; cnt.contour_num=borders[i-1][j+1]; contours.push_back(cnt); }
				else if(borders[i-1][j-1])
						{ borders[i][j]=borders[i-1][j-1]; cnt.time=i; cnt.freq=j; cnt.contour_num=borders[i-1][j-1]; contours.push_back(cnt); }
				else if(borders[i][j+1])
				{
						//we now have to look carefully at the entire vertical contour and find neighbors on the left.
						// the lowest nighboor will be the contour number. otherwise, new number for all the line
						int contour_heigth=0; do{contour_heigth++;}while(borders[i][j+contour_heigth]);
						contour_points+=contour_heigth-1;
						int vertical_neighboors=false;
						for(int h=-1;h<contour_heigth+2;h++)
						{
						  if(borders[i-1][j+h]){vertical_neighboors=borders[i-1][j+h]; break;}
						}
						if(!vertical_neighboors)// no vertical neighboor!
						{
								contour_num++;
								for(int h=0;h<contour_heigth;h++)
								{
								  borders[i][j]=contour_num;  cnt.time=i; cnt.freq=j; cnt.contour_num=contour_num; contours.push_back(cnt);
								  //str+=borders[i][j];
								  j++;
								}

						}
						else // found neighboor!!!
						{
								for(int h=0;h<contour_heigth;h++)
								{
								  borders[i][j]=vertical_neighboors; cnt.time=i; cnt.freq=j; cnt.contour_num=vertical_neighboors; contours.push_back(cnt);
								 // str+=borders[i][j];
								  j++;
								}
						}
				}
				else
				{
				  contour_num++;
				  borders[i][j]=contour_num; cnt.time=i; cnt.freq=j; cnt.contour_num=contour_num; contours.push_back(cnt);
				}
			   // str+=borders[i][j];
				contour_points++;
		}
		//else str+="0";
	 }
	// option->Memo10->Lines->Add(str.SubString(1,120));

  }
   if (contours.size() == 0) {
	  return;
  }
  sort(contours.begin(), contours.end()); // sort by contour_num (see struct definitions)
  /*
   for(int i=0;i<contour_points; i++)
   {
	 str=contours[i].time; str+=" , "; str+=contours[i].freq; str+=" , ";  str+=contours[i].contour_num;
	 option->Memo10->Lines->Add(str);
   }    */

   int i=0, BeginX, BeginY, EndX, EndY, current_contour;
   int num=1;
   do{
	   int freq_max_timing, freq_min_timing;
	   BeginX=DEFAULT_MAX; BeginY=DEFAULT_MAX; EndX=0; EndY=0;
	   current_contour=contours[i].contour_num;
	   do{
			 if(BeginX>contours[i].time) BeginX=contours[i].time;
			 if(EndX<contours[i].time)   EndX=contours[i].time;
			 if(BeginY>contours[i].freq) { BeginY=contours[i].freq; freq_min_timing=i;}  // when in the contour the minimum freq occured
			 if(EndY<contours[i].freq)   { EndY=contours[i].freq;   freq_max_timing=i;}
			 i++;
	   }while(contours[i].contour_num==current_contour && i<contour_points);
	 /*
	   // this optional code will smooth the frequency edges of the contour

	   if(freq_min_timing-freq_max_timing>6)
	   {
		BeginY=( contours[freq_min_timing-1].freq + contours[freq_min_timing-2].freq + contours[freq_min_timing-3].freq  )/3;
		EndY  =( contours[freq_max_timing+1].freq + contours[freq_max_timing+2].freq + contours[freq_max_timing+3].freq  )/3;
	   }
	   else if(freq_max_timing-freq_min_timing>6)
	   {
		BeginY=( contours[freq_min_timing+1].freq + contours[freq_min_timing+2].freq + contours[freq_min_timing+3].freq  )/3;
		EndY  =( contours[freq_max_timing-1].freq + contours[freq_max_timing-2].freq + contours[freq_max_timing-3].freq  )/3;
	   }
	  */
	   Contour_features[num][Contour_length]=(EndX-BeginX);//*(EndX-BeginX)+(EndY-BeginY)*(EndY-BeginY);
	   Contour_features[num][X_start]=BeginX;
	   Contour_features[num][X_end]=EndX;
	   Contour_features[num][Y_start]=BeginY;
	   Contour_features[num][Y_end]=EndY;
	   Contour_features[num][Contour_height]=EndY-BeginY; // always positive
	   if(freq_min_timing>freq_max_timing)Contour_features[num][Contour_height]=-Contour_features[num][Contour_height];
	  /*
	   str=num; str+=" , "; str+=Contour_features[num][X_start];  str+=" , ";  str+=Contour_features[num][X_end]; str+=" , ";
	   str+=Contour_features[num][Y_start]; str+=" , "; str+=Contour_features[num][Y_end];
	   option->Memo10->Lines->Add(str); */
	   num++;
	}while( i<min(contour_points,time*10));

	  //Contour_features[contour_num][Contour_length]=(EndX-BeginX)*(EndX-BeginX)+(EndY-BeginY)*(EndY-BeginY);


  // Continuity measurment
  //int *principal_num=new int[time];
  contour_num=1;
  int sum_length, max_length, principal_freq, current_n, sum_f_length;
  float Slope;
  int num_find=0;
  int previous_principal_num=0, persistant_principal_contour_freq=0;//, persistant_principal_contour_num;

  for (int i=0; i<time ; i++) //for each x cordinate in the edge map
  {   //initialized sum of lengthes of total contours for a certain x value

	  // we want to see if the previous principal contour (if any) still exists in the new time window
	  bool persistant_found=false;
	  if(previous_principal_num)
	  {
		 for(int j=4;j<freq;j++)
		 {
		   if(borders[i][j]==previous_principal_num)
		   {
				persistant_principal_contour_freq=j;
			   // persistant_principal_contour_num=previous_principal_num;
				persistant_found=true;
				break;
		   }
		 }  //if(!persistant_found){}
	  }  // end of if(previous_principal_num)

	  sum_length=0;  sum_f_length=0; max_length=-100;   principal_freq=0; // max_f_length=0;
	  num_find=0; //initialized number of contours for a certain x value
	  for(int j=0;j<freq;j++) if(borders[i][j])
	  {
	   current_n=min(borders[i][j],time*10);
	   sum_length+= Contour_features[current_n][Contour_length];//sum the length of contours at a certain edge corrdinate
	   sum_f_length+= abs(Contour_features[current_n][Contour_height]);

		   // is this the longest contour (including panalty for higher frequencies)?
		   if(max_length<Contour_features[current_n][Contour_length]-(j/option->contour_biasTB->Position))
		   {
			 max_length=Contour_features[current_n][Contour_length]-(j/option->contour_biasTB->Position);
			// max_f_length=abs(Contour_features[current_n][Contour_height]);
			 principal_freq=j;//contours[j].freq;
			 contour_num=current_n;
			 //Slope=-9;
		   }
		   num_find++;//sum the number of contours at a certain edge corrdinate
		}

	  if (num_find==0) //if there were no contours at that edge corrdinate
	  {
		  continuity_t[i]=0;//if there were no contours at that edge corrdinate
		//  principal_frequency[i]=0;
		  principal_num[i]=false;
		  previous_principal_num=false;
		  slope[i]=0;
	  }
	  else
	  {

		continuity_t[i]=(float)sum_length/(float)num_find;
		continuity_f[i]=(float)sum_f_length/(float)num_find;
        if(!slope[i])slope[i]=1;
        if(persistant_found)
        {
          if(persistant_principal_contour_freq!=principal_freq)slope[i]=persistant_principal_contour_freq;
        }
        else previous_principal_num=contour_num;
        if(contour_num!=old_principal_num)slope[i]*=-1; // a flag for a different principal contour here...
        old_principal_num=contour_num;
       // principal_frequency[i]=principal_freq;
        principal_num[i]=contour_num;
      }//if there were contours at that edge corrdinate average their lengths


  }
  /*
  // calculate the FM contours
   bool slope_defined;
   float slp,local_slp;
   for (int i=3; i<time-3 ; i++) //for each x cordinate in the edge map
  {
     slope_defined=true; slp=0;
     for(int j=-2;j<2;j++)
     {
        local_slp=principal_frequency[i+j]-principal_frequency[i+j-1];
        if(abs(local_slp)>5){slope_defined=false; break;}
        else slp+=local_slp;
     }
     if(!slope_defined)FMc[i]=-999;
     else FMc[i]=slp/4.0;
  }

   */
 }
  catch(...)
  {
    option->problem_file=3;
  }

  // 5. free the memory!!

  contours.erase(contours.begin(), contours.end());
  for (int i=0; i<time*10+50; i++) delete[] Contour_features[i];
  delete[] Contour_features;
  delete[] principal_num;

}







//////////////////////////////////////////////////////////////////////
// Extract the features from a binary file and send them to process_sound()
//////////////////////////////////////////////////////////////////////

bool CZT_FeatureCalc::do_binary(AnsiString fileName)
{
  return(0);
}


//////////////////////////////////////////////////////////////////////
// should we save this sound file?
//////////////////////////////////////////////////////////////////////


bool CZT_FeatureCalc::good_sound()
{
  if(
          syllable_number > option->minimum_syllables_per_files &&    //
         ( maxSyllableDur>option->syllable_duration_thresh ||        //
           maxBoutDur>option->bout_duration_thresh                   //
         )
    )
    return(true);
    else return(false);
}


//////////////////////////////////////////////////////////////////////
// The next few fucntions are doing the task of partition sound
// to syllables and calculate mean and variance of their features
//////////////////////////////////////////////////////////////////////





// Create m_variables of feature values from the decoded features in rec.
// note that the encoding in rec is designed to save memory (those are 8 and 16 bit variables)
// When we calculate the actual features, we also perform the pitch calculation since
// what we save is the raw harmonic pitch and mean frequency values -- pitch will be one of them.
void CZT_FeatureCalc::decode_sound(Record &rec)//, int sample)//bool detected, int sample)
{
	   // stage 1: decode rec
	   m_Pitch=rec.pitch;//   option->frequency_scale*((float)rec.pitch +120.0);
	   m_PeakFreq=rec.peakFr;  // option->frequency_scale*((float)rec.peakFr+120.0);
	   m_Entropy= (float)rec.entropy/100.0;
	   m_FM=      (float)rec.fm/10.0;
	   m_amplitude=(float)rec.amplitude/10.0;
	   m_PitchGoodness=(float)rec.pgood/10.0;
	   m_mfa=(float)rec.mean_freq_amp/10.0;
	 //  m_cepst0=  option->frequency_scale*((float)rec.cepst0+120.0);
	   m_AM=(float)rec.am/100.0;
	   if(option->range_x2){
		  m_Pitch*=2.0;
		  m_PeakFreq*=2.0;
		 // m_cepst0*=2.0;
		  }
	   else  if(option->range_x05){
		  m_Pitch*=0.5;
		  m_PeakFreq*=0.5;
		  //m_cepst0*=0.5;
		  }
	  // sound_detect();

	   // stage 2: deside of to accept the cepstrum pitch estimate:
	   if(option->dynamic_pitch_calculation) if(m_PitchGoodness< option->pitch_goodness_thresh   // if the goodness of pitch is miserable
			&& m_Entropy< option->pitch_entropy_thresh  // and the sound is tonal, based on entropy estimates
			|| m_Pitch>option->harmonic_pitch_thresh)    //  or the cepstrum gave an estimate which is higher than 1800Hz... [30 -8   86*pitch  /1800]
										m_Pitch=m_PeakFreq;  // pitch will be the peak frequency rather then cepstrum
}




 int CZT_FeatureCalc::msTable_to_vars(int i)
 {
	int tm;
	DataForm->MyQuery1->RecNo=i;
	tm=DataForm->MyQuery1->Fields->Fields[0]->AsFloat;
	features.second=(tm/10000)%60;
	features.minute=(tm/600000)%60;
	features.hour=(tm/36000000)%24;

   // features.day=option->day;
	m_file_index=DataForm->MyQuery1->Fields->Fields[1]->AsFloat;
	m_amplitude=DataForm->MyQuery1->Fields->Fields[2]->AsFloat;
	m_mfa=DataForm->MyQuery1->Fields->Fields[3]->AsFloat;
	m_Pitch=DataForm->MyQuery1->Fields->Fields[4]->AsFloat;
	m_PeakFreq=DataForm->MyQuery1->Fields->Fields[5]->AsFloat;
	m_FM=DataForm->MyQuery1->Fields->Fields[6]->AsFloat/10.0;
	m_AM=DataForm->MyQuery1->Fields->Fields[7]->AsFloat/100;
	m_PitchGoodness=DataForm->MyQuery1->Fields->Fields[8]->AsFloat;//  /10.0;
	m_Entropy=DataForm->MyQuery1->Fields->Fields[9]->AsFloat/100.0;
	m_principal_contour=120+DataForm->MyQuery1->Fields->Fields[10]->AsFloat;
	m_FMc=120+DataForm->MyQuery1->Fields->Fields[11]->AsFloat;
	m_continuity_t=DataForm->MyQuery1->Fields->Fields[12]->AsFloat/100.0;
	m_continuity_f=DataForm->MyQuery1->Fields->Fields[13]->AsFloat/100.0;
	if(option->dynamic_pitch_calculation) if(m_PitchGoodness< option->pitch_goodness_thresh   // if the goodness of pitch is miserable
		  && m_Entropy< option->pitch_entropy_thresh  // and the sound is tonal, based on entropy estimates
		  || m_Pitch>option->harmonic_pitch_thresh)    //  or the cepstrum gave an estimate which is higher than 1800Hz... [30 -8   86*pitch  /1800]
									  m_Pitch=m_PeakFreq;  // pitch will be the peak frequency rather then cepstrum


	if(option->slave_features)
	{
	   m_s1_amplitude=DataForm->MyQuery1->Fields->Fields[14]->AsFloat;
	   m_s1_entropy=DataForm->MyQuery1->Fields->Fields[15]->AsFloat;

	}
   // option->wave_names[0]=m_file_index;
	// DataForm->GetFileName(option->file_table,m_file_index);
	return(m_file_index);
 }



  int CZT_FeatureCalc::save_and_reset()
 {
		static int syllables_done=0;
		if(syllable_number)save_to_database(0);
		if(syllable_number>0)syllables_done+=syllable_number;
		syllable_number=0;
        reset_syllable_vars();
		return(syllables_done);
 }


//////////////////////////////////////////////////////////////////////
// Is the animal vocalizing now?
//////////////////////////////////////////////////////////////////////

/*
bool CZT_FeatureCalc::sound_detect()
{

	 if(option->hp_amp)  // fine segmentation
	 {
		if(m_amplitude-(4*option->entropy_thresh)>0 && m_mfa>option->amplitude_thresh)
		{sound_detected=true; return(true);}
		else {sound_detected=false; return(false);}
	 }

	 else        // regulare segmentation
	 {

	   if(!m_amplitude)return(false);

	 if( m_amplitude>option->amplitude_thresh )
	 {
		sound_detected=true;
		return(true);
	 }
	 else
	 {
		sound_detected=false;
		return(false);
	 }
	}
}
*/

//////////////////////////////////////////////////////////////////////
// Is the animal vocalizing now?
//////////////////////////////////////////////////////////////////////
bool CZT_FeatureCalc::sound_detect_feature(int currRec) {

 //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
 int feature = option->pri_segmented_feature;
 int secFeature;
 float thresh = option->adjustThreshold(feature, option->pri_thresh_raw, option->frequency, 1); //option->pri_thresh;
 float secThresh; //= option->sec_thresh;
 bool lt = option->reverse_segmentation;
 bool secLt; //= option->sec_lt;
 int secLogic; //= option->sec_logic;
 bool smoothGraph = option->smooth_feature;
 bool adaptThreshold = option->adapt_threshold;
 bool secSegment = option->sec_segmentation;
 float ft, secFt;
 bool cond1, cond2;

 switch(feature) {
		case 0: ft= amplitude[currRec];	break;
		case 1: ft=pitch[currRec]; break;
		case 2:	ft=peakFreq[currRec]; break;
		case 3: ft=pitchGoodness[currRec]; break;
		case 4:	ft=fm[currRec]; break;
		case 5: ft=am[currRec];	break;
		case 6:	ft=entropy[currRec]; break;
		case 7: ft=continuity_t[currRec]; break;
		case 8: ft=continuity_f[currRec]; break;
		default: ft=amplitude[currRec];
	  }
	  if (secSegment) {
		  secFeature = option->sec_segmented_feature;
		  secThresh = option->adjustThreshold(secFeature, option->sec_thresh_raw, option->frequency, 0); //option->sec_thresh;
		  secLt= option->sec_lt;
		  switch(secFeature) {
		    case 0: secFt= amplitude[currRec]; break;
			case 1: secFt=pitch[currRec]; break;
			case 2: secFt=peakFreq[currRec]; break;
			case 3:	secFt=pitchGoodness[currRec]; break;
			case 4: secFt=fm[currRec]; break;
			case 5:	secFt=am[currRec];	break;
			case 6: secFt=entropy[currRec];	break;
			case 7: secFt=continuity_t[currRec];break;
			case 8:	secFt=continuity_f[currRec]; break;
			default:secFt=amplitude[currRec];
		}
		if (!secLt) cond2 = secFt > secThresh;
		else  cond2 = secFt < secThresh;
	  }
	  float diff; // = hp_small[currRec] - hp_big[currRec];
	  if (!smoothGraph && !adaptThreshold) {
	  //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
		if (!ft) return(false);
		if (!lt) diff = ft-thresh;
		else diff = thresh-ft;
		} else if (smoothGraph && !adaptThreshold) {
		if (!lt) diff = hp_small[currRec]-thresh;
		else diff = thresh-hp_small[currRec];
		} else if (!smoothGraph && adaptThreshold) {
		if (!lt) diff = ft - hp_big[currRec];
		else diff = hp_big[currRec] - ft;
		} else if (smoothGraph && adaptThreshold) {
		if (!lt) diff = hp_small[currRec] - hp_big[currRec];
		else diff = hp_big[currRec] - hp_small[currRec];
	   }

	   cond1 = diff > 0;
	   if (!secSegment) {
		 if (cond1) sound_detected=true;
		   else sound_detected=false;
	   }  else {
		  secLogic = option->sec_logic;
		  if (cond1 && cond2 && !secLogic) sound_detected=true;
		  else if ( (cond1 || cond2) && secLogic) sound_detected=true;
		  else sound_detected=false;
	   }

	   //myfile <<__FILE__<<" : "<<__LINE__<<"\n"; //myfile.flush();
	   return sound_detected;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Parsing the sound to syllable units happens here
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CZT_FeatureCalc::parse_sound(int current_rec)
{
	   static int stop_duration=0;
	  // float thresh = option->amplitude_thresh;
	   // there are alternative methods to segment, outcome is experessed the sound_deteced bool variable
	   /*if (!option->create_feature_arrays)
		sound_detected=sound_detect();//this is done "live"
	   else if(option->custom_parse) {// unique case for using slave channels features to segment
		thresh = option->HP_big - option->HP_small;   //other way around?
		sound_detected=sound_detect_feature(option->segmentation_feature, thresh, option->reverse_segmentation);
	  }
 // maybe this is the problem, all this part is obsolite since we always create feature array and this is dones off line anyhow...

	   else // this is the default case:   */
	   //if (option->create_feature_arrays) {
		  //sound_detected=signal[current_rec]; // this is the usual case, using the signal array for segmenting
		   m_PeakFreq=peakFreq[current_rec];
		   m_mfa=mfa[current_rec];
		   m_amplitude=amplitude[current_rec];
		   m_Entropy=entropy[current_rec];
		   m_Pitch=pitch[current_rec];
		   m_PitchGoodness=pitchGoodness[current_rec];
		   m_FM=fm[current_rec];
		   m_AM=am[current_rec];
		   m_continuity_t=continuity_t[current_rec];
		   m_continuity_f=continuity_f[current_rec];

		   if (sound_detect_feature(current_rec) ) {
				signal[current_rec]=true;
				stop_duration=0;
				sound_detected=true; //Ofer Q
		  }
		  else { // give some innertia to sound duration...

		  if(stop_duration <= option->min_stop_duration) signal[current_rec]=true;
		  else signal[current_rec]=false;
		  stop_duration++;
		  sound_detected=false; //Ofer Q
		  }
	  // }

	 //  else sound_detected=sound_detect_feature(current_rec);


	   if(sound_detected)//(sound_detected)  // we are inside a syllable -- add features
	   {
		   if(syllDur==1) features.start=current_rec;  // this is the beginning of a syllable (start_on)
		   syllDur++;  stopDur=0;
		   if(option->simple_pitch_averaging) sum_pitch+=m_Pitch;
		   else
		   {
				  sum_pitch_simp+=m_Pitch;
				  tmp_sum=m_PitchGoodness;
				  sum_pitch+=m_Pitch*tmp_sum;
				  sum_factors+=tmp_sum;
		   }
		   sum_principal_contour+=m_principal_contour;
		   sum_entropy+=m_Entropy;
		   sum_fm+=m_FM;
		   sum_pgood+=m_PitchGoodness;
		   sum_am+=m_AM;
		   sum_peakFreq+=m_PeakFreq;
		   sum_amplitude+=m_amplitude;
		   sum_continuity_t+=m_continuity_t;
		   sum_continuity_f+=m_continuity_f;

		   if(min_pitch>m_Pitch && min_pitch>0)min_pitch=m_Pitch;
		   if(min_principal_contour>m_principal_contour)min_principal_contour=m_principal_contour;
		   if(min_FM>m_FM)min_FM=m_FM;
		   if(min_entropy>m_Entropy)min_entropy=m_Entropy;
		   if(min_peakFr>m_PeakFreq)min_peakFr=m_PeakFreq;
		   if(min_slope>m_FMc)min_slope=m_FMc;

		   if(max_pitch<m_Pitch)max_pitch=m_Pitch;
		   if(max_principal_contour<m_principal_contour)max_principal_contour=m_principal_contour;
		   if(max_FM<m_FM)max_FM=m_FM;
		   if(max_entropy<m_Entropy)max_entropy=m_Entropy;
		   if(max_peakFr<m_PeakFreq)max_peakFr=m_PeakFreq;
		   if(max_slope<m_FMc)max_slope=m_FMc;
		   if(max_amplitude<m_amplitude)max_amplitude=m_amplitude;
		   if(max_continuity_f<m_continuity_f)max_continuity_f=m_continuity_f;
		   if(max_continuity_t<m_continuity_t)max_continuity_t=m_continuity_t;
		   if(m_FMc>-100)sum_slope+=m_FMc;

		   SS_pitch+=m_Pitch*m_Pitch;
		   SS_entropy+=m_Entropy*m_Entropy;
		   SS_fm+=m_FM*m_FM;
		   SS_pgood+=m_PitchGoodness*m_PitchGoodness;
		   SS_am+=m_AM*m_AM;
		   SS_peakFreq+=m_PeakFreq*m_PeakFreq;
		   SS_amplitude+=m_amplitude*m_amplitude;

		   if(option->slave_features)// calculate syll table values for brain tables
		   {
			 sum_s1_amp+=m_s1_amplitude; sum_s2_amp+=m_s2_amplitude; sum_s3_amp+=m_s3_amplitude; sum_s4_amp+=m_s4_amplitude; sum_s5_amp+=m_s5_amplitude;
			 sum_s1_ent+=m_s1_entropy; sum_s2_ent+=m_s2_entropy; sum_s3_ent+=m_s3_entropy; sum_s4_ent+=m_s4_entropy; sum_s5_ent+=m_s5_entropy;
			 sum_s1_peak_frequency+=m_s1_peak_frequency; sum_s2_peak_frequency+=m_s2_peak_frequency; sum_s3_peak_frequency+=m_s3_peak_frequency;
			 sum_s4_peak_frequency+=m_s4_peak_frequency; sum_s5_peak_frequency+=m_s5_peak_frequency;
			 sum_s1_peak_power+=m_s1_peak_power; sum_s2_peak_power+=m_s2_peak_power; sum_s3_peak_power+=m_s3_peak_power;
			 sum_s4_peak_power+=m_s4_peak_power; sum_s5_peak_power+=m_s5_peak_power;
			 sum_s1_highpass_power+=m_s1_highpass_power; sum_s2_highpass_power+=m_s2_highpass_power; sum_s3_highpass_power+=m_s3_highpass_power;
			 sum_s4_highpass_power+=m_s4_highpass_power; sum_s5_highpass_power+=m_s5_highpass_power;
		   }

	   } // end of sound detected

	   else  // syllable is over?
	   {
		  if(stopDur>option->min_stop_duration         // stop was long enough to decide that syllable ended
			 && syllDur>option->min_syllable_duration) // syllable is long enough to count
		  {       // wrap it up -- syllable has ended
			 Add_syll_to_features_array();
		  } // end of "syllable is over"

		  // reset variables (note that we keep reseting every frame durign the stop)
		  if(stopDur>option->min_stop_duration)
		  {
			 if(maxSyllableDur<syllDur)maxSyllableDur=syllDur;
			 syllDur=0; sum_pitch=0; sum_principal_contour=0; sum_pitch_simp=0; sum_entropy=0;
			 sum_fm=0; sum_pgood=0; sum_amplitude=0;  sum_continuity_t=0; sum_continuity_f=0; sum_slope=0;
			 sum_am=0; sum_peakFreq=0;
			 sum_s1_amp=0; sum_s2_amp=0; sum_s3_amp=0; sum_s4_amp=0; sum_s1_ent=0; sum_s2_ent=0; sum_s3_ent=0; sum_s4_ent=0;
			 SS_pitch=0; SS_entropy=0; SS_fm=0; SS_pgood=0; SS_am=0; SS_peakFreq=0;  SS_amplitude=0;
			 min_pitch=99999; min_principal_contour=99999; min_FM=99999; min_entropy=99999; min_peakFr=99999; min_slope=99999;
			 max_pitch=0; max_principal_contour=0; max_FM=0; max_entropy=-99999; max_peakFr=0;
			 max_slope=-99999; max_amplitude=0; max_continuity_f=0; max_continuity_t=0;
			 chan_index=0;
			 for(int i=0;i<2;i++)for(int j=0;j<11;j++)mean_chan_amp[i][j]=0;
		  }

		  stopDur++;
	  } // end of else (no sound detected)
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This will allow us to segment the raw features table based on any arbitrary feature, including slave channels
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool  CZT_FeatureCalc::custom_segmentation(int feature, int thresh)
{
	  float ft;
	  switch(feature)
	  {
		case 0: ft=m_amplitude; break;
		case 1: ft=(m_Entropy); break;
		case 2: ft=m_Pitch; break;
		case 3: ft=m_FM; break;
		case 4: ft=m_PitchGoodness; break;
	  }
	  if(option->reverse_segmentation) ft*=-1; // segment the low rather then high values

	   if (abs(ft) < thresh)
		return (false);
	   else
		return (true);
}


void CZT_FeatureCalc::Add_syll_to_features_array()
{
		features.duration=syllDur;
		if(syllDur<2)syllDur=2; // prevent divition by zero, but this should not happen anyway...
	   // first handle pitch...
	   if(option->simple_pitch_averaging)
	   {
			features.mean_pitch=sum_pitch/syllDur;
			features.var_pitch=0.001*(SS_pitch-(1.0/syllDur)*(sum_pitch*sum_pitch))/(syllDur-1);
	   }
	   else
	   {
		   if(!sum_factors)sum_factors=1;
		   features.mean_pitch=sum_pitch/sum_factors;
		   features.var_pitch=0.001*(SS_pitch-(1.0/syllDur)*(sum_pitch_simp*sum_pitch_simp))/(syllDur-1);
		   sum_factors=0;
	   }
	   // now do the rest of the features
	   // factors are used for the 16bit encoding of floating point values
	   features.mean_principal_contour=option->frequency_scale*sum_principal_contour/syllDur;
	   features.mean_FM=10*sum_fm/syllDur;
	   features.mean_entropy=100*sum_entropy/syllDur;
	   features.mean_pGood=10*sum_pgood/syllDur;
	   features.mean_amplitude=10*sum_amplitude/syllDur;
	   features.mean_am=100000*SS_am/syllDur; // we use the squere AM as our feature...
	   features.mean_peakFr=sum_peakFreq/syllDur;
	   features.mean_continuity_f=option->frequency_scale*100* sum_continuity_f/syllDur;
	   features.mean_continuity_t=100* sum_continuity_t/syllDur;
	   features.mean_slope=100* sum_slope/syllDur;
	   features.min_pitch=min_pitch;
	   features.min_principal_contour=option->frequency_scale*min_principal_contour;
	   features.min_FM = 10*min_FM;
	   features.min_entropy = 100*min_entropy;
	   features.min_peakFr = min_peakFr;
	   features.min_slope = 100*min_slope;
	   features.max_pitch = max_pitch;
	   features.max_principal_contour=option->frequency_scale*max_principal_contour;
	   features.max_FM = 10*max_FM;
	   features.max_entropy = 100*max_entropy;
	   features.max_peakFr = max_peakFr;
	   features.max_slope = 100*max_slope;
	   features.max_amplitude = max_amplitude;
	   features.max_continuity_f = option->frequency_scale*100*max_continuity_f;
	   features.max_continuity_t = 100*max_continuity_t;
	   features.var_FM=      (SS_fm-(1.0/syllDur)*sum_fm*sum_fm)/(syllDur-1);
	   features.var_entropy= 100*(SS_entropy-(1.0/syllDur)*sum_entropy*sum_entropy)/(syllDur-1);
	   features.var_pGood=   0.01*(SS_pgood-(1.0/syllDur)*sum_pgood*sum_pgood)/(syllDur-1);
	   features.var_amplitude=   (SS_amplitude-(1.0/syllDur)*sum_amplitude*sum_amplitude)/(syllDur-1);
	   features.var_am=100000*(SS_am-(1.0/syllDur)*sum_am*sum_am)/(syllDur-1);
	   features.var_peakFr=  0.001*(SS_peakFreq-(1.0/syllDur)*sum_peakFreq*sum_peakFreq)/(syllDur-1);
	   mem_recs.push_back(features);//[syllable_number]=features;

	   if(option->slave_features) //can be relpaced by switch with no breaks (that's why the odd order)
	   {
		 features.s5_amplitude=sum_s5_amp/syllDur;
		 features.s5_entropy=sum_s5_ent/syllDur;
		 features.s5_peak_frequency=sum_s5_peak_frequency/syllDur;
		 features.s5_peak_power=sum_s5_peak_power/syllDur;
		 features.s5_highpass_power=sum_s5_highpass_power/syllDur;

		 features.s4_amplitude=sum_s4_amp/syllDur;
		 features.s4_entropy=sum_s4_ent/syllDur;
		 features.s4_peak_frequency=sum_s4_peak_frequency/syllDur;
		 features.s4_peak_power=sum_s4_peak_power/syllDur;
		 features.s4_highpass_power=sum_s4_highpass_power/syllDur;

		 features.s3_amplitude=sum_s3_amp/syllDur;
		 features.s3_entropy=sum_s3_ent/syllDur;
		 features.s3_peak_frequency=sum_s3_peak_frequency/syllDur;
		 features.s3_peak_power=sum_s3_peak_power/syllDur;
		 features.s3_highpass_power=sum_s3_highpass_power/syllDur;

		 features.s2_amplitude=sum_s2_amp/syllDur;
		 features.s2_entropy=sum_s2_ent/syllDur;
		 features.s2_peak_frequency=sum_s2_peak_frequency/syllDur;
		 features.s2_peak_power=sum_s2_peak_power/syllDur;
		 features.s2_highpass_power=sum_s2_highpass_power/syllDur;

		 features.s1_amplitude=sum_s1_amp/syllDur;
		 features.s1_entropy=sum_s1_ent/syllDur;
		 features.s1_peak_frequency=sum_s1_peak_frequency/syllDur;
		 features.s1_peak_power=sum_s1_peak_power/syllDur;
		 features.s1_highpass_power=sum_s1_highpass_power/syllDur;

	   }
	   syllable_number++;

}


////////////////////////////////////////////////////////////////////////////////////////


void  CZT_FeatureCalc::parse_bouts()
{
	  for(int i=1;i<recnum;i++)bout_signal[i]=false;

	  int i=0, stoped=0;
	  do{
		   if(signal[i])
		   {
				bout_signal[i]=true;
				if(stoped>0 && stoped<option->stop_duration_thresh) for(int j=stoped;j>0;j--) bout_signal[i-j]=true;
				stoped=0;
		   }
		   else stoped++;
		   i++;
	  }while(i<recnum);

	  boutDur=0;  maxBoutDur=0;
	  for(int i=0;i<recnum;i++)
	  {
		 if(bout_signal[i])boutDur++;
		 else
		 {
			 if(boutDur>maxBoutDur)maxBoutDur=boutDur;
			 boutDur=0;
		 }
	  }
}



//////////////////////////////////////////////////////////////////////
//  add slice to the image with derivatives and features
//////////////////////////////////////////////////////////////////////

void  CZT_FeatureCalc::RenderImageSlice(int itr)
{
	 //if(!m_bRenderImage) return;
	 //myfile<<__LINE__<<endl;
	 int nSpectral;
	 float pitch_factor=option->frequency_scale;
	 if(option->range_x2) pitch_factor*=2;
	 else if(option->range_x05) pitch_factor/=2;
	 CZT_FeatImgSlice* pCurrentSlice = new CZT_FeatImgSlice;
	 CZT_FeatImgSlice* pCurrentSonoSlice = new CZT_FeatImgSlice;
	 CZT_FeatImgSlice* pCurrentContourSlice = new CZT_FeatImgSlice;
	 float x=max_drv/2000000.25f;

	 // Add acoustics to the slice
	 pCurrentSlice->pitchGoodness=short(pitchGoodness[itr]/15);
	 pCurrentSonoSlice->pitchGoodness=pCurrentSlice->pitchGoodness;
	 pCurrentContourSlice->pitchGoodness=pCurrentSlice->pitchGoodness;
	 //short(10*(log10(pitchGoodness[itr]+1)*10-20));//
	 pCurrentSlice->entropy=short(200+(entropy[itr]*25));
	 pCurrentSonoSlice->entropy=pCurrentSlice->entropy;
	 pCurrentContourSlice->entropy=pCurrentSlice->entropy;

	 pCurrentSlice->fm=short(50+(fm[itr]*1.5));
	 pCurrentSonoSlice->fm=pCurrentSlice->fm;
	 pCurrentContourSlice->fm=pCurrentSlice->fm;

	 pCurrentSlice->peakfreq=(peakFreq[itr]/pitch_factor);
	 pCurrentSonoSlice->peakfreq=pCurrentSlice->peakfreq;
	 pCurrentContourSlice->peakfreq=pCurrentSlice->peakfreq;

	 pCurrentSlice->pitch=short(pitch[itr]/pitch_factor);
	 pCurrentSonoSlice->pitch=pCurrentSlice->pitch;
	 pCurrentContourSlice->pitch=pCurrentSlice->pitch;

	 pCurrentSlice->am=short(100+am[itr]*100);
	 pCurrentSonoSlice->am=pCurrentSlice->am;
	 pCurrentContourSlice->am=pCurrentSlice->am;

	 pCurrentSlice->DerivPow=short(amplitude[itr])*2;
	 pCurrentSonoSlice->DerivPow=pCurrentSlice->DerivPow;
	 pCurrentContourSlice->DerivPow=pCurrentSlice->DerivPow;

	 pCurrentSlice->mfa=short(mfa[itr]*2);
	 pCurrentSonoSlice->mfa=pCurrentSlice->mfa;
	 pCurrentContourSlice->mfa=pCurrentSlice->mfa;

	 //pCurrentSlice->mfa=short(mfa[itr]/pitch_factor);
	 pCurrentSlice->cepst0=short(pitch[itr])/10;
	 pCurrentSonoSlice->cepst0=pCurrentSlice->cepst0;
	 pCurrentContourSlice->cepst0=pCurrentSlice->cepst0;

	 pCurrentSlice->continuity_t=short(continuity_t[itr]*1.5);
	 pCurrentSonoSlice->continuity_t=pCurrentSlice->continuity_t;
	 pCurrentContourSlice->continuity_t=pCurrentSlice->continuity_t;

	 pCurrentSlice->continuity_f=short(continuity_f[itr]*3);
	 pCurrentSonoSlice->continuity_f=pCurrentSlice->continuity_f;
	 pCurrentContourSlice->continuity_f=pCurrentSlice->continuity_f;
	 int feature = option->pri_segmented_feature;
	 //myfile<<__LINE__<<endl;
  /*
	 if(hp_big[itr]<0)hp_big[itr]=0;
	 if(hp_small[itr]<0)hp_small[itr]=0;
	 if(hp_diff[itr]<0)hp_diff[itr]=0;

   */

	 switch(feature)
	  {
		case 0:
			pCurrentSlice->hp_big=Min(short(hp_big[itr]*2),256);
			pCurrentSlice->hp_small=Min(short(hp_small[itr]*2),256);
			pCurrentSlice->hp_diff=Min(short(hp_diff[itr]*2),256);
			break;
		case 1:
			pCurrentSlice->hp_big=Min(short(hp_big[itr]/pitch_factor),256);
			pCurrentSlice->hp_small=Min(short(hp_small[itr]/pitch_factor),256);
			pCurrentSlice->hp_diff=Min(short(hp_diff[itr]/pitch_factor),256);
			break;
		case 2:
			pCurrentSlice->hp_big=Min(short(hp_big[itr]/pitch_factor),256);
			pCurrentSlice->hp_small=Min(short(hp_small[itr]/pitch_factor),256);
			pCurrentSlice->hp_diff=Min(short(hp_diff[itr]/pitch_factor),256);
			break;
		case 3:
			pCurrentSlice->hp_big=Min(short(hp_big[itr]/15),256);
			pCurrentSlice->hp_small=Min(short(hp_small[itr]/15),256);
			pCurrentSlice->hp_diff=Min(short(hp_diff[itr]/15),256);
			break;
		case 4:
			pCurrentSlice->hp_big=Min(short(50+hp_big[itr]*1.5),256);
			pCurrentSlice->hp_small=Min(short(50+hp_small[itr]*1.5),256);
			pCurrentSlice->hp_diff=Min(short(50+hp_diff[itr]*1.5),256);
			break;
		case 5:
			pCurrentSlice->hp_big=Min(short(100+hp_big[itr]*100),256);
			pCurrentSlice->hp_small=Min(short(100+hp_small[itr]*100),256);
			pCurrentSlice->hp_diff=Min(short(100+hp_diff[itr]*100),256);
			break;
		case 6:
			pCurrentSlice->hp_big=Min(short(200+hp_big[itr]*25),256);
			pCurrentSlice->hp_small=Min(short(200+hp_small[itr]*25),256);
			pCurrentSlice->hp_diff=Min(short(200+hp_diff[itr]*25),256);
			break;
		case 7:
			pCurrentSlice->hp_big=Min(short(hp_big[itr]*1.5),256);
			pCurrentSlice->hp_small=Min(short(hp_small[itr]*1.5),256);
			pCurrentSlice->hp_diff=Min(short(hp_diff[itr]*1.5),256);
			break;
		case 8:
			pCurrentSlice->hp_big=Min(short(hp_big[itr]*3),256);
			pCurrentSlice->hp_small=Min(short(hp_small[itr]*3),256);
			pCurrentSlice->hp_diff=Min(short(hp_diff[itr]*3),256);
			break;
		default:
			pCurrentSlice->hp_big=Min(short(hp_big[itr]*2),256);
			pCurrentSlice->hp_small=Min(short(hp_small[itr]*2),256);
			pCurrentSlice->hp_diff=Min(short(hp_diff[itr]*3),256);
	  }
	  //myfile<<__LINE__<<endl;

	  pCurrentSonoSlice->hp_big=pCurrentSlice->hp_big;
	  pCurrentContourSlice->hp_big=pCurrentSlice->hp_big;

	  pCurrentSonoSlice->hp_small=pCurrentSlice->hp_small;
	  pCurrentContourSlice->hp_small=pCurrentSlice->hp_small;

	  pCurrentSonoSlice->hp_diff=pCurrentSlice->hp_diff;
	  pCurrentContourSlice->hp_diff=pCurrentSlice->hp_diff;


	 pCurrentSlice->principal_frequency=short(principal_frequency[itr]);
	 pCurrentSonoSlice->principal_frequency=pCurrentSlice->principal_frequency;
	 pCurrentContourSlice->principal_frequency=pCurrentSlice->principal_frequency;

	 pCurrentSlice->slope=short(FMc[itr])/2-50;//(100+(slope[itr]*50));
	 pCurrentSonoSlice->slope=pCurrentSlice->slope;
	 pCurrentContourSlice->slope=pCurrentSlice->slope;
	 //myfile<<__LINE__<<endl;
	 bool sound_draw=sound_detected;
	 if(!sound_detected && stopDur<=option->min_stop_duration)sound_draw=true;

	 pCurrentSlice->Sound=sound_draw;
	 pCurrentSonoSlice->Sound=pCurrentSlice->Sound;
	 pCurrentContourSlice->Sound=pCurrentSlice->Sound;

	 pCurrentSlice->Bout= bout_signal[itr]? 1:0;
	 pCurrentSonoSlice->Bout=pCurrentSlice->Bout;
	 pCurrentContourSlice->Bout=pCurrentSlice->Bout;

 // done features, now spectral levels for deriv, sono and contours:
		  //myfile<<__LINE__<<endl;


	 // spectral derivatives
	 for(int i=1;i<256;i++)
	 {
		  if (_isnan(Deriv[itr][i]*x) ||
				!_finite(Deriv[itr][i]*x) ||
				Deriv[itr][i]*x != Deriv[itr][i]*x) {
			nSpectral = 0;
		  }
		  else nSpectral=int(Deriv[itr][i]*x);
		  if(nSpectral>32766) nSpectral=32766;
		  if(nSpectral<-32766) nSpectral=-32766;
		  pCurrentSlice->spectral[i] =(short)nSpectral;
	 }

		  //myfile<<__LINE__<<endl;
	 m_Image.AddSlice(pCurrentSlice);
		  //myfile<<__LINE__<<endl;

	 //  sonogram:
	 for(int i=1;i<256;i++)
	 {

	  //myfile<<__LINE__<<endl;
			nSpectral=int(64000-Sono[itr][i]);
		   // myfile<<__LINE__<<endl;
			if(nSpectral>32766) nSpectral=32766;
			//myfile<<__LINE__<<endl;
			if(nSpectral<-32766) nSpectral=-32766;
			//myfile<<__LINE__<<endl;
			pCurrentSonoSlice->spectral[i] =(short)nSpectral;
		 //	myfile<<__LINE__<<endl;
	 }
		  //myfile<<__LINE__<<endl;

	 m_sonogram.AddSlice(pCurrentSonoSlice);
		   //myfile<<__LINE__<<endl;

	 //  contours
	  for(int i=1;i<256;i++)
	 {
	  //myfile<<__LINE__<<endl;
		if(Borders[itr][i])nSpectral=-32766;
		else nSpectral=32766;
		//myfile<<__LINE__<<endl;
		pCurrentContourSlice->spectral[i] =(short)nSpectral;
		//myfile<<__LINE__<<endl;
	 }
		  //myfile<<__LINE__<<endl;

	 m_contours.AddSlice(pCurrentContourSlice);
		  //myfile<<__LINE__<<endl;


}



//////////////////////////////////////////////////////////////////////
//  same as before, but for contours
//////////////////////////////////////////////////////////////////////

/*
void  CZT_FeatureCalc::RenderSonogramSlice(int itr)
{
	 int nSpectral;
	 CZT_FeatImgSlice* pCurrentSlice = new CZT_FeatImgSlice;

	 for(int i=1;i<256;i++)
	 {
		  // the slice needs some of this goodness..


		if(Borders[itr][i])nSpectral=-32766;
		else nSpectral=32766;
		pCurrentSlice->spectral[i] =(short)nSpectral;
	 }

	 pCurrentSlice->pitchGoodness=short(pitchGoodness[itr]/15);
	 pCurrentSlice->entropy=short(200+(entropy[itr]*25));
	 pCurrentSlice->fm=short(30+fm[itr]*1.5);
	 pCurrentSlice->peakfreq=(peakFreq[itr]/option->frequency_scale);
	 pCurrentSlice->pitch=short(pitch[itr]/option->frequency_scale);
	 pCurrentSlice->am=short(100+am[itr]*100);
	 pCurrentSlice->DerivPow=short(amplitude[itr])*2;
	 pCurrentSlice->mfa=short(mfa[itr])*2;
	 pCurrentSlice->cepst0=short(0);

	 pCurrentSlice->cepst0=short(cepst0[itr])/10;
	 pCurrentSlice->continuity_t=short(continuity_t[itr]*1.5);
	 pCurrentSlice->continuity_f=short(continuity_f[itr]*3);
	 pCurrentSlice->principal_frequency=short(principal_frequency[itr]);
	 pCurrentSlice->slope=short(15*FMc[itr]);
	 //pCurrentSlice->slope=short(slope[itr]);//(100+(slope[itr]*50));


	 bool sound_draw=0;//sound_detected;
	 //if(!sound_detected && stopDur<=option->min_stop_duration)sound_draw=true;
	 pCurrentSlice->Sound=sound_draw;
	 pCurrentSlice->Bout=0;// boutDur? 1:0;
	 m_sonogram.AddSlice(pCurrentSlice);

}

 */
AnsiString CZT_FeatureCalc::set_destination(int index)
{
	AnsiString destination_folder=option->moveTo_sound_folders[index];
	if(!DirectoryExists(destination_folder))if(!ForceDirectories(destination_folder))
	{
			  Application->MessageBox(L"Warning: cannot create folder! sound files will be saved in c:\rescue", NULL, MB_OK);
			  CreateDir("c:\\rescue\\");
			  destination_folder="c:\\rescue\\";
	}
	return(destination_folder);
}




AnsiString CZT_FeatureCalc::set_file_name(int index, AnsiString old_file_name)// index is the slave index (zero for sound channel)
{
   AnsiString new_name;
   // option 1: keep file name as is
   if(option->keep_file_name)
   {
	  new_name=ExtractFileName(option->wave_names[index]);
	  current_file_name=new_name;
	  return(new_name);
   }
   else //option 2: change file name to include proper serial number (and birdname)...
   {
	 if(index){new_name="S"; new_name+=index; new_name+="~";}  // if slave channel, call it S~...
	 else new_name="";
	 new_name+=features.bird_ID; new_name+="_"; new_name+=features.serial_number; new_name+="_";
	 new_name+=(int)features.month; new_name+="_";  new_name+=(int)features.day;  new_name+="_"; new_name+=(int)features.hour;
	 new_name+="_"; new_name+=(int)features.minute; new_name+="_"; new_name+=(int)features.second; new_name+=".wav";
	 current_file_name=new_name;
	 return(new_name);
   }

}




//////////////////////////////////////////////////////////////////////
//  manage the saving renaming and moving of all data: sound files, features files and database
// what about the ms table? that's elsewhere....
//////////////////////////////////////////////////////////////////////
void  CZT_FeatureCalc::handle_wave_files(int bird)
{
	AnsiString folder, new_file_name;
   // features=mem_recs[0];
	// we first move the sound file to a folder destination
  if(Slaves) for(int i=0;i<=Slaves;i++)
   {
	  folder=set_destination(i);
	  new_file_name=set_file_name(i, option->wave_names[i]);
	  newFile=folder+new_file_name;
	  file_names[i]=newFile;
	  move_the_wave(option->wave_names[i],newFile);
   }
   else
   {
	  folder=set_destination(bird);
	  new_file_name=set_file_name(0, option->wave_names[0]);
	  newFile=folder+new_file_name;
	  move_the_wave(option->wave_names[0],newFile);
  }
}



void  CZT_FeatureCalc::move_the_wave(AnsiString old_name,AnsiString new_name)
{
	 static int problems=0;
	 if(option->check_overwrite && FileExists(new_name))
	 {
	   option->halt=true; //DeleteFile(new_name);
	   option->Message="The destination folder already contains a sound file with the same name, you might be saving the same task twice...  -- process will now halt";
	   return;
	 }
	 else
	 {
		if(!RenameFile(old_name, new_name))
		{
			  // try to get rid of this problematic file
			  if(!DeleteFile(old_name))
			  {
				option->DeleteFile_failed=true;
				option->File_to_delete=old_name;  // if failed, we will try again later...
			  }
			  option->problem_file=new_name;
			  problems++;
			  if(problems==1)first = time(NULL);  /* Gets system time for the first case of file handling problem */
			  if(option->problems_thresh && problems==option->problems_thresh) // one problems too many?
			  {
				  second = time(NULL); /* Gets system time again */
				  if(difftime(second,first)<600) // lost 10 files in 10 minutes -- that's too much!
				  {
				   option->halt=true;
				   option->Message="Sound Analysis is experiencing problems saving sound files. Please check that the disk is not full, network connection is not down (if you save remotely) and then restart the engine. In the meanwhile, the recording continue localy, and analysis process is now paused";
				  }
				  else problems=0;
			  }
		}
	  }

	 if(!FileExists(new_name)) // we failed to move the new file into destination
	 {
	   problems++;
	/*  option->halt=true; // DeleteFile(old_name);
	   option->Message="could not copy the sound file into its destination -- process will now halt";  */
	 }
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




void  CZT_FeatureCalc::save_to_database(int bird)
{
  if(option->move_sound)
  {
		handle_wave_files(bird);  // rename and move wave files to their final destination
  }
  else
  {
		newFile=option->wave_names[0];// the new file is the same as the original, in this case.
		current_file_name=ExtractFileName(option->wave_names[0]);
  }


  features=mem_recs[0]; // this is a pointer for the first syllable features
  if(option->saveRawFile)
  {
	 try{
				save_ms_table(Slaves);  // first save the ms table if so indicated
		}
		catch(...)
		{
		   option->problem_file=4;
		}
  }

  if(option->saveRecFiles) //  save syllables tables if so indicated
  {



   try{

		if(syllable_number>0)for(int i=0;i<syllable_number;i++)
		{
			 option->syllCounter++;
			 features=mem_recs[i];
			 if(option->brain_mode)Add_Brain_Record();
			 else Add_Record();
		}
	 }
	 catch(...)
	 {
		option->problem_file=5;
	 }
  }
   // handle singing activity statistics

  try{
   int bird_num=option->select_table;
   for(int i=0;i<syllable_number;i++)
	{
		 features=mem_recs[i];
		 float duration=(float)features.duration*(float)option->advanceTb->Value;
		 float fm=(float)features.mean_FM/10.0;
		 if(bird_num)
		 {
		   if(duration<100)
		   {
			 if(fm<15)option->types[bird_num][0]++;
			 else if(features.mean_pitch>3000)option->types[bird_num][1]++;
			 else option->types[bird_num][2]++;
		   }
		   else
		   {
			 if(fm<15)option->types[bird_num][3]+=2;
			 else if(features.mean_pitch>2000)option->types[bird_num][4]+=2;
			 else option->types[bird_num][5]+=2;
		   }
	   }
	}
   }
   catch(...)
   {
	  option->problem_file=6;
   }

  // finally, erase the syllable data vector
  try{
  mem_recs.erase(mem_recs.begin(), mem_recs.end());
  }
  catch(...)
  {
	 option->problem_file=7;
  }
 // recs_exsist=false;


}






//////////////////////////////////////////////////////////////////////
//  add record to the database
//////////////////////////////////////////////////////////////////////


void  CZT_FeatureCalc::Add_Record()
{
		  // we now categorize each syllable to types, for the statistics histogram:

		 float duration=(float)features.duration*(float)option->advanceTb->Value;
		 float starton=(float)features.start*(float)option->advanceTb->Value;
		 float fm=(float)features.mean_FM/10.0;

		 // okay, add the record...
		DataForm->Insert_syllable_record(
				option->syllable_table_name,
				(double)features.serial_number,
				 //(int)
				 features.bird_ID,
				 starton,
				 duration,
				(float)features.mean_pitch,
				(float)features.mean_principal_contour,
				 fm,
				(float)features.mean_entropy/100.0,
				(float)features.mean_pGood/10.0,
				(float)features.mean_peakFr,
				(float)features.mean_am/100000.0,
				(float)features.mean_amplitude/10.0,
				(float)features.mean_continuity_f/100.0,
				(float)features.mean_continuity_t/100.0,
				(float)features.mean_slope/100.0,

				(float)features.min_pitch,
				(float)features.min_principal_contour,
				(float)features.min_FM/10.0,
				(float)features.min_entropy/100.0,
				(float)features.min_peakFr,
				(float)features.min_slope/100.0,

				(float)features.max_pitch,
				(float)features.max_principal_contour,
				(float)features.max_FM/10.0,
				(float)features.max_entropy/100.0,
				(float)features.max_peakFr,
				(float)features.max_slope/100.0,
				(float)features.max_amplitude,
				(float)features.max_continuity_f/100.0,
				(float)features.max_continuity_t/100.0,

				1000.0*(float)features.var_pitch,
				(float)features.var_FM,
				0.01*(float)features.var_entropy,
				100.0*(float)features.var_pGood,
				1000.0*(float)features.var_peakFr,
				(float)features.var_am/100000.0,

				 // finally, we add the time stamps
				(int)features.month,
				(int)features.day,
				(int)features.hour,
				(int)features.minute,
				(int)features.second,
				 current_file_name,
				 comments
		);

}




void  CZT_FeatureCalc::Add_Brain_Record()
{
		  // we now categorize each syllable to types, for the statistics histogram:
	   /*
		 float duration=(float)features.duration*(float)option->advanceTb->Value;
		 float starton=(float)features.start*(float)option->advanceTb->Value;
		 float fm=(float)features.mean_FM/10.0;

		 // okay, add the record...
		DataForm->Insert_syllable_brain_record(
				 option->syllable_table_name,
				(double)features.serial_number,
				 //(int)
				 features.bird_ID,
				 starton,
				 duration,
				(float)features.mean_pitch,
				(float)features.mean_principal_contour,
				 fm,
				(float)features.mean_entropy/100.0,
				(float)features.mean_pGood/10.0,
				(float)features.mean_peakFr,
				(float)features.mean_am/100000.0,
				(float)features.mean_amplitude/10.0,
				(float)features.mean_continuity_f/100.0,
				(float)features.mean_continuity_t/100.0,
				(float)features.mean_slope/100.0,

				(float)features.min_pitch,
				(float)features.min_principal_contour,
				(float)features.min_FM/10.0,
				(float)features.min_entropy/100.0,
				(float)features.min_peakFr,
				(float)features.min_slope/100.0,

				(float)features.max_pitch,
				(float)features.max_principal_contour,
				(float)features.max_FM/10.0,
				(float)features.max_entropy/100.0,
				(float)features.max_peakFr,
				(float)features.max_slope/100.0,
				(float)features.max_amplitude,
				(float)features.max_continuity_f/100.0,
				(float)features.max_continuity_t/100.0,

				1000.0*(float)features.var_pitch,
				(float)features.var_FM,
				0.01*(float)features.var_entropy,
				100.0*(float)features.var_pGood,
				1000.0*(float)features.var_peakFr,
				(float)features.var_am/100000.0,

				 // add brain data
				(float)features.s1_amplitude,
				(float)features.s1_entropy,
				(float)features.s1_peak_frequency,
				(float)features.s1_peak_power,
				(float)features.s1_highpass_power,

				(float)features.s2_amplitude,
				(float)features.s2_entropy,
				(float)features.s2_peak_frequency,
				(float)features.s2_peak_power,
				(float)features.s2_highpass_power,

				(float)features.s3_amplitude,
				(float)features.s3_entropy,
				(float)features.s3_peak_frequency,
				(float)features.s3_peak_power,
				(float)features.s3_highpass_power,

				(float)features.s4_amplitude,
				(float)features.s4_entropy,
				(float)features.s4_peak_frequency,
				(float)features.s4_peak_power,
				(float)features.s4_highpass_power,

				(float)features.s5_amplitude,
				(float)features.s5_entropy,
				(float)features.s5_peak_frequency,
				(float)features.s5_peak_power,
				(float)features.s5_highpass_power,

				 // finally, we add the time stamps
				(int)features.month,
				(int)features.day,
				(int)features.hour,
				(int)features.minute,
				(int)features.second,
				current_file_name
		);
		*/

}



// create arrays for each feature (of pitch, FM, etc.), based on the number of records
void CZT_FeatureCalc::create_arrays(int size)
{
	  pitch=new float[size];
	  principal_frequency=new float[size];
	  fm=new float[size];
	  am=new float[size];
	  entropy=new float[size];
	  pitchGoodness=new float[size];
	  peakFreq=new float[size];
	  amplitude=new float[size];
	  mfa=new float[size];
	//  cepst0=new float[size];
	  pitchS=new float[size];
	  fmS=new float[size];
	  entropyS=new float[size];
	  pitchGoodnessS=new float[size];
	  AMS=new float[size];
	  signal=new bool[size];
	  bout_signal=new bool[size];
	  hp_small=new float[size];
	  hp_big=new double[size];
	  hp_diff=new double[size];
	  arrays_created=true;
	  for(int i=0;i<size;i++)
	  {
		hp_big[i]=0;
		hp_small[i]=0;
		hp_diff[i]=0;
	  }

}


// this function save the features to arrays of pitch, FM, etc.
void CZT_FeatureCalc::add_features_to_vars()
{
   if(option->cepstrum_pitch) pitch[recnum]=m_Pitch;
   fm[recnum]=m_FM;
   am[recnum]=m_AM;
   entropy[recnum]=m_Entropy;
   pitchGoodness[recnum]=m_PitchGoodness;
   peakFreq[recnum]=m_PeakFreq;
   amplitude[recnum]=m_amplitude;
   mfa[recnum]=m_mfa;
  // cepst0[recnum]=m_cepst0;
   signal[recnum]=sound_detected;
   if(pitch[recnum]>0) pitchS[recnum]=(log(pitch[recnum])-option->pitchOpLb->Value)/option->PitchDVLB->Value; //6.4)/0.82;
   else pitchS[recnum]=0; //1.75*log(max(pitch[i],(float)1.0));

   entropyS[recnum]=(entropy[recnum]-option->EntropyOpLb->Value) / option->EntropyDVLB->Value;

   fmS[recnum]=(fm[recnum]-option->FMOpLb->Value) / option->FMDVLB->Value;
   AMS[recnum]=(am[recnum]-option->AMOpLb->Value) / option->AMDVLB->Value;
   if(pitchGoodness[recnum]>0)pitchGoodnessS[recnum]=(log(pitchGoodness[recnum])-option->PgoodOpLb->Value)/option->PgoodDVLB->Value;//5.35)/0.72;
   else pitchGoodnessS[recnum]=0;//1.75*log(max(pitchGoodness[i],(float)1.0));
 //if(fm[i]>0) fmS[i]=(log(fm[i])-4.05)/1.066; else fmS[i]=0;//(fm[i]-option->FMTB->Position)/(float)(option->FMDVTB->Position/10.0);
}






// this function extract data from the array position [recnum] to the feature curren (m_)variables
void CZT_FeatureCalc::feature_array_to_var(int i)
{
    m_amplitude=amplitude[i];
    m_Pitch=pitch[i];
	m_PitchGoodness=pitchGoodness[i];
    m_FM=fm[i];
    m_AM=am[i];
    m_Entropy=entropy[i];
	m_PeakFreq=peakFreq[i];
	m_mfa=mfa[i];
    m_continuity_f=continuity_f[i];
    m_continuity_t=continuity_t[i];
    m_principal_contour=principal_frequency[i];
    m_FMc=FMc[i];
    boutDur=bout_signal[i];
	sound_detected=signal[i];
}







void CZT_FeatureCalc::refine_segmentation()
{
   int window=10, i=1;
   do{
     do{i++;}while(i<recnum-window && !(signal[i]==0 && signal[i+1]==1) && !(signal[i]==1 && signal[i+1]==0) );
     if(i>=recnum-window)break;
     // here is a transition to sound
     int ref;
     float max=0, min=999;
     if(signal[i]==0) // beginning of syllable
     {
          for(int j=-window;j<window;j++)if(am[i+j]>max) {max=am[i+j]; ref=j; } // we have found the max AM for begining
          for(int j=-window;j<ref;j++) signal[i+j]=0;
          for(int j=ref;j<window; j++)signal[i+j]=1;
     }
     else  // end of syllable
     {
         for(int j=-window;j<window;j++)if(am[i+j]<min) {min=am[i+j]; ref=j; } // we have found the max AM for begining
         for(int j=-window;j<ref;j++) signal[i+j]=1;
         for(int j=ref;j<window; j++)signal[i+j]=0;
     }
     i+=window;
   }while(i<recnum-window);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CZT_FeatureCalc::note_dur(float thresh, int missmatches)
{
   int *note_durs;
   int ref;
   float diff;
   //calc_thresh(state_transition_TB->Position);
   int Iterations1=total_frames;
   note_durs=new int[Iterations1];
   for(int i=10; i<Iterations1;i++)note_durs[i]=0;
   for(int i=10; i<Iterations1-10;i++)
   {
      if(signal[i])
      {
        // go forwards until hit threshold of dis-similarity
        ref=0; 
        int missmatch=0;
        do{
            ref++; 
            diff = pow(pitchS[i]-pitchS[i+ref],2)
                  +pow(fmS[i]-fmS[i+ref],2)
                  +pow(entropyS[i]-entropyS[i+ref],2)
                  +pow(pitchGoodnessS[i]-pitchGoodnessS[i+ref],2);
            if(diff>thresh)missmatch++;
            else missmatch=0;
        }while(signal[i+ref] && missmatch<missmatches && i+ref<Iterations1);
        note_durs[i]+=ref;

        // go backwords until hit threshold of dis-similarity
        /*   */
        ref=0; missmatch=0;
        do{
            ref--;
            diff = pow(pitchS[i]-pitchS[i+ref],2)
                  +pow(fmS[i]-fmS[i+ref],2)
                  +pow(entropyS[i]-entropyS[i+ref],2)
                  +pow(pitchGoodnessS[i]-pitchGoodnessS[i+ref],2);
            if(diff>thresh)missmatch++;
            else missmatch=0;
        }while(signal[i+ref] && missmatch<missmatches && i+ref>1);

        note_durs[i]-=ref; // ref is now negative, so we actually add it...
     }
     DataForm->insert_dur(note_durs[i]);
    // Memo1->Lines->Add(note_durs[i]);
   }
   delete[] note_durs;

}


bool CZT_FeatureCalc::test_it(float* data, int position, int data_size, int dur_test, int tolerance)
{
     // starting from a position in the wave file, we look for fucked segments that are overalped
     int fucked=0;
     for(int i=1;i<dur_test;i++) // we only test for certain duration, say 100ms x 44.1
     {
        if(position+i>data_size)break; // we are done, no fucked segments
        if(data[position]==data[position+i])
        {
           fucked=1;   // okay, one equal number, but how many comes immediatly more after him?
           do{fucked++;}while(position+i+fucked<data_size && data[position+fucked]==data[position+i+fucked]);
        }
		if(fucked>tolerance)
				return true;
	 }
	 return false;
}



bool CZT_FeatureCalc::init_viewer(CWavIterator& wave)//& wave)//CWavIterator& wave)
{
	// simplified vesion of all
	ResetContents();
	max_drv=0;
	position=0;
	pcmPos=wave.GetHeadPosition();
	nFileTotal = wave.GetTotalSamples();
	recnum=0;
	int nTotal=0, advance, retract;
	float fReal1,fImag1;
	advance=nFileTotal/1000;
	int data_samples=512;//option->ztSamplesPerWindow;
	retract=data_samples-advance;

	for(int i=0;i<1000;i++)
	{
		 if(nFileTotal<position+(data_samples*2))
		 {
			wave.ResetContents();
			return(false);
		 }
		 nTotal += advance;//ztFeatureAdvance; // Advance ztFeatureAdvance samples at a time
		 recnum++;
		 wave.ReadSamples(pcmPos,m_cepst.m_pInput,512);
		 m_cepst.Run();
		// m_time_deriv_max=m_freq_deriv_max=m_amplitude=m_MaxPower=m_SumLog=m_LogSum=0;
		 for(int k=0;k<256;k++) //256 out of the whole 512, which is up to 11kHz
		 {
			   fReal1 =m_cepst.m_pOutput[k][ztReal];
			   fImag1 =m_cepst.m_pOutput[k][ztImaginary];
			   m_powSpec[k] = fReal1*fReal1+fImag1*fImag1;//m_time_deriv[i] + m_freq_deriv[i];
			   if(max_drv<m_powSpec[k])max_drv=m_powSpec[k];
		  }
		  RenderViewerSlice();
			  // Get ready for next window
		  memset(m_cepst.m_pInput,0,sizeof(float)*m_cepst.m_nIn);  // zero out fft input...
		  pcmPos-=retract; // Back up a little bit...(for over-lap). We can Assume there is always at least  ztBufferRetract number
		  position-=retract;
		  position+=data_samples;
		  if(pcmPos<0) break;
	 }// end of reading loop
	 wave.ResetContents();






	/*

	// ResetContents();  // Clean up any garbage from a previous run...
	 ZeroFFTs();
	 m_cepst.Initialize(ztFFT_CEPST);
	 m_Image.ResetContents();
	 m_sonogram.ResetContents();
	 m_contours.ResetContents();
	 position=0;
	 m_Taper1=CZT_MultiTaper::Instance().m_pTaper1;
	 max_drv=0;
	 //PCMPOS pcmPos;
	 pcmPos=wave.GetHeadPosition();

	//we are not using myFFT.m_nIn since we are going to keep the zero padding
	//if(option->range_x05) wave.ReadSamples(pcmPos, m_cepst.m_pInput, 2*option->ztSamplesPerWindow);
	//else
	wave.ReadSamples(pcmPos, m_cepst.m_pInput, option->ztSamplesPerWindow);
	nFileTotal  =    wave.GetTotalSamples();
	recnum=0;
	//  if(!slices)return(false); // wave file contains no data
	*/
	return(true);
}


bool CZT_FeatureCalc::viewer(CWavIterator& wave, int dur, bool fit_all)
{
	int nTotal=0, advance, retract;
	float fReal1,fImag1;
	if(fit_all)advance=nFileTotal/dur;
	else advance=option->Advance;//ztFeatureAdvance;  60
	int data_samples=option->ztSamplesPerWindow;
	retract=data_samples-advance;

	for(int i=0;i<dur;i++)
	{
		 if(nFileTotal<position+(data_samples*2)) return(false);

		 nTotal += advance;//ztFeatureAdvance; // Advance ztFeatureAdvance samples at a time
		 recnum++;

		 for(int j=0;j<data_samples;j++)
		 {
			   m_cepst.m_pInput[j] *=m_Taper1[j];
			   if(option->boost_amplitude)m_cepst.m_pInput[j] *=5;
		 }


		 try{
			 m_cepst.Run();
		 } catch ( ... ) {
		   return(false);
		 }
		 m_time_deriv_max=m_freq_deriv_max=m_amplitude=m_MaxPower=m_SumLog=m_LogSum=0;
		 for(int k=0;k<option->spectrum_range;k++) //256 out of the whole 512, which is up to 11kHz
		 {

			   fReal1 =m_cepst.m_pOutput[k][ztReal];
			   fImag1 =m_cepst.m_pOutput[k][ztImaginary];
			   m_powSpec[k] = fReal1*fReal1+fImag1*fImag1;//m_time_deriv[i] + m_freq_deriv[i];
			   if(max_drv<m_powSpec[k])max_drv=m_powSpec[k];
		  }
		  RenderViewerSlice();
			  // Get ready for next window
		  memset(m_cepst.m_pInput,0,sizeof(float)*m_cepst.m_nIn);  // zero out fft input...
		  pcmPos-=retract; // Back up a little bit...(for over-lap). We can Assume there is always at least  ztBufferRetract number
		  position-=retract;
		  if(option->range_x05)
		  {
		   float tmpdata[3000];
		   wave.ReadSamples(pcmPos,tmpdata,data_samples*2);
		   position+=data_samples*2;
		   for(int z=0;z<data_samples;z++) m_cepst.m_pInput[z]= tmpdata[z*2];
		  }
		  else {
				wave.ReadSamples(pcmPos,m_cepst.m_pInput,data_samples);
				position+=data_samples;
		  }
		  if(pcmPos<0) break;
	 }// end of reading loop
  return(true);
}


void  CZT_FeatureCalc::RenderViewerSlice()
{
	 if(!m_bRenderImage)
		  return;

	 int nSpectral;
	 CZT_FeatImgSlice* pCurrentSlice = new CZT_FeatImgSlice;
	 //float brightness=0.0000001;//max_drv/2000000000000000.25f;
	 // Spectrals
	 for(int i=1;i<128;i++)
	 {
		  // the slice needs some of this goodness..
		  nSpectral=int(-m_powSpec[i*2]*brightness);
		  //if(nSpectral>32766) nSpectral=32766;
		  if(nSpectral<-64000) nSpectral=-64000;//32766;
		  nSpectral+=32000;
		  pCurrentSlice->spectral[i] =(short)nSpectral;
	 }
	 m_Image.AddSlice(pCurrentSlice);

}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////


void CZT_FeatureCalc::CalculateDAS()
{
   //int *note_durs;
   int ref, counter;
   float diff;
   float similarity_reject_thresh=option->state_transition_Num->Value;//calc_thresh(state_transition_TB->Position);
   //note_durs=new int[Iterations1];
   for(int i=0; i<recnum;i++)FMc[i]=0;

   for(int i=10; i<recnum-10;i++)
   {
      //if(signal[i])
      {
        // go forwards until hit threshold of dis-similarity
        ref=0; counter=0;
        int missmatches=0;
        do{
            ref++; counter++;
            diff = pow(pitchS[i]-pitchS[i+ref],2)
                  +pow(fmS[i]-fmS[i+ref],2)
                  +pow(entropyS[i]-entropyS[i+ref],2)
                  +pow(pitchGoodnessS[i]-pitchGoodnessS[i+ref],2);
            if(diff>similarity_reject_thresh)missmatches++;
            else missmatches=0;
        }while(
        //signal[i+ref] &&
        missmatches<option->missmatchesTb->Position && i+ref<recnum);
        FMc[i]+=ref;

        // go backwords until hit threshold of dis-similarity
        ref=0; counter=0; missmatches=0;
		do{
            ref--; counter++;
            diff = pow(pitchS[i]-pitchS[i+ref],2)
                  +pow(fmS[i]-fmS[i+ref],2)
				  +pow(entropyS[i]-entropyS[i+ref],2)
				  +pow(pitchGoodnessS[i]-pitchGoodnessS[i+ref],2);
			if(diff>similarity_reject_thresh)missmatches++;
			else missmatches=0;
		}while(
		//signal[i+ref] &&
		missmatches<option->missmatchesTb->Position && i+ref>1);
		FMc[i]-=ref; // ref is now negative, so we actually add it...
	 }
	// Memo1->Lines->Add(note_durs[i]);
   }
   //delete[] note_durs;

}







void CZT_FeatureCalc::do_hpfilt() {
 if (recnum<10) return;
int feature = option->pri_segmented_feature;
	float* data = new float[recnum];

	switch(feature)
	  {
		case 0:
			for (int i=0; i < recnum; i++){data[i] = amplitude[i];}
			break;
		case 1:
			for (int i=0; i < recnum; i++) {data[i] = pitch[i];}
			break;
		case 2:
			for (int i=0; i < recnum; i++) {data[i] = peakFreq[i];}
			break;
		case 3:
			for (int i=0; i < recnum; i++) {data[i] = pitchGoodness[i];}
			break;
		case 4:
			for (int i=0; i < recnum; i++) {data[i] = fm[i];}
			break;
		case 5:
			for (int i=0; i < recnum; i++) {data[i] = am[i];}
			break;
		case 6:
			for (int i=0; i < recnum; i++) {data[i] = entropy[i];}
			break;
		case 7:
			for (int i=0; i < recnum; i++) {data[i] = continuity_t[i];}
			break;
		case 8:
		for (int i=0; i < recnum; i++) {data[i] = continuity_f[i];}
			break;
		default:
			for (int i=0; i < recnum; i++){data[i] = amplitude[i];}
	  }

   if (option->smooth_feature) {
		try{hp_filter(data, recnum ,option->hp_small, hp_small, 1);   // smooth the feature
		}catch(Exception & exception) {
		   for(int i=0;i<slices;i++) hp_small[i]=1;
		}
   } else {
		try{hp_filter(data, recnum ,0, hp_small, 1);
		}catch(Exception & exception) {
		   for(int i=0;i<slices;i++) hp_small[i]=1;
		}
   }

   float adaptMean = 0.0;
   float priThresh = option->adjustThreshold(feature, option->pri_thresh_raw, option->frequency, 1);
   if (option->adapt_threshold) {
		try{hp_filter(data, recnum ,option->hp_big, hp_big, 1);
		}catch(Exception & exception) {
		   for(int i=0;i<slices;i++) hp_big[i]=1;
		}

	  if (recnum) {
		   for(int i=0;i<recnum;i++) {
				adaptMean += hp_big[i];
		   }
			adaptMean /= recnum;
			////myfile<<"average:"<<adaptMean<<"\n";
			adaptMean -= priThresh; //option->pri_thresh;
			////myfile<<"adapt mean:"<<adaptMean<<"\n";
			for(int i=0;i<recnum;i++) {
			hp_big[i]-=adaptMean;
			}
		}
	   } else {
		  for(int i=0;i<recnum;i++) {
			hp_big[i]=priThresh; //option->pri_thresh;
			hp_diff[i]=hp_small[i]-priThresh; //option->pri_thresh;
		  }
	   }
	   delete[] data;
  /*
	m_Image.ResetContents();
	m_sonogram.ResetContents();
	m_contours.ResetContents();
	int save_mem = 0;
	int slice_num = 0;
   for(int itr=1; itr<recnum; itr++)
   {
	   save_mem++;
	   if(save_mem>4)save_mem=0;
	   if(render_image && (!option->save_image_mem || (!save_mem && slice_num<633) ))
	   {
		 slice_num++;
		 RenderImageSlice(itr);
	   }
	}
	*/
 }



 void CZT_FeatureCalc::Render_sonogram_images()
 {
	m_Image.ResetContents();
	m_sonogram.ResetContents();
	m_contours.ResetContents();
	int save_mem = 0;
	int slice_num = 0;
   for(int itr=1; itr<recnum; itr++)
   {
	   save_mem++;
	   if(save_mem>4)save_mem=0;
	   if(render_image && (!option->save_image_mem || (!save_mem && slice_num<633) ))
	   {
		 slice_num++;
		 RenderImageSlice(itr);
	   }
	}
 }






int CZT_FeatureCalc::hp_filter(float *data, int nobs,double lambda, double *output, BOOL ret)
{
   /*
		 *  Schneller Algorithmus zur Lösung des LGS BxX=Y
		 * unter Berücksichtigung, dass B eine pentadiagonale Matrix ist
         * siehe z.B. Helmut Späth "Numerik, Eine Einführung für
         * Mathematiker und Informatiker", S. 110 ff, Braunschweig &
         * Wiesbaden, 1994
   */

  double* a;
  double* b;
  double *c;
  register int i=0;//UINT  i=0;
  double H1=0;
  double H2=0;
  double H3=0;
  double H4=0;
  double H5=0;
  double HH1=0;
  double HH2=0;
  double HH3=0;
  //double HH4=0;
  double HH5=0;
  double Z;
  double HB;
  double HC;

   /* Testen, ob nobs >5 */
   
   if (nobs <= 5) return 1;
   
   /* Speicher in den HEAP */

   a = (double *) calloc(nobs, sizeof(double));
   b = (double *) calloc(nobs, sizeof(double)) ;
   c = (double *) calloc(nobs, sizeof(double));
   
   /* Testen auf Speicherproblem */
   if (a == NULL || b == NULL || c == NULL) return 2; 

   /* Da bei diesem Algorithmus, die Daten überschrieben wrden, müssen wir
	  ein neuen Array füllem.
   */

   for(i=0;i<nobs;i++) output[i]=data[i];

   /* Koeffizientenmatrix erstellen */


   a[0]=1.0+lambda;
   b[0]=-2.0*lambda;
   c[0]=lambda;

   for(i=1;i<nobs-2;i++)
   {
	 a[i]=6.0*lambda+1;
     b[i]=-4.0*lambda;
	 c[i]=lambda;
   }

   a[1]=5.0*lambda+1;
   a[nobs-1]=1.0+lambda;
   a[nobs-2]=5.0*lambda+1.0;

   b[nobs-2]=-2.0*lambda;
   b[nobs-1]=0.0;
   c[nobs-2]=0.0;
   c[nobs-1]=0.0;

  /* Vorwärts du junge Garde */
   for (i=0;i<nobs;i++)
   {
      Z=a[i]-H4*H1-HH5*HH2;
	  if (Z==0) return 3;
      HB=b[i];
	  HH1=H1;
      H1=(HB-H4*H2)/Z;
	  b[i]=H1;
      HC=c[i];
	  HH2=H2;
      H2=HC/Z;
	  c[i]=H2;
      a[i]=(output[i]-HH3*HH5-H3*H4)/Z;
	  HH3=H3;
      H3=a[i];
	  H4=HB-H5*HH1;
      HH5=H5;
	  H5=HC;
   }
   /* Rückwärts muss die junge Garde auch */
  H2=0;
  H1=a[nobs-1];
  output[nobs-1]=H1;


 for (i=nobs-1;i>0;i--)
 {
  output[i-1]=a[i-1]-b[i-1]*H1-c[i-1]*H2;
  H2=H1;
  H1=output[i-1];
 }

 /* Trend oder zyklische Komponente? */
 if (ret==FALSE) for(i=0;i<nobs;i++) output[i]=data[i]-output[i];

 /* Wer will schon Speicherlücken? */
 free(a);
 free(b);
 free(c);
 return 0;
}








int CZT_FeatureCalc::hp_filter(float *data, int nobs,double lambda, float *output, BOOL ret)
{
   /*
		 *  Schneller Algorithmus zur Lösung des LGS BxX=Y
		 * unter Berücksichtigung, dass B eine pentadiagonale Matrix ist
         * siehe z.B. Helmut Späth "Numerik, Eine Einführung für
         * Mathematiker und Informatiker", S. 110 ff, Braunschweig &
         * Wiesbaden, 1994
   */

  double* a;
  double* b;
  double *c;
  register int i=0;//UINT  i=0;
  double H1=0;
  double H2=0;
  double H3=0;
  double H4=0;
  double H5=0;
  double HH1=0;
  double HH2=0;
  double HH3=0;
  //double HH4=0;
  double HH5=0;
  double Z;
  double HB;
  double HC;

   /* Testen, ob nobs >5 */

   if (nobs <= 5) return 1;

   /* Speicher in den HEAP */

   a = (double *) calloc(nobs, sizeof(double));
   b = (double *) calloc(nobs, sizeof(double)) ;
   c = (double *) calloc(nobs, sizeof(double));

   /* Testen auf Speicherproblem */
   if (a == NULL || b == NULL || c == NULL) return 2;

   /* Da bei diesem Algorithmus, die Daten überschrieben wrden, müssen wir
	  ein neuen Array füllem.
   */

   for(i=0;i<nobs;i++) output[i]=data[i];

   /* Koeffizientenmatrix erstellen */


   a[0]=1.0+lambda;
   b[0]=-2.0*lambda;
   c[0]=lambda;

   for(i=1;i<nobs-2;i++)
   {
	 a[i]=6.0*lambda+1;
     b[i]=-4.0*lambda;
	 c[i]=lambda;
   }

   a[1]=5.0*lambda+1;
   a[nobs-1]=1.0+lambda;
   a[nobs-2]=5.0*lambda+1.0;

   b[nobs-2]=-2.0*lambda;
   b[nobs-1]=0.0;
   c[nobs-2]=0.0;
   c[nobs-1]=0.0;

  /* Vorwärts du junge Garde */
   for (i=0;i<nobs;i++)
   {
      Z=a[i]-H4*H1-HH5*HH2;
	  if (Z==0) return 3;
      HB=b[i];
	  HH1=H1;
      H1=(HB-H4*H2)/Z;
	  b[i]=H1;
      HC=c[i];
	  HH2=H2;
      H2=HC/Z;
	  c[i]=H2;
      a[i]=(output[i]-HH3*HH5-H3*H4)/Z;
	  HH3=H3;
      H3=a[i];
	  H4=HB-H5*HH1;
      HH5=H5;
	  H5=HC;
   }
   /* Rückwärts muss die junge Garde auch */
  H2=0;
  H1=a[nobs-1];
  output[nobs-1]=H1;


 for (i=nobs-1;i>0;i--)
 {
  output[i-1]=a[i-1]-b[i-1]*H1-c[i-1]*H2;
  H2=H1;
  H1=output[i-1];
 }

 /* Trend oder zyklische Komponente? */
 if (ret==FALSE) for(i=0;i<nobs;i++) output[i]=data[i]-output[i];

 /* Wer will schon Speicherlücken? */
 free(a);
 free(b);
 free(c);
 return 0;
}
