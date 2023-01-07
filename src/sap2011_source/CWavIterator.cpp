// CWavIterator.cpp: implementation of the CWavIterator class.
//
//////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <process.h>
#include <stdio.h>
#include <conio.h>
#include "CWavIterator.h"
#include "SplitSound.h"
#include "options3.h"
#include "math.h"
#include <Forms.hpp>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CWavIterator::CWavIterator()
{
	CommonConstruct();
}


CWavIterator::CWavIterator(char* pszFile)
{
	CommonConstruct();
	OpenFile(pszFile);

}



void CWavIterator::CommonConstruct()
{
	m_hWaveFile		= NULL;
	m_pRawSamples	= NULL;
	m_nBufferSize	= 4096;// read in up to 4024 samples of data at a time about 8k's worth)
	m_pRawSamples	= new short[m_nBufferSize+1];   //short is two-byte signed integer
	m_nTotalSamples = 0;

	// The buffers need to match for things to work....
	m_BufferMGR.m_nBufSize = m_nBufferSize;
	m_BufferMGR.Init();
}
CWavIterator::~CWavIterator()
{
	ResetContents();

	if(m_pRawSamples != NULL)
		delete [] m_pRawSamples;
	m_pRawSamples = NULL;
}
void CWavIterator::UseFilter(CZT_PcmBufferFilter& filter)
{
	m_BufferMGR.UseFilter(&filter);
}
void CWavIterator::ClearFilter()
{
	m_BufferMGR.UseFilter(NULL);
}
void CWavIterator::ResetContents()
{
	// close the file (if its opened)
	if(m_hWaveFile != NULL)
	{
		CloseHandle(m_hWaveFile);
		m_hWaveFile = NULL;
	}
	// clear internal buffer
	memset(m_pRawSamples,0,m_nBufferSize*2);
	m_nTotalSamples = 0;
	// Clear mgr..
	m_BufferMGR.ResetContents();
}


