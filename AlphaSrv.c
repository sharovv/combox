#include <Alpha.h>
#include <comboxdll.h>

static combox_dll_t combox_dll[] = { { &CLSID_Alpha, Alpha_GetClassObject }, COMBOX_DLL_END };

COMBOX_DLL( CLSID_Alpha, ComboxDllGetClassObject, Alpha_ServerCount, "Alpha class COM-server" )
