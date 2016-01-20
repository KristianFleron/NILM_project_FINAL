#include "includes.h"
#include "gui.h"

// Application variables
char headLine[20] = "Monitoring";
bool mode = 0; // 0 = Monitoring mode, 1 = Learning mode
int devicesConnected = 1; // amount of devices registered
bool hasMeanValue = 0; // is the mean value measured
bool modeButtonState = 1; // enable / disable mode button
bool settingsButtonState = 1; // enable / disable settings button
bool backButtonState = 0; // enable / disable back button
bool addDeviceButtonState = 1; // enable / disable back button
int screen = 0;
 //

// Get mode
bool gui_getMode(){return mode;}
/*************************************************************************
 * Function Name: gui_toggleMode
 * Parameters: none
 *
 * Return: none
 *
 * Description: Toggles between learning and monitoring mode
 *
 *************************************************************************/
void gui_toggleMode(pAlgoDevice_t pDevices,int devLamps[])
{
  mode=!mode;
  if(mode)
  {
    gui_learningScreen();
  }
  else
  {
    gui_monitoringScreen(pDevices,devLamps);
  }
}
/*************************************************************************
 * Function Name: gui_clearScreen
 * Parameters: none
 *
 * Return: none
 *
 * Description: clears the screen to all white
 *
 *************************************************************************/
void gui_clearScreen(void)
{
    // Clear background
    GLCD_SetFont(&Terminal_18_24_12,0x000000,0xffffffff);
    GLCD_SetWindow(0,0,319,239);
    GLCD_TextSetPos(0,0);
    GLCD_print("\f%s"," ");
}
/*************************************************************************
 * Function Name: gui_button
 * Parameters: xLeft, yUp, xRight, yDown, text, textPosX, textPosY, txtColor, backgroundColor
 *
 * Return: none
 *
 * Description: place button
 *
 *************************************************************************/
void gui_button(int xLeft, int yUp, int xRight, int yDown, pInt8U text, int textPosX, int textPosY, LdcPixel_t txtColor, LdcPixel_t backgroundColor)
{
    // Add Button
    GLCD_SetFont(&Terminal_18_24_12,txtColor,backgroundColor);
    GLCD_SetWindow(xLeft,yUp,xRight, yDown);
    GLCD_print("\f%s", " ");
    GLCD_SetWindow(xLeft+textPosX,yUp+textPosY,xRight,yDown);
    GLCD_TextSetPos(0,0);
    GLCD_print("\f%s", text);
}
/*************************************************************************
 * Function Name: gui_monitoringScreen
 * Parameters: None
 *
 * Return: none
 *
 * Description: GUI home screen (Monitoring mode)
 *
 *************************************************************************/
void gui_monitoringScreen(pAlgoDevice_t pDevices,int devLamps[])
{
  // Menu options
  settingsButtonState = 1; // Enable Settings
  modeButtonState = 1; // Enable Mode
  screen = 0; // Tracking screen, 0 = monitoring screen
  
  // Clear screen
  gui_clearScreen();
  
  // Entering MONITORING MODE
  strcpy(headLine, "Monitoring");
  
  // Set headline
  GLCD_SetFont(&Terminal_18_24_12,0x000000,0xffffffff);
  GLCD_SetWindow(95,0,225,23);
  GLCD_TextSetPos(0,0);
  GLCD_print("\f%s", headLine);
  
  // Mode button
  gui_button(0,190,80,239,"MODE",10,13,0x000000,0xff99ffff);
  
  // settings button
  // gui_button(239,190,319,239,"Set",10,13,0x000000,0xff99ffff);
  GLCD_LoadPic(259, 179, &button_settingsPic, 1); 
  
  // device img 1
  (devLamps[0]==1)? GLCD_LoadPic(28, 28, &lamp_onPic, 1):
        GLCD_LoadPic(28, 28, &lamp_offPic, 1);  
  GLCD_SetWindow(5,95,101,187);
  GLCD_TextSetPos(0,0);
  GLCD_print("\fDev 1\n\rP: %f\n\rQ: %f\n\rH: 46W",(*pDevices).dP, 
             (*pDevices).dQ);

  // device img 2  
  (devLamps[1]==1)? GLCD_LoadPic(134, 28, &lamp_onPic, 1):
     GLCD_LoadPic(134, 28, &lamp_offPic, 1);
  GLCD_SetWindow(111,95,207,187);
  GLCD_TextSetPos(0,0);
  GLCD_print("\fDev 2\n\rP: %f\n\rQ: %f\n\rH: 46W",(*(pDevices+1)).dP,
             (*(pDevices+1)).dQ);

  // device img 3
  (devLamps[2]==1)?  GLCD_LoadPic(240, 28, &lamp_onPic, 1):
     GLCD_LoadPic(240, 28, &lamp_offPic, 1);
  
 
 
  
  GLCD_SetWindow(219,95,313,187);
  GLCD_TextSetPos(0,0);

//    GLCD_print("\fDev 3\n\rP: %f\n\rQ: %f\n\rH: 46W",(*(pDevices+2)).dP,
//             (*(pDevices+2)).dQ);
  
}


