#include <objbase.h>

// {6D0BBC3D-59A3-4791-971C-7DBA2D56F12F}
static const GUID IID_IEcho = { 0x6d0bbc3d, 0x59a3, 0x4791, { 0x97, 0x1c, 0x7d, 0xba, 0x2d, 0x56, 0xf1, 0x2f } };

#undef INTERFACE
#define INTERFACE IEcho

DECLARE_INTERFACE( IEcho )
{
  STDMETHOD( QueryInterface )( THIS_ REFIID riid, void **ppi ) PURE;
  STDMETHOD_( ULONG, AddRef )( THIS ) PURE;
  STDMETHOD_( ULONG, Release )( THIS ) PURE;

  STDMETHOD( EchoSync ) ( THIS_ const int a ) PURE;
};
