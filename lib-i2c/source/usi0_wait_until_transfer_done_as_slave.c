////////////////////////////////////////////////////////////////////////////////
// File        : usi0_wait_until_transfer_done_as_slave.c
// Description : Wait until transfer is done when in slave mode.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: Oct / 10 / 2012
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
#if defined(I2C_HW_USI_H_INCLUDED) // ...
#include <avr/io.h>


// countOfEdges (2 per bit!) needs to be set to (16-n) & 0x0F to receive the
// desired amount of bits:
//  bits         =  1,  2,  3, ...,  8
//  n            =  2,  4,  6, ..., 16
//  countOfEdges = 14, 12, 10, ...,  0
uint8_t usi0_wait_until_transfer_done_as_slave(uint8_t countOfEdges)
{
    // Start reception by clearing the counter.
    countOfEdges &= 0x0F;
    I2C0_HW_STATUS_REG = (1 << USIOIF) | (1 << USISIF) | countOfEdges;
    // Wait until transfer finishes, at least somehow.
    while (!(I2C0_HW_STATUS_REG & (1<<USIOIF)))
    {
        // Check for and react upon exceptions.
        if (I2C0_HW_STATUS_REG & (1 << USIPF))
        {
            // Only start condition detector is allowed to block SCL!
            I2C0_HW_CONTROL_REG = USI_HOLD_ON_START | USI_SAMPLE_ON_FALLING_EDGE;
            return(I2C_STOPPED);
        }
        if (I2C0_HW_STATUS_REG & (1 << USISIF))
        {
            // Block SCL on START or address byte complete!
            I2C0_HW_CONTROL_REG = USI_HOLD_ON_ALL | USI_SAMPLE_ON_FALLING_EDGE;
            return(I2C_RESTARTED);
        }
    }
    return(I2C_SUCCESS);
}


#endif /* ... */
