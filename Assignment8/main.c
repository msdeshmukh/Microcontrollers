#include "msp.h"
#include "uart_driver.h"
#include "delay.h"
#include "spi_driver.h"

/**
 * main.c
 */

void OutputVoltage(void);

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	set_DCO(DCORSEL_12_MHz);
	Initialize_UART();
	Initialize_SPI();
	__enable_irq();
	while(1) {
	    if (CheckInputFlag() == INPUT_READY) {
	        OutputVoltage();
	    }
	}

}

void OutputVoltage(void) {
    uint16_t dac_voltage = GetInputValue();
    if (dac_voltage > 4095) {
        return;
    }

}
