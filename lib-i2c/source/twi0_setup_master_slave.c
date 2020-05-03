////////////////////////////////////////////////////////////////////////////////
// File        : twi0_setup_master_slave.c
// Project     : Roboter 2006
// Author      : R. Trapp
// Initial date: 9. Aug. 2012
// Release rev.: 
// Copyright   : (c) 2006-2012 R. Trapp / H.A.R.R.Y.
// License     : 
// Description : TWI hardware setup for master + slave operation.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_TWI_H_INCLUDED) && defined(I2C0_HW_AS_SLAVE) && defined(I2C0_HW_AS_MASTER)
#include "i2c_lib_private.h"
#include <avr/io.h>

// Initialize the IO-lines and the bus.
// Bit0 of 'slaveAddress' defines if answer to general call is enabled!
void twi0_setup_master_slave(uint8_t slaveAddress,
#ifndef I2C0_HW_NO_SLAVE_ADDRESS_MASK
                              uint8_t slaveMask,
#endif
                              uint8_t divider,
                              uint8_t prescaler)
{
    // This is for master mode.
    I2C0_HW_BITRATE_REG = divider;
    I2C0_HW_STATUS_REG = prescaler;
    // This is for slave mode.
    I2C0_HW_SLAVE_ADDRESS_REG = slaveAddress;
#ifndef I2C0_HW_NO_SLAVE_ADDRESS_MASK
    I2C0_HW_SLAVE_ADDRESS_MASK = slaveMask;
#endif
    slaveAckControl = (1 << TWEA);
    I2C0_ENABLE;
}


#endif /* defined(I2C_HW_TWI_H_INCLUDED) && defined(I2C0_HW_AS_SLAVE) && defined(I2C0_HW_AS_MASTER) */
