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
	//Run_Squarewave();
	Run_Trianglewave();
	__enable_irq();
	while(1) {
	}
}
