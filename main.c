//*****************************************************************************
// main.c
// Author: jkrachey@wisc.edu
//*****************************************************************************
#include "lab_buttons.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h>

#define MOVE_PIXELS   1

// Turn Debuggin off
#undef DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(x) printf x
#else
#define DEBUG_PRINT(x) do {} while (0)
#endif

#define UP_BUTTON 0x01

#define LOCAL_ID     0x00
#define REMOTE_ID    0x01


/******************************************************************************
 * Global Variables
 *****************************************************************************/
int game_state = 0; //0: Homescreen, 1: Shootout Screen, 2: Death screen, 3: Winning screen
int remote_ready = 0;
int local_ready = 0;
int countdown = 0;
int upper = 10000;
int lower = 2000;

//*****************************************************************************
//*****************************************************************************
int 
main(void)
{
	char msg[80];
  uint32_t rx_data;
  uint32_t tx_data;
  uint8_t buttons;
  
  ece210_initialize_board();
  ece210_lcd_add_msg("Wireless TEST CODE\n\r",TERMINAL_ALIGN_CENTER,LCD_COLOR_BLUE);
  sprintf(msg,"Local ID %d",LOCAL_ID);
  ece210_lcd_add_msg(msg,TERMINAL_ALIGN_CENTER,LCD_COLOR_BLUE);
  sprintf(msg,"Remote ID %d",REMOTE_ID);
  ece210_lcd_add_msg(msg,TERMINAL_ALIGN_CENTER,LCD_COLOR_BLUE);
  ece210_wireless_init(LOCAL_ID,REMOTE_ID);
	
	ece210_lcd_add_msg("Wild West Shootout!", TERMINAL_ALIGN_CENTER, LCD_COLOR_YELLOW);
	ece210_lcd_add_msg("Press Down To Begin Pairing", TERMINAL_ALIGN_CENTER, LCD_COLOR_CYAN);
  
  while(1)
  {
		if (game_state==0) {
			
			if (remote_ready == 1 && local_ready == 1) {
					countdown = (rand() % (upper – lower + 1)) + lower;
					game_state = 1;
			}
			
			if(ece210_wireless_data_avaiable())
			{
				rx_data = ece210_wireless_get();
				if( rx_data == UP_BUTTON)
				{
					ece210_lcd_add_msg("Pairing Message Received!", TERMINAL_ALIGN_CENTER, LCD_COLOR_GREEN);
					remote_ready = 1;
					
				}
			}
			
			if(AlertButtons)
			{
				AlertButtons = false;
				
				// Transmit data
				buttons = ece210_buttons_read();
				if(buttons == UP_BUTTON)
				{
					ece210_lcd_add_msg("Pairing Message Sent...", TERMINAL_ALIGN_CENTER, LCD_COLOR_RED);
					local_ready = 1;
					ece210_wireless_send(UP_BUTTON);
				}
			}
		}
		
		if (game_state==1) {
			
		}
	  
	        if (game_state==2) { //death screen
		   ece210_lcd_add_msg("Sorry you died first!", TERMINAL_ALIGN_CENTER, LCD_COLOR_RED);
	           ece210_lcd_add_msg("Press UP Button to return to Pairing", TERMINAL_ALIGN_CENTER, LCD_COLOR_CYAN);
			
			if(AlertButtons)
			{
				AlertButtons = false;

				buttons = ece210_buttons_read();
				if(buttons == UP_BUTTON) {
					game_state = 0;
					local_ready = 0;
					remote_ready = 0;
				}
			}
		}
	  if (game_state==3) { //winning screen
		   ece210_lcd_add_msg("You won! Congratulations", TERMINAL_ALIGN_CENTER, LCD_COLOR_RED);
	           ece210_lcd_add_msg("Press UP Button to return to Pairing", TERMINAL_ALIGN_CENTER, LCD_COLOR_CYAN);
			
			if(AlertButtons)
			{
				AlertButtons = false;

				buttons = ece210_buttons_read();
				if(buttons == UP_BUTTON) {
					game_state = 0;
					local_ready = 0;
					remote_ready = 0;
				}
			}
		}
			
	 
		}
	
  }
}


