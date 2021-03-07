////////////////////////////////////////////////////////////////////////////////
// File        : twi0_send_byte_single_master.c
// Description : Send one byte in single master mode - TWI.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: Aug /  9 / 2012
// Copyright   : © 2006-2021 R. Trapp / H.A.R.R.Y.
// License     : GNU General Public License V3 (GPLv3)
//               See project root LICENSE file or
//               https://www.gnu.org/licenses/gpl-3.0.html
////////////////////////////////////////////////////////////////////////////////
//  --- Doxygen ---
/// \file   twi0_send_byte_single_master.c
/// \brief  Byte submission in single master mode.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_TWI_H_INCLUDED) && !defined(I2C0_HW_AS_SLAVE) && defined(I2C0_HW_AS_MASTER) && defined(I2C0_HW_SINGLE_MASTER) || defined DOXYGEN_DOCU_IS_GENERATED
#include "i2c_lib_private.h"
#include <avr/io.h>

/// \brief
/// Submit one byte as a bus master (single master).
/// \details
/// Access the already adressed slave to write one byte.
/// Dedicated to TWI equipped devices.
///
/// The slave is expected to always send an 'ACK' answer.
/// Nope. *SMBus* spec allows a slave to NACK bytes in certain
/// cases. One slave always shall ACK its own address(es).
/// *I²C bus* specification is similar and allows any receiver
/// to send a NACK in case it cannot accept the data.
/// \todo Rework the ACK documentation.
/// \param dataByte gets sent out.
void twi0_send_byte_single_master(uint8_t dataByte)
{
    // Prepare data byte.
    I2C0_HW_DATA_REG = dataByte;
    // Start transmission, expect ACK.
    I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
    // Wait until finished.
    while (!(I2C0_HW_CONTROL_REG & (1 << TWINT))) {}
    switch (I2C0_HW_STATUS_REG & (0b11111<<TWS3))
    {
        case 0x28:  /* data successfully sent */
            break;
        case 0x30:  /* data sent, but no ACK received */
            i2c0_failure_info |= I2C_NO_ACK;
            break;
        default:
            i2c0_failure_info |= I2C_PROTOCOL_FAIL;
    }
}


#endif /* defined(I2C_HW_TWI_H_INCLUDED) && !defined(I2C0_HW_AS_SLAVE) && defined(I2C0_HW_AS_MASTER) && defined(I2C0_HW_SINGLE_MASTER) */
