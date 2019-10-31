#include "msp.h"
#include "uart_driver.h"
#include "adc_driver.h"

#define ADC_RES 16383
#define DECIMAL_ASCII 0x2E
#define NEW_LINE_ASCII 0x0A

static uint8_t dig_ascii_table = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};


/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	int ADC_Value;
	Initialize_ADC();
	Initialize_UART();
	while(1)
	{
	    if ((ADC_Value = ReadADC()) != -1) {
	        SendADCValue((uint32_t)ADC_Value);
	    }
	}
}

void SendADCValue(uint32_t ADC_Value) {
    float v = (3.3 * ADC_Value) / ADC_RES;
    uint8_t num_buff[3];
    num_buff[0] = (uint8_t)v;
    v -= num_buff[0];
    num_buff[1] = (uint8_t)(v / 0.1);
    v -= 0.1 * num_buff[1];
    num_buff[2] = (uint8_t)(v / 0.01);
    EUSCI_A0->TXBUF = dig_ascii_table[num_buff[0]];
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = DECIMAL_ASCII;
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = dig_ascii_table[num_buff[1]];
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = dig_ascii_table[num_buff[3]];
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = NEW_LINE_ASCII;
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
}
