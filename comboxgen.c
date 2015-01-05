#include <stdio.h>

#if defined( _WIN32 )
#include <windows.h>
static void create_guid( GUID *id ) { CoCreateGuid( id ); }
#else
typedef struct  _GUID
{
  unsigned int Data1;
  unsigned short Data2;
  unsigned short Data3;
  unsigned char Data4[ 8 ];
} GUID;
#include <uuid/uuid.h>
static void create_guid( GUID *id ) { uuid_generate( (unsigned char*)id ); }
#endif

void print_interface( const char *name_interface )
{
  GUID id;

  create_guid( &id );
  printf( "#ifndef _%s_h\n", name_interface );
  printf( "#define _%s_h\n\n", name_interface );
  printf( "#include <objbase.h>\n\n" );
  printf( "/* {%08lX-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X} */\n",
    id.Data1, id.Data2, id.Data3,
    id.Data4[0], id.Data4[1], id.Data4[2], id.Data4[3],
    id.Data4[4], id.Data4[5], id.Data4[6], id.Data4[7] );
  printf( "static const GUID IID_%s = { 0x%08lX, 0x%04X, 0x%04x, { 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X } };\n\n", 
    name_interface,
    id.Data1, id.Data2, id.Data3,
    id.Data4[0], id.Data4[1], id.Data4[2], id.Data4[3],
    id.Data4[4], id.Data4[5], id.Data4[6], id.Data4[7] );
  printf( "#undef INTERFACE\n" );
  printf( "#define INTERFACE %s\n\n", name_interface );
  printf( "DECLARE_INTERFACE( %s )\n{\n", name_interface );
  printf( "  STDMETHOD( QueryInterface )( THIS_ REFIID riid, void **ppi ) PURE;\n" );
  printf( "  STDMETHOD_( ULONG, AddRef )( THIS ) PURE;\n" );
  printf( "  STDMETHOD_( ULONG, Release )( THIS ) PURE;\n\n" );
  printf( "  STDMETHOD( Method1 )( THIS_ const int ) PURE;\n};\n\n" );
  printf( "#endif\n" );
}

void print_header( const char *name_class, const char *name_interface )
{
  GUID id;

  create_guid( &id );
  printf( "#ifndef _%s_h\n", name_class );
  printf( "#define _%s_h\n\n", name_class );
  printf( "#include <%s.h>\n\n", name_interface );
  printf( "/* {%08lX-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X} */\n",
    id.Data1, id.Data2, id.Data3,
    id.Data4[0], id.Data4[1], id.Data4[2], id.Data4[3],
    id.Data4[4], id.Data4[5], id.Data4[6], id.Data4[7] );
  printf( "static const GUID CLSID_%s = { 0x%08lX, 0x%04X, 0x%04x, { 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X } };\n\n", 
    name_class,
    id.Data1, id.Data2, id.Data3,
    id.Data4[0], id.Data4[1], id.Data4[2], id.Data4[3],
    id.Data4[4], id.Data4[5], id.Data4[6], id.Data4[7] );
  printf( "STDAPI %s_GetClassObject( REFCLSID rclsid, REFIID riid, LPVOID *ppi );\n", name_class );
  printf( "STDAPI %s_CreateInstance( REFCLSID rclsid, REFIID riid, LPVOID *ppi );\n", name_class );
  printf( "STDAPI_( ULONG ) %s_ServerCount( const int i );\n", name_class );
  printf( "STDAPI_( %s * ) %s_new( void );\n\n", name_interface, name_class );
  printf( "#endif\n" );
}

