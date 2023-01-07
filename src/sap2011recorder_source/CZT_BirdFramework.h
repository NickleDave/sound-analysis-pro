// CZT_BirdFramework.h: interface for the CZT_BirdFramework class.
/*
 *    Copyright (C) 2004 CCUNY
 *    Written by David Swigger (dswigger@yahoo.com)                    
 *
 *	Permission is granted to anyone to use this software for any
 *	non-commercial use - subject to the following restrictions:
 *
 *	1. The author is not responsible for the consequences of use of
 *		this software, no matter how awful, even if they arise
 *		from defects in it.
 *
 *	2. The origin of this software must not be misrepresented, either
 *		by explicit claim or by omission.
 *
 *	3. Altered versions must be plainly marked as such, and must not
 *		be misrepresented as being the original software.
 *
 */
// Central place for enums, descriptions and so forth..
// I am namespacing it to make things really simple to deal with....
/*
	Updating Version:
	In OCX code version updated in following files:
	ztBirdControlPack.odl
	ztBirdControlPack.cpp (_wVerMajor,_wVerMinor)
	resource
*/
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CZT_BIRDFRAMEWORK_H__AB54B686_00F1_4AC6_A2C0_4340E5473FC8__INCLUDED_)
#define AFX_CZT_BIRDFRAMEWORK_H__AB54B686_00F1_4AC6_A2C0_4340E5473FC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef THISWILLNEVERBEDEFINEDANDISJUSTAPLACEHOLDERCLASS
class CZT_BirdFramework  
{
public:
	CZT_BirdFramework();
	virtual ~CZT_BirdFramework();
};
#endif

// the bird namespace 
namespace ztBirdEngine
{
	// Engine event ID's and descriptors
	//////////////////////////////////////////////////////////////////////
	enum ztEngineEventIDS
	{
		ztee_Error_NoValidBirds = 0,
		ztee_Error_NoValidAudioDrivers,
		ztee_Warning_BirdNotUsed,
		ztee_Status_EngineStarted,
		ztee_Status_EngineStopped,
		ztee_Error_EngineNotStarted,
		ztee_Error_NoTempFilePathSet,
		ztee_Error_BirdDspChainInvalid,
		ztee_Error_BirdIODeviceInvalid,
		ztee_Total = 9
	};

	//Descriptors
	static char* zt_EngineEvent_Description[ztee_Total] =
	{
		"(stopping initialization) There are not any active or valid birds.",
		"(stopping initialization) No valid audio drivers selected.",
		"A Bird does not have a valid audio driver and will be de-activated.",
		"The engine has been successfully started.",
		"The engine has been stopped.",
		"The engine was not started due to errors on initialization.",
		"A Bird does not have a valid temp file path and will be de-activated.",
		"A dsp chain could not be built for a specific bird",
		"A birds io device could not be instanced."
	};

	// Bird event ID's and descriptors
	//////////////////////////////////////////////////////////////////////
	enum ztBirdEventIDS
	{
		ztbe_Error_DriverInvalid = 0,
		ztbe_Status_Activated,
		ztbe_PeakRecordingStart,
		ztbe_PeakRecordingStop,
		ztbe_Error_NoTempFilePath,
		ztbe_PeakCounterTrigger,
		ztbe_PlaySoundCompleted,
		ztbe_ClippingDetected,
		ztbe_Total = 8
	};

	//Descriptors
	//////////////////////////////////////////////////////////////////////
	static char* zt_ztBirdEvent_Description[ztbe_Total] =
	{
		"The birds audio driver is invalid.  The bird has been de-activated.",
		"The bird has been activated by the engine.",
		"Peak Threshold has been met, recording of raw data has commenced.",
		"Peak recording has stopped - threshold has not been met after wait period expired.",
		"A Bird does not have a valid temp file path.  The bird has been de-activated.",
		"The peak counter has been triggered.",
		"The sound triggered by TriggerSound has completed plaiying.",
		"Clipping was detected in the sample source"
	};

	// Bird State
	//////////////////////////////////////////////////////////////////////
	enum ztBirdStates
	{
		ztbs_Unitialized		=0,
		ztbs_Active				=1,
		ztbs_MonitoringOnly		=2,
		ztbs_DirectRecordMode	=4,
		ztbs_SlaveMode			=8
	};

	// Engine State
	//////////////////////////////////////////////////////////////////////
	enum ztEngineStates
	{
		ztes_Idle		= 0,
		ztes_Running	= 1,
		zted_Stopping	= 2
	};

