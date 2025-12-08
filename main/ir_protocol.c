#include <esp_log.h>
#include "ir_protocol.h"
#include <driver/rmt_types.h>

IR_PROTOCOL get_protocol(rmt_symbol_word_t* raw_data){

    switch (sizeof(raw_data))
    {
    case 34:
        return IR_PROTOCOL_NEC;
        break;
    
    default:
        return IR_UNKNOW_PROTOCOL;
        break;
    }

}

bool duration_in_range(uint32_t signal_duration, uint32_t spec_duration){
    return(signal_duration < (spec_duration + IR_PROTOCOL_DECODE_MARGIN) &&
           signal_duration > (spec_duration - IR_PROTOCOL_DECODE_MARGIN));
}