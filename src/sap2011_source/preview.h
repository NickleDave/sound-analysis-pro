//---------------------------------------------------------------------------

#ifndef previewH
#define previewH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "CZT_FeatureCalc.h"
#include "CZT_FeatureImage.h"
#include "options3.h"
//---------------------------------------------------------------------------
class TpreviewSound : public TForm
{
__published:	// IDE-managed Components
	TImage *Image1;
	TButton *Button1;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Image1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
private:
CWavIterator wave;
	CZT_FeatureCalc m_Calculator1;
	CZT_FeatureImage  m_Image;
public:		// User declarations
	__fastcall TpreviewSound(TComponent* Owner);
		AnsiString FileName;
	int sampling_rate;
};
//---------------------------------------------------------------------------
extern PACKAGE TpreviewSound *previewSound;
//---------------------------------------------------------------------------
#endif


