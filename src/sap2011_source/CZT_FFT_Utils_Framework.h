// CZT_FFT_Utils_Framework.h: interface for the CZT_FFT_Utils_Framework class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CZT_FFT_UTILS_FRAMEWORK_H__F294E9E3_9A70_424D_BCDB_8ED061A64AA6__INCLUDED_)
#define AFX_CZT_FFT_UTILS_FRAMEWORK_H__F294E9E3_9A70_424D_BCDB_8ED061A64AA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//__declspec(dllexport)

#ifdef _ZTFFTLIB_DLLAPI_
    #define ZTFFTLIB_DLLAPI  __declspec( dllexport )
#else
	#define ZTFFTLIB_DLLAPI  __declspec( dllimport )
#endif

#ifndef _ZTFFTLIB_NOAUTOLIB_
	// non-borland
	#ifndef __BORLANDC__ 
		#pragma comment(lib, "ztFFT_Utils.lib")
	#else
	// borland
		#pragma comment(lib, "ztFFT_Utilsbrlnd.lib")
	#endif
#endif

// placeholder class (does nothing)
#ifdef _ZTFFTLIB_DLLAPI_
class CZT_FFT_Utils_Framework  
{
public:
	CZT_FFT_Utils_Framework();
	virtual ~CZT_FFT_Utils_Framework();

};
#endif

// defines for data structs..
#define ztFFTW_DEFINE_COMPLEX(R, C) typedef R C[2]
ztFFTW_DEFINE_COMPLEX(float, ztfft_complex);

#ifndef DECLARE_HANDLE
#ifdef STRICT
typedef void *HANDLE;
#define DECLARE_HANDLE(name) struct name##__ { int unused; }; typedef struct
name##__ *name
#else
typedef void * HANDLE;
#define DECLARE_HANDLE(name) typedef HANDLE name
#endif
typedef HANDLE *PHANDLE;
#endif
// The namespace
namespace ztFFTAPI 
{
	// Enumeration
	// the complex type is an array with two numbers:
	enum ztComplex
	{
		ztReal = 0,
		ztImaginary = 1
	};

	// Declare the handles we use...
	//////////////////////////////////////////////////////////////////////
	DECLARE_HANDLE            (HZT_FFTR2C);		// Handle to fft
	DECLARE_HANDLE            (HZT_FFTR2HC);	// Handle to fft

	#ifdef __cplusplus
	extern "C" {  // only need to export C interface if
              // used by C++ source code
	#endif

	// HZT_FFTR2C routines (fftr2c_ prefixed)
	//////////////////////////////////////////////////////////////////////
	ZTFFTLIB_DLLAPI HZT_FFTR2C			fftr2c_Create(int nSize);				
	ZTFFTLIB_DLLAPI void				fftr2c_Destroy(HZT_FFTR2C hFFT);
	ZTFFTLIB_DLLAPI void				fftr2c_Run(HZT_FFTR2C hFFT);
	ZTFFTLIB_DLLAPI float*				fftr2c_GetInput(HZT_FFTR2C hFFT);	
	ZTFFTLIB_DLLAPI ztfft_complex*		fftr2c_GetOutput(HZT_FFTR2C hFFT);
	ZTFFTLIB_DLLAPI int					fftr2c_GetOutputSize(HZT_FFTR2C hFFT);

	// HZT_FFTR2HC routines (fftr2hc_ prefixed)
	//////////////////////////////////////////////////////////////////////
	ZTFFTLIB_DLLAPI HZT_FFTR2HC			fftr2hc_Create(int nSize);				
	ZTFFTLIB_DLLAPI void				fftr2hc_Destroy(HZT_FFTR2HC hFFT);
	ZTFFTLIB_DLLAPI void				fftr2hc_Run(HZT_FFTR2HC hFFT);			
	ZTFFTLIB_DLLAPI float*				fftr2hc_GetInput(HZT_FFTR2HC hFFT);	
	ZTFFTLIB_DLLAPI float*				fftr2hc_GetOutput_Re(HZT_FFTR2HC hFFT);	
	ZTFFTLIB_DLLAPI float*				fftr2hc_GetOutput_Im(HZT_FFTR2HC hFFT);	
	ZTFFTLIB_DLLAPI int					fftr2hc_GetOutputSize(HZT_FFTR2HC hFFT);
//	ZTFFTLIB_DLLAPI ztfft_complex*		fftr2hc_GetOutput(HZT_FFTR2C hFFT);
//	ZTFFTLIB_DLLAPI int					fftr2hc_GetOutputSize(HZT_FFTR2C hFFT);

	// End of Api functions
	#ifdef __cplusplus
	}
	#endif

	//////////////////////////////////////////////////////////////////////
	// HZT_FFTR2C Class Wrapper
	//////////////////////////////////////////////////////////////////////
	class CFFT_R2C
	{
		public:
		// constructor
		//////////////////////////////////////////////////////////////////////
		CFFT_R2C(){m_nIn = m_nOut = 0;m_pInput =NULL;m_pOutput = NULL;m_hFFT = NULL;};// normal construct
		CFFT_R2C(int nSize){m_hFFT = NULL;Initialize(nSize);};// construct with size

