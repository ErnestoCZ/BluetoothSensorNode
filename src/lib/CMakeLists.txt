zephyr_library_named(custom_lib)
zephyr_library_sources(gap.c gatt_ess.c gpios.c bme280_mod.c)
zephyr_library_include_directories(include)

# The app's own Zephyr library is created after Zephyr has wired up its
# libraries, so it misses the dependency on generated headers (offsets.h, ...).
# Without this, parallel builds can compile it before those headers exist.
add_dependencies(custom_lib zephyr_generated_headers)
