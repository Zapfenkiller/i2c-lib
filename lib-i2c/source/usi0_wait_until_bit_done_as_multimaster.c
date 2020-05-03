////////////////////////////////////////////////////////////////////////////////
// File        : usi0_wait_until_bit_done_as_multimaster.c
// Project     : Roboter 2006
// Author      : R. Trapp
// Initial date:  1. Jan. 2014
// Release rev.: 
// Copyright   : (c) 2006-2014 R. Trapp / H.A.R.R.Y.
// License     : 
// Description : Wait until transfer of one bit is done when in multi master
//               mode.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_USI_H_INCLUDED) && defined(I2C0_HW_AS_MASTER) && !defined(I2C0_HW_SINGLE_MASTER)
#include "i2c_lib_private.h"
#include <avr/io.h>
#include <util/delay.h>


void usi0_wait_until_bit_done_as_multimaster(void)
{
    USI0_HALFBIT_DELAY;
    I2C0_HW_STATUS_REG = (1 << USIOIF) | (1 << USISIF) | USI_COUNT_FOR_A_BIT_SLAVE;
    USI0_RELEASE_SCL;
    while (I2C0_SLAVE_NEEDS_TIME) {}
    USI0_HALFBIT_DELAY;
    USI0_PULL_SCL;
    if (I2C0_HW_STATUS_REG & (1 << USISIF))
    {
        i2c0_failure_info = I2C_PROTOCOL_FAIL | I2C_RESTARTED;
        USI0_SDA_DRIVER_DISABLE;
        I2C0_HW_CONTROL_REG = USI_HOLD_ON_START | USI_SAMPLE_ON_FALLING_EDGE;
        I2C0_HW_STATUS_REG = (1 << USIOIF) | (1 << USISIF) | USI_COUNT_FOR_A_BYTE_SLAVE;
        USI0_RELEASE_SCL;
        return;
    }
    if (I2C0_HW_STATUS_REG & (1 << USIPF))
    {
        i2c0_failure_info = I2C_PROTOCOL_FAIL | I2C_STOPPED;
        USI0_SDA_DRIVER_DISABLE;
        I2C0_HW_CONTROL_REG = USI_HOLD_ON_START | USI_SAMPLE_ON_FALLING_EDGE;
        I2C0_HW_STATUS_REG = (1 << USIOIF) | (1 << USISIF) | USI_COUNT_FOR_A_BYTE_SLAVE;
        USI0_RELEASE_SCL;
        return;
    }
}


#endif /* defined(I2C_HW_USI_H_INCLUDED) && defined(I2C0_HW_AS_MASTER) && !defined(I2C0_HW_SINGLE_MASTER) */