PCMPOS CWavIterator::OpenFile(char* pszFile)
{
	BOOL bSuccess = FALSE;


	ResetContents();

	// ok try and open the file before we do anything else....
	if(pszFile == NULL)
		return pcmpos_error;

	m_hWaveFile = CreateFile(pszFile,
							GENERIC_READ,                 // open for reading 
							FILE_SHARE_READ,                            // do not share
							NULL,                         // no security 
							OPEN_EXISTING,                // existing file only 
							FILE_ATTRIBUTE_NORMAL,        // normal file 
							NULL);                        // no attr. template

	// if it did not open we can not do anything
	if(m_hWaveFile == INVALID_HANDLE_VALUE)
	{
		DWORD dw = GetLastError();
                m_hWaveFile = NULL;
		return pcmpos_error;
	}

	//Check if sound is in LVD format first
	char *bbb = "lvd";
	IsLVD = (strcmpi(getFileExtension(pszFile),bbb)==0);//compare without case sensitivity

	if (IsLVD)
	{
	  if (ReadLVDHeader())
		  {
		  //ReadRiffHeader
		  m_WaveHeader.chkRiff.ckid = ztFOURCC_RIFF;
		  //ReadFormatHeader
		  m_WaveHeader.fccWave = ztFOURCC_WAVE;
		  m_WaveHeader.chkFmt.ckid = ztFOURCC_FMT;
          m_WaveHeader.chkFmt.dwSize =16;
		  m_WaveHeader.wFormatTag = 1;
		  m_WaveHeader.nChannels = 1;
		  m_WaveHeader.nSamplesPerSec = m_LVDHeader.SamplesPerSec;
		  m_WaveHeader.nAvgBytesPerSec = 2*m_LVDHeader.SamplesPerSec;
		  m_WaveHeader.nBlockAlign = 2;
		  m_WaveHeader.wBitsPerSample = 16;
		  //ReadDataHeader
		  m_WaveHeader.chkData.ckid = ztFOURCC_DATA;

		  bSuccess = TRUE;
		  }
	}
	else
	// first read the RIFF header...
	if(ReadRiffHeader())
	{
		// ok the riff was read ok...
		// now we can read in the format header
		if(ReadFormatHeader())
		{
			// ok last but not least...
			if(ReadDataHeader())
			{
				// ok we are good to go...
				bSuccess = TRUE;
			}
		}
	}

	if(!bSuccess)
	{
		ResetContents();
		return pcmpos_error;
	}

	// set the end pointers...
	m_lDataStart = SetFilePointer (m_hWaveFile,0, NULL, FILE_CURRENT);
	m_lDataEnd   = SetFilePointer(m_hWaveFile,0,NULL,FILE_END);
	m_nTotalSamples = ((m_lDataEnd-m_lDataStart)/2.0f);
	SetFilePointer (m_hWaveFile,m_lDataStart, NULL, FILE_BEGIN);
	if (IsLVD)	//add missed information
	{
		m_WaveHeader.chkData.dwSize =m_lDataEnd-m_lDataStart;
		m_WaveHeader.chkRiff.dwSize = m_WaveHeader.chkData.dwSize+(sizeof(WAVEHDR1) - 8);
		//this has no sense for LVD file, but added just for safety
	}

	if (IsLVD && (m_LVDHeader.Channels>1))	//more than one channel - create temporal LVD file
											//with one channel
	{
	 WORD*  	pSamples = new WORD[m_nTotalSamples];
	 int 		m_nTotalSamples1 = m_nTotalSamples/m_LVDHeader.Channels;
	 WORD*  	pSamples1 = new WORD[m_nTotalSamples1];
	 DWORD		dwRead = 0;
	 DWORD		dwWritten = 0;
	 int 		i;
	 int 		nChannels = int(m_LVDHeader.Channels);
	 char*		pszDestination = "C:\\SAP\\single_channel_00.lvd";
	 //char*		pszDestination = "C:\\SAP\\single_channel_01.lvd";
	 //char*		pszDestination = "C:\\SAP\\single_channel_02.lvd";
	 //char*		pszDestination = "C:\\SAP\\single_channel_03.lvd";
	 //char*		pszDestination = "C:\\SAP\\single_channel_04.lvd";
	 //char*		pszDestination = "C:\\SAP\\single_channel_05.lvd";

	 //char*		pszDestination = "C:\\SAP\\single_channel_06.lvd";
	 //char*		pszDestination = "C:\\SAP\\single_channel_07.lvd";
	 //char*		pszDestination = "C:\\SAP\\single_channel_08.lvd";
	 //char*		pszDestination = "C:\\SAP\\single_channel_09.lvd";
	 //char*		pszDestination = "C:\\SAP\\single_channel_10.lvd";
	 //char*		pszDestination = "C:\\SAP\\single_channel_11.lvd";
	 				//should be different for different instances!!!
	 double 	D;
	 ReadFile(m_hWaveFile,pSamples,m_nTotalSamples*2,&dwRead,NULL);
	 for (i = 0; i < m_nTotalSamples1; i++)
	   {
	   pSamples1[i] = pSamples[nChannels*i];
	   }
	 // ok lets create a new file...
	HANDLE	hOutput = CreateFile(pszDestination,   // open TWO.TXT
		GENERIC_WRITE,                // open for writing
		0,                            // do not share
		NULL,                         // no security
		CREATE_ALWAYS,                // create a new one - or erase old and we are going to write over it
		FILE_ATTRIBUTE_NORMAL,        // normal file
		NULL);                        // no attr. template

	if(hOutput == INVALID_HANDLE_VALUE)
		return FALSE;// no can do
	//write LVD header
	D = ntohd(m_LVDHeader.SamplesPerSec);
	WriteFile(hOutput,&D,8,&dwWritten,NULL);
	D = ntohd(1);	//1 channel
	WriteFile(hOutput,&D,8,&dwWritten,NULL);
	D = ntohd(m_LVDHeader.DateTime);
	WriteFile(hOutput,&D,8,&dwWritten,NULL);
	D = ntohd(m_LVDHeader.InputRange);
	WriteFile(hOutput,&D,8,&dwWritten,NULL);

	WriteFile(hOutput,pSamples1,2*m_nTotalSamples1,&dwWritten,NULL);
	FileClose(int(hOutput));
	ResetContents();
	pszFile=pszDestination;
	OpenFile(pszFile);
	}

	// we are done..
	if(m_WaveHeader.nChannels==1)return GetHeadPosition();
		if(m_WaveHeader.nChannels==2)
		{
		  //Application->MessageBox("stereo file", NULL, MB_OK);
		  CWavFileSplitter	 mySplitter;
		  ResetContents();
		  if(mySplitter.OpenFile(pszFile))
          {
                 // save left channel into file...
				 mySplitter.SaveChannel(CWavFileSplitter::left,"C:\\SAP\\left_channel.wav");
                 // save right channel into file...
                 mySplitter.SaveChannel(CWavFileSplitter::right,"C:\\SAP\\right_channel.wav");
          }
          // resuse it
          mySplitter.CloseFile();
          if(option->channel->ItemIndex==0)pszFile="C:\\SAP\\left_channel.wav";
          else if(option->channel->ItemIndex==1)pszFile="C:\\SAP\\right_channel.wav";
          else  // select channel with stronger signal
          {
             pszFile="C:\\SAP\\left_channel.wav";
             OpenFile(pszFile);
             PCMPOS pcmPos = GetHeadPosition();
             int nFileTotal  =  GetTotalSamples();
             float *tmpData=new float[nFileTotal];
             ReadSamples(pcmPos, tmpData, nFileTotal);
             long int avgsound1=0;
             for(int i=0;i<nFileTotal;i++)
                   avgsound1+=fabs(tmpData[i]);
             delete[] tmpData;
             avgsound1/=nFileTotal;
             ResetContents();

             pszFile="C:\\SAP\\right_channel.wav";
			 OpenFile(pszFile);
             pcmPos = GetHeadPosition();
             nFileTotal  =  GetTotalSamples();
             float *tmpData2=new float[nFileTotal];
             ReadSamples(pcmPos, tmpData2, nFileTotal);
             long int avgsound2=0;
             for(int i=0;i<nFileTotal;i++)
                   avgsound2+=fabs(tmpData2[i]);
             delete[] tmpData2;
             avgsound2/=nFileTotal;
			 ResetContents();
			 if(avgsound1>avgsound2)pszFile="C:\\SAP\\left_channel.wav";
			 else pszFile="C:\\SAP\\right_channel.wav";

		  }

		  OpenFile(pszFile);
        }
        return(1);
}



