#include <objbase.h>

// {DC126A90-D709-4a52-946A-60798D30E4C3}
static const GUID IID_IAlpha = { 0xdc126a90, 0xd709, 0x4a52, { 0x94, 0x6a, 0x60, 0x79, 0x8d, 0x30, 0xe4, 0xc3 } };

#undef INTERFACE
#define INTERFACE IAlpha

DECLARE_INTERFACE( IAlpha )
{
  STDMETHOD( QueryInterface )( THIS_ REFIID riid, void **ppi ) PURE;
  STDMETHOD_( unsigned long, AddRef )( THIS ) PURE;
  STDMETHOD_( unsigned long, Release )( THIS ) PURE;

  STDMETHOD( AlphaSet ) ( THIS_ const int a ) PURE;
};
