#include <stdio.h>

#include <Alpha.h>
#include <Delta.h>
#include <Echo.h>
#include <Foxtrot.h>

int main()
{
  IAlpha *alpha;
  IBravo *bravo;
  ICharlie *charlie;
  IEcho *echo;
  IFoxtrot *foxtrot;

  alpha = Alpha();
  alpha->AlphaSet( 66 );
  alpha->Release();

  if( Delta_CreateInstance( CLSID_Delta, IID_IBravo, (void **)&bravo ) != S_OK )
    return printf( "%s(%d) Delta_CreateInstance failed\n", __FILE__, __LINE__ );

  bravo->BravoPush( 77 );

  if( bravo->QueryInterface( IID_ICharlie, (void **)&charlie ) != S_OK )
    return printf( "%s(%d) QueryInterface failed\n", __FILE__, __LINE__ );

  charlie->CharlieTest( 88 );

  if( bravo->QueryInterface( IID_IAlpha, (void **)&alpha ) != S_OK )
    return printf( "%s(%d) QueryInterface failed\n", __FILE__, __LINE__ );
  
  alpha->AlphaSet( 166 );

  alpha->Release();
  bravo->Release();
  charlie->Release();

  if( Delta_CreateInstance( CLSID_Delta, IID_ICharlie, (void **)&charlie ) != S_OK )
    return printf( "%s(%d) Delta_CreateInstance failed\n", __FILE__, __LINE__ );

  charlie->CharlieTest( 99 );

  if( charlie->QueryInterface( IID_IBravo, (void **)&bravo ) != S_OK )
    return printf( "%s(%d) QueryInterface failed\n", __FILE__, __LINE__ );

  bravo->BravoPush( 111 );
  bravo->Release();
  charlie->Release();

  if( Echo_CreateInstance( CLSID_Echo, IID_IEcho, (void **)&echo ) != S_OK )
    return printf( "%s(%d) Echo_CreateInstance failed\n", __FILE__, __LINE__ );
  echo->EchoSync( 66 );
  echo->Release( );

  if( Foxtrot_CreateInstance( CLSID_Foxtrot, IID_IFoxtrot, (void **)&foxtrot ) != S_OK )
    return printf( "%s(%d) Foxtrot_CreateInstance failed\n", __FILE__, __LINE__ );
  foxtrot->FoxtrotSum( 222, 333 );
  if( foxtrot->QueryInterface( IID_IEcho, (void **)&echo ) != S_OK )
    return printf( "%s(%d) Foxtrot::QueryInterface failed\n", __FILE__, __LINE__ );
  echo->EchoSync(444 );
  echo->Release();
  foxtrot->Release();

  return 0;
}
