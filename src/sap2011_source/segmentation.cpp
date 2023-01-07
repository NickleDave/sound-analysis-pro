//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>

#ifndef SongMeasuresH
#include "SongMeasures.h"
#endif

#ifndef batchItH
#include "batchIt.h"
#endif

#ifndef sound_liveH
#include "sound_live.h"
#endif

#pragma hdrstop

#include "segmentation.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SDL_NumLab"
#pragma link "SDL_NumLab"
#pragma link "SDL_NumLab"
#pragma resource "*.dfm"
TFrame1 *Frame1;
//---------------------------------------------------------------------------
__fastcall TFrame1::TFrame1(TComponent* Owner)
	: TFrame(Owner)
{

}
//---------------------------------------------------------------------------

void __fastcall TFrame1::segment_byChange(TObject *Sender) {

	//option->pri_segmented_feature = segment_by->ItemIndex;
	if(option->module==0)// explore & score mode
	{
	  if (Sender == Form1->Frame11->segment_by) {
         option->pri_segmented_feature1 = segment_by->ItemIndex;
         Form1->ComboBox1Change_sf(Sender);
	  } else if (Sender == Form1->Frame12->segment_by) {
        option->pri_segmented_feature2 = segment_by->ItemIndex;
        Form1->ComboBox1Change_sf2(Sender);
      }
	}
	else if(option->module==1)// sound live mode
	{
	  // sound_live->(Sender);
	  option->pri_segmented_feature = segment_by->ItemIndex;
	  //sound_live->segment_sf(Sender);
	}
	else if(option->module==2)// batch mode
	{
	   option->pri_segmented_feature = segment_by->ItemIndex;
	   batch->segment_sf(Sender);
	}

}

