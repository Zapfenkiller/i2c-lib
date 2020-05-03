////////////////////////////////////////////////////////////////////////////////
// File        : usi0_receive_byte_any_master.c
// Project     : Roboter 2006
// Author      : R. Trapp
// Initial date: 24. Sep. 2012
// Release rev.: 
// Copyright   : (c) 2006-2014 R. Trapp / H.A.R.R.Y.
// License     : 
// Description : Receive one byte in multi or single master mode - USI.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_USI_H_INCLUDED) && defined(I2C0_HW_AS_MASTER)
#include "i2c_lib_private.h"
#include <avr/io.h>


// Receive one byte as a bus master.
uint8_t usi0_receive_byte_any_master(uint8_t sendAck)
{
    USI0_SDA_DRIVER_DISABLE;
    I2C0_HW_CONTROL_REG = USI_HOLD_ON_ALL | USI_SAMPLE_ON_FALLING_EDGE;
    for (uint8_t bitcount=0; bitcount<8; bitcount++)
    {
        usi0_wait_until_bit_done_as_multimaster();
        if (i2c0_failure_info & I2C_PROTOCOL_FAIL)
            return(0);
    }
    uint8_t dataByte = I2C0_HW_DATA_REG;
    // Prepare ACK or NACK reply and send it.
    if (sendAck)
    {
        USI0_PULL_SDA;
        USI0_SDA_DRIVER_ENABLE;
    }
    usi0_wait_until_bit_done_as_multimaster();
    USI0_SDA_DRIVER_DISABLE;
    if (!sendAck && (!(I2C0_HW_DATA_REG & 0x01)))
        i2c0_failure_info = I2C_ARBITRATION_LOST | I2C_NO_ACK;
    return(dataByte);
}


#endif /* defined(I2C_HW_USI_H_INCLUDED) && defined(I2C0_HW_AS_MASTER) */
