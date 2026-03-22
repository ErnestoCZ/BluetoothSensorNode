#pragma once
#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/assigned_numbers.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/logging/log.h>



void _init_gap(void);

void _connected(struct bt_conn *conn, uint8_t err);
void _disconnected(struct bt_conn *conn, uint8_t reason);
void _recycled(void);

extern struct k_work work_start_adv;
extern struct k_work work_toggle_adv;