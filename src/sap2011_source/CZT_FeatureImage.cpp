// CZT_FeatureImage.cpp: implementation of the CZT_FeatureImage class.
//
//////////////////////////////////////////////////////////////////////

#include "CZT_FeatureImage.h"
#include "options3.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CZT_FeatureImage::CZT_FeatureImage()
{
     InitColors();
    // previous_principal=0;
     ind=0;
     m_fSpectralBrightness = 256.0f;//256
	 m_lnPitchGoodness.dwColor = RGB(0,255,0);// green
	 m_lnEntropy.dwColor = RGB(0,255,255);// red
	 m_lnFM.dwColor =RGB(255,255,0);//   cyan
	 m_lnPeakFreq.dwColor =RGB (255,102,102);// yellow RGB(155,255,255);// something...
	 m_lnPitch.dwColor =RGB(0,0,255);//    blue
	 m_InAM.dwColor =RGB(0,100,255);//    vus
	 m_DerivPow.dwColor=RGB(0,0,255);// purple    DWORD newColor=RGB(0,255,255);
	 m_mfa.dwColor=RGB (255,102,102);// yellow    DWORD newColor=RGB(0,255,255);
	 m_continuity_t.dwColor=RGB(60,155,255);
	 m_continuity_f.dwColor=RGB(155,60,255);
	 m_principal_frequency.dwColor=RGB(255,155,60);
     m_slope.dwColor=RGB(60,255,60);
     m_cepst0.dwColor=RGB(0,255,155);



	 m_hp_small.dwColor=RGB(0,0,0);



	 //m_hp_big.dwColor=RGB(255,255,255);
	 m_hp_big.dwColor=RGB(0,255,255);
     m_hp_diff.dwColor=RGB(0,0,255);
     m_Sound.dwColor=RGB(0,0,155);
     m_Bout.dwColor=RGB(255,255,255);



     m_lnEntropy.bAlpha = 255;
     m_lnFM.bAlpha = 255;
     m_InAM.bAlpha = 255;
     m_lnPeakFreq.bAlpha = 255;
     m_lnPitch.bAlpha = 255;
     m_lnPitchGoodness.bAlpha = 255;
     m_DerivPow.bAlpha = 255;
     m_mfa.bAlpha = 255;
     m_continuity_t.bAlpha = 255;
     m_continuity_f.bAlpha = 255;
     m_principal_frequency.bAlpha = 255;
     m_slope.bAlpha = 255;
     m_cepst0.bAlpha = 255;
     m_hp_small.bAlpha = 255;
     m_hp_big.bAlpha = 255;
     m_hp_diff.bAlpha = 255;
     m_Sound.bAlpha = 128;
     m_Bout.bAlpha = 128;
     m_nMaxSlices = 10000;// maximum number of slices
}

