#include <esp_log.h>
#include <stdio.h>
#include <string.h>
#include "ir_protocol.h"
#include "ir_protocol_rc6.h"

bool rc6_validate_leading_code(rmt_symbol_word_t *raw_data){
    
    return duration_in_range(raw_data->duration0, RC6_LEADER_MARK) &&
           duration_in_range(raw_data->duration1, RC6_LEADER_SPACE);

}

bool rc6_validate_start_bit(rmt_symbol_word_t *raw_data){

    return duration_in_range(raw_data->duration0, RC6_START_BIT_MARK) &&
           duration_in_range(raw_data->duration1, RC6_START_BIT_SPACE);

}

bool rc6_parse_logic0(rmt_symbol_word_t *raw_data){
    
    return (duration_in_range(raw_data->duration0, RC6_BIT_SPACE) &&
            duration_in_range(raw_data->duration1, RC6_BIT_MARK));

}

bool rc6_parse_logic1(rmt_symbol_word_t *raw_data){

    return (duration_in_range(raw_data->duration0, RC6_BIT_MARK) &&
            duration_in_range(raw_data->duration1, RC6_BIT_SPACE));

}

IR_DATA* parse_rc6_frame(rmt_symbol_word_t *raw_data){

    rmt_symbol_word_t *cur = raw_data;
    IR_DATA* rc6_frame = malloc(sizeof(IR_DATA));
    if(!rc6_frame) return NULL;

    memset(rc6_frame, 0, sizeof(IR_DATA));

    rc6_frame->PROTOCOL = IR_PROTOCOL_RC6;
    rc6_frame->REPEAT_FRAME = false;
    snprintf(rc6_frame->NAME, sizeof(rc6_frame->NAME), "RC6_FRAME");

    if(!rc6_validate_leading_code(raw_data)){
        free(rc6_frame);
        return NULL;
    }

    cur++;

    if(!rc6_validate_start_bit(raw_data)){
        free(rc6_frame);
        return NULL;
    }

    //lets skip MODE & TOGGLE FOR NOW
    for(int j = 0; j < 5; j++) cur++;

    for(int i = 0; i < 8; i++){
        if(rc6_parse_logic0(cur)){
            rc6_frame->COMMAND |= 1 << i;
        }else if(rc6_parse_logic1(cur)){
            rc6_frame->COMMAND &= ~(1 << i);
        }else{
            return NULL;
        }
    }

    for(int i = 0; i < 8; i++){
        if(rc6_parse_logic0(cur)){
            rc6_frame->COMMAND |= 1 << i;
        }else if(rc6_parse_logic1(cur)){
            rc6_frame->COMMAND &= ~(1 << i);
        }else{
            return NULL;
        }
    }

    return rc6_frame;
    
}