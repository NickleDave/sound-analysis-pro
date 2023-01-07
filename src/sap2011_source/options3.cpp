//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "options3.h"
#include "start.h"
#include "records.h"
#include <math.h>
#include <Filectrl.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DBAccess"
#pragma link "MemDS"
#pragma link "MyAccess"
#pragma resource "*.dfm"
Toption *option;
//---------------------------------------------------------------------------
__fastcall Toption::Toption(TComponent* Owner)
        : TForm(Owner)
{
  squiz=1;
  warning=true;
  ztSamplesPerWindow=409;
  syllable_duration_thresh=300;
  bitmapHeight=256;//128;   this is actually the height of the bitmap y-axis, the lenght is obviously variable
  bout_duration_thresh=100;  // ends the bout
  stop_duration_thresh=7;
  //amplitude_thresh=130;
  entropy_thresh=-2;
  frequency_scale=43.0;
  sampling_rate=44100;
  cepstrum_pitch=true;
  chan=0; // this is the current channel which should be zero for all moddules but live and sound live
  calculate_continuity=true;
  save_image_mem=false;
  check_overwrite=false;
  contour_pitch=false;
  dynamic_pitch_calculation=true;
  peak_frequency_pitch=false;
  halt=false;
  saveRawFile=true;
  keep_original_rawfile_name=false;
  create_feature_arrays=false;
  save_all=false;
  saveRecFiles=true;
  doubleSamples=false;
  show_start=true;
  simple_pitch_averaging=false;
  move_sound=true;
  delete_trash_file=false;
  serial_by_time=false; // this will set priority to the syllable table serial number to be formated by time and then by the actual number given by the file...
  keep_file_name=true;
  entropy_and_power=false;
  entropy_or_power=true;
  Draw_sonogram=false;
  tapers_done=false;
  DVD=false;
  boost_amplitude=false;
  DeleteFile_failed=false;
  detect_low_noise=false;
  refine_segmentation=false;
  brain_mode=false;
  quick_cluster=false;
  age_in_table=false;
  range_x2=false;
  range_x05=false;
  slave_features=false;
  custom_parse=false;
  hp_amp=false;
  sono_display=false;
  reverse_segmentation=false;
  //testing=false;
  segmentation_feature=0;
  problem_file="";
  select_table=0; // this is the default table
  minimum_syllables_per_files=0;
  frequency_range_factor=1;
  minFreq=300;
  spectrum_range=260;// 11kHz is the max frequency we will examine, e.g., 512*43= 22,000Hz for sampling rate of 44.1kHz
  max_peak_frequency=8000;// we reject sounds-windows of high peak frequency, above 8000Hz;
  noise_thresh_freq=500/86;// We reject sound-windows of low peak frequency, below 1000Hz
  noise_ratio=0.5;
  high_freq_rejection=10000;//0.37; // proporation of energy at the higher frequency range, apove 8600Hz or so...
 // high_freq_rejection_amp=2200;
  min_entropy_freq=5;// 2kHz -- good for noisy cages...
  max_entropy_freq=245;//8600kHz;
  upper_pitch_bound=3;//7350 Hz limit
  lower_pitch_bound=55; // 400Hz minimum pitch estimate
  baseline=70; // this is the Db level default baseline
  Advance=44;   // 60 frames advance in each step of the window
  problems_thresh=10; // after 10 problems within 10 minutes we call it quits and will stop the processes
  pitch_goodness_thresh=100;
  pitch_entropy_thresh=-2;
  harmonic_pitch_thresh=1800.0;//1800Hz and above is not harmonic... note that pitch units are here in Hz
  min_syllable_duration=5; // syllable terminate only if its duration is 7ms or more...
  min_stop_duration=3;   // syllable terminate only if a stop is of at least 3 consequative occurance
  slow_execution=0;
  hp_small=5000;
  hp_big=50000000;
  //feature_file="c:/sap/features.txt";
  features_table="raw_features_default"; // this is the default table for the raw features
 // features_database="SAP";
  File_index=0;
  year=2005;
  //File_index=Count_Table_Recs(file_table);
  for(int i=0;i<10;i++)slave_mode[i]=0;
  if(!DataForm->table_exists("feature_scale"))create_feature_scale();
  else
  {
    try{ scale->Active=true; }
    catch(...)
    {//DataForm->back_mySQLClick(this);
    }
  }
  testClick(this);
  StartForm->ReadTapers();
  try{scale->Active=true;}
  catch(...) {
 // DataForm->back_mySQLClick(this);
  }//MessageDlg("Could not open the scale table, default featue scale will apply.", mtError, TMsgDlgButtons() << mbOK, 0); }
  
}


//---------------------------------------------------------------------------


void __fastcall Toption::FormClose(TObject *Sender, TCloseAction &Action)
{
  Visible=false;
  if(show_start) StartForm->Visible=true;
  StartForm->BringToFront();
}

//---------------------------------------------------------------------------

