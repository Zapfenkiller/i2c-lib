////////////////////////////////////////////////////////////////////////////////
// File        : usi0_send_byte_single_master.c
// Project     : Roboter 2006
// Author      : R. Trapp
// Initial date: 4. Sep. 2012
// Release rev.: 
// Copyright   : (c) 2006-2013 R. Trapp / H.A.R.R.Y.
// License     : 
// Description : Send one byte in single master mode - USI.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_USI_H_INCLUDED) && !defined(I2C0_HW_AS_SLAVE) && defined(I2C0_HW_AS_MASTER) && defined(I2C0_HW_SINGLE_MASTER)
#include "i2c_lib_private.h"
#include <avr/io.h>
#include <util/delay.h>


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
