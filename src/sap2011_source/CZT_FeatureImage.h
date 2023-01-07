// CZT_FeatureImage.h: interface for the CZT_FeatureImage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CZT_FEATUREIMAGE_H__EC48BD72_BF05_4DEF_8657_DA790C97F727__INCLUDED_)
#define AFX_CZT_FEATUREIMAGE_H__EC48BD72_BF05_4DEF_8657_DA790C97F727__INCLUDED_
#define sliceSize  256//128
/*
     No need to store an entire image for the calculations.
     Each slice (window) will have
     128 values for spectral (BYTES)
     5 shorts for positions of the other data for a total of 138 bytes
     per scanline.  So even for 1000 windows it amounts to 138k total.

     Later:
     Seems better if the brightness can be calculated in real-time...
     Ofer was doing  (brightness->Position*100000)   Then clipping from (-128) - (128)
     brightness has range of (1-5000)

          nColor=m_spec_deriv[i]/m_nBrightness;
          if(nColor>126) nColor=126;
          if(nColor<-126) nColor=-126;

     Brightness slider is defaulted to 4000...
     So a default brightness would be m_spec_deriv[i]/104000
     Which yields a useable value from -128 to 128 (approx)
     To get better granularity (and allow real-time changing of brightness)
     Change the default to have granularity of -32768 to 32768  EX:

     specral of : -13312000.0f : normal way(m_spec_deriv[i]/104000) = -128
     using new divisor (2000000.25f - derived by voodoo magic)
     We then can get a good dynamic range from 32768 -(-32768)
     Then clip it to either 32768 or -32768, and shove it into a short (2bytes)...

     So total data size (for a window) becomes:
     128*2 values for spectral (256 BYTES) +5*2(10 Bytes ofr accoustic features) = 266bytes per window...

     seb1.wav runs 2519 FFT windows... So
     2519*266=670054(bytes) =  670.054k - not bad at all...

     LATER:
     It works...m_fSpectralBrightness controls brightness range(1.0f-600.0f). 1.0f= really bright , 256.0f = normal
     
*/
// http://www.msoe.edu/eecs/ce/courseinfo/stl/list.htm (stl list docs)

#include <list>		// list class library
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
using namespace std;

class CZT_FeatImgSlice
{
public:
	CZT_FeatImgSlice(){};
	virtual ~CZT_FeatImgSlice(){};

     // Data
     //////////////////////////////////////////////////////////////////////
     short     spectral[sliceSize];
     short     pitchGoodness;
     short     entropy;
     short     fm;
     short     peakfreq;
     short     pitch;
     short     DerivPow;
     short     mfa;
     short     continuity_t;
     short     continuity_f;
     short     principal_frequency;
     short     slope;
     short     cepst0;
     short     am;
     short     hp_small;
     short     hp_big;
     short     hp_diff;
     bool      Sound;
     bool      Bout;
};
typedef list<CZT_FeatImgSlice*> CZT_FeatImgSliceList;


class CZT_Line// psuedo line structure
{
public:
	CZT_Line()
     {
          bAlpha=255;
          bRender = TRUE;
     };
	virtual ~CZT_Line(){};

     // one util to set both..
     inline void SetVal(short& val)
     {
          yFrom=yTo=val;
     };

     // Data
     //////////////////////////////////////////////////////////////////////
     short yFrom;
     short yTo;
     BYTE  bAlpha;
     DWORD dwColor;
     BOOL  bRender;
};


class CZT_FeatureImage
{
public:
		bool *signal_r, *bout_r;
	  //	float *hp_small_r;
      //  double *hp_big_r, *hp_diff_r;
        int    m_nMaxSlices;
        CZT_FeatureImage();
	virtual ~CZT_FeatureImage();

