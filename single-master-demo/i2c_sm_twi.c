////////////////////////////////////////////////////////////////////////////////
// File        : i2c_sm_twi.c
// Description : Single bus master application demo.
// Author      : R. Trapp
// Project     : I²C-Lib Demo
// Initial date: May /  2 / 2020
// Copyright   : © 2020 R. Trapp / H.A.R.R.Y.
// License     : https://mit-license.org/
//
//               The MIT License (MIT)
//
//               Permission is hereby granted, free of charge, to
//               any person obtaining a copy of this software and
//               associated documentation files (the “Software”),
//               to deal in the Software without restriction,
//               including without limitation the rights to use,
//               copy, modify, merge, publish, distribute,
//               sublicense, and/or sell copies of the Software,
//               and to permit persons to whom the Software is
//               furnished to do so, subject to the following
//               conditions:
//
//               The above copyright notice and this permission
//               notice shall be included in all copies or
//               substantial portions of the Software.
//
//               THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT
//               WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
//               INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//               MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
//               AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//               AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//               CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
//               ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//               FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
//               OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
////////////////////////////////////////////////////////////////////////////////


/// \file i2c_sm_twi.c
/// \brief
/// Demo application performing as single bus master.


// Check project settings (Makefile) to match this demo:
#ifndef I2C0_HW_AS_MASTER
#error "I2C0_HW_AS_MASTER is *not* set but needed here!"
#endif
#ifdef I2C0_HW_AS_SLAVE
#error "I2C0_HW_AS_SLAVE is set but invalid here!"
#endif
#ifndef I2C0_HW_SINGLE_MASTER
#error "I2C0_HW_SINGLE_MASTER is *not* set but needed here!"
#endif


// necessary includes
#include <avr/io.h>
#include <util/delay.h>
#include "i2c_hw.h"


// definitions for hardware
#define REMOTE             (0b0100010 << 1)
#define PCF8574            (0b0100000 << 1)
#define PCF8574_BITMASK    (1<<7)

#define LED_PORT           PORTB
#define LED_DDR            DDRB
#define LED_BIT            1
#define LED_BITMASK        (1<<LED_BIT)
#define ERROR_LED_BIT      0
#define ERROR_LED_BITMASK  (1<<ERROR_LED_BIT)

#define TRIGGER_PORT       PORTB
#define TRIGGER_DDR        DDRB
#define TRIGGER_READBACK   PINB
#define TRIGGER_BIT        2
#define TRIGGER_BITMASK    (1<<TRIGGER_BIT)


// definitions to support coding
#define INIT_LEDS          LED_DDR  |= LED_BITMASK | ERROR_LED_BITMASK
#define TURN_ALL_LEDS_OFF  LED_PORT &= ~(LED_BITMASK | ERROR_LED_BITMASK)
#define TURN_LED_ON        LED_PORT |= LED_BITMASK
#define TURN_LED_OFF       LED_PORT &= ~LED_BITMASK
#define TURN_ERROR_ON      LED_PORT |= ERROR_LED_BITMASK
#define TURN_ERROR_OFF     LED_PORT &= ~ERROR_LED_BITMASK
#define PREPARE_PB_PORT    TRIGGER_DDR &= ~TRIGGER_BITMASK
#define ENABLE_PB_PULLUP   TRIGGER_PORT |= TRIGGER_BITMASK
#define PUSHED             !(TRIGGER_READBACK & TRIGGER_BITMASK)


/******************************************************************************\

 Main

\******************************************************************************/


/// \brief
/// Pressing the pushbutton toggles LEDs connected to different
/// I²C slaves. If one of the slaves does not respond an optional
/// error indication shows up.
/// \details
/// The demo application initializes the micro hardware according
/// to the datasheet and schematic. It then loops infinitely.
/// Upon press of the pushbutton I²C accesses are done to toggle
/// the LED status of some connected I²C slave devices.
/// In case of some bus error - here it is just a non-responding
/// slave - one dedicated LED lights up. The error handling is
/// just optional and could be fully omitted for this simple demo.

int main (void)
{
   uint8_t toggle = 0;
   uint8_t retVal;
   uint8_t busErr;

   // init IO ports
   TURN_ALL_LEDS_OFF;
   INIT_LEDS;
//   PREPARE_PB_PORT;   // Implictly done by LED setup
   ENABLE_PB_PULLUP;
   // init I²C subsystem
   hardI2c0_init(0,                       // dummy
                 0,                       // dummy
                 I2C0_BITRATE,            // predefined by the lib
                 I2C0_PRESCALER);         // predefined by the lib

   // repeat until dooms day
   while (1)
   {
      if (PUSHED)
      {
         // the pushbutton got pressed
         toggle = ~toggle;
         if (toggle)
            TURN_LED_ON;
         else
            TURN_LED_OFF;

         // access the remote slave
         busErr = hardI2c0_openDevice(REMOTE | I2C_WRITE_ACCESS);
         if (!busErr)
         { // remote slave response
            hardI2c0_putByteAsMaster(toggle);
         }

         // access the PCF8574
         busErr |= hardI2c0_openDevice(PCF8574 | I2C_READ_ACCESS);
         if (!busErr)
         { // PCF8574 response
            retVal = hardI2c0_getByteAsMaster(I2C_SEND_NO_ACK);
            if (toggle)
               retVal &= ~PCF8574_BITMASK;
            else
               retVal |= PCF8574_BITMASK;
            hardI2c0_openDevice(PCF8574 | I2C_WRITE_ACCESS);
            hardI2c0_putByteAsMaster(toggle);
         }

         // I²C transfers done, release the bus
         hardI2c0_releaseBus();

         // simple bus error indication, just optional
         if (busErr)
            TURN_ERROR_ON;
         else
            TURN_ERROR_OFF;

         // do what still needs to be done
         _delay_ms(10);       // very simple debouncing
         while (PUSHED) {}    // wait until pushbutton released
         _delay_ms(10);       // very simple debouncing

      } // if (PUSHED)
   } // while (1)
}
