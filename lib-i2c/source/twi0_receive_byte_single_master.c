////////////////////////////////////////////////////////////////////////////////
// File        : twi0_receive_byte_single_master.c
// Description : Receive one byte in single master mode - TWI.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: Aug /  9 / 2012
// Copyright   : © 2006-2021 R. Trapp / H.A.R.R.Y.
// License     : GNU General Public License V3 (GPLv3)
//               See project root LICENSE file or
//               https://www.gnu.org/licenses/gpl-3.0.html
////////////////////////////////////////////////////////////////////////////////
//  --- Doxygen ---
/// \file   twi0_receive_byte_single_master.c
/// \brief  Byte reception of single master mode.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_TWI_H_INCLUDED) && !defined(I2C0_HW_AS_SLAVE) && defined(I2C0_HW_AS_MASTER) && defined(I2C0_HW_SINGLE_MASTER) || defined DOXYGEN_DOCU_IS_GENERATED
#include "i2c_lib_private.h"
#include <avr/io.h>

/// \brief
/// Receive one byte as a bus master.
/// \details
/// Access the already opened slave to read one byte.
/// Dedicated to TWI equipped devices.
///
/// According to the bus protocol the slave expects an 'ACK'
/// to indicate a subsequent transfer will follow. A slave
/// receiving a NACK response will assume that the actual data
/// transfer is the final one (aka last byte).
/// \param transferFollows sends an 'ACK' to the bus if set (!=0).
/// \returns Byte read from bus.
uint8_t twi0_receive_byte_single_master(uint8_t transferFollows)
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
        case 0x50:  /* data successfully received, ACK sent */
        case 0x58:  /* data successfully received, but no ACK sent */
            break;
        default:
            i2c0_failure_info |= I2C_PROTOCOL_FAIL;
    }
    return(dataByte);
}


#endif /* defined(I2C_HW_TWI_H_INCLUDED) && !defined(I2C0_HW_AS_SLAVE) && defined(I2C0_HW_AS_MASTER) && defined(I2C0_HW_SINGLE_MASTER) */
