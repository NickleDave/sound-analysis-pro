//---------------------------------------------------------------------------

#ifndef ClusterScreenH
#define ClusterScreenH
//---------------------------------------------------------------------------
#include <Classes.hpp>
class ClusterScreen  {
private:
	  int interval;
	  int intervalSize;
	  int chosenCluster;
	  int dotSize;
	  int zoom;
	  int xAxis;
	  int yAxis;
	  int euDist;
	  int showClust;
	  int iteration;
	  int writePermit;
	  int overwrite;
	  int feature[11];
	  int minDur;
	  int maxDur;
	  int matchTolerance;
	  int durDiff;
	  int pitchDiff;
	  int fmDiff;
	  int entropyDiff;
	  int chosenClusterName;
	  AnsiString filter;

public:
	ClusterScreen();
	~ClusterScreen() {}
	int getInterval();
	void setInterval(int value);
	int getIntervalSize();
	void setIntervalSize(int value);
	int getChosenCluster();
	void setChosenCluster(int value);
	int getChosenClusterName();
	void setChosenClusterName(int value);
	int getDotSize();
	void setDotSize(int value);
	int getZoom();
	void setZoom(int value);
	int getXAxis();
	void setXAxis(int value);
	int getYAxis();
	void setYAxis(int value);
	int getEuDist();
	void setEuDist(int value);
	int getShowClust();
	void setShowClust(int value);
	int getIteration();
	void setIteration(int value);
	int getWritePermit();
	void setWritePermit(int value);
	int getOverwrite();
	void setOverwrite(int value);
	int getFeature(int index);
	void setFeature(int index, int value);

	int  getMinDur();
	void setMinDur(int value);
	int  getMaxDur();
	void setMaxDur(int value);
	int  getMatchTolerance();
	void setMatchTolerance(int value);
	int  getDurDiff();
	void setDurDiff(int value);
	int  getPitchDiff();
	void setPitchDiff(int value);
	int  getFmDiff();
	void setFmDiff(int value);
	int  getEntropyDiff();
	void setEntropyDiff(int value);
	AnsiString getFilter();
	void setFilter(AnsiString value);


};
#endif

