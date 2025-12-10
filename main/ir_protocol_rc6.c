#include <esp_log.h>
#include <stdio.h>
#include <string.h>
#include "ir_protocol.h"
#include "ir_protocol_rc6.h"

bool rc6_validate_leading_code(rmt_symbol_word_t *raw_data){
    
    return duration_in_range(raw_data->duration0, RC6_LEADER_MARK) &&
           duration_in_range(raw_data->duration1, RC6_LEADER_SPACE);

}

bool rc6_parse_logic0(rmt_symbol_word_t *raw_data){
    if(raw_data->duration0 > 700) return true;  // bit mesclado
    
    int32_t diff = raw_data->duration1 - raw_data->duration0;
    
    if(abs(diff) < 20) {
        return (raw_data->duration0 < 460);
    }
    
    return (diff > 0);  // D1 > D0
}

bool rc6_parse_logic1(rmt_symbol_word_t *raw_data){
    if(raw_data->duration1 > 700) return true;  // bit mesclado
    
    int32_t diff = raw_data->duration0 - raw_data->duration1;
    
    if(abs(diff) < 20) {

        return (raw_data->duration0 > 460);
    }
    
    return (diff > 0);  // D0 > D1
}

IR_DATA* rc6_parse_frame(rmt_symbol_word_t *raw_data){
    rmt_symbol_word_t *cur = raw_data;
    IR_DATA* rc6_frame = malloc(sizeof(IR_DATA));
    
    if(!rc6_frame) return NULL;
    
    memset(rc6_frame, 0, sizeof(IR_DATA));
    rc6_frame->PROTOCOL = IR_PROTOCOL_RC6;
    rc6_frame->REPEAT_FRAME = false;
    snprintf(rc6_frame->NAME, sizeof(rc6_frame->NAME), "RC6_FRAME");
    
    // Validar leader (índice 0)
    if(!rc6_validate_leading_code(cur)){
        ESP_LOGE("RC6", "Leader validation failed!");
        free(rc6_frame);
        return NULL;
    }
    cur++;  // > 1
    
    // Pular start bit
    cur++;  // > 2
    
    // n quero mode
    for(int j = 0; j < 3; j++) cur++;  // > 5
    
    // nem toggle
    cur++;  // > 6
    
    ESP_LOGI("RC6", "Starting ADDRESS parse at index 6");
    
    for(int i = 0; i < 8; i++){
        if(rc6_parse_logic1(cur)){
            rc6_frame->ADDRESS |= (1 << (7 - i));
        }else if(!rc6_parse_logic0(cur)){
            free(rc6_frame);
            return NULL;
        }
        cur++;
    }
    
    ESP_LOGI("RC6", "ADDRESS = 0x%02X, Starting COMMAND parse", rc6_frame->ADDRESS);
    
    for(int i = 0; i < 7; i++){
        if(rc6_parse_logic1(cur)){
            rc6_frame->COMMAND |= (1 << (6 - i));
        }else if(!rc6_parse_logic0(cur)){
            free(rc6_frame);
            return NULL;
        }
        cur++;
    }
    
    ESP_LOGI("RC6", "COMMAND = 0x%02X", rc6_frame->COMMAND);
    
    return rc6_frame;
}