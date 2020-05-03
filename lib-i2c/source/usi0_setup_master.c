////////////////////////////////////////////////////////////////////////////////
// File        : usi0_setup_master.c
// Author      : R. Trapp
// Initial date: 1.5.2020 - fehlte irgendwie im Fundus ...
// Copyright   : (c) 2006-2013 R. Trapp / H.A.R.R.Y.
// License     : 
// Description : USI hardware setup for master and slave operation mode.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_USI_H_INCLUDED) && !defined(I2C0_HW_AS_SLAVE) && defined(I2C0_HW_AS_MASTER)
#include "i2c_lib_private.h"
#include <avr/io.h>


// Initialize the IO-lines, the bus and the USI-hardware.
void usi0_setup_master(void)
{
    // setup does not disrupt any I²C transfer!
    I2C0_HW_DDR &= ~((1 << I2C0_HW_SDA_BIT) | ( 1 << I2C0_HW_SCL_BIT));
    I2C0_HW_PORT |= ((1 << I2C0_HW_SDA_BIT) | ( 1 << I2C0_HW_SCL_BIT));
    // Enable SCL to drive the bus, SDA is kept recessive.
    I2C0_HW_STATUS_REG = (1<<USIOIF) | (1<<USISIF) | (1<<USIPF);
    I2C0_HW_DDR |= (1 << I2C0_HW_SCL_BIT);
    // Enable for arbitration with other master.
    I2C0_HW_CONTROL_REG = USI_HOLD_ON_START | USI_SAMPLE_ON_FALLING_EDGE;
//    slaveAckControl = ~0;
}


#endif /* defined(I2C_HW_USI_H_INCLUDED) && !defined(I2C0_HW_AS_SLAVE) && defined(I2C0_HW_AS_MASTER) */
