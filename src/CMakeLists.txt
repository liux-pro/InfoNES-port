cmake_minimum_required(VERSION 3.7)

project(InfoNES-port)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -O3  -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3  -Wall")


include_directories(.)
include_directories(InfoNES)

add_subdirectory(${CMAKE_SOURCE_DIR}/InfoNES)

aux_source_directory(${CMAKE_SOURCE_DIR} SRC)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -sUSE_SDL=2 --shell-file ${CMAKE_SOURCE_DIR}/shell_minimal.html")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -sUSE_SDL=2 --shell-file ${CMAKE_SOURCE_DIR}/shell_minimal.html")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
else ()
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS})
endif ()

add_executable("${PROJECT_NAME}" ${SRC})
target_link_libraries(${PROJECT_NAME} libInfoNES)


if (${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
else ()
    target_link_libraries("${PROJECT_NAME}"  ${SDL2_LIBRARIES})
endif ()



