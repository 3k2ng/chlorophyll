cmake_minimum_required(VERSION 3.20)
project(chlorophyll)

include(FetchContent)

FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/5.5.tar.gz
)
FetchContent_MakeAvailable(raylib)

add_executable(chlorophyll
    "./main.cpp"
)
set(raylib_VERBOSE 1)
target_include_directories(${PROJECT_NAME} PRIVATE
    "./include"
)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

file(GLOB texture_files "./data/textures/*")
foreach(file ${texture_files})
    file(RELATIVE_PATH file ${CMAKE_CURRENT_SOURCE_DIR} ${file})
    configure_file(${file} ${file} COPYONLY)
endforeach()
