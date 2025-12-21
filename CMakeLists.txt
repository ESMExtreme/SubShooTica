cmake_minimum_required(VERSION 3.28)
project(SubShootica LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)
set(FONTS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/Assets/Fonts")

# Collect all source files under Src/ automatically (reconfigures when files change)
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/Src/*.cpp"
    "${CMAKE_SOURCE_DIR}/Src/*.c"
    "${CMAKE_SOURCE_DIR}/Src/*.cxx"
)
# Copy entire Fonts folder to the runtime output directory after building the target

add_executable(main ${PROJECT_SOURCES})

# Ensure the compiler uses C++17
target_compile_features(main PRIVATE cxx_std_17)

# Make headers in Src/ discoverable with #include "Header.h"
target_include_directories(main PRIVATE "${CMAKE_SOURCE_DIR}/Src")

target_link_libraries(main PRIVATE SFML::Graphics)  

if(EXISTS "${FONTS_SOURCE_DIR}")
    add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:main>/Fonts"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${FONTS_SOURCE_DIR}" "$<TARGET_FILE_DIR:main>/Fonts"
        COMMENT "Copying fonts to runtime output directory"
    )
endif()



