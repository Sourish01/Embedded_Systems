/*
 * button_enable.c
 *
 *  Created on: 11-Mar-2025
 *      Author: A Jaaneshwaran
 */

#include <stdint.h>
#include <stdbool.h>
#include <./inc/tm4c123gh6pm.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
//#include <inc/hw_ints.h>
#include <driverlib/interrupt.h>
#include <driverlib/systick.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include "button_enable.h"

void GPIOF_SW_Init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);       // Enable GPIOF peripheral
    //GPIOUnlockPin(GPIO_PORTF_BASE,GPIO_PIN_4);
    //HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;      // Unlock GPIOCR register
    //HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= GPIO_PIN_0;         // Allow changes to PF0
    //HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;                 // Re-lock the GPIOCR register

    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4); // Configure PF4 (SW1) and PF0 (SW2) as input
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4 , GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU); // Enable pull-up resistors
}

void GPIOF_SW_Intr_Enable(void)
{
    //GPIOIntRegister(GPIO_PORTF_BASE, GPIOF_IntHandler); // Register the GPIO interrupt handler
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_FALLING_EDGE); // Set interrupt on falling edge for both switches
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_4 );    // Enable interrupt for PF4 and PF0
    IntEnable(INT_GPIOF);                                              // Enable GPIOF interrupt in the controller
}


