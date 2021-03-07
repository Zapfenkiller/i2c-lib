////////////////////////////////////////////////////////////////////////////////
// File        : twi0_send_byte_slave.c
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
/// \file   twi0_send_byte_slave.c
/// \brief  Byte submission in slave mode.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_TWI_H_INCLUDED) && defined(I2C0_HW_AS_SLAVE) && !defined(I2C0_HW_AS_MASTER) || defined DOXYGEN_DOCU_IS_GENERATED
#include <avr/io.h>

/// \brief
/// Send one byte to the bus master.
/// \details
/// When addressed as slave one byte is returned to the master.
/// The predefined amount of bytes is checked and in case of
/// misalignements an error code is prepared.
/// Dedicated to TWI equipped devices.
/// \param dataByte contains the byte to send to the master.
/// \param expectAck tells about the expected master ACK
/// submission. '0' indicates the last byte to send (from the
/// slave data set).
/// \todo Check why here we have the `excpectAck` but not with the
/// similar twi0_send_byte_slave_multi_master() routine.
/// \todo Might it make more sense to not expect the ACK but just
/// report the ACK status of the master?
void twi0_send_byte_slave(uint8_t dataByte, uint8_t expectAck)
{
    // Prepare data byte.
    I2C0_HW_DATA_REG = dataByte;
    // Start transmission.
    if (expectAck)
        I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
    else
        I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT);
    // Wait until finished.
    while (!(I2C0_HW_CONTROL_REG & (1 << TWINT))) {}
    switch (I2C0_HW_STATUS_REG & (0b11111<<TWS3))
    {
        case 0xB8:  /* byte sent, ACK received */
            break;
        case 0xC8:  /* last byte sent, but ACK received */
            i2c0_failure_info |= I2C_PROTOCOL_FAIL;
            // since expectAck == 0 is the indicator for the last byte to send
            // the following if clause will fail under this circumstance.
        case 0xC0:  /* byte sent, no ACK received */
            if (expectAck)
                i2c0_failure_info |= I2C_NO_ACK;
            I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | slaveAckControl;
            break;
        default:
            i2c0_failure_info |= I2C_PROTOCOL_FAIL;
    }
}


#endif /* defined(I2C_HW_TWI_H_INCLUDED) && defined(I2C0_HW_AS_SLAVE) && !defined(I2C0_HW_AS_MASTER) */
