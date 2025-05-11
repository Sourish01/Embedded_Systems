/*
 * rtc_i2c.c
 *
 *  Created on: 25-Apr-2025
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
#include <driverlib/uart.h>
#include "uart_enable.h"
#include "rtc_i2c.h"

// Initialize I2C3 on PD0(SCL) and PD1(SDA)
/*void I2C3_Init(void) {
    SYSCTL_RCGCI2C_R |= 0x08;       // Enable clock to I2C3
    SYSCTL_RCGCGPIO_R |= 0x08;      // Enable clock to GPIOD

    // Configure PORTD pins for I2C3
    GPIO_PORTD_AFSEL_R |= 0x03;     // PD0, PD1 for I2C3
    GPIO_PORTD_PCTL_R &= ~0x000000FF;
    GPIO_PORTD_PCTL_R |= 0x00000033; // Configure PD0, PD1 for I2C
    GPIO_PORTD_DEN_R |= 0x03;       // Digital enable PD0, PD1
    GPIO_PORTD_ODR_R |= 0x02;       // Open drain on SDA (PD1)

    I2C3_MCR_R = 0x10;              // Master mode
    I2C3_MTPR_R = 7;                // 100kHz @ 16MHz system clock
}*/
// Initialize I2C3 on PE4(SCL) and PE5(SDA)
void I2C2_Init(void) {
    SYSCTL_RCGCI2C_R |= 0x04;       // Enable clock to I2C2
    SYSCTL_RCGCGPIO_R |= 0x10;      // Enable clock to GPIOD

    // Configure PORTD pins for I2C3
    GPIO_PORTE_AFSEL_R |= 0x30;     // PE4 and PE5 for I2C3
    GPIO_PORTE_PCTL_R &= ~0x00FF0000;
    GPIO_PORTE_PCTL_R |= 0x00330000; // Configure PE4, PE5 for I2C
    GPIO_PORTE_DEN_R |= 0x30;       // Digital enable PE4, PE5
    GPIO_PORTE_ODR_R |= 0x20;       // Open drain on SDA (PD1)

    I2C2_MCR_R = 0x10;              // Master mode
    I2C2_MTPR_R = 7;                // 100kHz @ 16MHz system clock
}
/*
// Wait until I2C3 master is not busy and return error code
uint8_t I2C_Wait_Till_Done(void) {
    while(I2C3_MCS_R & 0x01) {}     // Wait until I2C master is not busy
    return I2C3_MCS_R & 0x0E;       // Return I2C error code
}

// Write one byte to an I2C device
uint8_t I2C3_ByteWrite(int slaveAddr, uint8_t memAddr, uint8_t data) {
    //LED_RED;
    uint8_t error;

    // Send slave address and register address
    I2C3_MSA_R = slaveAddr << 1;    // LSB = 0 for write
    I2C3_MDR_R = memAddr;
    I2C3_MCS_R = 0x03;              // Start, transmit
    error = I2C_Wait_Till_Done();
    if(error) return error;

    // Send data
    I2C3_MDR_R = data;
    I2C3_MCS_R = 0x05;              // Transmit, stop
    error = I2C_Wait_Till_Done();

    // Wait until bus is not busy
    while(I2C3_MCS_R & 0x40) {}

    return error;
}

// Read multiple bytes from an I2C device
uint8_t I2C3_Read(int slaveAddr, uint8_t memAddr, int byteCount, uint8_t* data) {
    uint8_t error;

    if(byteCount <= 0)
        return 0xFF;  // Error code for invalid byte count

    // Send slave address and register address
    I2C3_MSA_R = slaveAddr << 1;    // LSB = 0 for write
    I2C3_MDR_R = memAddr;
    I2C3_MCS_R = 0x03;              // Start, transmit
    error = I2C_Wait_Till_Done();
    if(error) return error;

    // Switch to read mode
    I2C3_MSA_R = (slaveAddr << 1) + 1;  // LSB = 1 for read

    if(byteCount == 1) {
        I2C3_MCS_R = 0x07;          // Start, receive, stop
    } else {
        I2C3_MCS_R = 0x0B;          // Start, receive, ack
    }

    error = I2C_Wait_Till_Done();
    if(error) return error;

    *data++ = I2C3_MDR_R;           // Store received data

    if(--byteCount == 0) {
        while(I2C3_MCS_R & 0x40) {} // Wait until bus is not busy
        return 0;                   // No error
    }

    // Read remaining bytes
    while(byteCount > 1) {
        I2C3_MCS_R = 0x09;          // Receive, ack
        error = I2C_Wait_Till_Done();
        if(error) return error;

        *data++ = I2C3_MDR_R;
        byteCount--;
    }

    // Read last byte
    I2C3_MCS_R = 0x05;              // Receive, stop
    error = I2C_Wait_Till_Done();
    *data = I2C3_MDR_R;

    while(I2C3_MCS_R & 0x40) {}     // Wait until bus is not busy

    return 0;
}*/
// Wait until I2C3 master is not busy and return error code
uint8_t I2C_Wait_Till_Done(void) {
    while(I2C2_MCS_R & 0x01) {}     // Wait until I2C master is not busy
    return I2C2_MCS_R & 0x0E;       // Return I2C error code
}

