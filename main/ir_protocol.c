#include <esp_log.h>
#include <stdio.h>
#include <string.h>
#include <driver/rmt_types.h>
#include "ir_protocol.h"

//PROTOCOLS
#include "ir_protocol_nec.h"
//PROTOCOLS

IR_PROTOCOL get_protocol(size_t signal_size){

    switch (signal_size)
    {
    case 34:
        return IR_PROTOCOL_NEC;
        break;

        
    case 21:
        return IR_PROTOCOL_RC6;
        break;
    
    default:
        return IR_UNKNOW_PROTOCOL;
        break;
    }

}

const char* protocol_to_string(IR_PROTOCOL p) {
    switch(p) {
        case IR_PROTOCOL_NEC: return "NEC";
        default: return "Unknown";
    }
}

void irdata_to_string(const IR_DATA *data, char *out, size_t out_size) {
    uint8_t addr = data->ADDRESS & 0xFF;
    uint8_t addr_inv = (data->ADDRESS >> 8) & 0xFF;
    uint8_t cmd = data->COMMAND & 0xFF;
    uint8_t cmd_inv = (data->COMMAND >> 8) & 0xFF;
    
    snprintf(out, out_size,
        "\naddress: 0x%02X (~0x%02X)\n"
        "command: 0x%02X (~0x%02X)\n",
        addr, addr_inv,
        cmd, cmd_inv
    );
}


IR_PROTOCOL save_signal(rmt_symbol_word_t* raw_data, size_t count){

    IR_PROTOCOL signal_protocol = get_protocol(count);
    switch (signal_protocol) {
    case IR_PROTOCOL_NEC:

        IR_DATA* data = nec_parse_frame(raw_data);
        if (data) {
            char out[128];
            irdata_to_string(data, out, sizeof(out));
            ESP_LOGW("PROTOCOL", "%s", out);
            free(data);
        }else{
            ESP_LOGW("PROTOCOL", "FALHA");
        }

        break;
    
    default:
        break;
    }

    ESP_LOGW("PROTOCOL","%s",protocol_to_string(signal_protocol));

    return IR_PROTOCOL_NEC;
}

bool duration_in_range(uint32_t signal_duration, uint32_t spec_duration){
    return(signal_duration < (spec_duration + IR_PROTOCOL_DECODE_MARGIN) &&
           signal_duration > (spec_duration - IR_PROTOCOL_DECODE_MARGIN));
}