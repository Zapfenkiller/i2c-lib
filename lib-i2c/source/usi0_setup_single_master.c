////////////////////////////////////////////////////////////////////////////////
// File        : usi0_setup_single_master.c
// Project     : Roboter 2006
// Author      : R. Trapp
// Initial date: 24. Oct. 2012
// Release rev.: 
// Copyright   : (c) 2006-2012 R. Trapp / H.A.R.R.Y.
// License     : 
// Description : USI hardware setup for master only (= single master) operation
//               mode.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_USI_H_INCLUDED) && !defined(I2C0_HW_AS_SLAVE) && defined(I2C0_HW_AS_MASTER) && defined(I2C0_HW_SINGLE_MASTER)
#include <avr/io.h>


// Initialize the IO-lines and the bus.
void usi0_setup_single_master(void)
{
    // setup does not disrupt any I²C transfer!
    I2C0_HW_DDR &= ~((1 << I2C0_HW_SDA_BIT) | ( 1 << I2C0_HW_SCL_BIT));
    I2C0_HW_PORT |= ((1 << I2C0_HW_SDA_BIT) | ( 1 << I2C0_HW_SCL_BIT));
    // Enable SCL to drive the bus, SDA is kept recessive.
    I2C0_HW_STATUS_REG = (1<<USIOIF) | (1<<USISIF) | (1<<USIPF);
    I2C0_HW_DDR |= (1 << I2C0_HW_SCL_BIT);
}


#endif /* defined(I2C_HW_USI_H_INCLUDED) && !defined(I2C0_HW_AS_SLAVE) && defined(I2C0_HW_AS_MASTER) && defined(I2C0_HW_SINGLE_MASTER) */