CZT_FeatureImage::~CZT_FeatureImage()
{
     ReleaseSlices();
}
void CZT_FeatureImage::ResetContents()
{
     ReleaseSlices();
}
void CZT_FeatureImage::AddSlice(CZT_FeatImgSlice* pSlice)
{
	 if(int(m_Slices.size())<m_nMaxSlices)
          m_Slices.push_back(pSlice);
     else
          delete pSlice;// dont want it
};
void CZT_FeatureImage::ReleaseSlices()
{

	 // delete all the delcicios slices of data..(mmmm....entropy)
	 int i=m_Slices.size();
	 int j=0;
	// m_Slices.clear();
	 while (m_Slices.size() > 0)
     {
		  int xx= m_Slices.size();
          delete m_Slices.front();
		  m_Slices.pop_front();
	 }
	// m_Slices.clear();
}
inline void CZT_FeatureImage::BlendPixel(DWORD& pSrcPixel,DWORD* pDestPixel,BYTE& bAlpha)
{
     // alpha blend src onto target using alpha
 	BYTE *src=(BYTE*)&pSrcPixel;
	BYTE *dst=(BYTE*)pDestPixel;
	dst[0]=(BYTE)(((src[0]-dst[0])*bAlpha+(dst[0]<<8))>>8);
	dst[1]=(BYTE)(((src[1]-dst[1])*bAlpha+(dst[1]<<8))>>8);
	dst[2]=(BYTE)(((src[2]-dst[2])*bAlpha+(dst[2]<<8))>>8);
}
void CZT_FeatureImage::InitColors()
{
     // Singed Char to Unsigned mapping looks like this:
    // s(-128 thru -1) : u(128 thru 255)
    // s(0 thru 128) :   u(0 thru 127)

    // We want -128 to be black and -1 to be whiter
    // continuing through to 128 (white)
    // Easiest way to build the map is to build
    // it like we will access it...
    unsigned char ucChar;
    signed char scChar;
    BYTE bColor=0;
    for(int i=-128;i<128;i++)
    {
         scChar=(signed char)i;
         ucChar=(unsigned char)scChar;
         m_dwSpecColors[ucChar] = RGB(bColor,bColor,bColor++);
    }
}
inline void CZT_FeatureImage::FindUsingIndex(CZT_FeatImgSliceList::const_iterator& iter,int nIndex)
{
     // This is exactly the reason I prefer MFC's collection classes.. Heres a list..
     // But I can only read it from end to end..How nice..So To get the proper iterator
     // I have to actually count each time I move forward..sigh..would of been nice to
     // just add a little function in the list called find_index and return an
     // appropriate iterator...Maybe there is one - but I did not find it.
     iter=m_Slices.begin();
     CZT_FeatImgSliceList::const_iterator iterStop=m_Slices.end();
     int nCount = 0;
     while((iter != iterStop) && nCount!=nIndex)
     {
          iter++;
          nCount++;
     }

}



inline void CZT_FeatureImage::ConvertSpectral(short* pSrc)
{
     // use our fudged scaler and brightness to re-res the
     // spectral data...
     // then shove the clipped version into the byte array..
     register short*     pShort=pSrc;
     register BYTE*      pByteDest=tmp_spectral;
     register BYTE*      pEnd = pByteDest+option->bitmapHeight;
     register int        nColor;
     if(m_fSpectralBrightness==0)m_fSpectralBrightness=1;
     while(pByteDest != pEnd)
     {
          nColor =int(*pSrc/m_fSpectralBrightness);
       //   nColor+=126;
          if(nColor > 126) nColor = 126;
          if(nColor < -126) nColor = -126;
          *pByteDest = (unsigned char)nColor;
          // move on down the road
          pByteDest++;
          pSrc++;
     }
}





inline void CZT_FeatureImage::PlotLine(int& y,DWORD* pCurPixel,CZT_Line& line)
{
     if(!line.bRender)
		  return;

	int feature = option->pri_segmented_feature;
	switch(feature)
	  {
		case 0:
			m_hp_small.dwColor=RGB(128,0,128);
			break;
		case 1:
			m_hp_small.dwColor=RGB(0,0,255);
			break;
		case 2:
			m_hp_small.dwColor=RGB(255,102,102);
			break;
		case 3:
			m_hp_small.dwColor=RGB(0,255,0);
			break;
		case 4:
			m_hp_small.dwColor=RGB(255,255,0);
			break;
		case 5:
			m_hp_small.dwColor=RGB(0,100,255);
			break;
		case 6:
			m_hp_small.dwColor=RGB(0,0,255);
			break;
		case 7:
			m_hp_small.dwColor=RGB(60,155,255);
			break;
		case 8:
			m_hp_small.dwColor=RGB(155,60,255);
			break;
		default:
			m_hp_small.dwColor=RGB(128,0,128);
	  }

     // Not really drawing a line (per se)..Actually we are just
     // connecting dots togther which (with enough dots) looks just
     // like a line.

     //    line.yFrom holds previous value
     //    line.yTo holds current value
     //    we need to connect y2 up to y1
     if(line.yTo != line.yFrom)
     {
        // they are not the same so lets figure out how
        // to connect them..
        if(y==line.yTo)// if we are on the line point, just plot it
             BlendPixel(line.dwColor,pCurPixel,line.bAlpha);
        else if(line.yFrom < line.yTo)  // pt before was lower
        {
             if((y >= line.yFrom) && (y<line.yTo))
                 BlendPixel(line.dwColor,pCurPixel,line.bAlpha);

        }
        else if(line.yFrom > line.yTo)// the pt before was before us
        {
             if((y <= line.yFrom) && (y >=line.yTo))// && (y>line.yTo))
                BlendPixel(line.dwColor,pCurPixel,line.bAlpha);
        }

     }
	 else if(y==line.yTo)
          BlendPixel(line.dwColor,pCurPixel,line.bAlpha);
}



