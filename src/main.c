/*
 * Copyright (c) 2023, Meta
 *
 * SPDX-License-Identifier: Apache-2.0
 */


#include <stdio.h>
#include <zephyr/logging/log.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/drivers/gpio.h>
#include "cshell.h"

const struct gpio_dt_spec led_board_alive = GPIO_DT_SPEC_GET(DT_ALIAS(led3),gpios);

LOG_MODULE_REGISTER(main,LOG_LEVEL_DBG);

const struct bt_le_adv_param *adv_param = BT_LE_ADV_PARAM((BT_LE_ADV_OPT_CONN),BT_GAP_ADV_SLOW_INT_MIN,BT_GAP_ADV_SLOW_INT_MAX,NULL);
struct bt_data ad_data [] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME, (sizeof(CONFIG_BT_DEVICE_NAME)-1)),
};
struct bt_data sd_data [] = {
	BT_DATA_BYTES(BT_DATA_UUID16_SOME, BT_UUID_16_ENCODE(BT_UUID_ESS_VAL))
};

/**
 * TODO need to be implemented 
 */
struct bt_conn_cb conn_cb = {
	.connected = NULL,
	.disconnected = NULL,
	.recycled = NULL,
};

int main(void)
{
	int ret = -1;
	initShell();
	
	if(gpio_is_ready_dt(&led_board_alive)){
		gpio_pin_configure_dt(&led_board_alive, GPIO_OUTPUT_ACTIVE);
		
	}
	
	ret = bt_enable(NULL);
	if(ret < 0) LOG_ERR("Failed to enable bluetooth");
	

	ret = bt_le_adv_start(adv_param,ad_data, ARRAY_SIZE(ad_data), sd_data,ARRAY_SIZE(sd_data));
	if(ret < 0) LOG_ERR("Failed start adverstising");


	return 0;
}
