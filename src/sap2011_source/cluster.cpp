//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "cluster.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <vector.h>
#include <list.h>
#include <algorithm>
#include <functional>
#include <iostream>
#include <algorithm>
//#include <mmsystem>
#include "SDL_matrix.hpp"
#include <fstream>
#include "start.h"
#include "records.h"
#include <iostream>
#include "CZT_FeatureCalc.h"
#include "options3.h"
//#pragma link "Matrix"
#define X_axis   0
#pragma package(smart_init)
#pragma link "SDL_NumLab"
#pragma link "SDL_rchart"
#pragma link "DBAccess"
#pragma link "MemDS"
#pragma link "MyAccess"
#pragma link "SDL_matrix"
#pragma link "DBAccess"
#pragma link "MemDS"
#pragma link "MyAccess"
#pragma link "SDL_NumLab"
#pragma link "SDL_rchart"
#pragma resource "*.dfm"

// we first define two structures (classes):
// EuclidianDist for saving the indexes (locations) of a pair of syllables
// and the distance between them
struct EuclideanDist
{
  int index1, index2;
  float distance;
  EuclideanDist(int ind1, int ind2, float dist) : index1(ind1), index2(ind2), distance(dist) {};
  EuclideanDist() : index1(0), index2(0), distance(0){};
};
bool operator<(const EuclideanDist &x, const EuclideanDist &y)
{ return x.distance < y.distance; }

/* the second structure, boundary is for keeping clusters indexes: the vector clust hold the actual clusters
  e.g., [0 x1 x2 ...0 y1 y2...0] and so on. boundaries tell us the starting point, endpoint and duration of each cluster */
struct boundary
{
  int start, end, size;
  boundary( int st, int en, int si) : start(st), end(en), size(si) {};
  boundary() : start(0), end(0), size(0){};
};


bool operator<(const boundary &x, const boundary &y)
{ return x.size < y.size; }


// we now declare vectores of those types and an also a simple integer vector tmpClust to hold clusters
vector<EuclideanDist> SyllableDist;
//slist<EuclideanDist> SyllableDist;
vector<int>tmpClust;
vector<boundary>boundaries;


//---------------------------------------------------------------------------

TclusterIt *clusterIt;
//---------------------------------------------------------------------------
__fastcall TclusterIt::TclusterIt(TComponent* Owner)
        : TForm(Owner)
{

  //MyConnection1->Connect();
  //MyConnection1->Database="sap";

  option->select_table=0;
  interval=new float*[20000];  // interval is a memory copy of a database interval used for clustering, can include up to 20000 records
  for(int i=0;i<20000;i++)interval[i]=new float[12];  // it contains up to 12 features
  for(int i=0;i<20000;i++)for(int j=0;j<12;j++) interval[i][j]=0;
  SyllableDist.reserve(200000); // SyllableDist store distances between pairs of syllables
  tmpClust.reserve(10000); boundaries.reserve(256);
 // avgFeature=new float*[10];
 // for(int i=0;i<10;i++) avgFeature[i]=new float[10];
 // avgRefFeature=new float*[10];
 //  for(int i=0;i<10;i++) avgRefFeature[i]=new float[10];

 /*
 Shape11->Brush->Color=TColor(RGB(250,180,250));
 Shape12->Brush->Color=TColor(RGB(250,250,180));
 Shape13->Brush->Color=TColor(RGB(180,180,250));
 Shape14->Brush->Color=TColor(RGB(180,250,180));
 Shape15->Brush->Color=TColor(RGB(100,100,100));
 */
 Shape11->Brush->Color=TColor(RGB(180,250,250));
 Shape12->Brush->Color=TColor(RGB(250,180,250));
 Shape13->Brush->Color=TColor(RGB(250,250,180));
 Shape14->Brush->Color=TColor(RGB(180,180,250));
 Shape15->Brush->Color=TColor(RGB(180,250,180));

  getScreenSetting("cls_xaxis");
  featureX->ItemIndex = cs.getXAxis();
  getScreenSetting("cls_yaxis");
  featureY->ItemIndex = cs.getYAxis();

  getScreenSetting("cls_intervalSize");
  intervalSize->Position = cs.getIntervalSize();
  for(int i=0;i<6;i++){features->Checked[i]=true; includeIt[i]=true;}


  intervalN->Value=intervalSize->Position;


  //getScreenSetting("cls_interval");
  //RadioGroup1->ItemIndex = cs.getInterval();
  DataForm->TheTable=DataForm->Records;
  first=true; // tell us that this is the first time we compute (in contrast to track a cluster found earlier...)
  ChosenOne=0;   // this is the cluster that the user chose to trace -- nothing chosen yet.

  getScreenSetting("cls_chosenCluster");
  ChosenCluster->ItemIndex = cs.getChosenCluster();
  getScreenSetting("cls_clusterName");
  clusterName->Text = cs.getChosenClusterName();


  problems=false;
  stayThere=true;   // meaning do not shift forward or backword until user decided to do so
  DVD_play=false;
  bias=0;
  dot=0;

  getScreenSetting("cls_dot");
  dot_size->ItemIndex = cs.getDotSize();
  getScreenSetting("cls_zoom");
  zoom1->ItemIndex = cs.getZoom();


  getScreenSetting("cls_eudist");
  EuclideanTB->Position=cs.getEuDist();
  getScreenSetting("cls_showclust");
  showClust->Position=cs.getShowClust();
  getScreenSetting("cls_iteration");
  Iterations->Position=cs.getIteration();
  getScreenSetting("cls_writepermit");
  writePremit->Checked=cs.getWritePermit();
  getScreenSetting("cls_overwrite");
  overwriteClusters->Checked=cs.getOverwrite();



  getScreenSetting("cls_mindur");
  thresh->Position=cs.getMinDur();
  getScreenSetting("cls_maxdur");
  TrackBar21->Position=cs.getMaxDur();
  getScreenSetting("cls_matchtolerance");
  toleranceTb->Position=cs.getMatchTolerance();
  getScreenSetting("cls_durdiff");
  TrackBar1->Position=cs.getDurDiff();
  getScreenSetting("cls_pitchdiff");
  TrackBar18->Position=cs.getPitchDiff();
  getScreenSetting("cls_fmdiff");
  TrackBar19->Position=cs.getFmDiff();
  getScreenSetting("cls_entropydiff");
  TrackBar20->Position=cs.getEntropyDiff();
  getScreenSetting("cls_filter");
  quary->Text = cs.getFilter();



  //clust.reserve(20000);
  color[0]=clRed;
  color[1]=clBlue;
  color[2]=clLime;
  color[3]=clYellow;
  color[4]=(TColor)RGB(0,0,0);
  color[5]=clFuchsia;
  color[6]=clOlive;
  color[7]=clSilver;
  color[8]=clGreen;
  color[9]=clAqua;
  color[10]=TColor(RGB(180,250,250));
  color[11]=TColor(RGB(250,180,250));
  color[12]=TColor(RGB(250,250,180));
  color[13]=TColor(RGB(180,180,250));
  color[14]=TColor(RGB(180,250,180));
  color[15]=TColor(RGB(100,100,100));
  color[16]=TColor(RGB(150,150,50));
  color[17]=TColor(RGB(50,50,150));

  if(option->quick_cluster)
  {
		analyze_all->Checked=true;
        get_ready();
        EuclideanTB->Position=100;
        StartClick(this);
  }
    String settingName[11];
  bool boolValue;
	for(int i=0;i<11;i++){
		settingName[i] =  "cls_feature";
	}
	char* foo = "";
	int value;
	for(int i=0;i<11;i++){
		itoa(i, foo, 10);
		settingName[i] += foo;
		getScreenSetting(settingName[i]);
		features->Checked[i]=cs.getFeature(i);
		includeIt[i]= boolValue;
		}


  includeIt[12]=false;

}
//---------------------------------------------------------------------------


void __fastcall TclusterIt::exitClick(TObject *Sender)
{
 for(int i=0;i<20000;i++) delete[] interval[i];
  delete[] interval;
 if(option->quick_cluster)option->quick_cluster=false;
 else StartForm->Visible=true;
}
//---------------------------------------------------------------------------

