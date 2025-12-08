#ifndef IR_PROTOCOL_H
#define IR_PROTOCOL_H

#include <driver/rmt_types.h>

#define IR_PROTOCOL_DECODE_MARGIN 200

typedef enum PROTOCOLS { IR_UNKNOW_PROTOCOL, IR_PROTOCOL_NEC, IR_PROTOCOL_RC6, IR_PROTOCOL_RC5, IR_PROTOCOL_SONY } IR_PROTOCOL;

typedef struct IR_DATA
{
    IR_PROTOCOL PROTOCOL;
    uint16_t ADDRESS;
    uint16_t COMMAND;
    bool REPEAT_FRAME;

} IR_DATA;


IR_PROTOCOL get_protocol(rmt_symbol_word_t* raw_data);
bool duration_in_range(uint32_t signal_duration, uint32_t spec_duration);

#endif