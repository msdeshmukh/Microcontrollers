#include "msp.h"
#include "lcd_driver.h"
#include "spi_driver.h"
#include "keypad_driver.h"
#include "delay.h"
#include "wavegen.h"

/**
 * main.c
 */

typedef enum {SQUARE, SAWTOOTH, SIN} State;
volatile State state;

void HandleSinInput(void);
void HandleSquareInput(void);
void HandleSawtoothInput(void);

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    P2->DIR |= BIT7;
    P2->SEL0 &= ~BIT7;
    P2->SEL1 &= ~BIT7;

	state = SQUARE;
	set_DCO(DCORSEL_12_MHz);
	Initialize_SPI();
	Initialize_keypad();
	Initialize_LCD();
	Initialize_Wavegen();

	while (1) {

	    switch(state) {

	    case SQUARE:
	        HandleSquareInput();
	        break;

	    case SAWTOOTH:
	        HandleSawtoothInput();
	        break;

	    case SIN:
	        HandleSinInput();
	        break;

	    default:
	        break;

	    }



	}
}

void HandleSquareInput(void) {
    uint8_t input;
    uint8_t duty = 5;
    Run_Squarewave();
    while (state == SQUARE) {
        Send_DAC_Voltage(out_volt);
        input = detect_key_press();
        switch(input) {

        case 0:
            TIMER_A0->CCR[0] = SQUARE_100_Hz;
            TIMER_A0->CCR[1] = (SQUARE_100_Hz / 10) * duty;
            break;

        case 1:
            TIMER_A0->CCR[0] = SQUARE_200_Hz;
            TIMER_A0->CCR[1] = (SQUARE_200_Hz / 10) * duty;
            break;

        case 2:
            TIMER_A0->CCR[0] = SQUARE_300_Hz;
            TIMER_A0->CCR[1] = (SQUARE_300_Hz / 10) * duty;
            break;

        case 3:
            TIMER_A0->CCR[0] = SQUARE_400_Hz;
            TIMER_A0->CCR[1] = (SQUARE_400_Hz / 10) * duty;
            break;

        case 4:
            TIMER_A0->CCR[0] = SQUARE_500_Hz;
            TIMER_A0->CCR[1] = (SQUARE_500_Hz / 10) * duty;
            break;


        case 7:
            state = SIN;
            break;

        case 8:
            state = SAWTOOTH;
            break;

        case 9:
            if (duty == 1) {
                break;;
            }
            TIMER_A0->CCR[1] -= TIMER_A0->CCR[0] / 10;
            duty--;
            break;

        case 10:
            TIMER_A0->CCR[1] = TIMER_A0->CCR[0] / 2;
            duty = 5;
            break;

        case 11:
            if (duty == 9) {
                break;;
            }
            TIMER_A0->CCR[1] += TIMER_A0->CCR[0] / 10;
            duty++;
            break;

        }
    }
}

void HandleSinInput(void) {
    uint8_t input;
    Run_Sinwave();
    while (state == SIN) {
        Send_DAC_Voltage(out_volt);
        input = detect_key_press();
        switch(input) {

        case 0:
            TIMER_A1->CCR[0] = SIN_100_Hz;
            break;

        case 1:
            TIMER_A1->CCR[0] = SIN_200_Hz;
            break;

        case 2:
            TIMER_A1->CCR[0] = SIN_300_Hz;
            break;

        case 3:
            TIMER_A1->CCR[0] = SIN_400_Hz;
            break;

        case 4:
            TIMER_A1->CCR[0] = SIN_500_Hz;
            break;

        case 6:
            state = SQUARE;
            break;

        case 8:
            state = SAWTOOTH;
            break;

        default:
            break;
    }
}
}

void HandleSawtoothInput(void) {
    uint8_t input;
    Run_Sawtooth();
    while (state == SAWTOOTH) {
        Send_DAC_Voltage(out_volt);
        input = detect_key_press();
        switch(input) {

        case 0:
            TIMER_A2->CCR[0] = SAW_100_Hz;
            break;

        case 1:
            TIMER_A2->CCR[0] = SAW_200_Hz;
            break;

        case 2:
            TIMER_A2->CCR[0] = SAW_300_Hz;
            break;

        case 3:
            TIMER_A2->CCR[0] = SAW_400_Hz;
            break;

        case 4:
            TIMER_A2->CCR[0] = SAW_500_Hz;
            break;

        case 6:
            state = SQUARE;
            break;

        case 7:
            state = SIN;
            break;

    }
}
}
