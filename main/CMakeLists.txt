idf_component_register(SRCS "ir_protocol_nec.c" "ir_protocol.c" "ir_tx.c" "ir_rx.c" "ir_common.c" "main.c"
                       PRIV_REQUIRES esp_driver_rmt
                       INCLUDE_DIRS ".")
