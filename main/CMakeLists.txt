idf_component_register(SRCS "ir_tx.c" "ir_rx.c" "main.c"
                       PRIV_REQUIRES esp_driver_rmt
                       INCLUDE_DIRS ".")
