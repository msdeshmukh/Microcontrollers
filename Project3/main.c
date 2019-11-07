#include "msp.h"
#include "delay.h"
#include "adc_driver.h"
#include "uart_driver.h"
#include "spi_driver.h"

/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	float ac_pp;
	float dc;
	float ac_rms;
	uint32_t frequency;
	set_DCO(DCORSEL_12_MHz);
	Initialize_ADC();
	Initialize_UART();
	Initialize_SPI();
    __enable_irq();
	while(1) {
	    if (Read_Measurement_Flag()) {
	        Send_DAC_Voltage(Read_Center());
	        Write_Desc_Values_To_VT100();
	        __enable_irq();
	    }
	}
}
