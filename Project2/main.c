#include "msp.h"
#include "lcd_driver.h"
#include "spi_driver.h"
#include "keypad_driver.h"
#include "delay.h"
#include "wavegen.h"

/**
 *  main.c
 *  Created on: Oct 18, 2019
 *  Author: Ryan Myers, Mihir Deshmukh
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
	set_DCO(DCORSEL_12_MHz);
	Initialize_SPI();
	Initialize_keypad();
	Initialize_Wavegen();

	//State based architecture where based on the key it moves into a state
	//and prints the corresponding waveform. The frequency of a pulse is
	//controlled in those functions below.
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

//Stay in square state unless a key that changes the waveform is pressed
//If a key to change the frequency is pressed, change the time at which
//the timer interrupts. If the duty cycle changes, change the time the
//second interrupt triggers.
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

        //Change state and break to while loop in main.
        case 7:
            state = SIN;
            break;

        case 8:
            state = SAWTOOTH;
            break;

        //Decrease the value of duty that controls the duty cycle
        case 9:
            if (duty == 1) {
                break;;
            }
            TIMER_A0->CCR[1] -= TIMER_A0->CCR[0] / 10;
            duty--;
            break;
        //Reset duty cycle back to 50%
        case 10:
            TIMER_A0->CCR[1] = TIMER_A0->CCR[0] / 2;
            duty = 5;
            break;
        //Increase the value of duty that controls the duty cycle
        case 11:
            if (duty == 9) {
                break;
            }
            TIMER_A0->CCR[1] += TIMER_A0->CCR[0] / 10;
            duty++;
            break;

        }
    }
}

//Stay in sin state unless a key that changes the waveform is pressed
//If a key to change the frequency is pressed, change the time at which
//the timer interrupts.
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

//Stay in saw state unless a key that changes the waveform is pressed
//If a key to change the frequency is pressed, change the time at which
//the timer interrupts.
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
