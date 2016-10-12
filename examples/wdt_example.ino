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
// Print out some messages thru the serial port to show that wdt
// is working.
//
//***************************************************************

#include <wdt101.h>
#define resetIterations 6

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("wdt_example: starting watchdog with 1s timeout");
  wdt.wdt_enable(1000000);  
}

void loop () {

  wdt.wdt_reset();

  // Delay a few times but reset the watchdog between delays.
  // Should see resetIterations messages
  for (int i=0; i<resetIterations; i++) {
    Serial.print  ("wdt_example: watchdog reset #");
    Serial.println(i);
    delay(500);
    wdt.wdt_reset();
  }

  // Set a big delay where the watchdog will timeout.  Print
  // message afterwards should never print
  //
  Serial.println("wdt_example: setting 2s delay to trigger watchdog");
  delay(2000000);
  Serial.println("ERROR: THIS LINE SHOULD NEVER BE PRINTED IF THE WATCHDOG IS WORKING");
  
}

