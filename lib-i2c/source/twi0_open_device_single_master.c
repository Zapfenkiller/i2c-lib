////////////////////////////////////////////////////////////////////////////////
// File        : twi0_open_device_single_master.c
// Description : Address one slave using single master mode - TWI.
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


#include "i2c_hw.h"
#if defined(I2C_HW_TWI_H_INCLUDED) && !defined(I2C0_HW_AS_SLAVE) && defined(I2C0_HW_AS_MASTER) && defined(I2C0_HW_SINGLE_MASTER)
#include "i2c_lib_private.h"
#include <avr/io.h>

// Address a certain slave. Return its status. This also occupies the bus.
enum I2C_FAILURE_type twi0_open_device_single_master(uint8_t deviceAddress)
{
    // Clear previous failures.
    i2c0_failure_info = I2C_SUCCESS;
    // Assert start condition.
    I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | (1 << TWSTA) | slaveAckControl;
    // Wait until done.
    while (!(I2C0_HW_CONTROL_REG & (1 << TWINT))) {}
    twi0_send_byte(deviceAddress);
    switch (I2C0_HW_STATUS_REG & (0b11111<<TWS3))
    {
        case 0x18:  /* address + W successfully sent */
        case 0x40:  /* address + R successfully sent */
            break;
        case 0x20:  /* address + W sent, but no ACK */
        case 0x48:  /* address + R sent, but no ACK */
            i2c0_failure_info |= I2C_NO_ACK;
            break;
        default:
            i2c0_failure_info |= I2C_PROTOCOL_FAIL;
    }
    return(i2c0_failure_info);
}


#endif /* defined(I2C_HW_TWI_H_INCLUDED) && !defined(I2C0_HW_AS_SLAVE) && defined(I2C0_HW_AS_MASTER) && defined(I2C0_HW_SINGLE_MASTER) */
