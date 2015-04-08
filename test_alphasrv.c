#include <stdio.h>

#include <Alpha.h>

STDAPI DllGetClassObject( REFCLSID clsid, REFIID iid, void **ppi );

int main()
{
  IAlpha *alpha;
  IClassFactory *factory;

  DllGetClassObject( &CLSID_Alpha, &IID_IClassFactory, (void **)&factory );
  factory->lpVtbl->CreateInstance( factory, NULL, &IID_IAlpha, (void **)&alpha );
  factory->lpVtbl->Release( factory );

  alpha->lpVtbl->AlphaSet( alpha, 66 );
  alpha->lpVtbl->Release( alpha );

  return 0;
}
