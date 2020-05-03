////////////////////////////////////////////////////////////////////////////////
// File        : usi0_addressed_as_slave.c
// Project     : Roboter 2006
// Author      : R. Trapp
// Initial date: 24. Sep. 2012
// Release rev.: 
// Copyright   : (c) 2006-2013 R. Trapp / H.A.R.R.Y.
// License     : 
// Description : Check if device is addressed as a slave. Report address
//               received.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined (I2C_HW_USI_H_INCLUDED) && defined (I2C0_HW_AS_SLAVE) && !defined (I2C0_HW_AS_MASTER)
#include "i2c_lib_private.h"
#include <avr/io.h>

// Check if USI detected its assigned address space. Return status of check:
// 0 = not addressed, !0 = addressed.
// Report address effectively received.
uint8_t usi0_addressed_as_slave(uint8_t *addressReceived, uint8_t slaveMask)
{
    I2C0_HW_CONTROL_REG = USI_HOLD_ON_START | USI_SAMPLE_ON_FALLING_EDGE;
    // Check for and react upon START condition.
    if (I2C0_HW_STATUS_REG & (1 << USIPF))
    {
        i2c0_failure_info &= ~I2C_RESTARTED;
    }
    if (!(I2C0_HW_STATUS_REG & (1 << USISIF)))
    {
        return(0);
    }
    i2c0_failure_info = I2C_SUCCESS;
    // Prepare pattern for address comparison later on.
    uint8_t addr = *addressReceived & ~slaveMask & 0xFE;
    // Wait for SCL to fall and then until byte is 'in'.
    while (!I2C0_SCL_IS_LOW) {}
    // Change SCL to block also until USIOIF is cleared
    I2C0_HW_CONTROL_REG = USI_HOLD_ON_ALL | USI_SAMPLE_ON_FALLING_EDGE;
    i2c0_failure_info |= usi0_wait_until_transfer_done_as_slave(USI_COUNT_FOR_A_BYTE_SLAVE);
    if (i2c0_failure_info)
    {
        return(0);
    }
    // Get complete addressfield (incl. R/W).
    *addressReceived = I2C0_HW_DATA_REG;
    // Check for coincidence.
    if (slaveAckControl &&
        (((*addressReceived & ~slaveMask & 0xFE) == addr) ||
            ((slaveMask & 0x01) && !(*addressReceived & 0xFE)))
       )
    {
        // Address matches!
        // Prepare ACK reply.
        USI0_PULL_SDA;
        USI0_SDA_DRIVER_ENABLE;
        // Wait until ACK bit is 'out'.
        i2c0_failure_info |= usi0_wait_until_transfer_done_as_slave(USI_COUNT_FOR_A_BIT_SLAVE);
        // Release SDA.
        USI0_SDA_DRIVER_DISABLE;
        USI0_RELEASE_SDA;
        return(~0);
    }
    i2c0_failure_info |= I2C_RESTARTED; // here means "bus is occupied"
    I2C0_HW_CONTROL_REG = USI_HOLD_ON_START | USI_SAMPLE_ON_FALLING_EDGE;
    return(0);
}


#endif /* defined (I2C_HW_USI_H_INCLUDED) && defined (I2C0_HW_AS_SLAVE) && !defined (I2C0_HW_AS_MASTER) */
