#include <stdio.h>

#include <Echo.h>
#include <combox.h>

class Echo: public IEcho
{
private:
  ComboxUnknown <Echo> Unk;
  int Internal;
public:
  Echo();
  ~Echo();

  STDMETHOD( QueryInterface )( REFIID riid, void **ppi ) { return Unk.QueryInterface( riid, ppi, IID_IEcho, static_cast<IEcho *>(this) ); }
  STDMETHOD_( unsigned long, AddRef )() { return Unk.AddRef(); }
  STDMETHOD_( unsigned long, Release )() { return Unk.Release( this ); }

  STDMETHOD( EchoSync )( const int a );
};

Echo::Echo()
{
  printf( "%s(%d): %s\n", __FILE__, __LINE__, __FUNCTION__ );
  Internal = 55;
}

Echo::~Echo()
{
  printf( "%s(%d): %s\n", __FILE__, __LINE__, __FUNCTION__ );
}

STDMETHODIMP Echo::EchoSync( const int a )
{
  printf( "%s(%d): %s( %d )\n", __FILE__, __LINE__, __FUNCTION__, a );
  Internal = a;
  return S_OK;
}

STDAPI Echo_GetClassObject( REFCLSID rclsid, REFIID riid, LPVOID *ppi ) { return ComboxGetClassObject<Echo>( CLSID_Echo, rclsid, riid, ppi ); }
STDAPI Echo_CreateInstance( REFCLSID rclsid, REFIID riid, LPVOID *ppi ) { return ComboxCreateInstance<Echo>( CLSID_Echo, rclsid, riid, ppi ); }
STDAPI_( unsigned long ) Echo_ServerCount( const int i ) { return ServerCount(i); }
