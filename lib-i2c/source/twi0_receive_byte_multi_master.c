////////////////////////////////////////////////////////////////////////////////
// File        : twi0_receive_byte_multi_master.c
// Project     : Roboter 2006
// Author      : R. Trapp
// Initial date: 9. Aug. 2012
// Release rev.: 
// Copyright   : (c) 2006-2013 R. Trapp / H.A.R.R.Y.
// License     : 
// Description : Receive one byte in multi master mode - TWI.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_TWI_H_INCLUDED) && defined(I2C0_HW_AS_MASTER) && !defined(I2C0_HW_SINGLE_MASTER)
#include "i2c_lib_private.h"
#include <avr/io.h>

// Receive one byte as a bus master.
uint8_t twi0_receive_byte_multi_master(uint8_t sendAck)
{
    uint8_t dataByte = twi0_receive_byte(sendAck);
    switch (I2C0_HW_STATUS_REG & (0b11111<<TWS3))
    {
        case 0x50:  /* data successfully received, ACK sent */
        case 0x58:  /* data successfully received, but no ACK sent */
            i2c0_failure_info = I2C_SUCCESS;
            break;
        case 0x00:
            I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | (1 << TWSTO) | slaveAckControl;
        default:
            i2c0_failure_info = I2C_PROTOCOL_FAIL;
    }
    return(dataByte);
}


#endif /* defined(I2C_HW_TWI_H_INCLUDED) && defined(I2C0_HW_AS_MASTER) && !defined(I2C0_HW_SINGLE_MASTER) */
