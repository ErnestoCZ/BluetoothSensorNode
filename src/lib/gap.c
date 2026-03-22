#include "gap.h"

LOG_MODULE_REGISTER(GAP);
#define DEVICENAME CONFIG_BT_DEVICE_NAME

const struct bt_le_adv_param adv_param[] = 
        BT_LE_ADV_PARAM(BT_LE_ADV_OPT_SCANNABLE | BT_LE_ADV_OPT_CONN,
        BT_LE_ADV_INTERVAL_DEFAULT,
        BT_LE_ADV_INTERVAL_DEFAULT,
        NULL);

const struct bt_data ad[] = {
        BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR),
        BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME, sizeof(CONFIG_BT_DEVICE_NAME)-1)
};

const struct bt_data sd[] = {
        BT_DATA_BYTES(BT_DATA_UUID16_SOME, BT_UUID_16_ENCODE(BT_UUID_ESS_VAL))
};

static struct bt_conn* _conn;

struct bt_conn_cb conn_cb = {
        .connected = _connected,
        .disconnected = _disconnected,
        .recycled = _recycled
};

//GAP CALLBACK IMPLEMENTATIONS
void _connected(struct bt_conn *conn, uint8_t err){
        switch (err)
        {
        case BT_HCI_ERR_SUCCESS:
                _conn = bt_conn_ref(conn);
                break;
        default:
                break;
        }
};
void _disconnected(struct bt_conn *conn, uint8_t reason){
        switch (reason)
        {
        default:
                bt_conn_unref(_conn);
                break;
        }
        
};
void _recycled(void){
        //push work to queue to start advertisements
        int ret = 0;
        ret = bt_le_adv_start(adv_param,ad,ARRAY_SIZE(ad),sd,ARRAY_SIZE(sd));
        if(ret < 0){
                LOG_ERR("Failed to start advertisements");
        };
};

void _init_gap(void){
    int ret;
    ret = bt_conn_cb_register(&conn_cb);
    if(ret < 0){
            LOG_ERR("Failed to register bt conn callbacks");
    };
    ret = bt_le_adv_start(adv_param,ad,ARRAY_SIZE(ad),sd,ARRAY_SIZE(sd));
    if(ret < 0){
            LOG_ERR("Failed to start advertisements");
    };
}