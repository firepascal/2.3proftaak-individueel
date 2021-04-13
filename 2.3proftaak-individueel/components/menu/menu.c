#include "lcd.h"
#include "menu.h"
#include "esp_log.h"
#include <string.h>
#include "game.h"
#include <stdio.h> 

#define TAG "menu"
#define MENU_MAIN_1 0               ///< Main menu
#define MENU_MAIN_2 1               ///< Score list
#define MENU_EXPLANATION 2          ///< Explanation Menu
#define MAIN_MENU_AMOUNT 2          ///< The amount of menus in the main menu loop

#define MAX_HIGHSCORE_SLOTS 5       ///< Maximum amount of highscores that can be saved
#define MAX_ITEMS_MENU_MAIN_1 3     ///< Amount of items in the main menu
#define MAX_STRING_LENGTH 20        ///< The maximum string length of any item in menus

//Attributes
int menu_state;                     ///< Tracks which menu is currently visible
int scroll_state;                   ///< Tracks which position the currently visible menu is in

char game_menu[MAX_ITEMS_MENU_MAIN_1][MAX_STRING_LENGTH] = {"Start","Reset Highscores", "Explanation"};
char scores[MAX_HIGHSCORE_SLOTS][MAX_STRING_LENGTH] = {"<Empty>", "<Empty>", "<Empty>", "<Empty>", "<Empty>"};

// Struct with all the menus
MENU_ITEM_STRUCT menus[] = 
{
    //id, nextid, name, line1, line2, line3, scrollable
    {
        MENU_MAIN_1,
        MENU_MAIN_2,
        "Game",
        "",
        "",
        "",
        true
    },
    {
        MENU_MAIN_2,
        MENU_MAIN_1,
        "Highscores",
        "",
        "",
        "",
        true
    },
    {
        MENU_EXPLANATION,
        MENU_MAIN_1,
        "Explanation",
        "Press Vol+ or Vol-",
        "to Jump. Avoid the",
        "Cactuses",
        false
    }
};

void menu_init()
{
    ESP_LOGI(TAG, "menu_init");

    //Initialise LCD
    lcd_init();

    //Initialise attributes
    menu_state = 0;
    scroll_state = 0;

    //Fill menu lists
    strcpy(menus[MENU_MAIN_1].line1, game_menu[0]);
    strcpy(menus[MENU_MAIN_1].line2, game_menu[1]);
    strcpy(menus[MENU_MAIN_1].line3, game_menu[2]);

    strcpy(menus[MENU_MAIN_2].line1, scores[0]);
    strcpy(menus[MENU_MAIN_2].line2, scores[1]);
    strcpy(menus[MENU_MAIN_2].line3, scores[2]);

    //Write the first menu on the LCD
    lcd_write_menu(&menus[menu_state], MAIN_MENU_AMOUNT);
}

void menu_next()
{
    ESP_LOGI(TAG, "menu_next");
    menu_state = menus[menu_state].nextid;
    scroll_state = 0;
    lcd_write_menu(&menus[menu_state], MAIN_MENU_AMOUNT);
}

void menu_add_score(int score)
{
    ESP_LOGI(TAG, "menu_add_highscore %d", (int)score);
    
    for (int i = 0; i < MAX_HIGHSCORE_SLOTS; i++)
    {
        if ( strcmp(scores[i], "<Empty>") == 0 )
        {
            char strbuffer[20] = "";
            sprintf(strbuffer, "%i", score);
            strcpy(scores[i], strbuffer);

            int arr_size = (sizeof(scores) / sizeof(scores[0]));
            strcpy(menus[MENU_MAIN_2].line1, scores[scroll_state]);
            strcpy(menus[MENU_MAIN_2].line2, scores[(scroll_state + 1) % arr_size]);
            strcpy(menus[MENU_MAIN_2].line3, scores[(scroll_state + 2) % arr_size]);

            lcd_write_menu(&menus[menu_state], MAIN_MENU_AMOUNT);
            break;
        }
        ESP_LOGI(TAG, "NOPE");
    }
}

/**
 * @brief Helper function used to scroll though the menu, replaces strings in the menu struct
 * 
 * @param[in] The list that needs to be scrolled through
 * @param[in] The size of the list that needs to be scrolled though
 * @param[in] The amount that needs to be scrolled down, enter a negative value to scroll up'
 * 
 */ 
void menu_scroll_helper(char menu_items[MAX_HIGHSCORE_SLOTS][MAX_STRING_LENGTH], int arr_size, int modifier)
{
    //Calculate new scroll state
    scroll_state = (scroll_state + modifier + arr_size) % arr_size;

    //Change the strings in the menu structs and return them to start if it exceeds the max array size
    strcpy(menus[menu_state].line1, menu_items[scroll_state]);
    strcpy(menus[menu_state].line2, menu_items[(scroll_state + 1) % arr_size]);
    strcpy(menus[menu_state].line3, menu_items[(scroll_state + 2) % arr_size]);
}

void menu_scroll_down()
{
    ESP_LOGI(TAG, "menu_scroll_down");
    if (menus[menu_state].scrollable == true)   //Check if the current menu is scrollable
    {
        int arr_size;
        switch (menu_state)
        {
        case MENU_MAIN_1:
            arr_size = (sizeof(game_menu) / sizeof(game_menu[0]));  //Calculate Array size
            menu_scroll_helper(game_menu, arr_size, 1);
            break;
        
        case MENU_MAIN_2:
            arr_size = (sizeof(scores) / sizeof(scores[0]));  //Calculate Array size
            menu_scroll_helper(scores, arr_size, 1);
            break;
        }
        lcd_write_menu(&menus[menu_state], MAIN_MENU_AMOUNT);
    }
}

void menu_scroll_up()
{
    ESP_LOGI(TAG, "menu_scroll_up");
    if (menus[menu_state].scrollable == true)   //Check if the current menu is scrollable
    {
        int arr_size;
        switch (menu_state)
        {
        case MENU_MAIN_1:
            arr_size = (sizeof(game_menu) / sizeof(game_menu[0]));  //Calculate Array size
            menu_scroll_helper(game_menu, arr_size, -1);
            break;
        
        case MENU_MAIN_2:
            arr_size = (sizeof(scores) / sizeof(scores[0]));  //Calculate Array size
            menu_scroll_helper(scores, arr_size, -1);
            break;
        }

        lcd_write_menu(&menus[menu_state], MAIN_MENU_AMOUNT);
    }
}

void menu_select_item()
{
    int arr_size;
    if (menu_state == MENU_MAIN_1)
    {
        switch (scroll_state)
        {
        case 0:
            game_is_running ? : game_start();
            break;
        case 1:
            arr_size = (sizeof(game_menu) / sizeof(game_menu[0]));  //Calculate Array size
            for (size_t i = 0; i < arr_size; i++)
            {
                strcpy(scores[i], "<empty>");
            }
            break;
        case 2:
            menu_state = (int)MENU_EXPLANATION;
            lcd_write_menu(&menus[menu_state], -1);
            break;
        }    
    }
}
