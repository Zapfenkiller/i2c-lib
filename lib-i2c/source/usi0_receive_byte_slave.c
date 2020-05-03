////////////////////////////////////////////////////////////////////////////////
// File        : usi0_receive_byte_slave.c
// Project     : Roboter 2006
// Author      : R. Trapp
// Initial date: 6. Sep. 2012
// Release rev.: 
// Copyright   : (c) 2006-2013 R. Trapp / H.A.R.R.Y.
// License     : 
// Description : Receive one byte in any slave mode - USI.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined (I2C_HW_USI_H_INCLUDED) && defined (I2C0_HW_AS_SLAVE)
#include "i2c_lib_private.h"
#include <avr/io.h>


// Receive one byte as slave.
uint8_t usi0_receive_byte_slave(uint8_t sendAck)
{
    I2C0_HW_CONTROL_REG = USI_HOLD_ON_ALL | USI_SAMPLE_ON_FALLING_EDGE;
    // Wait until data byte is 'in'.
    i2c0_failure_info |= usi0_wait_until_transfer_done_as_slave(USI_COUNT_FOR_A_BYTE_SLAVE);
    // Get byte just received.
    uint8_t dataByte = I2C0_HW_DATA_REG;
    if (!(i2c0_failure_info))
    {
        if (sendAck)
        {
            // Prepare ACK reply.
            USI0_PULL_SDA;
            USI0_SDA_DRIVER_ENABLE;
        }
        // Wait until ACK bit is 'out'.
        i2c0_failure_info |= usi0_wait_until_transfer_done_as_slave(USI_COUNT_FOR_A_BIT_SLAVE);
        USI0_SDA_DRIVER_DISABLE;
        USI0_RELEASE_SDA;
    }
    return(dataByte);
}


#endif /* defined (I2C_HW_USI_H_INCLUDED) && defined (I2C0_HW_AS_SLAVE) */
