////////////////////////////////////////////////////////////////////////////////
// File        : i2c_hw_twi.h
// Description : Private definitions of TWI library routines.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: Aug / 13 / 2012
// Copyright   : © 2006-2020 R. Trapp / H.A.R.R.Y.
// License     : https://mit-license.org/
//
//               The MIT License (MIT)
//               ---------------------
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
//  --- Doxygen ---
/// \file   i2c_hw_twi.h
/// \brief
/// A collection of useful things internal to the I<SUP>2</SUP>C library.
/// \details
/// Based on an older version of 2006 the I<SUP>2</SUP>C library had been rewritten.
/// This file contains definitions for ease of I<SUP>2</SUP>C library subroutines
/// programming, usage and code readability.
///
/// It is needed for the library routines to compile but not for using the library.
////////////////////////////////////////////////////////////////////////////////


#ifndef I2C_HW_TWI_H_INCLUDED
#define I2C_HW_TWI_H_INCLUDED


#include "i2c_def.h"
#include <inttypes.h>

/// \brief
/// A shortcut to aid enabling the TWI.
#define I2C0_ENABLE                 I2C0_HW_CONTROL_REG = (1 << TWEN) | slaveAckControl;


/// \brief
/// A chortcut to aid disabling the TWI.
#define I2C0_DISABLE                I2C0_HW_CONTROL_REG = 0;


/// \brief
/// The definition of the acknowledge bit state for 'ACK'.
#define I2C_ACKNOWLEDGE             0


/// \brief
/// The definition of the acknowledge bit state for 'NACK'.
#define I2C_NO_ACKNOWLEDGE          1


/// \brief
/// Aid for coding USI (TWI) routines.
#define I2C0_WAITING_FOR_SLAVE      (!(I2C0_HW_READBACK & (1 << I2C0_HW_SCL_BIT)))


/// \brief
/// Sends one byte to the bus.
/// \details
/// One byte is sent to the bus using the I<SUP>2</SUP>C protocol.
/// The slave is expected to send an 'ACK' answer.
///
/// This function merely serves as a subroutine for the different operation modes.
/// It does not do any failure checking since this depends on the state of
/// the TWI hardware.
/// \param dataByte to be sent to the slave.
void twi0_send_byte(uint8_t dataByte);


/// \brief
/// Returns one byte read from the bus.
/// \details
/// The slave is read using the I<SUP>2</SUP>C protocol.
/// According to the bus protocol the slave expects an 'ACK' to indicate subsequent
/// transfers. A slave receiving a NACK response will assume that the actual 
/// data transfer is the final one (aka last byte).
///
/// This function merely serves as a subroutine for the different operation modes.
/// It does not do any failure checking since this depends on the state of
/// the TWI hardware.
/// \param finalTransfer will suppress an 'ACK' to the bus if set (!=0).
/// \returns
/// Byte read from bus slave.
uint8_t twi0_receive_byte(uint8_t finalTransfer);


// =============================================================================
// All AVR devices with TWI receive their hardware definitions here.
// =============================================================================

#if defined (__AVR_ATmega8__)
#   define I2C0_HW_PORT                 PORTC
#   define I2C0_HW_READBACK             PINC
#   define I2C0_HW_DDR                  DDRC
#   define I2C0_HW_SCL_BIT              5
#   define I2C0_HW_SDA_BIT              4
#   define I2C0_HW_SLAVE_ADDRESS_REG    TWAR
#   define I2C0_HW_NO_SLAVE_ADDRESS_MASK
#   define I2C0_HW_BITRATE_REG          TWBR
#   define I2C0_HW_CONTROL_REG          TWCR
#   define I2C0_HW_DATA_REG             TWDR
#   define I2C0_HW_STATUS_REG           TWSR