// Write one byte to an I2C device
uint8_t I2C2_ByteWrite(int slaveAddr, uint8_t memAddr, uint8_t data) {
    ////LED_RED;
    uint8_t error;

    // Send slave address and register address
    I2C2_MSA_R = slaveAddr << 1;    // LSB = 0 for write
    I2C2_MDR_R = memAddr;
    I2C2_MCS_R = 0x03;              // Start, transmit
    error = I2C_Wait_Till_Done();
    if(error) return error;

    // Send data
    I2C2_MDR_R = data;
    I2C2_MCS_R = 0x05;              // Transmit, stop
    error = I2C_Wait_Till_Done();

    // Wait until bus is not busy
    while(I2C2_MCS_R & 0x40) {}

    return error;
}

// Read multiple bytes from an I2C device
uint8_t I2C2_Read(int slaveAddr, uint8_t memAddr, int byteCount, uint8_t* data) {
    uint8_t error;

    if(byteCount <= 0)
        return 0xFF;  // Error code for invalid byte count

    // Send slave address and register address
    I2C2_MSA_R = slaveAddr << 1;    // LSB = 0 for write
    I2C2_MDR_R = memAddr;
    I2C2_MCS_R = 0x03;              // Start, transmit
    error = I2C_Wait_Till_Done();
    if(error) return error;

    // Switch to read mode
    I2C2_MSA_R = (slaveAddr << 1) + 1;  // LSB = 1 for read

    if(byteCount == 1) {
        I2C2_MCS_R = 0x07;          // Start, receive, stop
    } else {
        I2C2_MCS_R = 0x0B;          // Start, receive, ack
    }

    error = I2C_Wait_Till_Done();
    if(error) return error;

    *data++ = I2C2_MDR_R;           // Store received data

    if(--byteCount == 0) {
        while(I2C2_MCS_R & 0x40) {} // Wait until bus is not busy
        return 0;                   // No error
    }

    // Read remaining bytes
    while(byteCount > 1) {
        I2C2_MCS_R = 0x09;          // Receive, ack
        error = I2C_Wait_Till_Done();
        if(error) return error;

        *data++ = I2C2_MDR_R;
        byteCount--;
    }

    // Read last byte
    I2C2_MCS_R = 0x05;              // Receive, stop
    error = I2C_Wait_Till_Done();
    *data = I2C2_MDR_R;

    while(I2C2_MCS_R & 0x40) {}     // Wait until bus is not busy

    return 0;
}
// Convert BCD to decimal
uint8_t BCD_to_Decimal(uint8_t bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

// Convert decimal to BCD
uint8_t Decimal_to_BCD(uint8_t decimal) {
    return ((decimal / 10) << 4) | (decimal % 10);
}
/*
// Initialize RTC
void RTC_Init(void) {
    uint8_t ctrlReg;

    // Read control register
    I2C3_Read(SLAVE_ADDR, CONTROL_REG, 1, &ctrlReg);

    // Set control register (enable oscillator, disable square wave)
    I2C3_ByteWrite(SLAVE_ADDR, CONTROL_REG, 0x00);

    // Ensure CH bit is cleared (bit 7 of seconds register)
    I2C3_ByteWrite(SLAVE_ADDR, SEC_REG, 0x00);

    // Uncomment to set initial time if needed
    // Set time to 12:00:00
    I2C3_ByteWrite(SLAVE_ADDR, HOUR_REG, 0x12);
    I2C3_ByteWrite(SLAVE_ADDR, MIN_REG, 0x07);
    I2C3_ByteWrite(SLAVE_ADDR, SEC_REG, 0x00);

    // Uncomment to set initial date if needed
    // Set date to 04/04/2025 (Friday)
    I2C3_ByteWrite(SLAVE_ADDR, DAY_REG, 0x07);     // Friday = 6
    I2C3_ByteWrite(SLAVE_ADDR, DATE_REG, 0x05);    // 4th
    I2C3_ByteWrite(SLAVE_ADDR, MONTH_REG, 0x04);   // April
    I2C3_ByteWrite(SLAVE_ADDR, YEAR_REG, 0x25);    // 2025
}

void RTC_write(uint8_t hour, uint8_t min, uint8_t sec,uint8_t day, uint8_t date, uint8_t month, uint8_t year){
    uint8_t ctrlReg;

    // Read control register
    I2C3_Read(SLAVE_ADDR, CONTROL_REG, 1, &ctrlReg);

    // Set control register (enable oscillator, disable square wave)
    I2C3_ByteWrite(SLAVE_ADDR, CONTROL_REG, 0x00);

    // Ensure CH bit is cleared (bit 7 of seconds register)
    I2C3_ByteWrite(SLAVE_ADDR, SEC_REG, 0x00);

    // Uncomment to set initial time if needed
    // Set time to 12:00:00
    I2C3_ByteWrite(SLAVE_ADDR, HOUR_REG, hour);
    I2C3_ByteWrite(SLAVE_ADDR, MIN_REG, min);
    I2C3_ByteWrite(SLAVE_ADDR, SEC_REG, sec);

    // Uncomment to set initial date if needed
    // Set date to 04/04/2025 (Friday)
    I2C3_ByteWrite(SLAVE_ADDR, DAY_REG, day);     // Friday = 6
    I2C3_ByteWrite(SLAVE_ADDR, DATE_REG, date);    // 4th
    I2C3_ByteWrite(SLAVE_ADDR, MONTH_REG, month);   // April
    I2C3_ByteWrite(SLAVE_ADDR, YEAR_REG, year);    // 2025
    ////LED_RED;
}

// Get date and time from RTC
void RTC_GetDateTime(uint8_t *hour, uint8_t *min, uint8_t *sec,
                     uint8_t *day, uint8_t *date, uint8_t *month, uint8_t *year) {
    uint8_t data[7];

    // Read all time/date registers in one burst read
    I2C3_Read(SLAVE_ADDR, 0x00, 7, data);

    *sec = BCD_to_Decimal(data[0] & 0x7F);  // Mask out CH bit
    *min = BCD_to_Decimal(data[1]);

    // Check if 12-hour or 24-hour mode
    if(data[2] & 0x40) {
        // 12-hour mode
        *hour = BCD_to_Decimal(data[2] & 0x1F);
        // Adjust for PM
        if(data[2] & 0x20) {
            *hour += 12;
            if(*hour == 24) *hour = 12;  // 12 PM should be 12, not 24
        } else {
            if(*hour == 12) *hour = 0;   // 12 AM should be 0
        }
    } else {
        // 24-hour mode
        *hour = BCD_to_Decimal(data[2] & 0x3F);
    }

    *day = BCD_to_Decimal(data[3]);
    *date = BCD_to_Decimal(data[4]);
    *month = BCD_to_Decimal(data[5]);
    *year = BCD_to_Decimal(data[6]);
}*/
// Initialize RTC
void RTC_Init(void) {
    uint8_t ctrlReg;

    // Read control register
    I2C2_Read(SLAVE_ADDR, CONTROL_REG, 1, &ctrlReg);

    // Set control register (enable oscillator, disable square wave)
    I2C2_ByteWrite(SLAVE_ADDR, CONTROL_REG, 0x00);

    // Ensure CH bit is cleared (bit 7 of seconds register)
    I2C2_ByteWrite(SLAVE_ADDR, SEC_REG, 0x00);

    // Uncomment to set initial time if needed
    // Set time to 12:00:00
    I2C2_ByteWrite(SLAVE_ADDR, HOUR_REG, 0x12);
    I2C2_ByteWrite(SLAVE_ADDR, MIN_REG, 0x07);
    I2C2_ByteWrite(SLAVE_ADDR, SEC_REG, 0x00);

    // Uncomment to set initial date if needed
    // Set date to 04/04/2025 (Friday)
    I2C2_ByteWrite(SLAVE_ADDR, DAY_REG, 0x07);     // Friday = 6
    I2C2_ByteWrite(SLAVE_ADDR, DATE_REG, 0x05);    // 4th
    I2C2_ByteWrite(SLAVE_ADDR, MONTH_REG, 0x04);   // April
    I2C2_ByteWrite(SLAVE_ADDR, YEAR_REG, 0x25);    // 2025
}

void RTC_write(uint8_t hour, uint8_t min, uint8_t sec,uint8_t day, uint8_t date, uint8_t month, uint8_t year){
    uint8_t ctrlReg;

    // Read control register
    I2C2_Read(SLAVE_ADDR, CONTROL_REG, 1, &ctrlReg);

    // Set control register (enable oscillator, disable square wave)
    I2C2_ByteWrite(SLAVE_ADDR, CONTROL_REG, 0x00);

    // Ensure CH bit is cleared (bit 7 of seconds register)
    I2C2_ByteWrite(SLAVE_ADDR, SEC_REG, 0x00);

    // Uncomment to set initial time if needed
    // Set time to 12:00:00
    I2C2_ByteWrite(SLAVE_ADDR, HOUR_REG, hour);
    I2C2_ByteWrite(SLAVE_ADDR, MIN_REG, min);
    I2C2_ByteWrite(SLAVE_ADDR, SEC_REG, sec);

    // Uncomment to set initial date if needed
    // Set date to 04/04/2025 (Friday)
    I2C2_ByteWrite(SLAVE_ADDR, DAY_REG, day);     // Friday = 6
    I2C2_ByteWrite(SLAVE_ADDR, DATE_REG, date);    // 4th
    I2C2_ByteWrite(SLAVE_ADDR, MONTH_REG, month);   // April
    I2C2_ByteWrite(SLAVE_ADDR, YEAR_REG, year);    // 2025
    ////LED_RED;
}

// Get date and time from RTC
void RTC_GetDateTime(uint8_t *hour, uint8_t *min, uint8_t *sec,
                     uint8_t *day, uint8_t *date, uint8_t *month, uint8_t *year) {
    uint8_t data[7];

    // Read all time/date registers in one burst read
    I2C2_Read(SLAVE_ADDR, 0x00, 7, data);

    *sec = BCD_to_Decimal(data[0] & 0x7F);  // Mask out CH bit
    *min = BCD_to_Decimal(data[1]);

    // Check if 12-hour or 24-hour mode
    if(data[2] & 0x40) {
        // 12-hour mode
        *hour = BCD_to_Decimal(data[2] & 0x1F);
        // Adjust for PM
        if(data[2] & 0x20) {
            *hour += 12;
            if(*hour == 24) *hour = 12;  // 12 PM should be 12, not 24
        } else {
            if(*hour == 12) *hour = 0;   // 12 AM should be 0
        }
    } else {
        // 24-hour mode
        *hour = BCD_to_Decimal(data[2] & 0x3F);
    }

    *day = BCD_to_Decimal(data[3]);
    *date = BCD_to_Decimal(data[4]);
    *month = BCD_to_Decimal(data[5]);
    *year = BCD_to_Decimal(data[6]);
}

// Update UART display with current date and time
void Update_Display(char *dateStr, char *timeStr,char *fancy_dateStr, char *fancy_timeStr) {
    uint8_t hour, min, sec, day, date, month, year;
//    char dateStr[16], timeStr[16];
    static uint8_t prevSec = 0xFF;  // To track if seconds have changed
    //char dateStr[16], timeStr[16];

    // Get current date and time
    RTC_GetDateTime(&hour, &min, &sec, &day, &date, &month, &year);

    // Only update display if seconds have changed
    if(sec != prevSec) {
        // Format date string: dd/mm/yyyy
        dateStr[0] = (date / 10) + '0';
        dateStr[1] = (date % 10) + '0';
        dateStr[2] = '/';
        dateStr[3] = (month / 10) + '0';
        dateStr[4] = (month % 10) + '0';
        dateStr[5] = '/';
        dateStr[6] = '2';
        dateStr[7] = '0';
        dateStr[8] = (year / 10) + '0';
        dateStr[9] = (year % 10) + '0';
        dateStr[10] = '\0';

        fancy_dateStr[0] = '2' ;
        fancy_dateStr[1] = '0' ;
        fancy_dateStr[2] = (year / 10) + '0' ;
        fancy_dateStr[3] = (year % 10) + '0' ;
        fancy_dateStr[4] = '\0' ;

        // Format time string: hh:mm:ss
        timeStr[0] = (hour / 10) + '0';
        timeStr[1] = (hour % 10) + '0';
        timeStr[2] = ':';
        timeStr[3] = (min / 10) + '0';
        timeStr[4] = (min % 10) + '0';
        timeStr[5] = ':';
        timeStr[6] = (sec / 10) + '0';
        timeStr[7] = (sec % 10) + '0';
        timeStr[8] = '\0';

        // Format time string: hh:mm:ss
        fancy_timeStr[0] = dateStr[0];
        fancy_timeStr[1] = dateStr[1];
        fancy_timeStr[2] = dateStr[2];
        fancy_timeStr[3] = dateStr[3];
        fancy_timeStr[4] = dateStr[4];
        fancy_timeStr[5] = ' ' ;
        fancy_timeStr[6] = (hour / 10) + '0';
        fancy_timeStr[7] = (hour % 10) + '0';
        fancy_timeStr[8] = ':';
        fancy_timeStr[9] = (min / 10) + '0';
        fancy_timeStr[10] = (min % 10) + '0';
        fancy_timeStr[11] = ':';
        fancy_timeStr[12] = (sec / 10) + '0';
        fancy_timeStr[13] = (sec % 10) + '0';
        fancy_timeStr[14] = '\0';



        // Send carriage return to return to start of line
        //UARTSend("\r");

        // Display date and time
        //UARTSend("Date: ");
        //UARTSend(dateStr);
        //UARTSend(" | Time: ");
        //UARTSend(timeStr);

        //LCD_Print(dateStr);
        //LCD_Cmd(0xC0);  // Move to 2nd line
        //LCD_Print(timeStr);
        //LCD_DisplayTimestampAndDate(timeStr, dateStr);

        prevSec = sec;  // Update previous second
    }
}
