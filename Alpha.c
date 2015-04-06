#include <stdio.h>

#include <Alpha.h>

#define COMBOX_CLASS Alpha
#define COMBOX_INTERFACE IAlpha
#include <combox.h>

typedef struct _Alpha
{
  COMBOX_VTBL;

  int Internal;
} Alpha;

static HRESULT init( IUnknown *pi )
{
  Alpha *p = (Alpha *)pi;

  printf( "%s(%d): init\n", __FILE__, __LINE__ );
  p->Internal = 55;
  return S_OK;
}

static void cleanup( IUnknown *pi )
{
  Alpha *p = (Alpha *)pi;

  p->Internal = 0;
  printf( "%s(%d): cleanup\n", __FILE__, __LINE__ );
}

static STDMETHODIMP AlphaSet( IAlpha *pi, const int a )
{
  Alpha *p = (Alpha *)pi;

  printf( "%s(%d): AlphaSet( %d )\n", __FILE__, __LINE__, a );
  p->Internal = a;
  return S_OK;
}

static IAlphaVtbl Vtbl = { 0, 0, 0, AlphaSet };
static combox_t combox = { &CLSID_Alpha, 1, { &IID_IAlpha }, { &Vtbl }, sizeof( Alpha ), init, cleanup };
