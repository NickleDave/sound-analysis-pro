//---------------------------------------------------------------------------


#ifndef segmentationH
#define segmentationH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "SDL_NumLab.hpp"
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include "options3.h"


//---------------------------------------------------------------------------
class TFrame1 : public TFrame
{
__published:	// IDE-managed Components
    TPanel *segment_panels;
    TNumLab *hp_small_num;
    TNumLab *hp_big_num;
    TNumLab *NumLab3;
    TNumLab *min_stop_durN;
    TNumLab *bout_Stop_N;
    TRadioGroup *segmentation_flip;
    TComboBox *segment_by;
    TStaticText *StaticText32;
    TCheckBox *smoothGraphCB;
    TTrackBar *hp_small;
    TStaticText *StaticText49;
    TCheckBox *adaptThreshCB;
    TStaticText *StaticText50;
    TTrackBar *hp_big;
    TPanel *secondary_segmentation_panel;
    TNumLab *secondaryThresholdTB;
    TComboBox *secondaryFeatureDD;
    TRadioGroup *secondaryComparisonRB;
    TRadioGroup *secondaryLogicRB;
    TCheckBox *secondaryFeatureCB;
    TStaticText *StaticText6;
    TStaticText *StaticText29;
    TTrackBar *min_stop_durTb;
    TTrackBar *bout_stop;
	TStaticText *StaticText30;
	TStaticText *StaticText54;
	TBitBtn *save;


	void __fastcall secondaryFeatureDDChange(TObject *Sender);
	void __fastcall segment_byChange(TObject *Sender);
	void __fastcall segmentation_flipClick(TObject *Sender);
	void __fastcall secondaryComparisonRBClick(TObject *Sender);
	void __fastcall secondaryLogicRBClick(TObject *Sender);
	void __fastcall secondaryFeatureCBClick(TObject *Sender);
	void __fastcall adaptThresholdCBClick(TObject *Sender);
	void __fastcall smoothGraphCBClick(TObject *Sender);
	void __fastcall hp_smallChange(TObject *Sender);
	void __fastcall hp_bigChange(TObject *Sender);
	void __fastcall min_stop_durTbChange(TObject *Sender);
	void __fastcall bout_stopChange(TObject *Sender);
	void __fastcall saveClick(TObject *Sender);

private:	// User declarations
public:		// User declarations
	 __fastcall TFrame1(TComponent* Owner);

};
//---------------------------------------------------------------------------
extern PACKAGE TFrame1 *Frame1;
//---------------------------------------------------------------------------
#endif
