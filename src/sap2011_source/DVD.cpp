//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "CZT_FeatureCalc.h"
#include "DVD.h"
#include "options3.h"
#include "records.h"
//#include <mmsystem>
#include "start.h"
#include "animals.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SDL_NumLab"
#pragma link "SDL_plot3d"
#pragma link "SDL_rchart"
#pragma link "SDL_Rot3D"
#pragma link "SDL_Rot3D"
#pragma resource "*.dfm"
#define user_defined  19
#define syllable_table_type   0
#define ms_table_type         1

TDateTime hatched;

vector<float>median_vector;

TDVD_map *DVD_map;
//---------------------------------------------------------------------------
__fastcall TDVD_map::TDVD_map(TComponent* Owner)
        : TForm(Owner)
{
 buffer=0; residuals=interval->Position-slide->Position;
    if(clear->Checked)add_data=interval->Position;
    else add_data=slide->Position;
 option->squiz=1;
 option->bitmapHeight=256;
 option->select_table=0;
 option->DVD=true;
 days=50;// for the duration histogram matrix size...
 red=255; green=255; blue=0;
 xVar="duration";
 yVar="mean_FM";
 Xfactor=option->deviationDuration->Value;
 Yfactor=option->deviationFM->Value;
 xfactor=Xfactor;yfactor=Yfactor;
 key_response=true;
 speed->Max=-5;
 dot= 1;
  // Dina
  color[0]=clRed; color[1]=clBlue; color[2]=clLime; color[3]=clYellow; color[4]=(TColor)RGB(0,0,0); color[5]=clFuchsia; color[6]=clOlive; color[7]=clSilver; color[8]=clGreen; color[9]=clAqua;
  color[10]=TColor(RGB(180,250,250)); color[11]=TColor(RGB(250,180,250));color[12]=TColor(RGB(250,250,180));color[13]=TColor(RGB(180,180,250));color[14]=TColor(RGB(180,250,180));color[15]=TColor(RGB(250,250,250));
  color[16]=TColor(RGB(150,150,50));color[17]=TColor(RGB(50,50,150)); color[user_defined]=clRed;


  median_vector.reserve(100);
  daily_means=new float*[11];
  dur=new float*[11]; mean_pitch=new float*[11]; var_pitch=new float*[11]; mean_fm=new float*[11];
  var_fm=new float*[11]; mean_ent=new float*[11]; var_ent=new float*[11]; mean_goodness=new float*[11];
  var_goodness=new float*[11];

  for(int i=0;i<11;i++)
  {
   daily_means[i]=new float[11];
   dur[i]=new float[101];    mean_pitch[i]=new float[101]; var_pitch[i]=new float[101];
   mean_fm[i]=new float[101]; var_fm[i]=new float[101]; mean_ent[i]=new float[101];
   var_ent[i]=new float[101]; mean_goodness[i]=new float[101]; var_goodness[i]=new float[101];

  }
  for(int i=0;i<10;i++) for(int j=0;j<10;j++) daily_means[i][j]=0;

}
//---------------------------------------------------------------------------


void __fastcall TDVD_map::Timer1Timer(TObject *Sender)
{
 short x,y;
 static int rx=0;
 if(display->ActivePage==syll_3D && rotate->Checked)
 {
   if(rx<30)rx++;
   else rx=0;
   if(rx<5) chart3D->ViewAngleX++;
   else if(rx<10) chart3D->ViewAngleY++;
   else if(rx<15) chart3D->ViewAngleZ++;
   else if(rx<20) chart3D->ViewAngleZ--;
   else if(rx<25) chart3D->ViewAngleY--;
   else if(rx<30) chart3D->ViewAngleX--;
 }

 try{
     if(backword->Down)
     {
      if(Location->Position>interval->Position+2)Location->Position-=slide->Position;//add_data;
      else Location->Position=Location->Max-interval->Position;
     }
     else
     {
      if(Location->Position<Location->Max-interval->Position-2)Location->Position+=slide->Position;//add_data;
      else Location->Position=2;
     }


     if(Location->SelEnd && Location->Position>=Location->SelEnd) Location->Position=Location->SelStart;


 if(table_type==syllable_table_type)
 {
// cumulative 2d syllable movies
     if(display->ActivePageIndex==2)
     {
        for(int i=0;i<slide->Position;i++)
        {
          x=find_feature(featureX->ItemIndex)*xScale->Position + xOffset->Position; if(x<1)x=1; if(x>549)x=549;
          y=find_feature(featureY->ItemIndex)*(100-yScale->Position) - yOffset->Position; y=400-y; if(y<1)y=1; if(y>400)y=400;
          if(jitter->Checked){x+=(float)random(5); y+=(float)random(5);}
          Image1->Canvas->Brush->Color=set_color();
          Image1->Canvas->FillRect(Rect(x,y,x+4-dot_size->ItemIndex,y+4-dot_size->ItemIndex));
          DataForm->MyQuery1->Next();
        }
     }


// 2d syllable movies
     else if(display->ActivePageIndex==1)
     {

		plot_data(0);
		/*
		if(sound->Checked)
          {
		  if(x<-2)int PlaySound("click0.wav", NULL, SND_SYNC);
		  else if(x<-1.75)PlaySound("click1.wav", NULL, SND_SYNC);
          else if(x<-1.5)PlaySound("click2.wav", NULL, SND_SYNC);
          else if(x<-1.25)PlaySound("click3.wav", NULL, SND_SYNC);
          else if(x<-1.0)PlaySound("click4.wav", NULL, SND_SYNC);
		  else if(x<-0.75)PlaySound("click5.wav", NULL, SND_SYNC);
          else if(x<-0.5)PlaySound("click6.wav", NULL, SND_SYNC);
          else if(x<-0.25)PlaySound("click7.wav", NULL, SND_SYNC);
          else if(x<0.0)PlaySound("click8.wav", NULL, SND_SYNC);
          else if(x<0.25)PlaySound("click9.wav", NULL, SND_SYNC);
          else if(x<0.5)PlaySound("click10.wav", NULL, SND_SYNC);
          else if(x<0.75)PlaySound("click11.wav", NULL, SND_SYNC);
          else if(x<1.0)PlaySound("click12.wav", NULL, SND_SYNC);
          else if(x<1.25)PlaySound("click13.wav", NULL, SND_SYNC);
          else if(x<1.5)PlaySound("click14.wav", NULL, SND_SYNC);
          else if(x<1.75)PlaySound("click15.wav", NULL, SND_SYNC);
          else if(x<2.0)PlaySound("click16.wav", NULL, SND_SYNC);
          else PlaySound("click17.wav", NULL, SND_SYNC);
		  } */

	 }


// 3d syllables movies
     else if(display->ActivePageIndex==3)
     {
	   plot_data(1);
	 }

  }// end syllable table type
// raw features movies
  else if(display->ActivePageIndex==4)
  {
     plot_raw_features();
  }

   }catch(...)
       {
           Timer1->Enabled=false;
           MessageDlg("Could not read data!", mtWarning, TMsgDlgButtons() << mbOK, 0);
       }
  //DataForm->MyQuery1->UnPrepare();
}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::speedChange(TObject *Sender)
{

  speed_val->Value=1000.0/(float)(-speed->Position);
  Timer1->Interval=-speed->Position;
}

