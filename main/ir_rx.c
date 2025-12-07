#include <esp_log.h>
#include <driver/rmt_rx.h>
#include "ir_common.h"
#include "ir_rx.h"

static rmt_channel_handle_t rx_chan = NULL;
static bool rx_enabled = false;

void ir_rx_init() {

    rmt_rx_channel_config_t rx_chan_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = 1 * 1000 * 1000,
        .mem_block_symbols = 64,
        .gpio_num = IR_RX_GPIO,
        .flags.invert_in = IR_INVERT_IN,
        .flags.with_dma = false
    };

    //1 - START RX CHANNEL
    ESP_ERROR_CHECK(rmt_new_rx_channel(&rx_chan_config, &rx_chan));

    //2 - CONFIG CARRIER
    ir_rx_carrier_config();

    //3 - ENABLE RMT
    ir_rx_enable();

}

void ir_rx_delete(){

    if(rx_enabled) ir_rx_disable();
    ESP_ERROR_CHECK(rmt_del_channel(rx_chan));

}

void ir_rx_carrier_config(){

    rmt_carrier_config_t rx_carrier_cfg = {
        .duty_cycle = 0.33,
        .frequency_hz = 25000,
        .flags.polarity_active_low = false
    };

    ESP_ERROR_CHECK(rmt_apply_carrier(rx_chan, &rx_carrier_cfg));

}

void ir_rx_enable(){

    if(!rx_enabled){
        ESP_ERROR_CHECK(rmt_enable(rx_chan));
        rx_enabled = true;
    }

}

void ir_rx_disable(){

    if(rx_enabled){
        ESP_ERROR_CHECK(rmt_disable(rx_chan));
        rx_enabled = false;
    }

}