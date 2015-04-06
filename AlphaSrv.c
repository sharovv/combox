#include <Alpha.h>
#include <comboxdll.h>

//COMBOXDLL( &CLSID_Alpha, Alpha_GetClassObject, Alpha_ServerCount, "Alpha class COM-server" );

STDAPI DllGetClassObject( REFCLSID rclsid, REFIID riid, LPVOID *ppi ) { return Alpha_GetClassObject( rclsid, riid, ppi ); }
STDAPI DllRegisterServer() { return ComboxDllRegisterServer( &CLSID_Alpha, "Alpha class COM-server" ); }
STDAPI DllUnregisterServer() { return ComboxDllUnregisterServer( &CLSID_Alpha ); }
STDAPI DllCanUnloadNow() { return ComboxDllCanUnloadNow( Alpha_ServerCount ); }
BOOL APIENTRY DllMain( HANDLE module, DWORD reason, LPVOID reserved ) { return ComboxDllMain( module, reason, Alpha_ServerCount ); }
