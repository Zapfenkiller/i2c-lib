////////////////////////////////////////////////////////////////////////////////
// File        : usi0_send_byte_single_master.c
// Description : Send one byte in single master mode - USI.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: Sep /  4 / 2012
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
/// \file   usi0_send_byte_single_master.c
/// \brief  Byte submission in single master mode.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_USI_H_INCLUDED) && !defined(I2C0_HW_AS_SLAVE) && defined(I2C0_HW_AS_MASTER) && defined(I2C0_HW_SINGLE_MASTER) || defined DOXYGEN_DOCU_IS_GENERATED
#include "i2c_lib_private.h"
#include <avr/io.h>
#include <util/delay.h>


/// \brief
/// Submit one byte as a bus master (single master).
/// \details
/// Access the already adressed slave to write one byte.
/// Dedicated to USI equipped devices.
///
/// The slave is expected to always send an 'ACK' answer.
/// Nope. *SMBus* spec allows a slave to NACK bytes in certain
/// cases. One slave always shall ACK its own address(es).
/// *I²C bus* specification is similar and allows any receiver
/// to send a NACK in case it cannot accept the data.
/// \todo Rework the ACK documentation.
/// \param dataByte gets sent out.
// Send one byte as a bus master.
// Sending out a byte is expected to receive an acknowledge!
void usi0_send_byte_single_master(uint8_t dataByte)
{
    // Prepare data byte.
    I2C0_HW_DATA_REG = dataByte;
    USI0_SDA_DRIVER_ENABLE;
    I2C0_HW_STATUS_REG = (1 << USIOIF) | 8;
    // Do until counter overflow or bus error.
    while (!(I2C0_HW_STATUS_REG & (1 << USIOIF)))
    {
        USI0_HALFBIT_DELAY;
        USI0_RELEASE_SCL;
        while(I2C0_SCL_IS_LOW) {}
        // Check for bus contention.
        if (I2C0_HW_STATUS_REG & ((1 << USISIF) | (1 << USIPF)))
        {
            // Turn off driver and retire.
            USI0_SDA_DRIVER_DISABLE;
            i2c0_failure_info |= I2C_PROTOCOL_FAIL;
            return;
        }
        USI0_HALFBIT_DELAY;
        USI0_PULL_SCL;
        USI0_CLOCK_SHIFTER_ONCE;
    }
    USI0_SDA_DRIVER_DISABLE;
    // Else check for ACK response.
    USI0_HALFBIT_DELAY;
    USI0_RELEASE_SCL;
    while(I2C0_SCL_IS_LOW) {}
    USI0_HALFBIT_DELAY;
    if (!I2C0_SDA_IS_LOW)
        i2c0_failure_info |= I2C_NO_ACK;
    USI0_PULL_SCL;
}


#endif /* defined(I2C_HW_USI_H_INCLUDED) && !defined(I2C0_HW_AS_SLAVE) && defined(I2C0_HW_AS_MASTER) && defined(I2C0_HW_SINGLE_MASTER) */
