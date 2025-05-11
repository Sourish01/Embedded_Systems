/*
 * keypad_matrix.c
 *
 *  Created on: 13-Mar-2025
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
#include "keypad_matrix.h"


void KP_Init_PortE_needed_pins(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);       // Enable GPIOE peripheral
    GPIOPinTypeGPIOOutputOD(GPIO_PORTE_BASE, GPIO_PIN_2 | GPIO_PIN_1 | GPIO_PIN_0); // Configure PE3, PE2 and PE1 , PE0 as output
}

void KP_Init_PortC_needed_pins(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);       // Enable GPIOF peripheral
    GPIOUnlockPin(GPIO_PORTC_BASE,GPIO_PIN_7 | GPIO_PIN_6 | GPIO_PIN_5 | GPIO_PIN_4);


    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_7 | GPIO_PIN_6 | GPIO_PIN_5 | GPIO_PIN_4); // Configure PF4 (SW1) and PF0 (SW2) as input
    GPIOPadConfigSet(GPIO_PORTC_BASE,GPIO_PIN_7 | GPIO_PIN_6 | GPIO_PIN_5 | GPIO_PIN_4 , GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU); // Enable pull-up resistors
}

void KP_SW_Intr_Enable(void)
{
    GPIOIntTypeSet(GPIO_PORTC_BASE, GPIO_PIN_7 | GPIO_PIN_6 | GPIO_PIN_5 | GPIO_PIN_4 , GPIO_FALLING_EDGE); // Set interrupt on falling edge for both switches
    //GPIOIntTypeSet(GPIO_PORTC_BASE, GPIO_PIN_7 | GPIO_PIN_6 | GPIO_PIN_5 | GPIO_PIN_4 , GPIO_LOW_LEVEL); // Set interrupt on falling edge for both switches
    GPIOIntEnable(GPIO_PORTC_BASE, GPIO_PIN_7 | GPIO_PIN_6 | GPIO_PIN_5 | GPIO_PIN_4);    // Enable interrupt for PF4 and PF0
    IntEnable(INT_GPIOC);                                              // Enable GPIOF interrupt in the controller
}
