set(COMPONENT_ADD_INCLUDEDIRS include)
set(COMPONENT_SRCS "buttonhandler.c")
set(COMPONENT_PRIV_REQUIRES menu mainHandler esp_peripherals input_key_service game)
register_component()