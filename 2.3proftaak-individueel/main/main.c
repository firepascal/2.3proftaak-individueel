#include "mainHandler.h"
#include "menu.h"
#include "buttonhandler.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


void app_main()
{
    //Initialize main components
	main_handler_t main_handler;
    mainHandler_init(&main_handler);

    //Initialize the buttons
    buttonHandler_init(&main_handler);
    
    //Initialize the menu
    menu_init();
}