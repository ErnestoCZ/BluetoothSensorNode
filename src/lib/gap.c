#include "gap.h"

LOG_MODULE_REGISTER(GAP);

//Private function definitions
void connected_cb(struct bt_conn *conn, uint8_t err);
void disconnected_cb(struct bt_conn *conn, uint8_t reason);
void recycled_cb(void);
void start_adv(void);
void stop_adv(void);

//STRUCTS
struct k_work start_adv_work;
struct k_work stop_adv_work;
static struct bt_conn* _conn;
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

struct bt_conn_cb conn_cb = {
        .connected = connected_cb,
        .disconnected = disconnected_cb,
        .recycled = recycled_cb
};
//GAP CALLBACK IMPLEMENTATIONS
void connected_cb(struct bt_conn *conn, uint8_t err){
        switch (err)
        {
        case BT_HCI_ERR_SUCCESS:
                _conn = bt_conn_ref(conn);
                break;
        default:
                break;
        }
};
void disconnected_cb(struct bt_conn *conn, uint8_t reason){
        switch (reason)
        {
                
        default:
                bt_conn_unref(_conn);
                        //TODO handling behavior on disconnect
                break;
        }
        
};
void recycled_cb(void){
        //TODO handling
};

void start_adv_work_handler(struct k_work *work){
        bt_le_adv_start(adv_param,ad,ARRAY_SIZE(ad),sd, ARRAY_SIZE(sd));
}
void stop_adv_work_handler(struct k_work *work){
                bt_le_adv_stop();
}


int init_gap(void){
        int ret = bt_conn_cb_register(&conn_cb);
    if(ret < 0){
            LOG_ERR("Failed to register bt conn callbacks");
                return ret;
        }else{
                LOG_INF("Success to register bt conn callbacks");
                k_work_init(&start_adv_work, start_adv_work_handler);
                k_work_init(&stop_adv_work, stop_adv_work_handler);
                return ret;
    };
}
