#include <stdio.h>

#include <Foxtrot.h>
#include <IEcho.h>

#define COMBOX_CLASS Foxtrot
#include <combox.h>

class Foxtrot: public IFoxtrot, IEcho
{
private:
  ComboxUnknown <Foxtrot> Unk;
  int Internal;
public:
  Foxtrot();
  ~Foxtrot();

  STDMETHOD( QueryInterface )( REFIID riid, void **ppi )
  {
    return Unk.QueryInterfacev( riid, ppi,
      &IID_IFoxtrot, static_cast<IFoxtrot *>(this),
      &IID_IEcho, static_cast<IEcho *>(this), NULL );
  }
  STDMETHOD_( ULONG, AddRef )() { return Unk.AddRef(); }
  STDMETHOD_( ULONG, Release )() { return Unk.Release( this ); }

  STDMETHOD( EchoSync ) ( THIS_ const int a );
  STDMETHOD( FoxtrotSum ) ( THIS_ const int a, const int b );
};

Foxtrot::Foxtrot()
{
  printf( "%s(%d): %s\n", __FILE__, __LINE__, __FUNCTION__ );
  Internal = 55;
}

Foxtrot::~Foxtrot()
{
  printf( "%s(%d): %s\n", __FILE__, __LINE__, __FUNCTION__ );
}

STDMETHODIMP Foxtrot::EchoSync( const int a )
{
  printf( "%s(%d): %s( %d )\n", __FILE__, __LINE__, __FUNCTION__, a );
  Internal = a;
  return S_OK;
}

STDMETHODIMP Foxtrot::FoxtrotSum( const int a, const int b )
{
  printf( "%s(%d): %s( %d, %d )\n", __FILE__, __LINE__, __FUNCTION__, a, b );
  Internal = a + b;
  return S_OK;
}

STDAPI_( IFoxtrot * ) Foxtrot_new( void ) { return (IFoxtrot *)ComboxInstance<Foxtrot>( CLSID_Foxtrot, IID_IFoxtrot ); }
