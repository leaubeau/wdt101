//***************************************************************
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
//***************************************************************


//***************************************************************
//
// File: wdt101.h
//
// Description:
//   Watch Dog Timer.  Class definitions for library wdt.
//
// Notes:
//
//   - Arc has two timers, Timer-0 is used.  Please check the file
//     arcv2_timer0.cpp.  CurieTimerOne uses Timer-1.
//
//
// Cautions:
//   - The module, Tone.cpp, also makes use of Arc Timer-1 which
//     is used here.
//
//***************************************************************


#ifndef wdt101_h
#define wdt101_h

#include <Arduino.h>
#include <aux_regs.h>
#include <interrupt.h>
#include <conf.h>

// Timer-1 is clocked at ARCV2_TIMER1_CLOCK_FREQ defined in conf.h
const unsigned int HZ_USEC = (ARCV2_TIMER0_CLOCK_FREQ / 1000000);  // Hz per micro second.

// TIMERx_CONTROL register.
const unsigned int ARC_TIMER_EN_INTR_BIT_FLAG = 0x01;  // Bit 0. Enable timer interrupt.
const unsigned int ARC_TIMER_NON_HALT_ONLY_BIT_FLAG = 0x02;  // Bit 1. Allow timer to run in Halted mode.
const unsigned int ARC_TIMER_WATCHDOG_BIT_FLAG = 0x04;  // Bit 2. Make timer a watchdog timer.
const unsigned int ARC_TIMER_INTR_PENDING_BIT_FLAG = 0x08;  // Bit 3. Interrupt pending and clearing bit.

typedef enum {
  SUCCESS = 0,
  INVALID_PERIOD,
  INVALID_DUTY_CYCLE
} timerErrType;

typedef enum {
  IDLE = 0,
  RUNNING,
  PAUSED
} timerStateType;


//
//  Class:  wdt
//
//  Description:
//    This class describes the functionalities of a Arc Timer, in particular, timer-1.
//  Timer-0 is not available for this module to utilize.  The timers are clocked by
//  a 32 HHz source and have 32-bit counters.
//

class WDT101
{
  public:
    // Constructor.
    WDT101();

    // The following methods are similar to the ones found in the AVR TimerOne library.

    //****************************
    // WDT
    //****************************
    void wdt_enable(unsigned long period = 1000000);

    inline void wdt_reset(void) { start(); }
    
    //****************************
    //  Run Control
    //****************************

    inline void start(void) {
      pause();
      pauseCount = 0;
      resume();
    }

    inline void stop(void) { return pause(); }

    inline void restart(void) { start(); }

    // Resume the timer from where it was paused.
    void resume(void);

    // Kill the timer, put it back to power up default.
    void kill(void);

    
    // Timer interrupt count.
    inline unsigned int readTickCount(void) { return tickCnt; }
    // Read and reset timer interrupt count.
    unsigned int rdRstTickCount(void);

    // Pausing the timer = no count up, no interrupt.
    void pause(void);

  private:
    unsigned int timerCountAddr;
    unsigned int timerControlAddr;
    unsigned int timerLimitAddr;
    unsigned int timerIrqNum;

    unsigned int tickCnt;

    timerStateType currState;
    unsigned int pauseCntrl;
    unsigned int pauseCount;

};

extern WDT101  wdt;


#endif // wdt101_h

