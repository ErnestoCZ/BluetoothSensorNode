zephyr_library_named(custom_lib)
zephyr_library_sources(gap.c gatt_ess.c gpios.c bme280_mod.c)
zephyr_library_include_directories(include)
