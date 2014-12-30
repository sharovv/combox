#include <IFoxtrot.h>

// {AD826708-75D9-49d8-A688-D245AD95EC3C}
static const GUID CLSID_Foxtrot = { 0xad826708, 0x75d9, 0x49d8, { 0xa6, 0x88, 0xd2, 0x45, 0xad, 0x95, 0xec, 0x3c } };

STDAPI Foxtrot_GetClassObject( REFCLSID rclsid, REFIID riid, LPVOID *ppi );
STDAPI Foxtrot_CreateInstance( REFCLSID rclsid, REFIID riid, LPVOID *ppi );
STDAPI_( ULONG ) Foxtrot_ServerCount( const int i );
STDAPI_( IFoxtrot * ) Foxtrot_new( void );