inline BOOL	CWavIterator::ReadRiffHeader()
{
	DWORD dwRead = 0;
	if(!ReadFile(m_hWaveFile,&m_WaveHeader.chkRiff,8,&dwRead,NULL))
		return FALSE;

	return m_WaveHeader.chkRiff.ckid == ztFOURCC_RIFF;
}


inline BOOL CWavIterator::ReadDataHeader()
{
	DWORD dwRead = 0;
	if(!ReadFile(m_hWaveFile,&m_WaveHeader.chkData,8,&dwRead,NULL))
		return FALSE;

	return m_WaveHeader.chkData.ckid == ztFOURCC_DATA;
}


inline BOOL CWavIterator::ReadFormatHeader()
{
	DWORD dwRead = 0;
	// now we are looking for a fourcc WAVE 
	if(!ReadFile(m_hWaveFile,&m_WaveHeader.fccWave,4,&dwRead,NULL))
		return FALSE;

	// make sure its what we expected..
	if(m_WaveHeader.fccWave != ztFOURCC_WAVE)
		return FALSE;

	// now we can read in the format header
	if(!ReadFile(m_hWaveFile,&m_WaveHeader.chkFmt,8,&dwRead,NULL))
		return FALSE;	

	// we need to know the position so we can offset (at the end)
	DWORD dwEndOfFmt =SetFilePointer(m_hWaveFile,0,NULL,FILE_CURRENT);	
	dwEndOfFmt+= m_WaveHeader.chkFmt.dwSize;

	// make sure its what we expect
	if(m_WaveHeader.chkFmt.ckid != ztFOURCC_FMT)
		return FALSE;

	// read format tag...
	if(!ReadFile(m_hWaveFile,&m_WaveHeader.wFormatTag,2,&dwRead,NULL))
		return FALSE;	

	// if the format is not 1 then we do not support it (1 = PCM)
	if(m_WaveHeader.wFormatTag != 1)
		return FALSE;

	// channels
	if(!ReadFile(m_hWaveFile,&m_WaveHeader.nChannels,2,&dwRead,NULL))
		return FALSE;	


        // call wave splitter to split the sound, decide which channel to use, and make wave iterator
        // use that channels


	// nSamplesPerSec
	if(!ReadFile(m_hWaveFile,&m_WaveHeader.nSamplesPerSec,4,&dwRead,NULL))
		return FALSE;	

	// nAvgBytesPerSec
	if(!ReadFile(m_hWaveFile,&m_WaveHeader.nAvgBytesPerSec,4,&dwRead,NULL))
		return FALSE;	

	// Block Alignment
	if(!ReadFile(m_hWaveFile,&m_WaveHeader.nBlockAlign,2,&dwRead,NULL))
		return FALSE;	

	// Bits Per Sample
	if(!ReadFile(m_hWaveFile,&m_WaveHeader.wBitsPerSample,2,&dwRead,NULL))
		return FALSE;	

	// ok we should be ok as long as we can move to the data chunk...
	return (SetFilePointer (m_hWaveFile,dwEndOfFmt, NULL, FILE_BEGIN) != INVALID_SET_FILE_POINTER);
}



