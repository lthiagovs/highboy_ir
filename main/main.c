#include <stdio.h>
#include <string.h>
#include <time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "ir_receiver.h"
#include "ir_path.h"
#include "ir_manager.h"

#define TAG "MAIN"

void app_main(void)
{
    ESP_LOGI(TAG, "Transmitindo arquivo RC6_73.ir");
    
    // Chama a função para transmitir o arquivo
    if (ir_transmit_file("RC6_73.ir")) {
        ESP_LOGI(TAG, "Transmissão do arquivo RC6_73.ir concluída com sucesso!");
    } else {
        ESP_LOGE(TAG, "Falha na transmissão do arquivo RC6_73.ir");
    }
    
    ESP_LOGI(TAG, "Aplicação finalizada");
    
    // Loop infinito ou outras tarefas
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}