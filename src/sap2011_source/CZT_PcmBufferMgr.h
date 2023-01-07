// CZT_PcmBufferMgr.h: interface for the CZT_PcmBufferMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CZT_PCMBUFFERMGR_H__4EF8B03F_0743_4A37_B66F_B1B2AE8C2B8A__INCLUDED_)
#define AFX_CZT_PCMBUFFERMGR_H__4EF8B03F_0743_4A37_B66F_B1B2AE8C2B8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "CZT_PcmBuffer.h"
class CZT_PcmBufferFilter;// forward
class CZT_PcmBufferMgr  
{
public:
	friend class CWavIterator;
	CZT_PcmBufferMgr();
	virtual ~CZT_PcmBufferMgr();

	// Utils
	//////////////////////////////////////////////////////////////////////
	CZT_PcmBuffer*			GetBufferForWriting();
	void					DontUse(CZT_PcmBuffer* pBuffer);// called after a call to GetBufferForWriting and do not want it used
	void					ResetContents();
	void					Init(int nMaxBuffers=3);
	int						ReadDataSegment(long lStart,long lEnd,float* pTarg);
	void					UseFilter(CZT_PcmBufferFilter* pFilter);
	
protected:

	// Utils
	//////////////////////////////////////////////////////////////////////
	void					ReleaseBuffers();

	// Data
	//////////////////////////////////////////////////////////////////////
	int						m_nMaxBuffers;
	int						m_nBufSize;
	CZT_PcmBufferList		m_ActiveBuffers;
	CZT_PcmBufferList		m_InActiveBuffers;
	CZT_PcmBufferFilter*	m_pFilter;
};

#endif // !defined(AFX_CZT_PCMBUFFERMGR_H__4EF8B03F_0743_4A37_B66F_B1B2AE8C2B8A__INCLUDED_)
