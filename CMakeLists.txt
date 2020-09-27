cmake_policy(SET CMP0048 NEW)

project(tiny_tuple VERSION 1.0.0 LANGUAGES CXX)
cmake_minimum_required(VERSION 3.10)

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-std=c++14 HAS_CXX14_FLAG)
check_cxx_compiler_flag(-std=c++17 HAS_CXX17_FLAG)
check_cxx_compiler_flag(-std=c++2a HAS_CXX20_FLAG)

if(HAS_CXX20_FLAG)
  set(CMAKE_CXX_STANDARD 20)
elseif(HAS_CXX17_FLAG)
  set(CMAKE_CXX_STANDARD 17)
elseif(HAS_CXX14_FLAG)
  set(CMAKE_CXX_STANDARD 14)
else()
  message(FATAL_ERROR "requires at least c++14")
endif()



find_package(kvasir_mpl)

add_library(tiny_tuple INTERFACE)
add_library(tiny_tuple::tiny_tuple ALIAS tiny_tuple)
target_link_libraries(tiny_tuple INTERFACE kvasir_mpl)
target_include_directories(tiny_tuple INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )
install(TARGETS tiny_tuple DESTINATION include)
install(TARGETS tiny_tuple EXPORT tiny_tuple-Targets DESTINATION include)
install(EXPORT tiny_tuple-Targets
    NAMESPACE tiny_tuple::
    DESTINATION lib/cmake/tiny_tuple
    )