//---------------------------------------------------------------------------

void TDVD_map::drawImage()
{
     for(int slice=1;slice<256;slice++)
     {
        int color;
        CZT_FeatImgSlice* pCurrentSlice = new CZT_FeatImgSlice;
        for(int i=1;i<256;i++)
        {
          int x=DVDdata[slice][i];
          switch(x)
          {
             case 0: color=-32766; break;
             case 1: color=0; break;
             case 2: color=10000; break;
             case 3: color=20000; break;
             case 4: color=30000; break;
          }
          /*if(DVDdata[slice][i]) color=32766;
          else color=-32766;                   */
          pCurrentSlice->spectral[i] =(short)color;
        }
     // Add acoustics to the slice
     pCurrentSlice->pitchGoodness=0;
     pCurrentSlice->entropy=0;
     pCurrentSlice->fm=0;
     pCurrentSlice->peakfreq=0;
     pCurrentSlice->pitch=0;
     pCurrentSlice->DerivPow=0;
     pCurrentSlice->Sound=0;
     pCurrentSlice->Bout=0;
     DVD_Image.AddSlice(pCurrentSlice);
   }
}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::open_tableClick(TObject *Sender)
{
   DataForm->TheTable=DataForm->Records;
   DataForm->openTableClick(this);
   str=DataForm->TheTable->Fields->Fields[0]->FieldName;
   if(str=="recnum")
   {
        if(display->ActivePage==raw_features_2D)display->ActivePage=graph;
        table_type=syllable_table_type;
   }
   else if(str=="time")
   {
        table_type=ms_table_type;
        DataForm->initiate_handler(DataForm->MyConnection1->Database, DataForm->TheTable->TableName);
        display->ActivePage=raw_features_2D;
		raw_features_chart->DataColor=clRed;
        raw_features_chart->Scale1X->RangeHigh=3000;
        raw_features_chart->Scale1X->RangeLow=0;
        raw_features_chart->Scale1Y->RangeHigh=1000;
        raw_features_chart->Scale1Y->RangeLow=0;
        displayChange(this);
   }
   else
   {
     MessageDlg("Table type was not recognized as syllable table or as raw features table -- please try again.", mtError, TMsgDlgButtons() << mbOK, 0);
     return;
   }
   table_name->Caption=DataForm->TheTable->TableName;

   /*
   if(DataForm->retrieve_animal(table_name->Caption))
   {

   bird_name->Caption=DataForm->MyQuery1->FieldValues["bird_name"];
   model->Caption=DataForm->MyQuery1->FieldValues["model"];
   TDateTime hatch=DataForm->MyQuery1->FieldValues["hatched"];
   hatched=hatch;
   TDateTime traintime=DataForm->MyQuery1->FieldValues["training"];
   training->Caption= int(traintime-hatch);
  }
  */
   if(DataForm->recordsNum->Value<interval_val->Value)
   {
     MessageDlg("Table do not contain enough data, try decreasing interval size and then reopen.", mtError, TMsgDlgButtons() << mbOK, 0);
     return;
   }
   Location->Max=DataForm->recNum-interval_val->Value;//5000;//Records->RecordCount;
   Location->Min=interval->Position;
   Location->Max=DataForm->recNum; //this is the total number of records in the database
   Location->Position=1;
   if(DataForm->recNum<interval_val->Value*2) MessageDlg("Table do not contain enough data, try reducing interval size and reopen.", mtInformation, TMsgDlgButtons() << mbOK, 0);
   else
   {
     play->Enabled=true;
     stop->Enabled=true;
     forword->Enabled=true;
     backword->Enabled=true;
     previous_evening->Enabled=true;
     previous_hour->Enabled=true;
     next_hour->Enabled=true;
     next_morning->Enabled=true;
     Location->Enabled=true;
     do_histogram->Enabled=true;
   }


}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::featureXClick(TObject *Sender)
{


          switch(featureX->ItemIndex)
          {
            case 0: xVar="duration"; Xfactor=option->deviationDuration->Value; break;// xFactor will convert the units of the feature to 256 max...
            case 1: xVar="mean_amplitude"; Xfactor=option->deviation_amplitude->Value; break;
            case 2: xVar="mean_pitch"; Xfactor=option->deviationPitch->Value; break;
            case 3: xVar="mean_FM"; Xfactor=option->deviationFM->Value; break;
            case 4: xVar="mean_entropy"; Xfactor=-option->deviationEntropy->Value; break;
            case 5: xVar="mean_mean_frequency"; Xfactor=option->deviationPeakFr->Value; break;
            case 6: xVar="mean_goodness_of_pitch";Xfactor=option->deviationPgood->Value; break;
            case 7: xVar="var_pitch"; Xfactor=option->deviationVarPitch->Value;break;
            case 8: xVar="var_FM"; Xfactor=option->deviationVarFM->Value;break;
            case 9: xVar="var_entropy"; Xfactor=option->deviationVarEnt->Value;break;
            case 10: xVar="var_goodness_of_pitch"; Xfactor=option->deviationVarPGood->Value;break;
            case 11: xVar="start_on"; Xfactor=0.1; break;
            case 12: xVar="mean_continuity_f"; Xfactor=option->cont_f_madVal->Value;break;
            case 13: xVar="mean_continuity_t"; Xfactor=option->cont_t_madVal->Value;break;
            case 14: xVar="mean_slope"; Xfactor=option->slope_madVal->Value; break;
          }
          if(Xfactor==0) Xfactor=1;
          xfactor=Xfactor;
		  if(display->ActivePage==syll_3D)chart3D->AxNameX=xVar;
		  if(display->TabIndex==0){dur_hist->CaptionX=xVar; Location->Position=0;}

}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::featureYClick(TObject *Sender)
{
     switch(featureY->ItemIndex)
        {
          case 0: yVar="duration"; Yfactor=option->deviationDuration->Value; break;// yFactor will convert the units of the feature to 256 max...
          case 1: yVar="mean_amplitude"; Yfactor=option->deviation_amplitude->Value; break;
          case 2: yVar="mean_pitch"; Yfactor=option->deviationPitch->Value; break;
          case 3: yVar="mean_FM"; Yfactor=option->deviationFM->Value; break;
          case 4: yVar="mean_entropy"; Yfactor=-option->deviationEntropy->Value; break;
          case 5: yVar="mean_mean_frequency"; Yfactor=option->deviationPeakFr->Value; break;
          case 6: yVar="mean_goodness_of_pitch";Yfactor=option->deviationPgood->Value; break;
          case 7: yVar="var_pitch"; Yfactor=option->deviationVarPitch->Value;break;
          case 8: yVar="var_FM"; Yfactor=option->deviationVarFM->Value;break;
          case 9: yVar="var_entropy"; Yfactor=option->deviationVarEnt->Value;break;
          case 10: yVar="var_goodness_of_pitch"; Yfactor=option->deviationVarPGood->Value;break;
          case 11: yVar="start_on"; Yfactor=0.1; break;
          case 12: yVar="mean_continuity_f"; Yfactor=option->cont_f_madVal->Value;break;
          case 13: yVar="mean_continuity_t"; Yfactor=option->cont_t_madVal->Value;break;
          case 14: yVar="mean_slope"; Yfactor=option->slope_madVal->Value; break;


        }
        if(Yfactor==0) Yfactor=1;
        yfactor=Yfactor;
        if(display->ActivePage==syll_3D)chart3D->AxNameY=yVar;
}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::FormClose(TObject *Sender, TCloseAction &Action)
{
  option->bitmapHeight=128;
  option->DVD=false;
  DVD_Image.ResetContents();
  StartForm->Visible=true;
  //delete DVD_map;
}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::xScaleChange(TObject *Sender)
{
  clear_mapClick(this);
  xfactor=Xfactor*float(xScale->Position)/50.0;
  if(xfactor==0)xfactor=1;
}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::yScaleChange(TObject *Sender)
{
  clear_mapClick(this);
  yfactor=Yfactor*float(yScale->Position)/50.0;
  if(yfactor==0)yfactor=1;
}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::intervalChange(TObject *Sender)
{
  interval_val->Value=interval->Position;
  residuals=interval->Position-slide->Position;
    if(clear->Checked)add_data=interval->Position;
    else add_data=slide->Position;
}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::LocationChange(TObject *Sender)
{

      if(table_type==syllable_table_type)
      {
        DataForm->Fetch_data(0, Location->Position, min_duration->Position, max_duration->Position, add_data);
		TDateTime dtm=DataForm->MyQuery1->FieldValues["serial_number"];
		date->Caption=FormatDateTime("dddd, mmmm d, yyyy ' at ' hh:mm:ss AM/PM", dtm);
		day=DataForm->MyQuery1->FieldValues["day"];
		hour=DataForm->MyQuery1->FieldValues["hour"];
		minute=DataForm->MyQuery1->FieldValues["minute"];
		/*
		str="month: "; str+=DataForm->MyQuery1->FieldValues["month"];
		str+="  day: ";
		day=DataForm->MyQuery1->FieldValues["day"];
		str+=day;
		str+="  time:   ";
		hour=DataForm->MyQuery1->FieldValues["hour"];
		str+=hour;
		minute=DataForm->MyQuery1->FieldValues["minute"];
		str+=":"; str+=minute;
		str+=":"; str+=DataForm->MyQuery1->FieldValues["second"];

		date->Caption=str;
		int a=int(DataForm->MyQuery1->FieldValues["serial_number"]);
		int b=int(hatched);
		current_age->Caption=a-b;*/
		filename->Caption=DataForm->MyQuery1->FieldValues["file_name"];
      }
      else
      {
         DataForm->get_recs(DataForm->TheTable->TableName, interval_val->Value);

      }


}
//---------------------------------------------------------------------------


