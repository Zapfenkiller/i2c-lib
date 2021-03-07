////////////////////////////////////////////////////////////////////////////////
// File        : twi0_send_byte_slave_multi_master.c
// Description : Send one byte in slave mode.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: Aug /  9 / 2012
// Copyright   : © 2006-2021 R. Trapp / H.A.R.R.Y.
// License     : GNU General Public License V3 (GPLv3)
//               See project root LICENSE file or
//               https://www.gnu.org/licenses/gpl-3.0.html
////////////////////////////////////////////////////////////////////////////////
//  --- Doxygen ---
/// \file   twi0_send_byte_slave_multi_master.c
/// \brief  Byte submission when capable master is addressed as slave.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_TWI_H_INCLUDED) && defined(I2C0_HW_AS_SLAVE) && defined(I2C0_HW_AS_MASTER) || defined DOXYGEN_DOCU_IS_GENERATED
#include "i2c_lib_private.h"
#include <avr/io.h>

/// \brief
/// Send one byte to the bus master.
/// \details
/// When addressed as slave one byte is returned to the master.
/// Dedicated to TWI equipped devices.
/// \todo See twi0_send_byte_slave() and the `expectAck` topic.
/// \param dataByte contains the byte to send to the master.
void twi0_send_byte_slave_multi_master(uint8_t dataByte)
{
    twi0_send_byte(dataByte);
    switch (I2C0_HW_STATUS_REG & (0b11111<<TWS3))
    {
        case 0xB8:  /* byte sent, ACK received */
            i2c0_failure_info = I2C_SUCCESS;
            break;
        case 0xC0:  /* byte sent, no ACK received */
        case 0xC8:  /* last byte sent, but ACK received */
            i2c0_failure_info = I2C_STOPPED;
            break;
        case 0x00:
            I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | (1 << TWSTO) | slaveAckControl;
        default:
            i2c0_failure_info = I2C_PROTOCOL_FAIL;
    }
}


#endif /* defined(I2C_HW_TWI_H_INCLUDED) && defined(I2C0_HW_AS_SLAVE) && defined(I2C0_HW_AS_MASTER) */
