////////////////////////////////////////////////////////////////////////////////
// File        : twi0_receive_byte_slave.c
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
/// \file   twi0_receive_byte_slave.c
/// \brief  Byte reception of slave mode.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_TWI_H_INCLUDED) && defined(I2C0_HW_AS_SLAVE) && !defined(I2C0_HW_AS_MASTER) || defined DOXYGEN_DOCU_IS_GENERATED
#include <avr/io.h>

/// \brief
/// Receive one byte as a slave device.
/// \details
/// Receive one byte from the master when addressed as slave device.
/// Dedicated to TWI equipped devices.
///
/// \todo
/// The slave is expected to always send an 'ACK' answer. ??
/// So why is there a parameter to decide if ACK is sent back?
/// Nope.
/// Rework this completely, we similar places already. See there
/// how to write it.
/// \param sendAck sends an 'ACK' to the bus if set (!=0).
/// \returns Byte read from bus.
uint8_t twi0_receive_byte_slave(uint8_t sendAck)
{
    // Start reception; select ACK option.
    if (sendAck)
        I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
    else
        I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT);
    // Wait until finished.
    while (!(I2C0_HW_CONTROL_REG & (1 << TWINT))) {}
    uint8_t dataByte = I2C0_HW_DATA_REG;
    switch (I2C0_HW_STATUS_REG & (0b11111<<TWS3))
    {
        case 0xA0:  /* STOP or repeated START condition */
            i2c0_failure_info |= I2C_STOPPED;
        case 0x88:  /* addressed, last byte received not ACKed */
        case 0x98:  /* general call, last byte received not ACKed */
            I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | slaveAckControl;
        case 0x80:  /* addressed, last byte received with ACK */
        case 0x90:  /* general call, last byte received with ACK */
            break;
        case 0x00:
            I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | (1 << TWSTO) | slaveAckControl;
        default:
            i2c0_failure_info |= I2C_PROTOCOL_FAIL;
    }
    return(dataByte);
}


#endif /* defined(I2C_HW_TWI_H_INCLUDED) && defined(I2C0_HW_AS_SLAVE) && !defined(I2C0_HW_AS_MASTER) */