	// Display Types
	//////////////////////////////////////////////////////////////////////
	enum ztDisplays
	{
		display_None=0,
		display_VisualizationStatsAndVars,
		display_VisualizationStats,
		display_VisualizationOnly,
		display_Log,
		ztDisplayType_Total = 5
	};
	// Visualization Types
	//////////////////////////////////////////////////////////////////////
	enum ztVisualizations
	{
		vis_None=0,
		vis_ScrollingWaveform,
		vis_WaveformPlot,
		ztVisualizationType_Total = 3
	};
	// Direct Recording Flags
	//////////////////////////////////////////////////////////////////////
	enum ztDirectRecordFlags
	{
		direct_record_start	= 1,
		direct_record_stop	= 2
	};

	// Audio Output Flags
	//////////////////////////////////////////////////////////////////////
	enum ztAudioOutputFlags
	{
		ztOutputLeft	=0,
		ztOutputRight	=1,
		ztOutputCenter	=2
	};

	//DisplayType Descriptors
	static char* zt_DisplayType_Description[ztDisplayType_Total] =
	{
		"None",
		"Visualization With Statistics and Variables",
		"Visualization With Statistics",
		"Visualization only"
	};
	//VisualizationType Descriptors
	static char* zt_VisualizationType_Description[ztVisualizationType_Total] =
	{
		"None",
		"Scrolling Waveform",
		"Waveform Plot"
	};
	// Value enums
	//////////////////////////////////////////////////////////////////////
	enum ztStringValues
	{
		vBirdID = 0,
		vBirdTempFilePath,// temp folder to write wavs into
		vBirdWaveFile,
		vBirdWaveFilePath,// where they end up
		vBirdEngineVersion,// string of version
		vBirdName,	// Name of the bird (used in generating files)
		vAudioSrcDescription,// recording source description (get only)
		vAudioTrgDescription,// output description (trigger) (get only)
		vBirdMasterID, // MasterID for a bird in slave mode

		//////////////////////////////////////////////////////////////////////
		// IOM Input Specifics
		//////////////////////////////////////////////////////////////////////
		vBird_IOM_InputModuleID, // id of the io module selected for input
		vBird_IOM_InputDeviceID,// id of the selected input device

		//////////////////////////////////////////////////////////////////////
		// IOM Output Specifics
		//////////////////////////////////////////////////////////////////////
		vBird_IOM_OutputModuleID,// id of the io module selected for output
		vBird_IOM_OutputDeviceID// id of the selected output device
	};
	enum ztLongValues
	{
		vBirdState= 0,
		vEngineState,
		vPeakThreshold,
		vPeakRecordDuration,
		vMonitorDisplay, // what is viewed when monitoring
		vRecordingDisplay,// what is viewed when recording (see above ztDisplays enums for valid values)
		vRecordingCutoffMS, // maximum number of milliseconds to record 
		vPeakComparator, // how many peaks to accept the recording?
		vDirectRecord,// start or stop direct recording (see above ztDirectRecordFlags enums for valid values)
		vPeakHP_Filter,// use a high-pass filter when looking for peaks 
		vMuteBirdData,// mutes all incoming data (0's it)
		vPeakTrigger,// number of peaks required to intiate a trigger event (0 turns off triggering)
		vOutputPanning,// DEPRICATED (WILL BE REMOVED SOON)
		vOutputVolume,// DEPRICATED (WILL BE REMOVED SOON)
		vClipThreshold,// threshold that will trigger the clipping led (default is 32767)
		vSamplesPerPixel,// how many samples per pixel to use in oscilliscope (default is 500)
		vVisualization,//which visual to use? (see above ztVisualizations for valid types)
		vPrebufferSamples,//how many samples to pre-record (default is 0)

		//////////////////////////////////////////////////////////////////////
		// IOM Input Specifics
		//////////////////////////////////////////////////////////////////////
		vBird_IOM_InputModuleVerMaj, //(get only) - major version of selected input module
		vBird_IOM_InputModuleVerMin,//(get only) - minor version of selected input module
		vBird_IOM_InputFormatIDX,// input format index
		vBird_IOM_InputChannel,// which channel
		vBird_IOM_InputPan,// not used at the moment
		vBird_IOM_InputGain,//(-10,000 = silence  0 = full volume)

		//////////////////////////////////////////////////////////////////////
		// IOM Output Specifics
		//////////////////////////////////////////////////////////////////////
		vBird_IOM_OutputModuleVerMaj, //(get only) - major version of selected output module
		vBird_IOM_OutputModuleVerMin,//(get only) - minor version of selected output module
		vBird_IOM_OutputFormatIDX,// output format index
		vBird_IOM_OutputChannel,// not used at the moment
		vBird_IOM_OutputPan,// panning of output (left,right,center)
		vBird_IOM_OutputGain//(-10,000 = silence  0 = full volume)
	};
	enum ztFloatVals
	{
		vPeakGain=0,
		//////////////////////////////////////////////////////////////////////
		// IOM Input Specifics
		//////////////////////////////////////////////////////////////////////
		vBird_IOM_InputScaler
	};

}// end of namespace ztBirdEngine 

