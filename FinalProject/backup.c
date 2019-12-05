/*
 * backup.c
 *
 *  Created on: Dec 4, 2019
 *      Author: ryan
 */
#include "backup.h"
#include "msp.h"

uint16_t t_echo_high;
uint16_t t_echo_low;

void TA0_0_IRQHandler(void) {

}

void Initialize_Backup(void) {
    BACKUP_PORT->DIR |= BUZZER | US_TRIG;
    BACKUP_PORT->DIR &= ~US_ECHO;
    BACKUP_PORT->SEL0 &= ~(BUZZER | US_TRIG | US_ECHO);
    BACKUP_PORT->SEL1 &= ~(BUZZER | US_TRIG | US_ECHO);
    BACKUP_PORT->REN &= ~(BUZZER | US_TRIG | US_ECHO);

    TIMER_A0->CTL |= TIMER_A_CTL_TASSEL_2;
    TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CM_3 | TIMER_A_CCTLN_SCS | TIMER_A_CCTLN_CAP | TIMER_A_CCTLN_CCIE;


}

float Measure_Distance(void) {

}
