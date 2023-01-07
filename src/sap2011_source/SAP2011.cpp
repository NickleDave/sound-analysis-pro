//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <atl\atlmod.h>
#include <atl\atlmod.h>
#include <atl\atlmod.h>
#include <tchar.h>
#include "COM_interface.h"
//---------------------------------------------------------------------------

USEFORM("table_dialog.cpp", tableDialog);
USEFORM("SongMeasures.cpp", Form1);
USEFORM("batchIt.cpp", batch);
USEFORM("C:\Program Files (x86)\Embarcadero\RAD Studio\7.0\ObjRepos\cpp\okcancl1.CPP", OKBottomDlg);
USEFORM("sound_live.cpp", sound_live);
USEFORM("live.cpp", sal);
USEFORM("cluster.cpp", clusterIt);
USEFORM("records.cpp", DataForm);
USEFORM("animals.cpp", animals_form);
USEFORM("OKCNHLP1.cpp", OKHelpBottomDlg);
USEFORM("start.cpp", StartForm);
USEFORM("DVD.cpp", DVD_map);
USEFORM("options3.cpp", option);
USEFORM("open_soundD.cpp", open_sound);
USEFORM("preview.cpp", previewSound);
USEFORM("segmentation.cpp", Frame1); /* TFrame: File Type */
//---------------------------------------------------------------------------
#pragma link "vclnp.lib"
//#pragma resource "*.tlb"	//removed by Alexei

// NOTE: You may derive a class from TComModule and use it as your server's
//       module object. (i.e. Assign an instance of that class to _Module).
//       However, you must *NOT* change the name _Module.
//
TComModule _ProjectModule(0 /*InitATLServer*/);
TComModule &_Module = _ProjectModule;

// The ATL Object map holds an array of _ATL_OBJMAP_ENTRY structures that
// described the objects of your OLE server. The MAP is handed to your
// project's CComModule-derived _Module object via the Init method.
//
BEGIN_OBJECT_MAP(ObjectMap)
  OBJECT_ENTRY(CLSID_SAPObject, TSAPObjectImpl)
END_OBJECT_MAP()
//---------------------------------------------------------------------------
WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->Title = "SAP2011";
		Application->HelpFile = "";
         Application->CreateForm(__classid(TStartForm), &StartForm);
		Application->CreateForm(__classid(TDataForm), &DataForm);
		Application->CreateForm(__classid(Toption), &option);
		Application->CreateForm(__classid(TtableDialog), &tableDialog);
		Application->CreateForm(__classid(Tanimals_form), &animals_form);
		Application->CreateForm(__classid(TOKHelpBottomDlg), &OKHelpBottomDlg);
		Application->CreateForm(__classid(Topen_sound), &open_sound);
		Application->CreateForm(__classid(TpreviewSound), &previewSound);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
