////////////////////////////////////////////////////////////////////////////////
// File        : twi0_open_device_single_master.c
// Description : Address one slave using single master mode - TWI.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: Aug /  9 / 2012
// Copyright   : © 2006-2021 R. Trapp / H.A.R.R.Y.
// License     : GNU General Public License V3 (GPLv3)
//               See project root LICENSE file or
//               https://www.gnu.org/licenses/gpl-3.0.html
////////////////////////////////////////////////////////////////////////////////
//  --- Doxygen ---
/// \file   twi0_open_device_single_master.c
/// \brief  Start accessing a certain slave in the single master mode.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_TWI_H_INCLUDED) && !defined(I2C0_HW_AS_SLAVE) && defined(I2C0_HW_AS_MASTER) && defined(I2C0_HW_SINGLE_MASTER) || defined DOXYGEN_DOCU_IS_GENERATED
#include "i2c_lib_private.h"
#include <avr/io.h>

/// \brief
/// Start access to a certain slave. Valid for a single bus master.
/// \details
/// Asserts a start sequence to the bus, followed by the slave
/// address.
/// Dedicated to TWI equipped devices.
/// \param deviceAddress defines which slave to access. It shall
/// define the READ or WRITE mode also.
/// \returns failure code according to `I2C_FAILURE_type`.
/// See `i2c_def.h`
enum I2C_FAILURE_type twi0_open_device_single_master(uint8_t deviceAddress)
{
    // Clear previous failures.
    i2c0_failure_info = I2C_SUCCESS;
    // Assert start condition.
    I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | (1 << TWSTA) | slaveAckControl;
    // Wait until done.
    while (!(I2C0_HW_CONTROL_REG & (1 << TWINT))) {}
    twi0_send_byte(deviceAddress);
    switch (I2C0_HW_STATUS_REG & (0b11111<<TWS3))
    {
        case 0x18:  /* address + W successfully sent */
        case 0x40:  /* address + R successfully sent */
            break;
        case 0x20:  /* address + W sent, but no ACK */
        case 0x48:  /* address + R sent, but no ACK */
            i2c0_failure_info |= I2C_NO_ACK;
            break;
        default:
            i2c0_failure_info |= I2C_PROTOCOL_FAIL;
    }
    return(i2c0_failure_info);
}


#endif /* defined(I2C_HW_TWI_H_INCLUDED) && !defined(I2C0_HW_AS_SLAVE) && defined(I2C0_HW_AS_MASTER) && defined(I2C0_HW_SINGLE_MASTER) */
