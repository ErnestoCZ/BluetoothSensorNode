#include "gpios.h"
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include "gap.h"

LOG_MODULE_REGISTER(GPIOS);

/**
 * @brief LEDs used by application to indicate states
 * 
 */
static const struct gpio_dt_spec led_board_status = GPIO_DT_SPEC_GET(DT_ALIAS(led0),gpios);
static const struct gpio_dt_spec led_ble_advertising_status = GPIO_DT_SPEC_GET(DT_ALIAS(led1),gpios);
static const struct gpio_dt_spec led_ble_gatt_device_connected_status = GPIO_DT_SPEC_GET(DT_ALIAS(led2),gpios);
#define LED0_LABEL DT_PROP(DT_ALIAS(led0),label)
#define LED1_LABEL DT_PROP(DT_ALIAS(led1),label)
#define LED2_LABEL DT_PROP(DT_ALIAS(led2),label)

/**
 * @brief BUTTONs used by application to control states
 * 
 */
static const struct gpio_dt_spec button_start_adv = GPIO_DT_SPEC_GET(DT_ALIAS(sw0),gpios);
static const struct gpio_dt_spec button_stop_adv = GPIO_DT_SPEC_GET(DT_ALIAS(sw1),gpios);


static int init_LEDS(void){
    int ret = -1;
    if(gpio_is_ready_dt(&led_board_status)){
        ret = gpio_pin_configure_dt(&led_board_status, GPIO_OUTPUT_ACTIVE);
        LOG_INF("Success to configure %s", LED0_LABEL);
    }else{
        LOG_ERR("Failed to configure %s", LED0_LABEL);
    };
    if(gpio_is_ready_dt(&led_ble_advertising_status)){
        ret = gpio_pin_configure_dt(&led_ble_advertising_status, GPIO_OUTPUT_INACTIVE);
    }else{
        LOG_ERR("Failed to configure %s", LED1_LABEL);
    };
    if(gpio_is_ready_dt(&led_ble_gatt_device_connected_status)){
        ret = gpio_pin_configure_dt(&led_ble_gatt_device_connected_status, GPIO_OUTPUT_INACTIVE);
    }else{
        LOG_ERR("Failed to configure %s", LED2_LABEL);
    };
    return ret;

};
struct gpio_callback button_start_adv_callback;
void button_start_adv_handler(const struct device *port,
					struct gpio_callback *cb,
					gpio_port_pins_t pins){
                        start_adv();
                    };
struct gpio_callback button_stop_adv_callback;
void button_stop_adv_handler(const struct device *port,
					struct gpio_callback *cb,
					gpio_port_pins_t pins){
                        stop_adv();
                    };

static int init_BUTTONS(void){
    int ret = -1;
    if(gpio_is_ready_dt(&button_start_adv)){
        ret = gpio_pin_configure_dt(&button_start_adv, GPIO_INPUT);
        ret = gpio_pin_interrupt_configure_dt(&button_start_adv, GPIO_INT_EDGE_FALLING);
        gpio_init_callback(&button_start_adv_callback,button_start_adv_handler,BIT(button_start_adv.pin));
        ret = gpio_add_callback_dt(&button_start_adv,&button_start_adv_callback);
    }    
    if(gpio_is_ready_dt(&button_stop_adv)){
        ret = gpio_pin_configure_dt(&button_stop_adv, GPIO_INPUT);
        ret = gpio_pin_interrupt_configure_dt(&button_stop_adv, GPIO_INT_EDGE_FALLING);
        gpio_init_callback(&button_stop_adv_callback,button_stop_adv_handler,BIT(button_stop_adv.pin));
        ret = gpio_add_callback_dt(&button_stop_adv,&button_stop_adv_callback);
    }
    
    return ret;

};

int init_gpios(void){

    int ret = init_LEDS();
    ret = init_BUTTONS();
    return ret;

};