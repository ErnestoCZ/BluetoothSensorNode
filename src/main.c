#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "lib/gatt_ess.h"
#include "lib/gap.h"
#include "lib/gpios.h"
#include "lib/bme280_mod.h"

LOG_MODULE_REGISTER(MAIN);


static void checkInitStatus(int error, const char* message){
    // const char prefix[] = (error == 0) ? "Success": "Failed"; 
    if(!error){
        LOG_INF("Success (code %d) init :  %s",error,message);
    }else{
        LOG_ERR("Failure (code %d) init :  %s",error,message);
    }
}

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

    return 0;
}
