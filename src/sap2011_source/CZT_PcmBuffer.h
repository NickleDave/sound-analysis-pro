// CZT_PcmBuffer.h: interface for the CZT_PcmBuffer class.
// More Clear way of dealing with wav data....
/*
	Ofer wants to read samples approx 256 at a time.

	Then before reading more he wants to backup approx .25% or so - IE:
	overlap things.

	The solution is not as straight-forward as one might think.  The reason
	it gets complex is because there will be an eq applied to the data.  This
	eq should not be re-applied to data already buffered, and 
	data should only be sucked out of a file once.

	Solution:

	Create a buffer cache where older-buffers get re-used but there are
	caches of freshly-used ones.. (this works).
*/
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CZT_PCMBUFFER_H__BD30D561_69C9_4EDB_853B_BC2E8155D79F__INCLUDED_)
#define AFX_CZT_PCMBUFFER_H__BD30D561_69C9_4EDB_853B_BC2E8155D79F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <list>
// Forwards
class CZT_PcmBuffer;
class CZT_PcmBufferFilter;
typedef std::list<CZT_PcmBuffer*> CZT_PcmBufferList;

class CZT_PcmBuffer  
{
public:
	CZT_PcmBuffer();
	CZT_PcmBuffer(int nBufSize);
	virtual ~CZT_PcmBuffer();

	// Public Utils
	//////////////////////////////////////////////////////////////////////	
	void					ReleaseBuffer();
	void					ResetContents();
	void					InitBuffer(int nBufSize=4096);
	int						Read(long lStart,long lEnd,float* pTarg);
	void					SetData(short* pRawSamples,long lLowerBound,int nAmt);

	// Data
	//////////////////////////////////////////////////////////////////////
	float*					m_pBuffer;
	int						m_nLeft;
	int						m_nBufferSize;
	int						m_nBufferContents;
	CZT_PcmBufferFilter*	m_pFilter;

	// PCM data positions that represents where this data came from
	// Conversion from PCM position to file-position handled in wave iterator
	//////////////////////////////////////////////////////////////////////
	long					m_lUpperPCM;
	long					m_lLowerPCM;

	// Static Util
	//////////////////////////////////////////////////////////////////////
	static void				ReleaseList(CZT_PcmBufferList* pList);

protected:

	// utils
	//////////////////////////////////////////////////////////////////////
	void					CommonConstruct();
	void					ConvertCopy(short* pRawSamples,int &nAmt);

};

#endif // !defined(AFX_CZT_PCMBUFFER_H__BD30D561_69C9_4EDB_853B_BC2E8155D79F__INCLUDED_)
