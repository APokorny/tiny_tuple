cmake_policy(SET CMP0048 NEW)

project(tiny_tuple VERSION 1.0.0 LANGUAGES CXX)
cmake_minimum_required(VERSION 3.10)

find_package(kvasir_mpl)

add_library(tiny_tuple INTERFACE include/tiny_tuple/map.h include/tiny_tuple/tuple.h)
add_library(TinyTuple::tiny_tuple ALIAS tiny_tuple)

target_include_directories(tiny_tuple PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

target_link_libraries(tiny_tuple INTERFACE kvasir_mpl)
install(TARGETS tiny_tuple EXPORT tine_tuple-Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
    )
install(EXPORT tiny_tuple-Targets
    FILE tiny_tuple.cmake
    NAMESPACE TinyTuple::
    DESTINATION lib/cmake/
    )
