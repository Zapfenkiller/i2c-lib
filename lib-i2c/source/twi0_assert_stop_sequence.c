////////////////////////////////////////////////////////////////////////////////
// File        : twi0_assert_stop_sequence.c
// Description : TWI asserts STOP sequence, releases the bus.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: Aug /  9 / 2012
// Copyright   : © 2006-2020 R. Trapp / H.A.R.R.Y.
// License     : https://mit-license.org/
//
//               The MIT License (MIT)
//               ---------------------
//
//               Permission is hereby granted, free of charge, to
//               any person obtaining a copy of this software and
//               associated documentation files (the “Software”),
//               to deal in the Software without restriction,
//               including without limitation the rights to use,
//               copy, modify, merge, publish, distribute,
//               sublicense, and/or sell copies of the Software,
//               and to permit persons to whom the Software is
//               furnished to do so, subject to the following
//               conditions:
//
//               The above copyright notice and this permission
//               notice shall be included in all copies or
//               substantial portions of the Software.
//
//               THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT
//               WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
//               INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//               MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
//               AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//               AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//               CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
//               ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//               FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
//               OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
