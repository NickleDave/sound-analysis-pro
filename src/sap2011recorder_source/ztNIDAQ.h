#ifdef __BUILDING_THE_DLL
        #define __EXPORT_TYPE __export
#else
        // we are not building the dll so lets include our lib...
        // use the nidaq lib...
        #pragma comment(lib, "ztNIDAQ.lib")
	#define __EXPORT_TYPE __import
#endif 

#ifdef __cplusplus
extern "C" {  // only need to export C interface if
             // used by C++ source code
#endif

// our exports
short __EXPORT_TYPE zt_DIG_In_Line (
	short        slot,
	short        port,
	short        linenum,
	short        FAR * state);

short __EXPORT_TYPE zt_DIG_Prt_Config (
	short        slot,
	short        port,
	short        latch_mode,
	short        direction);

short __EXPORT_TYPE zt_DIG_Out_Line (
	short        slot,
	short        port,
	short        linenum,
	short        state);

BOOL __EXPORT_TYPE  zt_NIDAQ_UsingDLL();
BOOL __EXPORT_TYPE  zt_NIDAQ_AllFunctionsFound();

// End of Api functions
#ifdef __cplusplus
}
#endif
