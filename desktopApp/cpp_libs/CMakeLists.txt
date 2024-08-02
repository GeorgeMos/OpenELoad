cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Eload VERSION 1.0)

# Set the build type to Debug
set(CMAKE_BUILD_TYPE debug)

# Set the output directories for libraries and executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/library)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/library)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/library)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(inc)

# Add the library source files
add_library(EloadLib SHARED
    ${CMAKE_SOURCE_DIR}/src/serial.cpp
    ${CMAKE_SOURCE_DIR}/src/usbCom.cpp
)

#Create library directory. it contains the .lib and .dll files
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/library)

# Add the executable
add_executable(Eload
    main/main.cpp
    src/usbCom.cpp
    #src/vcp_api.cpp
    src/serial.cpp
)

if(WIN32)
    set_target_properties(EloadLib PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS ON
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/library
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/library
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/library
    )
endif()

# Link the library to the executable
target_link_libraries(Eload EloadLib)

# Specify output directories for libraries and executables
set_target_properties(EloadLib PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/library
)

set_target_properties(Eload PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin
)
