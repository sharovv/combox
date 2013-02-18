cmake_minimum_required( VERSION 2.6 )
project( combox )

include_directories( ${CMAKE_SOURCE_DIR} )
if( NOT WIN32 )
include_directories( ${CMAKE_SOURCE_DIR}/objbase )
endif( NOT WIN32 )

add_library( alpha Alpha.c Delta.c Echo.cpp Foxtrot.cpp )

add_executable( test_combox test_combox.c )
target_link_libraries( test_combox alpha )

add_executable( test_combox_cpp test_combox_cpp.cpp )
target_link_libraries( test_combox_cpp alpha )
