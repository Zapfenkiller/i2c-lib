////////////////////////////////////////////////////////////////////////////////
// File        : twi0_send_byte_multi_master.c
// Project     : Roboter 2006
// Author      : R. Trapp
// Initial date: 9. Aug. 2013
// Release rev.: 
// Copyright   : (c) 2006-2012 R. Trapp / H.A.R.R.Y.
// License     : 
// Description : Send one byte in multi master mode - TWI.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_TWI_H_INCLUDED) && defined(I2C0_HW_AS_MASTER) && !defined(I2C0_HW_SINGLE_MASTER)
#include "i2c_lib_private.h"
#include <avr/io.h>

// Send one byte as a bus master.
void twi0_send_byte_multi_master(uint8_t dataByte)
{
    twi0_send_byte(dataByte);
    switch (I2C0_HW_STATUS_REG & (0b11111<<TWS3))
    {
        case 0x28:  /* data successfully sent */
            i2c0_failure_info = I2C_SUCCESS;
            break;
        case 0x30:  /* data sent, but no ACK received */
            i2c0_failure_info = I2C_NO_ACK;
            break;
        case 0x38:  /* arbitration lost while sending data */
            i2c0_failure_info = I2C_ARBITRATION_LOST;
//          I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | (1 << TWSTA) | slaveAckControl; // release SCL
            break;
        case 0x00:
            I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | (1 << TWSTO) | slaveAckControl;
        default:
            i2c0_failure_info = I2C_PROTOCOL_FAIL;
    }
}


#endif /* defined(I2C_HW_TWI_H_INCLUDED) && defined(I2C0_HW_AS_MASTER) && !defined(I2C0_HW_SINGLE_MASTER) */
