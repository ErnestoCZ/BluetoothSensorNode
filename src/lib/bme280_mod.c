#include "bme280_mod.h"
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/sensor_data_types.h>

LOG_MODULE_REGISTER(BME280_MOD);

#define PRIORITY 7
#define STACK_SIZE 1024
#define WORK_DELAY K_SECONDS(30)
#define BME280_NODE DT_ALIAS(env_sensor) 
const struct device* bme280_dev = DEVICE_DT_GET_OR_NULL(BME280_NODE);
static struct k_work_q bme_work_queue;
static struct k_work_delayable bme_work_del;


K_THREAD_STACK_DEFINE(stack, STACK_SIZE);
void bme280_work_handler(struct k_work* work){
    int rc = sensor_sample_fetch(bme280_dev);
    if(rc != 0) {
        LOG_ERR("Failed to execute sensor_sample_fetch()");
        return;
    }
    struct sensor_value temp;
    sensor_channel_get(bme280_dev, SENSOR_CHAN_AMBIENT_TEMP, &temp);
    struct sensor_value pres;
    sensor_channel_get(bme280_dev, SENSOR_CHAN_PRESS, &pres);
    struct sensor_value hum;
    sensor_channel_get(bme280_dev, SENSOR_CHAN_HUMIDITY, &hum);
    
    LOG_DBG("New BME280 values : %d Celsius | %d kPa | %d ",temp,pres,hum);
    
    k_work_reschedule_for_queue(&bme_work_queue,&bme_work_del, WORK_DELAY);
    
};



int bme280_mod_init(void){
    if(bme280_dev == NULL){
        return -ENODEV;
    }
    if(!device_is_ready(bme280_dev)){
        return device_is_ready(bme280_dev);
    }
    k_work_queue_init(&bme_work_queue);
    k_work_queue_start(&bme_work_queue,stack,K_THREAD_STACK_SIZEOF(stack),PRIORITY,NULL);
    k_thread_name_set(&bme_work_queue.thread,"bme280_wq");
    k_work_init_delayable(&bme_work_del,bme280_work_handler);
    
    LOG_INF("BME280 Module Initialized");
    
    return 0;
};
void bme280_mod_start(void){
    LOG_DBG("Starting BME280 Measurement...");
    k_work_reschedule_for_queue(&bme_work_queue,&bme_work_del,K_NO_WAIT);
    return 0;
};
void bme280_mod_stop(void){
    LOG_DBG("Stopping BME280 Measurement...");
    k_work_cancel_delayable(&bme_work_del);
    return 0;
};