void Toption::extract_file_attributes(AnsiString wave_name)
{
    /*
    we try an alternative method:
      - if user want an index or nothing -- give serial index to him with standard date inc
      -- if user want file age -- give it to him
      -- otherwise, try to get the most out of the file name

      options:
      0 -- use current time (also good if no one cares about time)
      1 -- use file age to index time
      2 -- use file name to extract time info

   */
    //static int time_index=0;
    AnsiString x, name;
    int string_size;
    int ii=1;
    double Age;
    unsigned short year1, month1, day1, Hour1, Min1, Sec1, Msec1;

    switch(format->ItemIndex)
    {
     case 0: // user want just an order index or nothing, use current date&time as ref
           file_age=Now(); // file_age i a global TDateTime, here we use now as file age bloffing
           Age=(double)file_age;
           file_age.DecodeTime(&Hour1, &Min1, &Sec1, &Msec1);
           file_age.DecodeDate(&year1, &month1, &day1);
           year=year1; month=month1; day=day1; hour=Hour1; minute=Min1; second=Sec1; msec=Msec1;
           ms_from_midnight=msec+second*1000+minute*60000+hour*3600000;
           str=(int)Age; str+="."; str+=ms_from_midnight;
           serial_number=str.ToDouble();
           break;

     case 1: // extract date and time from file age
            file_age=file_age.FileDateToDateTime(FileAge(wave_name));
            Age=(double)file_age;
            file_age.DecodeTime(&Hour1, &Min1, &Sec1, &Msec1);
            file_age.DecodeDate(&year1, &month1, &day1);
            year=year1; month=month1; day=day1; hour=Hour1; minute=Min1; second=Sec1; msec=Msec1;
            ms_from_midnight=msec+second*1000+minute*60000+hour*3600000;
            str=(int)Age; str+="."; str+=ms_from_midnight;
            serial_number=str.ToDouble();
            break;

     case 2: // use file name (more complicated...
            // first detect if SAP2 file name include a ms from midnight stamp
            name=ExtractFileName(wave_name);
            string_size=name.Length();
            int underlines=0;
            for(int i=1;i<=string_size;i++)if(name[i]=='_') underlines++;

            if(underlines==4)// sound analysis recorder file (probably) name_April_28_2010_63424484
            {
                // get the age of the file in days from file age
                /*
                file_age=file_age.FileDateToDateTime(FileAge(wave_name));
                Age=(double)file_age;
                file_age.DecodeDate(&year1, &month1, &day1);
                year=year1; month=month1; day=day1;
                */
                // skip name
                do{ii++;}while(name[ii]!='_' && ii<string_size);
                // find month
                ii++; x=""; do{x+=name[ii]; ii++;}while(name[ii]!='_' && ii<string_size);
                month=monthToInt(x);
                // find day
                ii++; x=""; do{x+=name[ii]; ii++;}while(name[ii]!='_' && ii<string_size);
                day=x.ToIntDef(0);
                // find year
                ii++; x=""; do{x+=name[ii]; ii++;}while(name[ii]!='_' && ii<string_size);
                year=x.ToIntDef(0);

                if(ii>string_size-3 || !year) // failed, overide and use file age instead...
                {
                    file_age=file_age.FileDateToDateTime(FileAge(wave_name));
                    Age=(double)file_age;
                    file_age.DecodeTime(&Hour1, &Min1, &Sec1, &Msec1);
                    file_age.DecodeDate(&year1, &month1, &day1);
                    year=year1; month=month1; day=day1; hour=Hour1; minute=Min1; second=Sec1; msec=Msec1;
                    ms_from_midnight=msec+second*1000+minute*60000+hour*3600000;
                    str=(int)Age; str+="."; str+=ms_from_midnight;
                    serial_number=str.ToDouble();
                }

                else // keep trying...
                {
                    // try extracting the ms since midnight
                    x=""; ii++;
                    do{x+=name[ii]; ii++; }while(name[ii]!='_' && name[ii]!='.' && ii<string_size-3);
                    ms_from_midnight=x.ToIntDef(0);

                    if(!ms_from_midnight)// failed, overide and use file age instead...
                    {
                        file_age=file_age.FileDateToDateTime(FileAge(wave_name));
                        Age=(double)file_age;
                        file_age.DecodeTime(&Hour1, &Min1, &Sec1, &Msec1);
                        file_age.DecodeDate(&year1, &month1, &day1);
                        year=year1; month=month1; day=day1; hour=Hour1; minute=Min1; second=Sec1; msec=Msec1;
                        ms_from_midnight=msec+second*1000+minute*60000+hour*3600000;
                        str=(int)Age; str+="."; str+=ms_from_midnight;
                        serial_number=str.ToDouble();
                    }
                    else // success
                    {
                        hour=ms_from_midnight/3600000;
                        minute= (ms_from_midnight/60000)%60;
                        second= (ms_from_midnight/1000)%60;
                        msec=ms_from_midnight%1000;
                        // now we have all we need from year, month, day, hour, sec, msec...
                        // we have to construct file age from these:
                        file_age= EncodeDate((Word)year, (Word)month, (Word)day);// + EncodeTime((Word)hour, (Word)minute,(Word)second,(Word)msec);
                        str=(int)file_age;
                        str+=".";
                        str+=ms_from_midnight;
                        serial_number=str.ToDouble();
                    }

                }
                return;
            } // end  if(underlines==4)


            if(underlines>5)// SAP file probably, but which one?
            {
                int dots=0;
                for(int i=1;i<=string_size;i++)if(name[i]=='.') dots++;
                if(dots==2) // SAP2 file: R109_39152.86197671_3_11_23_56_37.wav
                {
                    ii=1;
                    do{ii++;}while(name[ii]!='_' && ii<string_size);
                    ii++; x=""; do{x+=name[ii]; ii++;}while(name[ii]!='.' && ii<string_size); // find integer part of serial number
                    str=x.ToIntDef(0); // days ellapsed from 1900
                    ii++; x=""; do{x+=name[ii]; ii++;}while(name[ii]!='_' && ii<string_size); // find fraction part of serial number
                    ms_from_midnight=x.ToIntDef(0);
                    hour=ms_from_midnight/3600000;
                    minute= (ms_from_midnight/60000)%60;
                    second= (ms_from_midnight/1000)%60;
                    str+=".";
                    str+=ms_from_midnight;
                    serial_number=str.ToDouble();
                    file_age=serial_number;
                    file_age.DecodeDate(&year1, &month1, &day1);
                    year=year1;
                    month=month1;
                    day=day1;
                    return;
                }
                else  // probably older format of SAP, but which one?
                {
                   ii=1;
                   do{ii++;}while(name[ii]!='_' && ii<string_size);
                   do{ii++;}while(name[ii]!='_' && ii<string_size);
                   ii++;
                   if(name[ii]=='o' && name[ii+1]=='n') // format: bird109_97023_on_Oct_02_11_12
                   {
                        second=0;
                        ii=1;

                        file_age=file_age.FileDateToDateTime(FileAge(wave_name));
                        file_age.DecodeDate(&year1, &month1, &day1);
                        year=year1; month=month1; day=day1;
                        x=""; do{x+=name[ii];ii++;}while(name[ii]!='_' && ii<string_size);
                        // find serial number
                        x=""; ii++; do{x+=name[ii]; ii++; }while(name[ii]!='_' && name[ii]!='.' && ii<string_size-5);
                       //  serial_number=x.ToIntDef(0);
                        x=""; ii++; do{x+=name[ii]; ii++; }while(name[ii]!='_' && name[ii]!='.' && ii<string_size-5);
                        // fine month
                        x=""; ii++; do{x+=name[ii]; ii++; }while(name[ii]!='_' && name[ii]!='.' && ii<string_size-4);
                        month=monthToInt(x);
                        // find day of month
                        x=""; ii++; do{x+=name[ii]; ii++; }while(name[ii]!='_' && name[ii]!='.' && ii<string_size-4);
                        day=x.ToIntDef(0);
                        // find hour
                        x=""; ii++; do{x+=name[ii]; ii++; }while(name[ii]!='_' && name[ii]!='.' && ii<string_size-3);
                        hour=x.ToIntDef(0);
                        // find minute
                        x=""; ii++; do{x+=name[ii]; ii++; }while(name[ii]!='_' && name[ii]!='.' && ii<string_size-3);
                        minute=x.ToIntDef(0);
                        // find second
                        x=""; ii++; do{x+=name[ii]; ii++; }while(name[ii]!='_' && name[ii]!='.' && ii<string_size-3);
                        second=x.ToIntDef(0);
                        file_age= EncodeDate((Word)year, (Word)month, (Word)day) + EncodeTime((Word)hour, (Word)minute,(Word)second,(Word)0);
                        ms_from_midnight=second*1000+minute*60000+hour*3600000;
                        str=(int)file_age;
                        str+=".";
                        str+=ms_from_midnight;
                        serial_number=str.ToDouble();
                        return;
                   }
                   else
                   {
                       // R109_october_11_2006_23_56_37.wav
                        ii=1;
                        do{ii++;}while(name[ii]!='_' && ii<string_size);
                        ii++; x="";
                        do{x+=name[ii]; ii++;}while(name[ii]!='_' && ii<string_size); // find month name
                        month1=monthToInt(x);
                        ii++; x="";
                        do{x+=name[ii]; ii++;}while(name[ii]!='_' && ii<string_size); // find day
                        day1=x.ToIntDef(0);
                        ii++; x="";
                        do{x+=name[ii]; ii++;}while(name[ii]!='_' && ii<string_size); // find year
                        year1=x.ToIntDef(0);
                        ii++; x="";
                        do{x+=name[ii]; ii++;}while(name[ii]!='_' && ii<string_size); // find hour
                        hour=x.ToIntDef(0);
                        ii++; x="";
                        do{x+=name[ii]; ii++;}while(name[ii]!='_' && ii<string_size); // find min
                        minute=x.ToIntDef(0);
                        ii++; x="";
                        do{x+=name[ii]; ii++;}while(name[ii]!='_' && ii<string_size); // find sec
                        second=x.ToIntDef(0);
                        file_age=EncodeTime(hour, minute, second, 0);
                        file_age+=EncodeDate(year1, month1, day1);
                        ms_from_midnight=second*1000+minute*60000+hour*3600000;
                        str=(int)file_age;
                        str+=".";
                        str+=ms_from_midnight;
                        serial_number=str.ToDouble();
                        year=year1;
                        month=month1;
                        day=day1;
                        if(!year)
                        {
                            file_age=file_age.FileDateToDateTime(FileAge(wave_name));
                            Age=(double)file_age;
                            file_age.DecodeTime(&Hour1, &Min1, &Sec1, &Msec1);
                            file_age.DecodeDate(&year1, &month1, &day1);
                            year=year1; month=month1; day=day1; hour=Hour1; minute=Min1; second=Sec1; msec=Msec1;
                            ms_from_midnight=msec+second*1000+minute*60000+hour*3600000;
                            str=(int)Age; str+="."; str+=ms_from_midnight;
                            serial_number=str.ToDouble();
                        }
                   }
                }
            }  // end  if(underlines>5)
            else
            {
              // go for file age
                file_age=file_age.FileDateToDateTime(FileAge(wave_name));
                Age=(double)file_age;
                file_age.DecodeTime(&Hour1, &Min1, &Sec1, &Msec1);
                file_age.DecodeDate(&year1, &month1, &day1);
                year=year1; month=month1; day=day1; hour=Hour1; minute=Min1; second=Sec1; msec=Msec1;
                ms_from_midnight=msec+second*1000+minute*60000+hour*3600000;
                str=(int)Age; str+="."; str+=ms_from_midnight;
                serial_number=str.ToDouble();
            }

            break;
    }









   /*

        AnsiString x;
        AnsiString name=ExtractFileName(wave_names[0]);
        int string_size=name.Length();
        int i=1;
        double Age;
        //static double previous_serial=0;//, inc=0;
        unsigned short year1, month1, day1, Hour1, Min1, Sec1, Msec1;
        if(string_size<15)format->ItemIndex=1;
        switch(format->ItemIndex)
        {
 // extract data and time from file name -- the fractional part of the serial number is part of
 // the day elapsed (older format, but does not matter)
           case 0:  // bird109_97023_on_Oct_02_11_12
            // find bird ID
            x=""; do{x+=name[i];i++;}while(name[i]!='_' && i<string_size);
            if(i>string_size-3){format->ItemIndex=1;break; }
           // bird_ID=x.ToIntDef(0);

            // find serial number
            x=""; i++; do{x+=name[i]; i++; }while(name[i]!='_' && name[i]!='.' && i<string_size-5);
          //  serial_number=x.ToIntDef(0);
            x=""; i++; do{x+=name[i]; i++; }while(name[i]!='_' && name[i]!='.' && i<string_size-5);
            // fine month
            x=""; i++; do{x+=name[i]; i++; }while(name[i]!='_' && name[i]!='.' && i<string_size-4);
            month=x.ToIntDef(0);

            // find day of month
            x=""; i++; do{x+=name[i]; i++; }while(name[i]!='_' && name[i]!='.' && i<string_size-4);
            day=x.ToIntDef(0);

            // find hour
            x=""; i++; do{x+=name[i]; i++; }while(name[i]!='_' && name[i]!='.' && i<string_size-3);
            hour=x.ToIntDef(0);

            // find minute
            x=""; i++; do{x+=name[i]; i++; }while(name[i]!='_' && name[i]!='.' && i<string_size-3);
            minute=x.ToIntDef(0);

            // find second
            x=""; i++; do{x+=name[i]; i++; }while(name[i]!='_' && name[i]!='.' && i<string_size-3);
            second=x.ToIntDef(0);

            file_age= EncodeDate((Word)year, (Word)month, (Word)day) + EncodeTime((Word)hour, (Word)minute,(Word)second,(Word)0);
            ms_from_midnight=second*1000+minute*60000+hour*3600000;
            str=(int)file_age;
            str+=".";
            str+=ms_from_midnight;
            serial_number=str.ToDouble();


            break;

   // extract date and time from file age
    case 1:
            file_age=file_age.FileDateToDateTime(FileAge(wave_names[0]));//
            Age=(double)file_age;
            //serial_number=Age;//*(long)100000;
            //if(serial_number==previous_serial) serial_number++;
           // previous_serial=serial_number;

            file_age.DecodeTime(&Hour1, &Min1, &Sec1, &Msec1);
            file_age.DecodeDate(&year1, &month1, &day1);
            month=month1;
            day=day1;
            hour=Hour1;
            minute=Min1;
            second=Sec1;
            ms_from_midnight=second*1000+minute*60000+hour*3600000;

            str=(int)Age;
            str+=".";
            str+=ms_from_midnight;
            serial_number=str.ToDouble();

            break;


   // extract date from file age and time from file name ms ellapssed. This is the most accurate time estimate
    case 2:
			file_age=file_age.FileDateToDateTime(FileAge(wave_names[0]));//
            Age=(double)file_age;
            file_age.DecodeDate(&year1, &month1, &day1);
            month=month1;
            day=day1;

            do{i++;}while(name[i]!='_' && i<string_size);
            do{i++;}while(name[i]!='_' && i<string_size);
            do{i++;}while(name[i]!='_' && i<string_size);
            do{i++;}while(name[i]!='_' && i<string_size);

            if(i>string_size-3)
            {format->ItemIndex=1;break; }
            x=""; i++;
            do{x+=name[i]; i++; }while(name[i]!='_' && name[i]!='.' && i<string_size-3);
            ms_from_midnight=x.ToIntDef(0);
            hour=ms_from_midnight/3600000;
            minute= (ms_from_midnight/60000)%60;
            second= (ms_from_midnight/1000)%60;
            str=(int)Age;
            str+=".";
            str+=ms_from_midnight;
            serial_number=str.ToDouble();
            break;


     case 3:     // R109_39152.86197671_3_11_23_56_37.wav
            do{i++;}while(name[i]!='_' && i<string_size);
            i++; x=""; do{x+=name[i]; i++;}while(name[i]!='.' && i<string_size); // find integer part of serial number
            str=x.ToIntDef(0); // days ellapsed from 1900
            i++; x=""; do{x+=name[i]; i++;}while(name[i]!='_' && i<string_size); // find fraction part of serial number
            ms_from_midnight=x.ToIntDef(0);
            hour=ms_from_midnight/3600000;
            minute= (ms_from_midnight/60000)%60;
            second= (ms_from_midnight/1000)%60;
            str+=".";
            str+=ms_from_midnight;
            serial_number=str.ToDouble();
            file_age=serial_number;
            file_age.DecodeDate(&year1, &month1, &day1);
            month=month1;
            day=day1;
            break;

    case 4:     // R109_october_11_2006_23_56_37.wav  // oldest (case 0) is bird109_97023_on_Oct_02_11_12
            do{i++;}while(name[i]!='_' && i<string_size);
            i++; x="";
            do{x+=name[i]; i++;}while(name[i]!='_' && i<string_size); // find month name
            month1=monthToInt(x);
            i++; x="";
            do{x+=name[i]; i++;}while(name[i]!='_' && i<string_size); // find day
            day1=x.ToIntDef(0);
            i++; x="";
            do{x+=name[i]; i++;}while(name[i]!='_' && i<string_size); // find year
            year1=x.ToIntDef(0);
            i++; x="";
            do{x+=name[i]; i++;}while(name[i]!='_' && i<string_size); // find hour
            hour=x.ToIntDef(0);
            i++; x="";
            do{x+=name[i]; i++;}while(name[i]!='_' && i<string_size); // find min
            minute=x.ToIntDef(0);
            i++; x="";
            do{x+=name[i]; i++;}while(name[i]!='_' && i<string_size); // find sec
            second=x.ToIntDef(0);

            file_age=EncodeTime(hour, minute, second, 0);
            file_age+=EncodeDate(year1, month1, day1);
            //serial_number=(double)file_age;
            ms_from_midnight=second*1000+minute*60000+hour*3600000;
            str=(int)file_age;
            str+=".";
            str+=ms_from_midnight;
            serial_number=str.ToDouble();
            year=year1;
            month=month1;
            day=day1;
            break;

        }

    */

}



  int Toption::monthToInt(AnsiString x)
  {
            if(x=="January") month=1;
            else if(x=="February") month=2;
            else if(x=="March") month=3;
            else if(x=="April") month=4;
            else if(x=="May") month=5;
            else if(x=="June") month=6;
            else if(x=="July") month=7;
            else if(x=="August") month=8;
            else if(x=="September") month=9;
            else if(x=="October") month=10;
            else if(x=="November") month=11;
            else if(x=="December") month=12;
            else month=0;
            return(month);
  }
 

