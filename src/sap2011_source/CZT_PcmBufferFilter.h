// CZT_PcmBufferFilter.h: interface for the CZT_PcmBufferFilter class.
/*
	Abstract Base class for implementing a filter for
	use in a CWavIterator object.
*/
//////////////////////////////////////////////////////////////////////
/*
     Note : In your implemented classes you will see warnings indicating
     one or the other functions hides the other.. This is just borlands
     compiler warning you of the natural behavior of over-riding some virtuals
     and not others (I dont know why they warn you).
*/
#if !defined(AFX_CZT_PCMBUFFERFILTER_H__D86883B5_AC3B_493F_880F_14BD75DD5186__INCLUDED_)
#define AFX_CZT_PCMBUFFERFILTER_H__D86883B5_AC3B_493F_880F_14BD75DD5186__INCLUDED_

class CZT_PcmBufferFilter  
{
public:
	CZT_PcmBufferFilter();
	virtual ~CZT_PcmBufferFilter();

	// Virtuals
	//////////////////////////////////////////////////////////////////////
	// Only one of these functions should be implemented.
	virtual void	ProcessData(short* pSamples,int nCount){};
	virtual void	ProcessData(float* pSamples,int nCount){};

};

#endif // !defined(AFX_CZT_PCMBUFFERFILTER_H__D86883B5_AC3B_493F_880F_14BD75DD5186__INCLUDED_)
