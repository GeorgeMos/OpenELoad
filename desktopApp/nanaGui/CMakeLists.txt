cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Eload VERSION 1.0 LANGUAGES CXX)

# Set the build type to Debug
set(CMAKE_BUILD_TYPE debug)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


add_executable(EloadGui
    main/main.cpp
    src/mainWindow.cpp
    src/widgetBuilder.cpp
)

# Link the library
target_link_libraries(EloadGui PUBLIC ${CMAKE_SOURCE_DIR}/nana-master/build/bin/nana_v142_Debug_x64.lib)
target_link_libraries(EloadGui PUBLIC ${CMAKE_SOURCE_DIR}/../cpp_libs/library/Debug/EloadLib.lib)

target_link_directories(EloadGui PUBLIC ${CMAKE_SOURCE_DIR}/nana-master/build/bin)
target_link_directories(EloadGui PUBLIC ${CMAKE_SOURCE_DIR}/../cpp_libs/library/Debug)



# Include directories
target_include_directories(EloadGui PUBLIC ${CMAKE_SOURCE_DIR}/inc)
#include_directories(nana-master)
target_include_directories(EloadGui PUBLIC ${CMAKE_SOURCE_DIR}/nana-master/include)
#include_directories(nana-master/include/nana/audio)
target_include_directories(EloadGui PUBLIC ${CMAKE_SOURCE_DIR}../cpp_libs/inc)

target_include_directories(EloadGui PUBLIC ${CMAKE_SOURCE_DIR}/json-develop/single_include)

set_target_properties(EloadGui PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin
)