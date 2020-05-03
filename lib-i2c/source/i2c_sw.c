////////////////////////////////////////////////////////////////////////////////
// File        : i2c_sw.c
// Project     : Roboter 2006
// Author      : R. Trapp
// Initial date: 2. Aug. 2012
// Release rev.: 
// Copyright   : (c) 2006-2012 R. Trapp / H.A.R.R.Y.
// License     : 
// Description : Definitions for using software emulated single master I²C bus.
//               Based on older version of 2006.
//               These identifiers need to be defined already for the library
//               routines to work:
//                  I2C_SW_SDA_PORT
//                  I2C_SW_SDA_DDR
//                  I2C_SW_SDA_READBACK
//                  I2C_SW_SDA_BIT
//                  I2C_SW_SDA_BITPOS
//                  I2C_SW_SCL_PORT
//                  I2C_SW_SCL_DDR
//                  I2C_SW_SCL_READBACK
//                  I2C_SW_SCL_BIT
//                  I2C_SW_SCL_BITPOS
//               These identifiers are also recommended to be set already but
//               they get adjusted to default values if missing:
//                  I2C_SW_EMERGENCY_TIMEOUT_us
//                  F_I2C_SW
////////////////////////////////////////////////////////////////////////////////


#include "i2c_sw.h"
#include <avr/io.h>
#include <util/delay.h>


#define I2C_SW_RELEASE_SCL   I2C_SW_SCL_DDR &= ~I2C_SW_SCL_BITPOS
#define I2C_SW_PULL_SCL      I2C_SW_SCL_DDR |= I2C_SW_SCL_BITPOS
#define I2C_SW_RELEASE_SDA   I2C_SW_SDA_DDR &= ~I2C_SW_SDA_BITPOS
#define I2C_SW_PULL_SDA      I2C_SW_SDA_DDR |= I2C_SW_SDA_BITPOS
#define I2C_SW_SCL_IS_HIGH   (I2C_SW_SCL_READBACK & (I2C_SW_SCL_BITPOS))
#define I2C_SW_SDA_IS_HIGH   (I2C_SW_SDA_READBACK & (I2C_SW_SDA_BITPOS))


// =============================================================================
// These are for internal usage.
// =============================================================================

enum I2C_FAILURE_type softI2c_failure_info = I2C_SUCCESS;


#ifndef F_I2C_SW
#   define F_I2C_SW 100000UL
#   warning "'F_I2C_SW' found undefined. Now defaults to 100kHz.
#endif
void softI2c_waitHalfBitTime(void)
{
    _delay_us(500000UL/F_I2C_SW);
}


// I2C bus definition:
// Slower slave can keep SCL low to synchronize the master to its slower
// speed. A timeout is implemented to avoid hangups if the bus is dis-
// rupted.
#ifndef I2C_SW_EMERGENCY_TIMEOUT_us
#   define I2C_SW_EMERGENCY_TIMEOUT_us 20000UL
#   warning "'I2C_SW_EMERGENCY_TIMEOUT_us' found undefined. Now defaults to 20ms.
#endif
void softI2c_releaseScl(void)
{
    uint16_t    count_us;
    softI2c_waitHalfBitTime();
    I2C_SW_RELEASE_SCL;
    for (count_us=0; !I2C_SW_SCL_IS_HIGH; count_us++)
    {
        if (count_us >= I2C_SW_EMERGENCY_TIMEOUT_us)
        {
            softI2c_failure_info |= I2C_TIMED_OUT;
            break;
        }
        _delay_us(1);
    }
    softI2c_waitHalfBitTime();
}


void softI2c_releaseSda(void)
{
    uint16_t    count_us;
    I2C_SW_RELEASE_SDA;
    for (count_us=0; !I2C_SW_SDA_IS_HIGH; count_us++)
    {
        {
            softI2c_failure_info |= I2C_SDA_BLOCKED;
            break;
        }
        _delay_us(1);
    }
}


// =============================================================================
// These are the PUBLIC ones.
// =============================================================================

// Initialize the IO-lines and the bus.
void softI2c_init(void)
{
    I2C_SW_SDA_PORT &= ~I2C_SW_SDA_BITPOS;
    I2C_SW_SCL_PORT &= ~I2C_SW_SCL_BITPOS;
    softI2c_releaseBus();
}


// Put one byte to the bus, check for acknowledge.
void softI2c_putByte(uint8_t dataByte)
{
    uint8_t counter;
    for (counter=0; counter<8; counter++)
    {
        if (dataByte & 0x80)
            I2C_SW_RELEASE_SDA;
        else
            I2C_SW_PULL_SDA;
        dataByte <<= 1;
        softI2c_releaseScl();
        I2C_SW_PULL_SCL;
    }
    I2C_SW_RELEASE_SDA;
    softI2c_releaseScl();
    if (I2C_SW_SDA_IS_HIGH)
        softI2c_failure_info |= I2C_NO_ACK;
    I2C_SW_PULL_SCL;
}


// Return one byte from the bus, 'sendAck' gives acknowledge.
uint8_t softI2c_getByte(uint8_t sendAck)
{
    uint8_t counter;
    uint8_t dataByte=0;
    I2C_SW_RELEASE_SDA;
    for (counter=0; counter<8; counter++)
    {
        softI2c_releaseScl();
        dataByte <<= 1;
        if (I2C_SW_SDA_IS_HIGH)
            dataByte |= 0x01;
        I2C_SW_PULL_SCL;
    }
    if (sendAck)
        I2C_SW_PULL_SDA;
    else
        I2C_SW_RELEASE_SDA;
    softI2c_releaseScl();
    I2C_SW_PULL_SCL;
    return(dataByte);
}


// Address a certain slave. Return its status. This
// also occupies the bus.
enum I2C_FAILURE_type softI2c_openDevice(uint8_t address)
{
    softI2c_failure_info = I2C_SUCCESS;
    softI2c_releaseSda();
    softI2c_releaseScl();
    I2C_SW_PULL_SDA;
    softI2c_waitHalfBitTime();
    I2C_SW_PULL_SCL;
    softI2c_putByte(address);
    return(softI2c_failure_info);
}


// Release the bus by sending the STOP-sequence.
void softI2c_releaseBus(void)
{
    I2C_SW_PULL_SDA;
    softI2c_releaseScl();
    softI2c_releaseSda();
}


// Retrieve errors occuring during last access.
enum I2C_FAILURE_type softI2c_check4Error(void)
{
    return(softI2c_failure_info);
}


////////////////////////////////////////////////////////////////////////////////
//  Revision history:
//  -----------------
//
//  $Id:$
//
//  $Log:$
//
////////////////////////////////////////////////////////////////////////////////
