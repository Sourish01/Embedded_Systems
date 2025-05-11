/*
 * lcd_functions.h
 *
 *  Created on: 06-May-2025
 *      Author: Sourish
 */

#ifndef LCD_FUNCTIONS_H_
#define LCD_FUNCTIONS_H_

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
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/i2c.h"
#include "driverlib/systick.h"
#include "driverlib/rom.h"
#include "driverlib/can.h"
#include <driverlib/adc.h>
#include <driverlib/pwm.h>

#include "grlib/grlib.h"
#include "grlib/widget.h"
#include "grlib/canvas.h"
#include "grlib/checkbox.h"
#include "grlib/container.h"
#include "grlib/pushbutton.h"
#include "grlib/radiobutton.h"
#include "grlib/slider.h"
#include "utils/ustdlib.h"
#include "Kentec320x240x16_ssd2119_spi.h"
#include "touch.h"
#include "ustdlib.h"
#include "math.h"

#include "uart_enable.h"
#include "systick_enable.h"
#include "can_driver.h"

#define ENGINE_RPM 0x0C
#define VEHICLE_SPEED 0x0D
#define FUEL_PRESSURE 0x0A
#define MAF 0x10
#define THROTTLE_POS 0x11
#define CHECK_DTC 0x03
#define CLEAR_DTC 0x04

tContext sContext;

extern tPushButtonWidget HomeEngineRPMButton;
extern tPushButtonWidget HomeVehicleSpeedButton;
extern tPushButtonWidget HomeFuelPressureButton;
extern tPushButtonWidget HomeMAFButton;
extern tPushButtonWidget HomeThrottleButton;
extern tPushButtonWidget MoreButton;
extern tPushButtonWidget HomeCheckDTCButton;
extern tPushButtonWidget HomeClearDTCButton;
extern tPushButtonWidget BackButton;
extern tPushButtonWidget EngineRPMButton;
extern tPushButtonWidget EngineRPMCMButton;
extern tPushButtonWidget VehicleSpeedButton;
extern tPushButtonWidget VehicleSpeedCMButton;
extern tPushButtonWidget FuelPressureButton;
extern tPushButtonWidget FuelPressureCMButton;
extern tPushButtonWidget MAFButton;
extern tPushButtonWidget MAFCMButton;
extern tPushButtonWidget ThrottlePositionButton;
extern tPushButtonWidget ThrottlePositionCMButton;
extern tPushButtonWidget CheckDTCButton;
extern tPushButtonWidget ClearDTCButton;
extern tPushButtonWidget PrevButton;
extern tPushButtonWidget NextButton;
extern tPushButtonWidget HomeButton;

uint8_t DISPLAY_STATE;
extern bool engineRPMCMActive;
extern bool vehicleSpeedCMActive;
extern bool fuelPressureCMActive;
extern bool mafCMActive;
extern bool throttleCMActive;
extern const uint8_t HomeIcon[];

void HomeEngineRPMButtonHandler(tWidget*);
void HomeVehicleSpeedButtonHandler(tWidget*);
void HomeFuelPressureButtonHandler(tWidget*);
void HomeMAFButtonHandler(tWidget*);
void HomeThrottleButtonHandler(tWidget*);
void moreButtonHandler(tWidget*);
void backButtonHandler(tWidget*);
void HomeCheckDTCButtonHandler(tWidget*);
void HomeClearDTCButtonHandler(tWidget*);
void checkDTCHandler(tWidget*);
void clearDTCHandler(tWidget*);
void prevButtonHandler(tWidget*);
void nextButtonHandler(tWidget*);
void HomeHandler(tWidget*);
void backButtonHandler(tWidget*);
void HomeCheckDTCButtonHandler(tWidget*);
void HomeClearDTCButtonHandler(tWidget*);

extern void CAN_Request(uint8_t);

void LCD_Init(void);
void LCD_WriteData(uint16_t);
void LCD_WriteNoDTC(void);
void LCD_WriteDTCs(char*, int);
void LCD_WriteClearDTCs(void);
void ClrData(void);
void ClrDTC(void);

void Display_HomePage(void);
void RemoveHomePage(void);
void DisplayBackPage(void);
void RemoveBackPage(void);
void SetBackground(void);
void SetTitle(void);

void AddHomeWidgets(void);
void AddEngineRPMButtons(void);
void RemoveEngineRPMButtons(void);
void AddVehicleSpeedButtons(void);
void RemoveVehicleSpeedButtons(void);
void AddFuelPressureButtons(void);
void RemoveFuelPressureButtons(void);
void AddMAFButtons(void);
void RemoveMAFButtons(void);
void AddThrottlePosButtons(void);
void RemoveThrottlePosButtons(void);
void AddCheckDTCButton(void);
void RemoveCheckDTCButton(void);
void AddClearDTCButton(void);
void RemoveClearDTCButton(void);
void AddSurroundButtons(void);
void RemoveSurroundButtons(void);

void getOBDDataMode1(tWidget*);
void getCMOBDDataMode1(tWidget*);

void itoa(uint16_t, char*);


#endif /* LCD_FUNCTIONS_H_ */
