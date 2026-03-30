#include "gatt_ess.h"
#include "bme280_mod.h"
#include <zephyr/logging/log.h>
static const int16_t fake_temp = UINT16_MAX;
static const uint16_t fake_humidity = UINT16_MAX;
static const uint32_t fake_pressure = UINT32_MAX;
static bool temperature_ccc_notify = false;
static bool humidity_ccc_notify = false;
static bool pressure_ccc_notify = false;

static void ess_notify_temperature(void);
static void ess_notify_humidity(void);
static void ess_notify_pressure(void);
static void ess_notify_work_handler(struct k_work* work){
    ess_notify_temperature();
    ess_notify_humidity();
    ess_notify_pressure();
};
K_WORK_DEFINE(ess_notify_work,ess_notify_work_handler);

LOG_MODULE_REGISTER(GATT_ESS);
/**
 * @brief init for the corresponding gatt service ESS
 * 
 */
static void ess_temperature_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value){
    temperature_ccc_notify = (value == BT_GATT_CCC_NOTIFY) ? true : false;
    LOG_DBG("ESS_TEMPERATURE_CCC_CHANGED : %d ", value);
};
static void ess_humidity_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value){
    humidity_ccc_notify = (value == BT_GATT_CCC_NOTIFY) ? true : false;
    LOG_DBG("ESS_HUMIDITY_CCC_CHANGED");
};
static void ess_pressure_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value){
    pressure_ccc_notify = (value == BT_GATT_CCC_NOTIFY) ? true : false;
    LOG_DBG("ESS_PRESSURE_CCC_CHANGED");
};
static ssize_t bt_temp_read_callback(struct bt_conn *conn,
					    const struct bt_gatt_attr *attr,
					    void *buf, uint16_t len,
					    uint16_t offset){
                            struct bme280_values_t data;
                            static int16_t ble_temperature;
                            ble_temperature = (bme280_get_latest_data(&data) == 0) ?
                                               (uint16_t)(data.temperature * 100) :
                                               fake_temp;
                            return bt_gatt_attr_read(conn,attr,buf,len,offset,&ble_temperature,sizeof(ble_temperature));
                        };
static ssize_t bt_humidity_read_callback(struct bt_conn *conn,
					    const struct bt_gatt_attr *attr,
					    void *buf, uint16_t len,
					    uint16_t offset){
                            struct bme280_values_t data;
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
                            struct bme280_values_t data;
                            static uint32_t ble_pressure;
                            ble_pressure = (bme280_get_latest_data(&data) == 0) ?
                                            (uint32_t)(data.pressure * 1000) :
                                            fake_pressure;
                            return bt_gatt_attr_read(conn,attr,buf,len,offset,&ble_pressure,sizeof(ble_pressure));
                        };
BT_GATT_SERVICE_DEFINE(
    ess_service,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_ESS),
    BT_GATT_CHARACTERISTIC(
        BT_UUID_TEMPERATURE, 
        BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
        BT_GATT_PERM_READ,
        bt_temp_read_callback,//READ Callback
        NULL,//WRITE Callback
        NULL //USERDATA
    ),
    BT_GATT_CCC(ess_temperature_ccc_cfg_changed,
        BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    BT_GATT_CHARACTERISTIC(
        BT_UUID_HUMIDITY,
        BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
        BT_GATT_PERM_READ,
        bt_humidity_read_callback,
        NULL,
        NULL
    ),
    BT_GATT_CCC(ess_humidity_ccc_cfg_changed,
        BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    BT_GATT_CHARACTERISTIC(
        BT_UUID_PRESSURE,
        BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
        BT_GATT_PERM_READ,
        bt_pressure_read_callback,
        NULL,
        NULL
    ),
    BT_GATT_CCC(ess_pressure_ccc_cfg_changed,
        BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
);
void ess_notify_temperature(void){
    if(temperature_ccc_notify){
        struct bme280_values_t data;
        if(bme280_get_latest_data(&data) == 0){
            uint16_t ble_temperature = (uint16_t)(data.temperature * 100);
            bt_gatt_notify(NULL,&ess_service.attrs[2],&ble_temperature, sizeof(ble_temperature));
        }
    }
};
void ess_notify_humidity(void){
    if(humidity_ccc_notify){
        struct bme280_values_t data;
        if(bme280_get_latest_data(&data) == 0){
            uint16_t ble_humidity = (uint16_t)(data.humidity * 100);
            bt_gatt_notify(NULL,&ess_service.attrs[5],&ble_humidity, sizeof(ble_humidity));
        }
    }
};
void ess_notify_pressure(void){
    if(pressure_ccc_notify){
        struct bme280_values_t data;
        if(bme280_get_latest_data(&data) == 0){
            uint32_t ble_pressure = (uint32_t)(data.pressure * 1000);
            bt_gatt_notify(NULL,&ess_service.attrs[8],&ble_pressure, sizeof(ble_pressure));
        }
    }
};

void ess_notify(void){
    k_work_submit(&ess_notify_work);
}
void init_gatt_ess(void){
    LOG_INF("ESS Service: %d attributes", ess_service.attr_count);
};