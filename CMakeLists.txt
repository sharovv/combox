cmake_minimum_required( VERSION 2.6 )
project( combox )

if( 0 )
if( MSVC )
  set( CFLAGS_RELEASE "/nologo /MT /W3 /GX /O2 /FD /c /D_CRT_SECURE_NO_WARNINGS /D_CRT_SECURE_NO_DEPRECATE" )
  set( CFLAGS_DEBUG "/nologo /MTd /W3 /Gm /GX /Zi /Od /FD /GZ /c /D_CRT_SECURE_NO_WARNINGS /D_CRT_SECURE_NO_DEPRECATE" )
else( MSVC )
  set( CFLAGS_RELEASE "-Wall -O2 -fno-strict-aliasing -fomit-frame-pointer -s" )
  set( CFLAGS_DEBUG "-Wall -g" )
endif( MSVC )

set( CMAKE_C_FLAGS_RELEASE ${CFLAGS_RELEASE} )
set( CMAKE_C_FLAGS_DEBUG ${CFLAGS_DEBUG} )
set( CMAKE_CXX_FLAGS_RELEASE ${CFLAGS_RELEASE} )
set( CMAKE_CXX_FLAGS_DEBUG ${CFLAGS_DEBUG} )
endif( 0 )

include_directories( ${CMAKE_SOURCE_DIR} )
if( NOT WIN32 )
include_directories( ${CMAKE_SOURCE_DIR}/objbase )
endif( NOT WIN32 )

add_library( alpha Alpha.c Delta.c Echo.cpp Foxtrot.cpp )

add_executable( test_combox test_combox.c )
target_link_libraries( test_combox alpha )

add_executable( test_combox_cpp test_combox_cpp.cpp )
target_link_libraries( test_combox_cpp alpha )
