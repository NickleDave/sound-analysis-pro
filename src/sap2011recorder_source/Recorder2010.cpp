//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("david.cpp", Form1);
USEFORM("animals.cpp", animals_form);
USEFORM("table_dialog.cpp", tableDialog);
//---------------------------------------------------------------------------
WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->Title = "Sound Analysis Recorder";
         Application->CreateForm(__classid(TForm1), &Form1);
         Application->CreateForm(__classid(Tanimals_form), &animals_form);
         Application->CreateForm(__classid(TtableDialog), &tableDialog);
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
