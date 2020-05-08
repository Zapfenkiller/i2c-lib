////////////////////////////////////////////////////////////////////////////////
// File        : i2c_hw_usi.h
// Description : Private definitions of USI library routines.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: Sep / 18 / 2012
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
/// \file   i2c_hw_usi.h
/// \brief
/// A collection of useful things internal to the I<SUP>2</SUP>C library's USI
/// section.
/// \details
/// Based on an older version of 2006 the I<SUP>2</SUP>C library had been rewritten.
/// This file contains definitions for ease of I<SUP>2</SUP>C library subroutines
/// programming, usage and code readability when handling USI hardware.
///
/// This header file is needed for the library routines to compile but not
/// included by the user application when suing the library.
////////////////////////////////////////////////////////////////////////////////


#ifndef I2C_HW_USI_H_INCLUDED
#define I2C_HW_USI_H_INCLUDED


#include "i2c_def.h"
#include <inttypes.h>


/// \brief
/// Waits half of the desired bit time to achieve bus frequency with USI.
#define USI0_HALFBIT_DELAY              _delay_us(500000UL/F_I2C0_HW);


/// \brief
/// Aid for coding USI (TWI) routines.
#define I2C0_SLAVE_NEEDS_TIME           (!(I2C0_HW_READBACK & (1 << I2C0_HW_SCL_BIT)))


/// \brief
/// Aid for coding USI (TWI) routines.
#define I2C0_SCL_IS_LOW                 (!(I2C0_HW_READBACK & (1 << I2C0_HW_SCL_BIT)))


/// \brief
/// Aid for coding USI (TWI) routines.
#define I2C0_SCL_IS_HIGH                (I2C0_HW_READBACK & (1 << I2C0_HW_SCL_BIT))


/// \brief
/// Aid for coding USI (TWI) routines.
#define I2C0_SDA_IS_LOW                 (!(I2C0_HW_READBACK & (1 << I2C0_HW_SDA_BIT)))


/// \brief
/// Aid for coding USI (TWI) routines.
#define I2C0_SCL_AND_SDA_ARE_HIGH       ((I2C0_HW_READBACK & ((1 << I2C0_HW_SCL_BIT) | (1 << I2C0_HW_SDA_BIT))) == ((1 << I2C0_HW_SCL_BIT) | (1 << I2C0_HW_SDA_BIT)))


/// \brief
/// Aid for coding USI routines.
/// \details
/// The shifter action on the SDA line is controlled by software.
/// This mode is for the bus master, regardless of the data direction (read /
/// write).
/// The shifter will sample the SDA line when clocked.
/// Receiving data needs the SDA driver to be disabled.
/// To transmit the SDA driver must be enabled while its port state is set to
/// recessive.
/// In this operation mode the output latch is always transparent.
/// To ensure proper timing the software has to clock the shifter and also
/// handle the SCL line.
#define USI_SHIFT_ON_SOFTWARE_COMMAND   (0b00 << USICS0)


/// \brief
/// Aid for coding USI routines.
/// \details
/// The shifter action on the SDA line is controlled by SCL.
/// This mode is for receiving data from the bus.
/// The shifter will sample the SDA line at every falling edge of SCL.
/// Receiving data needs the SDA driver to be disabled, of course.
// To transmit the SDA driver must be enabled while its port state is set to
// recessive.
// In this operation mode the output latch is locked while SCL is low,
// it is transparent while SCL is high. So output data will be changed on
// the external clock's rising edge which violates the bus specification..
#define USI_SAMPLE_ON_FALLING_EDGE      (0b11 << USICS0)


/// \brief
/// Aid for coding USI routines.
/// \details
/// The shifter action on the SDA line is controlled by SCL.
/// This mode is for transmitting data to the bus.
/// To transmit the SDA driver must be enabled while its port state is set to
/// recessive.
/// In this operation mode the output latch is locked while SCL is high,
/// it is transparent while SCL is low. So output data will be changed on
/// the external clocks falling edge which matches the bus specification.
// The shifter will sample the SDA line at every rising edge of SCL.
// Receiving data needs the SDA driver to be disabled but will not work here
// reliably with respect to the bus specification. Although in most cases
// a valid read is possbile it might fail with certain bus slaves.
#define USI_SAMPLE_ON_RISING_EDGE       (0b10 << USICS0)


