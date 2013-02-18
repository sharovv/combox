#include <IAlpha.h>

// {A837CA31-8ADE-48c4-A543-FAEBE363EF6B}
static const GUID CLSID_Alpha = { 0xa837ca31, 0x8ade, 0x48c4, { 0xa5, 0x43, 0xfa, 0xeb, 0xe3, 0x63, 0xef, 0x6b } };

STDAPI Alpha_GetClassObject( REFCLSID rclsid, REFIID riid, LPVOID *ppi );
STDAPI Alpha_CreateInstance( REFCLSID rclsid, REFIID riid, LPVOID *ppi );
