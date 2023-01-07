// CZT_PcmBufferMgr.cpp: implementation of the CZT_PcmBufferMgr class.
//
//////////////////////////////////////////////////////////////////////
#include "CZT_PcmBufferMgr.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CZT_PcmBufferMgr::CZT_PcmBufferMgr()
{
	m_nMaxBuffers=3;
	m_nBufSize=4096;
	m_pFilter=NULL;
}
CZT_PcmBufferMgr::~CZT_PcmBufferMgr()
{
	ReleaseBuffers();
}
void CZT_PcmBufferMgr::ReleaseBuffers()
{
	// "Let my buffers go..."
	CZT_PcmBuffer::ReleaseList(&m_ActiveBuffers);
	CZT_PcmBuffer::ReleaseList(&m_InActiveBuffers);
}
void CZT_PcmBufferMgr::ResetContents()
{
	Init(m_nMaxBuffers);
}
void CZT_PcmBufferMgr::Init(int nMaxBuffers)
{
	ReleaseBuffers();
	m_nMaxBuffers = nMaxBuffers;

	// Fill The Inactive list...
	for(int i=0;i<m_nMaxBuffers;i++)
		m_InActiveBuffers.push_back(new CZT_PcmBuffer(m_nBufSize));
}
CZT_PcmBuffer* CZT_PcmBufferMgr::GetBufferForWriting()
{
	// We have two lists..An Inactive list and an active list..
	// Anything in the active list should have data in it,
	// The in-active are buffers ready for use.

	// First see if there are any inactive buffers we can
	// use..
	if(!m_InActiveBuffers.empty())
	{
		// Ok we have at least 1 buffer we can use thats
		// in the in-active list...
		CZT_PcmBuffer* pBuf = m_InActiveBuffers.front();
		m_InActiveBuffers.pop_front();// remove from inactive list..
		// put it on tail of active..
		m_ActiveBuffers.push_back(pBuf);
		pBuf->m_pFilter = m_pFilter;// assign filter
		return pBuf;
	}
	else
	{
		// Ok there are no inactive buffers - they
		// all have data in them..The most recently used
		// ones are on the tail of the list, so we pop off the front to
		// get the oldest..
		CZT_PcmBuffer* pBuf = m_ActiveBuffers.front();
		m_ActiveBuffers.pop_front();// remove it from head...

		// reset it
		pBuf->ResetContents();

		// and add it back onto the tail 
		m_ActiveBuffers.push_back(pBuf);
		pBuf->m_pFilter = m_pFilter;// assign filter
		return pBuf;
	}
}
void CZT_PcmBufferMgr::DontUse(CZT_PcmBuffer* pBuffer)
{
	// For some reason there was a decision to not use a buffer that
	// was retrieved using GetBufferForWriting, we have to remove it from
	// the m_ActiveBuffers list...

	// I am going to assume that GetBufferForWriting was only called
	// once before DontUse is called.. So the back() should be the
	// buffer in question, if not do a simple reset and forgeddabout-it
	if(m_ActiveBuffers.back() == pBuffer)
	{
		m_ActiveBuffers.pop_back();
		m_InActiveBuffers.push_back(pBuffer);
	}
	else
		pBuffer->ResetContents();
}
int	CZT_PcmBufferMgr::ReadDataSegment(long lStart,long lEnd,float* pTarg)
{
	// The moment of truth has arrived.....
	// In theory (havent tested anything yet) - the active buffer list
	// should have data organized top-down from when it was read and processed.
	// So to try and read this data segment we can iterate the list forward
	// and try to fulfill their request.
	int nAmt = lEnd-lStart;

	if(!m_ActiveBuffers.empty())
	{
		CZT_PcmBufferList::iterator Iter = m_ActiveBuffers.begin();
		CZT_PcmBufferList::iterator Last = m_ActiveBuffers.end();
		CZT_PcmBuffer* pBuffer;
		long lCurStart = lStart;
		int  nTotal=0;
		int  nRead=0;
		float* pCurTargPosition=pTarg;

		// loop while there are more items
		while(Iter != Last) 
		{
			pBuffer = *Iter;

			// Ok we have a buffer that should have some data in it...
			// Lets see if it has any of what they want..
			nRead = pBuffer->Read(lCurStart,lEnd,pCurTargPosition);
			if(nRead >0)
			{
				// they indeed filled up some of our data...
				pCurTargPosition	+= nRead;
				nTotal				+= nRead;
				lCurStart			+= nRead;
			}
			// If we are done then return....
			if(nTotal == nAmt)
				return nTotal;

			// increate iterator
			Iter++;
		}
		// Return what we found..
		return nTotal;
	}
	return 0;
}
void CZT_PcmBufferMgr::UseFilter(CZT_PcmBufferFilter* pFilter)
{
	m_pFilter = pFilter;

}
