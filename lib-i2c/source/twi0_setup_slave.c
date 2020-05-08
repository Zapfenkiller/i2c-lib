////////////////////////////////////////////////////////////////////////////////
// File        : twi0_setup_slave.c
// Description : TWI hardware setup for slave only operation.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: Aug /  9 / 2012
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
/// \file   twi0_setup_slave.c
/// \brief  Initializes just slave response.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_TWI_H_INCLUDED) && defined(I2C0_HW_AS_SLAVE) && !defined(I2C0_HW_AS_MASTER) || defined DOXYGEN_DOCU_IS_GENERATED
#include "i2c_lib_private.h"
#include <avr/io.h>

/// \brief
/// Initializes slave response.
/// \details
/// The hardware is set up for the predefined operation mode and
/// parameters.
/// Dedicated to TWI equipped devices.
///
/// \param slaveAddress defines the address to respond to when
/// another master accesses slave devices.
/// If bit 0 is set the slave also answers to the 'general call'
/// address (0).
/// \param slaveMask excludes all bits *set* as invalid for the
/// address comparison. `slaveMask` is omitted on devices without
/// adequate hardware support (e. g. ATmega8). Intentionally this
/// difference will lead to compile issues if the application code
/// gets moved between different TWI revisions (e. g. ATmega8 <=>
/// ATmega168). Using the `slaveMask` the response spans more than
/// just one single slave address.
/// \todo Check if this can get combined with the master+slave
/// init.
// Initialize the IO-lines and the bus.
// Bit0 of 'slaveAddress' defines if answer to general call is enabled!
#ifndef I2C0_HW_NO_SLAVE_ADDRESS_MASK
void twi0_setup_slave(uint8_t slaveAddress, uint8_t slaveMask)
#else
void twi0_setup_slave(uint8_t slaveAddress)
#endif
{
    I2C0_HW_SLAVE_ADDRESS_REG = slaveAddress;
#ifndef I2C0_HW_NO_SLAVE_ADDRESS_MASK
    I2C0_HW_SLAVE_ADDRESS_MASK = slaveMask;
#endif
    slaveAckControl = (1 << TWEA);
    I2C0_ENABLE;
}


#endif /* defined(I2C_HW_TWI_H_INCLUDED) && defined(I2C0_HW_AS_SLAVE) && !defined(I2C0_HW_AS_MASTER) */
