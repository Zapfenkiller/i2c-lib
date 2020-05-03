////////////////////////////////////////////////////////////////////////////////
// File        : twi0_setup_slave.c
// Description : TWI hardware setup for slave only operation.
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
#if defined(I2C_HW_TWI_H_INCLUDED) && defined(I2C0_HW_AS_SLAVE) && !defined(I2C0_HW_AS_MASTER)
#include <avr/io.h>

// Initialize the IO-lines and the bus.
// Bit0 of 'slaveAddress' defines if answer to general call is enabled!
#ifdef I2C0_HW_NO_SLAVE_ADDRESS_MASK
void twi0_setup_slave(uint8_t slaveAddress)
{
    I2C0_HW_SLAVE_ADDRESS_REG = slaveAddress;
    slaveAckControl = (1 << TWEA);
    I2C0_ENABLE;
}
#else
void twi0_setup_slave(uint8_t slaveAddress, uint8_t slaveMask)
{
    I2C0_HW_SLAVE_ADDRESS_REG = slaveAddress;
    I2C0_HW_SLAVE_ADDRESS_MASK = slaveMask;
    slaveAckControl = (1 << TWEA);
    I2C0_ENABLE;
}
#endif

// Über dieses ifdef mit den zwei Interfaces muss ich nochmal nachdenken!
// Irgendwie schlechter Stil, der Anwender muss sich sonst mit den Interna
// des µC.TWI doch noch auseinandersetzen. Dann lieber was mit ignorieren?
// Andererseits suchen unbedarfte Leute sich den Wolf wenn die Lib die
// Maske 'ignoriert' weil die HW es nicht kann ...

#endif /* defined(I2C_HW_TWI_H_INCLUDED) && defined(I2C0_HW_AS_SLAVE) && !defined(I2C0_HW_AS_MASTER) */
