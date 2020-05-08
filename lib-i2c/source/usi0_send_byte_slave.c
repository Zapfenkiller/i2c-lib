////////////////////////////////////////////////////////////////////////////////
// File        : usi0_send_byte_slave.c
// Description : Send one byte in any slave mode - USI.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: Sep /  6 / 2012
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
/// \file   usi0_send_byte_slave.c
/// \brief  Byte submission in slave mode.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_USI_H_INCLUDED) && defined(I2C0_HW_AS_SLAVE) || defined DOXYGEN_DOCU_IS_GENERATED
#include "i2c_lib_private.h"
#include <avr/io.h>


/// \brief
/// Send one byte to the bus master.
/// \details
/// When addressed as slave one byte is returned to the master.
/// The predefined amount of bytes is checked and in case of
/// misalignements an error code is prepared.
/// Dedicated to USI equipped devices.
/// \param dataByte contains the byte to send to the master.
/// \param expectAck tells about the expected master ACK
/// submission. '0' indicates the last byte to send (from the
/// slave data set).
/// \todo Might it make more sense to not expect the ACK but just
/// report the ACK status of the master?
void usi0_send_byte_slave(uint8_t dataByte)
{
    I2C0_HW_CONTROL_REG = USI_HOLD_ON_ALL | USI_SAMPLE_ON_RISING_EDGE;
    // Put data byte to USIDR.
    I2C0_HW_DATA_REG = dataByte;
    // Enable SDA to drive the bus
    USI0_SDA_DRIVER_ENABLE;
    // Wait until data byte is 'out'.
    i2c0_failure_info |= usi0_wait_until_transfer_done_as_slave(USI_COUNT_FOR_A_BYTE_SLAVE);
    USI0_SDA_DRIVER_DISABLE;
    if (i2c0_failure_info)
    {
        return;
    }
    I2C0_HW_CONTROL_REG = USI_HOLD_ON_ALL | USI_SAMPLE_ON_FALLING_EDGE;
    // Wait until ACK bit is 'in'.
    i2c0_failure_info |= usi0_wait_until_transfer_done_as_slave(USI_COUNT_FOR_A_BIT_SLAVE);
    if (i2c0_failure_info)
    {
        return;
    }
    if (I2C0_HW_DATA_REG & 0x01)
    {
        // Always flag the NACK.
        i2c0_failure_info |= I2C_NO_ACK;
        // Only start condition detector is allowed to block SCL!
        I2C0_HW_CONTROL_REG = USI_HOLD_ON_START | USI_SAMPLE_ON_FALLING_EDGE;
        return;
    }
}


#endif /* defined(I2C_HW_USI_H_INCLUDED) && defined(I2C0_HW_AS_SLAVE) */