void CZT_FeatureImage::DrawAllSlices(TImage* pTargetImg)
{
	 DrawSlices(0,m_Slices.size(),pTargetImg);
}





void CZT_FeatureImage::DrawSlices(int nStart,int nEnd,TImage* pTargetImg)
{

	//if(option->testing) { DrawTesting(nStart, nEnd, pTargetImg); return; }
	// DWORD yellowColor=RGB(0,255,255) , redColor=RGB(0,0,255) , blueColor=RGB(255,0,0) ,  whiteColor=RGB(255,255,255);
	// BYTE color_dept = 255;
	 int iterations=0;
	// int stop_duration=1000;  // high initial value for the silence duration in the begining of the file
	 bool birdsound;
	 if(m_Slices.empty())
	   return;
	 int nWidth = nEnd-nStart;
	 int nHeight = option->bitmapHeight;              // we should allow changing this for higher resolution in the future...

	 // init the raw image we will use to raster onto...
	 Graphics::TBitmap* pBmp = new Graphics::TBitmap();
	 pBmp->PixelFormat =pf32bit;// Assures DWORD'ness
	 pBmp->Width = nWidth;
	 pBmp->Height = nHeight/option->squiz;

	 // Images are stored upside-down....
	 DWORD* pStartPixel = (DWORD*)pBmp->ScanLine[(option->bitmapHeight/option->squiz)-1];
	 DWORD* pCurPixel;
	 BYTE* pSpectrals;

	 // Sort of do things reverse as any other image thing..
	 // We actually fill in each column (IE: slice)
	 CZT_FeatImgSlice* pCurSlice;
	 CZT_FeatImgSliceList::const_iterator iterCur;
	 CZT_FeatImgSliceList::const_iterator iterEnd;
	 // Get the start..& Stop
	 FindUsingIndex(iterCur,nStart);
	 FindUsingIndex(iterEnd,nEnd);

	 int nX=0;
	// float ent_thresh=250+option->entropy_thresh*50;//100+option->entropy_thresh*10;
	 // Get the current slice before we start..
	 pCurSlice = (*iterCur);

	 // init our entropy line
	 m_lnEntropy.yFrom = pCurSlice->entropy;
	 m_lnPitchGoodness.yFrom = pCurSlice->pitchGoodness;
	 m_lnFM.yFrom = pCurSlice->fm;
	 m_InAM.yFrom = pCurSlice->am;
	// m_lnPeakFreq.yFrom = pCurSlice->peakfreq;
	 //m_lnPitch.yFrom = pCurSlice->pitch;
	 //pCurSlice->DerivPow=(int)amplitude_r[iterations];
	 m_DerivPow.yFrom =pCurSlice->DerivPow;

	 m_mfa.yFrom =pCurSlice->mfa;
	 m_continuity_t.yFrom =pCurSlice->continuity_t;
	 m_continuity_f.yFrom =pCurSlice->continuity_f;

	 m_hp_small.yFrom =pCurSlice->hp_small;
	 m_hp_big.yFrom =pCurSlice->hp_big;
	 m_hp_diff.yFrom =pCurSlice->hp_diff;

	 //m_principal_frequency.yFrom =pCurSlice->principal_frequency;
	 //m_slope.yFrom =pCurSlice->slope;

	 m_cepst0.yFrom =pCurSlice->cepst0;
	 m_Sound.yFrom =0;//pCurSlice->Sound*4;
	 m_Bout.yFrom =0;//pCurSlice->Bout*2;

	 while(iterCur != iterEnd)
	 {
		  // Get the pixel for the bottom row
		  pCurPixel = pStartPixel+nX;//  Puts us on the appropriate column...
		  pCurSlice = (*iterCur);
		  pSpectrals = &tmp_spectral[0];
		  ConvertSpectral(pCurSlice->spectral);

		  if(option->save_image_mem)
		  {
				pCurSlice->Bout=bout_r[iterations*5];
				birdsound=signal_r[iterations*5];
		  }
		  else
		  {
				pCurSlice->Bout=bout_r[iterations] ;
				birdsound=signal_r[iterations];
				//pCurSlice->DerivPow=(int)amplitude_r[iterations];
				m_DerivPow.yTo =pCurSlice->DerivPow;//(int)amplitude_r[iterations];//
		  }

		 // m_lnPitch.yTo=pCurSlice->pitch;
		  m_lnEntropy.yTo=pCurSlice->entropy;
		  m_lnPitchGoodness.yTo=pCurSlice->pitchGoodness;
		  m_lnFM.yTo=pCurSlice->fm;
		  m_InAM.yTo=pCurSlice->am;
		  m_lnPeakFreq.yTo=pCurSlice->peakfreq;

		  m_lnPitch.yFrom=pCurSlice->pitch-1;
		  m_lnPitch.yTo=pCurSlice->pitch+1;

		  m_DerivPow.yTo =pCurSlice->DerivPow;
		  m_mfa.yTo =pCurSlice->mfa;
		  m_continuity_t.yTo =pCurSlice->continuity_t;
		  m_continuity_f.yTo =pCurSlice->continuity_f;

		  m_hp_small.yTo =pCurSlice->hp_small;
		  m_hp_big.yTo =pCurSlice->hp_big;
		  m_hp_diff.yTo =pCurSlice->hp_diff;

		  m_principal_frequency.yFrom=pCurSlice->principal_frequency-1;
		  m_principal_frequency.yTo =pCurSlice->principal_frequency+1;
		  m_slope.yFrom =(pCurSlice->slope)+50;
		  m_slope.yTo   =(pCurSlice->slope)+52;
		  m_cepst0.yTo =pCurSlice->cepst0;
		  m_Sound.yTo =10;//pCurSlice->Sound*4;
		  m_Bout.yTo =2;//pCurSlice->Bout;



		  iterations++;
		  // Draw Spectral
		  for(int y=0;y<nHeight;y+=option->squiz)
		  {
			*pCurPixel = m_dwSpecColors[*pSpectrals];
			pSpectrals+=option->squiz;// next spectral
			if (option->adapt_threshold)
				PlotLine(y,pCurPixel, m_hp_big);
			if (birdsound)
				 PlotLine(y,pCurPixel,m_Sound);
			if (option->doNotSegment) {
			   PlotLine(y,pCurPixel,m_lnEntropy);
			   PlotLine(y,pCurPixel,m_lnPitchGoodness);
			   PlotLine(y,pCurPixel,m_lnFM);
			   PlotLine(y,pCurPixel,m_InAM);
			   PlotLine(y,pCurPixel,m_lnPeakFreq);
			   PlotLine(y,pCurPixel,m_lnPitch);
			   PlotLine(y,pCurPixel,m_DerivPow);
			   PlotLine(y,pCurPixel,m_mfa);
			   PlotLine(y,pCurPixel, m_continuity_t);
			   PlotLine(y,pCurPixel, m_continuity_f);
			   PlotLine(y,pCurPixel, m_hp_diff);
			   PlotLine(y,pCurPixel, m_principal_frequency);
			   if(pCurSlice->slope>-50) PlotLine(y,pCurPixel, m_slope); // slope<0 only if the principal contour is continuous...
			   PlotLine(y,pCurPixel,m_cepst0);
			   /*if (birdsound) {
				 PlotLine(y,pCurPixel,m_Sound);
			   }*/
			} else if(birdsound ) { // plot accoustics
			   //PlotLine(y,pCurPixel,m_lnPitch);
			   PlotLine(y,pCurPixel,m_lnEntropy);
			   PlotLine(y,pCurPixel,m_lnPitchGoodness);
			   PlotLine(y,pCurPixel,m_lnFM);
			   PlotLine(y,pCurPixel,m_InAM);
			   PlotLine(y,pCurPixel,m_lnPeakFreq);
			   PlotLine(y,pCurPixel,m_lnPitch);
			   PlotLine(y,pCurPixel,m_DerivPow);
			   PlotLine(y,pCurPixel,m_mfa);
			   PlotLine(y,pCurPixel, m_continuity_t);
			   PlotLine(y,pCurPixel, m_continuity_f);
			   if (option->smooth_feature)
					PlotLine(y,pCurPixel, m_hp_small);
			   PlotLine(y,pCurPixel, m_hp_diff);
			   PlotLine(y,pCurPixel, m_principal_frequency);
			   if(pCurSlice->slope>-50) PlotLine(y,pCurPixel, m_slope); // slope<0 only if the principal contour is continuous...
			   PlotLine(y,pCurPixel,m_cepst0);
			   //PlotLine(y,pCurPixel,m_Sound);
			 }

			 if(pCurSlice->Bout) PlotLine(y,pCurPixel,m_Bout);
			 pCurPixel += nWidth;// next row
		  }

		  m_lnEntropy.yFrom =pCurSlice->entropy-3;
		  m_lnPitchGoodness.yFrom =pCurSlice->pitchGoodness-3;
		  m_lnFM.yFrom =pCurSlice->fm-3;
		  m_InAM.yFrom =pCurSlice->am-3;
		  m_lnPeakFreq.yFrom =pCurSlice->peakfreq-3;
		  m_lnPitch.yFrom =pCurSlice->pitch-3;
		  m_DerivPow.yFrom =pCurSlice->DerivPow-3;
		  m_mfa.yFrom =pCurSlice->mfa-3;
		  m_continuity_t.yFrom =pCurSlice->continuity_t-3;
		  m_continuity_f.yFrom =pCurSlice->continuity_f-3;
		  m_hp_small.yFrom =pCurSlice->hp_small-3;
		  m_hp_big.yFrom =pCurSlice->hp_big-3;
		  m_hp_diff.yFrom =pCurSlice->hp_diff-3;

		 // m_principal_frequency.yFrom =pCurSlice->principal_frequency-3;
		 // m_slope.yFrom =pCurSlice->slope-3;
		  m_cepst0.yFrom =pCurSlice->cepst0-3;
		  // increase list pos
		  iterCur++;
		  nX++;
	 }
	 // Ok lets stretch our bitmap onto the image...
// Allow app to set this...
//     pTargetImg->Stretch = TRUE;// stretch it onto the image...
	 pTargetImg->Picture->Bitmap = pBmp;
	 /*
		  12.23.2003 DWS MEM-Leak solution....

		  Help file states... Note: Assigning a new bitmap to the picture object's
		  Graphic property causes the picture object to destroy the existing bitmap
		   and take ownership of the new one. The VCL handles the details of freeing
		   the resources associated with the previous bitmap automatically.

		  There are actually other references to how TBitmap and other graphics
		  stuff is dealt with - but the deal is that the target image makes
		  a copy of the TBitmap* we send in and it is left up to us to free it...This
		  fact is not entirely clear from examples I looked at - but it definitely
		  solves the leak problem.
	 */
	 delete pBmp;// goodbye old friend......
}