#elif defined (__AVR_ATmega328__) || (__AVR_ATmega168__) || defined (__AVR_ATmega88__) || defined (__AVR_ATmega48__)
#   define I2C0_HW_PORT                 PORTC
#   define I2C0_HW_READBACK             PINC
#   define I2C0_HW_DDR                  DDRC
#   define I2C0_HW_SCL_BIT              5
#   define I2C0_HW_SDA_BIT              4
#   define I2C0_HW_SLAVE_ADDRESS_REG    TWAR
#   define I2C0_HW_SLAVE_ADDRESS_MASK   TWAMR
#   define I2C0_HW_BITRATE_REG          TWBR
#   define I2C0_HW_CONTROL_REG          TWCR
#   define I2C0_HW_DATA_REG             TWDR
#   define I2C0_HW_STATUS_REG           TWSR

#else
#   error "MCU type not known to i2c_hw_twi.h (or no MCU type defined at all)."
#endif


// =============================================================================
// Prepare hardware support for bus timing.
// =============================================================================

#if ((F_CPU/F_I2C0_HW) < 16)
#    define twiDivisor           0
#else
#    define twiDivisor           (((F_CPU/F_I2C0_HW)-16)/2)
#endif

#if (twiDivisor < 256)
#    if (twiDivisor < 10)
#        warning: "Ratio F_CPU/F_I2C0_HW too small, bus speed decreased!"
#        define I2C0_BITRATE     10
#    else
#        define I2C0_BITRATE     twiDivisor
#    endif
#    define I2C0_PRESCALER       0x00 /* :1 */
#elif (twiDivisor < 1021)
#    define I2C0_BITRATE         twiDivisor/4
#    define I2C0_PRESCALER       0x01 /* :4 */
#elif (twiDivisor < 4081)
#    define I2C0_BITRATE         twiDivisor/16
#    define I2C0_PRESCALER       0x02 /* :16 */
#elif (twiDivisor < 16321)
#    define I2C0_BITRATE         twiDivisor/64
#    define I2C0_PRESCALER       0x03 /* :64 */
#else
#    warning: "Ratio F_CPU/F_I2C0_HW too large, prescaling not possible, bus speed increased!"
#    define I2C0_BITRATE         0xFF
#    define I2C0_PRESCALER       0x03 /* :64 */
#endif


// =============================================================================
// Map implementation to standardized API.
// =============================================================================

#define hardI2c0_check4Error twi0_check4Error
enum I2C_FAILURE_type twi0_check4Error(void);

#if defined I2C0_HW_AS_SLAVE
#    if defined I2C0_HW_AS_MASTER
#        if defined I2C0_HW_SINGLE_MASTER
#            // opmode: single master + slave
#            error "Single bus master addressable as slave is void - or by whom else can it be adressed? Check your `I2C0_HW_AS_MASTER`, `I2C0_HW_SINGLE_MASTER` and `I2C0_HW_AS_SLAVE` settings!"
#        else
#            // opmode: multi master + slave
#            ifndef I2C0_HW_NO_SLAVE_ADDRESS_MASK
#                define hardI2c0_init(p1,p2,p3,p4)   twi0_setup_master_slave(p1,p2,p3,p4)
void twi0_setup_master_slave(uint8_t slaveAddress,
                             uint8_t slaveMask,
                             uint8_t bitrate,
                             uint8_t prescaler);
#            else
#                define hardI2c0_init(p1,p2,p3,p4)   twi0_setup_master_slave(p1,p3,p4)
void twi0_setup_master_slave(uint8_t slaveAddress,
                             uint8_t bitrate,
                             uint8_t prescaler);
