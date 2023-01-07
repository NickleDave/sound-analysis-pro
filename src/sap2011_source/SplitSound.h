//---------------------------------------------------------------------------

#ifndef SplitSoundH
#define SplitSoundH
#define UINT16 WORD
#define UINT32 DWORD

// CWavFileSplitter.h: interface for the CWavFileSplitter class.
//
//////////////////////////////////////////////////////////////////////
/*
DWS: 6/11/2002

  Usage:

  CWavFileSplitter	 mySplitter;
  if(mySplitter.OpenFile("myWav.wav"))
  {
	 // save left channel into file...
	 mySplitter.SaveChannel(CWavFileSplitter::left,"C:\\left_channel.wav"));
	 // save right channel into file...
	 mySplitter.SaveChannel(CWavFileSplitter::right,"C:\\right_channel.wav"));
  }
  // resuse it
  mySplitter.CloseFile();

  if(mySplitter.OpenFile("myWav2.wav"))
 {
	 // save left channel into file...
	 mySplitter.SaveChannel(CWavFileSplitter::left,"C:\\left_channel2.wav"));
	 // save right channel into file...
	 mySplitter.SaveChannel(CWavFileSplitter::right,"C:\\right_channel2.wav"));
  }

*/
//############################################################################
// Wav file structures
typedef UINT32 FOURCC;	// a four character code 

typedef struct CHUNKHDR1 {
  FOURCC ckid;		// chunk ID 
  UINT32 dwSize; 	// chunk size 
} CHUNKHDR1;

// MMIO macros 
#define mmioFOURCC1(ch0, ch1, ch2, ch3) \
  ((UINT32)(BYTE)(ch0) | ((UINT32)(BYTE)(ch1) << 8) | \
  ((UINT32)(BYTE)(ch2) << 16) | ((UINT32)(BYTE)(ch3) << 24))

#define FOURCC_RIFF1	mmioFOURCC1 ('R', 'I', 'F', 'F')
#define FOURCC_WAVE	mmioFOURCC1 ('W', 'A', 'V', 'E')
#define FOURCC_FMT	mmioFOURCC1 ('f', 'm', 't', ' ')
#define FOURCC_DATA	mmioFOURCC1 ('d', 'a', 't', 'a')

// simplified header for standard WAV files 
typedef struct WAVEHDR1
{
  CHUNKHDR1 chkRiff; //8 bytes
  FOURCC fccWave; // 4 bytes
  CHUNKHDR1 chkFmt;//8 bytes
  UINT16 wFormatTag;	/* format type  2 bytes*/
  UINT16 nChannels;	/* 2 bytes number of channels (i.e. mono, stereo, etc.) */
  UINT32 nSamplesPerSec; /* 4 bytes sample rate */
  UINT32 nAvgBytesPerSec;/* 4 bytes for buffer size estimation */
  UINT16 nBlockAlign;	/* 2 bytes block size of data */
  UINT16 wBitsPerSample;//2 bytes
  CHUNKHDR1 chkData;//8 bytes 
} WAVEHDR1;
 
class CWavFileSplitter  
{
public:

	enum Channels
	{
		left = 1,
		right = 0
	};

	CWavFileSplitter();
	virtual ~CWavFileSplitter();

	// gets
	// ================================================================
	int						GetChannels(){return m_WaveHeader.nChannels;};
	int						GetSamplesPerSec(){return m_WaveHeader.nSamplesPerSec;};

	// utils
	// ================================================================
	BOOL					OpenFile(char* pszFile);
	BOOL					SaveChannel(int nChannel, char* pszDestination);// 0 left 1 = right
	void					SetBufferSize(int nSize);
	void					CloseFile();// does on destruct but might want to re-use

protected:

	// utils
	// ================================================================
	void				PrepareHeaderForWrite(WAVEHDR1& hdr);

	// I seperated the reading into little functions
	BOOL				ReadRiffHeader();
	BOOL				ReadFormatHeader();
	BOOL				ReadDataHeader();

	// data 
	// ================================================================
	WAVEHDR1				m_WaveHeader;
	HANDLE				m_hInFile;
	int					m_nBufferSize;	

};



#endif
 