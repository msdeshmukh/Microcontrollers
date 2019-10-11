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
	int button = -1, keys = 0, i;
	int passkey[6] = {0,0,0,0,0,0};
	int lockkey[6] = {0,1,2,3,0,0};
	Write_string_LCD("LOCKED\nENTER KEY ", 17);
	while(1) {
	    while((keys < MAX_PASSKEY_LENGTH) && (button = detect_key_press()) != 255)
	    {
	        switch(button)
	        {
	            case 1:
	                Write_char_LCD(STAR);
	                passkey[keys] = 1;
	                keys ++;
	                break;
	            case 0:
                    Write_char_LCD(STAR);
                    passkey[keys] = 2;
                    keys ++;
	                break;
	            case 2:
                    Write_char_LCD(STAR);
                    passkey[keys] = 3;
                    keys ++;
	                break;
	            case 10:
                    Write_char_LCD(STAR);
                    passkey[keys] = 4;
                    keys ++;
	                break;
	            case 9:
                    Write_char_LCD(STAR);
                    passkey[keys] = 5;
                    keys ++;
	                break;
	            case 11:
                    Write_char_LCD(STAR);
                    passkey[keys] = 6;
                    keys ++;
	                break;
	            case 7:
                    Write_char_LCD(STAR);
                    passkey[keys] = 7;
                    keys ++;
	                break;
	            case 6:
                    Write_char_LCD(STAR);
                    passkey[keys] = 8;
                    keys ++;
	                break;
	            case 8:
	                Write_char_LCD(STAR);
	                passkey[keys] = 9;
	                keys ++;
	                break;
	            case 4:
	                Clear_LCD();
	                keys = 0;
	                Write_string_LCD("LOCKED\nENTER KEY ", 17);
	                break;
	            case 3:
                    Write_char_LCD(STAR);
                    passkey[keys] = 0;
                    keys ++;
	                break;
	            case 5:
	                if(keys != PASSKEY_LENGTH)
	                {
                        Clear_LCD();
                        keys = 0;
                        Write_string_LCD("LOCKED\nENTER KEY ", 17);
	                    break;
	                }
	                for(i = 0; i < keys; i++)
	                {
	                    if(passkey[i] != lockkey[i])
	                    {
	                        Clear_LCD();
	                        Write_string_LCD("LOCKED\nENTER KEY ", 17);
	                    }

	                }
	                if(i == keys)
	                {
	                    Clear_LCD();
	                    Write_string_LCD("UNLOCKED", 8);
	                }
	                keys = 0;
	                break;
	            default:
	                break;
	        }
	        delay_us(10);
	    }
	    if(keys >= MAX_PASSKEY_LENGTH && (button = detect_key_press()) == 5)
	    {
            if(keys != PASSKEY_LENGTH)
            {
                Clear_LCD();
                keys = 0;
                Write_string_LCD("LOCKED\nENTER KEY ", 17);
            }
            else if(keys == PASSKEY_LENGTH)
            {
                for(i = 0; i < keys; i++)
                {
                    if(passkey[i] != lockkey[i])
                    {
                        Clear_LCD();
                        Write_string_LCD("LOCKED\nENTER KEY ", 17);
                        break;
                    }

                }
                if(i == keys)
                {
                    Clear_LCD();
                    Write_string_LCD("UNLOCKED", 8);
                }
                keys = 0;
            }
	    }
	    delay_us(5);
	}
}
