////////////////////////////////////////////////////////////////////////////////
// File        : twi0_assert_stop_sequence.c
// Project     : Roboter 2006
// Author      : R. Trapp
// Initial date: 9. Aug. 2012
// Release rev.: 
// Copyright   : (c) 2006-2012 R. Trapp / H.A.R.R.Y.
// License     : 
// Description : TWI asserts STOP sequence.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined (I2C_HW_TWI_H_INCLUDED) && defined (I2C0_HW_AS_MASTER)
#include "i2c_lib_private.h"
#include <avr/io.h>

// Release the bus by asserting a stop sequence.
void twi0_assert_stop_sequence(void)
{
    // Assert stop condition.
    I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | (1 << TWSTO) | slaveAckControl;
    // Wait until done. Necessary?
//  while (I2C0_HW_CONTROL_REG & (1 << TWSTO)) {}
}


#endif /* defined (I2C_HW_TWI_H_INCLUDED) && defined (I2C0_HW_AS_MASTER) */
