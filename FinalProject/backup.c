/*
 * backup.c
 *
 *  Created on: Dec 4, 2019
 *      Author: ryan
 */
#include "backup.h"
#include "delay.h"
#include "msp.h"

static volatile uint16_t t_echo_high;
static volatile uint16_t t_echo_low;
static volatile uint8_t high_bool;
static volatile uint8_t gear;
static volatile uint32_t beep_freq;

// TimerA2.1 set up to capture ultrasonic echo pulse
void TA2_N_IRQHandler(void) {
    TIMER_A2->CCTL[1] &= ~TIMER_A_CCTLN_CCIFG;
    // Check timer overflow
    if (TIMER_A2->CCTL[1] & TIMER_A_CCTLN_COV && !high_bool) {
        t_echo_low += 65535;
        TIMER_A2->CCTL[1] &= ~TIMER_A_CCTLN_COV;
    }
    // start measuring pulse on first rising edge
    else if (TIMER_A2->CCTL[1] & TIMER_A_CCTLN_CCI && high_bool) {
        t_echo_high = TIMER_A2->CCR[1];
        high_bool = FALSE;
    }
    // stop measuring pulse on falling edge
    else if (!(TIMER_A2->CCTL[1] & TIMER_A_CCTLN_CCI) && !high_bool) {
        t_echo_low += TIMER_A2->CCR[1];
    }
}

// TimerA0.0 set up to periodically trigger buzzer
void TA0_0_IRQHandler(void) {
    static uint32_t irq_cnt = 0;
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
    // toggle buzzer when in reverse
    if (gear == GEAR_REVERSE && irq_cnt++ >= beep_freq) {
        BACKUP_PORT->OUT ^= BUZZER;
        irq_cnt = 0;
    }
    else if (gear == GEAR_DRIVE) {
        irq_cnt = 0;
        BACKUP_PORT->OUT &= ~BUZZER;
    }
}

// Initialize all peripherals
// TimerA2.1 used for ultrasonic measurements
void Initialize_Backup(void) {
    beep_freq = TWO_Hz_TOGGLE;
    BACKUP_PORT->DIR |= BUZZER | US_TRIG;
    BACKUP_PORT->DIR &= ~US_ECHO;
    BACKUP_PORT->SEL0 &= ~(BUZZER | US_TRIG);
    BACKUP_PORT->SEL1 &= ~(BUZZER | US_TRIG);
    BACKUP_PORT->REN &= ~(BUZZER | US_TRIG);

    BACKUP_PORT->SEL0 |= US_ECHO;
    BACKUP_PORT->SEL1 &= ~US_ECHO;

    BACKUP_PORT->OUT |= US_TRIG;
    BACKUP_PORT->OUT &= ~BUZZER;

    GEAR_SELECT_PORT->DIR &= ~GEAR_SELECTOR;
    GEAR_SELECT_PORT->SEL0 &= ~GEAR_SELECTOR;
    GEAR_SELECT_PORT->SEL1 &= ~GEAR_SELECTOR;
    GEAR_SELECT_PORT->REN &= ~GEAR_SELECTOR;

    TIMER_A2->CCTL[1] = TIMER_A_CCTLN_CCIE |
                        TIMER_A_CCTLN_CM_3 |
                        TIMER_A_CCTLN_CCIS_0 |
                        TIMER_A_CCTLN_SCS |
                        TIMER_A_CCTLN_CCIS_0 |
                        TIMER_A_CCTLN_CAP;
    TIMER_A2->CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_MC_2;
    NVIC->ISER[0] = (1 << (TA2_N_IRQn & 0x1F));

    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE;
    TIMER_A0->CTL = TIMER_A_CTL_MC_1 | TIMER_A_CTL_TASSEL_2;
    TIMER_A0->CCR[0] = 16384;
    NVIC->ISER[0] = (1 << (TA0_0_IRQn & 0x1f));


}

// Abstraction function to alter buzzer toggle frequency
void Set_Beep_Freq(uint32_t toggle_freq) {
    beep_freq = toggle_freq;
}

// Function to trigger ultrasonic sensor and begin distance measurement
float Measure_Distance(void) {
    float distance = 0;
    t_echo_high = 0;
    t_echo_low = 0;
    high_bool = TRUE;
    // Send ultrasonic sensor a high signal for 30uS
    BACKUP_PORT->OUT &= ~US_TRIG;
    delay_us(30);
    // Turn off trigger signal to begin measurement
    BACKUP_PORT->OUT |= US_TRIG;
    // Wait at least 60mS for ultrasonic measurement to take place
    delay_us(60000);
    // Adjust distance measurement using calibration function
    distance = (((t_echo_low - t_echo_high) / US_TIME_SCALE) * 34300) / 2.0;
    if (distance > US_MAX_DIST || distance < 0) {
        distance = US_MAX_DIST;
    }
    else if (distance < US_MIN_DIST) {
        distance = US_MIN_DIST;
    }
    return distance;
}

// Abstraction function to change gears
void Change_Gears(uint8_t gear_select) {
    gear = gear_select;
}
