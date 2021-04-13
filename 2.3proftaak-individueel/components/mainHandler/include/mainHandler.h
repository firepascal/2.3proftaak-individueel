#ifndef mainHandler_H
#define mainHandler_H

#include "esp_peripherals.h"
#include "esp_wifi.h"
#include "board.h"
#include "periph_wifi.h"

/**
 * @brief Structure containing information of the board which needs to be enabled only once
 *        but accessed by multiple components
 */ 
typedef struct{
    esp_periph_set_handle_t set;        ///< The ESP Peripherals
    audio_board_handle_t board_handle;  ///< Board handle for keys and audio pipeline
    esp_periph_handle_t wifi_handler;   ///< Wifi handler in case wifi is used
} main_handler_t;

/** 
 * @brief Initializes the main settings 
 * @param[in] The struct to contain the settings
 *
 */
void mainHandler_init(main_handler_t * main_handler);

#endif