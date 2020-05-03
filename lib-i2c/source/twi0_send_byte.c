////////////////////////////////////////////////////////////////////////////////
// File        : twi0_send_byte.c
// Project     : Roboter 2006
// Author      : R. Trapp
// Initial date: 9. Aug. 2012
// Release rev.: 
// Copyright   : (c) 2006-2012 R. Trapp / H.A.R.R.Y.
// License     : 
// Description : Send one byte - TWI.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_TWI_H_INCLUDED)
#include <avr/io.h>

// Send one byte.
inline void twi0_send_byte(uint8_t dataByte)
{
    // Prepare data byte.
    I2C0_HW_DATA_REG = dataByte;
    // Start transmission, always expect ACK.
    I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
    // Wait until finished.
    while (!(I2C0_HW_CONTROL_REG & (1 << TWINT))) {}
}

#endif /* defined(I2C_HW_TWI_H_INCLUDED) */
