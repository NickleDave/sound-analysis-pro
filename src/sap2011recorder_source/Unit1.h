//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class ofer : public TThread
{
private:
void __fastcall ofer::click(void);
protected:
        void __fastcall Execute();
public:
        __fastcall ofer(bool CreateSuspended);
};
//---------------------------------------------------------------------------
#endif
 