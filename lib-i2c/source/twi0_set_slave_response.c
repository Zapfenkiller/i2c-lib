////////////////////////////////////////////////////////////////////////////////
// File        : twi0_set_slave_response.c
// Description : Define slave response.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: Aug /  9 / 2012
// Copyright   : © 2006-2021 R. Trapp / H.A.R.R.Y.
// License     : GNU General Public License V3 (GPLv3)
//               See project root LICENSE file or
//               https://www.gnu.org/licenses/gpl-3.0.html
////////////////////////////////////////////////////////////////////////////////
//  --- Doxygen ---
/// \file   twi0_set_slave_response.c
/// \brief  Enables or disables a slave device to scan for its
/// address (range).
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_TWI_H_INCLUDED) && defined(I2C0_HW_AS_SLAVE) || defined DOXYGEN_DOCU_IS_GENERATED
#include "i2c_lib_private.h"
#include <avr/io.h>

/// \brief  Enables or disables a slave device to scan for its
/// address (range).
/// \details
/// Controls if a slave responds to its address. There might be
/// times when a slave is performing some high priority tasks and
/// needs to retire from the bus. If this is allowed at all
/// depends on the application and master setup.
/// Dedicated to TWI equipped devices.
///
/// Use this to effectively connect or disconnect a slave device
/// without physically removing it.
/// \param on enables slave response when set ('!0').
// Why is there `I2C0_ENABLE` given? => Because it is easy
// to use the already declared macro and without TWI enabled it
// makes no sense at all to do this.
/// \todo is it necessary to use `slaveAckControl` as a RAM byte
/// or could I do it better in case of TWI? (USI needs the byte)
/// \todo Check all accesses to `I2C0_HW_CONTROL_REG` if they
/// have the `slaveAckControl` attached as needed.
void twi0_set_slave_response(uint8_t on)
{
    if (on)
        slaveAckControl = (1 << TWEA);
    else
        slaveAckControl = (0 << TWEA);
    I2C0_ENABLE;
}


#endif /* defined(I2C_HW_TWI_H_INCLUDED) && defined(I2C0_HW_AS_SLAVE) */
