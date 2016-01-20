//change now
//yolo


/*************************************************************************
*
*    Used with ICCARM and AARM.
*
*    (c) Copyright IAR Systems 2008
*
*    File name   : main.c
*    Description : Main module
*
*    History :
*    1. Date        : August 5, 2008
*       Author      : Stanimir Bonev
*       Description : Create
*
*  This example project shows how to use the IAR Embedded Workbench for ARM
* to develop code for the IAR LPC2478-SK board.
*  It implements USB CDC (Communication Device Class) device and install
* it like a Virtual COM port. The UART0 is used for physical implementation
* of the RS232 port.
*
* Jumpers:
*  EXT/JLINK  - depending of power source
*  ISP_E      - unfilled
*  RST_E      - unfilled
*  BDS_E      - unfilled
*  C/SC       - SC
*
* Note:
*  After power-up the controller gets it's clock from internal RC oscillator that
* is unstable and may fail with J-Link auto detect, therefore adaptive clocking
* should always be used. The adaptive clock can be select from menu:
*  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.
*
* The LCD shares pins with Trace port. If ETM is enabled the LCD will not work.
*
*    $Revision: 28 $
**************************************************************************/
#include "includes.h"


#define NONPROT 0xFFFFFFFF
#define CRP1  	0x12345678
#define CRP2  	0x87654321
/*If CRP3 is selected, no future factory testing can be performed on the device*/
#define CRP3  	0x43218765

#ifndef SDRAM_DEBUG
#pragma segment=".crp"
#pragma location=".crp"
__root const unsigned crp = NONPROT;
#endif

#define LCD_VRAM_BASE_ADDR ((Int32U)&SDRAM_BASE_ADDR)

extern Int32U SDRAM_BASE_ADDR;
/*extern FontType_t Terminal_6_8_6;
extern FontType_t Terminal_9_12_6;
extern FontType_t Terminal_18_24_12;*/

#define BUF ((struct uip_eth_hdr *)&uip_buf[0])//web

/*************************************************************************
* Function Name: main
* Parameters: none
*
* Return: none
*
* Description: main
*
*************************************************************************/

void calcMeanRange(pAlgoPowers_t pLines){
    
    GLCD_print("Please don't change the load!\r\n");
    //calculating the mean
    int cnt=0;
    
    while(1){
        if(dataReady){
            //at cnt 10, check if the P and Q are negative.  i.e. if device is unplugged
            /*  if((cnt)==20){
            calcPower(dataP,dataQ,cnt,pLines);
            
            if((*pLines).P.mean<0 && (*pLines).Q.mean<0){
            GLCD_print("device is unplugged\r\n");
            cnt = 0;
            break;
            }
            
            }*/
            
            if((cnt)==10){
                calcPower(dataP, dataQ, cnt, pLines);
                // GLCD_print("\n device: %.2f %.2f\n\r",(*pLines).P.mean, (*pLines).Q.mean);
                GLCD_print("done the mean calculation!\r\n");
                cnt=0;
                break;
            }
            
            
            dataConversion(dataArray, dataP, dataQ, dataT, cnt);
            
            /*   //detect if the user has unplugged the load!!! Then
            if(dataP[cnt-1]-dataP[cnt]>5000){
            GLCD_print("Load unplugged... mean not calculated");
            //should I clear  dataP and dataQ???
            //cnt = 0;
            //break;
            }
            */
            
            
            cnt++;
            dataReady=0;
        }
    }
}



