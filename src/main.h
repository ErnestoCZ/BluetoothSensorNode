#pragma once
#include <zephyr/kernel.h>
#include "lib/gatt_ess.h"
#include "lib/bme280_mod.h"

#define SENSOR_DATA_PROCESS_STACK_SIZE 2048
#define SENSOR_DATA_PROCESS_PRIORITY 7
void sensor_data_process_handler(void);
K_THREAD_DEFINE(sensor_data_process_thread,
    SENSOR_DATA_PROCESS_STACK_SIZE,
    sensor_data_process_handler,
    NULL,
    NULL,
    NULL,
    SENSOR_DATA_PROCESS_PRIORITY,
    K_USER,
    0);

void sensor_data_process_handler(){
    
    static struct bme280_values_t previous_sensor_data;
    struct bme280_values_t current_sensor_data;

    while(1){
    if(k_sem_take(&bme280_data_ready_sem,K_FOREVER) == 0){

        if(bme280_get_latest_data(&current_sensor_data) == 0){
            
            if(bme280_data_has_changed(&previous_sensor_data,&current_sensor_data) == 1){
                previous_sensor_data = current_sensor_data;
                ess_notify();
            }
        }
        
    }

    }
}