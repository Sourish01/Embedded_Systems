/*
 * lcd_functions.c
 *
 *  Created on: 06-May-2025
 *      Author: Sourish
 */
#include "lcd_functions.h"

bool engineRPMCMActive = false;
bool vehicleSpeedCMActive = false;
bool fuelPressureCMActive = false;
bool mafCMActive = false;
bool throttleCMActive = false;

const uint8_t HomeIcon[] =
{
    IMAGE_FMT_8BPP_UNCOMP,  // Uncompressed 8-bit format
    20, 0,                  // Width: 20 pixels (little-endian)
    20, 0,                  // Height: 20 pixels

    // Color palette (2 entries)
    0X00, 0x01,                      // 1 palette entry (2 colors total)
    0x00, 0xff, 0x00,       // Index 0: Light Sea Green (background, in BGR)
    0x00, 0x00, 0xff,       // Index 1: Red (used for foreground/home icon)

    // Pixel data (20x20 = 400 bytes)
    // 0 = transparent, 1 = red
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x01,0x01,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x01,0x01,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x01,0x01,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

RectangularButton(HomeEngineRPMButton, NULL, NULL, NULL, &g_sKentec320x240x16_SSD2119, 85, 40,
                      150, 25, PB_STYLE_FILL | PB_STYLE_TEXT | PB_STYLE_RELEASE_NOTIFY| PB_STYLE_OUTLINE, ClrLightSeaGreen , ClrDarkGreen, 0, ClrWhite,
                      &g_sFontCm18b, "Engine RPM", NULL, NULL, 0, 0,
                      HomeEngineRPMButtonHandler);

RectangularButton(HomeVehicleSpeedButton, NULL, NULL, NULL, &g_sKentec320x240x16_SSD2119, 85, 80,
                      150, 25, PB_STYLE_FILL | PB_STYLE_TEXT | PB_STYLE_RELEASE_NOTIFY| PB_STYLE_OUTLINE, ClrLightSeaGreen , ClrDarkGreen, 0, ClrWhite,
                      &g_sFontCm18b, "Vehicle Speed", NULL, NULL, 0, 0,
                      HomeVehicleSpeedButtonHandler);

RectangularButton(HomeFuelPressureButton, NULL, NULL, NULL, &g_sKentec320x240x16_SSD2119, 85, 120,
                      150, 25, PB_STYLE_FILL | PB_STYLE_TEXT | PB_STYLE_RELEASE_NOTIFY| PB_STYLE_OUTLINE, ClrLightSeaGreen , ClrDarkGreen, 0, ClrWhite,
                      &g_sFontCm18b, "Fuel Pressure", NULL, NULL, 0, 0,
                      HomeFuelPressureButtonHandler);

RectangularButton(HomeMAFButton, NULL, NULL, NULL, &g_sKentec320x240x16_SSD2119, 85, 160,
                      150, 25, PB_STYLE_FILL | PB_STYLE_TEXT | PB_STYLE_RELEASE_NOTIFY| PB_STYLE_OUTLINE, ClrLightSeaGreen , ClrDarkGreen, 0, ClrWhite,
                      &g_sFontCm18b, "Mass Air Flow", NULL, NULL, 0, 0,
                      HomeMAFButtonHandler);

RectangularButton(HomeThrottleButton, NULL, NULL, NULL, &g_sKentec320x240x16_SSD2119, 85, 200,
                      150, 25, PB_STYLE_FILL | PB_STYLE_TEXT | PB_STYLE_RELEASE_NOTIFY| PB_STYLE_OUTLINE, ClrLightSeaGreen , ClrDarkGreen, 0, ClrWhite,
                      &g_sFontCm18b, "Throttle Position", NULL, NULL, 0, 0,
                      HomeThrottleButtonHandler);

RectangularButton(MoreButton, NULL, NULL, NULL, &g_sKentec320x240x16_SSD2119, 250, 190,
                      60, 25, PB_STYLE_FILL | PB_STYLE_TEXT | PB_STYLE_RELEASE_NOTIFY| PB_STYLE_OUTLINE, ClrPurple , ClrDarkGreen, 0, ClrWhite,
                      &g_sFontCm18b, "More..", NULL, NULL, 0, 0,
                      moreButtonHandler);

RectangularButton(HomeCheckDTCButton, NULL, NULL, NULL, &g_sKentec320x240x16_SSD2119, 85, 80,
                      150, 25, PB_STYLE_FILL | PB_STYLE_TEXT | PB_STYLE_RELEASE_NOTIFY| PB_STYLE_OUTLINE, ClrLightSeaGreen , ClrDarkGreen, 0, ClrWhite,
                      &g_sFontCm18b, "Check DTC", NULL, NULL, 0, 0,
                      HomeCheckDTCButtonHandler);

RectangularButton(HomeClearDTCButton, NULL, NULL, NULL, &g_sKentec320x240x16_SSD2119, 85, 120,
                      150, 25, PB_STYLE_FILL | PB_STYLE_TEXT | PB_STYLE_RELEASE_NOTIFY| PB_STYLE_OUTLINE, ClrLightSeaGreen , ClrDarkGreen, 0, ClrWhite,
                      &g_sFontCm18b, "Clear DTC", NULL, NULL, 0, 0,
                      HomeClearDTCButtonHandler);

RectangularButton(BackButton, NULL, NULL, NULL, &g_sKentec320x240x16_SSD2119, 10, 190,
                      60, 25, PB_STYLE_FILL | PB_STYLE_TEXT | PB_STYLE_RELEASE_NOTIFY| PB_STYLE_OUTLINE, ClrPurple , ClrDarkGreen, 0, ClrWhite,
                      &g_sFontCm18b, "Back", NULL, NULL, 0, 0,
                      backButtonHandler);

RectangularButton(EngineRPMButton, NULL, NULL, NULL, &g_sKentec320x240x16_SSD2119, 85, 160,
                      150, 25, PB_STYLE_FILL | PB_STYLE_TEXT | PB_STYLE_RELEASE_NOTIFY| PB_STYLE_OUTLINE, ClrLightSeaGreen , ClrDarkGreen, 0, ClrWhite,
                      &g_sFontCm18b, "Engine RPM", NULL, NULL, 0, 0,
                      getOBDDataMode1);

RectangularButton(EngineRPMCMButton, NULL, NULL, NULL, &g_sKentec320x240x16_SSD2119, 85, 120,
                      150, 25, PB_STYLE_FILL | PB_STYLE_TEXT | PB_STYLE_RELEASE_NOTIFY| PB_STYLE_OUTLINE, ClrLightSeaGreen , ClrDarkGreen, 0, ClrWhite,
                      &g_sFontCm16b, "Engine RPM (CM)", NULL, NULL, 0, 0,
                      getCMOBDDataMode1);

RectangularButton(VehicleSpeedButton, NULL, NULL, NULL, &g_sKentec320x240x16_SSD2119, 85, 160,
                      150, 25, PB_STYLE_FILL | PB_STYLE_TEXT | PB_STYLE_RELEASE_NOTIFY| PB_STYLE_OUTLINE, ClrLightSeaGreen , ClrDarkGreen, 0, ClrWhite,
                      &g_sFontCm18b, "Vehicle Speed", NULL, NULL, 0, 0,
                      getOBDDataMode1);

RectangularButton(VehicleSpeedCMButton, NULL, NULL, NULL, &g_sKentec320x240x16_SSD2119, 85, 120,
                      150, 25, PB_STYLE_FILL | PB_STYLE_TEXT | PB_STYLE_RELEASE_NOTIFY| PB_STYLE_OUTLINE, ClrLightSeaGreen , ClrDarkGreen, 0, ClrWhite,
                      &g_sFontCm16b, "Vehicle Speed (CM)", NULL, NULL, 0, 0,
                      getCMOBDDataMode1);

RectangularButton(FuelPressureButton, NULL, NULL, NULL, &g_sKentec320x240x16_SSD2119, 85, 160,
                      150, 25, PB_STYLE_FILL | PB_STYLE_TEXT | PB_STYLE_RELEASE_NOTIFY| PB_STYLE_OUTLINE, ClrLightSeaGreen , ClrDarkGreen, 0, ClrWhite,
                      &g_sFontCm18b, "Fuel Pressure", NULL, NULL, 0, 0,
                      getOBDDataMode1);

RectangularButton(FuelPressureCMButton, NULL, NULL, NULL, &g_sKentec320x240x16_SSD2119, 85, 120,
                      150, 25, PB_STYLE_FILL | PB_STYLE_TEXT | PB_STYLE_RELEASE_NOTIFY| PB_STYLE_OUTLINE, ClrLightSeaGreen , ClrDarkGreen, 0, ClrWhite,
                      &g_sFontCm16b, "Fuel Pressure (CM)", NULL, NULL, 0, 0,
                      getCMOBDDataMode1);

RectangularButton(MAFButton, NULL, NULL, NULL, &g_sKentec320x240x16_SSD2119, 85, 160,
                      150, 25, PB_STYLE_FILL | PB_STYLE_TEXT | PB_STYLE_RELEASE_NOTIFY| PB_STYLE_OUTLINE, ClrLightSeaGreen , ClrDarkGreen, 0, ClrWhite,
                      &g_sFontCm18b, "Mass Air Flow", NULL, NULL, 0, 0,
                      getOBDDataMode1);

RectangularButton(MAFCMButton, NULL, NULL, NULL, &g_sKentec320x240x16_SSD2119, 85, 120,
                      150, 25, PB_STYLE_FILL | PB_STYLE_TEXT | PB_STYLE_RELEASE_NOTIFY| PB_STYLE_OUTLINE, ClrLightSeaGreen , ClrDarkGreen, 0, ClrWhite,
                      &g_sFontCm16b, "Mass Air Flow (CM)", NULL, NULL, 0, 0,
                      getCMOBDDataMode1);

RectangularButton(ThrottlePositionButton, NULL, NULL, NULL, &g_sKentec320x240x16_SSD2119, 85, 160,
                      150, 25, PB_STYLE_FILL | PB_STYLE_TEXT | PB_STYLE_RELEASE_NOTIFY| PB_STYLE_OUTLINE, ClrLightSeaGreen , ClrDarkGreen, 0, ClrWhite,
                      &g_sFontCm18b, "Throttle Position", NULL, NULL, 0, 0,
                      getOBDDataMode1);

RectangularButton(ThrottlePositionCMButton, NULL, NULL, NULL, &g_sKentec320x240x16_SSD2119, 85, 120,
                      150, 25, PB_STYLE_FILL | PB_STYLE_TEXT | PB_STYLE_RELEASE_NOTIFY| PB_STYLE_OUTLINE, ClrLightSeaGreen , ClrDarkGreen, 0, ClrWhite,
                      &g_sFontCm16b, "Throttle Position (CM)", NULL, NULL, 0, 0,
                      getCMOBDDataMode1);

RectangularButton(CheckDTCButton, NULL, NULL, NULL, &g_sKentec320x240x16_SSD2119, 85, 160,
                      150, 25, PB_STYLE_FILL | PB_STYLE_TEXT | PB_STYLE_RELEASE_NOTIFY| PB_STYLE_OUTLINE, ClrLightSeaGreen , ClrDarkGreen, 0, ClrWhite,
                      &g_sFontCm18b, "Check DTC", NULL, NULL, 0, 0,
                      checkDTCHandler);

RectangularButton(ClearDTCButton, NULL, NULL, NULL, &g_sKentec320x240x16_SSD2119, 85, 160,
                      150, 25, PB_STYLE_FILL | PB_STYLE_TEXT | PB_STYLE_RELEASE_NOTIFY| PB_STYLE_OUTLINE, ClrLightSeaGreen , ClrDarkGreen, 0, ClrWhite,
                      &g_sFontCm18b, "Clear DTC", NULL, NULL, 0, 0,
                      clearDTCHandler);

RectangularButton(PrevButton, NULL, NULL, NULL, &g_sKentec320x240x16_SSD2119, 10, 190,
                      80, 25, PB_STYLE_FILL | PB_STYLE_TEXT | PB_STYLE_RELEASE_NOTIFY| PB_STYLE_OUTLINE, ClrPurple , ClrDarkGreen, 0, ClrWhite,
                      &g_sFontCm18b, "Prev", NULL, NULL, 0, 0,
                      prevButtonHandler);

RectangularButton(NextButton, NULL, NULL, NULL, &g_sKentec320x240x16_SSD2119, 230, 190,
                      80, 25, PB_STYLE_FILL | PB_STYLE_TEXT | PB_STYLE_RELEASE_NOTIFY| PB_STYLE_OUTLINE, ClrPurple , ClrDarkGreen, 0, ClrWhite,
                      &g_sFontCm18b, "Next", NULL, NULL, 0, 0,
                      nextButtonHandler);

RectangularButton(HomeButton, NULL, NULL, NULL, &g_sKentec320x240x16_SSD2119, 260, 10,
                      40, 25, PB_STYLE_FILL | PB_STYLE_IMG | PB_STYLE_RELEASE_NOTIFY| PB_STYLE_OUTLINE, ClrRed , ClrDarkGreen, 0, ClrWhite,
                      &g_sFontCm18b, "", HomeIcon, NULL, 0, 0,
                      HomeHandler);

void LCD_Init(void){
    uint32_t ui32SysClock;

    ui32SysClock = SysCtlClockGet();

    Kentec320x240x16_SSD2119Init(ui32SysClock);

    GrContextInit(&sContext, &g_sKentec320x240x16_SSD2119);
    TouchScreenInit(ui32SysClock);
    TouchScreenCallbackSet(WidgetPointerMessage);

}

void LCD_WriteData(uint16_t received_data){
    char buffer[6];
    char displayBuffer[12];
    itoa(received_data,buffer);
    strcpy(displayBuffer,buffer);
    switch(DISPLAY_STATE){
    case ENGINE_RPM:
        strcat(displayBuffer," RPM");
        break;
    case VEHICLE_SPEED:
        strcat(displayBuffer," km/h");
        break;
    case FUEL_PRESSURE:
        strcat(displayBuffer," kPa");
        break;
    case MAF:
        strcat(displayBuffer," gm/s");
        break;
    case THROTTLE_POS:
        strcat(displayBuffer," %");
        break;
    }
    GrContextBackgroundSet(&sContext,ClrWhite);
    GrContextForegroundSet(&sContext, ClrRed);
    GrContextFontSet(&sContext, g_psFontCm24b);
    GrStringDrawCentered(&sContext, displayBuffer, -1, 160, 100, true);
}

void LCD_WriteNoDTC(void){
    GrContextBackgroundSet(&sContext,ClrWhite);
    GrContextForegroundSet(&sContext, ClrRed);
    GrContextFontSet(&sContext, g_psFontCm24b);
    GrStringDrawCentered(&sContext, "No DTCs", -1, 160, 100, true);
}

void LCD_WriteDTCs(char* displayBuffer,int idx){
    GrContextBackgroundSet(&sContext,ClrWhite);
    GrContextForegroundSet(&sContext, ClrRed);
    GrContextFontSet(&sContext, g_psFontCm24b);
    GrStringDrawCentered(&sContext, displayBuffer, -1, 160, 60+30*idx, true);
}

void LCD_WriteClearDTCs(void){
    GrContextBackgroundSet(&sContext,ClrWhite);
    GrContextForegroundSet(&sContext, ClrRed);
    GrContextFontSet(&sContext, g_psFontCm24b);
    GrStringDrawCentered(&sContext, "Cleared DTCs", -1, 160, 100, true);
}

void ClrData(void){
    GrContextForegroundSet(&sContext, ClrWhite);
    GrRectFill(&sContext, &(tRectangle){90, 80, 230, 115});
}

void ClrDTC(void){
    GrContextForegroundSet(&sContext, ClrWhite);
    GrRectFill(&sContext, &(tRectangle){70, 50, 240, 130});
}

void Display_HomePage(void){
    SetBackground();
    SetTitle();
    AddHomeWidgets();
}

void RemoveHomePage(void){
    WidgetRemove((tWidget*)&HomeEngineRPMButton);
    GrContextForegroundSet(&sContext, ClrWhite);
    GrRectFill(&sContext, &(tRectangle){85, 40, 235, 65});
    WidgetRemove((tWidget *) &HomeVehicleSpeedButton);
    GrContextForegroundSet(&sContext, ClrWhite);
    GrRectFill(&sContext, &(tRectangle){85, 80, 235, 105});
    WidgetRemove((tWidget *) &HomeFuelPressureButton);
    GrContextForegroundSet(&sContext, ClrWhite);
    GrRectFill(&sContext, &(tRectangle){85, 120, 235, 145});
    WidgetRemove((tWidget *) &HomeMAFButton);
    GrContextForegroundSet(&sContext, ClrWhite);
    GrRectFill(&sContext, &(tRectangle){85, 160, 235, 185});
    WidgetRemove((tWidget *) &HomeThrottleButton);
    GrContextForegroundSet(&sContext, ClrWhite);
    GrRectFill(&sContext, &(tRectangle){85, 200, 235, 225});
    WidgetRemove((tWidget *) &MoreButton);
    GrContextForegroundSet(&sContext, ClrWhite);
    GrRectFill(&sContext, &(tRectangle){250, 190, 310, 215});
    GrContextForegroundSet(&sContext, ClrWhite);
    GrRectFill(&sContext, &(tRectangle){90, 80, 230, 115});
    WidgetPaint(WIDGET_ROOT);
}

void DisplayBackPage(void){
    WidgetAdd(WIDGET_ROOT,(tWidget*)&HomeCheckDTCButton);
    WidgetAdd(WIDGET_ROOT,(tWidget*)&HomeClearDTCButton);
    WidgetAdd(WIDGET_ROOT,(tWidget*)&BackButton);
    WidgetPaint(WIDGET_ROOT);
}

void RemoveBackPage(void){
    WidgetRemove((tWidget*)&HomeCheckDTCButton);
    GrContextForegroundSet(&sContext, ClrWhite);
    GrRectFill(&sContext, &(tRectangle){85, 80, 235, 105});
    WidgetRemove((tWidget*)&HomeClearDTCButton);
    GrContextForegroundSet(&sContext, ClrWhite);
    GrRectFill(&sContext, &(tRectangle){85, 120, 235, 145});
    WidgetRemove((tWidget*)&BackButton);
    GrContextForegroundSet(&sContext, ClrWhite);
    GrRectFill(&sContext, &(tRectangle){10, 190, 70, 215});
}

void SetBackground(void){
    GrContextForegroundSet(&sContext,ClrWhite);
    GrRectFill(&sContext, &(tRectangle){0, 0, 319, 239});
}

void SetTitle(void){
    GrContextForegroundSet(&sContext, ClrBlue);
    GrContextFontSet(&sContext, g_psFontCm24b);
    GrStringDrawCentered(&sContext, "OBD Scanner", -1, 160, 20, false);
}

void AddHomeWidgets(void){
    WidgetAdd(WIDGET_ROOT,(tWidget*)&HomeEngineRPMButton);
    WidgetAdd(WIDGET_ROOT,(tWidget*)&HomeVehicleSpeedButton);
    WidgetAdd(WIDGET_ROOT,(tWidget*)&HomeFuelPressureButton);
    WidgetAdd(WIDGET_ROOT,(tWidget*)&HomeMAFButton);
    WidgetAdd(WIDGET_ROOT,(tWidget*)&HomeThrottleButton);
    WidgetAdd(WIDGET_ROOT,(tWidget*)&MoreButton);

    WidgetPaint(WIDGET_ROOT);
}

void AddEngineRPMButtons(void){
    WidgetAdd(WIDGET_ROOT,(tWidget *) &EngineRPMButton);
    WidgetAdd(WIDGET_ROOT,(tWidget *) &EngineRPMCMButton);
    PushButtonTextSet(&EngineRPMCMButton,"Engine RPM (CM)");
    PushButtonFillColorSet(&EngineRPMCMButton,ClrLightSeaGreen);
    WidgetPaint(WIDGET_ROOT);
}

void RemoveEngineRPMButtons(void){
    WidgetRemove((tWidget *)&EngineRPMButton);
    WidgetRemove((tWidget *)&EngineRPMCMButton);
    GrContextForegroundSet(&sContext, ClrWhite);
    GrRectFill(&sContext, &(tRectangle){85, 120, 235, 145});
    GrContextForegroundSet(&sContext, ClrWhite);
    GrRectFill(&sContext, &(tRectangle){85, 160, 235, 185});
    WidgetPaint(WIDGET_ROOT);
}

void AddVehicleSpeedButtons(void){
    WidgetAdd(WIDGET_ROOT,(tWidget *) &VehicleSpeedButton);
    WidgetAdd(WIDGET_ROOT,(tWidget *) &VehicleSpeedCMButton);
    PushButtonTextSet(&VehicleSpeedCMButton,"Vehicle Speed (CM)");
    PushButtonFillColorSet(&VehicleSpeedCMButton,ClrLightSeaGreen);
    WidgetPaint(WIDGET_ROOT);
}

void RemoveVehicleSpeedButtons(void){
    WidgetRemove((tWidget *)&VehicleSpeedButton);
    WidgetRemove((tWidget *)&VehicleSpeedCMButton);
    GrContextForegroundSet(&sContext, ClrWhite);
    GrRectFill(&sContext, &(tRectangle){85, 120, 235, 145});
    GrContextForegroundSet(&sContext, ClrWhite);
    GrRectFill(&sContext, &(tRectangle){85, 160, 235, 185});
    WidgetPaint(WIDGET_ROOT);
}

void AddFuelPressureButtons(void){
    WidgetAdd(WIDGET_ROOT,(tWidget *) &FuelPressureButton);
    WidgetAdd(WIDGET_ROOT,(tWidget *) &FuelPressureCMButton);
    PushButtonTextSet(&FuelPressureCMButton,"Fuel Pressure (CM)");
    PushButtonFillColorSet(&FuelPressureCMButton,ClrLightSeaGreen);
    WidgetPaint(WIDGET_ROOT);
}

void RemoveFuelPressureButtons(void){
    WidgetRemove((tWidget *)&FuelPressureButton);
    WidgetRemove((tWidget *)&FuelPressureCMButton);
    GrContextForegroundSet(&sContext, ClrWhite);
    GrRectFill(&sContext, &(tRectangle){85, 120, 235, 145});
    GrContextForegroundSet(&sContext, ClrWhite);
    GrRectFill(&sContext, &(tRectangle){85, 160, 235, 185});
    WidgetPaint(WIDGET_ROOT);
}

void AddMAFButtons(void){
    WidgetAdd(WIDGET_ROOT,(tWidget *) &MAFButton);
    WidgetAdd(WIDGET_ROOT,(tWidget *) &MAFCMButton);
    PushButtonTextSet(&MAFCMButton,"Mass Air Flow (CM)");
    PushButtonFillColorSet(&MAFCMButton,ClrLightSeaGreen);
    WidgetPaint(WIDGET_ROOT);
}

void RemoveMAFButtons(void){
    WidgetRemove((tWidget *)&MAFButton);
    WidgetRemove((tWidget *)&MAFCMButton);
    GrContextForegroundSet(&sContext, ClrWhite);
    GrRectFill(&sContext, &(tRectangle){85, 120, 235, 145});
    GrContextForegroundSet(&sContext, ClrWhite);
    GrRectFill(&sContext, &(tRectangle){85, 160, 235, 185});
    WidgetPaint(WIDGET_ROOT);
}

void AddThrottlePosButtons(void){
    WidgetAdd(WIDGET_ROOT,(tWidget *) &ThrottlePositionButton);
    WidgetAdd(WIDGET_ROOT,(tWidget *) &ThrottlePositionCMButton);
    PushButtonTextSet(&ThrottlePositionCMButton,"Throttle Position (CM)");
    PushButtonFillColorSet(&ThrottlePositionCMButton,ClrLightSeaGreen);
    WidgetPaint(WIDGET_ROOT);
}

void RemoveThrottlePosButtons(void){
    WidgetRemove((tWidget *)&ThrottlePositionButton);
    WidgetRemove((tWidget *)&ThrottlePositionCMButton);
    GrContextForegroundSet(&sContext, ClrWhite);
    GrRectFill(&sContext, &(tRectangle){85, 120, 235, 145});
    GrContextForegroundSet(&sContext, ClrWhite);
    GrRectFill(&sContext, &(tRectangle){85, 160, 235, 185});
    WidgetPaint(WIDGET_ROOT);
}

void AddCheckDTCButton(void){
    WidgetAdd(WIDGET_ROOT,(tWidget*)&CheckDTCButton);
    WidgetPaint(WIDGET_ROOT);
}

void RemoveCheckDTCButton(void){
    WidgetRemove((tWidget *)&CheckDTCButton);
    GrContextForegroundSet(&sContext, ClrWhite);
    GrRectFill(&sContext, &(tRectangle){85, 160, 235, 185});
    WidgetPaint(WIDGET_ROOT);
}

void AddClearDTCButton(void){
    WidgetAdd(WIDGET_ROOT,(tWidget*)&ClearDTCButton);
    WidgetPaint(WIDGET_ROOT);
}

void RemoveClearDTCButton(void){
    WidgetRemove((tWidget *)&ClearDTCButton);
    GrContextForegroundSet(&sContext, ClrWhite);
    GrRectFill(&sContext, &(tRectangle){85, 160, 235, 185});
    WidgetPaint(WIDGET_ROOT);
}

void AddSurroundButtons(void){
    WidgetAdd(WIDGET_ROOT, (tWidget *) &NextButton);
    WidgetAdd(WIDGET_ROOT, (tWidget *) &PrevButton);
    WidgetAdd(WIDGET_ROOT, (tWidget *) &HomeButton);
    WidgetPaint(WIDGET_ROOT);
}

void RemoveSurroundButtons(void){
    WidgetRemove((tWidget *)&PrevButton);
    GrContextForegroundSet(&sContext, ClrWhite);
    GrRectFill(&sContext, &(tRectangle){10, 190, 90, 215});
    WidgetRemove((tWidget *)&NextButton);
    GrContextForegroundSet(&sContext, ClrWhite);
    GrRectFill(&sContext, &(tRectangle){230, 190, 310, 215});
    WidgetRemove((tWidget *)&HomeButton);
    GrContextForegroundSet(&sContext, ClrWhite);
    GrRectFill(&sContext, &(tRectangle){240, 10, 320, 45});
    WidgetPaint(WIDGET_ROOT);
}

void getOBDDataMode1(tWidget* psWidget){
    ClrData();
    CAN_Request(DISPLAY_STATE);
}

void getCMOBDDataMode1(tWidget* psWidget){
    ClrData();
    switch(DISPLAY_STATE){
    case ENGINE_RPM:
        engineRPMCMActive = !engineRPMCMActive;
        if(engineRPMCMActive==true){
            PushButtonTextSet(&EngineRPMCMButton,"Testing...");
            PushButtonFillColorSet(&EngineRPMCMButton,ClrRed);
            WidgetPaint(WIDGET_ROOT);
        }
        else{
            PushButtonTextSet(&EngineRPMCMButton,"Engine RPM (CM)");
            PushButtonFillColorSet(&EngineRPMCMButton,ClrLightSeaGreen);
            WidgetPaint(WIDGET_ROOT);
        }
        break;
    case VEHICLE_SPEED:
        vehicleSpeedCMActive = !vehicleSpeedCMActive;
        if(vehicleSpeedCMActive==true){
            PushButtonTextSet(&VehicleSpeedCMButton,"Testing...");
            PushButtonFillColorSet(&VehicleSpeedCMButton,ClrRed);
            WidgetPaint(WIDGET_ROOT);
        }
        else{
            PushButtonTextSet(&VehicleSpeedCMButton,"Vehicle Speed (CM)");
            PushButtonFillColorSet(&VehicleSpeedCMButton,ClrLightSeaGreen);
            WidgetPaint(WIDGET_ROOT);
        }
        break;
    case FUEL_PRESSURE:
        fuelPressureCMActive = !fuelPressureCMActive;
        if(fuelPressureCMActive==true){
            PushButtonTextSet(&FuelPressureCMButton,"Testing...");
            PushButtonFillColorSet(&FuelPressureCMButton,ClrRed);
            WidgetPaint(WIDGET_ROOT);
        }
        else{
            PushButtonTextSet(&FuelPressureCMButton,"Fuel Pressure (CM)");
            PushButtonFillColorSet(&FuelPressureCMButton,ClrLightSeaGreen);
            WidgetPaint(WIDGET_ROOT);
        }
        break;
    case MAF:
        mafCMActive = !mafCMActive;
        if(mafCMActive==true){
            PushButtonTextSet(&MAFCMButton,"Testing...");
            PushButtonFillColorSet(&MAFCMButton,ClrRed);
            WidgetPaint(WIDGET_ROOT);
        }
        else{
            PushButtonTextSet(&MAFCMButton,"Mass Air Flow (CM)");
            PushButtonFillColorSet(&MAFCMButton,ClrLightSeaGreen);
            WidgetPaint(WIDGET_ROOT);
        }
        break;
    case THROTTLE_POS:
        throttleCMActive = !throttleCMActive;
        if(throttleCMActive==true){
            PushButtonTextSet(&ThrottlePositionCMButton,"Testing...");
            PushButtonFillColorSet(&ThrottlePositionCMButton,ClrRed);
            WidgetPaint(WIDGET_ROOT);
        }
        else{
            PushButtonTextSet(&ThrottlePositionCMButton,"Throttle Position (CM)");
            PushButtonFillColorSet(&ThrottlePositionCMButton,ClrLightSeaGreen);
            WidgetPaint(WIDGET_ROOT);
        }
        break;
    default:break;
    }
}

void checkDTCHandler(tWidget* psWidget){
    ClrDTC();
    CAN_Request(DISPLAY_STATE);
}

void clearDTCHandler(tWidget* psWidget){
    ClrDTC();
    CAN_Request(DISPLAY_STATE);
}

void prevButtonHandler(tWidget* psWidget){
    if(DISPLAY_STATE==CHECK_DTC||DISPLAY_STATE==CLEAR_DTC){
        ClrDTC();
    }
    else{
        ClrData();
    }
    switch(DISPLAY_STATE){
    case ENGINE_RPM:
        RemoveEngineRPMButtons();
        engineRPMCMActive = false;

        AddClearDTCButton();
        DISPLAY_STATE = CLEAR_DTC;
        break;
    case VEHICLE_SPEED:
        RemoveVehicleSpeedButtons();
        vehicleSpeedCMActive = false;

        AddEngineRPMButtons();
        DISPLAY_STATE = ENGINE_RPM;
        break;
    case FUEL_PRESSURE:
        RemoveFuelPressureButtons();
        fuelPressureCMActive = false;

        AddVehicleSpeedButtons();

        DISPLAY_STATE = VEHICLE_SPEED;
        break;
    case MAF:
        RemoveMAFButtons();
        mafCMActive = false;

        AddFuelPressureButtons();

        DISPLAY_STATE = FUEL_PRESSURE;
        break;
    case THROTTLE_POS:

        RemoveThrottlePosButtons();
        throttleCMActive = false;

        AddMAFButtons();

        DISPLAY_STATE = MAF;
        break;
    case CHECK_DTC:
        RemoveCheckDTCButton();
        AddThrottlePosButtons();

        DISPLAY_STATE = THROTTLE_POS;
        break;
    case CLEAR_DTC:
        RemoveClearDTCButton();
        AddCheckDTCButton();
        DISPLAY_STATE = CHECK_DTC;
        break;
    default:break;
    }
}

void nextButtonHandler(tWidget* psWidget){
    if(DISPLAY_STATE==CHECK_DTC||DISPLAY_STATE==CLEAR_DTC){
        ClrDTC();
    }
    else{
        ClrData();
    }
    switch(DISPLAY_STATE){
    case ENGINE_RPM:
        RemoveEngineRPMButtons();
        engineRPMCMActive = false;

        AddVehicleSpeedButtons();

        DISPLAY_STATE = VEHICLE_SPEED;
        break;
    case VEHICLE_SPEED:
        RemoveVehicleSpeedButtons();
        vehicleSpeedCMActive = false;


        AddFuelPressureButtons();

        DISPLAY_STATE = FUEL_PRESSURE;
        break;
    case FUEL_PRESSURE:
        RemoveFuelPressureButtons();
        fuelPressureCMActive = false;

        AddMAFButtons();


        DISPLAY_STATE = MAF;
        break;
    case MAF:
        RemoveMAFButtons();
        mafCMActive = false;


        AddThrottlePosButtons();

        DISPLAY_STATE = THROTTLE_POS;
        break;
    case THROTTLE_POS:
        RemoveThrottlePosButtons();
        throttleCMActive = false;

        AddCheckDTCButton();
        DISPLAY_STATE = CHECK_DTC;
        break;
    case CHECK_DTC:
        RemoveCheckDTCButton();

        AddClearDTCButton();
        DISPLAY_STATE = CLEAR_DTC;
        break;
    case CLEAR_DTC:
        RemoveClearDTCButton();
        AddEngineRPMButtons();
        DISPLAY_STATE = ENGINE_RPM;
        break;
    default:break;
    }
}

void HomeHandler(tWidget* psWidget){
    if(DISPLAY_STATE==CHECK_DTC||DISPLAY_STATE==CLEAR_DTC){
        ClrDTC();
    }
    else{
        ClrData();
    }
    switch(DISPLAY_STATE){
    case ENGINE_RPM:
        RemoveEngineRPMButtons();
        engineRPMCMActive = false;
        break;
    case VEHICLE_SPEED:
        RemoveVehicleSpeedButtons();
        vehicleSpeedCMActive = false;
        break;
    case FUEL_PRESSURE:
        RemoveFuelPressureButtons();
        fuelPressureCMActive = false;
        break;
    case MAF:
        RemoveMAFButtons();
        mafCMActive = false;
        break;
    case THROTTLE_POS:
        RemoveThrottlePosButtons();
        throttleCMActive = false;
        break;
    case CHECK_DTC:
        RemoveCheckDTCButton();
    case CLEAR_DTC:
        RemoveClearDTCButton();
    default:break;
    }
    RemoveSurroundButtons();
    AddHomeWidgets();
}

void backButtonHandler(tWidget* psWidget){
    RemoveBackPage();
    AddHomeWidgets();
}

void HomeCheckDTCButtonHandler(tWidget* psWidget){
    RemoveBackPage();
    DISPLAY_STATE = CHECK_DTC;
    AddCheckDTCButton();
    AddSurroundButtons();
}

void HomeClearDTCButtonHandler(tWidget* psWidget){
    RemoveBackPage();
    DISPLAY_STATE = CLEAR_DTC;
    AddClearDTCButton();
    AddSurroundButtons();
}

void HomeEngineRPMButtonHandler(tWidget* psWidget){
    RemoveHomePage();
    DISPLAY_STATE = ENGINE_RPM;
    engineRPMCMActive = false;
    AddEngineRPMButtons();
    AddSurroundButtons();
}

void HomeVehicleSpeedButtonHandler(tWidget* psWidget){
    RemoveHomePage();
    DISPLAY_STATE = VEHICLE_SPEED;
    vehicleSpeedCMActive = false;
    AddVehicleSpeedButtons();
    AddSurroundButtons();
}

void HomeFuelPressureButtonHandler(tWidget* psWidget){
    RemoveHomePage();
    DISPLAY_STATE = FUEL_PRESSURE;
    fuelPressureCMActive = false;
    AddFuelPressureButtons();
    AddSurroundButtons();
}

void HomeMAFButtonHandler(tWidget* psWidget){
    RemoveHomePage();
    DISPLAY_STATE = MAF;
    mafCMActive = false;
    AddMAFButtons();
    AddSurroundButtons();
}

void HomeThrottleButtonHandler(tWidget* psWidget){
    RemoveHomePage();
    DISPLAY_STATE = THROTTLE_POS;
    throttleCMActive = false;
    AddThrottlePosButtons();
    AddSurroundButtons();
}

void moreButtonHandler(tWidget* psWidget){
    RemoveHomePage();
    DisplayBackPage();
}

void itoa(uint16_t value, char* buffer) {
    uint8_t i = 0, j = 0;
    uint16_t tmp;


    if (value == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }

    for (tmp = value; tmp > 0; tmp /= 10) {
        buffer[i++] = '0' + (tmp % 10);
    }


    buffer[i] = '\0';

    uint8_t len = i;
    for (j = 0; j < len / 2; j++) {
        char temp = buffer[j];
        buffer[j] = buffer[len - 1 - j];
        buffer[len - 1 - j] = temp;
    }
}


