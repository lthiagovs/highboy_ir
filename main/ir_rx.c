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

//RECEIVE - FALTA FAZER O CALLBACK!!!
rmt_symbol_word_t* ir_rx_receive(){

    ir_rx_enable();

    if(!rx_enabled) return NULL;

    rmt_symbol_word_t raw_signal[64];

    //CONFIG RECEIVE
    rmt_receive_config_t receive_config = {
        .signal_range_min_ns = 1250, 
        .signal_range_max_ns = 12000000,
    };

    //START RECEIVE
    ESP_ERROR_CHECK(rmt_receive(rx_chan, raw_signal, sizeof(raw_signal), &receive_config));

    ESP_LOGW("IR RX:", "SINAL RECEBIDO");

    size_t size_signal = sizeof(raw_signal) / sizeof(rmt_symbol_word_t);

    print_signal(raw_signal, size_signal);

    ir_rx_disable();

    return raw_signal;
    
}