void __fastcall TclusterIt::StartClick(TObject *Sender)
{
	// this function retrieve data from the database
	SyllableDist.erase(SyllableDist.begin(), SyllableDist.end());  // SyllableDist is the vector that contain distances
	//SyllableDist.clear();
	tmpClust.erase(tmpClust.begin(), tmpClust.end());
    boundaries.erase(boundaries.begin(), boundaries.end());
    for(int i=0;i<11;i++)
    {
      if(first) { PreviousPitch[i]=0; PreviousFm[i]=0, PreviousDur[i]=0;  }
      if(features->Checked[i])includeIt[i]=true;
	  else includeIt[i]=false;
    }


    if(!GetData())
    {
      MessageDlgPos("Could not fine enough data -- try moving the 'time control' to an earlier interval", mtWarning, TMsgDlgButtons() << mbOK, 0, 100,140);
      return;
    }
    computeDistance();
    Iterations->Max=1+SyllableDist.size()/1000;
    Iterations->Position=1+SyllableDist.size()/1000;
    maxClust->Value=SyllableDist.size();
	cluster();
	restart->Enabled=true;
	repeat->Enabled=true;
	reset->Enabled=true;
	bool ck=false;
	if(warnings->Checked){warnings->Checked=false; ck=true;}
	ChosenClusterClick(this);
	warnings->Checked=ck;
}
//---------------------------------------------------------------------------

void TclusterIt::computeDistance()
{
  /* we are now computing the mxm matrix of the distance between each pair of data across all
  features. For example with interval of data 1:5000 we compute the distances between 5000x5000
  pairs of syllables across all the features considered. We are not going to save this matrix
  instead, we only save those results that passed a threshold. namely, we will only save distances
  that are relatively small and save a huge amount of memory...
  The structure for saving is a Vector called SyllableDist with the fields: For a pair of syllables that passed the
  threshold we document: [distance, x, y], where x is a pointer to the record number of syllable 1 in the
  database, and y is a pointer for the location of syllable 2 */

  float distance, x;
  int z, featureNum=0; for(int i=0;i<11;i++)if(includeIt[i])featureNum++; // we now know how many features to consider
  prog->Position=0;  prog->Max=intervalSize->Position; // we set the progress bar position
  int lim=intervalSize->Position;  // lim is the number of records to include
  if(analyze_all->Checked)lim=DataForm->recNum;
  if(lim>19000)
  {
    MessageDlg("Only the first 19,000 records will be clustered.", mtInformation, TMsgDlgButtons() << mbOK, 0);
    lim=19000;
  }
  const float treshold=TreshDist->Value, tresholdRaw=treshold*featureNum; // instead of dividing by feature numbers...

  // lets go...
  int cnt=0;
  for(int i=1;i<=lim;i++)
  {
      for(int j=i+1;j<=lim;j++)    // this i j construction gives us all possible pairs i,j with no redundency (the matrix is symetric)
      {
         distance=0; z=0;
         do{
           if(includeIt[z]){x=interval[i][z]-interval[j][z]; distance+=x*x;}//distance+=pow(interval[i][z]-interval[j][z],2);
           z++;
         }while(distance<tresholdRaw && z<11); // do while threshold not yet met!!!!
         distance/=featureNum;
         if(distance<treshold) copyToVector(distance, i,j);  // we have copied the distance to the vector SyllableDist
      }
      if(cnt==100){prog->StepIt(); cnt=0;}
      else cnt++;
  }
  ClearAllClick(this);
}


 //*******************************************************************************

