#ifndef IR_PROTOCOL_RC6_H
#define IR_PROTOCOL_RC6_H

#define RC6_BASE_TIME 444

#define RC6_LEADER_MARK (RC6_BASE_TIME*6)
#define RC6_LEADER_SPACE (RC6_BASE_TIME*2)
#define RC6_START_BIT_MARK (RC6_BASE_TIME*2)
#define RC6_START_BIT_SPACE (RC6_BASE_TIME*2)
#define RC6_BIT_MARK (RC6_BASE_TIME*1)
#define RC6_BIT_SPACE (RC6_BASE_TIME*1)
#define RC6_TRAILER_BIT_MARK (RC6_BASE_TIME*2)
#define RC6_TRAILER_BIT_SPACE (RC6_BASE_TIME*2)
#define RC6_FREE_GAP (RC6_BASE_TIME*6)

bool rc6_validate_leading_code(rmt_symbol_word_t *raw_data);
bool rc6_validate_start_bit(rmt_symbol_word_t *raw_data);

bool rc6_parse_logic0(rmt_symbol_word_t *raw_data);
bool rc6_parse_logic1(rmt_symbol_word_t *raw_data);

#endif