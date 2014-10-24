#include <stdio.h>

#include <Delta.h>
#include <Alpha.h>
#include <combox.h>

typedef struct _Delta
{
  COMBOX_VTBL;

  IAlpha *alpha;
  int Internal;
} Delta;

static HRESULT init( IUnknown *pi )
{
  Delta *p = (Delta *)pi;

  Alpha_CreateInstance( &CLSID_Alpha, &IID_IAlpha, (void **)&p->alpha );
  printf( "%s(%d): %s 0x%08lX\n", __FILE__, __LINE__, __FUNCTION__, (unsigned long)p );
  p->Internal = 55;
  return S_OK;
}

static void cleanup( IUnknown *pi )
{
  Delta *p = (Delta *)pi;

  p->alpha->lpVtbl->Release( p->alpha );
  printf( "%s(%d): %s\n", __FILE__, __LINE__, __FUNCTION__ );
}

static STDMETHODIMP BravoPush( IBravo *pi, const int a )
{
  Delta *p = (Delta *)pi;

  printf( "%s(%d): %s( 0x%08lX, %d )\n", __FILE__, __LINE__, __FUNCTION__, (unsigned long)p, a );
  p->Internal = a;
  return S_OK;
}

static STDMETHODIMP CharlieTest( ICharlie *pi, const int a )
{
  Delta *p = combox_instance( pi );

  printf( "%s(%d): %s( 0x%08lX, %d )\n", __FILE__, __LINE__, __FUNCTION__, (unsigned long)p, a );
  p->Internal = a;
  return S_OK;
}  

static STDMETHODIMP QueryInterface( Delta *p, REFIID riid, void **ppi )
{
  return Combox_QueryInterfacev( (IUnknown *)p, riid, ppi,
    &IID_IBravo, (ComboxUnknown_t *)p,
    &IID_ICharlie, (ComboxUnknown_t *)p + 1,
    &IID_IAlpha, p->alpha, NULL );
}

static STDMETHODIMP Bravo_QueryInterface( IBravo *pi, REFIID riid, void **ppi )
{
  return QueryInterface( combox_instance( pi ), riid, ppi );
}

static STDMETHODIMP Charlie_QueryInterface( ICharlie *pi, REFIID riid, void **ppi )
{
  return QueryInterface( combox_instance( pi ), riid, ppi );
}

static IBravoVtbl BravoVtbl = { Bravo_QueryInterface, 0, 0, BravoPush };
static ICharlieVtbl CharlieVtbl = { Charlie_QueryInterface, 0, 0, CharlieTest };
static combox_t combox = { &CLSID_Delta, 2, { &IID_IBravo, &IID_ICharlie }, { &BravoVtbl, &CharlieVtbl }, sizeof( Delta ), init, cleanup };

STDAPI Delta_GetClassObject( REFCLSID rclsid, REFIID riid, LPVOID *ppi ) { return ComboxGetClassObject( rclsid, riid, ppi ); }
STDAPI Delta_CreateInstance( REFCLSID rclsid, REFIID riid, LPVOID *ppi ) { return ComboxCreateInstance( rclsid, riid, ppi ); }