/// \brief
/// Aid for coding USI routines.
#define USI_MODE_OFF                    (0b00 << USIWM0)


/// \brief
/// Aid for coding USI routines.
#define USI_HOLD_ON_START               (0b10 << USIWM0)


/// \brief
/// Aid for coding USI routines.
#define USI_HOLD_ON_ALL                 (0b11 << USIWM0)


/// \brief
/// Aid for coding USI routines.
/// \details
/// Enable SDA to drive the bus to dominant or recessive state.
#define USI0_SDA_DRIVER_ENABLE          I2C0_HW_DDR |= (1 << I2C0_HW_SDA_BIT)


/// \brief
/// Aid for coding USI routines.
/// \details
/// Prevent SDA from driving the bus to dominant state.
/// SDA keeps recessive regardingless of the USIDR's MSB or its port register state.
#define USI0_SDA_DRIVER_DISABLE         I2C0_HW_DDR &= ~(1 << I2C0_HW_SDA_BIT)


/// \brief
/// Aid for coding USI routines.
/// \details
/// Pull SCL to low state - dominant.
#define USI0_PULL_SCL                   I2C0_HW_PORT &= ~(1 << I2C0_HW_SCL_BIT)


/// \brief
/// Aid for coding USI routines.
/// \details
/// Release SCL to high state - recessive.
#define USI0_RELEASE_SCL                I2C0_HW_PORT |= (1 << I2C0_HW_SCL_BIT)


/// \brief
/// Aid for coding USI routines.
/// \details
/// Pull SDA to low state - dominant.
#define USI0_PULL_SDA                   I2C0_HW_PORT &= ~(1 << I2C0_HW_SDA_BIT)


/// \brief
/// Aid for coding USI routines.
/// \details
/// Release SDA to high state - recessive.
#define USI0_RELEASE_SDA                I2C0_HW_PORT |= (1 << I2C0_HW_SDA_BIT)


/// \brief
/// Aid for coding USI routines.
/// \details
/// clock the shifter once when in master mode.
#define   USI0_CLOCK_SHIFTER_ONCE       I2C0_HW_CONTROL_REG |= (1 << USICLK);


/// \brief
/// Aid for coding USI routines.
/// \details
/// USICNT[3:0] to transfer one complete byte in slave mode.
/// See also ::usi0_wait_until_transfer_done_as_slave().
#define USI_COUNT_FOR_A_BYTE_SLAVE      0


/// \brief
/// Aid for coding USI routines.
/// \details
/// USICNT[3:0] to transfer one bit in slave mode.
/// See also ::usi0_wait_until_transfer_done_as_slave().
#define USI_COUNT_FOR_A_BIT_SLAVE       14


/// \brief
/// Aid for coding USI routines.
/// \details
/// USICNT[3:0] to transfer one complete byte in master mode.
#define USI_COUNT_FOR_A_BYTE_MASTER     8


/// \brief
/// Aid for coding USI routines.
/// \details
/// USICNT[3:0] to transfer one bit in master mode.
#define USI_COUNT_FOR_A_BIT_MASTER      15


