////////////////////////////////////////////////////////////////////////////////
// File        : usi0_receive_byte_any_master.c
// Description : Receive one byte in multi or single master mode - USI.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: Sep / 24 / 2012
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


#include "i2c_hw.h"
#if defined(I2C_HW_USI_H_INCLUDED) && defined(I2C0_HW_AS_MASTER)
#include "i2c_lib_private.h"
#include <avr/io.h>


// Receive one byte as a bus master.
uint8_t usi0_receive_byte_any_master(uint8_t sendAck)
{
    USI0_SDA_DRIVER_DISABLE;
    I2C0_HW_CONTROL_REG = USI_HOLD_ON_ALL | USI_SAMPLE_ON_FALLING_EDGE;
    for (uint8_t bitcount=0; bitcount<8; bitcount++)
    {
        usi0_wait_until_bit_done_as_multimaster();
        if (i2c0_failure_info & I2C_PROTOCOL_FAIL)
            return(0);
    }
    uint8_t dataByte = I2C0_HW_DATA_REG;
    // Prepare ACK or NACK reply and send it.
    if (sendAck)
    {
        USI0_PULL_SDA;
        USI0_SDA_DRIVER_ENABLE;
    }
    usi0_wait_until_bit_done_as_multimaster();
    USI0_SDA_DRIVER_DISABLE;
    if (!sendAck && (!(I2C0_HW_DATA_REG & 0x01)))
        i2c0_failure_info = I2C_ARBITRATION_LOST | I2C_NO_ACK;
    return(dataByte);
}


#endif /* defined(I2C_HW_USI_H_INCLUDED) && defined(I2C0_HW_AS_MASTER) */