// Some helper routines that make using the system easier in borland
// (or at least should make it easier)
#ifdef ZT_INCLUDE_BORLAND_HELPERS
#include <string>
#include <fstream>
#include <iostream> 
#include <sstream> 
//////////////////////////////////////////////////////////////////////
/* ztGetBirdString

	Gets a std::string from a bird object...
*/
//////////////////////////////////////////////////////////////////////
static std::string ztGetBirdString(TztBird* pBird,int nID)
{
     WideString wsTemp=pBird->GetStringValue(nID);
	 AnsiString ansTemp=wsTemp;
	 std::string sRet=ansTemp.c_str();
	 return sRet;
}

//////////////////////////////////////////////////////////////////////
/* ztSetBirdString

	Set a string in the ocx using a char*
*/
//////////////////////////////////////////////////////////////////////
static void ztSetBirdString(TztBird* pBird,int nID,char* string)
{
	pBird->SetStringValue(nID,(wchar_t*)WideString(string));
}

//////////////////////////////////////////////////////////////////////
/* ztSetEngineString

	Set a string in the ocx using a char*
*/
//////////////////////////////////////////////////////////////////////
static void ztSetEngineString(TztBirdEngine* pEngine,int nID,char* string)
{
	 pEngine->SetStringValue(nID,(wchar_t*)WideString(string));
}
//////////////////////////////////////////////////////////////////////
/* ztGetBirdString

	Gets a std::string from an Engine object...
*/
//////////////////////////////////////////////////////////////////////
static std::string ztGetEngineString(TztBirdEngine* pEngine,int nID)
{
     WideString wsTemp=pEngine->GetStringValue(nID);
	 AnsiString ansTemp=wsTemp;
	 std::string sRet=ansTemp.c_str();
	 return sRet;
}

//////////////////////////////////////////////////////////////////////
/* CZT_OCX_BirdProps
	Simple class that should make reading/writing all pertinent bird
	ocx values easy.
*/
//////////////////////////////////////////////////////////////////////
class CZT_OCX_BirdProps  
{
public:

	CZT_OCX_BirdProps(){};
	virtual ~CZT_OCX_BirdProps(){};

	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	/*
		Retrieval Functions
	*/
	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////
	// Retrieve All Values
	//////////////////////////////////////////////////////////////////////
	void		RetrieveAllValues(TztBird* pBird)
	{
		RetrieveFloatValues(pBird);
		RetrieveStringValues(pBird);
		RetrieveLongValues(pBird);

	}

	//////////////////////////////////////////////////////////////////////
	// Retrieve Float Values
	//////////////////////////////////////////////////////////////////////
	void		RetrieveFloatValues(TztBird* pBird)
	{
		using namespace ztBirdEngine;
		m_fPeakGain = pBird->GetFloatValue(vPeakGain);
		m_fIOM_InputScaler= pBird->GetFloatValue(vBird_IOM_InputScaler);
	}

	//////////////////////////////////////////////////////////////////////
	// Retrieve String Values
	//////////////////////////////////////////////////////////////////////
	void		RetrieveStringValues(TztBird* pBird)
	{
		using namespace ztBirdEngine;

        m_sBirdID = ztGetBirdString(pBird,vBirdID);
        m_sBirdName = ztGetBirdString(pBird,vBirdName);
        m_sTempFilePath = ztGetBirdString(pBird,vBirdTempFilePath);
        m_sWaveFilePath = ztGetBirdString(pBird,vBirdWaveFilePath);
        m_sWaveFile = ztGetBirdString(pBird,vBirdWaveFile);
        m_sMasterID = ztGetBirdString(pBird,vBirdMasterID);
        m_sIOM_InputModuleID = ztGetBirdString(pBird,vBird_IOM_InputModuleID);
        m_sIOM_InputDeviceID = ztGetBirdString(pBird,vBird_IOM_InputDeviceID);
        m_sIOM_OutputModuleID = ztGetBirdString(pBird,vBird_IOM_OutputModuleID);
        m_sIOM_OutputDeviceID = ztGetBirdString(pBird,vBird_IOM_OutputDeviceID);

        m_sAudioSrcDescription= ztGetBirdString(pBird,vAudioSrcDescription);
        m_sAudioTrgDescription= ztGetBirdString(pBird,vAudioTrgDescription);
	}

