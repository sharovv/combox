#include <IEcho.h>

// {238E2503-C714-4b4d-A38B-1DE171CAAD43}
static const GUID CLSID_Echo = { 0x238e2503, 0xc714, 0x4b4d, { 0xa3, 0x8b, 0x1d, 0xe1, 0x71, 0xca, 0xad, 0x43 } };

STDAPI Echo_GetClassObject( REFCLSID rclsid, REFIID riid, LPVOID *ppi );
STDAPI Echo_CreateInstance( REFCLSID rclsid, REFIID riid, LPVOID *ppi );
