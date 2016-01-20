#include "algo.h"

//Returns the mean from an array of double
Flo64 getMean(pFlo64 pBuffer, Int32U BufferSize){
  Int32U Count;
  Flo64 m = 0;
  for (Count = 0; Count < BufferSize; ++Count){
    m += *(pBuffer+Count)/BufferSize;
  } 
  return m;
}

//Calculates the boundaries of the signal stored in Buffer
void getCI(pFlo64 pBuffer, Int32U BufferSize, pAlgoLine_t line){
  Int32U Count;
  Flo64 sd=0;
  //mean(x)
  (*line).mean = getMean(pBuffer,BufferSize);

  //Squared difference to mean
  for (Count = 0; Count < BufferSize; ++Count){
    sd += pow(*(pBuffer+Count)-(*line).mean,2);
  }
  sd /= (Flo64) (BufferSize-1);
  sd = sqrt(sd);                //standard deviation
  
  (*line).CiHigh = (*line).mean + 3*sd;
  (*line).CiLow = (*line).mean - 3*sd;
}

//Test if a point is within bounds
Int32U withinBounds(Flo64 x, pAlgoLine_t line ){
  return (x>(*line).CiLow)&&(x<(*line).CiHigh);
}

//test if a device is within the range of a profile device
Int32U withinRange(AlgoDevice_t dev, AlgoDevice_t profile , Flo64 interval){
    //1st element: lower   2nd element: higher
    Flo64 pBnds[] = {profile.dP-5000,profile.dP+5000};
    Flo64 qBnds[] = {profile.dQ-5000,profile.dQ+5000};
    
    if(dev.dP>pBnds[0] && dev.dP<pBnds[1]){
      //  if(dev.dQ>qBnds[0] && dev.dQ<qBnds[1]){
            return 1;            
       // }       
    }
    return 0;  
}

//Calculates intervals for P and Q
void calcPower(pFlo64 pP, pFlo64 pQ, Int32U size, pAlgoPowers_t power){
  getCI(pP,size,&((*power).P) );
  getCI(pQ,size,&((*power).Q) );
}

//Detects change in both P and Q
Int32U detectStepChange(pAlgoPowers_t power, Flo64 xP, Flo64 xQ){
  //Signal is within limits
  if ( (withinBounds(xP, &(*power).P))&&(withinBounds(xQ, &(*power).Q))){
    return 0;
  }else{
    return 1;
  }
}


void dataConversion(pInt8U pS, pFlo64 pP, pFlo64 pQ, pFlo64 pT, Int32U n){
  //pA is the pointer to the double array
  
  //3 , 15 , 27 are the indices for the begining of the 3 number strings 
  
  Int8U substring[9] = {0};
  strncpy(substring,pS+3,8);    
  substring[9]='\0' ;  
 *(pP+n)= atoi(substring);

  strncpy(substring,pS+15,8);    
  substring[9]='\0' ;  
 *(pQ+n)= atoi(substring);

  strncpy(substring,pS+27,8);    
  substring[9]='\0' ;  
 *(pT+n)= atoi(substring);
 
 
 //void getData(){
   
   
 //}
 
 
 
}

/*
//check aganst known devices
int determineDevice(pAlgoDevice_t pDevs, AlgoDevice_t tmpDev, pInt32S devNum, pInt32S pDevLamps){
  int i;
  int plugOut=0;
  int recognized=0;
  int updateScreen = 0;
  
 // GLCD_TextSetPos(5,0);
    //check aganst known devices
  for( i=0;i<3;i++){
      GLCD_print("devProfile %d p:%f  q:%f\r\n",i,(*(pDevs+i)).dP,(*(pDevs+i)).dQ);
      //if plugging out, the deltas will be negative
      if(tmpDev.dP<0 && tmpDev.dQ<0){
          plugOut=1;
          tmpDev.dP = fabs(tmpDev.dP);
          tmpDev.dQ = fabs(tmpDev.dQ);
      }
      GLCD_print("within range %d\r\n",withinRange(tmpDev,(*(pDevs+i)),5000));
      if(withinRange(tmpDev,(*(pDevs+i)),5000)){
          if(plugOut){
              GLCD_print("device %d unplugged!\r\n",i);
              plugOut=0;
              (*(pDevLamps+i))=0;
          }
          else{
              GLCD_print("device %d plugged in!\r\n",i);                           
               (*(pDevLamps+i))=1;
          }
          recognized = 1;
         // gui_monitoringScreen(pDevs,pDevLamps);
          updateScreen = 1;
          break;
      }
  }
  if(!recognized){
    GLCD_print("Unkown device\r\n");
  }
  
  return updateScreen;
  
}*/



