#include "lvgl/lvgl.h"  
#include "pinere_menu.h"  
#include "platform.h"  
  
int main(void) {  
    /* 1. Init System Resources */  
    lv_init();  
    platform_init(); // Encapsulates display/input init  
  
    /* 2. Init & Render PineRE (Encapsulated) */  
    pinere_ui_init(); // Initializes state, menu, and first render  
  
    /* 3. Event Loop */  
    while (1) {  
        lv_timer_handler();  
        platform_delay_ms(5);   
    }  
}  
  