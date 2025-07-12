idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES "ir_file" "ir_path" "ir_encoder" "ir_protocol" "ir_transmitter" freertos 
)