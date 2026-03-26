#pragma once 

struct bme280_values{
    double temperature;
    double pressure;
    double humidity;
};

int bme280_mod_init(void);
void bme280_mod_start(void);
void bme280_mod_stop(void);
int bme280_get_latest_data(struct bme280_values *dest);