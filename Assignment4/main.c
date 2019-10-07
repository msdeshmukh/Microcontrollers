#include "msp.h"
#include "delay.h"
#include "lcd_driver.h"
#include "keypad_driver.h"


/**
 *  main.c
 *  Created on: Oct 1, 2019
 *  Authors: Mihir Deshmukh, Ryan Myers
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	set_DCO(DCORSEL_24_MHz);
	Initialize_LCD();
	Initialize_keypad();
	int button = -1;
	while(1) {
	    if((button = detect_key_press()) != 255)
	    {
	        Clear_LCD();
	        switch(button)
	        {
	            case 1:
	                button = ONE;
	                break;
	            case 0:
	                button = TWO;
	                break;
	            case 2:
	                button = THREE;
	                break;
	            case 4:
	                button = FOUR;
	                break;
	            case 5:
	                button = FIVE;
	                break;
	            case 3:
	                button = SIX;
	                break;
	            case 10:
	                button = SEVEN;
	                break;
	            case 9:
	                button = EIGHT;
	                break;
	            case 11:
	                button = NINE;
	                break;
	            case 7:
	                button = STAR;
	                break;
	            case 6:
	                button = ZERO;
	                break;
	            case 8:
	                button = HASH;
	                break;
	            default:
	                break;
	        }
	        Write_char_LCD(button);
	        Home_LCD();
	        delay_us(10);
	    }
	    delay_us(5);
	}
}