     // Utils
     //////////////////////////////////////////////////////////////////////
     void                       AddSlice(CZT_FeatImgSlice* pSlice);
     int                        GetTotalWidth(){return m_Slices.size();};
     void                       DrawSlices(int nStart,int nEnd,TImage* pTargetImg);
     void                       DrawAllSlices(TImage* pTargetImg);
     void                       DrawDVDs(int nStart,int nEnd,TImage* pTargetImg);
     void                       DrawAllDVDs(TImage* pTargetImg);
     void                       ResetContents();
     void                       SetBrightness(float fVal){m_fSpectralBrightness = fVal;};
     void                       SetMaxSlices(int nVal){m_nMaxSlices = nVal;};
     void                       DrawRect(int x, int y);
     void                       DrawViewerSlices(TImage* pTargetImg);
     void                       DrawVSlices(int nStart,int nEnd,TImage* pTargetImg);

     // Gets
     //////////////////////////////////////////////////////////////////////
     CZT_Line&                  GetPitchGoodness(){return m_lnPitchGoodness;};
     CZT_Line&                  GetEntropy(){return m_lnEntropy;};
     CZT_Line&                  GetFM(){return m_lnFM;};
     CZT_Line&                  GetPeakFreq(){return m_lnPeakFreq;};
     CZT_Line&                  GetPitch(){return m_lnPitch;};
     CZT_Line&                  GetDerivPow(){return m_DerivPow;};
     CZT_Line&                  Getmfa(){return m_mfa;};
     CZT_Line&                  Getcontinuity_t(){return m_continuity_t;};
     CZT_Line&                  Getcontinuity_f(){return m_continuity_f;};
     CZT_Line&                  Getprincipal_frequency(){return m_principal_frequency;};
     CZT_Line&                  Getslope(){return m_slope;};
     CZT_Line&                  Getcepst0(){return m_cepst0;};
     CZT_Line&                  GetAM(){return m_InAM;};
     CZT_Line&                  Gethp_small(){return m_hp_small;};
     CZT_Line&                  Gethp_big(){return m_hp_big;};
     CZT_Line&                  Gethp_diff(){return m_hp_diff;};
     CZT_Line&                  GetSound(){return m_Sound;};
     CZT_Line&                  GetBout(){return m_Bout;};

protected:

     // Utils
     //////////////////////////////////////////////////////////////////////
     void                       InitColors();
     void                       ReleaseSlices();
     void                       FindUsingIndex(CZT_FeatImgSliceList::const_iterator& iter,int nIndex);
     void                       ConvertSpectral(short* pSrc);
     void                       BlendPixel(DWORD& pSrcPixel,DWORD* pDestPixel,BYTE& bAlpha);
     void                       PlotLine(int& y,DWORD* pCurPixel,CZT_Line& line);

     // Data
     //////////////////////////////////////////////////////////////////////
     DWORD                      m_dwSpecColors[sliceSize*2];
     BYTE                       tmp_spectral[sliceSize];
     CZT_FeatImgSliceList       m_Slices;
     float                      m_fSpectralBrightness;
    // short previous_principal[4];
     short ind;

     // Accoustic Renderering Params
     //////////////////////////////////////////////////////////////////////
     CZT_Line                     m_lnPitchGoodness;
     CZT_Line                     m_lnEntropy;
     CZT_Line                     m_lnFM;
     CZT_Line                     m_lnPeakFreq;
     CZT_Line                     m_lnPitch;
     CZT_Line                     m_DerivPow;
     CZT_Line                     m_mfa;
     CZT_Line                     m_continuity_t;
     CZT_Line                     m_continuity_f;
     CZT_Line                     m_principal_frequency;
     CZT_Line                     m_slope;
     CZT_Line                     m_cepst0;
     CZT_Line                     m_InAM;
     CZT_Line                     m_hp_small;
     CZT_Line                     m_hp_big;
     CZT_Line                     m_hp_diff;
     CZT_Line                     m_Sound;
     CZT_Line                     m_Bout;

};

#endif // !defined(AFX_CZT_FEATUREIMAGE_H__EC48BD72_BF05_4DEF_8657_DA790C97F727__INCLUDED_)
