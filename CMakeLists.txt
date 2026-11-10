cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

find_package(Doxygen)
if (DOXYGEN_FOUND)
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
endif()
# list(APPEND CMAKE_MODULE_PATH ${ZEPHYR_BASE}/modules/nanopb)
# include(nanopb)

project(nrf5340Playground LANGUAGES C CXX ASM)
# set(CMAKE_C_STANDARD 11)
# set(CMAKE_C_STANDARD_REQUIRED ON)
# set(CMAKE_C_EXTENSIONS ON)

add_subdirectory(src/lib)
target_sources(app PRIVATE src/main.c)
target_include_directories(app PRIVATE .)
target_link_libraries(app PUBLIC custom_lib)