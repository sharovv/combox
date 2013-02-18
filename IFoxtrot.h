#include <objbase.h>

// {52E1B592-A6BC-46aa-9FC6-804F038328C0}
static const GUID IID_IFoxtrot = { 0x52e1b592, 0xa6bc, 0x46aa, { 0x9f, 0xc6, 0x80, 0x4f, 0x3, 0x83, 0x28, 0xc0 } };

#undef INTERFACE
#define INTERFACE IFoxtrot

DECLARE_INTERFACE( IFoxtrot )
{
  /* Методы наследуемого интерфейса IUnknown */
  STDMETHOD( QueryInterface )( THIS_ REFIID riid, void **ppi ) PURE;
  STDMETHOD_( unsigned long, AddRef )( THIS ) PURE;
  STDMETHOD_( unsigned long, Release )( THIS ) PURE;

  STDMETHOD( FoxtrotSum ) ( THIS_ const int a, const int b ) PURE;
};
