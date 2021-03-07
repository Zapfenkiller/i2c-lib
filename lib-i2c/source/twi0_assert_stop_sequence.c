////////////////////////////////////////////////////////////////////////////////
// File        : twi0_assert_stop_sequence.c
// Description : TWI asserts STOP sequence, releases the bus.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: Aug /  9 / 2012
// Copyright   : © 2006-2021 R. Trapp / H.A.R.R.Y.
// License     : GNU General Public License V3 (GPLv3)
//               See project root LICENSE file or
//               https://www.gnu.org/licenses/gpl-3.0.html
////////////////////////////////////////////////////////////////////////////////
//  --- Doxygen ---
/// \file   twi0_assert_stop_sequence.c
/// \brief  TWI version of "bus release".
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined (I2C_HW_TWI_H_INCLUDED) && defined (I2C0_HW_AS_MASTER) || defined DOXYGEN_DOCU_IS_GENERATED
#include "i2c_lib_private.h"
#include <avr/io.h>

/// \brief
/// Release the bus when in any master mode.
/// \details
/// Dedicated to TWI equipped devices.
/// Makes the TWI to send a STOP condition to the bus.
void twi0_assert_stop_sequence(void)
{
    // Assert stop condition.
    I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | (1 << TWSTO) | slaveAckControl;
    // Wait until done. Necessary?
//  while (I2C0_HW_CONTROL_REG & (1 << TWSTO)) {}
}


#endif /* defined (I2C_HW_TWI_H_INCLUDED) && defined (I2C0_HW_AS_MASTER) */