/*************************************************************************
 * Function Name: gui_learningScreen
 * Parameters: None
 *
 * Return: none
 *
 * Description: GUI learning screen (Learning mode)
 *
 *************************************************************************/
void gui_learningScreen(void)
{
  // Menu options
  settingsButtonState = 1;
  modeButtonState = 1;
  screen = 1; // Tracking screen, 1 = learning screen
  
  // Clear screen
  gui_clearScreen();
  
  // Entering LEARNING MODE
  strcpy(headLine, "Learning Mode");
  
  // Set headline
  GLCD_SetFont(&Terminal_18_24_12,0x000000,0xffffffff);
  GLCD_SetWindow(85,0,235,23);
  GLCD_TextSetPos(0,0);
  GLCD_print("\f%s", headLine);
  
  if(modeButtonState)
  {
    // Mode button
    gui_button(0,190,80,239,"MODE",10,13,0x000000,0xff99ffff);
  }
  
  if(settingsButtonState)
  {
    // settings button
    gui_button(239,190,319,239,"Set",10,13,0x000000,0xff99ffff);
  }
  
  // Add Device
  gui_button(80,70,240,120,"Add Device",23,13,0x000000,0x0066ff66);
  
  if(devicesConnected)
  {
    // Delete All Devices
    gui_button(80,130,240,180,"Delete All",23,13,0x000000,0x00ff9966);
  }
  
  
  gui_button(0,0,319,50,dataArray,0,0,0x000000,0x00ff9966);
}
/*************************************************************************
 * Function Name: gui_addDeviceScreen
 * Parameters: None
 *
 * Return: none
 *
 * Description: GUI add device (Learning mode)
 *
 *************************************************************************/
void gui_addDeviceScreen(void)
{
  // Menu options
  settingsButtonState = 1;
  modeButtonState = 1;
  backButtonState = 1;
  screen = 2; // Tracking screen, 2 = Add Device screen
  
  // Clear screen
  gui_clearScreen();
  
  // Entering LEARNING MODE
  strcpy(headLine, "Learning Mode");
  
  // Set headline
  GLCD_SetFont(&Terminal_18_24_12,0x000000,0xffffffff);
  GLCD_SetWindow(85,0,235,23);
  GLCD_TextSetPos(0,0);
  GLCD_print("\f%s", headLine);
  
  // Display message
  GLCD_SetFont(&Terminal_18_24_12,0x000000,0xffffffff);
  GLCD_SetWindow(50,30,270,99);
  GLCD_TextSetPos(0,0);
  GLCD_print("\f%s", "Measuring Mean\n\r  Please Wait!");  
  
  if(modeButtonState)
  {
    // Mode button
    gui_button(0,190,80,239,"MODE",10,13,0x000000,0xff99ffff);
  }
  
  if(settingsButtonState)
  {
    // settings button
    gui_button(239,190,319,239,"Set",10,13,0x000000,0xff99ffff);
  }
}
/*************************************************************************
 * Function Name: gui_settingsScreen
 * Parameters: None
 *
 * Return: none
 *
 * Description: GUI settings screen
 *
 *************************************************************************/
void gui_settingsScreen(void)
{
  // Menu options
  settingsButtonState = 0;
  modeButtonState = 0;
  backButtonState = 1;
  screen = 3; // Tracking screen, 3 = settings screen
  
  // Display message
  GLCD_SetFont(&Terminal_18_24_12,0x000000,0xffffffff);
  GLCD_SetWindow(50,30,270,99);
  GLCD_TextSetPos(0,0);
  GLCD_print("\f%s", "Here is going\n\rto be settings...");  
  
  // Back button
  gui_button(0,190,80,239,"Back",10,13,0x000000,0xff99ffff);
  
}
/*************************************************************************
 * Function Name: gui_deleteDevicesScreen
 * Parameters: None
 *
 * Return: none
 *
 * Description: GUI settings screen
 *
 *************************************************************************/
void gui_deleteDevicesScreen(void)
{
  // Menu options
  settingsButtonState = 0;
  modeButtonState = 0;
  backButtonState = 1;
  screen = 3; // Tracking screen, 3 = settings screen
  
  // Display message
  GLCD_SetFont(&Terminal_18_24_12,0x000000,0xffffffff);
  GLCD_SetWindow(50,30,270,99);
  GLCD_TextSetPos(0,0);
  GLCD_print("\f%s", "Here is going\n\rto be settings...");  
  
  // Mode button
  gui_button(0,190,80,239,"Back",10,13,0x000000,0xfff99fff);
  
}