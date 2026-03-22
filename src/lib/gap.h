#pragma once
#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/assigned_numbers.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/logging/log.h>

/**
 * @brief Init of conn callbacks and k_work structures
 * 
 * @return int < 0 if error occurs else 0 
 */
int init_gap(void);

/**
 * @brief Start preconfigured advertisements
 * 
 * 
 */
void start_adv(void);
/**
 * @brief Stop preconfigured advertisements
 * 
 */
void stop_adv(void);