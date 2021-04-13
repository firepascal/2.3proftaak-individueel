#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "i2c-lcd1602.h"
#include "lcd.h"
#include "smbus.h"
#include <math.h>

#define TAG "lcd"

// Undefine USE_STDIN if no stdin is available (e.g. no USB UART) - a fixed delay will occur instead of a wait for a keypress.
#undef USE_STDIN

// General LCD information
#define I2C_MASTER_NUM           0
#define LCD_NUM_ROWS			 4
#define LCD_NUM_COLUMNS			 40
#define LCD_NUM_VIS_COLUMNS		 20

// Prototypes
void lcd_init_task(void * pvParameter);

// Attributes
i2c_lcd1602_info_t* info_lcd;

void lcd_init_task(void * pvParameter)
{   
    // Set up the SMBus
    i2c_port_t i2c_num = I2C_MASTER_NUM;
    uint8_t address = CONFIG_LCD1602_I2C_ADDRESS;
    smbus_info_t * smbus_info = smbus_malloc();
    smbus_init(smbus_info, i2c_num, address);
    smbus_set_timeout(smbus_info, 1000 / portTICK_RATE_MS);

    // Set up the LCD1602 device with backlight on
    info_lcd = i2c_lcd1602_malloc();
    i2c_lcd1602_init(info_lcd, smbus_info, true, LCD_NUM_ROWS, LCD_NUM_COLUMNS, LCD_NUM_VIS_COLUMNS);
    vTaskDelay(100 / portTICK_RATE_MS);

    // Turn cursor visibility off
    ESP_LOGI(TAG, "cursor off");
    i2c_lcd1602_set_cursor(info_lcd, false);
    i2c_lcd1602_set_blink(info_lcd, false);

    //Define custom characters
    ESP_LOGI(TAG, "define characters");
    uint8_t floor[8] = {0x1F, 0x1F, 0x14, 0x9, 0x12, 0x5, 0xA, 0x14};
    uint8_t not_selected[8] = {0x0, 0x0, 0x6, 0x9, 0x9, 0x6, 0x0, 0x0}; 
    uint8_t selected[8] = {0x0, 0x0, 0x6, 0xF, 0xF, 0x6, 0x0, 0x0}; 
    uint8_t player[8] = {0x4, 0xA, 0x4, 0x1F, 0x4, 0x4, 0xA, 0xA};
    uint8_t cactus[8] = {0x0, 0x4, 0x1E, 0xF, 0x1E, 0xF, 0x1E, 0xE};

    i2c_lcd1602_define_char(info_lcd, I2C_LCD1602_CHARACTER_CUSTOM_0, floor);
    i2c_lcd1602_define_char(info_lcd, I2C_LCD1602_CHARACTER_CUSTOM_1, not_selected);
    i2c_lcd1602_define_char(info_lcd, I2C_LCD1602_CHARACTER_CUSTOM_2, selected);
    i2c_lcd1602_define_char(info_lcd, I2C_LCD1602_CHARACTER_CUSTOM_3, player);
    i2c_lcd1602_define_char(info_lcd, I2C_LCD1602_CHARACTER_CUSTOM_4, cactus);
 
    // Delete finished task
    vTaskDelete(NULL);
}

void lcd_write_menu(MENU_ITEM_STRUCT *menu, int menus_in_loop)
{
    ESP_LOGI(TAG, "writing menu");
    i2c_lcd1602_clear(info_lcd);

    //Line 1
    i2c_lcd1602_move_cursor(info_lcd, 0,0);
    if (menu->scrollable){i2c_lcd1602_write_char(info_lcd, I2C_LCD1602_CHARACTER_ARROW_RIGHT);}
    i2c_lcd1602_write_string(info_lcd, menu->line1);

    //Line 2
    i2c_lcd1602_move_cursor(info_lcd, 0,1);
    if (menu->scrollable){i2c_lcd1602_write_char(info_lcd, ' ');}
    i2c_lcd1602_write_string(info_lcd, menu->line2);
    
    //Line 3
    i2c_lcd1602_move_cursor(info_lcd, 0,2);
    if (menu->scrollable){i2c_lcd1602_write_char(info_lcd, ' ');}
    i2c_lcd1602_write_string(lcd_info, menu->line3);

    //Line 4
    i2c_lcd1602_move_cursor(info_lcd, 0,3);
    i2c_lcd1602_write_string(info_lcd, menu->menu_name);

    if (menu->id != -1)  //Dont write the menu bubbles if the menu is not in the navigation loop    
    {
        i2c_lcd1602_move_cursor(lcd_info, 20 - menus_in_loop, 3);
        for (int i = 0; i < menus_in_loop; i++)
        {
            if(menu->id - i == 0){
                i2c_lcd1602_write_char(info_lcd, I2C_LCD1602_CHARACTER_CUSTOM_2);    
            }  else{
                i2c_lcd1602_write_char(info_lcd, I2C_LCD1602_CHARACTER_CUSTOM_1);    
            }
        }   
    } 
}

void lcd_write_game(info_game *info_game)
{
    ESP_LOGI(TAG, "Writing Game");
    i2c_lcd1602_clear(info_lcd);

    //Write player
    i2c_lcd1602_move_cursor(info_lcd, info_game->player_location.x, info_game->player_location.y);
    i2c_lcd1602_write_char(info_lcd, I2C_LCD1602_CHARACTER_CUSTOM_3);

    //Write obstacle
    i2c_lcd1602_move_cursor(info_lcd, info_game->obstacle.x, info_game->obstacle.y);
    i2c_lcd1602_write_char(info_lcd, I2C_LCD1602_CHARACTER_CUSTOM_4);

    //Write floor
    i2c_lcd1602_move_cursor(info_lcd, 0, 3);
    for (size_t i = 0; i < LCD_NUM_VIS_COLUMNS; i++)
    {
        i2c_lcd1602_write_char(info_lcd, I2C_LCD1602_CHARACTER_CUSTOM_0);
    }

    //Write score
    char strbuffer[LCD_NUM_VIS_COLUMNS]; 
    sprintf(strbuffer, "Score: %i", game_info->current_score);
    i2c_lcd1602_move_cursor(info_lcd, 0, 0);
    i2c_lcd1602_write_string(info_lcd, strbuffer);  
}

void lcd_init()
{
    ESP_LOGI(TAG, "Init LCD");
    xTaskCreate(&lcd_init_task, "lcd1602_task", 3*1024, NULL, 3, NULL);
    vTaskDelay(250 / portTICK_RATE_MS); 
}