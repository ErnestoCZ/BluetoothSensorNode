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
static bool should_advertise = false;
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
                if(should_advertise){
                        k_work_submit(&work_start_adv);
                }
                break;
        }
        
};
void _recycled(void){
        if(should_advertise){
                k_work_submit(&work_start_adv);
        }
};

void work_start_adv_handler(struct k_work *work){
        int ret = bt_le_adv_start(adv_param,ad,ARRAY_SIZE(ad),sd, ARRAY_SIZE(sd));
        if(ret < 0){
                LOG_ERR("Failed to start advertisements");
    }else{
        should_advertise = true;
};

}
void work_toggle_adv_handler(struct k_work *work){
        if(should_advertise){
                bt_le_adv_stop();
                should_advertise = !should_advertise;
        }else{
                bt_le_adv_start(adv_param,ad,ARRAY_SIZE(ad),sd,ARRAY_SIZE(sd));
                should_advertise = !should_advertise;
        }
};


struct k_work work_toggle_adv;
struct k_work work_start_adv;
void _init_gap(void){
    int ret;
        k_work_init(&work_toggle_adv,work_toggle_adv_handler);
        k_work_init(&work_start_adv,work_start_adv_handler);
        k_work_submit(&work_start_adv);
    ret = bt_conn_cb_register(&conn_cb);
    if(ret < 0){
            LOG_ERR("Failed to register bt conn callbacks");
    };
}
