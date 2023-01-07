//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "david.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall Unit1::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall ofer::ofer(bool CreateSuspended)
        : TThread(CreateSuspended)
{
  FreeOnTerminate=true;
  Priority=tpNormal;


}
//---------------------------------------------------------------------------
void __fastcall ofer::Execute()
{
  Synchronize(click);      //---- Place thread code here ----
}
//---------------------------------------------------------------------------

void __fastcall ofer::click(void)
{
 Form1->buttonClick(this);

}