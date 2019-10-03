#include "msp.h"
#include "lcd_driver.h"
#include "keypad_driver.h"


/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	Initialize_LCD();
	Initialize_keypad();
	int button = -1;
	while(1) {
	    if((button = detect_key_press()) != -1)
	    {
	        Clear_LCD();
	        switch(button)
	        {
	            case 1:
	                button = ONE;
	                break;
	            case 2:
	                button = TWO;
	                break;
	            case 3:
	                button = THREE;
	                break;
	            case 4:
	                button = FOUR
	                break;
	            case 5:
	                button = FIVE;
	                break;
	            case 6:
	                button = SIX;
	                break;
	            case 7:
	                button = SEVEN;
	                break;
	            case 8:
	                button = EIGHT;
	                break;
	            case 9:
	                button = NINE;
	                break;
	            case 10:
	                button = STAR;
	                break;
	            case 11:
	                button = ZERO
	                break;
	            case 12:
	                button = HASH;
	                break;
	            default:
	                break;
	        }
	        Write_char_LCD(button);
	    }
	}
}
