#ifndef IR_COMMOM_H
#define IR_COMMOM_H

#include <esp_mac.h>
#include <driver/rmt_types.h>

#define IR_RX_GPIO 1
#define IR_TX_GPIO 2

#define IR_INVERT_OUT false
#define IR_INVERT_IN false

void print_signal(rmt_symbol_word_t* raw_data, size_t count);

#endif