	//////////////////////////////////////////////////////////////////////
	// Retrieve Long Values
	//////////////////////////////////////////////////////////////////////
	void		RetrieveLongValues(TztBird* pBird)
	{
		using namespace ztBirdEngine;

		m_lBirdState=pBird->GetLongValue(vBirdState);
		m_lPeakThreshold=pBird->GetLongValue(vPeakThreshold);
		m_lPeakRecordDuration=pBird->GetLongValue(vPeakRecordDuration);
		m_lMonitorDisplay=pBird->GetLongValue(vMonitorDisplay); // what is viewed when monitoring
		m_lRecordingDisplay=pBird->GetLongValue(vRecordingDisplay);// what is viewed when recording (see above ztDisplays enums for valid values)
		m_lRecordingCutoffMS=pBird->GetLongValue(vRecordingCutoffMS); // maximum number of milliseconds to record 
		m_lPeakComparator=pBird->GetLongValue(vPeakComparator); // how many peaks to accept the recording?
		m_lPeakHP_Filter=pBird->GetLongValue(vPeakHP_Filter);// use a high-pass filter when looking for peaks 
		m_lPeakTrigger=pBird->GetLongValue(vPeakTrigger);// number of peaks required to intiate a trigger event (0 turns off triggering)
		m_lOutputPanning=pBird->GetLongValue(vOutputPanning);// DEPRICATED (WILL BE REMOVED SOON)
		m_lOutputVolume=pBird->GetLongValue(vOutputVolume);// DEPRICATED (WILL BE REMOVED SOON)
		m_lClipThreshold=pBird->GetLongValue(vClipThreshold);// the clip threshold (0-32767)
		m_lSamplesPerPixel=pBird->GetLongValue(vSamplesPerPixel);
		m_lVisualizationType=pBird->GetLongValue(vVisualization);
		m_lPrebufferSamples=pBird->GetLongValue(vPrebufferSamples);
		

		//////////////////////////////////////////////////////////////////////
		// IOM Input Specifics
		//////////////////////////////////////////////////////////////////////
		m_lIOM_InputModuleVerMaj=pBird->GetLongValue(vBird_IOM_InputModuleVerMaj); //(read only) - major version of selected input module
		m_lIOM_InputModuleVerMin=pBird->GetLongValue(vBird_IOM_InputModuleVerMin);//(read only) - minor version of selected input module
		m_lIOM_InputFormatIDX=pBird->GetLongValue(vBird_IOM_InputFormatIDX);// input format index
		m_lIOM_InputChannel=pBird->GetLongValue(vBird_IOM_InputChannel);// which channel
		m_lIOM_InputPan=pBird->GetLongValue(vBird_IOM_InputPan);// not used at the moment
		m_lIOM_InputGain=pBird->GetLongValue(vBird_IOM_InputGain);//(-10,000 = silence  0 = full volume)

		//////////////////////////////////////////////////////////////////////
		// IOM Output Specifics
		//////////////////////////////////////////////////////////////////////
		m_lIOM_OutputModuleVerMaj=pBird->GetLongValue(vBird_IOM_OutputModuleVerMaj); //(get only) - major version of selected output module
		m_lIOM_OutputModuleVerMin=pBird->GetLongValue(vBird_IOM_OutputModuleVerMin);//(get only) - minor version of selected output module
		m_lIOM_OutputFormatIDX=pBird->GetLongValue(vBird_IOM_OutputFormatIDX);// output format index
		m_lIOM_OutputChannel=pBird->GetLongValue(vBird_IOM_OutputChannel);// not used at the moment
		m_lIOM_OutputPan=pBird->GetLongValue(vBird_IOM_OutputPan);// panning of output (left,right,center)
		m_lIOM_OutputGain=pBird->GetLongValue(vBird_IOM_OutputGain);//(-10,000 = silence  0 = full volum

	}

	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	/*
		Put Functions
	*/
	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////
	// Put All Values
	//////////////////////////////////////////////////////////////////////
	void		PutAllValues(TztBird* pBird)
	{
		PutFloatValues(pBird);
		PutStringValues(pBird);
		PutLongValues(pBird);
	}


	//////////////////////////////////////////////////////////////////////
	// Put Float Values
	//////////////////////////////////////////////////////////////////////
	void		PutFloatValues(TztBird* pBird)
	{
		using namespace ztBirdEngine;
		pBird->SetFloatValue(vPeakGain,m_fPeakGain);
		pBird->SetFloatValue(vBird_IOM_InputScaler,m_fIOM_InputScaler);

	}