int main(void)
{
    int i=0,k=0;
    int cnt=0;
    int tempcnt=0;
    int concurCnt = 0;
    
   
    AlgoPowers_t prevPLines;
    AlgoPowers_t curPLines;
   
 
    int plugOut = 0;
    Flo64 cInterval = 5000;
    
    //web
    unsigned int e;
    uip_ipaddr_t ipaddr;
    struct timer periodic_timer, arp_timer;
    
    // int deviceCnt[3] = {0};
    //AlgoLine_t TestLine;
    // pAlgoLine_t pTestLine=&TestLine;
    AlgoDevice_t tmpDev;
//    AlgoDevice_t devProfiles[3]={0};
    AlgoDevice_t devProfiles[3];
    int devNum=0;
    
    
    bool addDevice = 1;
    
    Int32S devLamps[3];
    
    
    #if CDC_DEVICE_SUPPORT_LINE_CODING > 0
        CDC_LineCoding_t CDC_LineCoding;
    UartLineCoding_t UartLineCoding;
    #endif // CDC_DEVICE_SUPPORT_LINE_CODING > 0
        
    
    
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  GUI init  START
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // initialize touch parametres
    Int32U cursor_x = (C_GLCD_H_SIZE - CURSOR_H_SIZE)/2, cursor_y = (C_GLCD_V_SIZE - CURSOR_V_SIZE)/2;
    ToushRes_t XY_Touch;
    Boolean Touch = FALSE;
    
    
    GLCD_Ctrl (FALSE);
    
    // Init GPIO
    GpioInit();
    
    #ifndef SDRAM_DEBUG
    // MAM init
    MAMCR_bit.MODECTRL = 0;
    MAMTIM_bit.CYCLES  = 3;   // FCLK > 40 MHz
    MAMCR_bit.MODECTRL = 2;   // MAM functions fully enabled
    // Init clock
    InitClock();
    // SDRAM Init
    SDRAM_Init();
    #endif // SDRAM_DEBUG
        // Init VIC   ---interrupt
    VIC_Init();
    
    
    // GLCD init
    GLCD_Init (NULL, NULL);
    
    GLCD_Cursor_Dis(0);
    
    GLCD_Copy_Cursor ((Int32U *)Cursor, 0, sizeof(Cursor)/sizeof(Int32U));
    
    GLCD_Cursor_Cfg(CRSR_FRAME_SYNC | CRSR_PIX_32);
    
    GLCD_Move_Cursor(cursor_x, cursor_y);
    
    GLCD_Cursor_En(0);
    
    // Init touch screen
    TouchScrInit();
    
    // Touched indication LED
    USB_H_LINK_LED_SEL = 0; // GPIO
    USB_H_LINK_LED_FSET = USB_H_LINK_LED_MASK;
    USB_H_LINK_LED_FDIR |= USB_H_LINK_LED_MASK;
    
    
        
  // Initialize the ethernet device driver
  do
  {
    GLCD_TextSetPos(0,0);
  }
  while(!tapdev_init());
  GLCD_TextSetPos(0,0);

  // uIP web server
  // Initialize the uIP TCP/IP stack.
  uip_init();

  uip_ipaddr(ipaddr, 192,168,0,100);
  uip_sethostaddr(ipaddr);
  uip_ipaddr(ipaddr, 192,168,0,1);
  uip_setdraddr(ipaddr);
  uip_ipaddr(ipaddr, 255,255,255,0);
  uip_setnetmask(ipaddr);

  // Initialize the HTTP server.
  httpd_init();
    
    // Init UART 0
    UartInit(UART_0,0,NORM);
    
    
    __enable_interrupt();
    
    GLCD_Ctrl (TRUE);
    
    
    
    /* devProfiles[0].dP= 101366.22;   //blow dryer
    devProfiles[0].dQ =51765.90;
    
    devProfiles[1].dP= 35957.14;    //light bulb
    devProfiles[1].dQ = 9045.64;
    */
    
    
    //  GLCD_print("Device char %f %f\r\n",devProfiles[devNum].dP, devProfiles[devNum].dQ );
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // Update the baud rate
    UartLineCoding.dwDTERate = 115200;
    // Update the stop bits number
    UartLineCoding.bStopBitsFormat = UART_ONE_STOP_BIT;
    // Update the parity type
    UartLineCoding.bParityType = UART_NO_PARITY;
    // Update the word width
    UartLineCoding.bDataBits = (UartWordWidth_t)(3);
    //Description: Init UART Baud rate, Word width, Stop bits, Parity type
    UartSetLineCoding(UART_0,UartLineCoding);
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    
    //calculate the no load powerlines
    //calcMeanRange(&prevPLines);
    // GLCD_print("P %f %f\n\r",prevPLines.P.CiHigh, prevPLines.P.CiLow);
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    for(i=0; i<3; i++){
      devProfiles[i].dP=0;
      devProfiles[i].dQ=0;
      devLamps[i]=0;
    }
    
    // initialize gui
    gui_monitoringScreen(devProfiles,devLamps);
    
    GLCD_SetFont(&Terminal_6_8_6,0x0000FF,0x000cd4ff);
    GLCD_SetWindow(0,0,319,239);
    GLCD_TextSetPos(0,0);
    //calculate the no load powerlines
    calcMeanRange(&prevPLines);
    
    while(1)
    {
        
        ///////////////////////////////////////////////////////////////////////////////
        GLCD_SetFont(&Terminal_6_8_6,0x0000FF,0x000cd4ff);
        GLCD_SetWindow(0,0,319,239);
        GLCD_TextSetPos(0,0);
        
        /////////////////////////////////////////////////////////////////////////////////
        
        /////////////////////////////////////////////////////////////////////////////////
        //algo    START
        /////////////////////////////////////////////////////////////////////////////////
        if(dataReady){
            dataReady=0;
            GLCD_print("%s\r\n",dataArray);
            //  GLCD_print("reach reach!!!\r\n");
                        
            //convert the incoming data to floats
            dataConversion(dataArray, dataP, dataQ, dataT, cnt);           
            
            //keep track of devices
            GLCD_print("Device zero:%d   one:%d   two:%d\r",devLamps[0],
            devLamps[1],devLamps[2]);
            
            concurCnt=0;
            //detect a step change.
            if(detectStepChange(&prevPLines, dataP[cnt], dataQ[cnt] )){
                tempcnt=(int) fmod(cnt+4,45);
                while(cnt != tempcnt){
                    if(dataReady){
                        dataConversion(dataArray, dataP, dataQ, dataT, cnt);
                        if(detectStepChange(&prevPLines, dataP[cnt], dataQ[cnt] )){
                            concurCnt++;
                        }
                        cnt++;
                        dataReady=0;
                        if(cnt>=45){
                            cnt=0;
                        }
                    }
                }
                
                if(concurCnt>=3){
                    //calculate new powerlines
                    calcMeanRange(&curPLines);
                    
                    tmpDev.dP = curPLines.P.mean - prevPLines.P.mean;
                    tmpDev.dQ = curPLines.Q.mean - prevPLines.Q.mean;
                    GLCD_print("test dev dP is %f\r\n",tmpDev.dP);
                    GLCD_print("test dev dQ is %f\r\n",tmpDev.dQ);
                    
                    
                    //in learning phase
                    if(screen==1){
                        
                        // GLCD_print("we can now add DEVICES!!!\r\n");
                        
                        if(devNum==3){
                            GLCD_print("Can't learn anymore devices\r\n");
                            //learning=0;
                        }
                        else{
                            if(tmpDev.dP>0 && tmpDev.dQ>0){
                                
                                GLCD_print("add device or not?\r\n");
                                
                                if(addDevice){
                                    
                                    devProfiles[devNum].dP = tmpDev.dP;
                                    devProfiles[devNum].dQ = tmpDev.dQ;
                                    GLCD_print("new profile %d p:%f q:%f\r\n",devNum,
                                    tmpDev.dP,tmpDev.dQ);
                                    devNum++;
                                }
                                
                            }
                            else{
                                GLCD_print("device is unplugged\r\n");
                                //DO THE CHECK
                                
                                //   determineDevice(devProfiles,tmpDev,&devNum,devLamps);
                                //gui_monitoringScreen(devProfiles,devLamps);
                            }
                        }
                    }
                    //in user phase
                    else if (screen==0){
                        if(devNum==0){
                            GLCD_print("No devices on file. Please enter learning mode");
                        }
                        
                        else{
                            //check aganst known devices
                            for(k=0;k<3;k++){
                                GLCD_print("devProfile %d p:%f  q:%f\r\n",k, devProfiles[k].dP,
                                devProfiles[k].dQ);
                                //if plugging out, the deltas will be negative
                                if(tmpDev.dP<0 && tmpDev.dQ<0){
                                    plugOut=1;
                                    tmpDev.dP = fabs(tmpDev.dP);
                                    tmpDev.dQ = fabs(tmpDev.dQ);
                                }
                                
                                if(withinRange(tmpDev,devProfiles[k],cInterval)){
                                    if(plugOut){
                                        GLCD_print("device %d unplugged!\r\n",k);
                                        plugOut=0;
                                        devLamps[k]=0;
                                    }
                                    else{
                                        GLCD_print("device %d plugged in!\r\n",k);
                                        devLamps[k]=1;
                                    }
                                    gui_monitoringScreen(devProfiles,devLamps);
                                    break;
                                }
                            }
                        } // end of if(devNum>0)
                    }   // end user phase
                    prevPLines = curPLines;
                    GLCD_print("prevPlines %f, %f\r",prevPLines.P,prevPLines.Q);
                }       // end of (concurCnt>=3)
            }           // end of detectStepChange()
            cnt++;
            //dataReady=0;
            if(cnt >= 45){
                cnt=0;
            }
        }               // end of dataReady
        /////////////////////////////////////////////////////////////////////////////////
        //                      End of Algo
        /////////////////////////////////////////////////////////////////////////////////
        
        
        /////////////////////////////////////////////////////////////////////////////////
        //      GUI start
        /////////////////////////////////////////////////////////////////////////////////
        
        if(TouchGet(&XY_Touch))
        {
            cursor_x = XY_Touch.X;
            cursor_y = XY_Touch.Y;
            GLCD_Move_Cursor(cursor_x, cursor_y);
            
            if (FALSE == Touch)
            {
                Touch = TRUE;
                USB_H_LINK_LED_FCLR = USB_H_LINK_LED_MASK;
            }
        }
        
        // check the need to swtich screen
        else if(Touch)
        {
            switch(screen)
            {
                
                case 0: // 0 = Monitoring screen
                
                // Touch logic
                if(modeButtonState)
                {
                    if (cursor_x <= 80 && cursor_y >= 190)
                    {
                        gui_toggleMode(devProfiles,devLamps);
                        break;
                    }
                }
                if(settingsButtonState)
                {
                    if (cursor_x >= 239 && cursor_y >= 190)
                    {
                        gui_settingsScreen();
                        break;
                    }
                }
                break;
                case 1: // 1 = Learning screen
                // Touch logic
                if(modeButtonState)
                {
                    if (cursor_x <= 80 && cursor_y >= 190) //  mode
                    {
                        gui_toggleMode(devProfiles,devLamps);
                        break;
                    }
                }
                if(settingsButtonState)
                {
                    if (cursor_x >= 239 && cursor_y >= 190) //  set
                    {
                        gui_settingsScreen();
                        break;
                    }
                }
                if(addDeviceButtonState)
                {
                    // add device button placement (80,70,240,120)
                    if (cursor_x >= 80 && cursor_y >= 70 && cursor_x <= 240 && cursor_y <= 120)
                    {
                        addDevice = 1;
                        
                        //gui_addDeviceScreen();
                        break;
                    }
                }
                break;
                case 2: // 2 = Add device screen
                // Back button
                if(backButtonState)
                {
                    if (cursor_x <= 80 && cursor_y >= 190)
                    {
                        gui_learningScreen();
                        break;
                    }
                }
                break;
                case 3: // 3: Settings screen
                // Back button
                if(backButtonState)
                {
                    if (cursor_x <= 80 && cursor_y >= 190)
                    {
                        if(gui_getMode())
                        {
                            gui_learningScreen();
                            break;
                        }
                        else
                        {
                            gui_monitoringScreen(devProfiles,devLamps);
                            break;
                        }
                    }
                }
                if(settingsButtonState)
                {
                    if (cursor_x >= 239 && cursor_y >= 190)
                    {
                        gui_settingsScreen();
                        break;
                    }
                }
                break;
                case 4: // 4: Delete Devices screen
                // Back button
                if(backButtonState)
                {
                    if (cursor_x <= 80 && cursor_y >= 190)
                    {
                        gui_learningScreen();
                        break;
                    }
                }
                
                break;
            }
            USB_H_LINK_LED_FSET = USB_H_LINK_LED_MASK;
            Touch = FALSE;
            
        }
        /////////////////////////////////////////////////////////////////////////////////
        //Network and Web handling
        /////////////////////////////////////////////////////////////////////////////////
            uip_len = tapdev_read(uip_buf);
    if(uip_len > 0)
    {
      if(BUF->type == htons(UIP_ETHTYPE_IP))
      {
	      uip_arp_ipin();
	      uip_input();
	      /* If the above function invocation resulted in data that
	         should be sent out on the network, the global variable
	         uip_len is set to a value > 0. */
	      if(uip_len > 0)
        {
	        uip_arp_out();
	        tapdev_send(uip_buf,uip_len);
	      }
      }
      else if(BUF->type == htons(UIP_ETHTYPE_ARP))
      {
        uip_arp_arpin();
	      /* If the above function invocation resulted in data that
	         should be sent out on the network, the global variable
	         uip_len is set to a value > 0. */
	      if(uip_len > 0)
        {
	        tapdev_send(uip_buf,uip_len);
	      }
      }
    }
    else if(timer_expired(&periodic_timer))
    {
      timer_reset(&periodic_timer);
      for(e = 0; e < UIP_CONNS; e++)
      {
      	uip_periodic(e);
        /* If the above function invocation resulted in data that
           should be sent out on the network, the global variable
           uip_len is set to a value > 0. */
        if(uip_len > 0)
        {
          uip_arp_out();
          tapdev_send(uip_buf,uip_len);
        }
      }
#if UIP_UDP
      for(e = 0; e < UIP_UDP_CONNS; e++) {
        uip_udp_periodic(e);
        /* If the above function invocation resulted in data that
           should be sent out on the network, the global variable
           uip_len is set to a value > 0. */
        if(uip_len > 0) {
          uip_arp_out();
          tapdev_send();
        }
      }
#endif /* UIP_UDP */
      /* Call the ARP timer function every 10 seconds. */
      if(timer_expired(&arp_timer))
      {
        timer_reset(&arp_timer);
        uip_arp_timer();
      }
    }
   }
    


 
    
}