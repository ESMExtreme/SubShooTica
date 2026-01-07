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
add_executable(main src/main.cpp "Src/MainMenu.cpp" "Src/MainMenu.h" "Src/OptionsMenu.cpp"
        Src/SaveScreen.cpp
        Src/SaveScreen.h)

target_compile_features(main PRIVATE cxx_std_17)

target_link_libraries(main PRIVATE SFML::Graphics)

# --- Assets / Fonts handling ---
# Place your font files in: <repo-root>/Assets/Fonts/*.ttf (or other font extensions)
set(ASSETS_DIR "${CMAKE_SOURCE_DIR}/Assets")
file(GLOB_RECURSE FONT_FILES CONFIGURE_DEPENDS "${ASSETS_DIR}/Fonts/*")

file(GLOB_RECURSE OPTION_FILE CONFIGURE_DEPENDS "${ASSETS_DIR}/options.txt")

if(FONT_FILES)
    # Ensure destination directory exists
    add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:main>/Assets/Fonts"
    )
    # Copy each font to the runtime output directory
    foreach(_font ${FONT_FILES})
        add_custom_command(TARGET main POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${_font}" "$<TARGET_FILE_DIR:main>/Assets/Fonts"
            COMMENT "Copying font ${_font} to runtime output"
        )
    endforeach()
endif()
if(OPTION_FILE)
    # Ensure destination directory exists
    add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:main>/Assets"
    )
    # Copy options.txt to the runtime output directory
    foreach(_option ${OPTION_FILE})
        add_custom_command(TARGET main POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${_option}" "$<TARGET_FILE_DIR:main>/Assets"
            COMMENT "Copying options.txt to runtime output"
        )
    endforeach()
endif()
