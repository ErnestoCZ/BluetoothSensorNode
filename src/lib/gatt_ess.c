#include "gatt_ess.h"
#include "bme280_mod.h"
static const int16_t fake_temp = UINT16_MAX;
static const struct bt_gatt_cpf temperature_cpf = {
  .format = 0x0E,       // 0x0E = signed 16-bit integer (sint16)
  .exponent = -2,        // Value * 10^-2 (e.g., 2550 becomes 25.50)
  .unit = 0x272F,       // 0x272F = UUID for Celsius
  .name_space = 0x01,   // 0x01 (Bluetooth SIG)
  .description = 0x0000 // 0x0000 (No specific desc)
};

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
ssize_t bt_humidity_read_callback(struct bt_conn *conn,
					    const struct bt_gatt_attr *attr,
					    void *buf, uint16_t len,
					    uint16_t offset){
                            return bt_gatt_attr_read(conn,attr,buf,len,offset,&fake_temp,sizeof(uint16_t));
                        };
ssize_t bt_pressure_read_callback(struct bt_conn *conn,
					    const struct bt_gatt_attr *attr,
					    void *buf, uint16_t len,
					    uint16_t offset){
                            return bt_gatt_attr_read(conn,attr,buf,len,offset,&fake_temp,sizeof(uint16_t));
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