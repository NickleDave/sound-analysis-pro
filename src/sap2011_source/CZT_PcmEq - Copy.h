// CZT_PcmEq.h: interface for the CZT_PcmEq class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CZT_PCMEQ_H__ACBF32BA_3707_4563_A102_B558D3B0AED1__INCLUDED_)
#define AFX_CZT_PCMEQ_H__ACBF32BA_3707_4563_A102_B558D3B0AED1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "CZT_PcmBufferFilter.h"

class CZT_PcmEq  : public CZT_PcmBufferFilter
{
public:
	CZT_PcmEq();
	virtual ~CZT_PcmEq();

	// only one function needs implementing
	//////////////////////////////////////////////////////////////////////
	void	ProcessData(short* pSamples,int nCount);
};

#endif // !defined(AFX_CZT_PCMEQ_H__ACBF32BA_3707_4563_A102_B558D3B0AED1__INCLUDED_)
