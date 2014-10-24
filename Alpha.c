#include <stdio.h>

#include <Alpha.h>
#include <combox.h>

typedef struct _Alpha_t
{
  COMBOX_VTBL;

  int Internal;
} Alpha_t;

static HRESULT init( IUnknown *pi )
{
  Alpha_t *p = (Alpha_t *)pi;

  printf( "%s(%d): %s\n", __FILE__, __LINE__, __FUNCTION__ );
  p->Internal = 55;
  return S_OK;
}

static void cleanup( IUnknown *pi )
{
  Alpha_t *p = (Alpha_t *)pi;

  p->Internal = 0;
  printf( "%s(%d): %s\n", __FILE__, __LINE__, __FUNCTION__ );
}

static STDMETHODIMP AlphaSet( IAlpha *pi, const int a )
{
  Alpha_t *p = (Alpha_t *)pi;

  printf( "%s(%d): %s( %d )\n", __FILE__, __LINE__, __FUNCTION__, a );
  p->Internal = a;
  return S_OK;
}

static IAlphaVtbl Vtbl = { 0, 0, 0, AlphaSet };
static combox_t combox = { &CLSID_Alpha, 1, { &IID_IAlpha }, { &Vtbl }, sizeof( Alpha_t ), init, cleanup };

STDAPI Alpha_GetClassObject( REFCLSID rclsid, REFIID riid, LPVOID *ppi ) { return ComboxGetClassObject( rclsid, riid, ppi ); }
STDAPI Alpha_CreateInstance( REFCLSID rclsid, REFIID riid, LPVOID *ppi ) { return ComboxCreateInstance( rclsid, riid, ppi ); }
STDAPI_( IAlpha * ) Alpha( void ) { return (IAlpha *)ComboxInstance(); }