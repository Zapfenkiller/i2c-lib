////////////////////////////////////////////////////////////////////////////////
// File        : usi0_open_device_single_master.c
// Description : Address one slave using single master mode - USI.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: Oct / 25 / 2012
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
/// \file   usi0_open_device_single_master.c
/// \brief  Start accessing a certain slave in the single master mode.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_USI_H_INCLUDED) && !defined(I2C0_HW_AS_SLAVE) && defined(I2C0_HW_AS_MASTER) && defined(I2C0_HW_SINGLE_MASTER) || defined DOXYGEN_DOCU_IS_GENERATED
#include <avr/io.h>
#include <util/delay.h>


/// \brief
/// Start access to a certain slave. Valid for a single bus master.
/// \details
/// Asserts a start sequence to the bus, followed by the slave
/// address.
/// Dedicated to USI equipped devices.
/// \param deviceAddress defines which slave to access. It shall
/// define the READ or WRITE mode also.
/// \returns failure code according to `I2C_FAILURE_type`.
/// See `i2c_def.h`
// Check if the bus is available.
// Address a certain slave.
// Return its status.
// This also occupies the bus.
/// \todo Why is there some check on ARBITRATION LOSS in the code?
/// \todo what about clock stretching? (Prepared the possible solution)
/// Check the usi0_open_device_multi_master() for reference
enum I2C_FAILURE_type usi0_open_device_single_master(uint8_t deviceAddress)
{
    // Prepare SCL for a (RE)START sequence.
    USI0_SDA_DRIVER_DISABLE;
    USI0_HALFBIT_DELAY;
    USI0_RELEASE_SCL;
//  while(I2C0_SCL_IS_LOW)
//  { // wait for SCL to rise - slow devices block SCL as wait condition
//    // exception: USISIF indicates that a start condition already occured
//    // then SCL is blocked by our own 'START condition blocks SCL' circuit!
//      if (I2C0_HW_STATUS_REG & (1 << USISIF))
//          break;
//  }
    USI0_HALFBIT_DELAY;
    // ===> Check that no other master gave a START sequence meanwhile.
    if (!(I2C0_HW_STATUS_REG & (1 << USISIF)))
    {
        // Assert start condition.
        USI0_PULL_SDA;
        USI0_SDA_DRIVER_ENABLE;
        USI0_HALFBIT_DELAY;
        USI0_PULL_SCL;
        // IMPORTANT:
        // Start condition detector needs ample reaction time (50ns..300ns).
        // Clearing the USISIF without delay might take place BEFORE the
        // start condition detector sets it - we have a deadlock then since
        // USISIF set will block the SCL in its low state. The master clock
        // generator then waits on this since it thinks a slave is stretching
        // this cycle! The 1us delay ensures proper reaction time even at 20MHz.
        _delay_us(1);
        USI0_RELEASE_SDA;
        // Prepare hardware operation mode:
        I2C0_HW_CONTROL_REG = USI_HOLD_ON_START | USI_SHIFT_ON_SOFTWARE_COMMAND;
        // Clear previous failures.
        i2c0_failure_info = I2C_SUCCESS;
        // Send out device address field.
        // Clear START just generated but also STOP from previous bus cycles.
        I2C0_HW_STATUS_REG = (1 << USISIF) | (1 << USIPF);
        usi0_send_byte_single_master(deviceAddress);
    }
    else
    {
        i2c0_failure_info = I2C_ARBITRATION_LOST;
    }
    return(i2c0_failure_info);
}


#endif /* defined(I2C_HW_USI_H_INCLUDED) && !defined(I2C0_HW_AS_SLAVE) && defined(I2C0_HW_AS_MASTER) && defined(I2C0_HW_SINGLE_MASTER) */
