#include <stdio.h>

#include <Alpha.h>
#include <combox.h>

typedef struct _Alpha
{
  COMBOX_VTBL( IAlpha );

  int Internal;
} Alpha;

static HRESULT init( IUnknown *pi )
{
  Alpha *p = (Alpha *)pi;

  printf( "%s(%d): Alpha::init\n", __FILE__, __LINE__ );
  p->Internal = 55;
  return S_OK;
}

static void cleanup( IUnknown *pi )
{
  Alpha *p = (Alpha *)pi;

  p->Internal = 0;
  printf( "%s(%d): Alpha::cleanup\n", __FILE__, __LINE__ );
}

static STDMETHODIMP AlphaSet( IAlpha *pi, const int a )
{
  Alpha *p = (Alpha *)pi;

  printf( "%s(%d): Alpha::AlphaSet( %d )\n", __FILE__, __LINE__, a );
  p->Internal = a;
  return S_OK;
}

static IAlphaVtbl Vtbl = { 0, 0, 0, AlphaSet };
static combox_t combox = { &CLSID_Alpha, 1, { &IID_IAlpha }, { &Vtbl }, sizeof( Alpha ), init, cleanup };

STDAPI Alpha_GetClassObject( REFCLSID rclsid, REFIID riid, LPVOID *ppi ) { return ComboxGetClassObject( rclsid, riid, ppi ); }
STDAPI Alpha_CreateInstance( REFCLSID rclsid, REFIID riid, LPVOID *ppi ) { return ComboxCreateInstance( rclsid, riid, ppi ); }
