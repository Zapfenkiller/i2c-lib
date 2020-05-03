////////////////////////////////////////////////////////////////////////////////
// File        : twi0_setup_master.c
// Description : TWI hardware setup for master without slave operation.
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
#if defined(I2C_HW_TWI_H_INCLUDED) && !defined(I2C0_HW_AS_SLAVE) && defined(I2C0_HW_AS_MASTER)
#include <avr/io.h>

#define  slaveAckControl (0 << TWEA);

// Initialize the IO-lines and the bus.
void twi0_setup_master(uint8_t divider, uint8_t prescaler)
{
    I2C0_HW_BITRATE_REG = divider;
    I2C0_HW_STATUS_REG = prescaler;
    I2C0_ENABLE;
}


#endif /* defined(I2C_HW_TWI_H_INCLUDED) && !defined(I2C0_HW_AS_SLAVE) && defined(I2C0_HW_AS_MASTER) */
