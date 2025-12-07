#include <esp_log.h>
#include <driver/rmt_tx.h>
#include "ir_common.h"
#include "ir_tx.h"

static rmt_channel_handle_t tx_chan = NULL;
static bool tx_enabled = false;

void ir_tx_init(){

    rmt_tx_channel_config_t tx_chan_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .gpio_num = IR_TX_GPIO,
        .mem_block_symbols = 64,
        .resolution_hz = 1 * 1000 * 1000,
        .trans_queue_depth = 4,
        .flags.invert_out = IR_INVERT_OUT,
        .flags.with_dma = false
    };

    //1 - START TX CHANNEL
    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_chan_config, &tx_chan));

    //2 - CONFIG CARRIER
    ir_tx_carrier_config();

    //3 - ENABLE RMT
    ir_tx_enable();

}

void ir_tx_delete(){

    if(tx_enabled) ir_tx_disable();
    ESP_ERROR_CHECK(rmt_del_channel(tx_chan));

}

void ir_tx_carrier_config(){

    rmt_carrier_config_t tx_carrier_cfg ={
        .duty_cycle = 0.33,
        .frequency_hz = 38000,
        .flags.polarity_active_low = false
    };

    ESP_ERROR_CHECK(rmt_apply_carrier(tx_chan, &tx_carrier_cfg));

}

void ir_tx_enable(){

    if(!tx_enabled){
        ESP_ERROR_CHECK(rmt_enable(tx_chan));
        tx_enabled = true;
    }

}

void ir_tx_disable(){

    if(tx_enabled){
        ESP_ERROR_CHECK(rmt_disable(tx_chan));
        tx_enabled = false;
    }

}