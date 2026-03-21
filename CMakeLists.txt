cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(nrf5340Playground)

add_subdirectory(src/lib)
target_sources(app PRIVATE src/main.c)
target_link_libraries(app PUBLIC custom_lib)