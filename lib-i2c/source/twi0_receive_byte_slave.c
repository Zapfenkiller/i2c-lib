////////////////////////////////////////////////////////////////////////////////
// File        : twi0_receive_byte_slave.c
// Project     : Roboter 2006
// Author      : R. Trapp
// Initial date: 9. Aug. 2012
// Release rev.: 
// Copyright   : (c) 2006-2012 R. Trapp / H.A.R.R.Y.
// License     : 
// Description : Receive one byte in slave mode - TWI.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_TWI_H_INCLUDED) && defined(I2C0_HW_AS_SLAVE) && !defined(I2C0_HW_AS_MASTER)
#include <avr/io.h>

// Receive one byte as slave.
uint8_t twi0_receive_byte_slave(uint8_t sendAck)
{
    // Start reception; select ACK option.
    if (sendAck)
        I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
    else
        I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT);
    // Wait until finished.
    while (!(I2C0_HW_CONTROL_REG & (1 << TWINT))) {}
    uint8_t dataByte = I2C0_HW_DATA_REG;
    switch (I2C0_HW_STATUS_REG & (0b11111<<TWS3))
    {
        case 0xA0:  /* STOP or repeated START condition */
            i2c0_failure_info |= I2C_STOPPED;
        case 0x88:  /* addressed, last byte received not ACKed */
        case 0x98:  /* general call, last byte received not ACKed */
            I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | slaveAckControl;
        case 0x80:  /* addressed, last byte received with ACK */
        case 0x90:  /* general call, last byte received with ACK */
            break;
        case 0x00:
            I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | (1 << TWSTO) | slaveAckControl;
        default:
            i2c0_failure_info |= I2C_PROTOCOL_FAIL;
    }
    return(dataByte);
}


#endif /* defined(I2C_HW_TWI_H_INCLUDED) && defined(I2C0_HW_AS_SLAVE) && !defined(I2C0_HW_AS_MASTER) */