void TclusterIt::cluster()    // this is the main fuction
{
  SYSTEMTIME* tm = new SYSTEMTIME;

  //ofstream myfile;
  //myfile.open ("log_cluster.txt");
  //if (!myfile) {
  //	return;
  //}
  //GetSystemTime(tm);
  //myfile << tm->wHour<<":"<<tm->wMinute<<":"<<tm->wSecond<<"  Entering cluster().\n";

  // declarations:
  bool diffClust;
  int syllable1_count, syllable2_count, progInd=0; // progInd is for the progress-bar

  AnsiString str;
  EuclideanDist record; // For each pair of syllables, record will hold a pair of indexes and a distance
  //clust.erase(clust.begin(), clust.end());
  clust.clear();
  masterCluster.clear();
  clustLocation.clear();
  //vector<int> clust; clust.reserve(20000); // clust will hold indexes of neighboring syllables, for example for the first pair of syllable 0 342 2 0 ...
  //list<int>::iterator location1 , location2, location3, location4;
  int syllableLocation, location1, location2;
   // initiations:
  int iter=min((int)maxClust->Value,(int)SyllableDist.size())-1;  // iter is the number of sorted pairs included: cannot be more than the size of the syllable-distance array
  prog2->Position=0; prog2->Max=1+iter/1000;
  //advance(tempIter, iter);
  //SyllableDist.erase(tempIter, SyllableDist.end());
  //actual->Value = SyllableDist.end()->distance;
  SyllableDist[iter].distance;
 // featureYClick(this);// this will plot the raw features to chart3

  //clust.push_back(0); // insert first boundary. zeros are the boundaries between clusters e.g., 0,x1,x2,...0

  // we first sort the Euclidean distance vector from small to large distances. The pairs of syllables that are of shorter distance are the nearest neighbors...
  // GetSystemTime(tm);
  //myfile << tm->wHour<<":"<<tm->wMinute<<":"<<tm->wSecond<<"  Starting sort.\n";

  sort(SyllableDist.begin(), SyllableDist.end());
  // GetSystemTime(tm);
  // myfile << tm->wHour<<":"<<tm->wMinute<<":"<<tm->wSecond<<"  Sorted.\n";

  //SyllableDist.sort();

  // main loop: this will join all syllables into cluster array


	for(int i=0;i<iter;i++)
  {
	  progInd++;
	  if(progInd>1000){progInd=0; prog2->StepIt();} // progress the progress-bar display
	  record=SyllableDist[i]; // obtain indexes and distance from Euclidian distaces vector

	  int syllable1=record.index1;  int syllable2=record.index2;
	  syllable1_count=clustLocation.count(syllable1);
	  //syllable1_count=count(clust.begin(), clust.end(), syllable1);
	  syllable2_count=clustLocation.count(syllable2);
	  //syllable2_count=count(clust.begin(), clust.end(), syllable2);
	  //current++;
	  //list<int>::iterator tmpLocation = clust.end();
  // case 1: both syllables are new (counts in clust are zero)
	  if(!syllable1_count && !syllable2_count)
	  {
		list<int> aList;
		aList.push_back(syllable1);
		aList.push_back(syllable2);
		masterCluster.push_back(aList);                                                                    // we inserted both to the begining

		//clustSet.insert(syllable1);
		//clustSet.insert(syllable2);
		syllableLocation = masterCluster.size()-1;

		clustLocation.insert(make_pair(syllable1, syllableLocation));
		clustLocation.insert(make_pair(syllable2, syllableLocation));
		//clust.push_back(0);

	  }

  // case 2: only syllable1 is new, therefore, we should insert it to clust next to syllable 2
	  else if(!syllable1_count && syllable2_count)
	  {
		 location2=(clustLocation.find(syllable2))->second;
		 //location2=find(clust.begin(), clust.end(), syllable2);  // we find the location of syllable 2
		 //clust.insert(location2, syllable1);  // and insert syllable 1 next to it
		 masterCluster[location2].push_back(syllable1);
		 //clustSet.insert(syllable1);
		 clustLocation.insert(make_pair(syllable1, location2));
	  }

  // case 3: only syllable2 is new, therefore, we should insert it to clust next to syllable 1
      else if(syllable1_count && !syllable2_count)
	  {
		 location1=(clustLocation.find(syllable1))->second;
		 //location1=find(clust.begin(), clust.end(), syllable1); // we find the location of syllable 1
		 //clust.insert(location1, syllable2);  // and insert syllable 2 next to it
		 masterCluster[location1].push_back(syllable2);
		 //clustSet.insert(syllable2);
		 clustLocation.insert(make_pair(syllable2, location1));
	  }

  // case 4: both syllables exist in clust -- but are they in the same cluster? If not, perhaps clusters should join?
     else
	 {
		location1=(clustLocation.find(syllable1))->second;
		location2=(clustLocation.find(syllable2))->second;
		//location1=find(clust.begin(), clust.end(), syllable1);  // location of syllable 1
		//location2=find(clust.begin(), clust.end(), syllable2);  // location of syllable 2
		diffClust=false;  // default assumption: syllables 1 and 2 are members of the same cluster.
		if (location1 != location2) {
			diffClust = true;
		}
		 /*
		if(location1<location2) // we now start a search with location3 iterator to check this assumption...
		{
		  location3=location1;
		  do{
			 location3++;
			 if(*location3==0) {diffClust=true; break;}  // the two syllables are in different clusters
		  }while(location3!=location2 && location3!=clust.end());
		}
		*/




		if(diffClust)// the two syllables belong to two different clusters -- that should be joined?
		{
		   /*
           do{--location2;}while(*location2!=0); // find the beginning of cluster 2;
           location3=location2; ++location3;   // we place location3 just in the begining of that cluster
		   */
		   int lc, ClustSize=0;
		   ClustSize = masterCluster[location2].size();
	 /* Now we have to decide if to join the two clusters, but we allow veto power to features
     the user can activate. For example, the user can decide to prevent joining clusters of
     a duration difference of more than 50ms even if their other features are similar  */

		   float avgDur1=0, avgPitch1=0, avgMeanFr1=0, avgEnt1=0, avgFM1=0, avgGoodness1=0;
		   /*
		   do{
				++location3;
				lc=*location3;
		   */
		  // myfile <<  "mastercluster size: "<<masterCluster.size()<<"\tlocation2: " <<location2 << "\tlocation1:  " << location1 <<"\tmastercluster[loc1] size:  "<<masterCluster[location1].size() <<"\tmastercluster[loc2] size:  "<< masterCluster[location2].size()<<"\n";
		   for (list<int>::iterator listIter=masterCluster[location2].begin();
		   listIter != masterCluster[location2].end(); listIter++) {
				lc=*listIter;
				if(lc)
				{
				  avgDur1+=interval[lc][0];
				  avgPitch1+=interval[lc][1];
				  avgEnt1+=interval[lc][2];
				  avgFM1+=interval[lc][3];
				  avgMeanFr1+=interval[lc][4];
				  avgGoodness1+=interval[lc][5];
				  /*ClustSize++;  */
				}
		   } /*while(*location3!=0); // reached the end of cluster 2;
		   */
		   avgDur1/=(float)ClustSize;
		   avgPitch1/=(float)ClustSize;
		   avgMeanFr1/=(float)ClustSize;
           avgEnt1/=(float)ClustSize;
           avgFM1/=(float)ClustSize;
           avgGoodness1/=(float)ClustSize;

      // we now repeat a similar procedure for the second cluster to find mean values:
		   /*location4=location1; */
		   ClustSize=masterCluster[location1].size();
           float avgDur2=0 , avgPitch2=0, avgMeanFr2=0, avgEnt2=0, avgFM2=0, avgGoodness2=0;
		   /*
		   do{--location4;}while(*location4!=0 && location4!=clust.begin()); // we found the beginning of cluster 1;
           do{
                ++location4;
				lc=*location4;
				*/
		   for (list<int>::iterator listIter=masterCluster[location1].begin();
			listIter != masterCluster[location1].end(); listIter++) {
				lc=*listIter;
                 if(lc)
                 {
				   avgDur2+=interval[lc][0];
				   avgPitch2+=interval[lc][1];
				   avgEnt2+=interval[lc][2];
				   avgFM2+=interval[lc][3];
				   avgMeanFr2+=interval[lc][4];
				   avgGoodness2+=interval[lc][5];
				   /*ClustSize++;     */
				 }
		   } /*while(*location2!=0); // find the end of cluster 2;
		   */
           avgDur2/=(float)ClustSize;
		   avgPitch2/=(float)ClustSize;
		   avgMeanFr2/=(float)ClustSize;
           avgEnt2/=(float)ClustSize;
           avgFM2/=(float)ClustSize;
           avgGoodness2/=(float)ClustSize;

    // All set! We can now dedice if to join those clusters:

          if(   fabs(avgDur1-avgDur2)<durThresh->Value
			 && fabs(avgPitch1-avgPitch2)<pitchThresh->Value
			 && fabs(avgMeanFr1-avgMeanFr2)<meanFrThresh->Value
             && fabs(avgEnt1-avgEnt2)<entropyThresh->Value
             && fabs(avgFM1-avgFM2)<fmThresh->Value
             && fabs(avgGoodness1-avgGoodness2)<GoodnessThresh->Value)
		  {
		  /*
           tmpClust.erase(tmpClust.begin(), tmpClust.end());
		   tmpClust.insert(tmpClust.begin(), location2, location3);  // this temporary storage now have the second cluster
		   clust.erase(location2,location3); // we delete this cluster (including one zero)
           clust.insert (location1, tmpClust.begin(), tmpClust.end()); // and joining it with the other cluster
		   clust.erase(location1); // get rid of this extra zero...
		   */
		   for (list<int>::iterator listIter=masterCluster[location2].begin();
			listIter != masterCluster[location2].end(); listIter++) {
				(*clustLocation.find(*listIter)).second = location1;
		   }
		   masterCluster[location1].splice(masterCluster[location1].begin(), masterCluster[location2]);

		  }
        }
     }// end elese
   } // end for
  //GetSystemTime(tm);
  //myfile << tm->wHour<<":"<<tm->wMinute<<":"<<tm->wSecond<<" Done with cluster analysis.\n";

  for (unsigned int i = 0; i < masterCluster.size(); i++) {
	  if (masterCluster[i].size() > 0) {
         clust.push_back(0);
		 for (list<int>::iterator listIter=masterCluster[i].begin();
			listIter != masterCluster[i].end(); listIter++) {
				clust.push_back(*listIter);
		 }
	  }
  }

 // we are all set, we can now present our data in graphs or search for clusters of certain featurs:

 boundary Boundary;
 boundIndex=1;
  Boundary.start=0; Boundary.end=0; Boundary.size=0;
  boundaries.push_back(Boundary);
   for(int i=1; i<(int)clust.size(); i++)
   {
	 if(clust[i]==0)
     {
       Boundary.start=boundaries[boundIndex-1].end;
       Boundary.end=i;
       Boundary.size=Boundary.end-Boundary.start; // negative ensure correct sorting...
       boundaries.push_back(Boundary);
       boundIndex++;
     }
   }
  //	GetSystemTime(tm);
  // myfile << tm->wHour<<":"<<tm->wMinute<<":"<<tm->wSecond<<"  Done with boundaries analysis.\n";

   // we now sort the clusters according to their populations (large to small)
   sort(boundaries.begin(), boundaries.end());
  //  GetSystemTime(tm);
  // myfile << tm->wHour<<":"<<tm->wMinute<<":"<<tm->wSecond<<"  Sorted boundaries.\n";

 plot_raw_data();
  //GetSystemTime(tm);
  // myfile << tm->wHour<<":"<<tm->wMinute<<":"<<tm->wSecond<<"  plot_raw_data() done.\n";

 Cluster_track(1);
 // GetSystemTime(tm);
 //  myfile << tm->wHour<<":"<<tm->wMinute<<":"<<tm->wSecond<<"  cluster_track() done.\n";

 // myfile.close();
}
//*******************************************************************************


