////////////////////////////////////////////////////////////////////////////////
// File        : twi0_addressed_as_slave.c
// Description : Check if device is addressed as slave. Report address received.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: Aug /  9 / 2012
// Copyright   : © 2006-2021 R. Trapp / H.A.R.R.Y.
// License     : GNU General Public License V3 (GPLv3)
//               See project root LICENSE file or
//               https://www.gnu.org/licenses/gpl-3.0.html
////////////////////////////////////////////////////////////////////////////////
//  --- Doxygen ---
/// \file   twi0_addressed_as_slave.c
/// \brief  TWI slave address scan - slave only mode
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined (I2C_HW_TWI_H_INCLUDED) && defined (I2C0_HW_AS_SLAVE) && !defined (I2C0_HW_AS_MASTER) || defined DOXYGEN_DOCU_IS_GENERATED
#include "i2c_lib_private.h"
#include <avr/io.h>

/// \brief
/// Check if addressed as a slave device when just being a slave.
/// \details
/// Dedicated to TWI equipped devices.
/// \param addressReceived contains the assigned (base) address of
/// slave response(s). Used to report the complete address received
/// in case the slave responds to an address range.
/// \returns selection state, ~0 if selected, 0 if not.
uint8_t twi0_addressed_as_slave (uint8_t* addressReceived)
{
    i2c0_failure_info = I2C_SUCCESS;
    switch (I2C0_HW_STATUS_REG & (0b11111<<TWS3))
    {
        case 0xF8:  /* TWI idle or in transfer */
            return(0);
        case 0x60:  /* own address + W received */
        case 0x70:  /* general call address received */
        case 0xA8:  /* own address + R received */
            *addressReceived = I2C0_HW_DATA_REG;
            return(~0);
            break;
        case 0x00:
            I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | (1 << TWSTO) | slaveAckControl;
        default:
            i2c0_failure_info |= I2C_PROTOCOL_FAIL;
    }
    return(0);
}


#endif /* defined (I2C_HW_TWI_H_INCLUDED) && defined (I2C0_HW_AS_SLAVE) && !defined (I2C0_HW_AS_MASTER) */