// \brief
// Wait for transfer to finish.
// \details
// While in slave mode the slave has to wait until a certain count of bits is
// transferred.
// This transfer is fully controlled by the master, so there is a chance the master
// aborts the byte.
// The handling of this abortion is also done.
// Since this is a central functionality needed several times in checking for
// addressfield (is the slave addressed?) and subsequent data read/write cycles
// it makes sense to have this coded as helper function.
//
// \param countOfEdges needs to be set in accordance to the amount of bits to
// be transferred.
// <TABLE>
// <TR>
// <TD><b>Count of bits</b></TD>
// <TD><b>1</b></TD>
// <TD>2</TD>
// <TD>3</TD>
// <TD>4</TD>
// <TD>5</TD>
// <TD>6</TD>
// <TD>7</TD>
// <TD><b>8</b></TD>
// </TR>
// <TR>
// <TD>Preset to USICNT[3:0]\n(<b>countOfEdges</b>)</TD>
// <TD><b>14</b></TD>
// <TD>12</TD>
// <TD>10</TD>
// <TD>8</TD>
// <TD>6</TD>
// <TD>4</TD>
// <TD>2</TD>
// <TD><b>0</b></TD>
// </TR>
// </TABLE>
// See also\n
// ::USI_COUNT_FOR_A_BYTE_SLAVE or\n
// ::USI_COUNT_FOR_A_BIT_SLAVE\n
// for useful predefinitions.
uint8_t usi0_wait_until_transfer_done_as_slave(uint8_t countOfEdges);


// \brief
// Control of bit to send or receive.
// \details
// While in (multi) master mode the master has to control SCL for a bit to get
// sent or received. It also has to check for bus contention. If contention is
// detected the master retires from the bus with one of those failure codes:
// I2C_PROTOCOL_FAIL | I2C_RESTARTED if a START condition is detected
// I2C_PROTOCOL_FAIL | I2C_STOPPED if a STOP condition is detected
// i2c0_failure_info is updated accordingly.
void usi0_wait_until_bit_done_as_multimaster(void);


// =============================================================================
// All AVR devices with USI receive their hardware definitions here.
// =============================================================================

#if defined (__AVR_ATtiny2313__)
#   define I2C0_HW_PORT                 PORTB
#   define I2C0_HW_READBACK             PINB
#   define I2C0_HW_DDR                  DDRB
#   define I2C0_HW_SCL_BIT              7
#   define I2C0_HW_SDA_BIT              5
#   define I2C0_HW_CONTROL_REG          USICR
#   define I2C0_HW_DATA_REG             USIDR
#   define I2C0_HW_STATUS_REG           USISR

#elif defined (__AVR_ATtiny26__)
#   define I2C0_HW_PORT                 PORTB
#   define I2C0_HW_READBACK             PINB
#   define I2C0_HW_DDR                  DDRB
#   define I2C0_HW_SCL_BIT              2
#   define I2C0_HW_SDA_BIT              0
#   define I2C0_HW_CONTROL_REG          USICR
#   define I2C0_HW_DATA_REG             USIDR
#   define I2C0_HW_STATUS_REG           USISR

#elif defined (__AVR_ATmega169__)
#   define I2C0_HW_PORT                 PORTE
#   define I2C0_HW_READBACK             PINE
#   define I2C0_HW_DDR                  DDRE
#   define I2C0_HW_SCL_BIT              4
#   define I2C0_HW_SDA_BIT              5
#   define I2C0_HW_CONTROL_REG          USICR
#   define I2C0_HW_DATA_REG             USIDR
#   define I2C0_HW_STATUS_REG           USISR

#else
#   error "MCU type not known to i2c_hw_usi.h (or no MCU type defined at all)."
#endif


// =============================================================================
// Map implementation to standardized API.
// =============================================================================

#define hardI2c0_check4Error usi0_check4Error
enum I2C_FAILURE_type usi0_check4Error(void);