		// destructor
		//////////////////////////////////////////////////////////////////////
		virtual ~CFFT_R2C()
		{
			// release
			if(m_hFFT != NULL)
			{
				//ztFFTAPI::fftr2c_Destroy(m_hFFT);
				m_hFFT = NULL;
			}
		};

		// initialization
		//////////////////////////////////////////////////////////////////////
		inline void			Initialize(int nSize)
		{
			// if we already have a handle - release it
			if(m_hFFT != NULL)
			{
				//ztFFTAPI::fftr2c_Destroy(m_hFFT);
				m_hFFT = NULL;
			}
			// ok create a brand new one...
			m_hFFT = ztFFTAPI::fftr2c_Create(nSize);
			if(m_hFFT)
			{
				m_nIn = nSize;
				m_nOut = ztFFTAPI::fftr2c_GetOutputSize(m_hFFT);
				m_pInput = ztFFTAPI::fftr2c_GetInput(m_hFFT);
				m_pOutput = ztFFTAPI::fftr2c_GetOutput(m_hFFT);
			}
			else
			{
				m_nIn = m_nOut = 0;
				m_pInput =NULL;
				m_pOutput = NULL;
				m_hFFT = NULL;
			}
		};

		// Run the FFT
		//////////////////////////////////////////////////////////////////////
		inline void			Run()
		{
			ztFFTAPI::fftr2c_Run(m_hFFT);
		};

		// Data
		//////////////////////////////////////////////////////////////////////
		int				m_nIn;			// size of input array
		int				m_nOut;			// size of complex array
		float*			m_pInput;		// ptr to input data
		ztfft_complex*	m_pOutput;		// ptr to output data

		protected:
		
		// Data
		//////////////////////////////////////////////////////////////////////
		HZT_FFTR2C		m_hFFT;// handle
	};
	// end of HZT_FFTR2C Class Wrapper
	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////
	// HZT_FFTR2HC Class Wrapper
	//////////////////////////////////////////////////////////////////////
	class CFFT_R2HC
	{
		public:
		// constructor
		//////////////////////////////////////////////////////////////////////
		CFFT_R2HC(){m_nIn = m_nOut = 0;m_pInput =NULL;m_pRe = NULL;m_pIm=NULL;m_hFFT = NULL;};// normal construct
		CFFT_R2HC(int nSize){m_hFFT = NULL;Initialize(nSize);};// construct with size

		// destructor
		//////////////////////////////////////////////////////////////////////
		virtual ~CFFT_R2HC()
		{
			// release
			if(m_hFFT != NULL)
			{
				ztFFTAPI::fftr2hc_Destroy(m_hFFT);
				m_hFFT = NULL;
			}
		};

		// initialization
		//////////////////////////////////////////////////////////////////////
		inline void			Initialize(int nSize)
		{
			// if we already have a handle - release it
			if(m_hFFT != NULL)
			{
				ztFFTAPI::fftr2hc_Destroy(m_hFFT);
				m_hFFT = NULL;
			}
			// ok create a brand new one...
			m_hFFT = ztFFTAPI::fftr2hc_Create(nSize);
			if(m_hFFT)
			{
				m_nIn = nSize;
				m_nOut = ztFFTAPI::fftr2hc_GetOutputSize(m_hFFT);
				m_pInput = ztFFTAPI::fftr2hc_GetInput(m_hFFT);
				m_pRe= ztFFTAPI::fftr2hc_GetOutput_Re(m_hFFT);
				m_pIm= ztFFTAPI::fftr2hc_GetOutput_Im(m_hFFT);
			}
			else
			{
				m_nIn = m_nOut = 0;
				m_pInput =NULL;
				m_pRe = NULL;
				m_pIm = NULL;
				m_hFFT = NULL;
			}
		};

		// Run the FFT
		//////////////////////////////////////////////////////////////////////
		inline void			Run()
		{
			ztFFTAPI::fftr2hc_Run(m_hFFT);
		};

		// Data
		//////////////////////////////////////////////////////////////////////
		int				m_nIn;			// size of input array
		int				m_nOut;			// size of output arrays
		float*			m_pInput;		// ptr to input data
		float*			m_pRe;			// real part
		float*			m_pIm;			// imaginary part

		protected:
		
		// Data
		//////////////////////////////////////////////////////////////////////
		HZT_FFTR2HC		m_hFFT;// handle
	};
	// end of HZT_FFTR2C Class Wrapper
	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////

}// end of ztFFTAPI api
#endif // !defined(AFX_CZT_FFT_UTILS_FRAMEWORK_H__F294E9E3_9A70_424D_BCDB_8ED061A64AA6__INCLUDED_)
