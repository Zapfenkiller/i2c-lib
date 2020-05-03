////////////////////////////////////////////////////////////////////////////////
// File        : twi0_addressed_as_slave.c
// Project     : Roboter 2006
// Author      : R. Trapp
// Initial date: 9. Aug. 2012
// Release rev.: 
// Copyright   : (c) 2006-2012 R. Trapp / H.A.R.R.Y.
// License     : 
// Description : Check if device is addressed as a slave. Report address
//               received.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined (I2C_HW_TWI_H_INCLUDED) && defined (I2C0_HW_AS_SLAVE) && !defined (I2C0_HW_AS_MASTER)
#include "i2c_lib_private.h"
#include <avr/io.h>

// Check if TWI detected its assigned address space. Return status of check:
// 0 = not addressed, !0 = addressed.
// Report address effectively received.
uint8_t twi0_addressed_as_slave (uint8_t *addressReceived)
{
    i2c0_failure_info = I2C_SUCCESS;
    switch (I2C0_HW_STATUS_REG & (0b11111<<TWS3))
    {
        case 0xF8:  /* TWI idle or in transfer */
            return(0);
        case 0x60:  /* own address + W received */
        case 0x70:  /* general call address received */
        case 0xA8:  /* own address + R received */
            *addressReceived = I2C0_HW_DATA_REG;
            return(~0);
            break;
        case 0x00:
            I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | (1 << TWSTO) | slaveAckControl;
        default:
            i2c0_failure_info |= I2C_PROTOCOL_FAIL;
    }
    return(0);
}


#endif /* defined (I2C_HW_TWI_H_INCLUDED) && defined (I2C0_HW_AS_SLAVE) && !defined (I2C0_HW_AS_MASTER) */
