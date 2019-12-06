/*
 * backup.h
 *
 *  Created on: Dec 4, 2019
 *      Author: ryan
 */

#ifndef BACKUP_H_
#define BACKUP_H_

#include "msp.h"

#define BACKUP_PORT P5
#define US_TRIG BIT1
#define US_ECHO BIT6
#define BUZZER BIT2
#define US_VELOCITY_SCALE 58.0f
#define US_TIME_SCALE 8333333.3333f
#define FALSE 0
#define TRUE 1
#define US_MAX_DIST 200.0f
#define US_MIN_DIST 4.0f
#define GEAR_REVERSE 0x01
#define GEAR_DRIVE 0x00
#define TWO_Hz_TOGGLE 400
#define GEAR_SELECT_PORT P2
#define GEAR_SELECTOR BIT5

void Initialize_Backup(void);
float Measure_Distance(void);
void Set_Beep_Freq(uint32_t toggle_freq);
void TA2_N_IRQHandler(void);
void Change_Gears(uint8_t gear_select);

#endif /* BACKUP_H_ */