#            endif
#            define hardI2c0_putByteAsMaster     twi0_send_byte_multi_master
void twi0_send_byte_multi_master(uint8_t dataByte);
#            define hardI2c0_getByteAsMaster     twi0_receive_byte_multi_master
uint8_t twi0_receive_byte_multi_master(uint8_t sendAck);
#            define hardI2c0_openDevice          twi0_open_device_multi_master
enum I2C_FAILURE_type twi0_open_device_multi_master(uint8_t deviceAddress);
#            define hardI2c0_releaseBus          twi0_assert_stop_sequence
void twi0_assert_stop_sequence(void);
#            define hardI2c0_isAddressedAsSlave(p1, p2)  twi0_addressed_as_slave_multi_master(p1)
uint8_t twi0_addressed_as_slave_multi_master(uint8_t *addressReceived);
#            define hardI2c0_putByteAsSlave      twi0_send_byte_slave_multi_master
void twi0_send_byte_slave_multi_master(uint8_t dataByte);
#            define hardI2c0_getByteAsSlave      twi0_receive_byte_slave_multi_master
uint8_t twi0_receive_byte_slave_multi_master(uint8_t sendAck);
#            define hardI2c0_setSlaveResponse    twi0_set_slave_response
void twi0_set_slave_response(uint8_t on);
#        endif
#    else
#        // opmode: slave
#        ifndef I2C0_HW_NO_SLAVE_ADDRESS_MASK
#            define hardI2c0_init(p1,p2,p3,p4)   twi0_setup_slave(p1,p2)
void twi0_setup_slave(uint8_t slaveAddress,
                      uint8_t slaveMask);
#        else
#            define hardI2c0_init(p1,p2,p3,p4)   twi0_setup_slave(p1)
void twi0_setup_slave(uint8_t slaveAddress);
#        endif
#        define hardI2c0_isAddressedAsSlave(p1, p2)  twi0_addressed_as_slave(p1)
uint8_t twi0_addressed_as_slave(uint8_t *addressReceived);
#        define hardI2c0_putByteAsSlave          twi0_send_byte_slave
void twi0_send_byte_slave(uint8_t dataByte, uint8_t expectAck);
#        define hardI2c0_getByteAsSlave          twi0_receive_byte_slave
uint8_t twi0_receive_byte_slave(uint8_t sendAck);
#        define hardI2c0_setSlaveResponse        twi0_set_slave_response
void twi0_set_slave_response(uint8_t on);
#    endif
#else
#    if defined I2C0_HW_AS_MASTER
#        if defined I2C0_HW_SINGLE_MASTER
#            // opmode: single master, no slave
#            define hardI2c0_init(p1,p2,p3,p4)   twi0_setup_master(p3,p4)
void twi0_setup_master(uint8_t bitrate,
                       uint8_t prescaler);
#            define hardI2c0_putByteAsMaster     twi0_send_byte_single_master
void twi0_send_byte_single_master(uint8_t dataByte);
#            define hardI2c0_getByteAsMaster     twi0_receive_byte_single_master
uint8_t twi0_receive_byte_single_master(uint8_t sendAck);
#            define hardI2c0_openDevice          twi0_open_device_single_master
enum I2C_FAILURE_type twi0_open_device_single_master(uint8_t deviceAddress);
#            define hardI2c0_releaseBus          twi0_assert_stop_sequence
void twi0_assert_stop_sequence(void);
#        else
#            // opmode: multi master, no slave
#            define hardI2c0_init(p1,p2,p3,p4)   twi0_setup_master(p3,p4)
void twi0_setup_master(uint8_t bitrate,
                       uint8_t prescaler);
#            define hardI2c0_putByteAsMaster     twi0_send_byte_multi_master
void twi0_send_byte_multi_master(uint8_t dataByte);
#            define hardI2c0_getByteAsMaster     twi0_receive_byte_multi_master
uint8_t twi0_receive_byte_multi_master(uint8_t sendAck);
#            define hardI2c0_openDevice          twi0_open_device_multi_master
enum I2C_FAILURE_type twi0_open_device_multi_master(uint8_t deviceAddress);
#            define hardI2c0_releaseBus          twi0_assert_stop_sequence
void twi0_assert_stop_sequence(void);
#        endif
#    else
#        error "Neither master nor slave mode selected for I2C (TWI). Check your `I2C0_HW_AS_MASTER` and `I2C0_HW_AS_SLAVE` settings!"
#    endif
#endif


#endif // I2C_HW_TWI_H_INCLUDED


////////////////////////////////////////////////////////////////////////////////
//  Revision history:
//  -----------------
//
//  $Id:$
//
//  $Log:$
//
////////////////////////////////////////////////////////////////////////////////