void TclusterIt::Cluster_track(bool do_change)
{
 // first, to access the clusters it is convinient to have their indexes (start, end, duration) in a (small) vector, called boundaries.
  int index;
  float xVar, yVar;
  prog2->Position=0;
  //ofstream myfile;
  //myfile.open ("log_cluster_track.txt");
  //if (!myfile) {
  //	return;
  //}
  chart->ShowGraf(); // show the residuals graph
  TLabel *label;
  int included[20000], pos;
   float AvgPitch[16], AvgFm[16], AvgDur[16], AvgEnt[16], AvgGood[16], AvgMFR[16];
  for(int i=0;i<15;i++){AvgPitch[i]=0; AvgFm[i]=0; AvgDur[i]=0; AvgEnt[i]=0; AvgGood[i]=0; AvgMFR[i]=0;}
  // for(int i=0;i<10;i++)for(int j=0;j<10;j++)avgFeature[i][j]=0;
  // for(int i=0;i<10;i++)for(int j=0;j<10;j++)avgRefFeature[i][j]=0;
   float sims[16];
   for(int i=0;i<showClust->Position;i++)
   {
		int foo = boundIndex-1-i;
		if (foo<0) {
		  foo=0;
		}
	  int start=boundaries[foo].start;

	  int end=boundaries[foo].end;
	  pos=0;
	  for(int j=start; j<end; j++)
	  {
		index=clust[j];

		AvgDur[i]+=interval[index][0];
		AvgPitch[i]+=interval[index][1];
        AvgEnt[i]+=interval[index][2];
		AvgFm[i]+=interval[index][3];
        AvgMFR[i]+=interval[index][4];
        AvgGood[i]+=interval[index][5];
		pos++;
	  }
	  if(!pos)pos=1;
	  AvgDur[i]/=(float)pos; AvgPitch[i]/=(float)pos; AvgFm[i]/=(float)pos; AvgEnt[i]/=(float)pos; AvgGood[i]/=(float)pos; AvgMFR[i]/=(float)pos;
   }
   //GetSystemTime(tm);
  //myfile << tm->wHour<<":"<<tm->wMinute<<":"<<tm->wSecond<<"  after first loop.\n";



   if(first && do_change) for(int i=0;i<showClust->Position;i++)
   {
	  //if(features->Checked[i])avgRefFeature[index][i]=avgFeature[index][i];

	  PreviousDur[i]=AvgDur[i];
	  PreviousFm[i]=AvgFm[i];
	  PreviousPitch[i]=AvgPitch[i];
	  PreviousEnt[i]=AvgEnt[i];
	  PreviousGood[i]=AvgGood[i];
	  PreviousMFR[i]=AvgMFR[i];

   }

   else // we already have the refferences as RefDur,RefPitch, RefFM
   {
      int where;
	  float MinDiff=9999;
	 // GetSystemTime(tm);
//  myfile << tm->wHour<<":"<<tm->wMinute<<":"<<tm->wSecond<<"  b4 second loop.\n";

      for(int i=0;i<showClust->Position;i++)
      {
         /*  int f_count=0;
           sims[i]=0;
           for(int j=0;j<11;j++)
           {
             if(features->Checked[j])
			 {
                sims[i]+=avgFeature[j][]-avgRefFeature[j][];
                sims[i]*=sims[i];
                f_count++;
             }
           }
           sims[i]/=f_counts;
         */

			sims[i]=pow(AvgDur[i]   - RefDur,  2)
				   +pow(AvgFm[i]    - RefFM,   2)
                   +pow(AvgPitch[i] - RefPitch,2)
                   +pow(AvgEnt[i] - RefEnt,2)
                   +pow(AvgGood[i] - RefGood,2)
                   +pow(AvgMFR[i] - RefMFR,2)

                   ;

            if(sims[i]<MinDiff){ MinDiff=sims[i]; where=i; }
	  }

	  if(do_change)
	  {
	   ChosenOne=where;
       if(MinDiff>tolerance->Value) // 0.2
	   {
	     Beep();
         problems=true;
         repeat->Down=false;
         resetClick(this);
         StartClick(this);
         Application->MessageBox(L"Procedure failed: please try again: consider changing distance threshold, or changing the constraints",NULL,MB_OK);
	   }
       else// for(int i=0;i<10;i++)avgRefFeature[][i]=avgFeature[][i];

       {

		  RefDur=AvgDur[where];
		  RefPitch=AvgPitch[where];
          RefFM=AvgFm[where];
          RefEnt=AvgEnt[where];
          RefGood=AvgGood[where];
          RefMFR=AvgMFR[where];

       }

      //GetSystemTime(tm);
      // myfile << tm->wHour<<":"<<tm->wMinute<<":"<<tm->wSecond<<"  in else.\n";

	  }
   }



   int countClust[20]; for(int i=0;i<20;i++)countClust[i]=0;
   int SwitchIt;
  // GetSystemTime(tm);
  // myfile << tm->wHour<<":"<<tm->wMinute<<":"<<tm->wSecond<<"  b4 3rd loop.\n";

   for(int i=0;i<showClust->Position;i++)
   {
     int foo = boundIndex-1-i;
		if (foo<0) {
		  foo=0;
		}
	  int start=boundaries[foo].start;
	  int end=boundaries[foo].end;
	  if(analyze_all->Checked)SwitchIt=i;
	  else if(!first) { if(i==ChosenOne)SwitchIt=ChosenCluster->ItemIndex; else SwitchIt=15; }
	  else SwitchIt=i;

	  switch(SwitchIt)
	  {
		case 0: label=clust1; break;
		case 1: label=clust2; break;
		case 2: label=clust3; break;
		case 3: label=clust4; break;
		case 4: label=clust5; break;
		case 5: label=clust6; break;
		case 6: label=clust7; break;
		case 7: label=clust8; break;
		case 8: label=clust9; break;
		case 9: label=clust10; break;
		case 10: label=clust11; break;
		case 11: label=clust12; break;
		case 12: label=clust13; break;
		case 13: label=clust14; break;
		case 14: label=clust15; break;
		default: label=left_over; break;//label=left_over; break;
	  }
	  chart2->DataColor=color[SwitchIt];
	  for(int j=start+1; j<end; j++)
	  {
		countClust[SwitchIt]++;
		index=clust[j];
        included[pos]=index;
		pos++;
        //dur=interval[index][0];
		xVar=Find_Feature(index,0);
		yVar=Find_Feature(index,1);

		chart2->MarkAt(xVar,yVar,dot);

        if(analyze_all->Checked ||
                (  do_change && !first && !problems && i==ChosenOne && writePremit->Checked )
        )
        {
          DataForm->MyQuery1->RecNo=index+1;//interval[index][11];
		  int xx=DataForm->MyQuery1->FieldByName("cluster")->AsInteger;

          if(analyze_all->Checked) DataForm->ChangeTable(DataForm->TheTable->TableName, "cluster", i+bias, interval[index][11]);
          else if(xx<=0 || overwriteClusters->Checked)
          {
           DataForm->ChangeTable(DataForm->TheTable->TableName, "cluster", clusterName->Text, interval[index][11]);//DataForm->MyQuery1->RecNo);
         //  Memo3->Lines->Add(interval[index][11]);
          }
        } //end if !first


	  }// end for
      label->Caption=countClust[SwitchIt];
	  countClust[SwitchIt]=0;
   }
  //	GetSystemTime(tm);
  // myfile << tm->wHour<<":"<<tm->wMinute<<":"<<tm->wSecond<<"  after 3rd loop.\n";

   chart->DataColor=clBlue;
  // GetSystemTime(tm);
  // myfile << tm->wHour<<":"<<tm->wMinute<<":"<<tm->wSecond<<"  b4 4th loop.\n";

   for(int i=0;i<intervalSize->Position;i++)
   {
     int j=0;
     do{j++;}while(j<pos && i!=included[j]);
     if(j>pos-2)
     {
        xVar=Find_Feature(i,0);
        yVar=Find_Feature(i,1);
        chart->MarkAt(xVar,yVar,dot);
     }
   }
 //  GetSystemTime(tm);
  // myfile << tm->wHour<<":"<<tm->wMinute<<":"<<tm->wSecond<<"  after 4th loop.\n";

   counter->Caption=pos;
   vectorSize->Value=SyllableDist.size();
    prog->Position=0;
    chart->ShowGraf();chart2->ShowGraf();chart3->ShowGraf();
    redo->Enabled=true;
    residuals->Caption=(int)(100*(intervalSize->Position-counter->Caption.ToInt())/intervalSize->Position);
   // Beep();
   //GetSystemTime(tm);
  // myfile << tm->wHour<<":"<<tm->wMinute<<":"<<tm->wSecond<<"  the end of cluster_track.\n";
   // myfile.close();
}





void TclusterIt::copyToVector(float distance, int i, int j)
{
  EuclideanDist record;
  record.index1=i; record.index2=j; record.distance=distance;
  SyllableDist.push_back(record);
}

//*******************************************************************************