void __fastcall TDVD_map::LocationKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
 if (Key ==VK_RETURN  && key_response)
 {
    key_response=false;
    Location->SelStart=Location->Position;
    Location->SelEnd=Location->Max;
    Location->Brush->Color=clRed;
 }

 if(Key == VK_ESCAPE)
 {
   Location->SelStart=1;
   Location->SelEnd=Location->Max;

 }
}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::LocationKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
   if(Key ==VK_RETURN)
   {
      key_response=true;
      Location->SelEnd=Location->Position;
      Location->Brush->Color=clYellow;
   }
}
//---------------------------------------------------------------------------


void __fastcall TDVD_map::LocationEnter(TObject *Sender)
{
   Location->Brush->Color=clYellow;
}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::LocationExit(TObject *Sender)
{
Location->Brush->Color=clBtnFace;        
}
//---------------------------------------------------------------------------





void TDVD_map::plot_raw_features()
{

   float x=0,y=0;
    int i=DataForm->MyQuery1->RecordCount;
    if(i<interval_val->Value)
    {
        DataForm->initiate_handler(DataForm->MyConnection1->Database, DataForm->TheTable->TableName);
        return;
    }

   chart->DataColor=set_color();
   raw_features_chart->ClearGraf();
   for(int i=0;i<DataForm->MyQuery1->RecordCount;i++)
  {
      x=find_raw_feature(rawFeatureX->ItemIndex);
      if(jitter->Checked)x+=(float)random(jitter_raw);      //x+=(float)random(5)
      y=find_raw_feature(rawFeatureY->ItemIndex);
      if(jitter->Checked)y+=(float)random(jitter_raw);
      DataForm->MyQuery1->Next();
      raw_features_chart->MarkAt(x,y,dot);
  }
  raw_features_chart->ShowGraf();
}





