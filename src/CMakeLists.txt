cmake_minimum_required(VERSION 3.7)

project(InfoNES-port)

include_directories(.)
include_directories(InfoNES)

add_subdirectory(${CMAKE_SOURCE_DIR}/InfoNES)

aux_source_directory(${CMAKE_SOURCE_DIR} SRC)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
    # TODO(amitp): I don't know how to set this properly with add_compile_options
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2")
    set(SDL2_LIBRARIES "-s USE_SDL=2")
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



