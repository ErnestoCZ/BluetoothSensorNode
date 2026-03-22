#pragma once
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/assigned_numbers.h>

void init_gatt_ess(void);

ssize_t bt_temp_read_callback(struct bt_conn *conn,
					    const struct bt_gatt_attr *attr,
					    void *buf, uint16_t len,
					    uint16_t offset);


ssize_t bt_humidity_read_callback(struct bt_conn *conn,
					    const struct bt_gatt_attr *attr,
					    void *buf, uint16_t len,
					    uint16_t offset);

ssize_t bt_pressure_read_callback(struct bt_conn *conn,
					    const struct bt_gatt_attr *attr,
					    void *buf, uint16_t len,
					    uint16_t offset);