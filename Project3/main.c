#include "msp.h"
#include "delay.h"
#include "adc_driver.h"
#include "uart_driver.h"

/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	set_DCO(DCORSEL_12_MHz);
	Initialize_ADC();
	Initialize_UART();
    __enable_irq();
    uint32_t frequency;
	while(1) {
	    if (Read_Measurement_Flag()) {
	        //float dc = Read_DC();
	        //float ac_pp = Read_AC_PP();
	        //float ac_rms = Read_AC_RMS();
	         frequency = Read_Freq();
	    }
	}
}
