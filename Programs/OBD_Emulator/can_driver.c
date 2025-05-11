/*
 * can_driver.c
 *
 *  Created on: 25-Apr-2025
 *      Author: A Jaaneshwaran
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
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
#include <driverlib/uart.h>
#include <driverlib/can.h>
#include "uart_enable.h"
#include "can_driver.h"

extern void CAN0_Handler(void);
void CAN_Config(void) {

    // Enable CAN0 and Port F peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    // Wait for peripherals to be ready
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_CAN0) ||
          !SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)) {
    }

    // Configure GPIO pins for CAN
    //GPIOUnlockPin(GPIO_PORTF_BASE,GPIO_PIN_3 | GPIO_PIN_0);
    GPIOPinConfigure(GPIO_PE4_CAN0RX);
    GPIOPinConfigure(GPIO_PE5_CAN0TX);
    GPIOPinTypeCAN(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    CANInit(CAN0_BASE);
    // Configure CAN with the 50 MHz system clock
    CANBitRateSet(CAN0_BASE, SysCtlClockGet(), 500000);
    CANEnable(CAN0_BASE);

    // Register interrupt handler
    CANIntRegister(CAN0_BASE, CAN0_Handler);

    // Enable CAN interrupts
    CANIntEnable(CAN0_BASE, CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);

    // Set CAN interrupt priority (higher than ADC)
    IntPrioritySet(INT_CAN0, 0x40); // Priority 2 (higher)

    // Enable CAN interrupt in NVIC
    IntEnable(INT_CAN0);
}

void CAN_Timer_Set(uint32_t timerValue) {
    //UART_WriteString("In CAN Timer Set",16);
    UARTSend("In CAN Timer Set \n\r");
    static uint32_t txData;
    //static uint8_t txData;

    //txData = (uint8_t)(timerValue & 0xFF);
    txData = (timerValue);

    // Set up the CAN message
    txMessage.ui32MsgID = 0x100;  // Example ID; can be changed
    txMessage.ui32Flags = 0;
    txMessage.ui32MsgLen = sizeof(txData);
    txMessage.pui8MsgData = &txData;

    // Send the message
    CANMessageSet(CAN0_BASE, 7, &txMessage, MSG_OBJ_TYPE_TX);
    UARTSend("In CAN Timer Set succesffult\n");
}

void CAN_Timer_Start(void){
    static uint8_t temp = 0;
    //UART_WriteString("In CAN Timer Start",19);
    UARTSend("In CAN Timer Start \n\r");
    txMessage.ui32MsgID = 0x200;
    txMessage.ui32Flags = 0;
    txMessage.ui32MsgLen = 1;
    txMessage.pui8MsgData = &temp;
    CANMessageSet(CAN0_BASE,8,&txMessage,MSG_OBJ_TYPE_TX);
}

void CAN_Timer_Stop(void){
    static uint8_t temp2 = 0;
    //UART_WriteString("In CAN Timer Stop",18);
    UARTSend("In CAN Timer Stop \n\r");
    txMessage.ui32MsgID = 0x300;
    txMessage.ui32Flags = 0;
    txMessage.ui32MsgLen = 1;
    txMessage.pui8MsgData = &temp2;
    CANMessageSet(CAN0_BASE,9,&txMessage,MSG_OBJ_TYPE_TX);
}

void CAN_Timer_Pause(void){
    static uint8_t temp3 = 0;
    //UART_WriteString("In CAN Timer Pause",19);
    UARTSend("In CAN Timer Pause \n\r");
    txMessage.ui32MsgID = 0x400;
    txMessage.ui32Flags = 0;
    txMessage.ui32MsgLen = 1;
    txMessage.pui8MsgData = &temp3;
    CANMessageSet(CAN0_BASE,10,&txMessage,MSG_OBJ_TYPE_TX);
}

void CAN_PWM_Send(uint32_t pwm){
    UARTSend("In CAN PWM Send \n\r");
    static uint8_t txData[2];

    txData[0] = (uint8_t)(pwm&0xFF);
    txData[1] = (uint8_t)((pwm>>8)&0xFF);

    // Set up the CAN message
    txMessage.ui32MsgID = 0x500;  // Example ID; can be changed
    txMessage.ui32Flags = 0;
    txMessage.ui32MsgLen = 2;
    txMessage.pui8MsgData = txData;

    // Send the message
    CANMessageSet(CAN0_BASE, 11, &txMessage, MSG_OBJ_TYPE_TX);
}
void CAN_Set_RTC(uint8_t date,uint8_t mon,uint8_t year,uint8_t hour,uint8_t min,uint8_t sec){
    //UART_WriteString("RTC Message Sent",16);
    UARTSend("RTC Message Sent \n\r");
    static uint8_t txData[6];

    txData[0] = sec;
    txData[1] = min;
    txData[2] = hour;
    txData[3] = date;
    txData[4] = mon;
    txData[5] = year;

    // Set up the CAN message
    txMessage.ui32MsgID = 0x600;  // Example ID; can be changed
    txMessage.ui32Flags = 0;
    txMessage.ui32MsgLen = 6;
    txMessage.pui8MsgData = txData;

    CANMessageSet(CAN0_BASE, 12, &txMessage, MSG_OBJ_TYPE_TX);
    UARTSend("RTC Message Sent Successfully\n\r");
}