void __fastcall Toption::min_freqChange(TObject *Sender)
{
    min_entropy_freq=min_freq->Position;//*frequency_range_factor;
    min_freq_Tb->Value=min_freq->Position*43*frequency_range_factor;
}
//---------------------------------------------------------------------------

void __fastcall Toption::max_freqChange(TObject *Sender)
{
  max_entropy_freq=max_freq->Position;//*frequency_range_factor;
  max_freq_Tb->Value=max_freq->Position*43*frequency_range_factor;
}
//---------------------------------------------------------------------------

void __fastcall Toption::advanceChange(TObject *Sender)
{
  Advance=advance->Position;
  advanceTb->Value=1000.0*(float)advance->Position/(float)sampling_rate;//float(advance->Position)/44.1;
  DataForm->Settings(1, "advance_window", advance->Position);
  DataForm->Settings(2, "advance_window", advance->Position);
}
//---------------------------------------------------------------------------

void __fastcall Toption::spectrum_rangeTbChange(TObject *Sender)
{
  spectrum_range=spectrum_rangeTb->Position;
  spectrum_rangeN->Value=spectrum_rangeTb->Position*86;        
}
//---------------------------------------------------------------------------

void __fastcall Toption::min_syllablesChange(TObject *Sender)
{
 minimum_syllables_per_files=min_syllables->Position;
  mean_syllable_num->Value=min_syllables->Position;
}
//---------------------------------------------------------------------------

