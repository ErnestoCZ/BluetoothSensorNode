zephyr_include_directories(.)
add_library(cbluetooth_lib STATIC cgap.c)
target_link_libraries(cbluetooth_lib PUBLIC zephyr_interface)