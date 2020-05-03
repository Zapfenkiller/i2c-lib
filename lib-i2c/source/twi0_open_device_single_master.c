////////////////////////////////////////////////////////////////////////////////
// File        : twi0_open_device_single_master.c
// Project     : Roboter 2006
// Author      : R. Trapp
// Initial date: 9. Aug. 2012
// Release rev.: 
// Copyright   : (c) 2006-2012 R. Trapp / H.A.R.R.Y.
// License     : 
// Description : Address one slave using single master mode - TWI.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_TWI_H_INCLUDED) && !defined(I2C0_HW_AS_SLAVE) && defined(I2C0_HW_AS_MASTER) && defined(I2C0_HW_SINGLE_MASTER)
#include "i2c_lib_private.h"
#include <avr/io.h>

// Address a certain slave. Return its status. This also occupies the bus.
enum I2C_FAILURE_type twi0_open_device_single_master(uint8_t deviceAddress)
{
    // Clear previous failures.
    i2c0_failure_info = I2C_SUCCESS;
    // Assert start condition.
    I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | (1 << TWSTA) | slaveAckControl;
    // Wait until done.
    while (!(I2C0_HW_CONTROL_REG & (1 << TWINT))) {}
    twi0_send_byte(deviceAddress);
    switch (I2C0_HW_STATUS_REG & (0b11111<<TWS3))
    {
        case 0x18:  /* address + W successfully sent */
        case 0x40:  /* address + R successfully sent */
            break;
        case 0x20:  /* address + W sent, but no ACK */
        case 0x48:  /* address + R sent, but no ACK */
            i2c0_failure_info |= I2C_NO_ACK;
            break;
        default:
            i2c0_failure_info |= I2C_PROTOCOL_FAIL;
    }
    return(i2c0_failure_info);
}


#endif /* defined(I2C_HW_TWI_H_INCLUDED) && !defined(I2C0_HW_AS_SLAVE) && defined(I2C0_HW_AS_MASTER) && defined(I2C0_HW_SINGLE_MASTER) */
