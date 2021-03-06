/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : clock-arch.c
 *    Description : Implementation of architecture-specific clock functionality
 *
 *    History :
 *    1. Date        : October 4, 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 28 $
**************************************************************************/
#include "clock-arch.h"

volatile clock_time_t Ticks;

/*************************************************************************
 * Function Name: Tim0Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Timer 0 interrupt handler
 *
 *************************************************************************/
static
void Timer1IntrHandler (void)
{
  ++Ticks;
  T1IR_bit.MR0INT = 1; //change to timer1
  VICADDRESS = 0;
}

/*************************************************************************
 * Function Name: clock_init
 * Parameters: Int32U IntrPriority
 *
 * Return: none
 *
 * Description: Timer init
 *
 *************************************************************************/
void clock_init(Int32U IntrPriority)
{
  Ticks = 0;

  // Init Time1 
  //change to timer1
  PCONP_bit.PCTIM1 = 1; // Enable TMR0 clk
  T1TCR_bit.CE = 0;     // counting  disable
  T1TCR_bit.CR = 1;     // set reset
  T1TCR_bit.CR = 0;     // release reset
  T1CTCR_bit.CTM = 0;   // Timer Mode: every rising PCLK edge
  T1MCR_bit.MR1I = 1;   // Enable Interrupt on MR0
  T1MCR_bit.MR1R = 1;   // Enable reset on MR0
  T1MCR_bit.MR1S = 0;   // Disable stop on MR0
  // set timer 0 period
  T1PR = 0;
  T1MR1 = SYS_GetFpclk(TIMER0_PCLK_OFFSET)/(TICK_PER_SEC);
  // init timer 0 interrupt
  T1IR_bit.MR1INT = 1;  // clear pending interrupt
  VIC_SetVectoredIRQ(Timer1IntrHandler,IntrPriority,VIC_TIMER1);
  VICINTENABLE |= 1UL << VIC_TIMER1;
  T1TCR_bit.CE = 1;     // counting Enable
}

/*************************************************************************
 * Function Name: clock_init
 * Parameters: none
 *
 * Return: none
 *
 * Description: The current clock time, measured in system ticks
 *
 *************************************************************************/
clock_time_t clock_time(void)
{
  return(Ticks);
}
