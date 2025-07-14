#include <stdio.h>
#include <string.h>
#include <time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "ir_receiver.h"
#include "ir_path.h"
#include "ir_manager.h"

#define TAG "MAIN"

void app_main(void)
{
    static rmt_symbol_word_t raw_symbols[MEM_BLOCK_SYMBOLS];
    ir_receive_file("TESTANDO.ir", raw_symbols, 5000);
}