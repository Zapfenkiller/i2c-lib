////////////////////////////////////////////////////////////////////////////////
// File        : usi0_setup_single_master.c
// Description : USI hardware setup for master only (= single master) operation
//               mode.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: Oct / 24 / 2012
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
#if defined(I2C_HW_USI_H_INCLUDED) && !defined(I2C0_HW_AS_SLAVE) && defined(I2C0_HW_AS_MASTER) && defined(I2C0_HW_SINGLE_MASTER)
#include <avr/io.h>


// Initialize the IO-lines and the bus.
void usi0_setup_single_master(void)
{
    // setup does not disrupt any I�C transfer!
    I2C0_HW_DDR &= ~((1 << I2C0_HW_SDA_BIT) | ( 1 << I2C0_HW_SCL_BIT));
    I2C0_HW_PORT |= ((1 << I2C0_HW_SDA_BIT) | ( 1 << I2C0_HW_SCL_BIT));
    // Enable SCL to drive the bus, SDA is kept recessive.
    I2C0_HW_STATUS_REG = (1<<USIOIF) | (1<<USISIF) | (1<<USIPF);
    I2C0_HW_DDR |= (1 << I2C0_HW_SCL_BIT);
}


#endif /* defined(I2C_HW_USI_H_INCLUDED) && !defined(I2C0_HW_AS_SLAVE) && defined(I2C0_HW_AS_MASTER) && defined(I2C0_HW_SINGLE_MASTER) */
