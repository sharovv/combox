#include <objbase.h>

// {11571D41-C578-4034-922F-5BCF577ED9EE}
static const GUID IID_IBravo = { 0x11571d41, 0xc578, 0x4034, { 0x92, 0x2f, 0x5b, 0xcf, 0x57, 0x7e, 0xd9, 0xee } };

#undef INTERFACE
#define INTERFACE IBravo

DECLARE_INTERFACE( IBravo )
{
  /* Методы наследуемого интерфейса IUnknown */
  STDMETHOD( QueryInterface )( THIS_ REFIID riid, void **ppi ) PURE;
  STDMETHOD_( unsigned long, AddRef )( THIS ) PURE;
  STDMETHOD_( unsigned long, Release )( THIS ) PURE;

  STDMETHOD( BravoPush ) ( THIS_ const int a ) PURE;
};
