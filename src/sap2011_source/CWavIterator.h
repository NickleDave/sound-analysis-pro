// CWavIterator.h: interface for the CWavIterator class.
/*
	DWS:
	Allows one to iterate a wav file like this:

	CWavIterator wIterator("C:\\MyFile.wav");


	LONG pos = wIterator.MoveToHeadPosition();
	float fSample = 0.0f;

	while(pos != CWavIterator::END_POS)
	{
		fSample = wIterator.GetNextFloat(pos);
		// do something with the sample
	}

        This file is meant to be compiled under VC++ 6.0 Or
        Borland CPP Builder 5.0

	9.24.2003 DWS Update:
	All access to the iterator is now done through a PCMPOS type - which
	allows random access to sample data.

	Example:

	CWavIterator wIterator("C:\\MyFile.wav");
	PCMPOS pcmpos = wIterator.GetHeadPosition();
	float myData[256];
	while(pcmpos != pcmpos_end)
		wIterator.ReadSamples(pcmpos,myData,255);
*/
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CWAVITERATOR_H__7AB38DF6_8862_40D7_92D9_0718F53AB3F0__INCLUDED_)
#define AFX_CWAVITERATOR_H__7AB38DF6_8862_40D7_92D9_0718F53AB3F0__INCLUDED_

//////////////////////////////////////////////////////////////////////
// Wav file structures
//////////////////////////////////////////////////////////////////////
#ifndef UINT16
typedef unsigned short UINT16;
#endif
typedef UINT32 ztFOURCC;	// a four character code 

typedef struct CHUNKHDR {
  ztFOURCC ckid;		// chunk ID 
  UINT32 dwSize; 	// chunk size 
} CHUNKHDR;

// MMIO macros 
#define ztmmioFOURCC(ch0, ch1, ch2, ch3) \
  ((UINT32)(BYTE)(ch0) | ((UINT32)(BYTE)(ch1) << 8) | \
  ((UINT32)(BYTE)(ch2) << 16) | ((UINT32)(BYTE)(ch3) << 24))

#define ztFOURCC_RIFF	ztmmioFOURCC ('R', 'I', 'F', 'F')
#define ztFOURCC_WAVE	ztmmioFOURCC ('W', 'A', 'V', 'E')
#define ztFOURCC_FMT	ztmmioFOURCC ('f', 'm', 't', ' ')
#define ztFOURCC_DATA	ztmmioFOURCC ('d', 'a', 't', 'a')

// simplified header for standard WAV files 
typedef struct ztWAVEHDR 
{
  CHUNKHDR	chkRiff; //8 bytes
  ztFOURCC	fccWave; // 4 bytes
  CHUNKHDR	chkFmt;//8 bytes
  UINT16	wFormatTag;	/* format type  2 bytes*/
  UINT16	nChannels;	/* 2 bytes number of channels (i.e. mono, stereo, etc.) */
  UINT32	nSamplesPerSec; /* 4 bytes sample rate */
  UINT32	nAvgBytesPerSec;/* 4 bytes for buffer size estimation */
  UINT16	nBlockAlign;	/* 2 bytes block size of data */
  UINT16	wBitsPerSample;//2 bytes
  CHUNKHDR	chkData;//8 bytes 
} ztWAVEHDR;

typedef struct ztLVDHDR
{
  double	SamplesPerSec; /* 8 bytes sample rate */
  double	Channels; /* 8 bytes number of channels,
						the first ch is sound normally */
  double	DateTime; /* 8 bytes date time in a quite exotic format:
						when transferred to the string format,
						it looks like: 20090510191145.00
		   This means: year 2009, month 05, day 10, hour 19, min 11, sec 45,
		   parts of second after the point.*/
  double    InputRange; /* 8 bytes input range in Volts*/
} ztLVDHDR;

#include "CZT_PcmBufferMgr.h"
#include "CZT_PcmBufferFilter.h"
#define PCMPOS LONG
const static PCMPOS pcmpos_end =-1;
const static PCMPOS pcmpos_error =-1971;

//////////////////////////////////////////////////////////////////////
// CWavIterator class
//////////////////////////////////////////////////////////////////////
class CWavIterator  
{
public:

	CWavIterator();
	CWavIterator(char* pszFile);
	virtual ~CWavIterator();

	// Iteration Utils
	//////////////////////////////////////////////////////////////////////
	PCMPOS				GetHeadPosition();
	PCMPOS				GetTailPosition();

	// Reading Sample Data (returns amount read..)
	// if pos == pcmpos_end after a read, then there are no
	// more samples afterwards, alternatively monitoring the return
	// can yield same result (IE: watching for 0)
	//////////////////////////////////////////////////////////////////////
	int					ReadSamples(PCMPOS& pos,float* pDest,int nCount);

	// public utils
	//////////////////////////////////////////////////////////////////////
	int					GetTotalSamples(){return m_nTotalSamples;};
	void				ResetContents();// completely resets state	

	// Filtering data
	//////////////////////////////////////////////////////////////////////
	void				UseFilter(CZT_PcmBufferFilter& filter);
	void				ClearFilter();// reverse of use

	// Open file returns either the pcm start position
	// or pcmpos_error
	//////////////////////////////////////////////////////////////////////
	PCMPOS				OpenFile(char* pszFile);
		ztWAVEHDR			m_WaveHeader;
		ztLVDHDR            m_LVDHeader;

protected:

	// Misc
	//////////////////////////////////////////////////////////////////////
	void				CommonConstruct();

	// Buffering utils
	//////////////////////////////////////////////////////////////////////
	int					FillNewBuffer(LONG filePos);


	// Conversion Utils
	//////////////////////////////////////////////////////////////////////
	LONG				SamplePosToFilePos(PCMPOS pos);
	PCMPOS				FilePosToSamplePos(LONG pos);

	// next three functions Copied from wave splitter
	//////////////////////////////////////////////////////////////////////
	BOOL				ReadRiffHeader();
	BOOL				ReadFormatHeader();
	BOOL				ReadDataHeader();

	//Alexei's addition to read LVD files
	bool				IsLVD;
	char				*getFileExtension(char* fileName);
	BOOL				ReadLVDHeader();
	double 				ntohd( const double inDouble );

	// wave data
	//////////////////////////////////////////////////////////////////////
	HANDLE				m_hWaveFile;
	int					m_nBufferSize;
	int					m_nTotalSamples;
	short*				m_pRawSamples;
     //	ztWAVEHDR			m_WaveHeader;
	LONG				m_lDataStart;
	LONG				m_lDataEnd;

	// Internal buffering data
	//////////////////////////////////////////////////////////////////////	
	CZT_PcmBufferMgr	m_BufferMGR;
};

#endif // !defined(AFX_CWAVITERATOR_H__7AB38DF6_8862_40D7_92D9_0718F53AB3F0__INCLUDED_)