void CZT_FeatureImage::DrawAllDVDs(TImage* pTargetImg)
{
	 DrawDVDs(0,m_Slices.size(),pTargetImg);
}


void CZT_FeatureImage::DrawDVDs(int nStart,int nEnd,TImage* pTargetImg)
{
     if(m_Slices.empty()) return;
     int nWidth = nEnd-nStart;
     int nHeight = option->bitmapHeight;              // we should allow changing this for higher resolution in the future...
     // init the raw image we will use to raster onto...
     Graphics::TBitmap* pBmp = new Graphics::TBitmap();
     pBmp->PixelFormat =pf32bit;// Assures DWORD'ness
     pBmp->Width = nWidth;
     pBmp->Height = nHeight/option->squiz;

     // Images are stored upside-down....
     DWORD* pStartPixel = (DWORD*)pBmp->ScanLine[(option->bitmapHeight/option->squiz)-1];
     DWORD* pCurPixel;
     BYTE* pSpectrals;

     // Sort of do things reverse as any other image thing..
     // We actually fill in each column (IE: slice)
     CZT_FeatImgSlice* pCurSlice;
     CZT_FeatImgSliceList::const_iterator iterCur;
     CZT_FeatImgSliceList::const_iterator iterEnd;
     // Get the start..& Stop
     FindUsingIndex(iterCur,nStart);
     FindUsingIndex(iterEnd,nEnd);

     int nX=0;
    // float ent_thresh=100+option->entropy_thresh*10;
     // Get the current slice before we start..
     pCurSlice = (*iterCur);

     while(iterCur != iterEnd)
     {
          // Get the pixel for the bottom row
          pCurPixel = pStartPixel+nX;//  Puts us on the appropriate column...
          pCurSlice = (*iterCur);
          pSpectrals = &tmp_spectral[0];
          ConvertSpectral(pCurSlice->spectral);

          // Draw Spectral
          for(int y=0;y<nHeight;y+=option->squiz)
          {
            *pCurPixel = m_dwSpecColors[*pSpectrals];
            pSpectrals+=option->squiz;// next spectral
            pCurPixel += nWidth;// next row
          }
          iterCur++;
          nX++;
     }
     pTargetImg->Picture->Bitmap = pBmp;
     delete pBmp;// goodbye old friend......
}