PCMPOS CWavIterator::GetTailPosition()
{
	return FilePosToSamplePos(m_lDataEnd);
}



PCMPOS CWavIterator::GetHeadPosition()
{
	return FilePosToSamplePos(m_lDataStart);
}



int	CWavIterator::ReadSamples(PCMPOS& pos,float* pDest,int nCount)
{
	if(pos == pcmpos_end)
		return 0;// we can not read this..

	// They want data from pos thru pos + nCount...
	long lEnd = pos+nCount;
	int nFilled=0;// how many samples we fill this iteration

	// Clip to end if needed....
	if(lEnd > m_nTotalSamples)
	{
		lEnd = m_nTotalSamples;
		nCount = lEnd-pos;
	}

	// Lets see if we have any of that data stuffed away..
	int nRead = m_BufferMGR.ReadDataSegment(pos,lEnd,pDest);
	if(nRead != 0)
	{
		// Adjust the pos
		pos   += nRead;
		pDest += nRead;// also adjust the destination buffer..
		nFilled = nRead;

		// ok did we get all of it?
		if(nCount == nRead)
		{
			// if this was the last piece of data - then
			// mark their position as such
			if(SamplePosToFilePos(pos) == m_lDataEnd)
				pos=pcmpos_end;

			return nCount;// yup
		}
		// Adjust nCount
		nCount -=nRead;// and this
	}

	
	// ok so all of the data was not in the buffer...(maybe none of it was)
	// We need to read some more data into buffers and try again....
	// recursing into ourselves until the data request has been fulfilled, or
	// we run out of file to look at...
	if(FillNewBuffer(SamplePosToFilePos(pos))>0)
		return nFilled+ReadSamples(pos,pDest,nCount);

	return nFilled;
}
inline int CWavIterator::FillNewBuffer(LONG filePos)
{
	// Try and fill a new buffer..Return 0 if it did not happen...
	// Otherwise return the amount we filled...
	if(m_hWaveFile == NULL)
		return 0;// this is bad

	// move to our position
	DWORD dwTmp = SetFilePointer (m_hWaveFile,filePos, NULL, FILE_BEGIN);
	if(dwTmp == INVALID_SET_FILE_POINTER)
		return 0;//didnt happen

	// ok now we get to read..
	if(!ReadFile(m_hWaveFile,m_pRawSamples,m_nBufferSize*2,&dwTmp,NULL))
		return 0;// uh oh

	// Ok we read some data...
	int nRead = int(dwTmp/2.0f);

	//change big-endian to little-endian for LVD file
	if (IsLVD)
	  {
	  int i;
	  for (i = 0; i < nRead; i++)
		{
		m_pRawSamples[i] = ntohs(m_pRawSamples[i]); //swaps bytes in Word (or Short)
		}
	  }

	if(nRead <=0)
		return 0;// jic

	// Get a buffer and Copy our data into it...
	CZT_PcmBuffer* pCurBuffer = m_BufferMGR.GetBufferForWriting();
	pCurBuffer->SetData(m_pRawSamples,FilePosToSamplePos(filePos),nRead);

	return nRead;
}
inline LONG CWavIterator::SamplePosToFilePos(PCMPOS pos)
{
	return m_lDataStart+(pos*2);// *2 because in wave file samples are 2bytes...
}
inline PCMPOS CWavIterator::FilePosToSamplePos(LONG pos)
{
	// File position to sample position..
	// fileposition-m_lDataStart
	return LONG((pos-m_lDataStart)/2.0f);
}

