#include "gatt_ess.h"
#include "bme280_mod.h"
#include <zephyr/logging/log.h>
static const int16_t fake_temp = UINT16_MAX;
static const uint16_t fake_humidity = UINT16_MAX;
static const uint16_t fake_pressure = UINT32_MAX;

LOG_MODULE_REGISTER(GATT_ESS);
/**
 * @brief init for the corresponding gatt service ESS
 * 
 */
void init_gatt_ess(void){};
ssize_t bt_temp_read_callback(struct bt_conn *conn,
					    const struct bt_gatt_attr *attr,
					    void *buf, uint16_t len,
					    uint16_t offset){
                            struct bme280_values data;
                            static int16_t ble_temp;
                            ble_temp = (bme280_get_latest_data(&data) == 0) ?
                                        (uint16_t)(data.temperature * 100) :
                                        fake_temp;
                            return bt_gatt_attr_read(conn,attr,buf,len,offset,&ble_temp,sizeof(ble_temp));
                        };
static ssize_t bt_humidity_read_callback(struct bt_conn *conn,
					    const struct bt_gatt_attr *attr,
					    void *buf, uint16_t len,
					    uint16_t offset){
                            struct bme280_values data;
                            static uint16_t ble_humidity;
                            ble_humidity = (bme280_get_latest_data(&data) == 0) ? 
                                            (uint16_t)(data.humidity * 100) :
                                            fake_humidity;
                            return bt_gatt_attr_read(conn,attr,buf,len,offset,&ble_humidity,sizeof(ble_humidity));
                        };
static ssize_t bt_pressure_read_callback(struct bt_conn *conn,
					    const struct bt_gatt_attr *attr,
					    void *buf, uint16_t len,
					    uint16_t offset){
                            struct bme280_values data;
                            static uint32_t ble_pressure;
                            ble_pressure = (bme280_get_latest_data(&data) == 0) ?
                                            (uint32_t)(data.pressure * 1000) :
                                            fake_pressure;
                            return bt_gatt_attr_read(conn,attr,buf,len,offset,&ble_pressure,sizeof(ble_pressure));
                        };
BT_GATT_SERVICE_DEFINE(
    ess_service1,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_ESS),
    BT_GATT_CHARACTERISTIC(
        BT_UUID_TEMPERATURE, 
        BT_GATT_CHRC_READ,
        BT_GATT_PERM_READ,
        bt_temp_read_callback,//READ Callback
        NULL,//WRITE Callback
        NULL //USERDATA
    ),
    BT_GATT_CHARACTERISTIC(
        BT_UUID_HUMIDITY,
        BT_GATT_CHRC_READ,
        BT_GATT_PERM_READ,
        bt_humidity_read_callback,
        NULL,
        NULL
    ),
    //BT_GATT_CPF(&humidity_cpf)
    BT_GATT_CHARACTERISTIC(
        BT_UUID_PRESSURE,
        BT_GATT_CHRC_READ,
        BT_GATT_PERM_READ,
        bt_pressure_read_callback,
        NULL,
        NULL
    )
    //BT_GATT_CPF(&pressure_cpf)
);