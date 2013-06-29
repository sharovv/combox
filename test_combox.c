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

  if( Alpha_CreateInstance( &CLSID_Alpha, &IID_IAlpha, (void **)&alpha ) != S_OK )
    return printf( "%s(%d) Alpha_CreateInstance failed\n", __FILE__, __LINE__ );

  alpha->lpVtbl->AlphaSet( alpha, 66 );

  alpha->lpVtbl->Release( alpha );

  if( Delta_CreateInstance( &CLSID_Delta, &IID_IBravo, (void **)&bravo ) != S_OK )
    return printf( "%s(%d) Delta_CreateInstance failed\n", __FILE__, __LINE__ );

  bravo->lpVtbl->BravoPush( bravo, 77 );

  if( bravo->lpVtbl->QueryInterface( bravo, &IID_ICharlie, (void **)&charlie ) != S_OK )
    return printf( "%s(%d) QueryInterface failed\n", __FILE__, __LINE__ );

  charlie->lpVtbl->CharlieTest( charlie, 88 );

  if( bravo->lpVtbl->QueryInterface( bravo, &IID_IAlpha, (void **)&alpha ) != S_OK )
    return printf( "%s(%d) QueryInterface failed\n", __FILE__, __LINE__ );
  
  alpha->lpVtbl->AlphaSet( alpha, 166 );

  alpha->lpVtbl->Release( alpha );
  bravo->lpVtbl->Release( bravo );
  charlie->lpVtbl->Release( charlie );

  if( Delta_CreateInstance( &CLSID_Delta, &IID_ICharlie, (void **)&charlie ) != S_OK )
    return printf( "%s(%d) Delta_CreateInstance failed\n", __FILE__, __LINE__ );

  charlie->lpVtbl->CharlieTest( charlie, 99 );

  if( charlie->lpVtbl->QueryInterface( charlie, &IID_IBravo, (void **)&bravo ) != S_OK )
    return printf( "%s(%d) QueryInterface failed\n", __FILE__, __LINE__ );

  bravo->lpVtbl->BravoPush( bravo, 111 );
  bravo->lpVtbl->Release( bravo );
  charlie->lpVtbl->Release( charlie );

  if( Echo_CreateInstance( &CLSID_Echo, &IID_IEcho, (void **)&echo ) != S_OK )
    return printf( "%s(%d) Echo_CreateInstance failed\n", __FILE__, __LINE__ );
  echo->lpVtbl->EchoSync( echo, 66 );
  echo->lpVtbl->Release( echo );

  if( Foxtrot_CreateInstance( &CLSID_Foxtrot, &IID_IFoxtrot, (void **)&foxtrot ) != S_OK )
    return printf( "%s(%d) Foxtrot_CreateInstance failed\n", __FILE__, __LINE__ );
  foxtrot->lpVtbl->FoxtrotSum( foxtrot, 222, 333 );
  if( foxtrot->lpVtbl->QueryInterface( foxtrot, &IID_IEcho, (void **)&echo ) != S_OK )
    return printf( "%s(%d) Foxtrot::QueryInterface failed\n", __FILE__, __LINE__ );
  echo->lpVtbl->EchoSync( echo, 444 );
  echo->lpVtbl->Release( echo );
  foxtrot->lpVtbl->Release( foxtrot );

  return 0;
}
