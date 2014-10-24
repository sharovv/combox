#include <stdio.h>

#include <Foxtrot.h>
#include <IEcho.h>
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
  STDMETHOD_( unsigned long, AddRef )() { return Unk.AddRef(); }
  STDMETHOD_( unsigned long, Release )() { return Unk.Release( this ); }

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

STDAPI Foxtrot_GetClassObject( REFCLSID rclsid, REFIID riid, LPVOID *ppi ) { return ComboxGetClassObject<Foxtrot>( CLSID_Foxtrot, rclsid, riid, ppi ); }
STDAPI Foxtrot_CreateInstance( REFCLSID rclsid, REFIID riid, LPVOID *ppi ) { return ComboxCreateInstance<Foxtrot>( CLSID_Foxtrot, rclsid, riid, ppi ); }
