#include <objbase.h>

// {633C197B-F1C4-42fb-A326-FE5100A27E28}
static const GUID IID_ICharlie = { 0x633c197b, 0xf1c4, 0x42fb, { 0xa3, 0x26, 0xfe, 0x51, 0x0, 0xa2, 0x7e, 0x28 } };

#undef INTERFACE
#define INTERFACE ICharlie

DECLARE_INTERFACE( ICharlie )
{
  STDMETHOD( QueryInterface )( THIS_ REFIID riid, void **ppi ) PURE;
  STDMETHOD_( ULONG, AddRef )( THIS ) PURE;
  STDMETHOD_( ULONG, Release )( THIS ) PURE;

  STDMETHOD( CharlieTest ) ( THIS_ const int a ) PURE;
};
