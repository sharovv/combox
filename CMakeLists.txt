cmake_minimum_required( VERSION 2.6 )
project( combox )

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

include( CPack )
