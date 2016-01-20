/*************************************************************************
 *      Functions for NILM project GUI
 *   
 **************************************************************************/
#include "includes.h"


#ifndef __GUI_H
#define __GUI_H

// Functions prototypes
void gui_toggleMode(pAlgoDevice_t pDevices,int devLamps[]);
void gui_clearScreen(void);
void gui_button(int xLeft, int yUp, int xRight, int yDown, pInt8U text, int textPosX, int textPosY, LdcPixel_t txtColor, LdcPixel_t backgroundColor);
void gui_monitoringScreen(pAlgoDevice_t pDevices,int devLamps[]); // GUI monitoring screen
void gui_learningScreen(void); // GUI learning screen
void gui_addDeviceScreen(void); // GUI add device sceen
void gui_settingsScreen(void); // GUI Settings screen
void gui_deleteDevicesScreen(void); // Delete Devices screen

// Application variables
extern char headLine[20];
extern bool mode; // 0 = Monitoring mode, 1 = Learning mode
extern int devicesConnected; // amount of devices registered
extern bool hasMeanValue; // is the mean value measured
extern bool modeButtonState; // enable / disable mode button
extern bool settingsButtonState; // enable / disable settings button
extern bool backButtonState; // enable / disable back button
extern bool addDeviceButtonState; // enable / disable add device button
extern int screen; // int storing current screen
   
extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;

#endif // __GUI_H