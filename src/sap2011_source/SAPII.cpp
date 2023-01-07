//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("SAPII.res");
USEFORM("start.cpp", StartForm);
USEFORM("records.cpp", DataForm);
USEFORM("options3.cpp", option);
USEUNIT("tapers.cpp");
USEFORM("batchIt.cpp", batch);
USEFORM("cluster.cpp", clusterIt);
USEUNIT("CWavIterator.cpp");
USEUNIT("CZT_FeatureCalc.cpp");
USEUNIT("CZT_FeatureImage.cpp");
USEUNIT("CZT_PcmBuffer.cpp");
USEUNIT("CZT_PcmBufferFilter.cpp");
USEUNIT("CZT_PcmBufferMgr.cpp");
USEUNIT("CZT_PcmEq.cpp");
USEFORM("DVD.cpp", DVD_map);
USEFORM("live.cpp", sal);
USEFORM("SongMeasures.cpp", Form1);
USEUNIT("SplitSound.cpp");
USEFORM("table_dialog.cpp", tableDialog);
USEFORM("sound_live.cpp", sound_live);
USEFORM("animals.cpp", animals_form);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->HelpFile = "C:\\Program Files\\Sound Analysis Pro\\SAP_HELP.HLP";
                 Application->Title = "SAPII";
                 Application->CreateForm(__classid(TStartForm), &StartForm);
                 Application->CreateForm(__classid(TDataForm), &DataForm);
                 Application->CreateForm(__classid(Toption), &option);
                 Application->CreateForm(__classid(TtableDialog), &tableDialog);
                 Application->CreateForm(__classid(Tanimals_form), &animals_form);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