char *CWavIterator::getFileExtension(char* fileName)
        {
			char* extension = (char*) malloc(25);	//3 is sufficient
			int index=0;
			for(int i=strlen(fileName);i>0;i--)
			{
				if(fileName[i]=='.')
				{
					index=i; //get the last dot in the string
					break;
				}
			}
			strncpy(extension, fileName+index+1, strlen(fileName)-index);
			return extension;
		}

inline BOOL	CWavIterator::ReadLVDHeader()
{
	DWORD dwRead = 0;
	double inDouble;
	if(!ReadFile(m_hWaveFile,&inDouble,8,&dwRead,NULL))
		return FALSE;
	m_LVDHeader.SamplesPerSec = ntohd(inDouble);

	if(!ReadFile(m_hWaveFile,&inDouble,8,&dwRead,NULL))
		return FALSE;
	m_LVDHeader.Channels = ntohd(inDouble);

	if(!ReadFile(m_hWaveFile,&inDouble,8,&dwRead,NULL))
		return FALSE;
	m_LVDHeader.DateTime = ntohd(inDouble);

	if(!ReadFile(m_hWaveFile,&inDouble,8,&dwRead,NULL))
		return FALSE;
	m_LVDHeader.InputRange = ntohd(inDouble);
	//m_LVDHeader.InputRange = SwapBytes_double(m_LVDHeader.InputRange);

	return TRUE;
}

double CWavIterator::ntohd( const double inDouble )	//changes big-endian - > small-qendian
{
   double retVal;
   char *doubleToConvert = ( char* ) & inDouble;
   char *returnDouble = ( char* ) & retVal;

   // swap the bytes into a temporary buffer
   returnDouble[0] = doubleToConvert[7];
   returnDouble[1] = doubleToConvert[6];
   returnDouble[2] = doubleToConvert[5];
   returnDouble[3] = doubleToConvert[4];
   returnDouble[4] = doubleToConvert[3];
   returnDouble[5] = doubleToConvert[2];
   returnDouble[6] = doubleToConvert[1];
   returnDouble[7] = doubleToConvert[0];

   return retVal;
}