void __fastcall TclusterIt::FormClose(TObject *Sender, TCloseAction &Action)
{
  DataForm->MyQuery1->Close();
  //DataForm->MyCommand1->Close();
  //!!!!OFER
  //DataForm->MyConnection1->Close();
  exitClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TclusterIt::intervalSizeChange(TObject *Sender)
{
 insertScreenSetting("cls_intervalSize", intervalSize->Position);
 intervalN->Value=intervalSize->Position;
 Location->Min=intervalSize->Position;
 Iterations->Max=intervalSize->Position/10;
}
//---------------------------------------------------------------------------

void __fastcall TclusterIt::threshChange(TObject *Sender)
{
   //AnsiString str;
   durationTresh->Value=thresh->Position;
   insertScreenSetting("cls_mindur", thresh->Position);
  // str="duration>"; str+=thresh->Position;
  // DataForm->Records->Filter=str;
  // DataForm->Records->Filtered=true;
}
//---------------------------------------------------------------------------

void __fastcall TclusterIt::open_tableClick(TObject *Sender)
{
  DataForm->openTableClick(this);
  get_ready();
}
//---------------------------------------------------------------------------

void TclusterIt::get_ready()
{
  table_name->Caption=DataForm->TheTable->TableName;
  RecordsNum->Caption=DataForm->recNum;
  Location->Max=DataForm->recNum;//Records->RecordCount;
  development->Caption=(float)Location->Position*100/Location->Max;
  development->Caption=development->Caption.SetLength(5);
  Location->Min=intervalSize->Position;
  threshChange(this);
  Location->Max=DataForm->recNum-intervalSize->Position;//this is the total number of records in the database
  Location->Position=Location->Max;
  Start->Enabled=true;
  DataForm->Records->Active=false;
}

//---------------------------------------------------------------------------

void __fastcall TclusterIt::BitBtn5Click(TObject *Sender)
{
 chart->ClearGraf(); chart->ShowGraf();
}
//---------------------------------------------------------------------------

void __fastcall TclusterIt::clear_graphClick(TObject *Sender)
{
chart2->ClearGraf(); chart2->ShowGraf();

}
//---------------------------------------------------------------------------


void __fastcall TclusterIt::LocationChange(TObject *Sender)
{
 development->Caption=(float)Location->Position*100/Location->Max;
 development->Caption=development->Caption.SetLength(5);
 Location->Min=intervalSize->Position;
}
//---------------------------------------------------------------------------

void __fastcall TclusterIt::saveGraphClick(TObject *Sender)
{
   if(SaveDialog1->Execute()) chart->CopyToBMP(SaveDialog1->FileName.c_str(),0);
}
//---------------------------------------------------------------------------

void __fastcall TclusterIt::BitBtn6Click(TObject *Sender)
{
chart3->ClearGraf(); chart3->ShowGraf();
}
//---------------------------------------------------------------------------

void __fastcall TclusterIt::ClearAllClick(TObject *Sender)
{
  chart->ClearGraf(); chart->ShowGraf();
  chart2->ClearGraf(); chart2->ShowGraf();
  chart3->ClearGraf(); chart3->ShowGraf();
 // Memo1->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TclusterIt::IterationsChange(TObject *Sender)
{
  maxClust->Value=Iterations->Position*1000;
  insertScreenSetting("cls_iteration", Iterations->Position);
}
//---------------------------------------------------------------------------

void __fastcall TclusterIt::redoClick(TObject *Sender)
{
	//    SyllableDist.erase(SyllableDist.begin(), SyllableDist.end());
    ClearAllClick(this);
    problems=false;
    tmpClust.erase(tmpClust.begin(), tmpClust.end());
    boundaries.erase(boundaries.begin(), boundaries.end());
	cluster();

}
//---------------------------------------------------------------------------

void __fastcall TclusterIt::RadioGroup1Click(TObject *Sender)
{
  switch(RadioGroup1->ItemIndex)
  {
     case 0: intervalSize->Position=1000; break;
     case 1: intervalSize->Position=3000; break;
     case 2: intervalSize->Position=5000; break;
     case 3: intervalSize->Position=7500; break;
     case 4: intervalSize->Position=10000; break;
  }
  //insertScreenSetting("cls_interval", RadioGroup1->ItemIndex);

}
//---------------------------------------------------------------------------

void __fastcall TclusterIt::TrackBar1Change(TObject *Sender)
{
 durThresh->Value=(float)TrackBar1->Position/40.0;
 insertScreenSetting("cls_durdiff", TrackBar1->Position);
}
//---------------------------------------------------------------------------

void __fastcall TclusterIt::saveClustersClick(TObject *Sender)
{

   if(SaveDialog1->Execute())
   {
      wait->Visible=true;
      clusterIt->Refresh();
	 // filtered->TableName=SaveDialog1->FileName;
	  AnsiString str= "cluster="; str+=clusterName->Text;
	  DataForm->Records->Filter=str;
	  DataForm->Records->Filtered=true;
	 // filtered->EmptyTable();
	 // copyClust->Execute();
      wait->Visible=false;
   }   
}
//---------------------------------------------------------------------------

void __fastcall TclusterIt::save_graphClick(TObject *Sender)
{
 if(SaveDialog1->Execute()) chart2->CopyToBMP(SaveDialog1->FileName.c_str(),0);
}
//---------------------------------------------------------------------------

void __fastcall TclusterIt::restartClick(TObject *Sender)
{
 Start->Enabled=false;
 if(development->Caption.ToDouble() > 1 && !problems)
 {
        first=false;
        if(stayThere)stayThere=false;
        else Location->Position-=intervalSize->Position;
        StartClick(this);
 }
}
//---------------------------------------------------------------------------

void __fastcall TclusterIt::BitBtn10Click(TObject *Sender)
{
  wait->Visible=true;
  //clusterIt->Refresh();
  //for(int i=1; i<Location->Max; i++)
  str="update ";
  str+=DataForm->TheTable->TableName;
  str+=" set cluster=-2";
  DataForm->ChangeTable_freestyle(str);
  // DataForm->ChangeTable(DataForm->TheTable->TableName, "cluster", "-2", i);

  /*
  DataForm->Records->First();
  int cnt=DataForm->Records->RecordCount;
  for(int i=0;i<cnt;i++)
  {
   DataForm->Records->Edit();
   DataForm->Records->FieldByName("cluster")->AsInteger =-2;
   DataForm->Records->Post();
   DataForm->Records->Next();
  }      */
  wait->Visible=false;
}
//---------------------------------------------------------------------------

void __fastcall TclusterIt::resetClick(TObject *Sender)
{
  problems=false; first=true; restart->Enabled=false;  stayThere=true;
  Start->Enabled=true;
}
//---------------------------------------------------------------------------




void TclusterIt::createScreenSettingTable()
{
	 // if(table_exists(table_name)) return;
	  str="CREATE TABLE if  not exists `screensetting` ( ";
	  str += "`settingName` varchar(50) NOT NULL, ";
	  str += "`settingValue` varchar(50) DEFAULT NULL, ";
	  str += "PRIMARY KEY (`settingName`))";

	  MyCommand1->SQL->Clear();
	  MyCommand1->SQL->Add(str);
	  MyCommand1->Execute(1);
}

void TclusterIt::showTables()
 {
	MyQuery1->SQL->Clear();
	AnsiString str="SHOW TABLES;";
    MyQuery1->SQL->Add(str);
    MyQuery1->Execute();
	MyQuery1->First();
 }

bool TclusterIt::table_exists(AnsiString table_name)
{
  table_name=table_name.LowerCase();
  showTables();
  //DataForm->MyQuery1->First();
  MyQuery1->First();
  //for (int i=0; i < MyQuery1->RecordCount; i++)
  if (MyQuery1->RecordCount > 0) {
        do
		 {
			if (table_name==MyQuery1->Fields->Fields[0]->AsString)
			return(true);
		//MyQuery1->Next();
		 } while (MyQuery1->FindNext());
  }

  return(false);
}

bool TclusterIt::screenSettingExists(String settingName) {
	if(!table_exists("screensetting")){
		createScreenSettingTable();
		return false;
	}
   str="SELECT COUNT(*) FROM screensetting where settingName = '";
   str+=settingName;
   str+="';";
   MyQuery1->SQL->Clear();
   MyQuery1->SQL->Add(str);
   //try {
   MyQuery1->Execute();
   //} catch (...) {
		//createScreenSettingTable();
		//return false;
   //}


   int recNum=MyQuery1->Fields->Fields[0]->AsInteger;
   if (recNum) {
	 return true;
   } else {
	   return false;
   }
}
void TclusterIt::insertScreenSetting(String settingName, String settingValue)
{

	if (!screenSettingExists(settingName)) {
		str = "INSERT INTO screensetting ";
		str += "(settingname, settingvalue) values ('";
		str += settingName;
		str += "' , '";
		str += settingValue;
		str += "')";
	}  else {
		str = "UPDATE screensetting set settingvalue = '";
		str += settingValue;
		str += "' where settingName = '";
		str += settingName;
		str += "'";
    }


 MyCommand1->SQL->Clear();
 MyCommand1->SQL->Add(str);
 MyCommand1->Execute(1);
 //MyConnection1->Commit();
 MyCommand1->UnPrepare();


}

void TclusterIt::getScreenSetting(String settingName) {


   if (screenSettingExists(settingName)) {
	   str="SELECT settingValue FROM screensetting where settingName = '";
	   str+=settingName;
	   str+="';";
	   MyQuery1->SQL->Clear();
	   MyQuery1->SQL->Add(str);
	   MyQuery1->Execute();

	   if ( settingName != "cls_filter" ) {
			int value=MyQuery1->Fields->Fields[0]->AsInteger;
			if (settingName=="cls_chosenCluster") {
				cs.setChosenCluster(value);
			}  else if (settingName=="cls_dot") {
				cs.setDotSize(value);
			}   else if (settingName=="cls_intervalSize") {
				cs.setIntervalSize(value);
			}   else if (settingName=="cls_zoom") {
				cs.setZoom(value);
			}	else if (settingName=="cls_xaxis") {
				cs.setXAxis(value);
			}	else if (settingName=="cls_yaxis") {
				cs.setYAxis(value);
			}	else if (settingName=="cls_eudist") {
				cs.setEuDist(value);
			}	else if (settingName=="cls_showclust") {
				cs.setShowClust(value);
			}	else if (settingName=="cls_iteration") {
				cs.setIteration(value);
			}	else if (settingName=="cls_writepermit") {
				cs.setWritePermit(value);
			}	else if (settingName=="cls_overwrite") {
				cs.setOverwrite(value);
			}	else if (settingName=="cls_feature0") {
				cs.setFeature(0, value);
			}	else if (settingName=="cls_feature1") {
				cs.setFeature(1, value);
			}	else if (settingName=="cls_feature2") {
				cs.setFeature(2, value);
			}	else if (settingName=="cls_feature3") {
				cs.setFeature(3, value);
			}	else if (settingName=="cls_feature4") {
				cs.setFeature(4, value);
			}	else if (settingName=="cls_feature5") {
				cs.setFeature(5, value);
			} 	else if (settingName=="cls_feature6") {
				cs.setFeature(6, value);
			}	else if (settingName=="cls_feature7") {
				cs.setFeature(7, value);
			}	else if (settingName=="cls_feature8") {
				cs.setFeature(8, value);
			}	else if (settingName=="cls_feature9") {
				cs.setFeature(9, value);
			}	else if (settingName=="cls_feature10") {
				cs.setFeature(10, value);
			}	else if (settingName=="cls_mindur") {
				cs.setMinDur(value);
			}	else if (settingName=="cls_maxdur") {
				cs.setMaxDur(value);
			}	else if (settingName=="cls_matchtolerance") {
				cs.setMatchTolerance(value);
			}	else if (settingName=="cls_durdiff") {
				cs.setDurDiff(value);
			}	else if (settingName=="cls_pitchdiff") {
				cs.setPitchDiff(value);
			}	else if (settingName=="cls_fmdiff") {
				cs.setFmDiff(value);
			}	else if (settingName=="cls_entropydiff") {
				cs.setEntropyDiff(value);
			}   else if (settingName=="cls_clusterName") {
				cs.setChosenClusterName(value);
			}
			return;
		} else {
			String value=MyQuery1->Fields->Fields[0]->AsString;
			cs.setFilter(value);
		   return;
		}
   }
   return;
}

void TclusterIt::resetDefaultScreenSetting() {

	insertScreenSetting("cls_chosenCluster", 0);
	cs.setChosenCluster(0);
	ChosenCluster->ItemIndex = 0;

	insertScreenSetting("cls_dot", 3);
	cs.setDotSize(3);
	dot_size->ItemIndex = 3;

	insertScreenSetting("cls_interval", -1);
	cs.setInterval(-1);
	RadioGroup1->ItemIndex = -1;


	insertScreenSetting("cls_intervalSize", 3000);
	cs.setIntervalSize(3000);
	intervalSize->Position = 3000;

	insertScreenSetting("cls_clusterName", 1);
	cs.setChosenClusterName(1);
	clusterName->Text = 1;

	insertScreenSetting("cls_zoom", -1);
	cs.setZoom(-1);
	zoom1->ItemIndex = -1;


	insertScreenSetting("cls_xaxis", 0);
	cs.setXAxis(0);
	featureX->ItemIndex = 0;


	insertScreenSetting("cls_yaxis", 2);
	cs.setYAxis(2);
	featureY->ItemIndex = 2;


	insertScreenSetting("cls_eudist", 15);
	cs.setEuDist(15);
	EuclideanTB->Position = 15;



	insertScreenSetting("cls_showclust", 8);
	cs.setShowClust(8);
	showClust->Position = 8;


	insertScreenSetting("cls_iteration", 100);
	cs.setIteration(100);
	Iterations->Position = 100;


	insertScreenSetting("cls_writepermit", 1);
	cs.setWritePermit(1);
	writePremit->Checked = 1;



	insertScreenSetting("cls_overwrite", 0);
	cs.setOverwrite(0);
	overwriteClusters->Checked = 0;

	String settingName[11];
	for(int i=0;i<11;i++){
		settingName[i] =  "cls_feature";
	}
	char* foo = "";

	for(int i=0;i<6;i++){
		itoa(i, foo, 10);
		settingName[i] += foo;

		insertScreenSetting(settingName[i], 1);
		cs.setFeature(i, 1);
		features->Checked[i]=1;
	}

	for(int i=6;i<11;i++){
		itoa(i, foo, 10);
		settingName[i] += foo;

		insertScreenSetting(settingName[i], 0);
		cs.setFeature(i, 0);
		features->Checked[i]=0;
	}


	insertScreenSetting("cls_mindur", 25);
	cs.setMinDur(25);
	thresh->Position = 25;

	insertScreenSetting("cls_maxdur", 2000);
	cs.setMinDur(2000);
	TrackBar21->Position = 2000;


	insertScreenSetting("cls_matchtolerance", 2);
	cs.setMatchTolerance(2);
	toleranceTb->Position = 2;

	insertScreenSetting("cls_durdiff", 5);
	cs.setDurDiff(5);
	TrackBar1->Position = 5;

	insertScreenSetting("cls_pitchdiff", 30);
	cs.setPitchDiff(30);
	TrackBar18->Position = 30;

	insertScreenSetting("cls_entropydiff", 30);
	cs.setEntropyDiff(30);
	TrackBar20->Position = 30;

	insertScreenSetting("cls_fmdiff", 30);
	cs.setFmDiff(30);
	TrackBar19->Position = 30;

	insertScreenSetting("cls_filter", "cluster<>-2");
	cs.setFilter("cluster<>-2");
	quary->Text = "cluster<>-2";

}


void __fastcall TclusterIt::ChosenClusterClick(TObject *Sender)
{
  ChosenOne=ChosenCluster->ItemIndex;
  insertScreenSetting("cls_chosenCluster", ChosenOne);
  RefDur=PreviousDur[ChosenOne];
  RefPitch=PreviousPitch[ChosenOne];
  RefFM=PreviousFm[ChosenOne];
  RefEnt=PreviousEnt[ChosenOne];
  RefGood=PreviousGood[ChosenOne];
  RefMFR=PreviousMFR[ChosenOne];
  if(warnings->Checked){
	 clusterName->Text = InputBox("Cluster Identification", "Type cluster identification number (careful here!)","1");
	 clusterNameChange(this);
  }
  restart->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TclusterIt::backwardsClick(TObject *Sender)
{
Location->Position-=intervalSize->Position;
Stage->Caption=DataForm->MyQuery1->FieldValues["file_name"];
}
//---------------------------------------------------------------------------




void __fastcall TclusterIt::forwardClick(TObject *Sender)
{
Location->Position+=intervalSize->Position;
Stage->Caption=DataForm->MyQuery1->FieldValues["file_name"];      
}
//---------------------------------------------------------------------------


void __fastcall TclusterIt::move_backClick(TObject *Sender)
{
 Iterations->Position-=Iterations->Max/5;
         
}
//---------------------------------------------------------------------------

void __fastcall TclusterIt::all_data_includedClick(TObject *Sender)
{
 Iterations->Position=Iterations->Max;
}
//---------------------------------------------------------------------------
     

float TclusterIt::Find_Feature(int index, bool axis)
{
      float x;
	  TRadioGroup *feature_choise;
	  //String settingName;
	  if(axis==X_axis) {
		feature_choise=featureX;
		//settingName = "cls_xaxis";
	  } else {
		feature_choise=featureY;
		//settingName = "cls_yaxis";
		//insertScreenSetting("cls_yaxis", featureY->ItemIndex);
	  }
	  //insertScreenSetting(settingName, feature_choise->ItemIndex);
	  switch(feature_choise->ItemIndex)
      {
		case 0: x=interval[index][0]; break;
		case 1: x=interval[index][1]; break;
		case 2: x=interval[index][3]; break;
		case 3: x=interval[index][2]; break;
		case 4: x=interval[index][5]; break;
		case 5: x=interval[index][4]; break;
        case 6: x=interval[index][6]; break;
		case 7: x=interval[index][7]; break;
		case 8: x=interval[index][8]; break;
		case 9: x=interval[index][10]; break;
	  }
	  return(x);
}


void __fastcall TclusterIt::BitBtn14Click(TObject *Sender)
{
		if(SaveDialog1->Execute()) chart3->CopyToBMP(SaveDialog1->FileName.c_str(),0);
}
//---------------------------------------------------------------------------

void __fastcall TclusterIt::BitBtn15Click(TObject *Sender)
{
	  AnsiString str=quary->Text;
	  DataForm->Records->Filter=str;
	  DataForm->Records->Filtered=true;
}
//---------------------------------------------------------------------------

void __fastcall TclusterIt::noFilterClick(TObject *Sender)
{
	DataForm->Records->Filtered=false;
}
//---------------------------------------------------------------------------


void __fastcall TclusterIt::sizeClick(TObject *Sender)
{
  if(size->ItemIndex){chart3->Left=35; chart3->Top=35; chart3->Height=370; chart3->Width=550;}
  else {chart3->Left=208; chart3->Top=104; chart3->Height=300; chart3->Width=400;}
}
//---------------------------------------------------------------------------




void __fastcall TclusterIt::TrackBar18Change(TObject *Sender)
{
  pitchThresh->Value=(float)TrackBar18->Position/10.0;
  insertScreenSetting("cls_pitchdiff", TrackBar18->Position);
}
//---------------------------------------------------------------------------

void __fastcall TclusterIt::TrackBar19Change(TObject *Sender)
{
fmThresh->Value=(float)TrackBar19->Position/10.0;
insertScreenSetting("cls_fmdiff", TrackBar19->Position);
}
//---------------------------------------------------------------------------

void __fastcall TclusterIt::TrackBar20Change(TObject *Sender)
{
entropyThresh->Value=(float)TrackBar20->Position/40.0;
insertScreenSetting("cls_entropydiff", TrackBar20->Position);
}
//---------------------------------------------------------------------------

void __fastcall TclusterIt::TrackBar21Change(TObject *Sender)
{
  maxDurationThresh->Value=TrackBar21->Position;
  insertScreenSetting("cls_maxdur", TrackBar21->Position);
}
//---------------------------------------------------------------------------



void __fastcall TclusterIt::zoomClick(TObject *Sender)
{
 switch(zoom1->ItemIndex)
 {
	 case 0:chart->MouseAction=maZoomDrag; chart2->MouseAction=maZoomDrag; chart3->MouseAction=maZoomDrag; break;
	 case 1:chart->MouseAction=maPan;  chart2->MouseAction=maPan; chart3->MouseAction=maPan; break;
	 case 2:chart->MouseAction=maPan;  chart2->MouseAction=maZoomWind; chart3->MouseAction=maZoomWind; break;
 }
   insertScreenSetting("cls_zoom", zoom1->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall TclusterIt::zoom1Click(TObject *Sender)
{
switch(zoom1->ItemIndex)
 {
	 case 0:chart3->MouseAction=maZoomDrag; break;
	 case 1:chart3->MouseAction=maPan; break;
	 case 2:chart3->MouseAction=maZoomWind; break;
 }
}
//---------------------------------------------------------------------------



void __fastcall TclusterIt::FormCreate(TObject *Sender)
{
   Application->OnIdle = MyIdleHandler;
}
//---------------------------------------------------------------------------

void __fastcall TclusterIt::MyIdleHandler(TObject *Sender, bool &Done)
{
	if(development->Caption.ToDouble() > 1 && !problems && repeat->Down)
	{
		first=false;
		if(stayThere)stayThere=false;
		else Location->Position-=intervalSize->Position;
		StartClick(this);
	}

	else if(DVD_play) Timer1->Enabled=true;

}


void __fastcall TclusterIt::featureYClick(TObject *Sender)
{
  insertScreenSetting("cls_xaxis", featureX->ItemIndex);
  insertScreenSetting("cls_yaxis", featureY->ItemIndex);
  plot_raw_data();
  Cluster_track(0);
}
//---------------------------------------------------------------------------




float  TclusterIt::plot_raw_data()
{
  float X_feature, Y_feature, goo;
  ClearAllClick(this);
  boundaries.erase(boundaries.begin(), boundaries.end());
  chart3->DataColor=clRed;
  for(int i=1;i<intervalSize->Position;i++)
  {
	X_feature=Find_Feature(i,0);
	Y_feature=Find_Feature(i,1);
	// if DVD play we shall use colors based on cluster ID
    if(DVD_play)
    {
      short clust=interval[i][9];//DataForm->MyQuery1->FieldValues["cluster"];
      if(clust>0)chart3->DataColor=color[clust-1];
      else chart3->DataColor=clSilver;
    }
    chart3->MarkAt(X_feature, Y_feature,dot);
    goo=Y_feature;
  }
  chart3->ShowGraf();
  return(goo);
}




void __fastcall TclusterIt::BitBtn2Click(TObject *Sender)
{
  option->Visible=true;
  option->BringToFront();
  option->show_start=false;        
}
//---------------------------------------------------------------------------


bool TclusterIt::GetData()
{
    int i=0; float dur;
    if(analyze_all->Checked)DataForm->Fetch_data(0, 1, 0, 10000, min(DataForm->recNum,100000));
    else
    {
        DataForm->Fetch_data(0, Location->Position, thresh->Position, maxDurationThresh->Value, intervalSize->Position);
		if(DataForm->MyQuery1->RecordCount<intervalSize->Position/1.3) return(false);
    }   
    Stage->Caption=DataForm->MyQuery1->FieldValues["file_name"];
    do{

       dur=DataForm->MyQuery1->FieldValues["duration"];
       if(includeIt[0])interval[i][0]=(dur-option->medianDuration->Value)/option->deviationDuration->Value;
       if(includeIt[1])interval[i][1]=((float)DataForm->MyQuery1->FieldValues["mean_pitch"]-option->medianPitch->Value)/option->deviationPitch->Value;
       if(includeIt[2])interval[i][2]=((float)DataForm->MyQuery1->FieldValues["mean_entropy"]-option->medianEntropy->Value)/option->deviationEntropy->Value;
       if(includeIt[3])interval[i][3]=((float)DataForm->MyQuery1->FieldValues["mean_FM"]-option->medianFM->Value)/option->deviationFM->Value;
       if(includeIt[5])interval[i][4]=((float)DataForm->MyQuery1->FieldValues["mean_mean_frequency"]-option->medianPeakFr->Value)/option->deviationPeakFr->Value;
       if(includeIt[4])interval[i][5]=((float)DataForm->MyQuery1->FieldValues["mean_goodness_of_pitch"]-option->medianPgood->Value)/option->deviationPgood->Value;
       if(includeIt[6])interval[i][6]=((float)DataForm->MyQuery1->FieldValues["var_pitch"]-option->medianVarPitch->Value)/option->deviationVarPitch->Value;
       if(includeIt[7])interval[i][7]=((float)DataForm->MyQuery1->FieldValues["var_FM"]-option->medianVarFM->Value)/option->deviationVarFM->Value;
       if(includeIt[8])interval[i][8]=((float)DataForm->MyQuery1->FieldValues["var_entropy"]-option->medianVarEnt->Value)/option->deviationVarEnt->Value;
       if(includeIt[10])interval[i][10]=((float)DataForm->MyQuery1->FieldValues["var_goodness_of_pitch"]-option->medianVarPGood->Value)/option->deviationVarPGood->Value;
       interval[i][9]=DataForm->MyQuery1->FieldValues["cluster"];
       interval[i][11]=DataForm->MyQuery1->FieldValues["recnum"];//DataForm->MyQuery1->RecNo;
       i++;
       DataForm->MyQuery1->Next();

    }while(i<intervalSize->Position);// j is an emergency break...

	if(showData->Checked)for(i=0;i<intervalSize->Position;i++)
    {
       str="";
       for(int j=0;j<12;j++){str+=interval[i][j]; str+=" , "; }
       Memo1->Lines->Add(str);
    }

     toStage->Caption=DataForm->MyQuery1->FieldValues["file_name"];
     //DataForm->MyQuery1->UnPrepare();
     return(true);
}



void __fastcall TclusterIt::movieClick(TObject *Sender)
{
  if(DVD_play)DVD_play=false;
  else DVD_play=true;
}
//---------------------------------------------------------------------------


void __fastcall TclusterIt::Timer1Timer(TObject *Sender)
{

   try{   ////  handler bird109 read next
    if(Location->Position<Location->Max-intervalSize->Position)Location->Position+=slide->Position;
    else Location->Position=1;
    GetData();
    plot_raw_data();
    Timer1->Enabled=false;
    }catch(...)
       {
           Timer1->Enabled=false;
           MessageDlg("Could not read data!", mtWarning, TMsgDlgButtons() << mbOK, 0);
       }
}
//---------------------------------------------------------------------------



void __fastcall TclusterIt::Button2Click(TObject *Sender)
{
    GetData();
    plot_raw_data();
}
//---------------------------------------------------------------------------






void __fastcall TclusterIt::showClustChange(TObject *Sender)
{
  clust_num->Caption=showClust->Position;
  insertScreenSetting("cls_showclust", showClust->Position);
 
}
//---------------------------------------------------------------------------

void __fastcall TclusterIt::EuclideanTBChange(TObject *Sender)
{
   TreshDist->Value= (float)EuclideanTB->Position/1000;  // plot_raw_data();
   insertScreenSetting("cls_eudist", EuclideanTB->Position);
 // Cluster_track(0);
}
//---------------------------------------------------------------------------

void __fastcall TclusterIt::help1Click(TObject *Sender)
{
  	MessageDlgPos("Euclidean distance is the square root of square differences across all features (i.e., sqrt(pitch1-pitch2)^2 and so forth). We first scale the features based on their mean values and their variance. Then, we examine the distance between every two sounds across all the scaled features included. Finally, we sort them from large to small distances and only cluster those pairs that are below the distance selected. The distance is unit less, and should be judged against the mean scaled values, which is unity.  ", mtInformation, TMsgDlgButtons() << mbOK, 0, 100, 50);
}
//---------------------------------------------------------------------------

void __fastcall TclusterIt::help7Click(TObject *Sender)
{
   MessageDlgPos("At the first round, Sound Analysis recognizes the cluster you selected and labels all the member syllables with the ID number you chose. Then, it moves one step back (e.g., 3000 syllables), perform cluster analysis again, and look for the cluster that is most similar to the one you selected. This procedure is then repeated recursively. Note that this procedure might fail and visual inspection of the process is warranted. See user manual for further instructions.  ", mtInformation, TMsgDlgButtons() << mbOK, 0, 350, 50);

}
//---------------------------------------------------------------------------


void __fastcall TclusterIt::help4Click(TObject *Sender)
{
  MessageDlgPos("If the clustering procedure had merged two clusters you may attempt to temporarily incease the threshold (without clustering all over again). Move the slider to the left and click 'Restart'. See additional means in the 'constraints' tab", mtInformation, TMsgDlgButtons() << mbOK, 0, 450, 50);

}
//---------------------------------------------------------------------------

void __fastcall TclusterIt::help5Click(TObject *Sender)
{
   MessageDlgPos("The circles below will allow you to identify the appropriate cluster by its color. Once you selected a legend you must identify the cluster by typing an ID number in the edit box to the left. Then you may click the trace-bakc button. The numbers to the right of the legend represent the number of members (syllable) of each type", mtInformation, TMsgDlgButtons() << mbOK, 0, 100,140);

}
//---------------------------------------------------------------------------


void __fastcall TclusterIt::help6Click(TObject *Sender)
{
    MessageDlgPos("The track-bar below can be used to navigate throughout the syllable table: slide it all the way to the left to see clusters at the begining of the table, and all the was to the right to see clusters at the end", mtInformation, TMsgDlgButtons() << mbOK, 0, 400,500);

}
//---------------------------------------------------------------------------




void __fastcall TclusterIt::speedChange(TObject *Sender)
{
Timer1->Interval=speed->Position;        
}
//---------------------------------------------------------------------------


void __fastcall TclusterIt::select_quarryClick(TObject *Sender)
{
  str=select_it->Text;   // this is the querry you typed, and will be executed by querry1
  DataForm->Select_data(str);
  recs1->Value=DataForm->MyQuery1->RecordCount;
}
//---------------------------------------------------------------------------

void __fastcall TclusterIt::cluster_itClick(TObject *Sender)
{

    DataForm->MyQuery1->First();
    for(int i=1;i<=DataForm->MyQuery1->RecordCount;i++)
    {
        done1->Value=i;
        clusterIt->Refresh();
        int num=DataForm->MyQuery1->FieldValues["serial_number"];
        str="UPDATE ";
        str+=DataForm->TheTable->TableName;
        str+=" SET cluster=";
        str+=cluster_name->Text;
        str+=" WHERE serial_number=";
        str+=num;
        DataForm->MyCommand1->SQL->Clear();
        DataForm->MyCommand1->SQL->Add(str);
        DataForm->MyCommand1->Execute(1);
        DataForm->MyQuery1->Next();

   }
}
//---------------------------------------------------------------------------

void __fastcall TclusterIt::BitBtn3Click(TObject *Sender)
{
  str=select_it->Text;   // this is the querry you typed, and will be executed by querry1
  DataForm->Select_data_freeStyle(str);
   recs1->Value=DataForm->MyQuery1->RecordCount;
}
//---------------------------------------------------------------------------

void __fastcall TclusterIt::BitBtn8Click(TObject *Sender)
{
    Screen->Cursor = crHourGlass;
    DataForm->MyQuery1->First();
    for(int i=1;i<=DataForm->MyQuery1->RecordCount;i++)
    {
        done1->Value=i;
        clusterIt->Refresh();
        int num=DataForm->MyQuery1->FieldValues["recnum"];
        str="UPDATE ";
        str+=DataForm->TheTable->TableName;
        str+=" SET cluster=";
        str+=cluster_name->Text;
        str+=" WHERE serial_number=";
        str+=num;
        DataForm->MyCommand1->SQL->Clear();
        DataForm->MyCommand1->SQL->Add(str);
        DataForm->MyCommand1->Execute(1);
        DataForm->MyQuery1->Next();

   }
   Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TclusterIt::toleranceTbChange(TObject *Sender)
{
 tolerance->Value=(float)toleranceTb->Position/40.0;
 insertScreenSetting("cls_matchtolerance", toleranceTb->Position);
}
//---------------------------------------------------------------------------




void __fastcall TclusterIt::TrackBar2Change(TObject *Sender)
{
  GoodnessThresh->Value=TrackBar2->Position/10.0;
}
//---------------------------------------------------------------------------

void __fastcall TclusterIt::BitBtn16Click(TObject *Sender)
{
  str="If checked, SAP2 will analyze and cluster the entire table in a single shot. This option is not useful for traking clusters";
  MessageDlgPos(str, mtInformation, TMsgDlgButtons() << mbOK, 0, 100, 50);
}
//---------------------------------------------------------------------------

void __fastcall TclusterIt::analyze_allClick(TObject *Sender)
{
   if(analyze_all->Checked) {bias=1; dot_size->ItemIndex=1; dot_sizeClick(this);}
   else {bias=0; dot_size->ItemIndex=3; dot_sizeClick(this);}
}
//---------------------------------------------------------------------------

void __fastcall TclusterIt::dot_sizeClick(TObject *Sender)
{
  switch(dot_size->ItemIndex)
  {
    case 0: dot= 26; break;
    case 1: dot= 14; break;
	case 2: dot= 1; break;
	case 3: dot= 0; break;


  }
	insertScreenSetting("cls_dot", dot_size->ItemIndex);
}
//---------------------------------------------------------------------------






void __fastcall TclusterIt::writePremitClick(TObject *Sender)
{
	int value;
	if (writePremit->Checked)
	{
		value = 1;
	} else {
		value = 0;
	}
	insertScreenSetting("cls_writepermit", value);
}
//---------------------------------------------------------------------------

void __fastcall TclusterIt::overwriteClustersClick(TObject *Sender)
{
   int value;
	if (overwriteClusters->Checked)
	{
		value = 1;
	} else {
		value = 0;
	}
	insertScreenSetting("cls_overwrite", value);
}
//---------------------------------------------------------------------------
 void __fastcall TclusterIt::clusterNameChange(TObject *Sender)
{
	AnsiString name=  "cls_clusterName";
	//String name =  "cls_clusterName";
	 insertScreenSetting(name, clusterName->Text);
}
//---------------------------------------------------------------------------


void __fastcall TclusterIt::clusterNameFooChange(TObject *Sender)
{
	AnsiString name=  "cls_clusterName";
	//String name =  "cls_clusterName";
	 insertScreenSetting(name, clusterName->Text);
}

void __fastcall TclusterIt::featuresClick(TObject *Sender)
{
	String settingName[11];

	for(int i=0;i<11;i++){
		settingName[i] =  "cls_feature";
	}
	char* foo = "";
	int value;
	for(int i=0;i<11;i++){
		itoa(i, foo, 10);
		settingName[i] += foo;
		if (features->Checked[i]) {
			value = 1;
		} else {
			 value = 0;
		}
		insertScreenSetting(settingName[i], value);
		}
}
//---------------------------------------------------------------------------






void __fastcall TclusterIt::quaryChange(TObject *Sender)
{
	String name =  "cls_filter";
	 insertScreenSetting(name, quary->Text);
}

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

void __fastcall TclusterIt::BitBtn1Click(TObject *Sender)
{
	resetDefaultScreenSetting();
}


//---------------------------------------------------------------------------


void __fastcall TclusterIt::TrackBar3Change(TObject *Sender)
{
  meanFrThresh->Value=(float)TrackBar3->Position/10.0;
}
//---------------------------------------------------------------------------


void __fastcall TclusterIt::FormShow(TObject *Sender)
{
  warnings->Checked=true;
  restart->Enabled=false;
}
//---------------------------------------------------------------------------


void __fastcall TclusterIt::LinkLabel1LinkClick(TObject *Sender, const UnicodeString Link,
          TSysLinkType LinkType)
{
  AnsiString link=Link;
  ShellExecute(0, "open", link.c_str(), 0, 0, 1);
}
//---------------------------------------------------------------------------

