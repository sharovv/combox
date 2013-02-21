#ifndef _combox_h
#define _combox_h

#include <objbase.h>
#include <stdlib.h>
#include <stdarg.h>

static unsigned long ServerCount( const int delta )
{
  static unsigned long server_count = 0UL;
  if( delta < -1 || delta > 1 )
  {
    server_count = 0UL;
  }
  else
  {
    server_count += delta;
  }
  return server_count;
}

#if defined( __cplusplus ) && !defined( CINTERFACE )

template <typename TC> struct ComboxUnknown
{
  unsigned long RefCount;
  ComboxUnknown() { RefCount = 1; }
  STDMETHOD( QueryInterface )( REFIID riid, void **ppi, REFIID iid, void *iface ) 
  { 
    if( IsEqualGUID( riid, IID_IUnknown ) || IsEqualGUID( riid, iid ) ) 
    {
      *ppi = iface;
      AddRef();
      return S_OK; 
    }
    return E_NOINTERFACE;
  }
  STDMETHOD( QueryInterfacev )( REFIID riid, void **ppi, ... ) 
  { 
    IUnknown *iface;
    va_list args;
    const GUID *iid;
    HRESULT r = E_NOINTERFACE;

    va_start( args, ppi );
    while( (iid = va_arg( args, const GUID * )) != NULL )
    {
      if( (iface = va_arg( args, IUnknown *)) != NULL )
      {
        if( IsEqualGUID( riid, *iid ) || IsEqualGUID( riid, IID_IUnknown ) )
        {
          AddRef();
          *ppi = (void *)iface;
          r = S_OK;
          break;
        }
      }
      else
      {
        break;
      }
    }
    va_end( args );
    return r;
  }
  STDMETHOD_( unsigned long, AddRef )()
  {
    ServerCount( 1 );
    return ++RefCount;
  }
  STDMETHOD_( unsigned long, Release )( TC *instance )
  {
    ServerCount( -1 );
    if( --RefCount == 0 )
      delete instance;
    return RefCount;
  }
};

template <typename TC> class ComboxClassFactory: public IClassFactory
{
private:
  ComboxUnknown <ComboxClassFactory> Unk;
public:
  ComboxClassFactory() { ServerCount( -(int)ServerCount( 0 ) ); }
  STDMETHOD( QueryInterface )( REFIID riid, void **ppi ) { return Unk.QueryInterface( riid, ppi, IID_IClassFactory, static_cast<IClassFactory *>(this) ); }
  STDMETHOD_( unsigned long, AddRef )() { return Unk.AddRef(); }
  STDMETHOD_( unsigned long, Release )() { return Unk.Release( this ); }

  STDMETHOD( CreateInstance )( THIS_ IUnknown *unk_outer, REFIID riid, void **ppi )
  {
    TC *pn;
    HRESULT r;

    if( unk_outer != 0 )
      return CLASS_E_NOAGGREGATION;

    pn = new TC;

    if( pn == NULL )
      return E_OUTOFMEMORY;

    if( (r = pn->QueryInterface( riid, ppi )) != S_OK )
    {
      delete pn;
    }
    else
    {
      pn->Release();
    }
    return r;
  }
  STDMETHOD( LockServer )( THIS_ BOOL flock )
  {
    if( flock != 0 )
      ServerCount(1);
    else
      ServerCount(-1);
    return S_OK;
  }
};

template <typename CF> 
static HRESULT ComboxGetClassObject( REFCLSID clsid, REFCLSID rclsid, REFIID riid, LPVOID *ppi )
{
  if( IsEqualGUID( rclsid, clsid ) )
  {
    static ComboxClassFactory<CF> cf;
    return cf.QueryInterface( riid, ppi );
  }
  return CLASS_E_CLASSNOTAVAILABLE;
}

template <typename CF>
static HRESULT ComboxCreateInstance( REFCLSID clsid, REFCLSID rclsid, REFIID riid, LPVOID *ppi )
{
  IClassFactory *icf = NULL;
  HRESULT r;

  if( (r = ComboxGetClassObject<CF>( clsid, rclsid, IID_IClassFactory, (void **)&icf )) != S_OK )
    return r;
  r = icf->CreateInstance( NULL, riid, ppi );
  icf->Release();
  return r;
}