float TDVD_map::plot_data(bool three_D)
{
    static float oldx=0, oldy=0,oldz=0, previous_end=1000000;
    static int bout_cont=false, morning=100000, old_day=day;
    float x, y, z, goo;
   // Rchart::TDrawCan pos;

    if(morning_blink->Checked)
    {
	   if(day != old_day) { morning=0; old_day=day; if(three_D)chart3D->ColorChart=clYellow; else chart->ChartColor=clYellow;}
       if(morning<10) morning++;
       if(morning==4 || morning==5) if(three_D)chart3D->ColorChart=clWhite;   else chart->ChartColor=clWhite;

    }

    if(clear->Checked || residuals<1) {add_data=interval->Position; if(three_D)chart3D->Clear(); else chart->ClearGraf();}
    else if(buffer>residuals)
    {
      add_data=slide->Position;
      do{ if(three_D)chart3D->RemoveFirstItem(); else chart->RemoveFirstItem(); buffer--; }while(buffer>residuals);
    }

    if(!three_D && line->Checked)
    {
     x=find_feature(featureX->ItemIndex);
     y=find_feature(featureY->ItemIndex);
     if(three_D) z=find_feature(featureZ->ItemIndex);
     goo=y;
     chart->MoveTo(x, y);
    }

    if(backword->Down)
    {
      DataForm->MyQuery1->Last();
      for(int i=0;i<add_data;i++)
      {
        buffer++;
        x=find_feature(featureX->ItemIndex);
        y=find_feature(featureY->ItemIndex);
        z=find_feature(featureZ->ItemIndex);
        if(jitter->Checked){x+=(float)random(100)/500.0; y+=(float)random(100)/500.0; if(three_D)z+=(float)random(100)/500.0; }
        if(three_D)
        {
          chart3D->ColorData=set_color();
          chart3D->MarkAt(x,y,z,dot);
          if(line->Checked)
          {
           float start_on=find_feature(12), duration=find_feature(11);
           if(color_scheme->ItemIndex==3) if(x>oldx)chart3D->ColorData=clRed; else chart3D->ColorData=clBlue;
           if(start_on>previous_end || start_on<previous_end+bout_termination_dur->Position)
           {
              // Dina
              if(bout_cont>bout_count->Position)
              {
				chart3D->ColorData=(TColor)RGB(0,0,0);
               // chart3D->MarkAt(oldx,oldy,oldz,dot);
              }

              chart3D->Moveto(x, y, z);
              bout_cont=false;
            }

            {
			  if(color_scheme->ItemIndex==3) if(x>oldx)chart3D->ColorData=clRed;
			  else chart3D->ColorData=clBlue;
              chart3D->Drawto(x, y, z);
              bout_cont++;
            }
            oldx=x;
            previous_end=duration+ start_on;
          }

        }
        // 2D chart
        else
        {
          chart->DataColor=set_color();
          chart->MarkAt(x,y,dot);
          if(line->Checked)
          {
            float start_on=find_feature(12), duration=find_feature(11);
            if(color_scheme->ItemIndex==3) if(x>oldx)chart->DataColor=clRed; else chart->DataColor=clBlue;
           if(start_on>previous_end || start_on<previous_end+bout_termination_dur->Position)
           {
              // Dina
              if(bout_cont>bout_count->Position)
              {
				chart->DataColor=(TColor)RGB(0,0,0);
			   //	chart->MarkAt(oldx,oldy,dot);
              }

              chart->MoveTo(x, y);
              bout_cont=false;
            }

            {
			  if(color_scheme->ItemIndex==3) if(x>oldx)chart->DataColor=clRed; else chart->DataColor=clBlue;
              chart->DrawTo(x, y);
              bout_cont++;
            }
            oldx=x;
            previous_end=duration+ start_on;
          }
        }
        if(calc_daily->Checked)calc();
        DataForm->MyQuery1->Prior();
      }
    }


    else // not reverse but forward
    {
      for(int i=0;i<add_data;i++)
      {
        buffer++;
        x=find_feature(featureX->ItemIndex);
        y=find_feature(featureY->ItemIndex);
        z=find_feature(featureZ->ItemIndex);
        if(jitter->Checked){x+=(float)random(100)/500.0; y+=(float)random(100)/500.0; if(three_D) z+=(float)random(100)/500.0;}

        if(three_D)
        {
          chart3D->ColorData=set_color();
          if(cluster_mode->Down || synsong->Down)chart3D->MarkAt(x,y,z,dot);
          if(line->Checked)
          {
            float start_on=find_feature(16), duration=find_feature(15);
            if(start_on<previous_end || start_on>previous_end+bout_termination_dur->Position)
            {
              // Dina
              if(bout_cont>bout_count->Position)
              {
				chart3D->ColorData=(TColor)RGB(0,0,0);
               // chart3D->MarkAt(oldx,oldy, oldz,dot);
              }

              chart3D->Moveto(x, y,z);
              bout_cont=false;
            }
            else // bout continues
            {
            if(color_scheme->ItemIndex==3) if(x>oldx)chart3D->ColorData=clRed; else chart3D->ColorData=clBlue;
            chart3D->Drawto(x, y, z);
            bout_cont++;
            }
            oldx=x; oldy=y;
            previous_end=duration+ start_on;
          }
        }

        else // 2D chart
        {
          chart->DataColor=set_color();
          if(cluster_mode->Down || synsong->Down)chart->MarkAt(x,y,dot);
          if(line->Checked)
          {
            float start_on=find_feature(16), duration=find_feature(15);
            if(start_on<previous_end || start_on>previous_end+bout_termination_dur->Position)
            {
              // Dina
              if(bout_cont>bout_count->Position)
              {
				chart->DataColor=(TColor)RGB(0,0,0);
                //chart->MarkAt(oldx,oldy,dot);
              }

              chart->MoveTo(x, y);
              bout_cont=false;
            }
            else // bout continues
            {
			if(color_scheme->ItemIndex==3) if(x>oldx)chart->DataColor=clRed; else chart->DataColor=clBlue;
			chart->DrawTo(x, y);
			bout_cont++;
			}
			oldx=x; oldy=y;
			previous_end=duration+ start_on;
          }
		}

        if(calc_daily->Checked)calc();
		DataForm->MyQuery1->Next();
	  }
	}

	if(three_D) chart3D->MakeVisible();
	else chart->ShowGraf();
	return(goo);
}



float TDVD_map::find_feature(int feature)
{
      static float prev_dur=0;
      float x;
      switch(feature)
      {
        case 0: x=((float)DataForm->MyQuery1->FieldValues["duration"]-option->medianDuration->Value)/option->deviationDuration->Value; break;
        case 1: x=((float)DataForm->MyQuery1->FieldValues["mean_amplitude"]-option->mean_amplitude->Value)/option->deviation_amplitude->Value; break;
        case 2: x=((float)DataForm->MyQuery1->FieldValues["mean_pitch"]-option->medianPitch->Value)/option->deviationPitch->Value; break;
        case 3: x=((float)DataForm->MyQuery1->FieldValues["mean_FM"]-option->medianFM->Value)/option->deviationFM->Value; break;
        case 4: x=((float)DataForm->MyQuery1->FieldValues["mean_entropy"]-option->medianEntropy->Value)/option->deviationEntropy->Value; break;
        case 5: x=((float)DataForm->MyQuery1->FieldValues["mean_mean_frequency"]-option->medianPeakFr->Value)/option->deviationPeakFr->Value; break;
        case 6: x=((float)DataForm->MyQuery1->FieldValues["mean_goodness_of_pitch"]-option->medianPgood->Value)/option->deviationPgood->Value; break;
        case 7: x=((float)DataForm->MyQuery1->FieldValues["var_pitch"]-option->medianVarPitch->Value)/option->deviationVarPitch->Value; break;
        case 8: x=((float)DataForm->MyQuery1->FieldValues["var_FM"]-option->medianVarFM->Value)/option->deviationVarFM->Value; break;
        case 9: x=((float)DataForm->MyQuery1->FieldValues["var_entropy"]-option->medianVarEnt->Value)/option->deviationVarEnt->Value; break;
        case 10: x=((float)DataForm->MyQuery1->FieldValues["var_goodness_of_pitch"]-option->medianVarPGood->Value)/option->deviationVarPGood->Value; break;
        case 11: x=(float)DataForm->MyQuery1->FieldValues["start_on"]-prev_dur;
                 x-=option->medianStop->Value; x/=option->deviationStop->Value;
                 prev_dur=(float)DataForm->MyQuery1->FieldValues["start_on"]+DataForm->MyQuery1->FieldValues["duration"]; break;
        case 12: x=((float)DataForm->MyQuery1->FieldValues["mean_continuity_t"]-option->cont_t_medianVal->Value)/option->cont_t_madVal->Value; break;
        case 13: x=((float)DataForm->MyQuery1->FieldValues["mean_continuity_f"]-option->cont_f_medianVal->Value)/option->cont_f_madVal->Value; break;
        case 14: x=((float)DataForm->MyQuery1->FieldValues["mean_slope"]-option->median_slope->Value)/option->slope_madVal->Value; break;
        case 15: x=(float)DataForm->MyQuery1->FieldValues["duration"]; break;
        case 16: x=(float)DataForm->MyQuery1->FieldValues["start_on"];  break;
      }
      return(x);
}


