////////////////////////////////////////////////////////////////////////////////
// File        : usi0_send_byte_slave.c
// Project     : Roboter 2006
// Author      : R. Trapp
// Initial date: 6. Sep. 2012
// Release rev.: 
// Copyright   : (c) 2006-2013 R. Trapp / H.A.R.R.Y.
// License     : 
// Description : Send one byte in any slave mode - USI.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_USI_H_INCLUDED) && defined(I2C0_HW_AS_SLAVE)
#include "i2c_lib_private.h"
#include <avr/io.h>


// Send one byte to the bus when in slave mode.
void usi0_send_byte_slave(uint8_t dataByte)
{
    I2C0_HW_CONTROL_REG = USI_HOLD_ON_ALL | USI_SAMPLE_ON_RISING_EDGE;
    // Put data byte to USIDR.
    I2C0_HW_DATA_REG = dataByte;
    // Enable SDA to drive the bus
    USI0_SDA_DRIVER_ENABLE;
    // Wait until data byte is 'out'.
    i2c0_failure_info |= usi0_wait_until_transfer_done_as_slave(USI_COUNT_FOR_A_BYTE_SLAVE);
    USI0_SDA_DRIVER_DISABLE;
    if (i2c0_failure_info)
    {
        return;
    }
    I2C0_HW_CONTROL_REG = USI_HOLD_ON_ALL | USI_SAMPLE_ON_FALLING_EDGE;
    // Wait until ACK bit is 'in'.
    i2c0_failure_info |= usi0_wait_until_transfer_done_as_slave(USI_COUNT_FOR_A_BIT_SLAVE);
    if (i2c0_failure_info)
    {
        return;
    }
    if (I2C0_HW_DATA_REG & 0x01)
    {
        // Always flag the NACK.
        i2c0_failure_info |= I2C_NO_ACK;
        // Only start condition detector is allowed to block SCL!
        I2C0_HW_CONTROL_REG = USI_HOLD_ON_START | USI_SAMPLE_ON_FALLING_EDGE;
        return;
    }
}


#endif /* defined(I2C_HW_USI_H_INCLUDED) && defined(I2C0_HW_AS_SLAVE) */
