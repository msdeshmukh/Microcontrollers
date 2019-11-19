#include "msp.h"
#include "i2c_driver.h"
#include "delay.h"

/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	Initialize_I2C();
	//set_DCO(DCORSEL_12_MHz);
	uint16_t test_addr = 100;
	uint8_t test_write = 1;
	uint8_t test_read;
	while (1) {
	    if(test_addr == EEPROM_SIZE) {
	        test_addr = 0;
	    }
	    if (test_write == 255) {
	        test_write = 1;
	    }
	    delay_us(40000);
	    WriteEEPROM(test_addr, test_write);
	    delay_us(10000);
	    test_read = ReadEEPROM(test_addr);
	    if (test_read == test_write) {
	        P1->OUT |= BIT1;
	    }
	    else {
	        P1->OUT &= ~BIT1;
	    }
	    //test_addr++;
	    test_write++;
	}
}
