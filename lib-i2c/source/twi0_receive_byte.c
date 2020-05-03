////////////////////////////////////////////////////////////////////////////////
// File        : twi0_receive_byte.c
// Project     : Roboter 2006
// Author      : R. Trapp
// Initial date: 9. Aug. 2012
// Release rev.: 
// Copyright   : (c) 2006-2012 R. Trapp / H.A.R.R.Y.
// License     : 
// Description : Receive one byte - TWI.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_TWI_H_INCLUDED)
#include <avr/io.h>

// Receive one byte.
uint8_t twi0_receive_byte(uint8_t sendAck)
{
    // Start reception; select ACK option.
    if (sendAck)
        I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
    else
        I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT);
    // Wait until finished.
    while (!(I2C0_HW_CONTROL_REG & (1 << TWINT))) {}
    // Return data byte received.
    return(I2C0_HW_DATA_REG);
}


#endif /* defined(I2C_HW_TWI_H_INCLUDED) */
