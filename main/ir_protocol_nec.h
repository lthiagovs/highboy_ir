#ifndef IR_PROTOCOL_NEC_H
#define IR_PROTOCOL_NEC_H

#define NEC_LEADING_CODE_DURATION_0  9000
#define NEC_LEADING_CODE_DURATION_1  4500
#define NEC_PAYLOAD_ZERO_DURATION_0  560
#define NEC_PAYLOAD_ZERO_DURATION_1  560
#define NEC_PAYLOAD_ONE_DURATION_0   560
#define NEC_PAYLOAD_ONE_DURATION_1   1690
#define NEC_REPEAT_CODE_DURATION_0   9000
#define NEC_REPEAT_CODE_DURATION_1   2250

bool nec_validate_leading_code(rmt_symbol_word_t *raw_data);
bool nec_parse_logic0(rmt_symbol_word_t *raw_data);
bool nec_parse_logic1(rmt_symbol_word_t *raw_data);
IR_DATA* nec_parse_frame(rmt_symbol_word_t *raw_data);

#endif