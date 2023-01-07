// CZT_PcmBuffer.cpp: implementation of the CZT_PcmBuffer class.
//
//////////////////////////////////////////////////////////////////////
#include "CZT_PcmBuffer.h"
#include "CZT_PcmBufferFilter.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CZT_PcmBuffer::CZT_PcmBuffer()
{
	m_nBufferSize = 4096;
	CommonConstruct();

}
CZT_PcmBuffer::CZT_PcmBuffer(int nBufSize)
{
	m_nBufferSize = nBufSize;
	CommonConstruct();
}
void CZT_PcmBuffer::CommonConstruct()
{
	m_pBuffer	= new float[m_nBufferSize+1];
	m_lUpperPCM=-1;// the uppor bound 
	m_lLowerPCM=-1;// the lower bound 
	m_nBufferContents = 0;// nothing in here
	m_pFilter = NULL;
}
CZT_PcmBuffer::~CZT_PcmBuffer()
{
	ReleaseBuffer();
}
void CZT_PcmBuffer::ReleaseBuffer()
{
	if(m_pBuffer != NULL)
		delete [] m_pBuffer;
	m_pBuffer = NULL;
}
void CZT_PcmBuffer::InitBuffer(int nBufSize)
{
	ReleaseBuffer();
	m_nBufferSize = nBufSize;
	m_pBuffer	= new float[m_nBufferSize+1];
	m_lUpperPCM=-1;// the uppor bound 
	m_lLowerPCM=-1;// the lower bound 
	m_nBufferContents = 0;// nothing in here
}
void CZT_PcmBuffer::ResetContents()
{
	m_lUpperPCM=-1;// the uppor bound 
	m_lLowerPCM=-1;// the lower bound 
	m_nBufferContents = 0;// nothing in here
}
inline void CZT_PcmBuffer::ConvertCopy(short* pRawSamples,int &nAmt)
{
	// Convert-Copy
	register short* pCur = pRawSamples;
	register short* pEnd = &pRawSamples[nAmt];
	register float* pUs = m_pBuffer;
	while(pCur < pEnd)
	{
		*pUs = float(*pCur);
		pCur++;
		pUs++;
	}
}
void CZT_PcmBuffer::SetData(short* pRawSamples,long lLowerBound,int nAmt)
{
	m_lLowerPCM = lLowerBound;
	m_lUpperPCM	= lLowerBound+nAmt;
	m_nBufferContents = nAmt;

	// Use one conditional (if) and unroll
	if(m_pFilter)
	{
		// filter shorts
		m_pFilter->ProcessData(pRawSamples,nAmt);

		// copy data
		ConvertCopy(pRawSamples,nAmt);

		// filter floats
		m_pFilter->ProcessData(m_pBuffer,nAmt);
	}
	else
		ConvertCopy(pRawSamples,nAmt);
}
int	CZT_PcmBuffer::Read(long lStart,long lEnd,float* pTarg)
{
	// Someone is requesting data from PCM sample positions
	// lStart thru lEnd
	// First we need to see if there is any of that data
	// in our buffer..
	if(lStart >= m_lLowerPCM &&	lStart < m_lUpperPCM)
	{
        register float* pSrc =m_pBuffer+(lStart-m_lLowerPCM);
		// Ok we have at least some of it...
		if(lEnd == lStart)
		{
			// they want one piece of data, and we have it...
			*pTarg = *pSrc;
			return 1;
		}
		else if(lEnd < m_lUpperPCM)
		{
			// We have everything they want....
			// So copy data in our buffer starting at:
			// lStart-m_lLowerPCM (gives us buffer start index)
			// Thru
			// m_lUpperPCM-lEnd (buffer end index)
			int nAmt =lEnd-lStart;
			memcpy(pTarg,pSrc,nAmt*sizeof(float));
			return nAmt;			
		}
		else
		{
			// We give them what we have...
			// So copy data in our buffer starting at:
			// lStart-m_lLowerPCM (gives us buffer start index)
			// Thru
			// m_lUpperPCM-lStart (buffer end index)
			int nAmt =m_lUpperPCM-lStart;
			memcpy(pTarg,pSrc,nAmt*sizeof(float));
			return nAmt;
		}
	}
	else
		return 0;// we have nothing of what they want
}
void CZT_PcmBuffer::ReleaseList(CZT_PcmBufferList* pList)
{
	while(pList->size() > 0)
	{
		delete pList->back();
		pList->pop_back();
	}
}


