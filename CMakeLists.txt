cmake_minimum_required( VERSION 2.6 )
project( combox )

if( MSVC )
  set( CFLAGS_RELEASE "/MT /W3 /O2 /wd4996" )
  set( CFLAGS_DEBUG  "/MTd /W3 /Od /wd4996 /Zi" )
  set( CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/install )
else( MSVC )
  if( NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE RELEASE )
  endif( NOT CMAKE_BUILD_TYPE )
  set( CFLAGS_RELEASE "-O2 -Wall -fno-strict-aliasing -Wno-unused-function" )
  set( CFLAGS_DEBUG "-g -Wall -fno-strict-aliasing -Wno-unused-function" )
endif( MSVC )

set( CMAKE_C_FLAGS_RELEASE ${CFLAGS_RELEASE} )
set( CMAKE_C_FLAGS_DEBUG ${CFLAGS_DEBUG} )
set( CMAKE_CXX_FLAGS_RELEASE "${CFLAGS_RELEASE} -Wno-delete-non-virtual-dtor" )
set( CMAKE_CXX_FLAGS_DEBUG "${CFLAGS_DEBUG} -Wno-delete-non-virtual-dtor" )

include_directories( ${CMAKE_SOURCE_DIR} )
if( NOT WIN32 )
include_directories( ${CMAKE_SOURCE_DIR}/objbase )
endif()

add_executable( comboxgen comboxgen.c )
if( NOT WIN32 )
target_link_libraries( comboxgen uuid )
endif()

add_library( alpha Alpha.c Delta.c Echo.cpp Foxtrot.cpp )

add_executable( test_combox test_combox.c )
target_link_libraries( test_combox alpha )
install( TARGETS test_combox RUNTIME DESTINATION bin )

add_executable( test_combox_cpp test_combox_cpp.cpp )
target_link_libraries( test_combox_cpp alpha )
install( TARGETS test_combox_cpp RUNTIME DESTINATION bin )

include( shared.txt )
add_shared_library( alphasrv AlphaSrv.c AlphaSrv.def )
target_link_libraries( alphasrv alpha )

add_executable( test_alphasrv test_alphasrv.c )
target_link_shared_libraries( test_alphasrv alphasrv )
install( TARGETS test_alphasrv RUNTIME DESTINATION bin )

include( CPack )
