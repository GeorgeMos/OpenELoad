cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Eload VERSION 1.0)

# Set the build type to Debug
set(CMAKE_BUILD_TYPE debug)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(inc)
include_directories(nana-master)
include_directories(nana-master/include)
include_directories(../cpp_libs/inc)

add_executable(EloadGui
    main/main.cpp
)

# Find the dynamic library
find_library(NANA NAMES nana PATHS /nana-master/build/bin)

find_library(ELOAD_GUI NAMES eload_gui PATHS ${CMAKE_SOURCE_DIR}/../cpp_libs/library)

# Link the library
target_link_libraries(EloadGui PUBLIC ${nana} PUBLIC ${eload_gui})

#target_link_libraries(EloadGui PUBLIC ${ELOAD_GUI})

set_target_properties(EloadGui PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin
)