float TDVD_map::find_raw_feature(int feature)
{
      float x;
      switch(feature)
      {
        case 0: jitter_raw=5; x=(float)DataForm->MyQuery1->FieldValues["amplitude"]; break;
        case 1: jitter_raw=100;x=(float)DataForm->MyQuery1->FieldValues["pitch"]; break;
        case 2: jitter_raw=10;x=(float)DataForm->MyQuery1->FieldValues["fm"]; break;
        case 3: jitter_raw=20;x=(float)DataForm->MyQuery1->FieldValues["entropy"]; break;
        case 4: jitter_raw=30;x=(float)DataForm->MyQuery1->FieldValues["mean_frequency"]; break;
        case 5: jitter_raw=15;x=(float)DataForm->MyQuery1->FieldValues["goodness"]; break;
        case 6: jitter_raw=15;x=(float)DataForm->MyQuery1->FieldValues["continuity_f"]; break;
        case 7: jitter_raw=20;x=(float)DataForm->MyQuery1->FieldValues["continuity_t"]; break;
        case 8: jitter_raw=5;x=(float)DataForm->MyQuery1->FieldValues["slope"]; break;
      }
      return(x);
}


void __fastcall TDVD_map::zoomClick(TObject *Sender)
{
switch(zoom->ItemIndex)
 {
     case 0:chart->MouseAction=maZoomDrag; break;
     case 1:chart->MouseAction=maPan; break;
     case 2:chart->MouseAction=maZoomWind; break;
 }
}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::dot_sizeClick(TObject *Sender)
{
  switch(dot_size->ItemIndex)
  {
    case 0: dot= 26; break;
    case 1: dot= 14; break;
    case 2: dot= 1; break;
    case 3: dot= 0; break;


  }
}
//---------------------------------------------------------------------------

TColor TDVD_map::set_color()
{
    static short day=1;
    short newDay;
	float dec_time;
	float dt;
	int cl;
    switch(color_scheme->ItemIndex)
    {
     case 0:  // color by clusters
        clust=DataForm->MyQuery1->FieldValues["cluster"];
        if(clust>0)return(color[clust-1]);
        else return(clSilver);


    case 1: // color by time of day (rainbow)
        if(hour<11 ) return(clRed);
        else if(hour<13) return((TColor)RGB(255,150,0));
        else if(hour<15) return(clGreen);
        else if(hour<18) return(clBlue);
		else return((TColor)RGB(0,0,0));

    case 2: // time of day cutoff
         dec_time=hour+(float)(minute/100);
         if(dec_time<time_cut->Value) return(clRed); else return((TColor)RGB(0,0,0));

    case 3:  // linear time
		/*
		newDay=DataForm->MyQuery1->FieldValues["day"];
        if(newDay!=day)
        {
          day=newDay;
          if(green>10)green-=time_progress->Position;
		}
		return((TColor)RGB(red,green,blue));
		*/
		dt=(float)Location->Position/(float)Location->Max*255;
		cl=dt;
		return((TColor)RGB(cl,0,255-cl));

    case 4:
        
        return(color[user_defined]);


   }
   return(clWhite); // default
}


