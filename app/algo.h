/*************************************************************************
 *      Functions for NILM project algorithm
 *   
 **************************************************************************/
#include "includes.h"
#include <math.h>
   
#ifndef __ALGO_H
#define __ALGO_H

typedef struct _AlgoLine_t
{
  Flo64 mean;
  Flo64 CiLow;
  Flo64 CiHigh;
} AlgoLine_t, *pAlgoLine_t;

typedef struct _AlgoPowers_t
{
  AlgoLine_t P; //active power
  AlgoLine_t Q; //reactive power
} AlgoPowers_t, *pAlgoPowers_t;

typedef struct _AlgoDevice_t
{
  Flo64 dP;
  Flo64 dQ;
} AlgoDevice_t, *pAlgoDevice_t;



//Returns the mean from an array of double
Flo64 getMean(pFlo64 pBuffer, Int32U BufferSize);

//Calculates the boundaries of the signal stored in Buffer
void getCI(pFlo64 pBuffer, Int32U BufferSize, pAlgoLine_t line);

//Calculates intervals for P and Q
void calcPower(pFlo64 pP, pFlo64 pQ, Int32U size, pAlgoPowers_t power);

//Test if a point is within bounds
Int32U withinBounds(Flo64 x, pAlgoLine_t line );

//test if a device is within the range of a profile device
Int32U withinRange(AlgoDevice_t dev, AlgoDevice_t profile , Flo64 interval);

//Detects change in both P and Q
Int32U detectStepChange(pAlgoPowers_t power, Flo64 xP, Flo64 xQ);

// Loads data from string to integer arrays
void dataConversion(pInt8U pS, pFlo64 pP, pFlo64 pQ, pFlo64 pT, Int32U n);

//Calculate change
//void calculateChange(pAlgoDevice_t , 




#endif // __ALGO_H

