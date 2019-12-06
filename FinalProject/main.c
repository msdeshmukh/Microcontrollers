#include "msp.h"
#include "backup.h"
#include "delay.h"



/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	set_DCO(DCORSEL_12_MHz);
	Initialize_Backup();
	float distance;
	__enable_irq();
	while(1) {
	    distance = Measure_Distance();
	    Set_Beep_Freq(TWO_Hz_TOGGLE * ((1.0 * distance) / US_MAX_DIST));
	}
}
