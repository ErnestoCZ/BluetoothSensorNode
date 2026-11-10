#include "gap.h"
#include <zephyr/bluetooth/hci.h>

LOG_MODULE_REGISTER(GAP);

//Private function definitions
void connected_cb(struct bt_conn *conn, uint8_t err);
void disconnected_cb(struct bt_conn *conn, uint8_t reason);
void recycled_cb(void);
void start_adv(void);
void stop_adv(void);

//STRUCTS
static struct {
    struct bt_conn *conns[CONFIG_BT_MAX_CONN];
    uint8_t count;
} active_session = {0}; // Everything inside is zeroed out
struct k_work start_adv_work;
struct k_work stop_adv_work;
struct k_work_delayable restart_adv_work;
// static struct bt_conn* active_conns[CONFIG_BT_MAX_CONN] = {NULL};
// static uint8_t active_conns_count = 0;
const struct bt_le_adv_param adv_param[] = 
        BT_LE_ADV_PARAM(BT_LE_ADV_OPT_SCANNABLE | BT_LE_ADV_OPT_CONN | BT_LE_ADV_OPT_USE_IDENTITY,
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
        if(err){
                LOG_ERR("Failed to connect device with error %d (%s)", err, bt_hci_err_to_str(err));
        }
        
        for (int i = 0; i < CONFIG_BT_MAX_CONN; i++)
        {
                if(active_session.conns[i] == NULL){
                        active_session.conns[i] = bt_conn_ref(conn);
                        active_session.count++;
                        LOG_INF("Connected : Saved to slot %d",i);
                        break;
                }
        }
        if(active_session.count < CONFIG_BT_MAX_CONN){
                k_work_reschedule(&restart_adv_work,K_MSEC(500));
        }
};
void disconnected_cb(struct bt_conn *conn, uint8_t reason){
        char addr[BT_ADDR_LE_STR_LEN];
        bt_addr_le_to_str(bt_conn_get_dst(conn),addr,sizeof(addr));
        if(reason){
                LOG_ERR("Disconnected device with the ADDRESS %s for reason %s", addr, bt_hci_err_to_str(reason));
        }
        //release from active conns
        for (int i = 0; i < CONFIG_BT_MAX_CONN; i++)
        {
                if(active_session.conns[i] == conn){
                        bt_conn_unref(active_session.conns[i]);
                        active_session.conns[i] = NULL;
                        active_session.count--;
                        LOG_INF("Slot %d cleared, Reason: %s", i, bt_hci_err_to_str(reason));
                        return;
                }
        }
        if(active_session.count < CONFIG_BT_MAX_CONN){
                k_work_reschedule(&restart_adv_work,K_MSEC(500));
        }
};
void recycled_cb(void){
};

void start_adv_work_handler(struct k_work *work){
        int ret = bt_le_adv_start(adv_param,ad,ARRAY_SIZE(ad),sd, ARRAY_SIZE(sd));
        if(!ret) {
                //TODO turn on corresponding led
        }
}
void stop_adv_work_handler(struct k_work *work){
        int ret = bt_le_adv_stop();
        if(!ret){
                //TODO turn off corresponding led
        }
}
void start_adv(void){k_work_submit(&start_adv_work);}
void stop_adv(void){k_work_submit(&stop_adv_work);}


int init_gap(void){
        int ret = bt_conn_cb_register(&conn_cb);
    if(ret < 0){
            LOG_ERR("Failed to register bt conn callbacks");
                return ret;
        }else{
                LOG_INF("Success to register bt conn callbacks");
                k_work_init(&start_adv_work, start_adv_work_handler);
                k_work_init(&stop_adv_work, stop_adv_work_handler);
                k_work_init_delayable(&restart_adv_work,start_adv_work_handler);
                return ret;
    };
}
