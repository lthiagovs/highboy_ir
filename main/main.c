#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <esp_log.h>
#include "ir_rx.h"

int app_main(){

    ir_rx_init();

    while (1) {
        ir_rx_receive();
        vTaskDelay(pdMS_TO_TICKS(1000));
        ESP_LOGI("CLEAR", "\e[1;1H\e[2J");
    }

    return 0;

}