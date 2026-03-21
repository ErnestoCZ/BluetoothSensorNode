zephyr_library_named(custom_lib)
zephyr_library_sources(gatt_ess.c)
zephyr_library_include_directories(include)

# List the actual .c files, not the directory
# zephyr_library_include_directories(.)
# add_library(custom_lib STATIC gatt.c)

# # Include the current directory so headers can be found
# target_include_directories(custom_lib PUBLIC .)

# # Link to zephyr_interface to get Zephyr's compiler flags/headers
# target_link_libraries(custom_lib PUBLIC zephyr_interface)