void __fastcall Toption::min_peak_freqChange(TObject *Sender)
{
   noise_thresh_freq=min_peak_freq->Position/86;   //frequency_range_factor* we divide by 86 to obtain the raw peak freq values
   min_peak->Value=frequency_range_factor*min_peak_freq->Position;
}
//---------------------------------------------------------------------------

void __fastcall Toption::BoutMinDurTBChange(TObject *Sender)
{
bout_duration_thresh=1.4*BoutMinDurTB->Position;
BoutDurLow->Value=bout_duration_thresh;
}
//---------------------------------------------------------------------------

void __fastcall Toption::SyllMinDurTbChange(TObject *Sender)
{
     syllable_duration_thresh=1.4*SyllMinDurTb->Position;
     SyllDurLow->Value=syllable_duration_thresh;
}
//---------------------------------------------------------------------------

 //cepstrum_range

void __fastcall Toption::upper_cepstChange(TObject *Sender)
{
     upper_cepstN->Value=frequency_range_factor*86*256/upper_cepst->Position;//   86*300/3= 8600Hz
     upper_pitch_bound=upper_cepst->Position;//*frequency_range_factor;
}
//---------------------------------------------------------------------------

void __fastcall Toption::lower_cepstChange(TObject *Sender)
{
     lower_cepstN->Value=frequency_range_factor*86*256/lower_cepst->Position;//   86*300/3= 8600Hz
     lower_pitch_bound=lower_cepst->Position;//*frequency_range_factor;
}
//---------------------------------------------------------------------------

void __fastcall Toption::pitch_averagingClick(TObject *Sender)
{
  if(pitch_averaging->ItemIndex)simple_pitch_averaging=false;
  else simple_pitch_averaging=true;
}
//---------------------------------------------------------------------------

void __fastcall Toption::harmonic_pitchChange(TObject *Sender)
{
   harmonic_pitch_thresh=harmonic_pitch->Position*86;//frequency_range_factor*
   harmonic_pitch_threshB->Value=harmonic_pitch->Position*86*frequency_range_factor;
}
//---------------------------------------------------------------------------

void __fastcall Toption::min_syllable_durTbChange(TObject *Sender)
{
  min_syllable_duration=min_syllable_durTb->Position;
  min_syllable_durN->Value=min_syllable_durTb->Position*advanceTb->Value;
}
//---------------------------------------------------------------------------

void __fastcall Toption::min_stop_durTbChange(TObject *Sender)
{
  min_stop_duration=min_stop_durTb->Position;
  min_stop_durN->Value=min_stop_durTb->Position*advanceTb->Value;//*1.4;
}
//---------------------------------------------------------------------------



void __fastcall Toption::Serial_by_timeCbClick(TObject *Sender)
{
  if(Serial_by_timeCb->Checked)serial_by_time=true;
  else serial_by_time=false;
}
//---------------------------------------------------------------------------

void __fastcall Toption::sound_detection_termsClick(TObject *Sender)
{
   if(sound_detection_terms->ItemIndex){  entropy_and_power=false; entropy_or_power=true;}
   else {  entropy_and_power=true; entropy_or_power=false;}
}
//---------------------------------------------------------------------------



void __fastcall Toption::bout_stopChange(TObject *Sender)
{
   stop_duration_thresh=bout_stop->Position;
   bout_Stop_N->Value=bout_stop->Position*advanceTb->Value;//*1.4;
}
//---------------------------------------------------------------------------


void __fastcall Toption::TrackBar2Change(TObject *Sender)
{
medianPitch->Value=TrackBar2->Position;        
}
//---------------------------------------------------------------------------

void __fastcall Toption::TrackBar4Change(TObject *Sender)
{
medianFM->Value=(float)TrackBar4->Position/10.0;        
}
//---------------------------------------------------------------------------

void __fastcall Toption::TrackBar6Change(TObject *Sender)
{
medianEntropy->Value=(float)TrackBar6->Position/(-100.0);        
}
//---------------------------------------------------------------------------

void __fastcall Toption::TrackBar8Change(TObject *Sender)
{
medianPeakFr->Value=(float)TrackBar8->Position;
}
//---------------------------------------------------------------------------

