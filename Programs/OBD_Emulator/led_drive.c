/*
 * led_drive.c
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
#include "led_drive.h"


void LEDs_Init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);       // Enable GPIOF peripheral
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 ); // Enable PF1 (Red), PF2 (Blue), PF3 (Green) as outputs | GPIO_PIN_3
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 , 0);       // Turn off all LEDs initially | GPIO_PIN_3
}