void CZT_FeatureImage::DrawRect(int x, int y)
{
     CZT_FeatImgSlice* pCurSlice;
     CZT_FeatImgSliceList::const_iterator iterCur;
     FindUsingIndex(iterCur,x);
     pCurSlice = (*iterCur);
     pCurSlice->Bout=y;

}



void CZT_FeatureImage::DrawViewerSlices(TImage* pTargetImg)
{
     DrawVSlices(0,m_Slices.size(),pTargetImg);
}





void CZT_FeatureImage::DrawVSlices(int nStart,int nEnd,TImage* pTargetImg)
{
   //if(option->testing) { DrawTesting(nStart, nEnd, pTargetImg); return; }
    // DWORD yellowColor=RGB(0,255,255) , redColor=RGB(0,0,255) , blueColor=RGB(255,0,0) ,  whiteColor=RGB(255,255,255);
    // BYTE color_dept = 255;
     int iterations=0;
    // int stop_duration=1000;  // high initial value for the silence duration in the begining of the file
     bool birdsound;
	 if(m_Slices.empty())
       return;
     int nWidth = nEnd-nStart;
	 int nHeight = option->bitmapHeight/2;              // we should allow changing this for higher resolution in the future...

     // init the raw image we will use to raster onto...
     Graphics::TBitmap* pBmp = new Graphics::TBitmap();
     pBmp->PixelFormat =pf32bit;// Assures DWORD'ness
     pBmp->Width = nWidth;
     pBmp->Height = nHeight/option->squiz;

     // Images are stored upside-down....
     DWORD* pStartPixel = (DWORD*)pBmp->ScanLine[(nHeight/option->squiz)-1];
     DWORD* pCurPixel;
     BYTE* pSpectrals;

     // Sort of do things reverse as any other image thing..
     // We actually fill in each column (IE: slice)
     CZT_FeatImgSlice* pCurSlice;
     CZT_FeatImgSliceList::const_iterator iterCur;
     CZT_FeatImgSliceList::const_iterator iterEnd;
     // Get the start..& Stop
     FindUsingIndex(iterCur,nStart);
     FindUsingIndex(iterEnd,nEnd);

     int nX=0;
    // float ent_thresh=250+option->entropy_thresh*50;//100+option->entropy_thresh*10;
     // Get the current slice before we start..
     pCurSlice = (*iterCur);
     while(iterCur != iterEnd)
     {
          // Get the pixel for the bottom row
          pCurPixel = pStartPixel+nX;//  Puts us on the appropriate column...
          pCurSlice = (*iterCur);
          pSpectrals = &tmp_spectral[0];
          ConvertSpectral(pCurSlice->spectral);
          iterations++;
          // Draw Spectral
          for(int y=0;y<nHeight;y+=option->squiz)
          {
            *pCurPixel = m_dwSpecColors[*pSpectrals];
            pSpectrals+=option->squiz;// next spectral
            pCurPixel += nWidth;// next row
          }
          iterCur++;
          nX++;
     }
     pTargetImg->Picture->Bitmap = pBmp;
     delete pBmp;// goodbye old friend......
}