void __fastcall Toption::TrackBar1Change(TObject *Sender)
{
medianPgood->Value=(float)TrackBar1->Position;
}
//---------------------------------------------------------------------------

void __fastcall Toption::TrackBar5Change(TObject *Sender)
{
deviationFM->Value=(float)TrackBar5->Position/10.0;
}
//---------------------------------------------------------------------------

void __fastcall Toption::TrackBar11Change(TObject *Sender)
{
medianVarPitch->Value=TrackBar11->Position*1000;
}
//---------------------------------------------------------------------------

void __fastcall Toption::TrackBar14Change(TObject *Sender)
{
medianVarEnt->Value=(float)TrackBar14->Position/1000.0;
}
//---------------------------------------------------------------------------

void __fastcall Toption::ContTBChange(TObject *Sender)
{
medianVarFM->Value=ContTB->Position;        
}
//---------------------------------------------------------------------------

void __fastcall Toption::TrackBar15Change(TObject *Sender)
{
medianVarPeakFr->Value=(float)TrackBar15->Position*5000.0;
}
//---------------------------------------------------------------------------

void __fastcall Toption::TrackBar17Change(TObject *Sender)
{
medianVarPGood->Value=(float)TrackBar17->Position*100.0;
}
//---------------------------------------------------------------------------

void __fastcall Toption::TrackBar20Change(TObject *Sender)
{
medianDuration->Value=TrackBar20->Position;
}
//---------------------------------------------------------------------------

void __fastcall Toption::TrackBar3Change(TObject *Sender)
{
deviationPitch->Value=TrackBar3->Position;        
}
//---------------------------------------------------------------------------

void __fastcall Toption::TrackBar7Change(TObject *Sender)
{
deviationEntropy->Value=(float)TrackBar7->Position/100.0;        
}
//---------------------------------------------------------------------------

void __fastcall Toption::TrackBar9Change(TObject *Sender)
{
deviationPeakFr->Value=(float)TrackBar9->Position;
}
//---------------------------------------------------------------------------

void __fastcall Toption::TrackBar10Change(TObject *Sender)
{
deviationPgood->Value=(float)TrackBar10->Position;
}
//---------------------------------------------------------------------------

void __fastcall Toption::TrackBar12Change(TObject *Sender)
{
deviationVarPitch->Value=TrackBar12->Position*1000;
}
//---------------------------------------------------------------------------

void __fastcall Toption::TrackBar13Change(TObject *Sender)
{
deviationVarEnt->Value=(float)TrackBar13->Position/1000.0;
}
//---------------------------------------------------------------------------

void __fastcall Toption::ContDVTBChange(TObject *Sender)
{
deviationVarFM->Value=(float)ContDVTB->Position;        
}
//---------------------------------------------------------------------------

void __fastcall Toption::TrackBar16Change(TObject *Sender)
{
deviationVarPeakFr->Value=(float)TrackBar16->Position*5000.0;
}
//---------------------------------------------------------------------------

void __fastcall Toption::TrackBar18Change(TObject *Sender)
{
deviationVarPGood->Value=(float)TrackBar18->Position*100.0;
}
//---------------------------------------------------------------------------

void __fastcall Toption::TrackBar19Change(TObject *Sender)
{
deviationDuration->Value=(float)TrackBar19->Position/10.0;
}
//---------------------------------------------------------------------------

void __fastcall Toption::allow_overwriteClick(TObject *Sender)
{
    if(allow_overwrite->Checked)check_overwrite=false;
    else check_overwrite=true;
}
//---------------------------------------------------------------------------

void __fastcall Toption::TimeWarpTBChange(TObject *Sender)
{
TimeWarpLB->Value=(float)TimeWarpTB->Position/100;
}
//---------------------------------------------------------------------------

void __fastcall Toption::pitchWTBChange(TObject *Sender)
{
pitchWLb->Value=(float)pitchWTB->Position/10;
}
//---------------------------------------------------------------------------

void __fastcall Toption::FMWTBChange(TObject *Sender)
{
FMWLB->Value=(float)FMWTB->Position/10;
}
//---------------------------------------------------------------------------

void __fastcall Toption::entropyWTBChange(TObject *Sender)
{
entropyWLB->Value=(float)entropyWTB->Position/10;
}
//---------------------------------------------------------------------------

void __fastcall Toption::goodnessWTBChange(TObject *Sender)
{
  goodnessWLB->Value=(float)goodnessWTB->Position/10;
}
//---------------------------------------------------------------------------

void __fastcall Toption::pitchWCbClick(TObject *Sender)
{
if(!FMWCB->Checked && !entropyWCB->Checked && !goodnessWCB->Checked)pitchWCb->Checked=true;
        if(pitchWCb->Checked==true)
        {
                pitchWTB->Visible=true;
                pitchWLb->Value=(float)pitchWTB->Position/10;
        }
        else {pitchWTB->Visible=false; pitchWLb->Value=0;}        
}
//---------------------------------------------------------------------------

void __fastcall Toption::FMWCBClick(TObject *Sender)
{
 if(!pitchWCb->Checked && !entropyWCB->Checked && !goodnessWCB->Checked)FMWCB->Checked=true;
      if(FMWCB->Checked==true)
        {
                FMWTB->Visible=true;
                FMWLB->Value=(float)FMWTB->Position/10;
        }
        else {FMWTB->Visible=false; FMWLB->Value=0;}        
}
//---------------------------------------------------------------------------

void __fastcall Toption::entropyWCBClick(TObject *Sender)
{
if(!FMWCB->Checked && !pitchWCb->Checked && !goodnessWCB->Checked)entropyWCB->Checked=true;
     if(entropyWCB->Checked==true)
        {
                entropyWTB->Visible=true;
                entropyWLB->Value=(float)entropyWTB->Position/10;
        }
        else {entropyWTB->Visible=false; entropyWLB->Value=0;}        
}
//---------------------------------------------------------------------------

void __fastcall Toption::contWCBClick(TObject *Sender)
{
if(!FMWCB->Checked && !entropyWCB->Checked && !pitchWCb->Checked)goodnessWCB->Checked=true;
     if(goodnessWCB->Checked==true)
        {
                goodnessWTB->Visible=true;
                goodnessWLB->Value=(float)goodnessWTB->Position/10;
        }
        else {goodnessWTB->Visible=false; goodnessWLB->Value=0;}
}
//---------------------------------------------------------------------------

void __fastcall Toption::updateClick(TObject *Sender)
{
   DataForm->settings->Active=true;
   DataSource2->DataSet=DataForm->settings;
   CRDBGrid1->DataSource=DataSource2;
   CRDBGrid1->Enabled=true;
}
//---------------------------------------------------------------------------


void __fastcall Toption::testClick(TObject *Sender)
{
  if(!DirectoryExists(in_sound->Text))
        if(!ForceDirectories(in_sound->Text))
			   Application->MessageBox(L"input sound folder cannot be created: please go to 'input setting' change folder names and click 'test'", NULL, MB_OK);
  if(!DirectoryExists(out_sound->Text)) if(!ForceDirectories(out_sound->Text))
			   Application->MessageBox(L"output sound folder cannot be created: please go to 'input setting' change folder names and click 'test'", NULL, MB_OK);
 // if(!DirectoryExists(out_bin->Text)) if(!ForceDirectories(out_bin->Text))
 //              Application->MessageBox(L"output binary folder cannot be created: please go to 'input setting' change folder names and click 'test'", NULL, MB_OK);
  raw_feature_folder="c:\\SAP\\bin\\";
  moveTo_sound_folders[0]="c:\\SAP\\sounds\\";
}
//---------------------------------------------------------------------------



