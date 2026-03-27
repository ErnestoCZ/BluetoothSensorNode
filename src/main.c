#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "lib/gatt_ess.h"
#include "lib/gap.h"
#include "lib/gpios.h"
#include "lib/bme280_mod.h"
#include <zephyr/drivers/display.h>
#include <lvgl.h>
LOG_MODULE_REGISTER(MAIN);


static void checkInitStatus(int error, const char* message){
    // const char prefix[] = (error == 0) ? "Success": "Failed"; 
    if(!error){
        LOG_INF("Success (code %d) init :  %s",error,message);
    }else{
        LOG_ERR("Failure (code %d) init :  %s",error,message);
    }
}
#define OLED_NODE DT_ALIAS(oled_display)

static const struct device* oled = DEVICE_DT_GET_OR_NULL(OLED_NODE);

int main(void)
{
LOG_INF("App started...");
    int ret;
    ret = bt_enable(NULL);
    checkInitStatus(ret,"bt_enable()");
    if(ret) return -1;

    ret = init_gap();
    checkInitStatus(ret,"init_gap()");
    if(ret) return -1;
    
    init_gatt_ess();


    ret = init_gpios();
    checkInitStatus(ret,"init_gpios()");
    if(ret) return -1;
    
    ret = bme280_mod_init();
    checkInitStatus(ret,"bme280_mod_init()");
    if(ret) return -1;
    
    bme280_mod_start();
    const struct device *display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));

    if (!device_is_ready(display_dev)) {
        return 0;
    }

    ret = display_clear(display_dev);
    if(!ret){
        LOG_ERR("Failed to clear display");
    }
    
    lv_obj_t *hello_label = lv_label_create(lv_scr_act());
    lv_label_set_text(hello_label, "Hello nRF!");
    lv_obj_align(hello_label, LV_ALIGN_CENTER, 0, 0);
    display_blanking_off(display_dev);
    
    lv_task_handler();

    return 0;
}
