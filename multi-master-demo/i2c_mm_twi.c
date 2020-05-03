////////////////////////////////////////////////////////////////////////////////
// File        : i2c_mm_twi.c
// Description : Multi-master mode on TWI hardware.
// Author      : R. Trapp
// Project     : I²C-Lib Demo
// Initial date: May /  1 / 2020
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


/// \file i2c_mm_twi.c
/// \brief
/// Demo application performing as one master of several masters.


// Check project settings (Makefile) to match this demo:
#ifndef I2C0_HW_AS_MASTER
#error "I2C0_HW_AS_MASTER is *not* set but needed here!"
#endif
#ifdef I2C0_HW_AS_SLAVE
#error "I2C0_HW_AS_SLAVE is set but invalid here!"
#endif
#ifdef I2C0_HW_SINGLE_MASTER
#error "I2C0_HW_SINGLE_MASTER is set but invalid here!"
#endif


// In addition those are needed to be defined by the make system (hardware.mad):
#define PCF8574_BITPOS     (1<<7)

#define LED_PORT           PORTB
#define LED_DDR            DDRB
#define LED_BIT            1
#define LED_BITPOS         (1<<LED_BIT)

#define TRIGGER_PORT       PORTB
#define TRIGGER_DDR        DDRB
#define TRIGGER_READBACK   PINB
#define TRIGGER_BIT        2
#define TRIGGER_BITPOS     (1<<TRIGGER_BIT)

// necessary includes
#include <avr/io.h>
#include <util/delay.h>
#include "i2c_hw.h"


// definitions for hardware
#define PCF8574_ADDRESS         0x40 // 0b0100aaa << 1, a = 0
#define TURN_LED_ON             LED_PORT |= LED_BITPOS
#define TURN_LED_OFF            LED_PORT &= ~LED_BITPOS
#define TRIGGERED               !(TRIGGER_READBACK & TRIGGER_BITPOS)


// declare (and init) some variables
uint8_t slave_read_access_counter = 0;
uint8_t toggle = 0;


/******************************************************************************\

  Master activities on IӃ bus.

\******************************************************************************/


/// \brief
/// Gets one byte from a PCF8574. PCF8574 A2..A0 setting is 0b000.
/// Returns state. Does not release the bus!
/// \details
/// Only way to loose arbitration here is during addressing cycle.
/// \returns Failure code(s) occured during last bus access(es).
uint8_t get_byte_from_pcf8574 (uint8_t* byte_p)
{
    if (!hardI2c0_openDevice(PCF8574_ADDRESS | I2C_READ_ACCESS))
    {
        *byte_p = hardI2c0_getByteAsMaster(I2C_SEND_NO_ACK);
    }
    return(hardI2c0_check4Error());
}


/// \brief
/// Puts one byte to a PCF8574. PCF8574 A2..A0 setting is 0b000.
/// Returns state. Does not release the bus!
/// \details
/// PCF8574 is addressed to be written to. If bus or slave is not available the
/// appropriate error code is returned. If bus and slave are available the byte
/// is sent. In case the arbitration is lost (due to another master accessing the
/// bus or even the same slave!) the attempt is retired, else the bus is released.
/// In case another master addresses the same slave the arbitration might get lost
/// in the data section instead of the address cycle! If both masters write the
/// exact same data section then the access is transparent to both masters - none
/// of them looses the arbitration, both do their access not knowing that the other
/// one does it also.
/// \returns Failure code(s) occured during last bus access(es).
uint8_t put_byte_to_pcf8574 (uint8_t byte)
{
    if (!hardI2c0_openDevice(PCF8574_ADDRESS | I2C_WRITE_ACCESS))
    {
        hardI2c0_putByteAsMaster(byte);
    }
    return(hardI2c0_check4Error());
}


/******************************************************************************\

 main loop

\******************************************************************************/


int main (void)
{
    uint8_t retVal;

    // init IO ports
    TURN_LED_OFF;                           // preset off state
    LED_DDR |= LED_BITPOS;                  // prepare as output
    TRIGGER_DDR &= ~TRIGGER_BITPOS;         // prepare as input
    TRIGGER_PORT |= TRIGGER_BITPOS;         // turn on pull up
    PORTB |= 0x01;
    // init IӃ subsystem
    hardI2c0_init(OWN_SLAVE_ADDRESS & 0xFE, // ensure NO general call response
                  0,                        // single address response only
                  I2C0_BITRATE,             // defined by the lib
                  I2C0_PRESCALER);          // defined by the lib
    // looping infinitely
    while (1)
    {
        // This section is dedicated to the application
        if (TRIGGERED)
        { // the pushbutton got pressed, now access IӃ slaves.
            toggle = ~toggle;
            while (1)
            {
                if (!get_byte_from_pcf8574(&retVal))
                { // reading PCF8574 successfully
                    if (toggle)
                        retVal &= ~PCF8574_BITPOS;
                    else
                        retVal |= PCF8574_BITPOS;
                    if(!put_byte_to_pcf8574(retVal))
                    { // success
                        break;
                    }
                }
                if (hardI2c0_check4Error() & I2C_NO_ACK)
                { // no such slave device - skip or die looping ...
                    hardI2c0_releaseBus();
                    break;
                }
                while(hardI2c0_check4Error() & (I2C_ARBITRATION_LOST | I2C_RESTARTED))
                {
//                    do_other_activities();
                }
            } // PCF8574 is done
            // simple bus error indication
            if (hardI2c0_check4Error())
                DDRB |= 0x01; // bus error to LED
            else
                DDRB &= ~0x01; // LED off, no bus error
            // Do what still needs to be done after pressing the pushbutton:
            _delay_ms(10); // very simple debouncing
            // wait until pushbutton released
            while (TRIGGERED)
            {
//                do_other_activities();
            }
            _delay_ms(10); // very simple debouncing
        } // if (TRIGGERED)
        else
        {
            // here additional actions if not triggered could take place
        }
    } // while (1)
    return(0);
}