void __fastcall Toption::data_windowChange(TObject *Sender)
{
  windowTB->Value=(float)data_window->Position/44.1;
  DataForm->Settings(1, "FFT_window", data_window->Position);
  DataForm->Settings(2, "FFT_window", data_window->Position);
  ztSamplesPerWindow=data_window->Position;
  tapers_done=true;
  StartForm->ReadTapers();
}
//---------------------------------------------------------------------------


void __fastcall Toption::goodness_thresh_TBChange(TObject *Sender)
{
   pitch_goodness_thresh=goodness_thresh_TB->Position;
   goodness_thresh->Value=goodness_thresh_TB->Position;
}
//---------------------------------------------------------------------------

void __fastcall Toption::entropy_threshold_TBChange(TObject *Sender)
{
   pitch_entropy_thresh=(float)-entropy_threshold_TB->Position/10.0;
   Entropy_thresh->Value=(float)-entropy_threshold_TB->Position/10.0;
}
//---------------------------------------------------------------------------


void __fastcall Toption::pitchTBChange(TObject *Sender)
{
  pitchOpLb->Value=(float)pitchTB->Position/1000.0;
}
//---------------------------------------------------------------------------

void __fastcall Toption::FMTBChange(TObject *Sender)
{
  FMOpLb->Value=(float)FMTB->Position/100.0;
}
//---------------------------------------------------------------------------

void __fastcall Toption::EntropyTBChange(TObject *Sender)
{
   EntropyOpLb->Value=-(float)EntropyTB->Position/1000.0;
}
//---------------------------------------------------------------------------

void __fastcall Toption::PgoodTBChange(TObject *Sender)
{
  PgoodOpLb->Value=(float)PgoodTB->Position/1000.0;
}
//---------------------------------------------------------------------------

void __fastcall Toption::PitchDVTBChange(TObject *Sender)
{
  PitchDVLB->Value=(float)PitchDVTB->Position/1000.0;
}
//---------------------------------------------------------------------------

void __fastcall Toption::FMDVTBChange(TObject *Sender)
{
  FMDVLB->Value=(float)FMDVTB->Position/100.0;
}
//---------------------------------------------------------------------------

void __fastcall Toption::EntropyDVTBChange(TObject *Sender)
{
  EntropyDVLB->Value=(float)EntropyDVTB->Position/1000.0;
}
//---------------------------------------------------------------------------

void __fastcall Toption::PgoodDVTBChange(TObject *Sender)
{
  PgoodDVLB->Value=(float)PgoodDVTB->Position/1000.0;
}
//---------------------------------------------------------------------------

void __fastcall Toption::boost_ampClick(TObject *Sender)
{
  if(boost_amp->Checked) boost_amplitude=true;
  else boost_amplitude=false;        
}
//---------------------------------------------------------------------------

void __fastcall Toption::startClick(TObject *Sender)
{
   StartForm->Visible=true;
   StartForm->BringToFront();      
}
//---------------------------------------------------------------------------

void __fastcall Toption::AMTBChange(TObject *Sender)
{
   AMOpLb->Value=(float)AMTB->Position/100.0;
}
//---------------------------------------------------------------------------

void __fastcall Toption::AMDVTBChange(TObject *Sender)
{
    AMDVLB->Value=(float)AMDVTB->Position/100.0;
}
//-------------------------------------------
void __fastcall Toption::AMWTBChange(TObject *Sender)
{
        AMWLB->Value=(float)AMWTB->Position/10;        
}
//---------------------------------------------------------------------------

void __fastcall Toption::Amplitude_threshChange(TObject *Sender)
{
		//amplitude_thresh=125-Amplitude_thresh->Position;
	   // amptreshL->Value=amplitude_thresh;
}
//---------------------------------------------------------------------------

void __fastcall Toption::Entropy_thresholdChange(TObject *Sender)
{
		entropy_thresh=(float)(-Entropy_threshold->Position)/50.0;
        enttreshL->Value=entropy_thresh;
}
//---------------------------------------------------------------------------

void __fastcall Toption::CSpinButton1DownClick(TObject *Sender)
{
  scale->Next();
  setScale();
}
//---------------------------------------------------------------------------

void __fastcall Toption::CSpinButton1UpClick(TObject *Sender)
{
  scale->Prior();
  setScale();
}
//---------------------------------------------------------------------------

void Toption::setScale()
{
   pitchOpLb->Value=scale->FieldValues["log_pitch_median"];
   PitchDVLB->Value=scale->FieldValues["log_pitch_mad"];
   FMOpLb->Value=scale->FieldValues["fm_median"];
   FMDVLB->Value=scale->FieldValues["fm_mad"];
   AMOpLb->Value=scale->FieldValues["am_median"];
   AMDVLB->Value=scale->FieldValues["am_mad"];
   EntropyOpLb->Value=scale->FieldValues["entropy_median"];
   EntropyDVLB->Value=scale->FieldValues["entropy_mad"];
   PgoodOpLb->Value=scale->FieldValues["log_goodness_of_pitch_median"];
   PgoodDVLB->Value=scale->FieldValues["log_goodness_of_pitch_mad"];
   medianDuration->Value=scale->FieldValues["syllable_duration_median"];
   deviationDuration->Value=scale->FieldValues["syllable_duration_mad"];
   medianPitch->Value=scale->FieldValues["syllable_mean_pitch_median"];
   deviationPitch->Value=scale->FieldValues["syllable_mean_pitch_mad"];
   medianFM->Value=scale->FieldValues["syllable_mean_FM_median"];
   deviationFM->Value=scale->FieldValues["syllable_mean_FM_mad"];
   medianEntropy->Value=scale->FieldValues["syllable_mean_entropy_median"];
   deviationEntropy->Value=scale->FieldValues["syllable_mean_entropy_mad"];
   medianPgood->Value=scale->FieldValues["syllable_mean_goodness_of_pitch_median"];
   deviationPgood->Value=scale->FieldValues["syllable_mean_goodness_of_pitch_mad"];
   medianPeakFr->Value=scale->FieldValues["syllable_mean_peak_frequency_median"];
   deviationPeakFr->Value=scale->FieldValues["syllable_mean_peak_frequency_mad"];
   medianVarPitch->Value=scale->FieldValues["syllable_var_pitch_median"];
   deviationVarPitch->Value=scale->FieldValues["syllable_var_pitch_mad"];
   medianVarFM->Value=scale->FieldValues["syllable_var_FM_median"];
   deviationVarFM->Value=scale->FieldValues["syllable_var_FM_mad"];
   medianVarEnt->Value=scale->FieldValues["syllable_var_entropy_median"];
   deviationVarEnt->Value=scale->FieldValues["syllable_var_entropy_mad"];
   medianVarPGood->Value=scale->FieldValues["syllable_var_goodness_of_pitch_median"];
   deviationVarPGood->Value=scale->FieldValues["syllable_var_goodness_of_pitch_mad"];
   medianVarPeakFr->Value=scale->FieldValues["syllable_var_peak_frequency_median"];
   deviationVarPeakFr->Value=scale->FieldValues["syllable_var_peak_frequency_mad"];
}

