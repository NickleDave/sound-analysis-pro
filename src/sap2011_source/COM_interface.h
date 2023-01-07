// 1.2
// Unit1.h : Declaration of the TSAPObjectImpl

#ifndef COM_interfaceH
#define COM_interfaceH

#define _ATL_APARTMENT_THREADED

#include "SAP2011_TLB.h"

/////////////////////////////////////////////////////////////////////////////
// TSAPObjectImpl     Implements ISAPObject, default interface of SAPObject
//                  and ISAPObjectEvents, the default source interface
// ThreadingModel : tmApartment
// Dual Interface : TRUE
// Event Support  : TRUE
// Default ProgID : SAP2011.SAPObject
// Description    : Interface implementation for ISAPObject
/////////////////////////////////////////////////////////////////////////////
class ATL_NO_VTABLE TSAPObjectImpl :
  public CComObjectRootEx<CComSingleThreadModel>,
  public CComCoClass<TSAPObjectImpl, &CLSID_SAPObject>,
  public IConnectionPointContainerImpl<TSAPObjectImpl>,
  public TEvents_SAPObject<TSAPObjectImpl>,
  public IDispatchImpl<ISAPObject, &IID_ISAPObject, &LIBID_SAP2011>
{
public:
  TSAPObjectImpl()
  {
  }

  // Data used when registering Object
  //
  DECLARE_THREADING_MODEL(otApartment);

  DECLARE_PROGID("SAP2011.SAPObject");
  //DECLARE_PROGID("SAP2011_1.SAPObject");
  //DECLARE_PROGID("SAP2011_2.SAPObject");
  //DECLARE_PROGID("SAP2011_3.SAPObject");
  //DECLARE_PROGID("SAP2011_4.SAPObject");
  //DECLARE_PROGID("SAP2011_5.SAPObject");

  //DECLARE_PROGID("SAP2011_6.SAPObject");
  //DECLARE_PROGID("SAP2011_7.SAPObject");
  //DECLARE_PROGID("SAP2011_8.SAPObject");
  //DECLARE_PROGID("SAP2011_9.SAPObject");
  //DECLARE_PROGID("SAP2011_10.SAPObject");
  //DECLARE_PROGID("SAP2011_11.SAPObject");

  DECLARE_DESCRIPTION("Interface implementation for ISAPObject");

  // Function invoked to (un)register object
  //
  static HRESULT WINAPI UpdateRegistry(BOOL bRegister)
  {
	TTypedComServerRegistrarT<TSAPObjectImpl>
    regObj(GetObjectCLSID(), GetProgID(), GetDescription());
    return regObj.UpdateRegistry(bRegister);
  }


DECLARE_GET_CONTROLLING_UNKNOWN()

BEGIN_COM_MAP(TSAPObjectImpl)
  COM_INTERFACE_ENTRY(ISAPObject)
  COM_INTERFACE_ENTRY2(IDispatch, ISAPObject)
  COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(TSAPObjectImpl)
  CONNECTION_POINT_ENTRY(DIID_ISAPObjectEvents)
END_CONNECTION_POINT_MAP()

// ISAPObject
public:
  STDMETHOD(DoBatch());
  STDMETHOD(SoundList1Add(BSTR FileName));
  STDMETHOD(SoundList1Clear());
  STDMETHOD(SoundList2Add(BSTR FileName));
  STDMETHOD(SoundList2Clear());
  STDMETHOD(Init(long ButtonNumber));
  STDMETHOD(SetBatchDir(BSTR DirName));
  STDMETHOD(DeleteRecords());
  STDMETHOD(SetBatchMode(long BatchMode));
  //STDMETHOD(send_vector());
};

#endif //Unit1H
