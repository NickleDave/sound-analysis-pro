//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("SAP.res");
USEFORM("start.cpp", StartForm);
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
USEFORM("records.cpp", DataForm);
USEFORM("SongMeasures.cpp", Form1);
USEUNIT("tapers.cpp");
USEFORM("batchIt.cpp", batch);
USEFORM("table_dialog.cpp", tableDialog);
USEFORM("options3.cpp", option);
USEUNIT("SplitSound.cpp");
USELIB("C:\MATLAB7\extern\lib\win32\borland\bc54\mclxlmain.lib");
USELIB("C:\MATLAB7\extern\lib\win32\borland\bc54\libeng.lib");
USELIB("C:\MATLAB7\extern\lib\win32\borland\bc54\libfixedpoint.lib");
USELIB("C:\MATLAB7\extern\lib\win32\borland\bc54\libmat.lib");
USELIB("C:\MATLAB7\extern\lib\win32\borland\bc54\libmex.lib");
USELIB("C:\MATLAB7\extern\lib\win32\borland\bc54\libmwservices.lib");
USELIB("C:\MATLAB7\extern\lib\win32\borland\bc54\libmx.lib");
USELIB("C:\MATLAB7\extern\lib\win32\borland\bc54\libut.lib");
USELIB("C:\MATLAB7\extern\lib\win32\borland\bc54\mclcom.lib");
USELIB("C:\MATLAB7\extern\lib\win32\borland\bc54\mclcommain.lib");
USELIB("C:\MATLAB7\extern\lib\win32\borland\bc54\mclmcr.lib");
USELIB("C:\MATLAB7\extern\lib\win32\borland\bc54\mclmcrrt.lib");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TStartForm), &StartForm);
                 Application->CreateForm(__classid(TDataForm), &DataForm);
                 Application->CreateForm(__classid(Toption), &option);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
