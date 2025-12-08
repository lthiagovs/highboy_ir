#include <esp_log.h>
#include "ir_protocol.h"
#include "ir_protocol_nec.h"

bool nec_parse_logic0(rmt_symbol_word_t *raw_data){
    return duration_in_range(raw_data->duration0, NEC_PAYLOAD_ZERO_DURATION_0) && 
           duration_in_range(raw_data->duration1, NEC_PAYLOAD_ZERO_DURATION_1);
}

bool nec_parse_logic1(rmt_symbol_word_t *raw_data){
    return duration_in_range(raw_data->duration0, NEC_PAYLOAD_ONE_DURATION_0) &&
           duration_in_range(raw_data->duration1, NEC_PAYLOAD_ONE_DURATION_1);
}

bool nec_validate_leading_code(rmt_symbol_word_t *raw_data){

    return duration_in_range(raw_data->duration0, NEC_LEADING_CODE_DURATION_0) &&
           duration_in_range(raw_data->duration1, NEC_LEADING_CODE_DURATION_1);

}

IR_DATA* nec_parse_frame(rmt_symbol_word_t *raw_data){

    rmt_symbol_word_t *cur = raw_data;
    IR_DATA nec_frame;
    nec_frame.PROTOCOL = IR_PROTOCOL_NEC;
    nec_frame.ADDRESS = 0;
    nec_frame.COMMAND = 0;
    nec_frame.REPEAT_FRAME = false;

    cur++;

    if(!nec_validate_leading_code(raw_data)) return NULL;

    for (int i = 0; i < 16; i++) {
        if (nec_parse_logic1(cur)) {
            nec_frame.ADDRESS |= 1 << i;
        } else if (nec_parse_logic0(cur)) {
            nec_frame.ADDRESS &= ~(1 << i);
        } else {
            return false;
        }
        cur++;
    }

    for (int i = 0; i < 16; i++) {
        if (nec_parse_logic1(cur)) {
            nec_frame.COMMAND |= 1 << i;
        } else if (nec_parse_logic0(cur)) {
            nec_frame.COMMAND &= ~(1 << i);
        } else {
            return false;
        }
        cur++;
    }

    return &nec_frame;

}
