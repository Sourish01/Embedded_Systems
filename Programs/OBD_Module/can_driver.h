/*
 * can_driver.h
 *
 *  Created on: 25-Apr-2025
 *      Author: Sourish
 */

#ifndef CAN_DRIVER_H_
#define CAN_DRIVER_H_
tCANMsgObject txMessage;
void CAN_Config(void);
void CAN_Timer_Set(uint32_t timerValue);
void CAN_Timer_Start(void);
void CAN_Timer_Stop(void);
void CAN_Timer_Pause(void);
void CAN_PWM_Send(uint32_t pwm);
void CAN_Set_RTC(uint8_t date,uint8_t mon,uint8_t year,uint8_t hour,uint8_t min,uint8_t sec);

#endif /* CAN_DRIVER_H_ */