void __fastcall TDVD_map::line_widthChange(TObject *Sender)
{
   chart->LineWidth=line_width->Position;
   line_val->Value=line_width->Position+1;
}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::color_schemeClick(TObject *Sender)
{
    change_color->Enabled=false;
    switch(color_scheme->ItemIndex)
    {
        case 4:
           if (ColorDialog1->Execute())
           color[user_defined] = ColorDialog1->Color;
           change_color->Enabled=true;
           break;
    }
}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::slideChange(TObject *Sender)
{
slide_val->Value=slide->Position;
residuals=interval->Position-slide->Position;
    if(clear->Checked)add_data=interval->Position;
    else add_data=slide->Position;
}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::cluster_modeClick(TObject *Sender)
{

  if(cluster_mode->Down)
  {
    chart->ClearGraf();
    buffer=0;
  //  jitter->Checked=true;
    Timer1->Interval=50;
    dot_size->ItemIndex=2;
    line_width->Position=1;
    interval->Position=4000;
    slide->Position=500;
    line->Checked=false;
    sound->Checked=false;
    clear->Checked=false;
  }
  else if(syntax_mode->Down)
  {
    chart->ClearGraf();
    buffer=0;
   // jitter->Checked=false;
    Timer1->Interval=20;
    dot_size->ItemIndex=0;
    line_width->Position=1;
    interval->Position=350;
    slide->Position=100;
    line->Checked=true;
    sound->Checked=false;
    clear->Checked=true;
  }
  else
  {
    chart->ClearGraf();
    buffer=0;
   // jitter->Checked=false;
    Timer1->Interval=200;
    dot_size->ItemIndex=0;
    line_width->Position=10;
    interval->Position=4;
    slide->Position=1;
    line->Checked=true;
    sound->Checked=true;
    clear->Checked=true;
  }

}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::next_hourClick(TObject *Sender)
{
 Screen->Cursor = crHourGlass;
 int ref_hour=hour;
 do{
    Location->Position+=200;
 }while(Location->Position<Location->Max && hour==ref_hour);
 ref_hour=hour;
 do{
    Location->Position--;
 }while(hour==ref_hour);
 Location->Position++;
 Screen->Cursor = crDefault;

}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::next_morningClick(TObject *Sender)
{
 Screen->Cursor = crHourGlass;
 int ref_day=day;
 do{
    Location->Position+=1000;
 }while(Location->Position<Location->Max && day==ref_day);

 ref_day=day;
 do{
    Location->Position-=50;
 }while(day==ref_day);

 ref_day=day;
 do{
    Location->Position++;
 }while(day==ref_day);
 Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::previous_eveningClick(TObject *Sender)
{
 Screen->Cursor = crHourGlass;
 int ref_day=day;
 if(Location->Position>1001) do{
	Location->Position-=1000;
 }while(Location->Position>1001 && day==ref_day);

 ref_day=day;
 do{
    Location->Position+=50;
 }while(day==ref_day);

 ref_day=day;
 do{
    Location->Position--;
 }while(day==ref_day);
 Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::previous_hourClick(TObject *Sender)
{
   Screen->Cursor = crHourGlass;
 int ref_hour=hour;
 if(Location->Position>201) do{
    Location->Position-=200;
 }while(Location->Position>201 && hour==ref_hour);
 ref_hour=hour;
 do{
    Location->Position++;
 }while(hour==ref_hour);
 Location->Position--;
 Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::do_histogramClick(TObject *Sender)
{
  Screen->Cursor = crHourGlass;
  int ref_day, training_day=1;
  float freq[100],feature_data, subtruct, divide;
  if(!dailyhist->Checked)
  {
    days=Location->Max/histIntVal->Value;
    interval->Position=1000;
    slide->Position=1000;
  }
  else
  {
    days=50;
    interval->Position=histInt->Position;
    slide->Position=histInt->Position;
  }
  
  dur_hist->Clear();
  dur_hist->GridMat->Resize(100,days);// Days=50
  AnsiString the_feature;
  switch(featureX->ItemIndex)
  {
     case 0: the_feature="duration"; subtruct=option->medianDuration->Value; divide=option->deviationDuration->Value;  break;
     case 1: the_feature="mean_amplitude";  subtruct=option->mean_amplitude->Value; divide=option->deviation_amplitude->Value; break;
     case 2: the_feature="mean_pitch";  subtruct=option->medianPitch->Value; divide=option->deviationPitch->Value; break;
     case 3: the_feature="mean_FM";  subtruct=option->medianFM->Value; divide=option->deviationFM->Value; break;
     case 4: the_feature="mean_entropy";  subtruct=option->medianEntropy->Value; divide=option->deviationEntropy->Value; break;
     case 5: the_feature="mean_mean_frequency";  subtruct=option->medianPeakFr->Value; divide=option->deviationPeakFr->Value; break;
     case 6: the_feature="mean_goodness_of_pitch";  subtruct=option->medianPgood->Value; divide=option->deviationPgood->Value; break;
     case 7: the_feature="var_pitch";  subtruct=option->medianVarPitch->Value; divide=option->deviationVarPitch->Value; break;
     case 8: the_feature="var_FM";  subtruct=option->medianVarFM->Value; divide=option->deviationVarFM->Value; break;
     case 9: the_feature="var_entropy";  subtruct=option->medianVarEnt->Value; divide=option->deviationVarEnt->Value;  break;
     case 10: the_feature="var_goodness_of_pitch";  subtruct=option->medianVarPGood->Value; divide=option->deviationVarPGood->Value; break;
   }
    if(autoscale->ItemIndex)
   {
      subtruct=min_val->Text.ToDouble();
      divide=max_val->Text.ToDouble();
      divide=(divide-subtruct)/100;
   }
  do{  // for the entire table
    int count=0; // will count valid data for each day, less then 100 will be discurded
    for(int i=0; i<100; i++) freq[i]=0;  // clear the histogram frequency array
    ref_day=day;
    bool cont=true;
    do{  // for a developmental day
        Location->Position+=slide->Position;
        day=DataForm->MyQuery1->FieldValues["day"];
        DataForm->MyQuery1->First();
        for(int i=0;i<slide->Position;i++)
        {
           feature_data=DataForm->MyQuery1->FieldValues[the_feature];


           feature_data-=subtruct; feature_data/=divide;
           if(!autoscale->ItemIndex)feature_data=50+feature_data*10;// to give a typical range of 0-100
           DataForm->MyQuery1->Next();
           if(trim_edges->Checked) { if(feature_data<99 && feature_data>0) {freq[(int)feature_data]++; count++;} }
           else
           {
             if(feature_data>99) feature_data=99;
             if(feature_data<0)  feature_data=0;
             freq[(int)feature_data]++;
             count++;
           }
        }
        if(dailyhist->Checked && day==ref_day) cont=true;
        else if(!dailyhist->Checked && count<histIntVal->Value) cont=true;
        else cont=false;
    }while(cont && Location->Position<Location->Max-2200);// this is still the same day...

    float sqFr=0;
    for(int i=0; i<100; i++)if(count>100)
    {
      freq[i]/=count; // normalize the matrix
      if(i>20)sqFr+=pow(freq[i],2)*i*10;
      dur_hist->GridMat->Elem[100-i][training_day]=(int)(1000*freq[i]);
      str=i; str+=","; str+=freq[i];
      if(histdata->Checked) Memo1->Lines->Add(str);
    }
    if(count>100)training_day++;
  }while(Location->Position<Location->Max-2200);
  dur_hist->SetViewAngles(10,185);
  Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::zoom1Click(TObject *Sender)
{
    switch(zoom1->ItemIndex)
 {
     case 0:dur_hist->MouseAction=maPan; break;
     case 1:dur_hist->MouseAction=maZoomDrag; break;
     case 2:dur_hist->MouseAction=maZoomWind; break;
 }
}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::save_histoClick(TObject *Sender)
{
  if(SavePictureDialog1->Execute())
     dur_hist->CopyToBMP(SavePictureDialog1->FileName,0);
}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::colorsChange(TObject *Sender)
{
   dur_hist->ColorScaleHigh=colors->Position;
}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::autoscaleClick(TObject *Sender)
{
   if(!autoscale->ItemIndex){min_valT->Visible=false; max_valT->Visible=false; min_val->Visible=false; max_val->Visible=false;}
   else {min_valT->Visible=true; max_valT->Visible=true; min_val->Visible=true; max_val->Visible=true;}
}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::min_durationChange(TObject *Sender)
{
durationTresh->Value=min_duration->Position;
}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::max_durationChange(TObject *Sender)
{
max_durationT->Value=max_duration->Position;
}
//---------------------------------------------------------------------------


void __fastcall TDVD_map::clear_mapClick(TObject *Sender)
{
  Image1->Canvas->Brush->Color=clWhite;
  Image1->Canvas->FillRect(Rect(0,0,600,600));
}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::save_bitmapClick(TObject *Sender)
{
  if (ColorDialog1->Execute())
  {
    color[user_defined] = ColorDialog1->Color;
    change_color->Enabled=true;
    raw_features_chart->DataColor= color[user_defined];
  }

}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::BitBtn2Click(TObject *Sender)
{
StartForm->Visible=true;
   StartForm->BringToFront();          
}
//---------------------------------------------------------------------------


void TDVD_map::calc()
{
    static int daily_counter[11]={0,0,0,0,0,0,0,0,0,0,0}; // mean feature values per day
    static int morning_counter[11]={0,0,0,0,0,0,0,0,0,0,0};
    static int previous_day[11]={0,0,0,0,0,0,0,0,0,0,0};
   // static int syllable_counter[11];
    str="";
    int clast=DataForm->MyQuery1->FieldValues["cluster"];
    if(clust<=0)return;

    float _dur=DataForm->MyQuery1->FieldValues["duration"];
    daily_means[clust][0]+=_dur;
    float _mean_pitch=DataForm->MyQuery1->FieldValues["mean_pitch"];
    daily_means[clust][1]+=_mean_pitch;
    float _var_pitch=DataForm->MyQuery1->FieldValues["var_pitch"];
    daily_means[clust][2]+=_var_pitch;
    float _mean_fm=DataForm->MyQuery1->FieldValues["mean_FM"];
    daily_means[clust][3]+=_mean_fm;
    float _var_fm=DataForm->MyQuery1->FieldValues["var_FM"];
    daily_means[clust][4]+=_var_fm;
    float _mean_ent=DataForm->MyQuery1->FieldValues["mean_entropy"];
    daily_means[clust][5]+=_mean_ent;
    float _var_ent=DataForm->MyQuery1->FieldValues["var_entropy"];
    daily_means[clust][6]+=_var_ent;
    float _mean_goodness=DataForm->MyQuery1->FieldValues["mean_goodness_of_pitch"];
    daily_means[clust][7]+=_mean_goodness;
    float _var_goodness=DataForm->MyQuery1->FieldValues["var_goodness_of_pitch"];
    daily_means[clust][8]+=_var_goodness;

    daily_counter[clust]++;

    day=DataForm->MyQuery1->FieldValues["day"];
    hour=DataForm->MyQuery1->FieldValues["hour"];
    minute=DataForm->MyQuery1->FieldValues["minute"];

    if(previous_day[clast]!=day)
    {

      if(!morning_counter[clast])// reall the beginning of the day
      {
        // calc and publish daily means
        for(int i=0;i<9;i++) daily_means[clust][i]/=daily_counter[clust];  // compute means
        str=-99; str+=","; str+=previous_day[clast]; str+=","; str+=clast; str+=",";
        str+=daily_means[clust][0]; str+=","; str+=daily_means[clust][1]; str+=","; str+=daily_means[clust][2]; str+=",";
        str+=daily_means[clust][3]; str+=","; str+=daily_means[clust][4]; str+=","; str+=daily_means[clust][5]; str+=",";
        str+=daily_means[clust][6]; str+=","; str+=daily_means[clust][7]; str+=","; str+=daily_means[clust][8];
        Memo1->Lines->Add(str);
         // clean up
        daily_counter[clust]=0;
        for(int i=0;i<9;i++)daily_means[clust][i]=0;
      }

      //dur, **mean_pitch, **var_pitch, **mean_fm, **var_fm, **mean_ent, **var_ent, **mean_goodness, **var_goodness
      if(morning_counter[clast]<101)
      {
        dur[clast][morning_counter[clast]]=_dur;
        mean_pitch[clast][morning_counter[clast]]=_mean_pitch;
        var_pitch[clast][morning_counter[clast]]=_var_pitch;
        mean_fm[clast][morning_counter[clast]]=_mean_fm;
        var_fm[clast][morning_counter[clast]]=_var_fm;
        mean_ent[clast][morning_counter[clast]]=_mean_ent;
        var_ent[clast][morning_counter[clast]]=_var_ent;
        mean_goodness[clast][morning_counter[clast]]=_mean_goodness;
        var_goodness[clast][morning_counter[clast]]=_var_goodness;
        morning_counter[clast]++;
      }
    }
    if(morning_counter[clast]==100)
    {
      previous_day[clast]=day;
      morning_counter[clast]=0;
      str=day; str+=","; str+=hour; str+="."; str+=minute; str+=","; str+=clast; str+=",";

      for(int i=0;i<100;i++)median_vector.push_back(dur[clast][i]);
      sort(median_vector.begin(), median_vector.end());
      str+=median_vector[50]; str+=",";
      median_vector.erase(median_vector.begin(), median_vector.end());

      for(int i=0;i<100;i++)median_vector.push_back(mean_pitch[clast][i]);
      sort(median_vector.begin(), median_vector.end());
      str+=median_vector[50]; str+=",";
      median_vector.erase(median_vector.begin(), median_vector.end());

      for(int i=0;i<100;i++)median_vector.push_back(var_pitch[clast][i]);
      sort(median_vector.begin(), median_vector.end());
      str+=median_vector[50]; str+=",";
      median_vector.erase(median_vector.begin(), median_vector.end());

      for(int i=0;i<100;i++)median_vector.push_back(mean_fm[clast][i]);
      sort(median_vector.begin(), median_vector.end());
      str+=median_vector[50]; str+=",";
      median_vector.erase(median_vector.begin(), median_vector.end());

      for(int i=0;i<100;i++)median_vector.push_back(var_fm[clast][i]);
      sort(median_vector.begin(), median_vector.end());
      str+=median_vector[50]; str+=",";
      median_vector.erase(median_vector.begin(), median_vector.end());

      for(int i=0;i<100;i++)median_vector.push_back(mean_ent[clast][i]);
      sort(median_vector.begin(), median_vector.end());
      str+=median_vector[50]; str+=",";
      median_vector.erase(median_vector.begin(), median_vector.end());

      for(int i=0;i<100;i++)median_vector.push_back(var_ent[clast][i]);
      sort(median_vector.begin(), median_vector.end());
      str+=median_vector[50]; str+=",";
      median_vector.erase(median_vector.begin(), median_vector.end());

      for(int i=0;i<100;i++)median_vector.push_back(mean_goodness[clast][i]);
      sort(median_vector.begin(), median_vector.end());
      str+=median_vector[50]; str+=",";
      median_vector.erase(median_vector.begin(), median_vector.end());

      for(int i=0;i<100;i++)median_vector.push_back(var_goodness[clast][i]);
      sort(median_vector.begin(), median_vector.end());
      str+=median_vector[50]; str+=",";
      median_vector.erase(median_vector.begin(), median_vector.end());

      Memo1->Lines->Add(str);
   }
    //  var_entropy[clast]/=100;
  //    sort(EntVar[clast].begin, EntVar[clast].end);

      //str+=",";str+=var_entropy[clast];
      
      
    //  var_entropy[clast]=0;
   // }
}


void __fastcall TDVD_map::histIntChange(TObject *Sender)
{
  histIntVal->Value=histInt->Position;        
}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::dailyhistClick(TObject *Sender)
{
  if(dailyhist->Checked)
  {histInt->Visible=false; histIntVal->Visible=false; intervTxt->Visible=false; syll_label->Visible=false;}
  else
  {histInt->Visible=true; histIntVal->Visible=true; intervTxt->Visible=true;syll_label->Visible=true;}
}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::showGridClick(TObject *Sender)
{
  if(showGrid->Checked) dur_hist->MeshVisible=true;
  else dur_hist->MeshVisible=false;
}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::colorMinChange(TObject *Sender)
{
 dur_hist->ColorScaleLow=(colorMin->Position)/10;
}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::contrast3DChange(TObject *Sender)
{
 dur_hist->ScaleFactZ=float(contrast3D->Position)/50.0;
}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::bout_termination_durChange(TObject *Sender)
{
  bout_termination_dur_val->Value=bout_termination_dur->Position;        
}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::bout_countChange(TObject *Sender)
{
    bout_count_val->Value=bout_count->Position+1;
}
//---------------------------------------------------------------------------


void __fastcall TDVD_map::optionsClick(TObject *Sender)
{
        option->Visible=true;
        option->BringToFront();
        option->show_start=false;
}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::auto_scaleClick(TObject *Sender)
{
  if(auto_scale->Checked){chart3D->AutoScale=true; chart3D->AutoOrigin=true; }
  else { chart3D->AutoScale=false; chart3D->AutoOrigin=false; }
}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::bound_boxClick(TObject *Sender)
{
   if(bound_box->Checked) chart3D->BoundBoxStyle=bbFaces;
   else chart3D->BoundBoxStyle=bbNone;
}
//---------------------------------------------------------------------------


void __fastcall TDVD_map::featureZClick(TObject *Sender)
{
          switch(featureZ->ItemIndex)
          {
            case 0: zVar="duration"; break;// xFactor will convert the units of the feature to 256 max...
            case 1: zVar="mean_amplitude"; break;
            case 2: zVar="mean_pitch";  break;
            case 3: zVar="mean_FM";  break;
            case 4: zVar="mean_entropy"; break;
            case 5: zVar="mean_mean_frequency";  break;
            case 6: zVar="mean_goodness_of_pitch";break;
            case 7: zVar="var_pitch"; break;
            case 8: zVar="var_FM"; break;
            case 9: zVar="var_entropy"; break;
            case 10: zVar="var_goodness_of_pitch"; break;
            case 11: zVar="start_on"; break;
            case 12: zVar="mean_continuity_f"; break;
            case 13: zVar="mean_continuity_t"; break;
            case 14: zVar="mean_slope";  break;
          }
        chart3D->AxNameZ=zVar;
}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::displayChange(TObject *Sender)
{
	featureX->Visible=true;
	featureY->Visible=true;

  if(display->ActivePage==syll_3D)
  {
     featureXClick(this);
     featureYClick(this);
     featureZClick(this);
  }
  else if(display->ActivePage==raw_features_2D)
  {
    featureX->Visible=false;
    featureY->Visible=false;
  }
  else if(display->TabIndex==0)// histogram
  {
    featureY->Visible=false;
  }

}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::rawFeatureXClick(TObject *Sender)
{
  TRadioGroup *rawFeature;
  bool axis;
  if(Sender==rawFeatureX){axis=0; rawFeature=rawFeatureX;}
  else {axis=1; rawFeature=rawFeatureY;}
  switch(rawFeature->ItemIndex)
  {
     //amplitude
     case 0:  if(axis){raw_features_chart->Scale1Y->RangeHigh=120; raw_features_chart->Scale1Y->RangeLow=0;}
                              else {raw_features_chart->Scale1X->RangeHigh=120; raw_features_chart->Scale1X->RangeLow=0;} break;
     //  pitch
     case 1:   if(axis){raw_features_chart->Scale1Y->RangeHigh=10000; raw_features_chart->Scale1Y->RangeLow=0;}
                               else {raw_features_chart->Scale1X->RangeHigh=10000; raw_features_chart->Scale1X->RangeLow=0;}break;
     //fm
     case 2:   if(axis){raw_features_chart->Scale1Y->RangeHigh=1000; raw_features_chart->Scale1Y->RangeLow=0;}
                              else {raw_features_chart->Scale1X->RangeHigh=1000; raw_features_chart->Scale1X->RangeLow=0;}break;
     // entropy
     case 3:   if(axis){raw_features_chart->Scale1Y->RangeHigh=0; raw_features_chart->Scale1Y->RangeLow=-1000;}
                              else {raw_features_chart->Scale1X->RangeHigh=0; raw_features_chart->Scale1X->RangeLow=-1000;}break;
     // mean frequency
     case 4:   if(axis){raw_features_chart->Scale1Y->RangeHigh=10000; raw_features_chart->Scale1Y->RangeLow=0;}
                              else {raw_features_chart->Scale1X->RangeHigh=10000; raw_features_chart->Scale1X->RangeLow=0;}break;
     // goodness
     case 5:   if(axis){raw_features_chart->Scale1Y->RangeHigh=3000; raw_features_chart->Scale1Y->RangeLow=0;}
                              else {raw_features_chart->Scale1X->RangeHigh=3000; raw_features_chart->Scale1X->RangeLow=0;}break;
    //continuity F
     case 6:  jitter_raw=30; if(axis){raw_features_chart->Scale1Y->RangeHigh=3000; raw_features_chart->Scale1Y->RangeLow=0;}
                              else {raw_features_chart->Scale1X->RangeHigh=3000; raw_features_chart->Scale1X->RangeLow=0;}break;
     // continuity t
     case 7:  jitter_raw=30; if(axis){raw_features_chart->Scale1Y->RangeHigh=10000; raw_features_chart->Scale1Y->RangeLow=0;}
                              else {raw_features_chart->Scale1X->RangeHigh=1000; raw_features_chart->Scale1X->RangeLow=0;}break;
     case 8: break; // slope...

  }
}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::playClick(TObject *Sender)
{
   if(display->TabIndex==0)
   {
	 please_wait->Visible=true;
	 DVD_map->Repaint();
	 date->Visible=false;
	 do_histogramClick(this);
	 contrast3DChange(this);
	 please_wait->Visible=false;
	 date->Visible=true;
	 return;
   }
   green=255; // reset linear time color to red
   chart->ChartColor=clWhite;
   Timer1->Enabled=true;
   play->Enabled=false;
   stop->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::stopClick(TObject *Sender)
{
    green=255; // reset linear time color to red
    chart->ChartColor=clWhite;
    chart->ClearGraf();
    buffer=0; residuals=interval->Position-slide->Position;
    if(clear->Checked)add_data=interval->Position;
    else add_data=slide->Position;
    Timer1->Enabled=false;
    play->Enabled=true;
    stop->Enabled=false;
    DataForm->MyQuery1->Close();
}
//---------------------------------------------------------------------------

void __fastcall TDVD_map::new_birdClick(TObject *Sender)
{
  /*
  animals_form->datapages->ActivePageIndex=1;
  if (animals_form->ShowModal() == mrOk)
  {
	bird_name->Caption=animals_form->name->Text;
	current_age->Caption=animals_form->age->Value;
    model->Caption=animals_form->song->Text;
    training->Caption=animals_form->training->Text;
  }
  if(animals_form->new_bird) animals_form->Insert_bird_record();
  */
}
//---------------------------------------------------------------------------





void __fastcall TDVD_map::BitBtn4Click(TObject *Sender)
{
  chart3D->Clear();
}
//---------------------------------------------------------------------------


void __fastcall TDVD_map::zoom_rawClick(TObject *Sender)
{
switch(zoom_raw->ItemIndex)
 {
     case 0:raw_features_chart->MouseAction=maZoomDrag; break;
     case 1:raw_features_chart->MouseAction=maPan; break;
     case 2:raw_features_chart->MouseAction=maZoomWind; break;
 }
}
//---------------------------------------------------------------------------


void __fastcall TDVD_map::LinkLabel1LinkClick(TObject *Sender, const UnicodeString Link,
          TSysLinkType LinkType)
{
AnsiString link=Link;
  ShellExecute(0, "open", link.c_str(), 0, 0, 1);
}
//---------------------------------------------------------------------------

