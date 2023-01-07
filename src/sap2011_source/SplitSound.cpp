//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SplitSound.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWavFileSplitter::CWavFileSplitter()
{
	m_hInFile = NULL;
	m_nBufferSize = 256000;   
}


CWavFileSplitter::~CWavFileSplitter()
{
	CloseFile();
}


void CWavFileSplitter::SetBufferSize(int nSize)
{
	if(nSize <2)
		m_nBufferSize = 1000;
	else
	{
		// make sure its an even number
		if((nSize%2) != 0)
			nSize++;
		m_nBufferSize=nSize;
	}
}


void CWavFileSplitter::CloseFile()
{
	if(m_hInFile != NULL)
		CloseHandle(m_hInFile);
	m_hInFile = NULL;
}



BOOL CWavFileSplitter::OpenFile(char* pszFile)
{
	if(pszFile == NULL) return FALSE;

	BOOL bSuccess = FALSE;

	m_hInFile = CreateFile(pszFile,
							GENERIC_READ,                 // open for reading
							0,                            // do not share 
							NULL,                         // no security 
							OPEN_EXISTING,                // existing file only 
							FILE_ATTRIBUTE_NORMAL,        // normal file 
							NULL);                        // no attr. template 


	// if it did not open we can not do anything
	if(m_hInFile == INVALID_HANDLE_VALUE)
		return FALSE;

       //	DWORD dwRead = 0;

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
		CloseHandle(m_hInFile);

	return bSuccess;
}



inline BOOL	CWavFileSplitter::ReadRiffHeader()
{
	DWORD dwRead = 0;
	if(!ReadFile(m_hInFile,&m_WaveHeader.chkRiff,8,&dwRead,NULL))
		return FALSE;

	return m_WaveHeader.chkRiff.ckid == FOURCC_RIFF1;
}



inline BOOL CWavFileSplitter::ReadFormatHeader()
{
	DWORD dwRead = 0;
	// now we are looking for a fourcc WAVE 
	if(!ReadFile(m_hInFile,&m_WaveHeader.fccWave,4,&dwRead,NULL))
		return FALSE;

	// make sure its what we expected..
	if(m_WaveHeader.fccWave != FOURCC_WAVE)
		return FALSE;

	// now we can read in the format header
	if(!ReadFile(m_hInFile,&m_WaveHeader.chkFmt,8,&dwRead,NULL))
		return FALSE;	

	// we need to know the position so we can offset (at the end)
	DWORD dwEndOfFmt =SetFilePointer(m_hInFile,0,NULL,FILE_CURRENT);	
	dwEndOfFmt+= m_WaveHeader.chkFmt.dwSize;

	// make sure its what we expect
	if(m_WaveHeader.chkFmt.ckid != FOURCC_FMT)
		return FALSE;

	// read format tag...
	if(!ReadFile(m_hInFile,&m_WaveHeader.wFormatTag,2,&dwRead,NULL))
		return FALSE;	

	// if the format is not 1 then we do not support it (1 = PCM)
	if(m_WaveHeader.wFormatTag != 1)
		return FALSE;

	// channels
	if(!ReadFile(m_hInFile,&m_WaveHeader.nChannels,2,&dwRead,NULL))
		return FALSE;	

	// nSamplesPerSec
	if(!ReadFile(m_hInFile,&m_WaveHeader.nSamplesPerSec,4,&dwRead,NULL))
		return FALSE;	

	// nAvgBytesPerSec
	if(!ReadFile(m_hInFile,&m_WaveHeader.nAvgBytesPerSec,4,&dwRead,NULL))
		return FALSE;	

	// channels
	if(!ReadFile(m_hInFile,&m_WaveHeader.nBlockAlign,2,&dwRead,NULL))
		return FALSE;	

	// channels
	if(!ReadFile(m_hInFile,&m_WaveHeader.wBitsPerSample,2,&dwRead,NULL))
		return FALSE;	

	// ok we should be ok as long as we can move to the data chunk...
	return (SetFilePointer (m_hInFile,dwEndOfFmt, NULL, FILE_BEGIN) != INVALID_SET_FILE_POINTER);

}


inline BOOL CWavFileSplitter::ReadDataHeader()
{
	DWORD dwRead = 0;
	if(!ReadFile(m_hInFile,&m_WaveHeader.chkData,8,&dwRead,NULL))
		return FALSE;

	return m_WaveHeader.chkData.ckid == FOURCC_DATA;
}



