#include "mainHandler.h"
#include "esp_log.h"
#include <string.h>
#include "nvs_flash.h"

#define TAG "mainhandler"

// Prototypes
void mainHandler_init_board(main_handler_t * main_handler);
void mainHandler_init_wifi(main_handler_t * main_handler);
void nvs_init();

void mainHandler_init(main_handler_t * main_handler)
{
    nvs_init();
    mainHandler_init_board(main_handler);
    //mainHandler_init_wifi(main_handler);  //Uncomment to enable WIFI
}

/**
 * @brief initializes the board, keys and peripherals
 * @param[in] The struct to contain the settings
 * 
 */ 
void mainHandler_init_board(main_handler_t * main_handler)
{
    ESP_LOGI(TAG, "Initialize peripherals management");
    esp_periph_config_t periph_cfg = DEFAULT_ESP_PERIPH_SET_CONFIG();
    main_handler->set = esp_periph_set_init(&periph_cfg);

    ESP_LOGI(TAG, "Initialize and start peripherals");
    audio_board_key_init(main_handler->set);

    ESP_LOGI(TAG, "Start codec chip");
    main_handler->board_handle = audio_board_init();
    audio_hal_ctrl_codec(main_handler->board_handle->audio_hal, AUDIO_HAL_CODEC_MODE_DECODE, AUDIO_HAL_CTRL_START);
}

/**
 * @brief Initializes the wifi and connects to the network set in config
 * @param[in] The struct to contain the settings
 * 
 */
void mainHandler_init_wifi(main_handler_t * main_handler)
{
    ESP_LOGI(TAG, "Initialize the tcp/ip adapter");
    tcpip_adapter_init();

    ESP_LOGI(TAG, "Log in to current network");
    periph_wifi_cfg_t wifi_cfg = 
    {
        .ssid = CONFIG_WIFI_SSID,
        .password = CONFIG_WIFI_PASSWORD,
    };

	if (strlen(CONFIG_WIFI_IDENTITY) > 0)
    {
		wifi_cfg.identity = strdup(CONFIG_WIFI_IDENTITY);
    }

    ESP_LOGI(TAG, "Connect to the interwebs");
    main_handler->wifi_handler = periph_wifi_init(&wifi_cfg);
    esp_periph_start(main_handler->set, main_handler->wifi_handler);
    periph_wifi_wait_for_connected(main_handler->wifi_handler, portMAX_DELAY);
}

/**
 * @brief Starts the NVS, required to use wifi
 */ 
void nvs_init()
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES) 
    {
        nvs_flash_erase();
        err = nvs_flash_init();
    }
}
