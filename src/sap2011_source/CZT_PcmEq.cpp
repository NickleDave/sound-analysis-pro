// CZT_PcmEq.cpp: implementation of the CZT_PcmEq class.
//
//////////////////////////////////////////////////////////////////////
#include "CZT_PcmEq.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CZT_PcmEq::CZT_PcmEq()
{

}

CZT_PcmEq::~CZT_PcmEq()
{

}
void CZT_PcmEq::ProcessData(short* pSamples,int nCount)
{

 static float xv[3]={0,0,0} ;
 static float yv[3]={0,0,0};
 //for(int i=0;i<nCount;i++) pSamples[i]=out[i];
 // float in[4096];
//  for(int i=0;i<=nCount;i++) in[i]=pSamples[i];

  for(int i=0;i<nCount;i++)
  {
        xv[0] = xv[1]; xv[1] = xv[2];
        xv[2] = pSamples[i] /1.020353514;// GAIN;
        yv[0] = yv[1]; yv[1] = yv[2];
        yv[2] =   (xv[0] + xv[2]) - 2 * xv[1] + ( -0.9605029194 * yv[0]) + (  1.9597070338 * yv[1]);
        pSamples[i] = yv[2];
  }
                                                    /*


    float cut=350.0;
    float in[4096], out[40960];
    float w = 2.0 * 44100;
    float Norm;
    cut *= 2.0F * 3.14;
    Norm = 1.0 / (cut + w);
    float a0 = w * Norm;
    float a1 = -a0;
    float b1 = (w - cut) * Norm;
    for(int i=0; i<=nCount; i++) in[i]=pSamples[i];
    out[0]=0;
    for(int i=1; i<=nCount; i++)
    {
     out[i] = in[i]*a0 + in[i-1]*a1 + out[i-1]*b1;
    }
     for(int i=0; i<=nCount; i++)  pSamples[i]=out[i];

       float cutoff = 250;//cutoff freq in Hz
       float fs = 44100;//sampling frequency //(e.g. 44100Hz)
       float f = 2 * sin (3.14 * cutoff / fs) //[approximately]
       float q = 0.5; //resonance/bandwidth [0 < q <= 1]  most res: q=1, less: q=0
       float low;// = lowpass output
       float high;// = highpass output
       float band;// = bandpass output
       float notch;// = notch output
       float scale = q;
       low=high=band=0;


//--beginloop
low = low + f * band;
high = scale * input - low - q*band;
band = f * high + band;
notch = high + low;
//--endloop                          


     /*







     // This function needs filled out..Will go over with ofer...
   for(int i=0;i<nCount;i++)
     {
      pSamples[i]=pSamples[i]-pSamples[i+1];

     }
     pSamples[nCount]=0;
                                              




     for(int i=2;i<nCount-2;i++) x[i]=pSamples[i+1]*2+(pSamples[i])-(pSamples[i-1])-pSamples[i-2]*2;
     for(int i=2;i<nCount-2;i++) pSamples[i]=x[i]/5;

     pSamples[1]=pSamples[1]-pSamples[0];pSamples[0]=0;
     pSamples[nCount-2]=pSamples[nCount-2]-pSamples[nCount-1];pSamples[nCount-1]=0;  pSamples[nCount]=0;  */
}
