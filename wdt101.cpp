//***************************************************************
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
//***************************************************************

//***************************************************************
//
// Module: wdt
//
//
// Notes:
//
//   - Arc has two timers, Timer-0 is used.  Please check the file
//     arcv2_timer0.cpp.
//
// Cautions:
//   - The module, Tone.cpp, also makes use of Arc Timer-1 which
//     is used here.
//
//***************************************************************


#include "wdt101.h"
#include <conf.h>

WDT101 wdt;

WDT101::WDT101() :
  tickCnt(0), currState(IDLE)
{
  timerCountAddr = ARC_V2_TMR0_COUNT;
  timerControlAddr = ARC_V2_TMR0_CONTROL;
  timerLimitAddr = ARC_V2_TMR0_LIMIT;
  timerIrqNum = ARCV2_IRQ_TIMER0;
}


// Method:  kill
// Description:
//   Set the timer back to power up default condition.

void WDT101::kill()
{
  interrupt_disable(timerIrqNum);  // Disable Timer at controller
  aux_reg_write(timerControlAddr, 0);  // Disable Timer itself
  aux_reg_write(timerLimitAddr, 0);
  aux_reg_write(timerCountAddr, 0);

  tickCnt = 0;
  currState = IDLE;
  pauseCntrl = pauseCount = 0;
}


unsigned int WDT101::rdRstTickCount(void)
{
  unsigned int tmp;

  tmp = tickCnt;
  tickCnt = 0;
  return tmp;
}


void WDT101::pause(void)
{
  if(currState != RUNNING)
    return;

  pauseCntrl = aux_reg_read(timerControlAddr);
  pauseCount = aux_reg_read(timerCountAddr);

  interrupt_disable(timerIrqNum);  // Disable Timer at controller
  aux_reg_write(timerControlAddr, 0);  // Disable Timer itself

  currState = PAUSED;
}


void WDT101::resume(void)
{
  if(currState != PAUSED)
    return;

  aux_reg_write(timerCountAddr, pauseCount);
  aux_reg_write(timerControlAddr, pauseCntrl);
  currState = RUNNING;

  // Re-enable timer interrupt once timer is reloaded with previous stop values.
  interrupt_enable(timerIrqNum);
}



// Method: wdt_enable
// Kick off the timer as a watchdog with a period in microseconds.
//
void WDT101::wdt_enable(unsigned long period)
{
  if (period == 0) { return; }  // invalid timer value

  unsigned int periodHz = period * HZ_USEC;
  
  interrupt_disable(timerIrqNum);     // Disable Timer at controller
  aux_reg_write(timerControlAddr, 0); // Disable Timer itself

  aux_reg_write(timerLimitAddr, periodHz);  // Load Timer period

  aux_reg_write(timerCountAddr, 0);  // Reset variables
  tickCnt = 0;

  aux_reg_write(timerControlAddr, ARC_TIMER_WATCHDOG_BIT_FLAG);

  currState = RUNNING;
  return;
}




