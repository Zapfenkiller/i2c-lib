////////////////////////////////////////////////////////////////////////////////
// File        : usi0_wait_until_bit_done_as_multimaster.c
// Description : Wait until transfer of one bit is done when in multi master
//               mode.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: Jan /  1 / 2014
// Copyright   : © 2006-2021 R. Trapp / H.A.R.R.Y.
// License     : GNU General Public License V3 (GPLv3)
//               See project root LICENSE file or
//               https://www.gnu.org/licenses/gpl-3.0.html
////////////////////////////////////////////////////////////////////////////////
//  --- Doxygen ---
/// \file   usi0_wait_until_bit_done_as_multimaster.c
/// \brief  Enables or disables a slave device to scan for its
/// address (range).
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_USI_H_INCLUDED) && defined(I2C0_HW_AS_MASTER) && !defined(I2C0_HW_SINGLE_MASTER) || defined DOXYGEN_DOCU_IS_GENERATED
#include "i2c_lib_private.h"
#include <avr/io.h>
#include <util/delay.h>


/// \brief
/// Control of bit to send or receive.
/// \details
/// While in (multi) master mode the master has to control SCL for a bit to get
/// sent or received. It also has to check for bus contention. If contention is
/// detected the master retires from the bus with one of those failure codes:
/// I2C_PROTOCOL_FAIL | I2C_RESTARTED if a START condition is detected
/// I2C_PROTOCOL_FAIL | I2C_STOPPED if a STOP condition is detected
/// i2c0_failure_info is updated accordingly.
void usi0_wait_until_bit_done_as_multimaster(void)
{
    USI0_HALFBIT_DELAY;
    I2C0_HW_STATUS_REG = (1 << USIOIF) | (1 << USISIF) | USI_COUNT_FOR_A_BIT_SLAVE;
    USI0_RELEASE_SCL;
    while (I2C0_SLAVE_NEEDS_TIME) {}
    USI0_HALFBIT_DELAY;
    USI0_PULL_SCL;
    if (I2C0_HW_STATUS_REG & (1 << USISIF))
    {
        i2c0_failure_info = I2C_PROTOCOL_FAIL | I2C_RESTARTED;
        USI0_SDA_DRIVER_DISABLE;
        I2C0_HW_CONTROL_REG = USI_HOLD_ON_START | USI_SAMPLE_ON_FALLING_EDGE;
        I2C0_HW_STATUS_REG = (1 << USIOIF) | (1 << USISIF) | USI_COUNT_FOR_A_BYTE_SLAVE;
        USI0_RELEASE_SCL;
        return;
    }
    if (I2C0_HW_STATUS_REG & (1 << USIPF))
    {
        i2c0_failure_info = I2C_PROTOCOL_FAIL | I2C_STOPPED;
        USI0_SDA_DRIVER_DISABLE;
        I2C0_HW_CONTROL_REG = USI_HOLD_ON_START | USI_SAMPLE_ON_FALLING_EDGE;
        I2C0_HW_STATUS_REG = (1 << USIOIF) | (1 << USISIF) | USI_COUNT_FOR_A_BYTE_SLAVE;
        USI0_RELEASE_SCL;
        return;
    }
}


#endif /* defined(I2C_HW_USI_H_INCLUDED) && defined(I2C0_HW_AS_MASTER) && !defined(I2C0_HW_SINGLE_MASTER) */
