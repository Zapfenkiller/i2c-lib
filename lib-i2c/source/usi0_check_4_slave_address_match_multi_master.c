////////////////////////////////////////////////////////////////////////////////
// File        : usi0_check_4_slave_address_match_multi_master.c
// Project     : Roboter 2006
// Author      : R. Trapp
// Initial date: 01. Mai 2013
// Release rev.: 
// Copyright   : (c) 2006-2014 R. Trapp / H.A.R.R.Y.
// License     : 
// Description : Check if device address received matches asigned address space.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_USI_H_INCLUDED) && defined(I2C0_HW_AS_MASTER) && !defined(I2C0_HW_SINGLE_MASTER)
#include "i2c_lib_private.h"
#include <avr/io.h>


// Assume the byte in the receving register to be a slave address.
// Check if this address matches the assigned address space.
// Return status of check:
// 0 = not addressed, !0 = addressed.
// Report address effectively received.
// Handles ACK / NACK to the bus!
// ==============================
uint8_t usi0_check_4_slave_address_match_multi_master(uint8_t *addressReceived, uint8_t slaveMask)
{
    // Prepare pattern for address comparison later on.
    uint8_t addr = *addressReceived & ~slaveMask & 0xFE;
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


#endif /* defined(I2C_HW_USI_H_INCLUDED) && defined(I2C0_HW_AS_MASTER) && !defined(I2C0_HW_SINGLE_MASTER) */
