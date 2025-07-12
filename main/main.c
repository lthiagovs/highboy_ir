#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "ir_transmitter.h"
#include "ir_protocol.h"
#include "ir_encoder.h"
#include "ir_file.h"
#include "ir_path.h"

#define TAG "IR_TRANSMIT"

void app_main(void)
{
    ESP_LOGI(TAG, "🚀 Iniciando transmissor IR com arquivo .ir salvo");
    
    // Inicializar módulo de arquivos
    if (ir_path_init() != IR_PATH_OK) {
        ESP_LOGE(TAG, "❌ Falha ao inicializar SD card");
        return;
    }
    
    // Inicializar transmissor
    if (!rmt_tx_init()) {
        ESP_LOGE(TAG, "❌ Erro ao inicializar transmissor IR");
        return;
    }
    
    // Criar encoders
    rmt_encoder_handle_t copy_encoder = NULL;
    rmt_copy_encoder_config_t copy_encoder_config = {};
    esp_err_t err = rmt_new_copy_encoder(&copy_encoder_config, &copy_encoder);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "❌ Erro ao criar copy encoder: %s", esp_err_to_name(err));
        return;
    }
    
    ir_encoder_handle_t ir_encoder = NULL;
    ir_encoder_config_t ir_config = IR_ENCODER_DEFAULT_CONFIG();
    err = ir_encoder_new(&ir_config, &ir_encoder);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "❌ Erro ao criar IR encoder: %s", esp_err_to_name(err));
        return;
    }
    
    ESP_LOGI(TAG, "✅ Transmissor e encoders configurados");
    
    // Nome do arquivo .ir que foi salvo (ajuste conforme necessário)
    const char* ir_filename = "RC6_73.ir";
    
    // Ler arquivo .ir
    ESP_LOGI(TAG, "📖 Lendo arquivo: %s", ir_filename);
    
    char file_content[512];
    
    if (ir_path_read_file(ir_filename, file_content, sizeof(file_content)) == IR_PATH_OK) {
        size_t file_size = strlen(file_content);
        ESP_LOGI(TAG, "✅ Arquivo lido com sucesso (%zu bytes)", file_size);
        
        // Mostrar conteúdo do arquivo
        ESP_LOGI(TAG, "📄 === CONTEÚDO DO ARQUIVO ===");
        printf("%s\n", file_content);
        ESP_LOGI(TAG, "📄 === FIM DO ARQUIVO ===");
        
        // Parse do arquivo .ir
        ir_file_data_t file_data;
        if (ir_file_parse_string(file_content, &file_data)) {
            ESP_LOGI(TAG, "✅ Arquivo .ir parseado com sucesso!");
            ESP_LOGI(TAG, "📄 Nome: %s", file_data.name);
            ESP_LOGI(TAG, "📄 Tipo: %s", file_data.type);
            ESP_LOGI(TAG, "📄 Protocolo: %s", file_data.protocol);
            
            if (strcmp(file_data.type, "parsed") == 0) {
                ESP_LOGI(TAG, "📄 Endereço: 0x%08lX", (unsigned long)file_data.address);
                ESP_LOGI(TAG, "📄 Comando: 0x%08lX", (unsigned long)file_data.command);
            } else if (strcmp(file_data.type, "raw") == 0) {
                ESP_LOGI(TAG, "📄 Frequência: %luHz", (unsigned long)file_data.frequency);
                ESP_LOGI(TAG, "📄 Duty Cycle: %.2f%%", file_data.duty_cycle);
                ESP_LOGI(TAG, "📄 Dados RAW: %zu valores", file_data.raw_data_count);
            }
            
            // === TRANSMISSÃO ===
            ESP_LOGI(TAG, "\n📡 === INICIANDO TRANSMISSÃO ===");
            
            for (int i = 0; i < 5; i++) {
                ESP_LOGI(TAG, "📤 Transmissão %d/5: %s", i + 1, file_data.name);
                
                if (ir_file_transmit(&file_data, ir_encoder)) {
                    ESP_LOGI(TAG, "✅ Transmissão %d OK!", i + 1);
                } else {
                    ESP_LOGE(TAG, "❌ Falha na transmissão %d", i + 1);
                }
                
                // Delay entre transmissões
                vTaskDelay(pdMS_TO_TICKS(2000));
            }
            
            ESP_LOGI(TAG, "🎯 Todas as transmissões concluídas!");
            
            // Cleanup dados do arquivo
            ir_file_free_data(&file_data);
            
        } else {
            ESP_LOGE(TAG, "❌ Falha ao fazer parse do arquivo .ir");
        }
        
    } else {
        ESP_LOGE(TAG, "❌ Falha ao ler arquivo: %s", ir_filename);
        ESP_LOGI(TAG, "💡 Certifique-se de que o arquivo existe no cartão SD");
    }
    
    // === LIMPEZA ===
    ESP_LOGI(TAG, "\n🧹 Limpando recursos...");
    
    if (ir_encoder) {
        ir_encoder_del(ir_encoder);
        ESP_LOGI(TAG, "✅ IR encoder liberado");
    }
    
    if (copy_encoder) {
        rmt_del_encoder(copy_encoder);
        ESP_LOGI(TAG, "✅ Copy encoder liberado");
    }
    
    rmt_tx_delete();
    ESP_LOGI(TAG, "✅ Transmissor IR finalizado");
    
    ESP_LOGI(TAG, "🏁 Transmissão IR concluída!");
    
    // Loop infinito para manter a task viva
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}