	//////////////////////////////////////////////////////////////////////
	// Put String Values
	//////////////////////////////////////////////////////////////////////
	void		PutStringValues(TztBird* pBird)
	{
		using namespace ztBirdEngine;
		pBird->SetStringValue(vBirdID,(wchar_t*)WideString(m_sBirdID.c_str()));
		pBird->SetStringValue(vBirdName,(wchar_t*)WideString(m_sBirdName.c_str()));
		pBird->SetStringValue(vBirdTempFilePath,(wchar_t*)WideString(m_sTempFilePath.c_str()));
		pBird->SetStringValue(vBirdWaveFilePath,(wchar_t*)WideString(m_sWaveFilePath.c_str()));
		pBird->SetStringValue(vBirdWaveFile,(wchar_t*)WideString(m_sWaveFile.c_str()));
		pBird->SetStringValue(vBirdMasterID,(wchar_t*)WideString(m_sMasterID.c_str()));
		pBird->SetStringValue(vBird_IOM_InputModuleID,(wchar_t*)WideString(m_sIOM_InputModuleID.c_str()));
		pBird->SetStringValue(vBird_IOM_InputDeviceID,(wchar_t*)WideString(m_sIOM_InputDeviceID.c_str()));
		pBird->SetStringValue(vBird_IOM_OutputModuleID,(wchar_t*)WideString(m_sIOM_OutputModuleID.c_str()));
		pBird->SetStringValue(vBird_IOM_OutputDeviceID,(wchar_t*)WideString(m_sIOM_OutputDeviceID.c_str()));
	}

	//////////////////////////////////////////////////////////////////////
	// Put Long Values
	//////////////////////////////////////////////////////////////////////
	void		PutLongValues(TztBird* pBird)
	{
		using namespace ztBirdEngine;

		pBird->SetLongValue(vBirdState,m_lBirdState);
		pBird->SetLongValue(vPeakThreshold,m_lPeakThreshold);
		pBird->SetLongValue(vPeakRecordDuration,m_lPeakRecordDuration);
		pBird->SetLongValue(vMonitorDisplay,m_lMonitorDisplay); // what is viewed when monitoring
		pBird->SetLongValue(vRecordingDisplay,m_lRecordingDisplay);// what is viewed when recording (see above ztDisplays enums for valid values)
		pBird->SetLongValue(vRecordingCutoffMS,m_lRecordingCutoffMS); // maximum number of milliseconds to record 
		pBird->SetLongValue(vPeakComparator,m_lPeakComparator); // how many peaks to accept the recording?
		pBird->SetLongValue(vPeakHP_Filter,m_lPeakHP_Filter);// use a high-pass filter when looking for peaks 
		pBird->SetLongValue(vPeakTrigger,m_lPeakTrigger);// number of peaks required to intiate a trigger event (0 turns off triggering)
		pBird->SetLongValue(vOutputPanning,m_lOutputPanning);// DEPRICATED (WILL BE REMOVED SOON)
		pBird->SetLongValue(vOutputVolume,m_lOutputVolume);// DEPRICATED (WILL BE REMOVED SOON)
		pBird->SetLongValue(vClipThreshold,m_lClipThreshold);// DEPRICATED (WILL BE REMOVED SOON)
		pBird->SetLongValue(vSamplesPerPixel,m_lSamplesPerPixel);
		pBird->SetLongValue(vVisualization,m_lVisualizationType);
		pBird->SetLongValue(vPrebufferSamples,m_lPrebufferSamples);

		//////////////////////////////////////////////////////////////////////
		// IOM Input Specifics
		//////////////////////////////////////////////////////////////////////
		pBird->SetLongValue(vBird_IOM_InputFormatIDX,m_lIOM_InputFormatIDX);// input format index
		pBird->SetLongValue(vBird_IOM_InputChannel,m_lIOM_InputChannel);// which channel
		pBird->SetLongValue(vBird_IOM_InputPan,m_lIOM_InputPan);// not used at the moment
		pBird->SetLongValue(vBird_IOM_InputGain,m_lIOM_InputGain);//(-10,000  silence  0  full volume)

		//////////////////////////////////////////////////////////////////////
		// IOM Output Specifics
		//////////////////////////////////////////////////////////////////////
		pBird->SetLongValue(vBird_IOM_OutputFormatIDX,m_lIOM_OutputFormatIDX);// output format index
		pBird->SetLongValue(vBird_IOM_OutputChannel,m_lIOM_OutputChannel);// not used at the moment
		pBird->SetLongValue(vBird_IOM_OutputPan,m_lIOM_OutputPan);// panning of output (left,right,center)
		pBird->SetLongValue(vBird_IOM_OutputGain,m_lIOM_OutputGain);//(-10,000  silence  0  full volum

	}

