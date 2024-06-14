cmake_minimum_required(VERSION 3.30 FATAL_ERROR)

# must be set before `project(... LANGUAGES CXX`
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD
    "0e5b6991-d74f-4b3d-a41c-cf096e0b2508")
# required for `import std;`
set(CMAKE_CXX_MODULE_STD ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
# required for `import hello_lib;`
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

project(hello-cxx-modules 
        VERSION 1.0.0
        LANGUAGES CXX)

add_library(hello_lib STATIC)  # can also be SHARED
target_sources(hello_lib
  PUBLIC
    FILE_SET CXX_MODULES FILES # required for `export module`
      hello_lib.cxx)
target_compile_features(hello_lib 
                        PRIVATE cxx_std_23  # required for `import std;`
                        INTERFACE cxx_std_20  # so that module can be imported downstream
)

add_executable(hello)
target_sources(hello PRIVATE hello.cxx)
target_compile_features(hello 
                        PRIVATE cxx_std_23  # required for `import std;`
)
target_link_libraries(hello 
                      PRIVATE hello_lib
)
