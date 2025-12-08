#ifndef IR_RX_H
#define IR_RX_H

#include <driver/rmt_types.h>

//manel
void ir_rx_init();
void ir_rx_delete();
void ir_rx_carrier_config();
void ir_rx_enable();
void ir_rx_disable();

rmt_symbol_word_t* ir_rx_receive();

#endif