	// Float Values
	//////////////////////////////////////////////////////////////////////
	float		m_fPeakGain;

	// Long Values
	//////////////////////////////////////////////////////////////////////
	long		m_lBirdState;
	long		m_lPeakThreshold;
	long		m_lPeakRecordDuration;
	long		m_lMonitorDisplay; // what is viewed when monitoring
	long		m_lRecordingDisplay;// what is viewed when recording (see above ztDisplays enums for valid values)
	long		m_lRecordingCutoffMS; // maximum number of milliseconds to record 
	long		m_lPeakComparator; // how many peaks to accept the recording?
	long		m_lPeakHP_Filter;// use a high-pass filter when looking for peaks 
	long		m_lPeakTrigger;// number of peaks required to intiate a trigger event (0 turns off triggering)
	long		m_lOutputPanning;// DEPRICATED (WILL BE REMOVED SOON)
	long		m_lOutputVolume;// DEPRICATED (WILL BE REMOVED SOON)
	long		m_lClipThreshold;// defaults to 32767
	long		m_lSamplesPerPixel;// defaults to 500
	long		m_lVisualizationType;//defaults to scrolling waveform
	long		m_lPrebufferSamples;//how many samples to pre-record (default is 0)

	//////////////////////////////////////////////////////////////////////
	// IOM Input Specifics
	//////////////////////////////////////////////////////////////////////
	long		m_lIOM_InputModuleVerMaj; //(read only) - major version of selected input module
	long		m_lIOM_InputModuleVerMin;//(read only) - minor version of selected input module
	long		m_lIOM_InputFormatIDX;// input format index
	long		m_lIOM_InputChannel;// which channel
	long		m_lIOM_InputPan;// not used at the moment
	long		m_lIOM_InputGain;//(-10,000 = silence  0 = full volume)
	float		m_fIOM_InputScaler;//(.1-2.0f Default = 1.0f - Scaler applied to raw samples before processing)

	//////////////////////////////////////////////////////////////////////
	// IOM Output Specifics
	//////////////////////////////////////////////////////////////////////
	long		m_lIOM_OutputModuleVerMaj; //(get only) - major version of selected output module
	long		m_lIOM_OutputModuleVerMin;//(get only) - minor version of selected output module
	long		m_lIOM_OutputFormatIDX;// output format index
	long		m_lIOM_OutputChannel;// not used at the moment
	long		m_lIOM_OutputPan;// panning of output (left,right,center)
	long		m_lIOM_OutputGain;//(-10,000 = silence  0 = full volum	

	// String Values
	//////////////////////////////////////////////////////////////////////
    std::string m_sBirdID;// id of bird
    std::string m_sBirdName;//name of bird
    std::string m_sTempFilePath;// temp folder to write wavs into
    std::string m_sWaveFilePath;// where accepted wavs end up
    std::string m_sWaveFile;//output wave file
    std::string m_sMasterID;// MasterID for a bird in slave mode
    std::string m_sIOM_InputModuleID;// id of the io module selected for input
    std::string m_sIOM_InputDeviceID;// id of the selected input device
    std::string m_sIOM_OutputModuleID;// id of the io module selected for output
    std::string m_sIOM_OutputDeviceID;// id of the selected output device

	// Read Only String Values
	//////////////////////////////////////////////////////////////////////
    std::string m_sAudioSrcDescription;
    std::string m_sAudioTrgDescription;
};
//////////////////////////////////////////////////////////////////////
/* CZT_OCX_Serializer
	Simple class that should make serializing all pertinent bird
	ocx values easy.
*/
//////////////////////////////////////////////////////////////////////
class CZT_OCX_Serializer
{
public:

	CZT_OCX_Serializer(TztBird* pBird){m_pBird=pBird;};
	virtual ~CZT_OCX_Serializer(){};

	// Pure Virtual calls
	//////////////////////////////////////////////////////////////////////
	virtual	bool		SaveValue(std::string* psName,std::string* psValue)=0;
	virtual	bool		ReadValue(std::string* psName,std::string* psReturnValue)=0;

