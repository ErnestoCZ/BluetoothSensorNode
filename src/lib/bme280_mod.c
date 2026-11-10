#include "bme280_mod.h"
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/sensor_data_types.h>

LOG_MODULE_REGISTER(BME280_MOD);

#define BME280_WORK_QUEUE_PRIORITY 7
#define BME280_STACK_SIZE 1024
#define BME280_WORK_DELAY K_SECONDS(10)
#define BME280_MUTEX_TIMEOUT K_FOREVER
#define BME280_NODE DT_ALIAS(env_sensor) 
const struct device* bme280_dev = DEVICE_DT_GET_OR_NULL(BME280_NODE);
static struct k_work_q bme_work_queue;
static struct k_work_delayable bme_work_del;
static struct bme280_values_t bme280_data = {0.0};

K_SEM_DEFINE(bme280_data_ready_sem,0,1);
K_MUTEX_DEFINE(bme280_mutex);
K_THREAD_STACK_DEFINE(bme280_work_queue_stack, BME280_STACK_SIZE);
void bme280_work_handler(struct k_work* work){
    static struct sensor_value temp, pres, hum;
    int rc = sensor_sample_fetch(bme280_dev);
    if(rc != 0) {
        LOG_ERR("Failed to execute sensor_sample_fetch()");
        goto reschedule;
    }
    sensor_channel_get(bme280_dev, SENSOR_CHAN_AMBIENT_TEMP, &temp);
    sensor_channel_get(bme280_dev, SENSOR_CHAN_PRESS, &pres);
    sensor_channel_get(bme280_dev, SENSOR_CHAN_HUMIDITY, &hum);
    
    k_mutex_lock(&bme280_mutex, BME280_MUTEX_TIMEOUT);
    bme280_data.temperature = sensor_value_to_double(&temp);
    bme280_data.pressure = sensor_value_to_double(&pres);
    bme280_data.humidity = sensor_value_to_double(&hum);
    k_mutex_unlock(&bme280_mutex);
    k_sem_give(&bme280_data_ready_sem);
    // LOG_DBG("New BME280 values : %d Celsius | %d kPa | %d ",temp.val1,pres.val1,hum.val1);
reschedule:    
    k_work_reschedule_for_queue(&bme_work_queue,&bme_work_del, BME280_WORK_DELAY);
    
};



int bme280_mod_init(void){
    if(bme280_dev == NULL){
        return -ENODEV;
    }
    if(!device_is_ready(bme280_dev)){
        return -ENODEV;
    }
    k_work_queue_init(&bme_work_queue);
    k_work_queue_start(&bme_work_queue,bme280_work_queue_stack,K_THREAD_STACK_SIZEOF(bme280_work_queue_stack),BME280_WORK_QUEUE_PRIORITY,NULL);
    k_thread_name_set(&bme_work_queue.thread,"bme280_wq");
    k_work_init_delayable(&bme_work_del,bme280_work_handler);
    
    LOG_INF("BME280 Module Initialized");
    
    return 0;
};
void bme280_mod_start(void){
    LOG_DBG("Starting BME280 Measurement...");
    k_work_reschedule_for_queue(&bme_work_queue,&bme_work_del,K_NO_WAIT);
};
void bme280_mod_stop(void){
    LOG_DBG("Stopping BME280 Measurement...");
    k_work_cancel_delayable(&bme_work_del);
};

int bme280_get_latest_data(struct bme280_values_t *dest){
    if(dest == NULL){
        return -EINVAL;
    }
    k_mutex_lock(&bme280_mutex,BME280_MUTEX_TIMEOUT);
    *dest = bme280_data;
    k_mutex_unlock(&bme280_mutex);
    return 0;
};


int bme280_data_has_changed(const struct bme280_values_t *s1, const struct bme280_values_t *s2){
    if(s1 == NULL || s2 == NULL) return -EINVAL;
    
    if(
        s1->humidity != s2->humidity ||
        s1->pressure != s2->pressure ||
        s1->temperature != s2->temperature
    ) return 1;

    return 0;
};