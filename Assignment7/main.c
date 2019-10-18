#include "msp.h"
#include "spi_driver.h"
#include "wavegen.h"

/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	Initialize_SPI();
	Run_Squarewave();
	while(1)
	{
	    if (square_pulse == 0) {
	        Send_DAC_Voltage(2.0);
	    }
	    else {
	        Send_DAC_Voltage(0.0);
	    }
	}
}
