////////////////////////////////////////////////////////////////////////////////
// File        : usi0_open_device_multi_master.c
// Description : Address one slave using multi master mode - USI.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: Sep / 18 / 2012
// Copyright   : © 2006-2021 R. Trapp / H.A.R.R.Y.
// License     : GNU General Public License V3 (GPLv3)
//               See project root LICENSE file or
//               https://www.gnu.org/licenses/gpl-3.0.html
////////////////////////////////////////////////////////////////////////////////
//  --- Doxygen ---
/// \file   usi0_open_device_multi_master.c
/// \brief  Start accessing a certain slave in a multi master mode.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_USI_H_INCLUDED) && defined(I2C0_HW_AS_MASTER) && !defined(I2C0_HW_SINGLE_MASTER) || defined DOXYGEN_DOCU_IS_GENERATED
#include "i2c_lib_private.h"
#include <avr/io.h>
#include <util/delay.h>

/// \brief
/// Start access to a certain slave. Valid for any multi master bus
/// mode.
/// \details
/// Asserts a start sequence to the bus, followed by the slave
/// address. Keeps track of the various states that could arise
/// from the slave / bus reaction.
/// Dedicated to USI equipped devices.
/// \param deviceAddress defines which slave to access. It shall
/// define the READ or WRITE mode also.
/// \returns failure code according to `I2C_FAILURE_type`.
/// See `i2c_def.h`
// Prepare for (re)start sequence.
// Check if the bus is available.
// Address a certain slave.
// Return its status.
// This also occupies the bus.
enum I2C_FAILURE_type usi0_open_device_multi_master(uint8_t deviceAddress)
{
    if (I2C0_HW_STATUS_REG & (1 << USIPF))
    {
        i2c0_failure_info &= ~(I2C_RESTARTED | I2C_ARBITRATION_LOST);
        I2C0_HW_STATUS_REG = (1 << USIPF);
    }
    if (i2c0_failure_info & (I2C_RESTARTED | I2C_ARBITRATION_LOST))
        return(i2c0_failure_info);
    // Prepare SCL for a (RE)START sequence.
    USI0_SDA_DRIVER_DISABLE;
    USI0_HALFBIT_DELAY;
    I2C0_HW_CONTROL_REG = USI_HOLD_ON_START | USI_SAMPLE_ON_FALLING_EDGE;
    USI0_RELEASE_SCL;
    while(I2C0_SCL_IS_LOW)
    { // wait for SCL to rise - slow devices block SCL as wait condition
      // exception: USISIF indicates that a start condition already occured
      // then SCL is blocked by our own 'START condition blocks SCL' circuit!
        if (I2C0_HW_STATUS_REG & (1 << USISIF))
            break;
    }
    USI0_HALFBIT_DELAY;
    USI0_PULL_SDA;
    USI0_SDA_DRIVER_ENABLE;
    USI0_HALFBIT_DELAY;
    USI0_PULL_SCL;
    i2c0_failure_info = I2C_RESTARTED;
    usi0_send_byte_multi_master(deviceAddress);
    switch (i2c0_failure_info)
    {
        case I2C_SUCCESS:
        case I2C_SUCCESS | I2C_NO_ACK:
        case I2C_ARBITRATION_LOST | I2C_RESTARTED:
            break;
        default:
            I2C0_HW_CONTROL_REG = USI_HOLD_ON_START | USI_SAMPLE_ON_FALLING_EDGE;
            I2C0_HW_STATUS_REG = (1 << USISIF);
            USI0_RELEASE_SCL;
    }
    return(i2c0_failure_info);
}


#endif /* defined(I2C_HW_USI_H_INCLUDED) && defined(I2C0_HW_AS_MASTER) && !defined(I2C0_HW_SINGLE_MASTER) */
