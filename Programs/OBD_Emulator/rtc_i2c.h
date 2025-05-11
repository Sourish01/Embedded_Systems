/*
 * rtc_i2c.h
 *
 *  Created on: 25-Apr-2025
 *      Author: A Jaaneshwaran
 */

#ifndef RTC_I2C_H_
#define RTC_I2C_H_

// DS1307 parameters
#define SLAVE_ADDR              0x68        // 1101000 = 0x68
#define SEC_REG                 0x00
#define MIN_REG                 0x01
#define HOUR_REG                0x02
#define DAY_REG                 0x03
#define DATE_REG                0x04
#define MONTH_REG               0x05
#define YEAR_REG                0x06
#define CONTROL_REG             0x07

extern void I2C2_Init(void);
extern uint8_t I2C2_ByteWrite(int slaveAddr, uint8_t memAddr, uint8_t data);
extern uint8_t I2C2_Read(int slaveAddr, uint8_t memAddr, int byteCount, uint8_t* data);
extern uint8_t I2C_Wait_Till_Done(void);
extern uint8_t BCD_to_Decimal(uint8_t bcd);
extern uint8_t Decimal_to_BCD(uint8_t decimal);
extern void RTC_Init(void);
extern void RTC_write(uint8_t hour, uint8_t min, uint8_t sec,
               uint8_t day, uint8_t date, uint8_t month, uint8_t year);
extern void RTC_GetDateTime(uint8_t *hour, uint8_t *min, uint8_t *sec,
                     uint8_t *day, uint8_t *date, uint8_t *month, uint8_t *year);
extern void Update_Display(char *dateStr, char *timeStr,char *fancy_dateStr, char *fancy_timeStr);



#endif /* RTC_I2C_H_ */
