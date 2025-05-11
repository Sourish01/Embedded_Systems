/*
 * systick_enable.c
 *
 *  Created on: 11-Mar-2025
 *      Author: A Jaaneshwaran
 */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>  // For isalnum()
#include <./inc/tm4c123gh6pm.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <inc/hw_ints.h>
#include <driverlib/interrupt.h>
#include <driverlib/systick.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <driverlib/uart.h>
#include "systick_enable.h"

void SysTick_Init(void)
{
    SysTickPeriodSet(SYSTICK_RELOAD_VALUE - 1); // Set reload value for 20ms
    //SysTickIntRegister(SysTick_Handler);       // Register interrupt handler

}

void Systick_Interrupt_Enable(void)
{
    SysTickIntEnable();                        // Enable SysTick interrupt
    SysTickEnable();                           // Enable SysTick
}

