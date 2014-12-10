#ifndef _objbase_h
#define _objbase_h

#if defined( _WIN32 )

#error This file must never to be included in Windows, check include paths.

#endif

#ifndef BOOL
#define BOOL int
#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif /* NULL */

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FAR
#define FAR
#endif

#ifndef LPVOID
#define LPVOID void FAR *
#endif

#ifndef EXTERN_C
#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C extern
#endif
#endif

#ifndef _HRESULT_DEFINED
#define _HRESULT_DEFINED
typedef int HRESULT;
#endif /* !_HRESULT_DEFINED */

#ifndef STDMETHODCALLTYPE
#ifdef _WIN32
#define STDMETHODCALLTYPE __stdcall
#else
#define STDMETHODCALLTYPE
#endif
#endif

#ifndef STDAPICALLTYPE
#ifdef _WIN32
#define STDAPICALLTYPE __stdcall
#else
#define STDAPICALLTYPE
#endif
#endif

#ifndef STDAPI
#if defined( _WIN32 ) && !defined( _MSC_VER )
#define STDAPI EXTERN_C __declspec(dllexport) HRESULT STDAPICALLTYPE
#else
#define STDAPI EXTERN_C HRESULT STDAPICALLTYPE
#endif
#endif

#ifndef STDAPI_
#if defined( _WIN32 ) && !defined( _MSC_VER )
#define STDAPI_(t) EXTERN_C __declspec(dllexport) t STDAPICALLTYPE
#else
#define STDAPI_(t) EXTERN_C t STDAPICALLTYPE
#endif
#endif

#ifndef STDMETHODIMP
#define STDMETHODIMP HRESULT STDMETHODCALLTYPE
#endif
#ifndef STDMETHODIMP_
#define STDMETHODIMP_(t) t STDMETHODCALLTYPE
#endif

#ifndef DECLARE_INTERFACE

#if defined(__cplusplus) && !defined(CINTERFACE)
#define interface struct
#define STDMETHOD(m) virtual HRESULT STDMETHODCALLTYPE m
#define STDMETHOD_(t,m) virtual t STDMETHODCALLTYPE m
#define PURE = 0
#define THIS_
#define THIS void
#define DECLARE_INTERFACE(i) interface i
#define DECLARE_INTERFACE_(i,b) interface i: public b

#else

#define interface struct
#define STDMETHOD(m) HRESULT (STDMETHODCALLTYPE *m)
#define STDMETHOD_(t,m) t (STDMETHODCALLTYPE *m)
#define PURE
#define THIS_ INTERFACE FAR *,
#define THIS INTERFACE FAR *
#ifdef CONST_VTABLE
#define CONST_VTBL const
#define DECLARE_INTERFACE(i) typedef interface i {\
const struct i##Vtbl FAR* lpVtbl;\
}i;\
typedef const struct i##Vtbl i##Vtbl;\
const struct i##Vtbl
#else
#define CONST_VTBL
#define DECLARE_INTERFACE(i) typedef interface i {\
struct i##Vtbl FAR* lpVtbl;\
} i;\
typedef struct i##Vtbl i##Vtbl;\
struct i##Vtbl
#endif
#define DECLARE_INTERFACE_(i,b) DECLARE_INTERFACE(i)

#endif /* __cplusplus */

#endif /* DECLARE_INTERFACE */

#ifndef GUID_DEFINED
#define GUID_DEFINED
typedef struct  _GUID
{
  unsigned int Data1;
  unsigned short Data2;
  unsigned short Data3;
  unsigned char Data4[ 8 ];
} GUID;

#endif /* !GUID_DEFINED */
#if !defined( __LPGUID_DEFINED__ )
#define __LPGUID_DEFINED__
typedef GUID FAR *LPGUID;

#endif /* !__LPGUID_DEFINED__ */

#if !defined( __IID_DEFINED__ )
#define __IID_DEFINED__
typedef GUID IID;
typedef IID FAR *LPIID;

#define IID_NULL            GUID_NULL
#define IsEqualIID(riid1, riid2) IsEqualGUID(riid1, riid2)
typedef GUID CLSID;

typedef CLSID FAR *LPCLSID;

#define CLSID_NULL          GUID_NULL
#define IsEqualCLSID(rclsid1, rclsid2) IsEqualGUID(rclsid1, rclsid2)

#if defined(__cplusplus)

#ifndef _REFGUID_DEFINED
#define _REFGUID_DEFINED
#define REFGUID             const GUID &
#endif /* !_REFGUID_DEFINED */

#ifndef _REFIID_DEFINED
#define _REFIID_DEFINED
#define REFIID              const IID &
#endif /* !_REFIID_DEFINED */

#ifndef _REFCLSID_DEFINED
#define _REFCLSID_DEFINED
#define REFCLSID            const CLSID &
#endif /* !_REFCLSID_DEFINED */

#ifndef _REFFMTID_DEFINED
#define _REFFMTID_DEFINED
#define REFFMTID            const FMTID &
#endif /* !_REFFMTID_DEFINED */

#else /* !__cplusplus */

#ifndef _REFGUID_DEFINED
#define _REFGUID_DEFINED
#define REFGUID             const GUID * const
#endif /* !_REFGUID_DEFINED */

