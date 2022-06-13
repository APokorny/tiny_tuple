cmake_policy(SET CMP0057 NEW)
cmake_policy(SET CMP0048 NEW)
cmake_policy(SET CMP0077 NEW)
cmake_minimum_required(VERSION 3.14)
project(tiny_tuple VERSION 0.1.0 LANGUAGES CXX)

include(cmake/CPM.cmake)
CPMAddPackage(
    NAME kvasir_mpl
    GITHUB_REPOSITORY kvasir-io/mpl
    GIT_TAG 59cd21381c3f3285939c0d32dbfe657398c52ec9
    )
CPMAddPackage("gh:TheLartians/PackageProject.cmake@1.9.0")

add_library(tiny_tuple INTERFACE)
add_library(tiny_tuple::tiny_tuple ALIAS tiny_tuple)
add_compile_options(tiny_tuple INTERFACE cxx_std_17)
target_link_libraries(tiny_tuple INTERFACE kvasir_mpl)
target_include_directories(tiny_tuple INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

packageProject(
    NAME ${PROJECT_NAME}
    VERSION ${PROJECT_VERSION}
    BINARY_DIR ${PROJECT_BINARY_DIR}
    INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include
    INCLUDE_DESTINATION include/${PROJECT_NAME}-${PROJECT_VERSION}
    DEPENDENCIES "kvasir_mpl 1.0.0"
    VERSION_HEADER "${PROJECT_NAME}/version.h"
    EXPORT_HEADER "${PROJECT_NAME}/export.h"
    NAMESPACE tiny_tuple
    COMPATIBILITY AnyNewerVersion
    DISABLE_VERSION_SUFFIX YES
    ARCH_INDEPENDENT YES
    )
