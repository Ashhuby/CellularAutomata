cmake_minimum_required(VERSION 3.16)
project(CellularAutomata VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 3.0.2 COMPONENTS Graphics Window System REQUIRED)

add_executable(CellularAutomata 
    src/main.cpp
    src/Game.cpp
    src/Grid.cpp
    )

target_link_libraries(CellularAutomata PRIVATE SFML::Graphics SFML::Window SFML::System)

# Copy font to output directory
add_custom_command(TARGET CellularAutomata POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/BarFont.ttf
        $<TARGET_FILE_DIR:CellularAutomata>/BarFont.ttf
)

if(WIN32)
    # This will copy all dependent DLLs (including freetype, zlib, bzip2, libpng, etc.)
    add_custom_command(TARGET CellularAutomata POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Copying all dependency DLLs..."
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:CellularAutomata>
            $<TARGET_FILE_DIR:CellularAutomata>
        COMMAND_EXPAND_LISTS)
endif()