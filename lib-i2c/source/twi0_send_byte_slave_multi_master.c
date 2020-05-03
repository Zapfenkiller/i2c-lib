////////////////////////////////////////////////////////////////////////////////
// File        : twi0_send_byte_slave_multi_master.c
// Project     : Roboter 2006
// Author      : R. Trapp
// Initial date: 9. Aug. 2012
// Release rev.: 
// Copyright   : (c) 2006-2013 R. Trapp / H.A.R.R.Y.
// License     : 
// Description : Send one byte in slave mode.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_TWI_H_INCLUDED) && defined(I2C0_HW_AS_SLAVE) && defined(I2C0_HW_AS_MASTER)
#include "i2c_lib_private.h"
#include <avr/io.h>

void twi0_send_byte_slave_multi_master(uint8_t dataByte)
{
    twi0_send_byte(dataByte);
    switch (I2C0_HW_STATUS_REG & (0b11111<<TWS3))
    {
        case 0xB8:  /* byte sent, ACK received */
            i2c0_failure_info = I2C_SUCCESS;
            break;
        case 0xC0:  /* byte sent, no ACK received */
        case 0xC8:  /* last byte sent, but ACK received */
            i2c0_failure_info = I2C_STOPPED;
            break;
        case 0x00:
            I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | (1 << TWSTO) | slaveAckControl;
        default:
            i2c0_failure_info = I2C_PROTOCOL_FAIL;
    }
}


#endif /* defined(I2C_HW_TWI_H_INCLUDED) && defined(I2C0_HW_AS_SLAVE) && defined(I2C0_HW_AS_MASTER) */
