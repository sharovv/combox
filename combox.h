#ifndef _combox_h
#define _combox_h

#include <objbase.h>
#include <stdlib.h>
#include <stdarg.h>

#if !defined( COMBOX_VTBL_ONLY ) || (defined( __cplusplus ) && !defined( CINTERFACE ))

static unsigned int ServerCount( const int delta )
{
  static unsigned int server_count = 0UL;
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

#endif

#if defined( __cplusplus ) && !defined( CINTERFACE )

template <typename TC> struct ComboxUnknown
{
  unsigned int RefCount;
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
  STDMETHOD_( unsigned int, AddRef )()
  {
    ServerCount( 1 );
    return ++RefCount;
  }
  STDMETHOD_( unsigned int, Release )( TC *instance )
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
  STDMETHOD_( unsigned int, AddRef )() { return Unk.AddRef(); }
  STDMETHOD_( unsigned int, Release )() { return Unk.Release( this ); }

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

template <typename CF>
static void *ComboxInstance( REFCLSID rclsid, REFIID riid )
{
  void *pv;
  return ComboxCreateInstance<CF>( rclsid, rclsid, riid, (LPVOID *)&pv ) == S_OK ? pv: (void *)0;
}

#define COMBOX_CONCAT_( a, b ) a##b
#define COMBOX_CONCAT( a, b ) COMBOX_CONCAT_( a, b )

#if defined( COMBOX_CLASS )
class COMBOX_CLASS;
STDAPI COMBOX_CONCAT( COMBOX_CLASS, _GetClassObject )( REFCLSID rclsid, REFIID riid, LPVOID *ppi ) { return ComboxGetClassObject<COMBOX_CLASS>( COMBOX_CONCAT( CLSID_, COMBOX_CLASS ), rclsid, riid, ppi ); }
STDAPI COMBOX_CONCAT( COMBOX_CLASS, _CreateInstance )( REFCLSID rclsid, REFIID riid, LPVOID *ppi ) { return ComboxCreateInstance<COMBOX_CLASS>( COMBOX_CONCAT( CLSID_, COMBOX_CLASS ), rclsid, riid, ppi ); }
STDAPI_( unsigned int ) COMBOX_CONCAT( COMBOX_CLASS, _ServerCount )( const int i ) { return ServerCount(i); }
#endif

#else /* pure C implementation */

#include <string.h>

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
  unsigned int RefCount;
} ComboxUnknown_t;

#define COMBOX_VTBL ComboxUnknown_t ComboxInterface[ COMBOX_MAX_INTERFACE ]

typedef struct _ComboxClassFactory_t
{
  IClassFactoryVtbl *lpVtbl;
  unsigned int RefCount;
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
  int i;
  ComboxUnknown_t *u;

  if( IsEqualGUID( riid, &IID_IUnknown ) )
  {
    pi->lpVtbl->AddRef( pi );
    *ppi = pi;
    return S_OK;
  }
  for( i = 0; i < combox.num_interfaces; i++ )
  {
    if( IsEqualGUID( riid, combox.interface_id[i] ) )
    {
      pi->lpVtbl->AddRef( pi );
      u = combox_instance( pi );
      *ppi = u + i;
      return S_OK;
    }
  }
  /* Stub to supress "define but not used" warning */
  {
    IUnknown *iunk;
    static const GUID iff =
    {
      0xFFFFFFFF, 0xFFFF, 0xFFFF, { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }
    };
    if( Combox_QueryInterfacev( pi, &iff, (void **)&iunk, NULL ) == S_OK )
    {
      iunk->lpVtbl->Release( iunk );
    }
  }
  return E_NOINTERFACE;
}

static STDMETHODIMP_( unsigned int ) ComboxUnknown_AddRef( IUnknown *pi )
{
  ComboxUnknown_t *p = combox_instance( pi );

  p->RefCount++;
  ServerCount( 1 );
  return p->RefCount;
}

static STDMETHODIMP_( unsigned int ) ComboxUnknown_Release( IUnknown *pi )
{
  ComboxUnknown_t *p = combox_instance( pi );
  unsigned int i = --(p->RefCount);

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

static STDMETHODIMP_( unsigned int ) ComboxClassFactory_AddRef( IClassFactory *pi )
{
  ComboxClassFactory_t *p = (ComboxClassFactory_t *)pi;

  p->RefCount++;
  ServerCount( 1 );
  return p->RefCount;
}

static STDMETHODIMP_( unsigned int ) ComboxClassFactory_Release( IClassFactory *pi )
{
  ComboxClassFactory_t *p = (ComboxClassFactory_t *)pi;
  unsigned int i = --(p->RefCount);

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

static void *ComboxInstance( void )
{
  void *pv = (void *)0;
  return (ComboxCreateInstance( combox.class_id, combox.interface_id[0], (LPVOID *)&pv ) == S_OK) ? pv: (void *)0;
}

#define COMBOX_CONCAT_( a, b ) a##b
#define COMBOX_CONCAT( a, b ) COMBOX_CONCAT_( a, b )

#if defined( COMBOX_CLASS )
STDAPI COMBOX_CONCAT( COMBOX_CLASS, _GetClassObject )( REFCLSID rclsid, REFIID riid, LPVOID *ppi ) { return ComboxGetClassObject( rclsid, riid, ppi ); }
STDAPI COMBOX_CONCAT( COMBOX_CLASS, _CreateInstance )( REFCLSID rclsid, REFIID riid, LPVOID *ppi ) { return ComboxCreateInstance( rclsid, riid, ppi ); }
STDAPI_( unsigned int ) COMBOX_CONCAT( COMBOX_CLASS, _ServerCount )( const int i ) { return ServerCount(i); }
#endif

#endif

#endif
