////////////////////////////////////////////////////////////////////////////////
// File        : twi0_send_byte.c
// Description : Send one byte - TWI.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: Aug /  9 / 2012
// Copyright   : © 2006-2021 R. Trapp / H.A.R.R.Y.
// License     : GNU General Public License V3 (GPLv3)
//               See project root LICENSE file or
//               https://www.gnu.org/licenses/gpl-3.0.html
////////////////////////////////////////////////////////////////////////////////
//  --- Doxygen ---
/// \file   twi0_send_byte.c
/// \brief  Generic byte submission.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_TWI_H_INCLUDED) || defined DOXYGEN_DOCU_IS_GENERATED
#include <avr/io.h>

/// \details
/// One byte is sent to the bus using the I<SUP>2</SUP>C protocol.
/// This function merely serves as a subroutine for the different
/// operation modes. It does not do any failure checking since
/// this depends on the state of the TWI hardware.
///
/// The slave is expected to always send an 'ACK' answer.
/// Nope. *SMBus* spec allows a slave to NACK bytes in certain
/// cases. One slave always shall ACK its own address(es).
/// *I²C bus* specification is similar and allows any receiver
/// to send a NACK in case it cannot accept the data.
/// \param dataByte gets sent out.
/// \todo Rework the ACK documentation.
inline void twi0_send_byte(uint8_t dataByte)
{
    // Prepare data byte.
    I2C0_HW_DATA_REG = dataByte;
    // Start transmission, always expect ACK.
    I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
    // Wait until finished.
    while (!(I2C0_HW_CONTROL_REG & (1 << TWINT))) {}
}

#endif /* defined(I2C_HW_TWI_H_INCLUDED) */
