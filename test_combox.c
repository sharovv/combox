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

  alpha = Alpha_new();
  alpha->lpVtbl->AlphaSet( alpha, 66 );
  alpha->lpVtbl->Release( alpha );

  bravo = Delta_new();

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

  echo = Echo_new();
  echo->lpVtbl->EchoSync( echo, 66 );
  echo->lpVtbl->Release( echo );

  foxtrot = Foxtrot_new();
  foxtrot->lpVtbl->FoxtrotSum( foxtrot, 222, 333 );
  if( foxtrot->lpVtbl->QueryInterface( foxtrot, &IID_IEcho, (void **)&echo ) != S_OK )
    return printf( "%s(%d) Foxtrot::QueryInterface failed\n", __FILE__, __LINE__ );
  echo->lpVtbl->EchoSync( echo, 444 );
  echo->lpVtbl->Release( echo );
  foxtrot->lpVtbl->Release( foxtrot );

  return 0;
}
