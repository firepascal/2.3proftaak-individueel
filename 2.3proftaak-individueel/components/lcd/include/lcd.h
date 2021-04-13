#ifndef LCD_H
#define LCD_H

#include <stdbool.h>

/**
 *  @brief Structure containing all information about a menu
 */
typedef struct{
    int id;                                 ///< The ID of the menu
    int nextid;                             ///< The next menu to navigate to when menu_next() is called
    char menu_name[17];                     ///< Name of the menu, shown in the bottom left of the LCD
    char line1[20];                         ///< Text shown on the first line
    char line2[20];                         ///< Text shown in the second line
    char line3[20];                         ///< Text shown in the third line
    bool scrollable;                        ///< Set to TRUE if the menu can be scrolled through
} MENU_ITEM_STRUCT;

/**
 * combines the x and y position in 1 struct
 */ 
typedef struct{
    int x;                                  ///< The x location 
    int y;                                  ///< The y location 
} Simple_Coordinate;

/**
 * @brief Structure containing all information about the current game
 */ 
typedef struct {
    int current_score;                      ///< The amount of points the player has
    Simple_Coordinate player_location;      ///< The x and y position of the player
    Simple_Coordinate obstacle;             ///< The x and y position of the object the player needs to jump over
} info_game;

/**
 * @brief Takes a structure containing information about the menu and writes this information
 *        on the LCD in the right format
 * @param[in] The structure with information which needs to be written on the LCD
 * @param[in] The amount of menus in the main loop, set to -1 to disable for the current menu 
 */ 
void lcd_write_menu(MENU_ITEM_STRUCT *menu, int menus_in_loop);

/**
 * @brief       Takes the information about the game and writes this on the LCD in the right format
 * @param[in]   The structure containing information about the game
 */ 
void lcd_write_game(Game_Info *game_info);

/**
 * @brief Initializes the LCD module
 */ 
void lcd_init();

#endif