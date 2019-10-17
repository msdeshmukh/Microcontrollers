#include "msp.h"
#include "spi_driver.h"

/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	Initialize_SPI();
	Send_DAC_Voltage(2.0);

	while(1);
}
