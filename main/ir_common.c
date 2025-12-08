#include <esp_log.h>
#include "ir_common.h"
#include <driver/rmt_types.h>
#include "esp_mac.h"

void print_signal(rmt_symbol_word_t* raw_data, size_t count){
    if (!raw_data || count == 0) {
        ESP_LOGW("IR_RX", "printRaw: dados inválidos");
        return;
    }
    
    ESP_LOGI("IR_RX", "═══ Raw RMT Data (%d items) ═══", count);
    
    // Limitar a 50 para não sobrecarregar
    size_t max_display = (count > 50) ? 50 : count;
    
    for(size_t i = 0; i < max_display; i++){
        ESP_LOGI("IR_RX", "[%2d] D0:%5d D1:%5d L0:%d L1:%d", 
            i,
            raw_data[i].duration0,
            raw_data[i].duration1,
            raw_data[i].level0,
            raw_data[i].level1
        );
    }
    
    if (count > 50) {
        ESP_LOGI("IR_RX", "... (%d items omitidos)", count - 50);
    }
}