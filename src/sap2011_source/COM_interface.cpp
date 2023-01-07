 // 1.1
// UNIT1 : Implementation of TSAPObjectImpl (CoClass: SAPObject, Interface: ISAPObject)

#include <vcl.h>
#pragma hdrstop

#include "COM_interface.h"
#include "SongMeasures.h"
#include "Start.h"

/////////////////////////////////////////////////////////////////////////////
// TSAPObjectImpl

STDMETHODIMP TSAPObjectImpl::DoBatch()
{
  Form1->DoBatchClick(Application);
  return S_OK;
}

STDMETHODIMP TSAPObjectImpl::SoundList1Add(BSTR FileName)
{
  /*wchar_t* wstr = FileName;
  char* ascii = new char[wcslen(wstr) + 1];
  wcstombs( ascii, wstr, wcslen(wstr) );
  Form1->List1->Items->Add(ascii);*/
  int len = (int) SysStringLen(FileName);
  char *cvt;

  cvt = (char *) malloc(1+len);
  WideCharToMultiByte( CP_ACP, 0, FileName, -1, cvt, len, NULL, NULL );
  cvt[len] = '\0';
  Form1->List1->Items->Add(cvt);
  free(cvt);
  return S_OK;
}

STDMETHODIMP TSAPObjectImpl::SoundList1Clear()
{
  Form1->List1->Items->Clear();
  return S_OK;
}

STDMETHODIMP TSAPObjectImpl::SoundList2Add(BSTR FileName)
{
  /*wchar_t* wstr = FileName;
  char* ascii = new char[wcslen(wstr) + 1];
  wcstombs( ascii, wstr, wcslen(wstr) );
  Form1->List2->Items->Add(ascii);*/
  int len = (int) SysStringLen(FileName);
  char *cvt;

  cvt = (char *) malloc(1+len);
  WideCharToMultiByte( CP_ACP, 0, FileName, -1, cvt, len, NULL, NULL );
  cvt[len] = '\0';
  Form1->List2->Items->Add(cvt);
  free(cvt);
  return S_OK;
}

STDMETHODIMP TSAPObjectImpl::SoundList2Clear()
{
  Form1->List2->Items->Clear();
  return S_OK;
}

STDMETHODIMP TSAPObjectImpl::Init(long ButtonNumber)
{
  if (ButtonNumber==1) {
	 StartForm->singleClick(Application);
  }
  return S_OK;
}

STDMETHODIMP TSAPObjectImpl::SetBatchDir(BSTR DirName)
{
  /*wchar_t* wstr = DirName;
  char* ascii = new char[wcslen(wstr) + 1];
  wcstombs( ascii, wstr, wcslen(wstr) );
  Form1->DirectoryListBox1->Directory=ascii;*/
  int len = (int) SysStringLen(DirName);
  char *cvt;

  cvt = (char *) malloc(1+len);
  WideCharToMultiByte( CP_ACP, 0, DirName, -1, cvt, len, NULL, NULL );
  cvt[len] = '\0';
  Form1->DirectoryListBox1->Directory=cvt;
  free(cvt);
  return S_OK;
}


STDMETHODIMP TSAPObjectImpl::DeleteRecords()
{
  DataForm->similarity->Active=false;
  DataForm->similarity->EmptyTable();
  DataForm->similarity->Active=true;
  return S_OK;
}


STDMETHODIMP TSAPObjectImpl::SetBatchMode(long BatchMode)
{
  if (BatchMode<0 || BatchMode>3) BatchMode = 1;
  Form1->MtimesN->ItemIndex = BatchMode;
  return S_OK;
}

/*
STDMETHODIMP TSAPObjectImpl::send_vector()
{
  int a[10];
  return a;//S_OK;
}
*/
