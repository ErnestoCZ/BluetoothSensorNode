#pragma once 
#include <zephyr/kernel.h>

extern struct k_sem bme280_data_ready_sem;

struct bme280_values_t{
    double temperature;
    double pressure;
    double humidity;
};

/**
 * @brief Performs bme280 corresponding initializations
 * 
 * @return -ENODEV if bme280 device does not exists, 0 by success
 */
int bme280_mod_init(void);

/**
 * @brief Starts bme280_mod corresponding work
 * 
 */
void bme280_mod_start(void);

/**
 * @brief Stops bme280_mod corresponding work
 * 
 */
void bme280_mod_stop(void);

/**
 * @brief Helper to get the latest values sampled by the bme280 sensor
 * 
 * @param dest 
 * @return int -EINVAL if passed struct is NULL, 0 by success
 */
int bme280_get_latest_data(struct bme280_values_t *dest);

/**
 * @brief Helper to compare bme280_value_t structs
 * 
 * @param s1 
 * @param s2 
 * @return int -EINVAL if at least one passed struct is NULL, 1 if at least one value in struct changed, 0 if noting changed
 */
int bme280_data_has_changed(const struct bme280_values_t *s1, const struct bme280_values_t *s2);