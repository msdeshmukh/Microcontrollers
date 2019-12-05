#include "msp.h"
#include "backup.h"
#include "delay.h"


/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	while(1) {
	    BACKUP_PORT->OUT |= BUZZER;
	    delay_us(50000);
	    BACKUP_PORT->OUT &= ~BUZZER;
	    delay_us(50000);
	}
}