#ifndef _REFIID_DEFINED
#define _REFIID_DEFINED
#define REFIID              const IID * const
#endif /* !_REFIID_DEFINED */

#ifndef _REFCLSID_DEFINED
#define _REFCLSID_DEFINED
#define REFCLSID            const CLSID * const
#endif /* !_REFCLSID_DEFINED */

#ifndef _REFFMTID_DEFINED
#define _REFFMTID_DEFINED
#define REFFMTID            const FMTID * const
#endif /* !_REFFMTID_DEFINED */

#endif /* !__cplusplus */

#endif /* !__IID_DEFINED__ */

#if !defined( CLSCTX_INPROC_SERVER )
#define CLSCTX_INPROC_SERVER 0x1
#endif

#undef INTERFACE
#define INTERFACE IUnknown

DECLARE_INTERFACE( IUnknown )
{
  STDMETHOD( QueryInterface ) ( THIS_ REFIID riid, void FAR * FAR *ppvObject) PURE;
  STDMETHOD_( unsigned int, AddRef ) ( THIS ) PURE;
  STDMETHOD_( unsigned int, Release ) ( THIS ) PURE;
};

typedef IUnknown FAR *LPUNKNOWN;

/* {00000000-0000-0000-C000-000000000046} */
static const GUID IID_IUnknown =
{ 0x00000000, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };

#undef INTERFACE
#define INTERFACE IClassFactory

DECLARE_INTERFACE( IClassFactory )
{
  STDMETHOD( QueryInterface ) ( THIS_ REFIID riid, void FAR * FAR *ppvObject) PURE;
  STDMETHOD_( unsigned int, AddRef ) ( THIS ) PURE;
  STDMETHOD_( unsigned int, Release ) ( THIS ) PURE;

  STDMETHOD( CreateInstance )( THIS_ IUnknown FAR *pUnkOuter, REFIID riid, void FAR *FAR *ppvObject ) PURE;
  STDMETHOD( LockServer )( THIS_ int fLock ) PURE;
};

/* {00000001-0000-0000-C000-000000000046} */
static const GUID IID_IClassFactory =
{ 0x00000001, 0x0000, 0xC000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };

#if defined(__cplusplus)

static inline bool IsEqualGUID( const GUID *a, const GUID *b )
{
  return
    ((a->Data1 == b->Data1) &&
    (a->Data2 == b->Data2) &&
    (a->Data3 == b->Data3) &&
    (a->Data4[0] == b->Data4[0]) &&
    (a->Data4[1] == b->Data4[1]) &&
    (a->Data4[2] == b->Data4[2]) &&
    (a->Data4[3] == b->Data4[3]) &&
    (a->Data4[4] == b->Data4[4]) &&
    (a->Data4[5] == b->Data4[5]) &&
    (a->Data4[6] == b->Data4[6]) &&
    (a->Data4[7] == b->Data4[7]));
}

static inline bool IsEqualGUID( const GUID &a, const GUID &b )
{
  return
    ((a.Data1 == b.Data1) &&
    (a.Data2 == b.Data2) &&
    (a.Data3 == b.Data3) &&
    (a.Data4[0] == b.Data4[0]) &&
    (a.Data4[1] == b.Data4[1]) &&
    (a.Data4[2] == b.Data4[2]) &&
    (a.Data4[3] == b.Data4[3]) &&
    (a.Data4[4] == b.Data4[4]) &&
    (a.Data4[5] == b.Data4[5]) &&
    (a.Data4[6] == b.Data4[6]) &&
    (a.Data4[7] == b.Data4[7]));
}
/*
#define IsEqualGUID( a, b ) (((a)->Data1 == (b)->Data1) && \
                             ((a)->Data2 == (b)->Data2) && \
                             ((a)->Data3 == (b)->Data3) && \
                             ((a)->Data4[0] == (b)->Data4[0]) && \
                             ((a)->Data4[1] == (b)->Data4[1]) && \
                             ((a)->Data4[2] == (b)->Data4[2]) && \
                             ((a)->Data4[3] == (b)->Data4[3]) && \
                             ((a)->Data4[4] == (b)->Data4[4]) && \
                             ((a)->Data4[5] == (b)->Data4[5]) && \
                             ((a)->Data4[6] == (b)->Data4[6]) && \
                             ((a)->Data4[7] == (b)->Data4[7]))
*/
#else
#define IsEqualGUID( a, b ) \
       (((long long *)a)[0] == ((long long *)b)[0] && \
        ((long long *)a)[1] == ((long long *)b)[1])
#endif

#define CoInitialize( pvReserved )
#define CoUninitialize()

/* Result codes */
#define S_OK    ((HRESULT)(0L))
#define S_FALSE ((HRESULT)(1L))

/* OLE compatible error codes */
#define E_FAIL                           ((HRESULT)(0x80004005L))
#define E_NOINTERFACE                    ((HRESULT)(0x80004002L))
#define CLASS_E_NOAGGREGATION            ((HRESULT)(0x80040110L))
#define CLASS_E_CLASSNOTAVAILABLE        ((HRESULT)(0x80040111L))
#define E_OUTOFMEMORY                    ((HRESULT)(0x8007000EL))

#endif
