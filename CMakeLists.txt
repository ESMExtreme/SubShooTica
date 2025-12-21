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
# Collect all source files under Src/ automatically (reconfigures when files change)
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/Src/*"

)
add_executable(main src/main.cpp "Src/MainMenu.cpp" "Src/MainMenu.h" "Src/OptionsMenu.cpp")

target_compile_features(main PRIVATE cxx_std_17)

target_link_libraries(main PRIVATE SFML::Graphics)



