#ifndef game_H
#define game_H

#include <stdbool.h>

/**
 * @brief Allocates and initializes game resources and starts the game
 *
 */ 
void start_game();

/**
 * @brief Makes the player character jump into the air! :)
 *        No other inputs can be giving whilst the player is jumping
 * 
 */ 
void jump_game();

/**
 * @brief Stops the game and releases all game resources, then sends the score to the menu
 * 
 */ 
void stop_game();


bool game_is_running;       ///< Set to TRUE if a game is currently running

#endif