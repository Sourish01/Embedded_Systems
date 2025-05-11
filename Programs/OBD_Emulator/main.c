#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>  // For isalnum()
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include <./inc/tm4c123gh6pm.h>
#include <driverlib/interrupt.h>
#include <driverlib/systick.h>
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/systick.h"
#include "driverlib/can.h"

#include "uart_enable.h"
#include "systick_enable.h"
#include "can_driver.h"

#define ENGINE_RPM 0x0C
#define VEHICLE_SPEED 0x0D
#define FUEL_PRESSURE 0x0A
#define MAF 0x10
#define THROTTLE_POS 0x11

tCANMsgObject txMessage;
tCANMsgObject rxMessage;

uint8_t txData[8];
uint8_t rxData[8];
uint8_t mode;

void CAN0_Handler(void);
void ProcessAndSendData(float);
void OBD_Parser(void);
void CAN_Response(uint8_t);
void Set_Data(uint8_t byte3,uint8_t byte4,uint8_t byte5,uint8_t byte6,uint8_t byte7);

int main(void)
{
    SysTick_Init();
    CAN_Config();

    rxMessage.ui32MsgID = 0x7DF;
    rxMessage.ui32MsgIDMask = 0x7FF;
    rxMessage.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;
    rxMessage.ui32MsgLen = 8;
    rxMessage.pui8MsgData = rxData;
    CANMessageSet(CAN0_BASE, 2, &rxMessage, MSG_OBJ_TYPE_RX);

    UART0_init();
    while(1){
    }
    return 0;
}

void CAN_Response(uint8_t pid){
    txMessage.ui32MsgID = 0x7E8;
    txMessage.ui32Flags = 0;
    txMessage.ui32MsgLen = 8;

    //txData[0] = 2;
    txData[1] = 0x40+mode;
    txData[2] = pid;
    switch(pid){
    case(ENGINE_RPM):
            txData[0] = 4;
            Set_Data(0x12,0x34,0x00,0x00,0x00);
            //UARTSend("Engine is Set");
            break;
    case(VEHICLE_SPEED):
            txData[0] = 3;
            Set_Data(0x12,0x00,0x00,0x00,0x00);
            break;
    case(FUEL_PRESSURE):
            txData[0] = 3;
            Set_Data(0x12,0x00,0x00,0x00,0x00);
            break;
    case(MAF):
             txData[0] = 4;
             Set_Data(0x12,0x34,0x00,0x00,0x00);
             break;
    case(THROTTLE_POS):
             txData[0] = 3;
             Set_Data(0x12,0x00,0x00,0x00,0x00);
             break;
    default:
        txData[0] = 0;
        Set_Data(0x00,0x00,0x00,0x00,0x00);
        break;
    }
    txMessage.pui8MsgData = txData;
    CANMessageSet(CAN0_BASE,1,&txMessage,MSG_OBJ_TYPE_TX);
    //UARTSend("Sent CAN Response \n");
}
void CAN0_Handler(void){
    uint32_t status = CANIntStatus(CAN0_BASE,CAN_INT_STS_CAUSE);

    if(status==CAN_INT_INTID_STATUS){
        //UARTSend("Error CAN Frame \n\r");
        uint32_t error = CANStatusGet(CAN0_BASE,CAN_STS_CONTROL);
        if (error & CAN_STATUS_BUS_OFF) {
            //UARTSend("CAN Bus Off Error!\n\r");
            CANDisable(CAN0_BASE);
            CANEnable(CAN0_BASE);
        }
        // Check for error warning
        if (error & CAN_STATUS_EWARN) {
            //UARTSend("CAN Error Warning!\n\r");
        }
        // Check for data overrun
        if (error & CAN_STATUS_RXOK) {
            //UARTSend("CAN Data Overrun!\n\r");
        }
        // If any other error is detected, print it
        if (error != 0) {
            //UARTSend("CAN Controller Error: 0x");
            //UARTSendHex(error); // You need to implement UARTSendHex to print the hexadecimal error code
            //UARTSend("\n\r");
        }
        //UARTSend("CAN controller error occurred!\n\r");
        CANIntClear(CAN0_BASE, status); // Clear status interrupt
    }
    else if(status==2){
        CANMessageGet(CAN0_BASE,2,&rxMessage,true);
        //UARTSend("Received CAN Request \n\r");
        OBD_Parser();
        rxMessage.ui32MsgID = 0x7DF;
        rxMessage.ui32MsgIDMask = 0x7FF;
        rxMessage.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;
        rxMessage.ui32MsgLen = 8;
        rxMessage.pui8MsgData = rxData;
        CANMessageSet(CAN0_BASE, 2, &rxMessage, MSG_OBJ_TYPE_RX);
        CANIntClear(CAN0_BASE, status); // clear interrupt
    }
    else {
        // Clear other interrupts
        CANIntClear(CAN0_BASE, status);
        //UARTSend("Didnt Recieve CAN Request \n\r");
    }
}

void OBD_Parser(){
    uint8_t data_length = rxData[1];
    uint8_t response_type = 0;
    char buffer[10];
    int j;
    uint8_t response_PID = 0;
    //uint16_t received_data = 0;
    mode = rxData[1];
    response_PID = rxData[2];
//    sprintf(buffer,"%02X",rxData[0]);
//    UARTSend(buffer);
//    UARTSend("\n");
//    sprintf(buffer,"%02X",rxData[1]);
//    UARTSend(buffer);
//    UARTSend("\n");
//    sprintf(buffer,"%02X",rxData[2]);
//    UARTSend(buffer);
//    UARTSend("\n");
//    sprintf(buffer,"%02X",rxData[3]);
//    UARTSend(buffer);
//    UARTSend("\n");
//    sprintf(buffer,"%02X",rxData[4]);
//    UARTSend(buffer);
//    UARTSend("\n");
//    sprintf(buffer,"%02X",rxData[5]);
//    UARTSend(buffer);
//    UARTSend("\n");
//    sprintf(buffer,"%02X",rxData[6]);
//    UARTSend(buffer);
//    UARTSend("\n");
//    sprintf(buffer,"%02X",rxData[7]);
//    UARTSend(buffer);
//    UARTSend("\n");

    UARTCharPut(UART0_BASE,0xDF);
    UARTCharPut(UART0_BASE,0x07);
    for(j = 0;j<8;j++)
        UARTCharPut(UART0_BASE,rxData[j]);

//    UARTSend("\n");


    CAN_Response(response_PID);

}

void Set_Data(uint8_t byte3,uint8_t byte4,uint8_t byte5,uint8_t byte6,uint8_t byte7){
    txData[3] = byte3;
    txData[4] = byte4;
    txData[5] = byte5;
    txData[6] = byte6;
    txData[7] = byte7;
}


