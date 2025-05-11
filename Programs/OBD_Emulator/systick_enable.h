/*
 * systick_enable.h
 *
 *  Created on: 11-Mar-2025
 *      Author: A Jaaneshwaran
 */

#ifndef SYSTICK_ENABLE_H_
#define SYSTICK_ENABLE_H_

#define SYS_CLOCK_HZ 16000000      // 16 MHz system clock
#define SYSTICK_RELOAD_VALUE (SYS_CLOCK_HZ/50)  // 20ms reload value

extern void SysTick_Init(void);
extern void Systick_Interrupt_Enable(void);


#endif /* SYSTICK_ENABLE_H_ */
