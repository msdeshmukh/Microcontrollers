#include "msp.h"
#include "delay.h"
#include "lcd_driver.h"
#include "lock.h"
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
	int button = -1, keys = 0, passkey = 0, divisor = DIVISOR;
	Write_string_LCD("LOCKED\nENTER KEY ", 17);
	while(1) {
	    while((keys < MAX_PASSKEY_LENGTH) && (button = detect_key_press()) != 255)
	    {
	        switch(button)
	        {
	            case 1:
	                Write_char_LCD(STAR);
	                passkey += 1 * divisor;
	                if(divisor > 1)
	                    divisor = divisor/ 10;
	                break;
	            case 0:
	                Write_char_LCD(STAR);
                    passkey += 2 * divisor;
                    if(divisor > 1)
                        divisor = divisor/ 10;
	                break;
	            case 2:
	                Write_char_LCD(STAR);
                    passkey += 3 * divisor;
                    if(divisor > 1)
                        divisor = divisor/ 10;
	                break;
	            case 4:
	                Write_char_LCD(STAR);
                    passkey += 4 * divisor;
                    if(divisor > 1)
                        divisor = divisor/ 10;
	                break;
	            case 5:
	                Write_char_LCD(STAR);
                    passkey += 5 * divisor;
                    if(divisor > 1)
                        divisor = divisor/ 10;
	                break;
	            case 3:
	                Write_char_LCD(STAR);
                    passkey += 6 * divisor;
                    if(divisor > 1)
                        divisor = divisor/ 10;
	                break;
	            case 10:
	                Write_char_LCD(STAR);
                    passkey += 7 * divisor;
                    if(divisor > 1)
                        divisor = divisor/ 10;
	                break;
	            case 9:
	                Write_char_LCD(STAR);
                    passkey += 8 * divisor;
                    if(divisor > 1)
                        divisor = divisor/ 10;
	                break;
	            case 11:
	                Write_char_LCD(STAR);
                    passkey += 9 * divisor;
                    if(divisor > 1)
                        divisor = divisor/ 10;
	                break;
	            case 7:
	                Clear_LCD();
	                keys = 0;
	                Write_string_LCD("LOCKED\nENTER KEY ", 17);
	                break;
	            case 6:
	                Write_char_LCD(STAR);
                    passkey += 0 * divisor;
                    if(divisor > 1)
                        divisor = divisor/ 10;
	                break;
	            case 8:
	                button = HASH;
	                break;
	            default:
	                break;
	        }
	        keys ++;
	        delay_us(100);
	    }
	    delay_us(5);
	}
}
