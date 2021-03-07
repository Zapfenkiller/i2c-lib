////////////////////////////////////////////////////////////////////////////////
// File        : twi0_receive_byte_slave_multi_master.c
// Description : Receive one byte in slave mode - TWI.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: Aug /  9 / 2012
// Copyright   : © 2006-2021 R. Trapp / H.A.R.R.Y.
// License     : GNU General Public License V3 (GPLv3)
//               See project root LICENSE file or
//               https://www.gnu.org/licenses/gpl-3.0.html
////////////////////////////////////////////////////////////////////////////////
//  --- Doxygen ---
/// \file   twi0_receive_byte_slave_multi_master.c
/// \brief  Byte reception of multi master being accessed as slave.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_TWI_H_INCLUDED) && defined(I2C0_HW_AS_SLAVE) && defined (I2C0_HW_AS_MASTER) || defined DOXYGEN_DOCU_IS_GENERATED
#include "i2c_lib_private.h"
#include <avr/io.h>

/// \brief
/// Receive one byte as a slave device.
/// \details
/// Receive one byte from the master when addressed as slave device.
/// Dedicated to TWI equipped devices.
///
/// \todo
/// The slave is expected to always send an 'ACK' answer.
/// So why is there a parameter to decide if ACK is sent back?
///
/// According to the bus protocol the master expects an 'ACK'
/// on any byte sent to a slave device.
/// \param transferFollows sends an 'ACK' to the bus if set (!=0). ??
/// \returns Byte read from bus.
uint8_t twi0_receive_byte_slave_multi_master(uint8_t transferFollows)
{
    // Start reception; select ACK option.
    if (transferFollows)
        I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
    else
        I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT);
    // Wait until finished.
    while (!(I2C0_HW_CONTROL_REG & (1 << TWINT))) {}
    uint8_t dataByte = I2C0_HW_DATA_REG;
    switch (I2C0_HW_STATUS_REG & (0b11111<<TWS3))
    {
        case 0xA0:  /* STOP or repeated START condition */
            i2c0_failure_info = I2C_STOPPED;
            break;
        case 0x80:  /* addressed, byte received and ACKed */
        case 0x88:  /* addressed, byte received and not ACKed */
        case 0x90:  /* general call, byte received and ACKed */
        case 0x98:  /* general call, byte received and not ACKed */
            i2c0_failure_info = I2C_SUCCESS;
            break;
        case 0x00:
            I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | (1 << TWSTO) | slaveAckControl;
        default:
            i2c0_failure_info = I2C_PROTOCOL_FAIL;
    }
    return(dataByte);
}


#endif /* defined(I2C_HW_TWI_H_INCLUDED) && defined(I2C0_HW_AS_SLAVE) && defined (I2C0_HW_AS_MASTER) */
