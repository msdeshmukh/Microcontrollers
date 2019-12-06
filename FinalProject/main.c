#include "msp.h"
#include "backup.h"
#include "delay.h"
#include "uart_driver.h"

#define NEW_LINE_ASCII 0x0D

void Write_Distance(uint32_t distance);

static uint8_t dig_ascii_table[10] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};

/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	set_DCO(DCORSEL_12_MHz);
	Initialize_Backup();
	Initialize_UART();
	uint32_t distance;
	__enable_irq();
	while(1) {
	    if (!(GEAR_SELECT_PORT->IN & GEAR_SELECTOR)) {
	        Change_Gears(GEAR_DRIVE);
	    }
	    else {
	        BACKUP_PORT->OUT &= ~BUZZER;
	        Change_Gears(GEAR_REVERSE);
	    }
	    distance = (uint32_t)Measure_Distance() / 2;
	    Write_Distance(distance);
	    Set_Beep_Freq(TWO_Hz_TOGGLE * ((1.0 * distance) / US_MAX_DIST));
	}
}

void Write_Distance(uint32_t distance) {
    uint8_t num_buff[3];
    num_buff[0] = distance / 100;
    distance -= num_buff[0] * 100;
    num_buff[1] = distance / 10;
    distance -= num_buff[1] * 10;
    num_buff[2] = distance;
    EUSCI_A0->TXBUF = dig_ascii_table[num_buff[0]];
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = dig_ascii_table[num_buff[1]];
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = dig_ascii_table[num_buff[2]];
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = NEW_LINE_ASCII;
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
}
