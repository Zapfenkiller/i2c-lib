////////////////////////////////////////////////////////////////////////////////
// File        : i2c_sm_usi.c
// Description : Slave mode application demo.
// Author      : R. Trapp
// Project     : I²C-Lib Demo
// Initial date: May /  2 / 2020
// Copyright   : © 2006-2021 R. Trapp / H.A.R.R.Y.
// License     : GNU General Public License V3 (GPLv3)
//               See project root LICENSE file or
//               https://www.gnu.org/licenses/gpl-3.0.html
////////////////////////////////////////////////////////////////////////////////


/// \file i2c_sm_usi.c
/// \brief
/// Demo application performing as bus slave.


// Check project settings (Makefile) to match this demo:
#ifdef I2C0_HW_AS_MASTER
#error "I2C0_HW_AS_MASTER is set but invalid here!"
#endif
#ifndef I2C0_HW_AS_SLAVE
#error "I2C0_HW_AS_SLAVE is *not* set but needed here!"
#endif
#ifdef I2C0_HW_SINGLE_MASTER
#error "I2C0_HW_SINGLE_MASTER is set but invalid here!"
#endif


// necessary includes
#include <avr/io.h>
#include <util/delay.h>
#include "i2c_hw.h"


// definitions for hardware
#define OWN_SLAVE_ADDRESS  (0b0100010 << 1)

#define LED_PORT           PORTB
#define LED_DDR            DDRB
#define LED_BIT            1
#define LED_BITMASK        (1<<LED_BIT)


// definitions to support coding
#define INIT_LEDS          LED_DDR  |= LED_BITMASK
#define TURN_ALL_LEDS_OFF  LED_PORT &= ~LED_BITMASK
#define TURN_LED_ON        LED_PORT |= LED_BITMASK
#define TURN_LED_OFF       LED_PORT &= ~LED_BITMASK


// declare (and init) some variables
uint8_t slave_read_access_counter = 0;


/******************************************************************************\

  Slave and master activities on IӃ bus.

\******************************************************************************/


/// \brief
/// Implements the slave actions.
/// \details
/// Reading via IӃ just sends a count value and increments it afterwards.
/// Writing via IӃ decides if a LED is turned on or off. If byte is 0x00
/// then LED gets off, else on.
void do_the_slave_activities (void)
{
}


/******************************************************************************\

 main loop

\******************************************************************************/


int main (void)
{
    uint8_t retVal;

   // init IO ports
   TURN_ALL_LEDS_OFF;
   INIT_LEDS;
   // init I²C subsystem
   hardI2c0_init(OWN_SLAVE_ADDRESS,
                 0,                       // dummy
                 I2C0_BITRATE,            // predefined by the lib
                 I2C0_PRESCALER);         // predefined by the lib

   // repeat until dooms day
   while (1)
   {

    uint8_t addressReceived = OWN_SLAVE_ADDRESS & 0xFE;
    if (hardI2c0_isAddressedAsSlave(&addressReceived, 0))
    {
        if ((addressReceived & 0x01) == I2C_READ_ACCESS)
        {
            // read access(es) = send byte(s)
            while (1)
            {
                // Reply the current counter setting.
                hardI2c0_putByteAsSlave(slave_read_access_counter++);
                if (hardI2c0_check4Error())
                    // leave slave mode transmission on NACK (or any error)
                    break;
                // IMPORTANT!
                // If after a certain amount of bytes transmitted a NACK
                // is expected this must be handled in addition. It is missing
                // here in this example.
                // The best way out of communication in such a situation
                // is just to leave this loop and have hardI2co_isAddressedAsSlave()
                // been called once - or is a separate function needed in the
                // lib for this abortion?
            }
        }
        else
        {
            // write access(es) = receive byte(s)
            // -> in fact will receive every byte the master sends.
            while (1)
            {
                uint8_t byte = hardI2c0_getByteAsSlave(I2C_SEND_ACK);
                if (hardI2c0_check4Error())
                    // leave slave mode reception on STOP (or any error)
                    break;
                if (byte)
                    TURN_LED_ON;
                else
                    TURN_LED_OFF;
            }
        }
    }


   } // while (1)
}