	// Serialize
	//////////////////////////////////////////////////////////////////////
	virtual void		Serialize(bool bIsWriting)
	{
		CZT_OCX_BirdProps props;

		if(bIsWriting)
			//RetrieveAllValues
			props.RetrieveAllValues(m_pBird);

		// serialize each item....

		// Float Values
		//////////////////////////////////////////////////////////////////////
		SerializeFloat(&props.m_fPeakGain,"fPeakGain",bIsWriting);		

		// Long Values
		//////////////////////////////////////////////////////////////////////
		SerializeLong(&props.m_lBirdState,"lBirdState",bIsWriting);
		SerializeLong(&props.m_lPeakThreshold,"lPeakThreshold",bIsWriting);
		SerializeLong(&props.m_lPeakRecordDuration,"lPeakRecordDuration",bIsWriting);
		SerializeLong(&props.m_lMonitorDisplay,"lMonitorDisplay",bIsWriting);
		SerializeLong(&props.m_lRecordingDisplay,"lRecordingDisplay",bIsWriting);
		SerializeLong(&props.m_lRecordingCutoffMS,"lRecordingCutoffMS",bIsWriting);
		SerializeLong(&props.m_lPeakComparator,"lPeakComparator",bIsWriting);
		SerializeLong(&props.m_lPeakHP_Filter,"lPeakHP_Filter",bIsWriting);
		SerializeLong(&props.m_lPeakTrigger,"lPeakTrigger",bIsWriting);
		SerializeLong(&props.m_lOutputPanning,"lOutputPanning",bIsWriting);
		SerializeLong(&props.m_lOutputVolume,"lOutputVolume",bIsWriting);
		SerializeLong(&props.m_lClipThreshold,"lClipThreshold",bIsWriting);
		SerializeLong(&props.m_lSamplesPerPixel,"lSamplesPerPixel",bIsWriting);
		SerializeLong(&props.m_lVisualizationType,"lVisualizationType",bIsWriting);
		SerializeLong(&props.m_lPrebufferSamples,"lPrebufferSamples",bIsWriting);		

		//////////////////////////////////////////////////////////////////////
		// IOM Input Specifics
		//////////////////////////////////////////////////////////////////////
		SerializeLong(&props.m_lIOM_InputModuleVerMaj,"lIOM_InputModuleVerMa",bIsWriting);
		SerializeLong(&props.m_lIOM_InputModuleVerMin,"lIOM_InputModuleVerMin",bIsWriting);
		SerializeLong(&props.m_lIOM_InputFormatIDX,"lIOM_InputFormatIDX",bIsWriting);
		SerializeLong(&props.m_lIOM_InputChannel,"lIOM_InputChannel",bIsWriting);
		SerializeLong(&props.m_lIOM_InputPan,"lIOM_InputPan",bIsWriting);
		SerializeLong(&props.m_lIOM_InputGain,"lIOM_InputGain",bIsWriting);
		SerializeFloat(&props.m_fIOM_InputScaler,"fIOM_InputScaler",bIsWriting);

		
		//////////////////////////////////////////////////////////////////////
		// IOM Output Specifics
		//////////////////////////////////////////////////////////////////////
		SerializeLong(&props.m_lIOM_OutputModuleVerMaj,"lIOM_OutputModuleVerMaj",bIsWriting);
		SerializeLong(&props.m_lIOM_OutputModuleVerMin,"lIOM_OutputModuleVerMin",bIsWriting);
		SerializeLong(&props.m_lIOM_OutputFormatIDX,"lIOM_OutputFormatIDX",bIsWriting);
		SerializeLong(&props.m_lIOM_OutputChannel,"lIOM_OutputChannel",bIsWriting);
		SerializeLong(&props.m_lIOM_OutputPan,"lIOM_OutputPan",bIsWriting);
		SerializeLong(&props.m_lIOM_OutputGain,"lIOM_OutputGain",bIsWriting);

		// String Values
		//////////////////////////////////////////////////////////////////////
		SerializeString(&props.m_sBirdID,"sBirdID",bIsWriting);
		SerializeString(&props.m_sBirdName,"sBirdName",bIsWriting);
		SerializeString(&props.m_sTempFilePath,"sTempFilePath",bIsWriting);
		SerializeString(&props.m_sWaveFilePath,"sWaveFilePath",bIsWriting);
		SerializeString(&props.m_sWaveFile,"sWaveFile",bIsWriting);
		SerializeString(&props.m_sMasterID,"sMasterID",bIsWriting);
		SerializeString(&props.m_sIOM_InputModuleID,"sIOM_InputModuleID",bIsWriting);
		SerializeString(&props.m_sIOM_InputDeviceID,"sIOM_InputDeviceID",bIsWriting);
		SerializeString(&props.m_sIOM_OutputModuleID,"sIOM_OutputModuleID",bIsWriting);
		SerializeString(&props.m_sIOM_OutputDeviceID,"sIOM_OutputDeviceID",bIsWriting);

		// Read Only String Values
		//////////////////////////////////////////////////////////////////////
		SerializeString(&props.m_sAudioSrcDescription,"sAudioSrcDescription",bIsWriting);
		SerializeString(&props.m_sAudioTrgDescription,"sAudioTrgDescription",bIsWriting);

		if(!bIsWriting)
			//RetrieveAllValues
			props.PutAllValues(m_pBird);
	}

