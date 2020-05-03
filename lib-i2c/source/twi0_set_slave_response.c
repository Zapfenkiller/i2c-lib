////////////////////////////////////////////////////////////////////////////////
// File        : twi0_set_slave_response.c
// Project     : Roboter 2006
// Author      : R. Trapp
// Initial date: 9. Aug. 2012
// Release rev.: 
// Copyright   : (c) 2006-2012 R. Trapp / H.A.R.R.Y.
// License     : 
// Description : Define slave response.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_TWI_H_INCLUDED) && defined(I2C0_HW_AS_SLAVE)
#include "i2c_lib_private.h"
#include <avr/io.h>

void twi0_set_slave_response(uint8_t on)
{
    if (on)
        slaveAckControl = (1 << TWEA);
    else
        slaveAckControl = (0 << TWEA);
    I2C0_ENABLE;
}


#endif /* defined(I2C_HW_TWI_H_INCLUDED) && defined(I2C0_HW_AS_SLAVE) */
