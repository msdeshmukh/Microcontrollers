#include "msp.h"
#include "lcd_driver.h"
#include "spi_driver.h"
#include "keypad_driver.h"
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
	state = SQUARE;
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

        case 1:
            TIMER_A0->CCR[0] = SQUARE_100_Hz;
            TIMER_A0->CCR[1] = (SQUARE_100_Hz / 10) * duty;
            break;

        case 2:
            TIMER_A0->CCR[0] = SQUARE_200_Hz;
            TIMER_A0->CCR[1] = (SQUARE_200_Hz / 10) * duty;
            break;

        case 3:
            TIMER_A0->CCR[0] = SQUARE_300_Hz;
            TIMER_A0->CCR[1] = (SQUARE_300_Hz / 10) * duty;
            break;

        case 4:
            TIMER_A0->CCR[0] = SQUARE_400_Hz;
            TIMER_A0->CCR[1] = (SQUARE_400_Hz / 10) * duty;
            break;

        case 5:
            TIMER_A0->CCR[0] = SQUARE_500_Hz;
            TIMER_A0->CCR[1] = (SQUARE_500_Hz / 10) * duty;
            break;


        case 8:
            state = SIN;
            break;

        case 9:
            state = SAWTOOTH;
            break;

        case 10:
            if (duty == 1) {
                return;
            }
            TIMER_A0->CCR[1] -= TIMER_A0->CCR[0] / 10;
            duty--;
            break;

        case 11:
            TIMER_A0->CCR[1] = TIMER_A0->CCR[0] / 2;
            duty = 5;
            break;

        case 12:
            if (duty == 9) {
                return;
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
        input = detect_key_press();
        switch(input) {
        case 7:
            state = SQUARE;
            break;

        case 9:
            state = SAWTOOTH;
            break;
    }
}
}

void HandleSawtoothInput(void) {
    uint8_t input;
    Run_Sawtooth();
    while (state == SAWTOOTH) {
        input = detect_key_press();
        switch(input) {
        case 7:
            state = SQUARE;
            break;

        case 8:
            state = SIN;
            break;
    }
}
}
