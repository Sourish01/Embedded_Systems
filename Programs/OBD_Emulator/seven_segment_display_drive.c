/*
 * seven_segment_display_drive.c
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
//#include <inc/hw_ints.h>
#include <driverlib/interrupt.h>
#include <driverlib/systick.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include "seven_segment_display_drive.h"

void drive_7seg_number (int n){
    switch (n){
    case 0:
        DRIVE_0 ;
        simpledelayMs(1);
        //SysCtlDelay(20);
        break;
    case 1:
        DRIVE_1 ;
        simpledelayMs(1);
        //SysCtlDelay(20);
        break;
    case 2:
        DRIVE_2;
        simpledelayMs(1);
        //SysCtlDelay(20);
        break;
    case 3:
        DRIVE_3;
        simpledelayMs(1);
        //SysCtlDelay(20);
        break;
    case 4:
        DRIVE_4;
        simpledelayMs(1);
        //SysCtlDelay(20);
        break;
    case 5:
        DRIVE_5;
        simpledelayMs(1);
        //SysCtlDelay(20);
        break;
    case 6:
        DRIVE_6;
        simpledelayMs(1);
        //SysCtlDelay(20);
        break;
    case 7:
        DRIVE_7;
        simpledelayMs(1);
        //SysCtlDelay(20);
        break;
    case 8:
        DRIVE_8;
        simpledelayMs(1);
        //SysCtlDelay(20);
        break;
    case 9:
        DRIVE_9;
        simpledelayMs(1);
        //SysCtlDelay(20);
        break;
    default:
        DRIVE_0;
        simpledelayMs(1);
        //SysCtlDelay(20);

    }
}

void simpledelayMs(int n) {
    //n = 10 ;
    int i,j ;
    for(i = 0 ; i < n ; i++){
        for(j = 0; j < 3180; j++) {} /* do nothing for 1 ms */
    }
    //pollSwitch();
}

/* This function initializes the Seven Segment Display Cathode BJT control, */
void Init_PortA_needed_pins(void)
{

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);       // Enable GPIOF peripheral
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_7 | GPIO_PIN_6 | GPIO_PIN_5 | GPIO_PIN_4); // Configure PF3, PF2 and PF1 as output
}

/* This function initializes the Seven Segment Display Common Cathode data ports  */
void Init_PortB_needed_pins(void)
{

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);       // Enable GPIOF peripheral
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_7 | GPIO_PIN_6 | GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1 | GPIO_PIN_0 ); // Configure PF3, PF2 and PF1 as output
}

void drive_7seg_display(int first, int second, int third , int fourth){
    SELECT_POS_0;
    drive_7seg_number(first);
    SELECT_POS_1;
    drive_7seg_number(second);
    SELECT_POS_2;
    drive_7seg_number(third);
    SELECT_POS_3;
    drive_7seg_number(fourth);
    SELECT_POS_3;
    drive_7seg_number(fourth);
    SELECT_POS_2;
    drive_7seg_number(third);
    SELECT_POS_1;
    drive_7seg_number(second);
    SELECT_POS_0;
    drive_7seg_number(first);
}
