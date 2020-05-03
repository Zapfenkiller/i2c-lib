////////////////////////////////////////////////////////////////////////////////
// File        : usi0_assert_stop_sequence.c
// Project     : Roboter 2006
// Author      : R. Trapp
// Initial date: 5. Sep. 2012
// Release rev.: 
// Copyright   : (c) 2006-2014 R. Trapp / H.A.R.R.Y.
// License     : 
// Description : USI asserts STOP sequence.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined (I2C_HW_USI_H_INCLUDED) && defined (I2C0_HW_AS_MASTER)
#include <avr/io.h>
#include <util/delay.h>


// Release the bus by asserting a stop sequence.
void usi0_assert_stop_sequence(void)
{
    USI0_PULL_SDA;
    USI0_SDA_DRIVER_ENABLE;
    USI0_HALFBIT_DELAY;
    I2C0_HW_CONTROL_REG = USI_HOLD_ON_START | USI_SAMPLE_ON_FALLING_EDGE;
    USI0_RELEASE_SCL;
    while(I2C0_SCL_IS_LOW) {}
    USI0_HALFBIT_DELAY;
    USI0_SDA_DRIVER_DISABLE;
//  USI0_RELEASE_SDA;
}


#endif /* defined (I2C_HW_USI_H_INCLUDED) && defined (I2C0_HW_AS_MASTER) */
