#include "msp.h"
#include "lcd_driver.h"
#include "spi_driver.h"
#include "keypad_driver.h"
#include "wavegen.h"

/**
 * main.c
 */

typedef enum {SQUARE, SAWTOOTH, SIN, OFF} State;
volatile State state;

void HandleSinInput(void);
void HandleSquareInput(void);
void HandleSawtoothInput(void);

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	state = OFF;
	int input = -1;
	Initialize_SPI();
	Initialize_keypad();
	Initialize_LCD();
	Initialize_Wavegen();

	while (1) {

	    switch(state) {
	    case OFF:
	        break;

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
    while (state == SQUARE) {
        switch(input = detect_key_press()) {
        case 8:
            state = SIN;
            Run_Sinwave();
            break;

        case 9:
            state = SAWTOOTH;
            Run_Sawtooth();
            break;
        }
    }
}

void HandleSinInput(void) {
    uint8_t input;
    while (state == SIN) {
        switch(input = detect_key_press()) {
        case 7:
            state = SQUARE;
            Run_Squarewave();
            break;

        case 9:
            state = SAWTOOTH;
            Run_Sawtooth();
            break;
    }
}
}

void HandleSawtoothInput(void) {
    uint8_t input;
    while (state == SAWTOOTH) {
        switch(input = detect_key_press()) {
        case 7:
            state = SQUARE;
            Run_Squarewave();
            break;

        case 8:
            state = SIN;
            Run_Sinwave();
            break;
    }
}
}
