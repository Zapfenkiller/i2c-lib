////////////////////////////////////////////////////////////////////////////////
// File        : usi0_assert_stop_sequence.c
// Description : USI asserts STOP sequence, releases the bus.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: Sep /  5 / 2012
// Copyright   : © 2006-2021 R. Trapp / H.A.R.R.Y.
// License     : GNU General Public License V3 (GPLv3)
//               See project root LICENSE file or
//               https://www.gnu.org/licenses/gpl-3.0.html
////////////////////////////////////////////////////////////////////////////////
//  --- Doxygen ---
/// \file   usi0_assert_stop_sequence.c
/// \brief  USI version of "bus release".
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined (I2C_HW_USI_H_INCLUDED) && defined (I2C0_HW_AS_MASTER) || defined DOXYGEN_DOCU_IS_GENERATED
#include <avr/io.h>
#include <util/delay.h>


/// \brief
/// Release the bus when in any master mode.
/// \details
/// Send a STOP condition to the bus.
/// Respect clock stretching eventually done by slower slave(s).
/// Dedicated to USI equipped devices.
void usi0_assert_stop_sequence(void)
{
    USI0_PULL_SDA;
    USI0_SDA_DRIVER_ENABLE;
    USI0_HALFBIT_DELAY;
    I2C0_HW_CONTROL_REG = USI_HOLD_ON_START | USI_SAMPLE_ON_FALLING_EDGE;
    USI0_RELEASE_SCL;
    while(I2C0_SCL_IS_LOW) {}
    USI0_HALFBIT_DELAY;
    USI0_SDA_DRIVER_DISABLE;
}


#endif /* defined (I2C_HW_USI_H_INCLUDED) && defined (I2C0_HW_AS_MASTER) */
