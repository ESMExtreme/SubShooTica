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
add_executable(main ${PROJECT_SOURCES}
        Src/Level1.h
        Src/Level1.cpp
        Src/Level2.h
        Src/Level2.cpp
        Src/GameMap.cpp
        Src/GameMap.h
        Src/Level1.cpp
        Src/Level1.cpp)

target_compile_features(main PRIVATE cxx_std_17)

target_link_libraries(main PRIVATE
    SFML::Graphics
    SFML::Audio
)


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

# --- JSON save files handling ---
# Copy save*.json files
file(GLOB JSON_SAVE_FILES CONFIGURE_DEPENDS "${ASSETS_DIR}/save*.json")
if(JSON_SAVE_FILES)
    foreach(_json ${JSON_SAVE_FILES})
        add_custom_command(TARGET main POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${_json}" "$<TARGET_FILE_DIR:main>/Assets"
            COMMENT "Copying JSON save ${_json} to runtime output"
        )
    endforeach()
endif()

# --- Map backgrounds handling ---
# Copy Map directory with locked/unlocked subdirectories
file(GLOB_RECURSE MAP_FILES CONFIGURE_DEPENDS "${ASSETS_DIR}/Media/Map/*")
if(MAP_FILES)
    foreach(_map ${MAP_FILES})
        get_filename_component(_map_dir "${_map}" DIRECTORY)
        file(RELATIVE_PATH _rel_dir "${ASSETS_DIR}/Media" "${_map_dir}")
        add_custom_command(TARGET main POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${_map}" "$<TARGET_FILE_DIR:main>/Assets/Media/${_rel_dir}"
            COMMENT "Copying map background ${_map} to runtime output"
        )
    endforeach()
endif()

file(GLOB_RECURSE MEDIA_FILES CONFIGURE_DEPENDS "${ASSETS_DIR}/Media/*")

if(MEDIA_FILES)
    # Ensure destination directory exists
    add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:main>/Assets/Media"
    )
    # Copy each media file to the runtime output directory
    foreach(_media ${MEDIA_FILES})
        add_custom_command(TARGET main POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${_media}" "$<TARGET_FILE_DIR:main>/Assets/Media"
            COMMENT "Copying media ${_media} to runtime output"
        )
    endforeach()
endif()

file(GLOB_RECURSE AUDIO_FILES CONFIGURE_DEPENDS "${ASSETS_DIR}/Audio/*")

if(AUDIO_FILES)
    # Ensure destination directory exists
    add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:main>/Assets/Audio"
    )
    # Copy each audio file to the runtime output directory
    foreach(_audio ${AUDIO_FILES})
        add_custom_command(TARGET main POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${_audio}" "$<TARGET_FILE_DIR:main>/Assets/Audio"
            COMMENT "Copying audio ${_audio} to runtime output"
        )
    endforeach()
endif()

# --- Enemy config files handling ---
# Copy level enemy configuration files (level1_enemies.txt, level2_enemies.txt, etc.)
file(GLOB ENEMY_CONFIG_FILES CONFIGURE_DEPENDS "${ASSETS_DIR}/level*_enemies.txt")
if(ENEMY_CONFIG_FILES)
    foreach(_config ${ENEMY_CONFIG_FILES})
        add_custom_command(TARGET main POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${_config}" "$<TARGET_FILE_DIR:main>/Assets"
            COMMENT "Copying enemy config ${_config} to runtime output"
        )
    endforeach()
endif()

# --- Audio handling ---
