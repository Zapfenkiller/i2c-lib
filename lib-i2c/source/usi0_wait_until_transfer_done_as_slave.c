////////////////////////////////////////////////////////////////////////////////
// File        : usi0_wait_until_transfer_done_as_slave.c
// Project     : Roboter 2006
// Author      : R. Trapp
// Initial date: 10. Oct. 2012
// Release rev.: 
// Copyright   : (c) 2006-2013 R. Trapp / H.A.R.R.Y.
// License     : 
// Description : Wait until transfer is done when in slave mode.
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
