////////////////////////////////////////////////////////////////////////////////
// File        : i2c_mms_usi.c
// Description : Multi-master-with-slave mode on USI hardware.
// Author      : R. Trapp
// Project     : I²C-Lib Demo
// Initial date: Apr / 30 / 2020
// Copyright   : © 2006-2021 R. Trapp / H.A.R.R.Y.
// License     : GNU General Public License V3 (GPLv3)
//               See project root LICENSE file or
//               https://www.gnu.org/licenses/gpl-3.0.html
////////////////////////////////////////////////////////////////////////////////


/// \file i2c_mms_usi.c
/// \brief
/// Demo application performing as one master of several masters and as slave.


// Check project settings (Makefile) to match this demo:
#ifndef I2C0_HW_AS_MASTER
#error "I2C0_HW_AS_MASTER is *not* set but needed here!"
#endif
#ifndef I2C0_HW_AS_SLAVE
#error "I2C0_HW_AS_SLAVE is *not* set but needed here!"
#endif
#ifdef I2C0_HW_SINGLE_MASTER
#error "I2C0_HW_SINGLE_MASTER is set but invalid here!"
#endif


// In addition those are needed to be defined by the make system (hardware.mad):
#define OWN_SLAVE_ADDRESS  0x44
#define REMOTE_SLAVE       0x42
#define PCF8574_BITPOS     (1<<4)

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
#define TURN_LED_OFF            LED_PORT |= (1<<LED_BIT)
#define TURN_LED_ON             LED_PORT &= ~(1<<LED_BIT)
#define TRIGGERED               !(TRIGGER_READBACK & TRIGGER_BITPOS)


// declare (and init) some variables
uint8_t slave_read_access_counter = 0;
uint8_t toggle = 0;


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
}


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
/// bus or even the same slave!) the attempt is retired.
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


/// \brief
/// Puts one byte to a remote slave.
/// Returns state. Releases bus except for bus arbitration loss.
/// \details
/// The remote slave is addressed to be written to. If bus or slave is not available the
/// appropriate error code is returned. If bus and slave are available the byte
/// is sent. In case the arbitration is lost (due to another master accessing the
/// bus or even the same slave!) the attempt is retired, else the bus is released.
/// In case another master addresses the same slave the arbitration might get lost
/// in the data section instead of the address cycle! If both masters write the
/// exact same data section then the access is transparent to both masters - none
/// of them looses the arbitration, both do their access not knowing that the other
/// one does it also.
/// \returns Failure code(s) occured during last bus access(es).
uint8_t put_byte_to_remote_slave (uint8_t byte)
{
    if (!hardI2c0_openDevice(REMOTE_SLAVE | I2C_WRITE_ACCESS))
    {
        hardI2c0_putByteAsMaster(byte);
    }
    // skip bus release if arbitration lost
    if (!(hardI2c0_check4Error() & I2C_ARBITRATION_LOST))
    {
        hardI2c0_releaseBus();
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
    // init IӃ subsystem
    hardI2c0_init(OWN_SLAVE_ADDRESS & 0xFE, // ensure NO general call response
                  0,                        // single address response only
                  I2C0_BITRATE,             // defined by the lib
                  I2C0_PRESCALER);          // defined by the lib
    // looping infinitely
    while (1)
    {
        // This section checks if the device is addressed as slave
        // and handles the assigned actions.
        do_the_slave_activities();

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
                    break;
                }
                while(hardI2c0_check4Error() & (I2C_ARBITRATION_LOST | I2C_RESTARTED))
                {
                    do_the_slave_activities();
                }
            } // PCF8574 is done, now the remote slave - bus still is occupied
            // simple bus error indication
            if (hardI2c0_check4Error())
                DDRB |= 0x08; // bus error to LED
            else
                DDRB &= ~0x08; // LED off, no bus error
            // Usually at this point the arbitration is complete and no other
            // master is on the bus. But to be perfectly sure (and also to give
            // a good example) an arbitration still is possible with this code.
            // Access the remote slave now.
            while (1)
            {
                if(!(put_byte_to_remote_slave(toggle) & ~I2C_NO_ACK))
                { // success or no such device (prevents looping until dooms day)
                    break;
                }
                while(hardI2c0_check4Error() & (I2C_ARBITRATION_LOST | I2C_RESTARTED))
                {
                    do_the_slave_activities();
                }
            } // The remote slave is also done now, the bus has been released.
            // simple bus error indication
            if (hardI2c0_check4Error())
                DDRB |= 0x01; // bus error to LED
            else
                DDRB &= ~0x01; // LED off, no bus error
            // bus error code to terminal - just for debugging purposes
            // Do what still needs to be done after pressing the pushbutton:
            _delay_ms(10); // very simple debouncing
            // wait until pushbutton released
            while (TRIGGERED)
            {
                do_the_slave_activities();
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
