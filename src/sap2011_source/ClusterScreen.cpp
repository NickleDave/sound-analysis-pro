//---------------------------------------------------------------------------


#pragma hdrstop

#include "ClusterScreen.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

	ClusterScreen::ClusterScreen() {
		interval = -1;
		intervalSize = -1;
		chosenCluster = -1;
		dotSize = -1;
		zoom = -1;
		xAxis = -1;
		yAxis = -1;
		euDist = -1;
		showClust = -1;
		iteration = -1;
		writePermit = -1;
		overwrite = -1;
		minDur = -1;
		maxDur = -1;
		matchTolerance = -1;
		durDiff = -1;
		pitchDiff = -1;
		fmDiff = -1;
		entropyDiff = -1;
		for (int i = 0; i < 11; i++) {
			feature[i] = -1;
        }
		chosenClusterName = -1;
		filter = "";
	}

	int ClusterScreen::getInterval() {
		if (interval > -1) {
			return interval;
		} else {
			return -1;
		}
    }
	void ClusterScreen::setInterval(int value) {
		interval = value;
	}
	int ClusterScreen::getIntervalSize() {
		if (intervalSize > -1) {
			return intervalSize;
		} else {
			return 3000;
		}
    }
	void ClusterScreen::setIntervalSize(int value) {
		intervalSize = value;
	}
	int ClusterScreen::getChosenCluster() {
		if (chosenCluster > -1) {
			return chosenCluster;
		} else {
			return 0;
		}
	}
	void ClusterScreen::setChosenCluster(int value) {
		chosenCluster = value;
	}
	int ClusterScreen::getChosenClusterName() {
		if (chosenClusterName > -1) {
			return chosenClusterName;
		} else {
			return 1;
		}
	}
	void ClusterScreen::setChosenClusterName(int value) {
		chosenClusterName = value;
	}
	int ClusterScreen::getDotSize() {
		if (dotSize > -1) {
			return dotSize;
		} else {
			return 3;
		}
    }
	void ClusterScreen::setDotSize(int value) {
		dotSize = value;
    }
	int ClusterScreen::getZoom() {
		if (zoom) {
			return zoom;
		} else {
			return 0;
		}
    }
	void ClusterScreen::setZoom(int value) {
		zoom = value;
    }
	int ClusterScreen::getXAxis() {
		if (xAxis > -1) {
			return xAxis;
		} else {
			return 0;
		}
    }
	void ClusterScreen::setXAxis(int value) {
		xAxis = value;
    }
	int ClusterScreen::getYAxis() {
		if (yAxis > -1) {
			return yAxis;
		} else {
			return 2;
		}
    }
	void ClusterScreen::setYAxis(int value) {
		yAxis = value;
    }
	int ClusterScreen::getEuDist() {
		if (euDist > -1) {
			return euDist;
		} else {
			return 15;
		}
	}
	void ClusterScreen::setEuDist(int value) {
		euDist = value;
	}
	int ClusterScreen::getShowClust() {
		if (showClust > -1) {
			return showClust;
		} else {
			return 8;
		}
	}
	void ClusterScreen::setShowClust(int value) {
		showClust = value;
    }
	int ClusterScreen::getIteration() {
		if (iteration > -1) {
			return iteration;
		} else {
			return 100;
		}
    }
	void ClusterScreen::setIteration(int value) {
		iteration = value;
    }
	int ClusterScreen::getWritePermit() {
		if (writePermit > -1) {
			return writePermit;
		} else {
			return 1;
		}
	}
	void ClusterScreen::setWritePermit(int value) {
		writePermit = value;
	}
	int ClusterScreen::getOverwrite() {
		if (overwrite > -1) {
			return overwrite;
		} else {
			return 0;
		}
	}
	void ClusterScreen::setOverwrite(int value){
		overwrite = value;
	}

	int ClusterScreen::getFeature(int index) {
		if (feature[index] > -1)  {
			return feature[index];
		} else {
			if (index < 6) {
				return 1;
			} else {
				return 0;
            }

        }
	}
	void ClusterScreen::setFeature(int index, int value) {
		feature[index] = value;
	}

	int  ClusterScreen::getMinDur()   {
		if (minDur > -1) {
			return minDur;
		} else {
			return 25;
		}
	}
	void ClusterScreen::setMinDur(int value){
		minDur = value;
	}
	int  ClusterScreen::getMaxDur() {
		if (maxDur > -1) {
			return maxDur;
		} else {
			return 2000;
		}
	}
	void ClusterScreen::setMaxDur(int value) {
		maxDur = value;
	}
	int  ClusterScreen::getMatchTolerance() {
		if (matchTolerance > -1) {
			return matchTolerance;
		} else {
			return 2;
		}
	}
	void ClusterScreen::setMatchTolerance(int value) {
		matchTolerance = value;
	}
	int  ClusterScreen::getDurDiff() {
		if (durDiff > -1) {
			return durDiff;
		} else {
			return 5;
		}
	}
	void ClusterScreen::setDurDiff(int value) {
		durDiff = value;
	}
	int  ClusterScreen::getPitchDiff() {
		if (pitchDiff > -1) {
			return pitchDiff;
		} else {
			return 30;
		}
	}
	void ClusterScreen::setPitchDiff(int value) {
		pitchDiff = value;
	}
	int  ClusterScreen::getFmDiff() {
		if (fmDiff > -1) {
			return fmDiff;
		} else {
			return 30;
		}
	}
	void ClusterScreen::setFmDiff(int value) {
		fmDiff = value;
	}
	int  ClusterScreen::getEntropyDiff() {
		if (entropyDiff > -1) {
			return entropyDiff;
		} else {
			return 30;
		}
	}
	void ClusterScreen::setEntropyDiff(int value) {
		entropyDiff = value;
	}
	AnsiString ClusterScreen::getFilter() {
		if (filter != "") {
			return filter;
		} else {
			return "cluster<>-2";
		}
	}
	void ClusterScreen::setFilter(AnsiString value) {
		filter = value;
	}
