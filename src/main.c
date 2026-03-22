#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "lib/gatt_ess.h"
#include "lib/gap.h"

LOG_MODULE_REGISTER(main);



int main(void)
{
LOG_INF("App started...");
    _init_gatt_ess();
    int ret;
    ret = bt_enable(NULL);
    if(ret < 0){
        LOG_ERR("Failed to enable bluetooth");
    }else{
        _init_gap();
    }
    _init_gpios();

        return 0;
}
