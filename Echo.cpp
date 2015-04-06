#include <stdio.h>

#include <Echo.h>

#define COMBOX_CLASS Echo
#define COMBOX_INTERFACE IEcho
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
  STDMETHOD_( ULONG, AddRef )() { return Unk.AddRef(); }
  STDMETHOD_( ULONG, Release )() { return Unk.Release( this ); }

  STDMETHOD( EchoSync )( const int a );
};

Echo::Echo()
{
  printf( "%s(%d): Echo\n", __FILE__, __LINE__ );
  Internal = 55;
}

Echo::~Echo()
{
  printf( "%s(%d): ~Echo\n", __FILE__, __LINE__ );
}

STDMETHODIMP Echo::EchoSync( const int a )
{
  printf( "%s(%d): EchoSync( %d )\n", __FILE__, __LINE__, a );
  Internal = a;
  return S_OK;
}
