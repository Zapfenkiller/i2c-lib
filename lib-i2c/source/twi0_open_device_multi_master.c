////////////////////////////////////////////////////////////////////////////////
// File        : twi0_open_device_multi_master.c
// Description : Address one slave using multi master mode - TWI.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: Aug /  9 / 2012
// Copyright   : © 2006-2021 R. Trapp / H.A.R.R.Y.
// License     : GNU General Public License V3 (GPLv3)
//               See project root LICENSE file or
//               https://www.gnu.org/licenses/gpl-3.0.html
////////////////////////////////////////////////////////////////////////////////
//  --- Doxygen ---
/// \file   twi0_open_device_multi_master.c
/// \brief  Start accessing a certain slave in a multi master mode.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_TWI_H_INCLUDED) && defined(I2C0_HW_AS_MASTER) && !defined(I2C0_HW_SINGLE_MASTER) || defined DOXYGEN_DOCU_IS_GENERATED
#include "i2c_lib_private.h"
#include <avr/io.h>

/// \brief
/// Start access to a certain slave. Valid for any multi master bus
/// mode.
/// \details
/// Asserts a start sequence to the bus, followed by the slave
/// address. Keeps track of the various states that could arise
/// from the slave / bus reaction.
/// Dedicated to TWI equipped devices.
/// \param deviceAddress defines which slave to access. It shall
/// define the READ or WRITE mode also.
/// \returns failure code according to `I2C_FAILURE_type`.
/// See `i2c_def.h`
enum I2C_FAILURE_type twi0_open_device_multi_master(uint8_t deviceAddress)
{
//uint8_t twistat = I2C0_HW_STATUS_REG & (0b11111<<TWS3);
    switch (I2C0_HW_STATUS_REG & (0b11111<<TWS3))
    {
        case 0x08:  // 00001 START already successfully sent
        case 0x10:  // 00010 repeated START already successfully sent
            break;
        default:
            // Assert start condition - TWI-HW will wait for the next opportunity
            I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | (1 << TWSTA) | slaveAckControl;
            // Wait until done.
            while (!(I2C0_HW_CONTROL_REG & (1 << TWINT))) {}
    }
    // Datasheet requires to clear TWSTA after START has been sent!
    I2C0_HW_CONTROL_REG &= ~((1 << TWINT) | (1 << TWSTA));
    switch (I2C0_HW_STATUS_REG & (0b11111<<TWS3))
    {
        case 0x08:  // 00001 START successfully sent
        case 0x10:  // 00010 repeated START successfully sent
            // Try to send out the addressfield
            twi0_send_byte(deviceAddress);
            switch (I2C0_HW_STATUS_REG & (0b11111<<TWS3))
            {
                case 0x18:  // 00011 address + W successfully sent
                case 0x40:  // 01000 address + R successfully sent
                    i2c0_failure_info = I2C_SUCCESS;
                    break;
                case 0x20:  // 00100 address + W sent, but no ACK
                case 0x48:  // 01001 address + R sent, but no ACK
                    i2c0_failure_info = I2C_NO_ACK;
                    break;
                case 0x38:  // 00111 arbitration lost, not addressed by remote master
                case 0x68:  // 01101 arbitration lost, own address + W received while acting as master
                case 0x78:  // 01111 arbitration lost, general call address received while acting as master
                case 0xB0:  // 10110 arbitration lost, own address + R received while acting as master
                    i2c0_failure_info = I2C_RESTARTED | I2C_ARBITRATION_LOST;
                    break;
                case 0x00:
                    I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | (1 << TWSTO) | slaveAckControl;
                default:
                    i2c0_failure_info = I2C_PROTOCOL_FAIL;
            }
            return(i2c0_failure_info);
        case 0x60:  // 01100 own address + W received
        case 0x70:  // 01110 general call address received
        case 0xA8:  // 10101 own address + R received
            i2c0_failure_info = I2C_RESTARTED | I2C_ARBITRATION_LOST;
            return(i2c0_failure_info);
        case 0x00:
            I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | (1 << TWSTO) | slaveAckControl;
        default:
            i2c0_failure_info = I2C_PROTOCOL_FAIL;
    }
    return(i2c0_failure_info);
}


#endif /* defined(I2C_HW_TWI_H_INCLUDED) && defined(I2C0_HW_AS_MASTER) && !defined(I2C0_HW_SINGLE_MASTER) */