#else /* pure C implementation */

#define COMBOX_MAX_INTERFACE 4
#define COMBOX_FIRST_INTERFACE 0x80000000UL

typedef struct _combox_t
{
  const GUID *class_id;
  int num_interfaces;
  const GUID *interface_id[ COMBOX_MAX_INTERFACE ];
  void *vtbl[ COMBOX_MAX_INTERFACE ];
  int size;
  HRESULT (*init)( IUnknown *pi );
  void (*cleanup)( IUnknown *pi );
} combox_t;

static combox_t combox;

typedef struct _ComboxUnknown_t
{
  IUnknownVtbl *lpVtbl;
  unsigned long RefCount;
} ComboxUnknown_t;

#define COMBOX_VTBL( iface ) iface##Vtbl *lpVtbl; unsigned long RefCount; ComboxUnknown_t ComboxInterface[ COMBOX_MAX_INTERFACE - 1 ]

typedef struct _ComboxClassFactory_t
{
  IClassFactoryVtbl *lpVtbl;
  unsigned long RefCount;
} ComboxClassFactory_t;

static void *combox_instance( void *pi )
{
  ComboxUnknown_t *p = (ComboxUnknown_t *)pi;

  if( p->RefCount >= COMBOX_FIRST_INTERFACE && p->RefCount < (COMBOX_FIRST_INTERFACE + combox.num_interfaces) )
  {
    p = p - (p->RefCount - COMBOX_FIRST_INTERFACE);
  }
  return p;
}

static HRESULT Combox_QueryInterfacev( IUnknown *pi, REFIID riid, void **ppi, ... )
{
  IUnknown *iface;
  va_list args;
  const GUID *iid;
  HRESULT r = E_NOINTERFACE;

  va_start( args, ppi );
  while( (iid = va_arg( args, const GUID * )) != NULL )
  {
    if( (iface = va_arg( args, IUnknown *)) != NULL )
    {
      if( IsEqualGUID( riid, iid ) || IsEqualGUID( riid, &IID_IUnknown ) )
      {
        iface->lpVtbl->AddRef( iface );
        *ppi = iface;
        r = S_OK;
        break;
      }
    }
    else
    {
      break;
    }
  }
  va_end( args );
  return r;
}


static STDMETHODIMP ComboxUnknown_QueryInterface( IUnknown *pi, REFIID riid, void **ppi )
{
  return Combox_QueryInterfacev( pi, riid, ppi,
    combox.interface_id[0], pi,
    combox.num_interfaces > 1 ? combox.interface_id[1]: NULL, combox.num_interfaces > 1 ? pi: NULL,
    combox.num_interfaces > 2 ? combox.interface_id[2]: NULL, combox.num_interfaces > 2 ? pi: NULL,
    combox.num_interfaces > 3 ? combox.interface_id[3]: NULL, combox.num_interfaces > 3 ? pi: NULL, NULL );
}

static STDMETHODIMP_( unsigned long ) ComboxUnknown_AddRef( IUnknown *pi )
{
  ComboxUnknown_t *p = combox_instance( pi );

  p->RefCount++;
  ServerCount( 1 );
  return p->RefCount;
}

static STDMETHODIMP_( unsigned long ) ComboxUnknown_Release( IUnknown *pi )
{
  ComboxUnknown_t *p = combox_instance( pi );
  unsigned long i = --(p->RefCount);

  if( i == 0 )
  {
    if( combox.cleanup != 0 )
      combox.cleanup( (IUnknown *)p );
    free( p );
  }
  ServerCount( -1 );
  return i;
}

static STDMETHODIMP ComboxClassFactory_QueryInterface( IClassFactory *pi, REFIID riid, void **ppi )
{
  if( IsEqualGUID( riid, &IID_IUnknown ) || IsEqualGUID( riid, &IID_IClassFactory ) )
  {
    *ppi = pi;
  }
  else
  {
    return E_NOINTERFACE;
  }
  pi->lpVtbl->AddRef( pi );
  return S_OK;
}

