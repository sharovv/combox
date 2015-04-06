#ifndef _comboxdll_h
#define _comboxdll_h

#include <objbase.h>
#include <combox.h>

#if defined( _WIN32 )

#include <stdio.h>
#include <olectl.h>

static HINSTANCE ComboxModule;

static char *combox_guid2str( const GUID *g, char *s )
{
  sprintf( s, "{%08lX-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X}",
    g->Data1,
    g->Data2,
    g->Data3,
    g->Data4[0], g->Data4[1],
    g->Data4[2], g->Data4[3], g->Data4[4], g->Data4[5], g->Data4[6], g->Data4[7] );
  return s;
}

static HRESULT ComboxDllRegisterServer( const GUID *clsid, const char *desc ) 
{
  char filename[ 260 ], tmp[ 260 ] = "CLSID\\";
  DWORD len;
  HKEY hkey;

  combox_guid2str( clsid, tmp + 6 );

  len = GetModuleFileName( ComboxModule, filename, _MAX_PATH );

  if( RegCreateKey( HKEY_CLASSES_ROOT, tmp, &hkey ) != ERROR_SUCCESS )
    return SELFREG_E_CLASS;

  if( RegSetValue( hkey, "InprocServer32", REG_SZ, filename, len ) != ERROR_SUCCESS )
  {
    RegCloseKey( hkey );
    return SELFREG_E_CLASS;
  }

  if( RegSetValue( hkey, NULL, REG_SZ, desc, strlen( desc ) ) != ERROR_SUCCESS )
  {
    RegCloseKey( hkey );
    return SELFREG_E_CLASS;
  }
  RegCloseKey( hkey );

  //добавляем ThreadingModel к InprocServer
  strcat( tmp, "\\InprocServer32" );
  if( RegOpenKey( HKEY_CLASSES_ROOT, tmp, &hkey ) != ERROR_SUCCESS )
    return SELFREG_E_CLASS;

  strcpy( tmp, "Apartment" );
  if( RegSetValueEx( hkey, "ThreadingModel", 0, REG_SZ, tmp, strlen( tmp ) ) != ERROR_SUCCESS )
  {
    RegCloseKey( hkey );
    return SELFREG_E_CLASS;
  }
  RegCloseKey( hkey );

  return S_OK;
}

static HRESULT ComboxDllUnregisterServer( const GUID *clsid )
{
  char tmp[ 260 ] = "CLSID\\";

  combox_guid2str( clsid, tmp + 6 );

  strcat( tmp, "\\InprocServer32" );

  RegDeleteKey( HKEY_CLASSES_ROOT, tmp );

  strcpy( tmp, "CLSID\\" );
  combox_guid2str( clsid, tmp + 6 );
  RegDeleteKey( HKEY_CLASSES_ROOT, tmp );

  return S_OK;
}

static ULONG ComboxDllCanUnloadNow( ULONG (STDAPICALLTYPE *server_count)( const int ) ) 
{
  return server_count( 0 ) == 0 ? S_OK: S_FALSE;
}

static BOOL ComboxDllMain( HANDLE module, DWORD reason, ULONG (STDAPICALLTYPE *server_count)( const int ) )
{
  if( reason == DLL_PROCESS_ATTACH ) 
  { 
    ComboxModule = (HINSTANCE)module; 
    server_count( -2 );
  }
  return TRUE;
}

#define COMBOX_DLL( clsid, get_class_object, server_count, description ) \
STDAPI DllGetClassObject( REFCLSID rclsid, REFIID riid, LPVOID *ppi ) { return get_class_object( rclsid, riid, ppi ); } \
STDAPI DllRegisterServer() { return ComboxDllRegisterServer( clsid, description ); } \
STDAPI DllUnregisterServer() { return ComboxDllUnregisterServer( clsid ); } \
STDAPI DllCanUnloadNow() { return ComboxDllCanUnloadNow( server_count ); } \
BOOL APIENTRY DllMain( HANDLE module, DWORD reason, LPVOID reserved ) { return ComboxDllMain( module, reason, server_count ); }

#else

#define COMBOX_DLL( clsid, get_class_object, server_count, description ) \
STDAPI DllGetClassObject( REFCLSID rclsid, REFIID riid, LPVOID *ppi ) { return get_class_object( rclsid, riid, ppi ); }

#endif

#endif
