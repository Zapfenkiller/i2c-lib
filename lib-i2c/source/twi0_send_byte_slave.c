////////////////////////////////////////////////////////////////////////////////
// File        : twi0_send_byte_slave.c
// Project     : Roboter 2006
// Author      : R. Trapp
// Initial date: 9. Aug. 2012
// Release rev.: 
// Copyright   : (c) 2006-2012 R. Trapp / H.A.R.R.Y.
// License     : 
// Description : Send one byte in slave mode.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_TWI_H_INCLUDED) && defined(I2C0_HW_AS_SLAVE) && !defined(I2C0_HW_AS_MASTER)
#include <avr/io.h>

void twi0_send_byte_slave(uint8_t dataByte, uint8_t expectAck)
{
    // Prepare data byte.
    I2C0_HW_DATA_REG = dataByte;
    // Start transmission.
    if (expectAck)
        I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
    else
        I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT);
    // Wait until finished.
    while (!(I2C0_HW_CONTROL_REG & (1 << TWINT))) {}
    switch (I2C0_HW_STATUS_REG & (0b11111<<TWS3))
    {
        case 0xB8:  /* byte sent, ACK received */
            break;
        case 0xC8:  /* last byte sent, but ACK received */
            i2c0_failure_info |= I2C_PROTOCOL_FAIL;
            // since expectAck == 0 is the indicator for the last byte to send
            // the following if clause will fail under this circumstance.
        case 0xC0:  /* byte sent, no ACK received */
            if (expectAck)
                i2c0_failure_info |= I2C_NO_ACK;
            I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | slaveAckControl;
            break;
        default:
            i2c0_failure_info |= I2C_PROTOCOL_FAIL;
    }
}


#endif /* defined(I2C_HW_TWI_H_INCLUDED) && defined(I2C0_HW_AS_SLAVE) && !defined(I2C0_HW_AS_MASTER) */