#if defined I2C0_HW_AS_SLAVE
#    if defined I2C0_HW_AS_MASTER
#        if defined I2C0_HW_SINGLE_MASTER
#            // opmode: single master + slave
#            error "Single bus master addressable as slave is void - or by whom else can it be adressed?"
#        else
#            // opmode: multi master + slave
#           define hardI2c0_init(p1,p2,p3,p4)    usi0_setup_master_slave()
void usi0_setup_master_slave(void);
#           define hardI2c0_putByteAsMaster      usi0_send_byte_multi_master
void usi0_send_byte_multi_master(uint8_t dataByte);
#           define hardI2c0_getByteAsMaster      usi0_receive_byte_any_master
uint8_t usi0_receive_byte_any_master(uint8_t sendAck);
#           define hardI2c0_openDevice           usi0_open_device_multi_master
enum I2C_FAILURE_type usi0_open_device_multi_master(uint8_t deviceAddress);
#           define hardI2c0_releaseBus           usi0_assert_stop_sequence
void usi0_assert_stop_sequence(void);
#           define hardI2c0_isAddressedAsSlave(p1, p2)  usi0_addressed_as_slave_multi_master(p1, p2)
uint8_t usi0_addressed_as_slave_multi_master(uint8_t *addressReceived, uint8_t slaveMask);
#           
uint8_t usi0_check_4_slave_address_match_multi_master(uint8_t *addressReceived, uint8_t slaveMask);
#           define hardI2c0_putByteAsSlave       usi0_send_byte_slave
void usi0_send_byte_slave(uint8_t dataByte);
#           define hardI2c0_getByteAsSlave       usi0_receive_byte_slave
uint8_t usi0_receive_byte_slave(uint8_t sendAck);
#           define hardI2c0_setSlaveResponse     usi0_set_slave_response
void usi0_set_slave_response(uint8_t on);
#        endif
#    else
#        // opmode: slave
#       define hardI2c0_init(p1,p2,p3,p4)        usi0_setup_master_slave()
void usi0_setup_master_slave(void);
#       define hardI2c0_isAddressedAsSlave(p1, p2)   usi0_addressed_as_slave(p1, p2)
uint8_t usi0_addressed_as_slave(uint8_t *addressReceived, uint8_t slaveMask);
#       define hardI2c0_putByteAsSlave           usi0_send_byte_slave
void usi0_send_byte_slave(uint8_t dataByte);
#       define hardI2c0_getByteAsSlave           usi0_receive_byte_slave
uint8_t usi0_receive_byte_slave(uint8_t sendAck);
#       define hardI2c0_setSlaveResponse         usi0_set_slave_response
void usi0_set_slave_response(uint8_t on);
#    endif
#else
#    if defined I2C0_HW_AS_MASTER
#        if defined I2C0_HW_SINGLE_MASTER
#            // opmode: single master, no slave
#           define hardI2c0_init(p1,p2,p3,p4)    usi0_setup_single_master()
void usi0_setup_single_master(void);
#           define hardI2c0_putByteAsMaster      usi0_send_byte_single_master
void usi0_send_byte_single_master(uint8_t dataByte);
#           define hardI2c0_getByteAsMaster      usi0_receive_byte_any_master
uint8_t usi0_receive_byte_any_master(uint8_t sendAck);
#           define hardI2c0_openDevice           usi0_open_device_single_master
enum I2C_FAILURE_type usi0_open_device_single_master(uint8_t deviceAddress);
#           define hardI2c0_releaseBus           usi0_assert_stop_sequence
void usi0_assert_stop_sequence(void);
#        else
#            // opmode: multi master, no slave
#           define hardI2c0_init(p1,p2,p3,p4)    usi0_setup_master()
void usi0_setup_master(void);
#           define hardI2c0_putByteAsMaster      usi0_send_byte_multi_master
void usi0_send_byte_multi_master(uint8_t dataByte);
#           define hardI2c0_getByteAsMaster      usi0_receive_byte_any_master
uint8_t usi0_receive_byte_any_master(uint8_t sendAck);
#           define hardI2c0_openDevice           usi0_open_device_multi_master
enum I2C_FAILURE_type usi0_open_device_multi_master(uint8_t deviceAddress);
#           define hardI2c0_releaseBus           usi0_assert_stop_sequence
void usi0_assert_stop_sequence(void);
#        endif
#    else
#        error "Neither master nor slave mode selected for I2C (TWI)."
#    endif
#endif


#endif // I2C_HW_USI_H_INCLUDED


////////////////////////////////////////////////////////////////////////////////
//  Revision history:
//  -----------------
//
//  $Id:$
//
//  $Log:$
//
////////////////////////////////////////////////////////////////////////////////
