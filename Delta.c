#include <stdio.h>

#include <Delta.h>
#include <Alpha.h>

#define COMBOX_CLASS Delta
#define COMBOX_INTERFACE IBravo
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

  p->alpha = Alpha_new();
  printf( "%s(%d): init 0x%08lX\n", __FILE__, __LINE__, (unsigned long)p );
  p->Internal = 55;
  return S_OK;
}

static void cleanup( IUnknown *pi )
{
  Delta *p = (Delta *)pi;

  p->alpha->lpVtbl->Release( p->alpha );
  printf( "%s(%d): cleanup\n", __FILE__, __LINE__ );
}

static STDMETHODIMP BravoPush( IBravo *pi, const int a )
{
  Delta *p = (Delta *)pi;

  printf( "%s(%d): BravoPush( 0x%08lX, %d )\n", __FILE__, __LINE__, (unsigned long)p, a );
  p->Internal = a;
  return S_OK;
}

static STDMETHODIMP CharlieTest( ICharlie *pi, const int a )
{
  Delta *p = combox_instance( pi );

  printf( "%s(%d): CharlieTest( 0x%08lX, %d )\n", __FILE__, __LINE__, (unsigned long)p, a );
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
