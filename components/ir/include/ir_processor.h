#ifndef IR_PROCESSOR_H
#define IR_PROCESSOR_H

#include <stdint.h>
#include <stddef.h>
#include "driver/rmt_rx.h"
#include "ir_decoder.h"

// Função principal para processar sinais IR
void process_ir_signal(const rmt_symbol_word_t* symbols, size_t count);

// Função principal para processar sinais IR
void process_ir_signal_filename(const rmt_symbol_word_t* symbols, size_t count, const char* custom_filename);

// Função para salvar arquivo .ir
bool save_ir_file(const char* protocol, const ir_decoded_data_t* decoded);

// Função para salvar arquivo .ir
bool save_ir_file_filename(const char* protocol, const ir_decoded_data_t* decoded, const char* custom_filename);

#endif // IR_PROCESSOR_H