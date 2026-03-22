#include "gpios.h"
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

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
static const struct gpio_dt_spec button_start_stop_advertisements = GPIO_DT_SPEC_GET(DT_ALIAS(sw0),gpios);


static void _init_LEDS(void){
    if(gpio_is_ready_dt(&led_board_status)){
        gpio_pin_configure_dt(&led_board_status, GPIO_OUTPUT_ACTIVE);
        LOG_INF("Success to configure %s", LED0_LABEL);
    }else{
        LOG_ERR("Failed to configure %s", LED0_LABEL);
    };
    if(gpio_is_ready_dt(&led_ble_advertising_status)){
        gpio_pin_configure_dt(&led_ble_advertising_status, GPIO_OUTPUT_INACTIVE);
    }else{
        LOG_ERR("Failed to configure %s", LED1_LABEL);
    };
    if(gpio_is_ready_dt(&led_ble_gatt_device_connected_status)){
        gpio_pin_configure_dt(&led_ble_gatt_device_connected_status, GPIO_OUTPUT_INACTIVE);
    }else{
        LOG_ERR("Failed to configure %s", LED2_LABEL);
    };

};
struct gpio_callback button_start_stop_adv_callback;
void button_start_stop_adv_handler(const struct device *port,
					struct gpio_callback *cb,
					gpio_port_pins_t pins){
                        //TODO push adv start/stop work to queue
                        LOG_INF("Button clicked");
                    };

static void _init_BUTTONS(void){
    if(gpio_is_ready_dt(&button_start_stop_advertisements)){
        gpio_pin_configure_dt(&button_start_stop_advertisements, GPIO_INPUT);
        gpio_pin_interrupt_configure_dt(&button_start_stop_advertisements, GPIO_INT_EDGE_FALLING);
        gpio_init_callback(&button_start_stop_adv_callback,button_start_stop_adv_handler,BIT(button_start_stop_advertisements.pin));
        gpio_add_callback_dt(&button_start_stop_advertisements,&button_start_stop_adv_callback);
    }else{
        LOG_ERR("Failed to configure Button0");
    }

};

void _init_gpios(void){
    _init_LEDS();
    _init_BUTTONS();

};