void Toption::create_feature_scale()
{
  AnsiString str;
  str="CREATE TABLE IF NOT EXISTS `feature_scale`(`domain` varchar(100) NOT NULL default '',";

  str+="`frequency_range` float(22,5) NOT NULL default '1',";
  str+="`deriv_thresh` float(22,5) NOT NULL default '50',";
  str+="`entropy_thresh` float(22,5) NOT NULL default '20',";
  str+="`display_contrast` float(22,5) NOT NULL default '10',";
  str+="`FFT_window` float(22,5) NOT NULL default '300',";
  str+="`advance_window` float(22,5) NOT NULL default '100',";
  str+="`hp_big` float(22,5) NOT NULL default '500',";
  str+="`hp_small` float(22,5) NOT NULL default '50',";
  str+="`fine_segmentation` tinyint(4) default '0',";
  str+="`pagination` tinyint(4) default '0',";
  str+="`log_pitch_median` float(22,5) NOT NULL default '0.00000',";
  str+="`log_pitch_mad` float(22,5) NOT NULL default '0.00000', ";
  str+="`fm_median` float(22,5) NOT NULL default '0.00000',";
  str+="`fm_mad` float(22,5) NOT NULL default '0.00000',";
  str+="`am_median` float(22,5) NOT NULL default '0.00000',";
  str+="`am_mad` float(22,5) NOT NULL default '0.00000',";
  str+="`entropy_median` float(22,5) NOT NULL default '0.00000',";
  str+="`entropy_mad` float(22,5) NOT NULL default '0.00000',";
  str+="`log_goodness_of_pitch_median` float(22,5) NOT NULL default '0.00000',";
  str+="`log_goodness_of_pitch_mad` float(22,5) NOT NULL default '0.00000',";
  str+="`syllable_duration_median` float(22,5) NOT NULL default '0.00000',";
  str+="`syllable_duration_mad` float(22,5) NOT NULL default '0.00000', ";
  str+="`syllable_mean_pitch_median` float(22,5) NOT NULL default '0.00000',";
  str+="`syllable_mean_pitch_mad` float(22,5) NOT NULL default '0.00000', ";
  str+="`syllable_mean_FM_median` float(22,5) NOT NULL default '0.00000',";
  str+="`syllable_mean_FM_mad` float(22,5) NOT NULL default '0.00000',";
  str+="`syllable_mean_entropy_median` float(22,5) NOT NULL default '0.00000',";
  str+="`syllable_mean_entropy_mad` float(22,5) NOT NULL default '0.00000',";
  str+="`syllable_mean_goodness_of_pitch_median` float(22,5) NOT NULL default '0.00000',";
  str+="`syllable_mean_goodness_of_pitch_mad` float(22,5) NOT NULL default '0.00000',";
  str+="`syllable_mean_peak_frequency_median` float(22,5) NOT NULL default '0.00000',";
  str+="`syllable_mean_peak_frequency_mad` float(22,5) NOT NULL default '0.00000',";
  str+="`syllable_var_pitch_median` float(22,5) NOT NULL default '0.00000', ";
  str+="`syllable_var_pitch_mad` float(22,5) NOT NULL default '0.00000',";
  str+="`syllable_var_FM_median` float(22,5) NOT NULL default '0.00000',";
  str+="`syllable_var_FM_mad` float(22,5) NOT NULL default '0.00000', ";
  str+="`syllable_var_entropy_median` float(22,5) NOT NULL default '0.00000',";
  str+="`syllable_var_entropy_mad` float(22,5) NOT NULL default '0.00000', ";
  str+="`syllable_var_goodness_of_pitch_median` float(22,5) NOT NULL default '0.00000', ";
  str+="`syllable_var_goodness_of_pitch_mad` float(22,5) NOT NULL default '0.00000', ";
  str+="`syllable_var_peak_frequency_median` float(22,5) NOT NULL default '0.00000',";
  str+="`syllable_var_peak_frequency_mad` float(22,5) NOT NULL default '0.00000' )";
  DataForm->MyCommand1->SQL->Clear();
  DataForm->MyCommand1->SQL->Add(str);
  DataForm->MyCommand1->Execute(1);
  scale->Active=true;
  add_record_to_scale("zebra finch");

}


void __fastcall Toption::newScaleClick(TObject *Sender)
{
  AnsiString str = InputBox("Feature scale settings", "Type a name for the new settings, e.g., 'canary'", "");
  if(str.Length()>1) add_record_to_scale(str);
}


void Toption::add_record_to_scale(AnsiString str)
{
   scale->Last();
   scale->Append();

   scale->FieldValues["frequency_range"]=frequency_range->Position;
   scale->FieldValues["deriv_thresh"]=Amplitude_thresh->Position;
   scale->FieldValues["entropy_thresh"]=Entropy_threshold->Position;
   scale->FieldValues["display_contrast"]=sldrBrightness->Position;
   scale->FieldValues["FFT_window"]=data_window->Position;
   scale->FieldValues["advance_window"]=advance->Position;
   scale->FieldValues["fine_segmentation"]=hp_amp;
   if (paginationOn) scale->FieldValues["pagination"]=1;
	else scale->FieldValues["pagination"]=paginationOn;
   scale->FieldValues["hp_big"]=hp_big_num->Value;
   scale->FieldValues["hp_small"]=hp_small_num->Value;

   scale->FieldValues["domain"]=str;
   scale->FieldValues["log_pitch_median"]=pitchOpLb->Value;
   scale->FieldValues["log_pitch_mad"]=PitchDVLB->Value;
   scale->FieldValues["fm_median"]=FMOpLb->Value;
   scale->FieldValues["fm_mad"]=FMDVLB->Value;
   scale->FieldValues["am_median"]=AMOpLb->Value;
   scale->FieldValues["am_mad"]=AMDVLB->Value;
   scale->FieldValues["entropy_median"]=EntropyOpLb->Value;
   scale->FieldValues["entropy_mad"]=EntropyDVLB->Value;
   scale->FieldValues["log_goodness_of_pitch_median"]=PgoodOpLb->Value;
   scale->FieldValues["log_goodness_of_pitch_mad"]=PgoodDVLB->Value;
   scale->FieldValues["syllable_duration_median"]=medianDuration->Value;
   scale->FieldValues["syllable_duration_mad"]=deviationDuration->Value;
   scale->FieldValues["syllable_mean_pitch_median"]=medianPitch->Value;
   scale->FieldValues["syllable_mean_pitch_mad"]=deviationPitch->Value;
   scale->FieldValues["syllable_mean_FM_median"]=medianFM->Value;
   scale->FieldValues["syllable_mean_FM_mad"]=deviationFM->Value;
   scale->FieldValues["syllable_mean_entropy_median"]=medianEntropy->Value;
   scale->FieldValues["syllable_mean_entropy_mad"]=deviationEntropy->Value;
   scale->FieldValues["syllable_mean_goodness_of_pitch_median"]=medianPgood->Value;
   scale->FieldValues["syllable_mean_goodness_of_pitch_mad"]=deviationPgood->Value;
   scale->FieldValues["syllable_mean_peak_frequency_median"]=medianPeakFr->Value;
   scale->FieldValues["syllable_mean_peak_frequency_mad"]=deviationPeakFr->Value;
   scale->FieldValues["syllable_var_pitch_median"]=medianVarPitch->Value;
   scale->FieldValues["syllable_var_pitch_mad"]=deviationVarPitch->Value;
   scale->FieldValues["syllable_var_FM_median"]=medianVarFM->Value;
   scale->FieldValues["syllable_var_FM_mad"]=deviationVarFM->Value;
   scale->FieldValues["syllable_var_entropy_median"]=medianVarEnt->Value;
   scale->FieldValues["syllable_var_entropy_mad"]=deviationVarEnt->Value;
   scale->FieldValues["syllable_var_goodness_of_pitch_median"]=medianVarPGood->Value;
   scale->FieldValues["syllable_var_goodness_of_pitch_mad"]=deviationVarPGood->Value;
   scale->FieldValues["syllable_var_peak_frequency_median"]=medianVarPeakFr->Value;
   scale->FieldValues["syllable_var_peak_frequency_mad"]=deviationVarPeakFr->Value;
   scale->Append();
}

