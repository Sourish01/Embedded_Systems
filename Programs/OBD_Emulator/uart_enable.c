/*
 * uart_enable.c
 *
 *  Created on: 11-Mar-2025
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
#include "uart_enable.h"


void UART0_init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
}

void UART0_Int_Enable(void)
{
    IntEnable(INT_UART0);                            // Enable UART0 interrupt in the interrupt controller
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT); // Enable RX and receive timeout interrupts
}

void removeNonAlphanumeric(char *str) {
    int i = 0, j = 0;
    //LED_WHITE;
    while (str[i] != '\0') {
        //UARTSend("\n"); // Extra newline for separation
        //UARTCharSend(&str[i]);
        //UARTSend("\n"); // Extra newline for separation
        if ((isalnum((unsigned char)str[i])) || (((unsigned char)str[i]) == 58)) {  // Keep only alphanumeric characters
            str[j++] = str[i];
        }
        i++;
    }

    str[j] = '\0';  // Null terminate the modified string
    UARTSend("\n"); // Extra newline for separation
    UARTSend(str);
    UARTSend("\n"); // Extra newline for separation

}

void extract_blinkrate_info(char * buffer, char * command, char * blinkrate)
{
    int i = 0 , j = 0 , k = 0;
    int index_command_ends = -1 ;


    while(buffer[i] != '\0'){
        if((buffer[i] >=48 ) && (buffer[i] <= 57)){
            index_command_ends = i ;

            break;
        }
        i++ ;
    }


    if(index_command_ends != -1) {
        //strncpy(command, buffer, index_command_ends);
        j = 0;
        while(buffer[j] != '\0') {
            if(j < index_command_ends){
                command[j] = buffer[j] ;

            } else {
                break;
            }
            j++ ;
        }
        command[index_command_ends] = '\0' ;
        k = 0;
        while(buffer[j] != '\0'){
            blinkrate[k] = buffer[j] ;
            j++ ;
            k++ ;
        }
        blinkrate[k] = '\0' ;

        //strncpy(blinkrate, buffer + index_command_ends, 20 - index_command_ends);
    } else {
        //strncpy(command,buffer,20);
        k = 0 ;
        while(buffer[k] != '\0'){
            command[k] = buffer[k] ;
            k++ ;
        }
        command[k] = '\0' ;
        blinkrate[0] = '\0';
    }
}


void upd_buffer(char curr_val, char * buffer, char * command , char * blinkrate ,  int * state , int * rtc_data, uint8_t *buffer_updated){
    static int buffer_pointer = 0;
    static int str_length = 0;
    static int collected_one_entry = 0;
    //int i = 0 ;
    if(str_length < 29){
        if ((curr_val == 127) || (curr_val == 8)) {
                if(str_length > 0){
                   buffer_pointer --;
                   str_length -- ;
                   buffer[buffer_pointer] = '\0' ;
                }
        } else if(((curr_val >= 65) && (curr_val <= 90)) ){
                buffer[buffer_pointer] = curr_val ;
                buffer_pointer ++ ;
                str_length ++ ;
        } else if(((curr_val >= 48) && (curr_val <= 57)) ){
            buffer[buffer_pointer] = curr_val ;
            buffer_pointer ++ ;
            str_length ++ ;
        } else if (((curr_val >= 97) && (curr_val <= 122)) ){
                buffer[buffer_pointer] = curr_val - 32 ;
                buffer_pointer ++ ;
                str_length ++ ;
        } else if ((curr_val == 32) || (curr_val == 9)){
                buffer[buffer_pointer] = curr_val ;
                buffer_pointer ++ ;
                str_length ++ ;
        } else if ((curr_val == 58)){
            buffer[buffer_pointer] = curr_val ;
            buffer_pointer ++ ;
            str_length ++ ;
        }
     }

    if(str_length == 29){
        collected_one_entry = 1 ;
        buffer[buffer_pointer] = '\0' ;
        removeNonAlphanumeric(buffer);
    } else if((str_length < 29 ) && (curr_val == 13)){
        collected_one_entry = 1 ;
        buffer[buffer_pointer] = '\0' ;
        removeNonAlphanumeric(buffer);
    }

    if(collected_one_entry == 1) {
        *buffer_updated = 1 ;
        extract_blinkrate_info(buffer, command, blinkrate);
        //*blink_rate = atoi(blinkrate) ;
        perform_action(command,state,rtc_data);


        buffer_pointer = 0;
        str_length = 0;
        collected_one_entry = 0;

    }


}

void perform_action(char * buffer, int * state, int * rtc_data){
    if(strcmp(buffer,"SWTSET") == 0){
            *state = 1 ;
            *rtc_data = 0 ;
    } else if(strcmp(buffer,"SWTSTART") == 0){
        *state = 2 ;
        *rtc_data = 0 ;
    } else if (strcmp(buffer,"SWTSTOP") == 0){
        *state = 9 ;
        *rtc_data = 0 ;
    } else if (strcmp(buffer,"SWTPAUSE") == 0){
        *state = 3 ;
        *rtc_data = 0 ;
    } else if (strcmp(buffer,"SWTRESUME") == 0){
        *state = 2 ;
        *rtc_data = 0 ;
    } else if (strcmp(buffer,"SETRTC") == 0){
        *rtc_data = 1 ;
    }
}

void UARTSend(const char *pucBuffer)
{
    while (*pucBuffer)
    {
        UARTCharPut(UART0_BASE, *pucBuffer++);
    }
}

void UARTCharSend(const char *pucBuffer)
{
    //while (*pucBuffer)
    //{
        UARTCharPut(UART0_BASE, *pucBuffer);
    //}
}
// Function to send hexadecimal value over UART
void UARTSendHex(uint32_t value) {
    char hexString[9];  // Buffer to hold the 8-digit hex string
    uint8_t i;
    // Convert the value into a hexadecimal string
    for (i = 0; i < 8; i++) {
        uint8_t nibble = (value >> (28 - 4 * i)) & 0xF;
        if (nibble < 10) {
            hexString[i] = '0' + nibble;  // Convert to character for 0-9
        } else {
            hexString[i] = 'A' + (nibble - 10);  // Convert to character for A-F
        }
    }
    hexString[8] = '\0';  // Null-terminate the string

    // Send the hex string over UART
    UARTSend(hexString);
}
