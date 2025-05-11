

/**
 * main.c
 */
#include "lcd_functions.h"
//

tCANMsgObject txMessage;
tCANMsgObject rxMessage;

volatile uint32_t CMTicks = 0;
volatile uint32_t prevCMTicks = 0;

uint8_t txData[8];
uint8_t rxData[8];
bool gotOBD = false;

//Peripheral Interrupt Handlers,Processing
void SysTick_Handler(void);
void CAN0_Handler(void);
void OBD_Parser(uint8_t*);
void ProcessDTC(int,char*);
void CAN_Request(uint8_t);


int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    uint32_t ui32SysClock = SysCtlClockGet();
    SysTick_Init();
    Systick_Interrupt_Enable();
    UART0_init();
    CAN_Config();
    LCD_Init();
    Display_HomePage();
    IntMasterEnable();

    while(1){
        WidgetMessageQueueProcess();
        if(gotOBD==true){
            gotOBD = false;
            OBD_Parser(rxData);
        }
    }
    return 0;
}

void CAN_Request(uint8_t pid){
    txMessage.ui32MsgID = 0x7DF;
    txMessage.ui32Flags = 0;
    txMessage.ui32MsgLen = 8;
    if(pid==0x03||pid==0x04){
        txData[0] = 1;
        txData[1] = pid;
        int i;
        for(i=2;i<8;i++){
            txData[i] = 0;
        }
    }
    else{
        txData[0] = 2;
        txData[1] = 1;
        txData[2] = pid;
        int i;
        for(i=3;i<8;i++){
            txData[i] = 0;
        }
    }
    txMessage.pui8MsgData = txData;
    CANMessageSet(CAN0_BASE,2,&txMessage,MSG_OBJ_TYPE_TX);
}

void SysTick_Handler(void){
    if((engineRPMCMActive||vehicleSpeedCMActive||fuelPressureCMActive||mafCMActive||throttleCMActive)==true){
        if(CMTicks-prevCMTicks<150){
            CMTicks++;
        }
        else{
            ClrData();
            CAN_Request(DISPLAY_STATE);
            prevCMTicks = CMTicks;
        }
    }
}

void CAN0_Handler(void){
    uint32_t status = CANIntStatus(CAN0_BASE,CAN_INT_STS_CAUSE);

    if(status==CAN_INT_INTID_STATUS){
        uint32_t error = CANStatusGet(CAN0_BASE,CAN_STS_CONTROL);
        if (error & CAN_STATUS_BUS_OFF) {
            CANDisable(CAN0_BASE);
            CANEnable(CAN0_BASE);
        }
        if (error & CAN_STATUS_EWARN) {
        }
        if (error & CAN_STATUS_RXOK) {
        }
        if (error != 0) {
        }
        CANIntClear(CAN0_BASE, status);
    }
    else if(status==1){
        CANMessageGet(CAN0_BASE,1,&rxMessage,true);
        gotOBD = true;
        rxMessage.ui32MsgID = 0x7E8;
        rxMessage.ui32MsgIDMask = 0x7FF;
        rxMessage.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;
        rxMessage.ui32MsgLen = 8;
        rxMessage.pui8MsgData = rxData;
        CANMessageSet(CAN0_BASE, 1, &rxMessage, MSG_OBJ_TYPE_RX);
        CANIntClear(CAN0_BASE, status); // clear interrupt
    }
    else {

        CANIntClear(CAN0_BASE, status);
    }
}


void OBD_Parser(uint8_t* rxData){
    uint8_t data_length = rxData[0];
    uint8_t response_type = rxData[1];
    if(response_type==0x41){
        uint8_t response_PID = 0;
        uint16_t received_data = 0;
        int i;
        for(i=2;i<1+data_length;i++){
            if(i==2){
                response_PID = rxData[i];
            }
            else{
                received_data = (received_data<<8)|rxData[i];
            }
        }
        switch(response_PID){
            case(ENGINE_RPM):{
                uint16_t rpmData = received_data/4;
                LCD_WriteData(rpmData);
                break;
            }
            case(VEHICLE_SPEED):{
                uint16_t speedData = received_data;
                LCD_WriteData(speedData);
                break;
            }
            case(FUEL_PRESSURE):{
                uint16_t pressureData = received_data*3;
                LCD_WriteData(pressureData);
                break;
            }
            case(MAF):{
                uint16_t mafData = received_data/100;
                LCD_WriteData(mafData);
                break;
            }
            case(THROTTLE_POS):{
                uint16_t throttleData = (uint16_t)((received_data*100)>>8);
                LCD_WriteData(throttleData);
                break;
            }
            default:break;
        }
    }
    else if(response_type == 0x43){
        uint8_t numDTCs = (data_length - 1) / 2;
        if(numDTCs > 3) numDTCs = 3;
        if(numDTCs==0){
            LCD_WriteNoDTC();
        }
        int i;
        for(i = 0; i < numDTCs; i++){
            char displayBuffer[30];
            ProcessDTC(i,displayBuffer);
            LCD_WriteDTCs(displayBuffer,i);
        }
    }
    else if(response_type==0x44){
        LCD_WriteClearDTCs();
    }
}

void ProcessDTC(int idx,char* displayBuffer){
    uint8_t msb = rxData[2 + idx*2];
    uint8_t lsb = rxData[3 + idx*2];

    uint8_t ch1 = (msb & 0xC0) >> 6;
    char* system;
    switch(ch1){
        case 0: system = "P"; break;
        case 1: system = "C"; break;
        case 2: system = "B"; break;
        case 3: system = "U"; break;
    }

    uint8_t first_digit = (msb & 0x30) >> 4;
    uint8_t second_digit = (msb & 0x0F);
    uint8_t third_digit = (lsb & 0xF0) >> 4;
    uint8_t fourth_digit = (lsb & 0x0F);

    char firstdigitbuf[6];
    char seconddigitbuf[6];
    char thirddigitbuf[6];
    char fourthdigitbuf[6];
    itoa((uint16_t)first_digit,firstdigitbuf);
    itoa((uint16_t)second_digit,seconddigitbuf);
    itoa((uint16_t)third_digit,thirddigitbuf);
    itoa((uint16_t)fourth_digit,fourthdigitbuf);
    strcpy(displayBuffer,system);
    strcat(displayBuffer,firstdigitbuf);
    strcat(displayBuffer,seconddigitbuf);
    strcat(displayBuffer,thirddigitbuf);
    strcat(displayBuffer,fourthdigitbuf);
}
