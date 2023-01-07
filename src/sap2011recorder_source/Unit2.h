//---------------------------------------------------------------------------

#ifndef Unit2H
#define Unit2H
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class boo : public TThread
{
private:
void __fastcall click();
void __fastcall set();
void __fastcall doit();
void __fastcall progress();
void __fastcall name();
void __fastcall four1(float *data, unsigned long nn, int isign);
void __fastcall realft(float *data, unsigned long n, int isign);
int I;
float J;
AnsiString str, newFile;
protected:
        void __fastcall Execute();
public:
        __fastcall boo(bool CreateSuspended);
};
//---------------------------------------------------------------------------
#endif
 