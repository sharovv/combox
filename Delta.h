#include <IBravo.h>
#include <ICharlie.h>

// {455E8AD2-1A02-4687-B8A5-624EA06DC82F}
static const GUID CLSID_Delta = { 0x455e8ad2, 0x1a02, 0x4687, { 0xb8, 0xa5, 0x62, 0x4e, 0xa0, 0x6d, 0xc8, 0x2f } };

STDAPI Delta_GetClassObject( REFCLSID rclsid, REFIID riid, LPVOID *ppi );
STDAPI Delta_CreateInstance( REFCLSID rclsid, REFIID riid, LPVOID *ppi );
STDAPI_( unsigned int ) Delta_ServerCount( const int i );
STDAPI_( IBravo * ) Delta_new( void );
