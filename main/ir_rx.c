#include <esp_log.h>
#include <driver/rmt_rx.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "ir_common.h"
#include "ir_protocol.h"
#include "ir_rx.h"

static rmt_channel_handle_t rx_chan = NULL;
rmt_rx_channel_config_t rx_chan_config;
static bool rx_enabled = false;
static QueueSetHandle_t receive_queue;

bool rmt_rx_callback(rmt_channel_handle_t rx_chan, const rmt_rx_done_event_data_t *edata, void *user_data){

    BaseType_t high_task_wakeup = pdFALSE;
    QueueHandle_t receive_queue = (QueueHandle_t)user_data;
    xQueueSendFromISR(receive_queue, edata, &high_task_wakeup);
    return high_task_wakeup == pdTRUE;

}

void ir_rx_init() {

    rx_chan_config = (rmt_rx_channel_config_t){
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = 1 * 1000 * 1000,
        .mem_block_symbols = 64,
        .gpio_num = IR_RX_GPIO,

 
        .flags.invert_in = IR_INVERT_IN,
        .flags.with_dma = false
    };

    //1 - START RX CHANNEL
    ESP_ERROR_CHECK(rmt_new_rx_channel(&rx_chan_config, &rx_chan));

    //2 - REGISTER CALLBACK
    receive_queue = xQueueCreate(1, sizeof(rmt_rx_done_event_data_t));
    assert(receive_queue);
    rmt_rx_event_callbacks_t cbs = {
        .on_recv_done = rmt_rx_callback
    };
    ESP_ERROR_CHECK(rmt_rx_register_event_callbacks(rx_chan, &cbs, receive_queue));

    //3 - CONFIG CARRIER
    //ir_rx_carrier_config();

}

void ir_rx_delete(){

    if(rx_enabled) ir_rx_disable();
    ESP_ERROR_CHECK(rmt_del_channel(rx_chan));

}

void ir_rx_carrier_config(){

    rmt_carrier_config_t rx_carrier_cfg = {
        .duty_cycle = 0.33,
        .frequency_hz = 38000,
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

rmt_symbol_word_t* ir_rx_receive(){

    ir_rx_enable();
    ///

    rmt_symbol_word_t raw_symbols[64];
    rmt_rx_done_event_data_t rx_data;

    rmt_receive_config_t receive_config = {
        .signal_range_min_ns = 1250, 
        .signal_range_max_ns = 12000000,
    };

    ESP_ERROR_CHECK(rmt_receive(rx_chan, raw_symbols, sizeof(raw_symbols), &receive_config));

    if (xQueueReceive(receive_queue, &rx_data, pdMS_TO_TICKS(1000)) == pdPASS) {

        //HANDLE THE SIGNAL
        
        print_signal(rx_data.received_symbols, rx_data.num_symbols);
        save_signal(rx_data.received_symbols, rx_data.num_symbols);

        //HANDLE THE SIGNAL

        ESP_ERROR_CHECK(rmt_receive(rx_chan, raw_symbols, sizeof(raw_symbols), &receive_config));

    }

    ///
    ir_rx_disable();
    return raw_symbols;

}