	//////////////////////////////////////////////////////////////////////
	// Helpers
	//////////////////////////////////////////////////////////////////////

	// Serialize a long value
	//////////////////////////////////////////////////////////////////////
	virtual bool		SerializeLong(long* lVal,std::string Name,bool bIsWriting)
	{
		if(bIsWriting)
		{
			// convert it to a string....
			std::ostringstream os; 
			os << *lVal;
			std::string sVal(os.str());
			return SaveValue(&Name,&sVal);
		}
		else
		{
			// we are reading it..
			std::string sVal;
			if(!ReadValue(&Name,&sVal))
				return false;
			// convert it to a long value..
			*lVal = atol(sVal.c_str());
			return true;
		}
	}
	// Serialize a Float value
	//////////////////////////////////////////////////////////////////////
	virtual bool		SerializeFloat(float* fVal,std::string Name,bool bIsWriting)
	{
		if(bIsWriting)
		{
			// convert it to a string....
			std::ostringstream os; 
			os << *fVal;
			std::string sVal(os.str());
			return SaveValue(&Name,&sVal);
		}
		else
		{
			// we are reading it..
			std::string sVal;
			if(!ReadValue(&Name,&sVal))
				return false;
			// convert it to a long value..
			*fVal = atof(sVal.c_str());
			return true;
		}
	}
	// Serialize a String value
	//////////////////////////////////////////////////////////////////////
	virtual bool		SerializeString(std::string* sVal,std::string Name,bool bIsWriting)
	{
		if(bIsWriting)
			return SaveValue(&Name,sVal);
		else
			return ReadValue(&Name,sVal);
	}	

	// Public data
	//////////////////////////////////////////////////////////////////////
	TztBird*		m_pBird;

};
//////////////////////////////////////////////////////////////////////
/* CZT_OCX_FileSerializer
	
	  class that implements serializer to a file...
	  does not check naming - assumes reads and writes
	  happen in order...
*/
//////////////////////////////////////////////////////////////////////
// basic file operations
 
class CZT_OCX_FileSerializer : public CZT_OCX_Serializer
{
public:

	CZT_OCX_FileSerializer(TztBird* pBird): CZT_OCX_Serializer(pBird)
	{	
		m_poStream=NULL;
		m_piStream=NULL;
	};

	virtual ~CZT_OCX_FileSerializer()
	{
		ReleaseStream();
	};

	// WriteToFile(char *psFilePath)
	//////////////////////////////////////////////////////////////////////
	bool	WriteToFile(char* psFilePath)
	{
		ReleaseStream();
		m_poStream = new std::ofstream(psFilePath);
        if(!m_poStream->is_open())
        {
            ReleaseStream();
            return false;
        }

		Serialize(true);

		m_poStream->close();
		ReleaseStream();
		return true;

	}
	// SaveValue (CZT_OCX_Serializer implementation)
	//////////////////////////////////////////////////////////////////////
	bool		SaveValue(std::string* psName,std::string* psValue)
	{
		if(m_poStream == NULL)
			return false;
		(*m_poStream) << (*psValue);
		(*m_poStream) << "\n";
		return true;
	}
	// ReadValue (CZT_OCX_Serializer implementation)
	//////////////////////////////////////////////////////////////////////
	bool		ReadValue(std::string* psName,std::string* psReturnValue)
	{
		if(m_piStream == NULL)
			return false;
		if(m_piStream->eof())
			return false;

		std::getline((*m_piStream),(*psReturnValue));

		return true;

	}
	// ReadFromFile
	//////////////////////////////////////////////////////////////////////
	bool	ReadFromFile(char* psFilePath)
	{
		ReleaseStream();
		m_piStream = new std::ifstream(psFilePath);
		if(!m_piStream->is_open())
        {
            ReleaseStream();
            return false;
        }

		Serialize(false);
		ReleaseStream();
		return true;

	}
	// ReleaseStream
	//////////////////////////////////////////////////////////////////////
	void	ReleaseStream()
	{
		if(m_poStream != NULL)
			delete m_poStream;

		if(m_piStream != NULL)
			delete m_piStream;
		
		m_piStream=NULL;
		m_poStream=NULL;
	}

protected:

	// data
	//////////////////////////////////////////////////////////////////////
	std::ofstream* m_poStream;
	std::ifstream* m_piStream;

};
#endif// end of ifdef ZT_INCLUDE_BORLAND_HELPERS

#endif // !defined(AFX_CZT_BIRDFRAMEWORK_H__AB54B686_00F1_4AC6_A2C0_4340E5473FC8__INCLUDED_)
