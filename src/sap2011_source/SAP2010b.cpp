//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("SongMeasures.cpp", Form1);
USEFORM("live.cpp", sal);
USEFORM("start.cpp", StartForm);
USEFORM("options3.cpp", option);
USEFORM("DVD.cpp", DVD_map);
USEFORM("sound_live.cpp", sound_live);
USEFORM("batchIt.cpp", batch);
USEFORM("table_dialog.cpp", tableDialog);
USEFORM("cluster.cpp", clusterIt);
USEFORM("animals.cpp", animals_form);
USEFORM("records.cpp", DataForm);
USEFORM("c:\program files\embarcadero\rad studio\7.0\ObjRepos\Cpp\okcancl1.CPP", OKBottomDlg);
USEFORM("OKCNHLP1.cpp", OKHelpBottomDlg);
USEFORM("open_soundD.cpp", open_sound);
USEFORM("preview.cpp", previewSound);
//---------------------------------------------------------------------------
WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->Title = "SAP2010";
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