void print_c_class( const char *name_class, const char *name_interface )
{
  printf( "#include <%s.h>\n\n", name_class );
  printf( "#define COMBOX_CLASS %s\n", name_class );
  printf( "#define COMBOX_INTERFACE %s\n", name_interface );
  printf( "#include <combox.h>\n" );
  printf( "\n" );
  printf( "typedef struct _%s\n", name_class );
  printf( "{\n" );
  printf( "  COMBOX_VTBL;\n" );
  printf( "  int Internal;\n" );
  printf( "} %s;\n", name_class );
  printf( "\n" );
  printf( "static HRESULT init( IUnknown *pi )\n" );
  printf( "{\n" );
  printf( "  %s *p = (%s *)pi;\n", name_class, name_class );
  printf( "  p->Internal = 0;\n" );
  printf( "  return S_OK;\n" );
  printf( "}\n" );
  printf( "\n" );
  printf( "static void cleanup( IUnknown *pi )\n" );
  printf( "{\n" );
  printf( "  %s *p = (%s *)pi;\n", name_class, name_class );
  printf( "  p->Internal = 0;\n" );
  printf( "}\n" );
  printf( "\n" );
  printf( "static STDMETHODIMP Method1( %s *pi, const int a )\n", name_interface );
  printf( "{\n" );
  printf( "  %s *p = combox_instance( pi );\n", name_class );
  printf( "  p->Internal = a;\n" );
  printf( "  return S_OK;\n" );
  printf( "}\n" );
  printf( "\n" );
  printf( "static %sVtbl Vtbl = { 0, 0, 0, Method1 };\n", name_interface );
  printf( "static combox_t combox = { &CLSID_%s, 1, { &IID_%s }, { &Vtbl }, sizeof( %s ), init, cleanup };\n", name_class, name_interface, name_class );
}

void print_cpp_class( const char *name_class, const char *name_interface )
{
  printf( "#include <%s.h>\n\n", name_class );
  printf( "#define COMBOX_CLASS %s\n", name_class );
  printf( "#define COMBOX_INTERFACE %s\n", name_interface );
  printf( "#include <combox.h>\n" );
  printf( "\n" );
  printf( "class %s: public %s\n", name_class, name_interface );
  printf( "{\n" );
  printf( "private:\n" );
  printf( "  ComboxUnknown <%s> Unk;\n", name_class );
  printf( "  int Internal;\n" );
  printf( "public:\n" );
  printf( "  %s();\n", name_class );
  printf( "  ~%s();\n", name_class );
  printf( "\n" );
  printf( "  STDMETHOD( QueryInterface )( REFIID riid, void **ppi ) { return Unk.QueryInterface( riid, ppi, IID_%s, static_cast<%s *>(this) ); }\n", name_interface, name_interface );
  printf( "  STDMETHOD_( ULONG, AddRef )() { return Unk.AddRef(); }\n" );
  printf( "  STDMETHOD_( ULONG, Release )() { return Unk.Release( this ); }\n" );
  printf( "\n" );
  printf( "  STDMETHOD( Method1 )( const int a );\n" );
  printf( "};\n" );
  printf( "\n" );
  printf( "%s::%s()\n", name_class, name_class );
  printf( "{\n" );
  printf( "  Internal = 55;\n" );
  printf( "}\n" );
  printf( "\n" );
  printf( "%s::~%s()\n", name_class, name_class );
  printf( "{\n" );
  printf( "}\n" );
  printf( "\n" );
  printf( "STDMETHODIMP %s::Method1( const int a )\n", name_class );
  printf( "{\n" );
  printf( "  Internal = a;\n" );
  printf( "  return S_OK;\n" );
  printf( "}\n" );
}

int main( int argc, char *argv[] )
{
  int err = 1;

  if( argc > 1 )
  {
    if( strcmp( argv[1], "interface" ) == 0 && argc == 3 )
    {
      print_interface( argv[2] ); err = 0;
    }
    else if( strcmp( argv[1], "header" ) == 0 && argc == 4 )
    {
      print_header( argv[2], argv[3] ); err = 0;
    }
    else if( strcmp( argv[1], "c-class" ) == 0 && argc == 4 )
    {
      print_c_class( argv[2], argv[3] ); err = 0;
    }
    else if( strcmp( argv[1], "cpp-class" ) == 0 && argc == 4 )
    {
      print_cpp_class( argv[2], argv[3] ); err = 0;
    }
  }
  if( err )
  {
    fprintf( stderr,
      "Usage:\n\n"
      "comboxgen interface <Interface>\n"
      "comboxgen header <Class> <Interface>\n"
      "comboxgen c-class <Class> <Interface>\n"
      "comboxgen cpp-class <Class> <Interface>\n\n" );
  }
  return err;
}
