#ifndef menu_H
#define menu_H

/**
 * @brief Initializes the menu, also initializes the LCD
 */ 
void menu_init();

/**
 * @brief Navigates to the next menu in the main menu loop and displays this on the LCD
 */ 
void menu_next();

/**
 * @brief Scrolls down the currently visible menu if this menu has a scrollable list
 */ 
void menu_scroll_down();

/**
 * @brief Scrolls up the currently visible menu if this menu has a scrollable list
 */ 
void menu_scroll_up();

/**
 * @brief Perform the action of the item which is currently selected on the LCD
 */ 
void menu_select_item();

/**
 * @brief Add a score to the current list of scores and refresh the list
 * @param[in] The score to be added
 */ 
void menu_add_score(int score);

#endif