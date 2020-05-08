////////////////////////////////////////////////////////////////////////////////
// File        : usi0_wait_until_bit_done_as_multimaster.c
// Description : Wait until transfer of one bit is done when in multi master
//               mode.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: Jan /  1 / 2014
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
