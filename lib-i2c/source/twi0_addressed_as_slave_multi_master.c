////////////////////////////////////////////////////////////////////////////////
// File        : twi0_addressed_as_slave_multi_master.c
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
/// \file   twi0_addressed_as_slave_multi_master.c
/// \brief  TWI slave address scan - multi-master-slave mode
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined (I2C_HW_TWI_H_INCLUDED) && defined (I2C0_HW_AS_SLAVE) && (I2C0_HW_AS_MASTER) || defined DOXYGEN_DOCU_IS_GENERATED
#include "i2c_lib_private.h"
#include <avr/io.h>

/// \brief
/// Check if addressed as a slave device when in multimaster mode.
/// \details
/// Dedicated to TWI equipped devices.
/// Keeps track with possible arbitration loss.
/// \param is used to report the complete address received
/// in case the slave responds to an address range.
/// \returns selection state, ~0 if selected, 0 if not.
uint8_t twi0_addressed_as_slave_multi_master (uint8_t *addressReceived)
{
    switch (I2C0_HW_STATUS_REG & (0b11111<<TWS3))
    {
        case 0xA0:  /* STOP or repeated START condition from remote master */
        case 0xC0:  /* byte sent, no ACK received */
        case 0xC8:  /* last byte sent, but ACK received */
            i2c0_failure_info = I2C_STOPPED;
            I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | slaveAckControl;
        case 0xF8:  // 11111 TWI idle or in transfer
            return(0);
        case 0x38:  // 00111 arbitration lost, not addressed by remote master
            i2c0_failure_info = I2C_RESTARTED; // here means "bus is occupied"
            I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | slaveAckControl;
            return(0);
        case 0x68:  // 01101 arbitration lost, own address + W received while acting as master
        case 0x78:  // 01111 arbitration lost, general call address received while acting as master
        case 0xB0:  // 10110 arbitration lost, own address + R received while acting as master
            i2c0_failure_info = I2C_ARBITRATION_LOST;
            *addressReceived = I2C0_HW_DATA_REG;
            return(~0);
        case 0x60:  // 01100 own address + W received
        case 0x70:  // 01110 general call address received
        case 0xA8:  // 10101 own address + R received
            i2c0_failure_info = I2C_SUCCESS;
            *addressReceived = I2C0_HW_DATA_REG;
            return(~0);
        case 0x00:
            I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | (1 << TWSTO) | slaveAckControl;
        default:
            i2c0_failure_info = I2C_PROTOCOL_FAIL;
    }
    return(0);
}


/*
        case 0x00:  // 00000 Bus error (illegal START or STOP)
        case 0x08:  // 00001 START successfully sent
        case 0x10:  // 00010 repeated START successfully sent
        case 0x18:  // 00011 address + W successfully sent
        case 0x20:  // 00100 address + W sent, but no ACK
        case 0x28:  // 00101 data successfully sent
        case 0x30:  // 00110 data sent, but no ACK received
        case 0x38:  // 00111 arbitration lost, not addressed by remote master
        case 0x40:  // 01000 address + R successfully sent
        case 0x48:  // 01001 address + R sent, but no ACK
        case 0x50:  // 01010 data received, ACK returned to bus
        case 0x58:  // 01011 data received, NACK returned to bus
        case 0x60:  // 01100 own address + W received
        case 0x68:  // 01101 arbitration lost, own address + W received while acting as master
        case 0x70:  // 01110 general call address received
        case 0x78:  // 01111 arbitration lost, general call address received while acting as master
        case 0x80:  // 10000 addressed, last byte received with ACK
        case 0x88:  // 10001 addressed, last byte received not ACKed
        case 0x90:  // 10010 general call, last byte received with ACK
        case 0x98:  // 10011 general call, last byte received not ACKed
        case 0xA0:  // 10100 STOP or repeated START condition
        case 0xA8:  // 10101 own address + R received
        case 0xB0:  // 10110 arbitration lost, own address + R received while acting as master
        case 0xB8:  // 10111 byte sent, ACK received
        case 0xC0:  // 11000 byte sent, no ACK received
        case 0xC8:  // 11001 last byte sent, no ACK expected but ACK received
        case 0xD0:  // 11010 unused?
        case 0xD8:  // 11011 unused?
        case 0xE0:  // 11100 unused?
        case 0xE8:  // 11101 unused?
        case 0xF0:  // 11110 unused?
        case 0xF8:  // TWI idle or in transfer, TWINT is '0' here!
*/


#endif /* defined (I2C_HW_TWI_H_INCLUDED) && defined (I2C0_HW_AS_SLAVE) && (I2C0_HW_AS_MASTER) */