static STDMETHODIMP_( unsigned long ) ComboxClassFactory_AddRef( IClassFactory *pi )
{
  ComboxClassFactory_t *p = (ComboxClassFactory_t *)pi;

  p->RefCount++;
  ServerCount( 1 );
  return p->RefCount;
}

static STDMETHODIMP_( unsigned long ) ComboxClassFactory_Release( IClassFactory *pi )
{
  ComboxClassFactory_t *p = (ComboxClassFactory_t *)pi;
  unsigned long i = --(p->RefCount);

  ServerCount( -1 );
  return i;
}

static STDMETHODIMP ComboxClassFactory_CreateInstance( IClassFactory *pi, IUnknown *unk_outer, REFIID riid, void **ppi )
{
  ComboxUnknown_t *pu;
  IUnknown *pn;
  HRESULT r;
  int i;

  *ppi = 0;

  if( unk_outer != 0 )
    return CLASS_E_NOAGGREGATION;

  if( (pn = malloc( combox.size )) == NULL )
    return E_OUTOFMEMORY;

  memset( pn, 0, combox.size );

  for( i = 0, pu = (ComboxUnknown_t *)pn; i < combox.num_interfaces; i++, pu++ )
  {
    pu->lpVtbl = combox.vtbl[i];
    if( i > 0 )
    {
      pu->RefCount = COMBOX_FIRST_INTERFACE + i;
    }
    else
    {
      pu->RefCount = 0;
    }

    if( pu->lpVtbl->QueryInterface == 0 )
      pu->lpVtbl->QueryInterface = ComboxUnknown_QueryInterface;
    
    if( pu->lpVtbl->AddRef == 0 )
      pu->lpVtbl->AddRef = ComboxUnknown_AddRef;
    
    if( pu->lpVtbl->Release == 0 )
      pu->lpVtbl->Release = ComboxUnknown_Release;
  }
  
  if( (r = pn->lpVtbl->QueryInterface( pn, riid, ppi )) != S_OK )
  {
    free( pn );
    return r;
  }

  if( combox.init != 0 )
  {
    if( (r = combox.init( pn )) != S_OK )
    {
      pn->lpVtbl->Release( pn );
      pn = 0;
    }
  }
  return r;
}

static STDMETHODIMP ComboxClassFactory_LockServer( IClassFactory *pi, int flock )
{
  if( flock != 0 )
    ServerCount( 1 );
  else
    ServerCount( -1 );
  return S_OK;
}

static IClassFactoryVtbl ComboxClassFactoryVtbl = 
{
  ComboxClassFactory_QueryInterface,
  ComboxClassFactory_AddRef,
  ComboxClassFactory_Release,
  ComboxClassFactory_CreateInstance,
  ComboxClassFactory_LockServer
};

static ComboxClassFactory_t ComboxClassFactory = { &ComboxClassFactoryVtbl, 0 };

static HRESULT STDMETHODCALLTYPE ComboxGetClassObject( REFCLSID rclsid, REFIID riid, LPVOID *ppi )
{
  *ppi = 0;

  if( IsEqualGUID( rclsid, combox.class_id ) )
  {
    return ComboxClassFactory.lpVtbl->QueryInterface( (IClassFactory *)&ComboxClassFactory, riid, ppi );
  }
  return CLASS_E_CLASSNOTAVAILABLE;
}

static HRESULT STDMETHODCALLTYPE ComboxCreateInstance( REFCLSID rclsid, REFIID riid, LPVOID *ppi )
{
  IClassFactory *icf = NULL;
  HRESULT r;
  void *pv = NULL;

  if( (r = ComboxGetClassObject( rclsid, &IID_IClassFactory, (void **)&icf )) != S_OK )
    return r;

  r = icf->lpVtbl->CreateInstance( icf, NULL, riid, &pv );

  icf->lpVtbl->Release( icf );

  *ppi = pv;

  return r;
}

#endif 

#endif
