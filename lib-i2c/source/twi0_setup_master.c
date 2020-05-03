////////////////////////////////////////////////////////////////////////////////
// File        : twi0_setup_master.c
// Project     : Roboter 2006
// Author      : R. Trapp
// Initial date: 9. Aug. 2012
// Release rev.: 
// Copyright   : (c) 2006-2012 R. Trapp / H.A.R.R.Y.
// License     : 
// Description : TWI hardware setup for master without slave operation.
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
