zephyr_include_directories(.)
add_library(shell_lib STATIC cshell.c)
target_link_libraries(shell_lib PUBLIC zephyr_interface)