//---------------------------------------------------------------------------

void __fastcall Toption::TrackBar21Change(TObject *Sender)
{
  medianStop->Value=TrackBar21->Position;
}
//---------------------------------------------------------------------------

void __fastcall Toption::TrackBar22Change(TObject *Sender)
{
   deviationStop->Value=TrackBar22->Position;
}
//---------------------------------------------------------------------------




void __fastcall Toption::problems_trackChange(TObject *Sender)
{
  problems_num->Value=problems_track->Position;
  problems_thresh=problems_num->Value;
}
//---------------------------------------------------------------------------

void __fastcall Toption::calc_continuityClick(TObject *Sender)
{
   if(calc_continuity->Checked)calculate_continuity=true;
   else calculate_continuity=false;
}
//---------------------------------------------------------------------------

void __fastcall Toption::cont_t_medianTbChange(TObject *Sender)
{
   cont_t_medianVal->Value=cont_t_medianTb->Position;
}
//---------------------------------------------------------------------------

void __fastcall Toption::cont_t_madTbChange(TObject *Sender)
{
cont_t_madVal->Value=cont_t_madTb->Position;

}
//---------------------------------------------------------------------------

void __fastcall Toption::cont_f_medianTbChange(TObject *Sender)
{
cont_f_medianVal->Value=cont_f_medianTb->Position;

}
//---------------------------------------------------------------------------

void __fastcall Toption::cont_f_madTbChange(TObject *Sender)
{
        cont_f_madVal->Value=cont_f_madTb->Position;
}
//---------------------------------------------------------------------------

void __fastcall Toption::s_min_fr_tbChange(TObject *Sender)
{
   s_min_fr->Value=43*s_min_fr_tb->Position;
}
//---------------------------------------------------------------------------

void __fastcall Toption::s_max_fr_tbChange(TObject *Sender)
{
 s_max_fr->Value=43*s_max_fr_tb->Position;
}
//---------------------------------------------------------------------------

void __fastcall Toption::amp_baseTbChange(TObject *Sender)
{
   amp_base->Value=amp_baseTb->Position;
   baseline=amp_base->Value;        
}
//---------------------------------------------------------------------------




void __fastcall Toption::noise_directionClick(TObject *Sender)
{
  if(noise_detect->ItemIndex) detect_low_noise=true;
  else detect_low_noise=false;
}
//---------------------------------------------------------------------------

void __fastcall Toption::pitch_methodClick(TObject *Sender)
{
  switch(pitch_method->ItemIndex)
  {
    case 0:
                dynamic_pitch->Visible=false;
                cepst_bounds->Visible=true;
                contour_pitch=false;
                peak_frequency_pitch=false;
				dynamic_pitch_calculation=false;
				cepstrum_pitch=true;
                break;
    case 1:
                dynamic_pitch->Visible=false;
                cepst_bounds->Visible=false;
                contour_pitch=false;
                peak_frequency_pitch=true;
				dynamic_pitch_calculation=false;
				cepstrum_pitch=true;
                break;
    case 2:
                dynamic_pitch->Visible=true;
                cepst_bounds->Visible=true;
				contour_pitch=false;
                peak_frequency_pitch=false;
				dynamic_pitch_calculation=true;
				cepstrum_pitch=true;
				break;
	case 3:
				dynamic_pitch->Visible=false;
                cepst_bounds->Visible=false;
				contour_pitch=true;
                peak_frequency_pitch=false;
				dynamic_pitch_calculation=false;
				cepstrum_pitch=false;
    break;

  }
}
//---------------------------------------------------------------------------

void __fastcall Toption::age_or_dayClick(TObject *Sender)
{
if(age_or_day->ItemIndex) age_in_table=false;
  else age_in_table=true;
}
//---------------------------------------------------------------------------

void __fastcall Toption::frequency_rangeChange(TObject *Sender)
{
  int i=pow(2.0,4-frequency_range->Position);
  frequency_range_val->Value=sampling_rate/i;
  DataForm->Settings(1, "frequency_range", frequency_range->Position);
  switch(frequency_range->Position)
  {
    case 1: frequency_range_factor=0.5; range_x2=false;  range_x05=true; break;
    case 2: frequency_range_factor=1; range_x2=false;  range_x05=false; break;
    case 3: frequency_range_factor=2; range_x2=true;  range_x05=false; break;
  }
  frequency_range_adjustments();
}
//---------------------------------------------------------------------------


void Toption::frequency_range_adjustments()
{
  min_freqChange(this);
  max_freqChange(this);
  upper_cepstChange(this);
  lower_cepstChange(this);
 // harmonic_pitchChange(this);
 // min_peak_freqChange(this);

}






void __fastcall Toption::fine_segmentationClick(TObject *Sender)
{
  segmentation_pannel->Visible=fine_segmentation->Down;
}
//---------------------------------------------------------------------------

void __fastcall Toption::HP_bigChange(TObject *Sender)
{
  hp_big_num->Value=HP_big->Position;
}
//---------------------------------------------------------------------------

void __fastcall Toption::HP_smallChange(TObject *Sender)
{
  hp_small_num->Value=HP_small->Position;
}
//---------------------------------------------------------------------------
 float Toption::adjustThreshold(int feature, float thresh, int frequency, bool pri) {
 float adjValue;
 float factor = frequency / 125;
  switch(feature) {
	case 0:
         adjValue = (125 - thresh) / 1.25;
		 break;
	case 1:
		 adjValue = (125 - thresh) * factor;
		 break;
	case 2:
         adjValue = (125 - thresh) * factor;
		 break;
	case 3:
         adjValue = (125 - thresh) * 36;
		 break;
	case 4:
         adjValue = (125 - thresh) * 0.72;
		 break;
	case 5:
         adjValue = (62.5 - (float)thresh) / 62.5;
		 break;
	case 6:
         adjValue = (-thresh) * 0.064;
		 break;
	case 7:
         adjValue = (125 - thresh) * 2.4;
		 break;
	case 8:
         adjValue = (125 - thresh) * 2.4;
		 break;
	default:
            adjValue = (125 - thresh) / 1.25;
	}
    /*if (pri) {
       pri_thresh = adjValue;
    }  else {
       sec_thresh = adjValue;
    }*/
    return adjValue;
 }

 //---------------------------------------------------------------------------
void __fastcall Toption::minfr_tbChange(TObject *Sender)
{
   minfr->Value=minfr_tb->Position;
   minFreq=minfr_tb->Position;
}
//---------------------------------------------------------------------------