BOOL CWavFileSplitter::SaveChannel(int nChannel, char* pszDestination)
{
	// can not do it without valid input...
	if(m_hInFile == NULL)
		return FALSE;

	// also it has to be a stereo file..
	if(GetChannels() != 2)
		return FALSE;

	// make sure its 16 bit...
	if(m_WaveHeader.wBitsPerSample != 16)
		return FALSE;

	// before we do anything
	// move teh pointer in the source file to the actual data
	DWORD dwPtr = SetFilePointer (m_hInFile, sizeof(m_WaveHeader), NULL, FILE_BEGIN); 	
	
	// make sure it succeeded
	if(dwPtr == INVALID_SET_FILE_POINTER)
		return FALSE;

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

	//int			nChannels = m_WaveHeader.nChannels;// we fix on exit of this function
	DWORD			dwWritten = 0;
	BYTE*			pSamples = new BYTE[(m_nBufferSize+1)*2];
	WORD*			pChannelSamples = new WORD[m_nBufferSize+1];
	WORD*			plSample=NULL;
	WORD*			prSample=NULL;
	WORD*			prNewSample=NULL;
	DWORD			dwRead = 0;	
	BYTE*			lpbEnd;
	DWORD			dwCounter = 0;
	DWORD			dwTotalOut = 0;
	WAVEHDR1		outHeader;

	// first we write out the wave header...	
	PrepareHeaderForWrite(outHeader);
	
	WriteFile(hOutput,&outHeader,sizeof(outHeader),&dwWritten,NULL);

	SetFilePointer (hOutput, dwWritten, NULL, FILE_BEGIN); 

	// buffersize *2 since 2 bytes in a word
	ReadFile(m_hInFile,pSamples,(m_nBufferSize*2),&dwRead,NULL);

	while(dwRead > 0)
	{
		lpbEnd = pSamples;
		lpbEnd += dwRead;

		plSample =(WORD*) &pSamples[0];// first word in buffer
		prSample =(WORD*) &pSamples[2];// second word in buffer
		prNewSample = &pChannelSamples[0];

		dwCounter = 0;

		// loop through samples and build output buffer
		while((void*)plSample < (void*)lpbEnd)
		{	
			switch(nChannel)
			{
				case left:
					*prNewSample = *plSample;
					break;
				case right:
					*prNewSample = *prSample;
					break;
			}
			// increase pointers
			plSample+=2;
			prSample+=2;
			// increase counter
			dwCounter++;
			prNewSample++;
		}
		// ok write out our buffer...
		WriteFile(hOutput,(BYTE*)pChannelSamples,(dwCounter*2),&dwWritten,NULL);
		dwTotalOut += dwWritten;

		// read some more
		ReadFile(m_hInFile,pSamples,(m_nBufferSize*2),&dwRead,NULL);
	}

	// set the total output size 	
	outHeader.chkData.dwSize =dwTotalOut; 
	// the dwSize of the riff header includes not only the size of the chunk but
	// the total size of the pcm data as well (IE: it treats the whole file as a chunk)
	outHeader.chkRiff.dwSize = dwTotalOut+(sizeof(WAVEHDR1) - 8);

	SetFilePointer (hOutput, 0, NULL, FILE_BEGIN); 
	
	WriteFile(hOutput,&outHeader,sizeof(outHeader),&dwWritten,NULL);

	// close output...
	CloseHandle(hOutput);

	// cleanup
	delete [] pSamples;
	delete [] pChannelSamples;

	return TRUE;
}


void CWavFileSplitter::PrepareHeaderForWrite(WAVEHDR1& hdr)
{	
	hdr.chkRiff.ckid = FOURCC_RIFF1;
       	hdr.fccWave = FOURCC_WAVE;
       	hdr.chkFmt.ckid = FOURCC_FMT;
	hdr.chkFmt.dwSize =16;
       	hdr.wFormatTag = 1;
	hdr.nChannels = 1;
	hdr.nSamplesPerSec = m_WaveHeader.nSamplesPerSec;//rate;
	hdr.nBlockAlign = 2;
	hdr.nAvgBytesPerSec =hdr.nSamplesPerSec * 2;
	hdr.wBitsPerSample = 16;
	hdr.chkData.ckid = FOURCC_DATA;	
}