void __fastcall TFrame1::secondaryFeatureDDChange(TObject *Sender) {
	//option->sec_segmented_feature = secondaryFeatureDD->ItemIndex;
	if(option->module==0)// explore & score mode
	{
	  if (Sender == Form1->Frame11->secondaryFeatureDD) {
		 option->sec_segmented_feature1 = secondaryFeatureDD->ItemIndex;
		 Form1->secondaryFeatureDDChange_sf(Sender);
	  } else if (Sender == Form1->Frame12->secondaryFeatureDD) {
		option->sec_segmented_feature2 = secondaryFeatureDD->ItemIndex;
		Form1->secondaryFeatureDDChange_sf2(Sender);
	  }
	}
	else if(option->module==1)// sound live mode
	{
	  // sound_live->(Sender);
	  option->sec_segmented_feature=secondaryFeatureDD->ItemIndex;
	}
	else if(option->module==2)// batch mode
	{
	  // batch->(Sender);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFrame1::secondaryComparisonRBClick(TObject *Sender) {
	//option->sec_lt = secondaryComparisonRB->ItemIndex;
	if(option->module==0)// explore & score mode
	{
	  if (Sender == Form1->Frame11->secondaryComparisonRB) {
		 option->sec_lt1 = secondaryComparisonRB->ItemIndex;
         Form1->secondaryComparisonRBClick_sf(Sender);
	  } else if (Sender == Form1->Frame12->secondaryComparisonRB) {
        option->sec_lt2 = secondaryComparisonRB->ItemIndex;
        Form1->secondaryComparisonRBClick_sf2(Sender);
      }
	}
	else if(option->module==1)// sound live mode
	{
	  // sound_live->(Sender);
	  option->sec_lt = secondaryComparisonRB->ItemIndex;
	}
	else if(option->module==2)// batch mode
	{
	  // batch->(Sender);
	}
}
// ---------------------------------------------------------------------------
void __fastcall TFrame1::segmentation_flipClick(TObject *Sender) {
	//option->reverse_segmentation = segmentation_flip->ItemIndex;
	if(option->module==0)// explore & score mode
	{
	  if (Sender == Form1->Frame11->segmentation_flip) {
		 option->reverse_segmentation1 = segmentation_flip->ItemIndex;
         Form1->segmentation_flipClick_sf(Sender);
	  } else if (Sender == Form1->Frame12->segmentation_flip) {
        option->reverse_segmentation2 = segmentation_flip->ItemIndex;
        Form1->segmentation_flipClick_sf2(Sender);
      }
	}
    else if(option->module==1)// sound live mode
	{
	  // sound_live->(Sender);
	  option->reverse_segmentation = segmentation_flip->ItemIndex;
	}
	else if(option->module==2)// batch mode
	{
	  // batch->(Sender);
	}

}

// ---------------------------------------------------------------------------
void __fastcall TFrame1::smoothGraphCBClick(TObject *Sender) {
	if (smoothGraphCB->Checked) {
		//option->smooth_feature = true;
		hp_small->Visible = true;
		hp_small_num->Visible = true;
		StaticText49->Visible = true;
        //option->hp_small = hp_small->Position;
	}
	else {
		//option->smooth_feature = false;
		hp_small->Visible = false;
		hp_small_num->Visible = false;
		StaticText49->Visible = false;
        //option->hp_small = 0;
	}
	if(option->module==0)// explore & score mode
	{
		if (Sender == Form1->Frame11->smoothGraphCB)  {
		   if (smoothGraphCB->Checked) {
			  option->smooth_feature1 = true;
		   } else {
			  option->smooth_feature1 = false;
		   }
           Form1->smoothGraphClick_sf(Sender);
        } else if (Sender == Form1->Frame12->smoothGraphCB) {
          if (smoothGraphCB->Checked) {
		      option->smooth_feature2 = true;
           } else {
              option->smooth_feature2 = false;
           }
          Form1->smoothGraphClick_sf2(Sender);
        }
	}
	else if(option->module==1)// sound live mode
	{
	  // sound_live->(Sender);
	  if (smoothGraphCB->Checked) option->smooth_feature = true;
	  else option->smooth_feature = false;
	}

	else if(option->module==2)// batch mode
	{
	  // batch->(Sender);
	}
}
// ---------------------------------------------------------------------------
void __fastcall TFrame1::adaptThresholdCBClick(TObject *Sender) {
	if (adaptThreshCB->Checked) {
		//option->adapt_threshold = true;
		hp_big->Visible = true;
		hp_big_num->Visible = true;
		StaticText50->Visible = true;
	} else {
		//option->adapt_threshold = false;
		hp_big->Visible = false;
		hp_big_num->Visible = false;
		StaticText50->Visible = false;
	}
	if(option->module==0)// explore & score mode
	{
	  if (Sender == Form1->Frame11->adaptThreshCB) {
		 if (adaptThreshCB->Checked) {
			  option->adapt_threshold1 = true;
		   } else {
			  option->adapt_threshold1 = false;
		   }
		 Form1->adaptThresholdCBClick_sf(Sender);
	  } else if (Sender == Form1->Frame12->adaptThreshCB) {
		if (adaptThreshCB->Checked) {
			  option->adapt_threshold2 = true;
		   } else {
			  option->adapt_threshold2 = false;
		   }
		Form1->adaptThresholdCBClick_sf2(Sender);
	  }
	}
	else if(option->module==1)// sound live mode
	{
	  // sound_live->(Sender);
	  if (adaptThreshCB->Checked)option->adapt_threshold1 = true;
	  else option->adapt_threshold1 = false;
	}

	else if(option->module==2)// batch mode
	{
	  // batch->(Sender);
	}
}
// ---------------------------------------------------------------------------

void __fastcall TFrame1::hp_smallChange(TObject *Sender) {
	//option->hp_small = hp_small->Position * 100;
	option->HP_small->Position = hp_small->Position;
	hp_small_num->Value = hp_small->Position;
	if(option->module==0)// explore & score mode
	{
	  if (Sender == Form1->Frame11->hp_small) {
		 option->hp_small1 = hp_small->Position * 100;
		 Form1->hp_smallChange_sf(Sender);
	  } else if (Sender == Form1->Frame12->hp_small){
         option->hp_small2 = hp_small->Position * 100;
         Form1->hp_smallChange_sf(Sender);
      }
	}
	else if(option->module==1)// sound live mode
	{
	  // sound_live->(Sender);
	  option->hp_small = hp_small->Position * 100;
	}
	else if(option->module==2)// batch mode
	{
	  batch->hp_smallChange_sf(Sender);
	}
}
// ---------------------------------------------------------------------------

void __fastcall TFrame1::hp_bigChange(TObject *Sender) {
	float x;
	x = hp_big->Position;
	x /= 30; // range 1.0 to 10.0
	//option->hp_big = exp(x);
	option->HP_big->Position = hp_big->Position;
	hp_big_num->Value = hp_big->Position;
	if(option->module==0)// explore & score mode
	{
	  if (Sender == Form1->Frame11->hp_big) {
		 option->hp_big1 = exp(x);
		 Form1->hp_bigChange_sf(Sender);
	   } else if (Sender == Form1->Frame12->hp_big) {
		 option->hp_big2 = exp(x);
		 Form1->hp_bigChange_sf(Sender);
	   }

	  } else if(option->module==1)// sound live mode
	{
	  // sound_live->(Sender);
	  option->hp_big = exp(x);
	}
	else if(option->module==2)// batch mode
	{
	  batch->hp_smallChange_sf(Sender);
	}
}
// ---------------------------------------------------------------------------


void __fastcall TFrame1::secondaryFeatureCBClick(TObject *Sender) {
	if (secondaryFeatureCB->Checked) {
		secondaryLogicRB->Visible = true;
		secondaryFeatureDD->Visible = true;
		secondaryComparisonRB->Visible = true;
		secondaryThresholdTB->Visible = true;
		//option->sec_segmentation = true;
	}
	else {
		secondaryLogicRB->Visible = false;
		secondaryFeatureDD->Visible = false;
		secondaryComparisonRB->Visible = false;
		secondaryThresholdTB->Visible = false;
		//option->sec_segmentation = false;
	}
	if(option->module==0)// explore & score mode
	{
	  if (Sender == Form1->Frame11->secondaryFeatureCB) {
		 if (secondaryFeatureCB->Checked) {
			option->sec_segmentation1 = true;
		 } else {
		   option->sec_segmentation1 = false;
		 }
		 Form1->secondaryFeatureCBClick_sf(Sender);
	  } else if (Sender == Form1->Frame12->secondaryFeatureCB) {
		if (secondaryFeatureCB->Checked) {
			option->sec_segmentation2 = true;
		 } else {
		   option->sec_segmentation2 = false;
		 }
		Form1->secondaryFeatureCBClick_sf2(Sender);
	  }
	}
	else if(option->module==1)// sound live mode
	{
	  // sound_live->(Sender);
	  if (secondaryFeatureCB->Checked) option->sec_segmentation1 = true;
	  else option->sec_segmentation1 = false;
	}

	else if(option->module==2)// batch mode
	{
	  if (secondaryFeatureCB->Checked) option->sec_segmentation1 = true;
	  else option->sec_segmentation1 = false;
	  batch->secondaryFeatureCBClick_sf(Sender);
	}
}
// ---------------------------------------------------------------------------


void __fastcall TFrame1::secondaryLogicRBClick(TObject *Sender)
{
	//option->sec_logic = secondaryLogicRB->ItemIndex;
	if(option->module==0)// explore & score mode
	{
	  if (Sender == Form1->Frame11->secondaryLogicRB) {
		 option->sec_logic1 = secondaryLogicRB->ItemIndex;
		 Form1->secondaryLogicRBClick_sf(Sender);
	  } else if (Sender == Form1->Frame12->secondaryLogicRB) {
		option->sec_logic2 = secondaryLogicRB->ItemIndex;
		Form1->secondaryLogicRBClick_sf2(Sender);
	  }
	}
	else if(option->module==1)// sound live mode
	{
	  // sound_live->(Sender);
	  option->sec_logic = secondaryLogicRB->ItemIndex;
	}
	else if(option->module==2)// batch mode
	{
	  // batch->(Sender);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFrame1::min_stop_durTbChange(TObject *Sender)
{
	 option->stop_duration_thresh = min_stop_durTb->Position;
	 min_stop_durN->Value=min_stop_durTb->Position;
	 if(option->module==0)// explore & score mode
	{
	  if (Sender == Form1->Frame11->min_stop_durTb) Form1->min_stop_durTbChange_sf(Sender);
	  else if (Sender == Form1->Frame12->min_stop_durTb) Form1->min_stop_durTbChange_sf2(Sender);
	}
	else if(option->module==1)// sound live mode
	{
	  // sound_live->(Sender);
	}
	else if(option->module==2)// batch mode
	{
	  // batch->(Sender);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFrame1::bout_stopChange(TObject *Sender)
{
	 option->bout_duration_thresh = bout_stop->Position;
	 bout_Stop_N->Value=bout_stop->Position;
	if(option->module==0)// explore & score mode
	{
	 if (Sender == Form1->Frame11->bout_stop) Form1->bout_stopChange_sf(Sender);
	 else if (Sender == Form1->Frame12->bout_stop) Form1->bout_stopChange_sf2(Sender);
	}
	else if(option->module==1)// sound live mode
	{
	  // sound_live->(Sender);
	}
	else if(option->module==2)// batch mode
	{
	  // batch->(Sender);
	}
}
//---------------------------------------------------------------------------




//Fix it
void __fastcall TFrame1::saveClick(TObject *Sender)
{
  // update options by updating all frame params in options vars
  option->pri_segmented_feature = segment_by->ItemIndex;
  //option->pri_thresh_raw=NumLab3->Value;
  option->sec_segmented_feature = secondaryFeatureDD->ItemIndex;
  option->sec_thresh_raw=secondaryThresholdTB->Value;
  option->reverse_segmentation = segmentation_flip->ItemIndex;
  option->sec_lt = secondaryComparisonRB->ItemIndex;
  if (smoothGraphCB->Checked) option->smooth_feature = true;
  else option->smooth_feature = false;
  if (adaptThreshCB->Checked) option->adapt_threshold = true;
  else option->adapt_threshold = false;
  option->hp_small = hp_small->Position * 100;
  float x=hp_big->Position;
  x /= 30;
  option->hp_big = exp(x);
  if (secondaryFeatureCB->Checked) option->sec_segmentation = true;
  else option->sec_segmentation = false;
  option->sec_logic = secondaryLogicRB->ItemIndex;
  option->stop_duration_thresh = min_stop_durTb->Position;
  option->bout_duration_thresh = bout_stop->Position;
  if(option->module==1) // sound live mode
  {
	 sound_live->save_frame_sf(Sender);
  }
  else if(option->module==2)// batch mode
  {
	  batch->save_frame_sf(Sender);
  }
  // save frame params to database

}
//---------------------------------------------------------------------------

