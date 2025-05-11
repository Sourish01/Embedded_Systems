/*
 * seven_segment_display_drive.h
 *
 *  Created on: 18-Feb-2025
 *      Author: A Jaaneshwaran
 */

#ifndef SEVEN_SEGMENT_DISPLAY_DRIVE_H_
#define SEVEN_SEGMENT_DISPLAY_DRIVE_H_

#define  SELECT_POS_0 (GPIO_PORTA_DATA_R=0x10)
#define  SELECT_POS_1 (GPIO_PORTA_DATA_R=0x20)
#define  SELECT_POS_2 (GPIO_PORTA_DATA_R=0x40)
#define  SELECT_POS_3 (GPIO_PORTA_DATA_R=0x80)
#define  DRIVE_0 (GPIO_PORTB_DATA_R=0x3F)
#define  DRIVE_1 (GPIO_PORTB_DATA_R=0x06)
#define  DRIVE_2 (GPIO_PORTB_DATA_R=0x5B)
#define  DRIVE_3 (GPIO_PORTB_DATA_R=0x4F)
#define  DRIVE_4 (GPIO_PORTB_DATA_R=0x66)
#define  DRIVE_5 (GPIO_PORTB_DATA_R=0x6D)
#define  DRIVE_6 (GPIO_PORTB_DATA_R=0x7D)
#define  DRIVE_7 (GPIO_PORTB_DATA_R=0x07)
#define  DRIVE_8 (GPIO_PORTB_DATA_R=0x7F)
#define  DRIVE_9 (GPIO_PORTB_DATA_R=0x6F)

extern void drive_7seg_number (int n);
extern void Init_PortA_needed_pins(void);
extern void Init_PortB_needed_pins(void);
extern void simpledelayMs(int n);
extern void drive_7seg_display(int first, int second, int third , int fourth);


#endif /* SEVEN_SEGMENT_DISPLAY_DRIVE_H_ */
