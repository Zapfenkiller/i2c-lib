////////////////////////////////////////////////////////////////////////////////
// File        : twi0_setup_slave.c
// Project     : Roboter 2006
// Author      : R. Trapp
// Initial date: 9. Aug. 2012
// Release rev.: 
// Copyright   : (c) 2006-2012 R. Trapp / H.A.R.R.Y.
// License     : 
// Description : TWI hardware setup for slave only operation.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_TWI_H_INCLUDED) && defined(I2C0_HW_AS_SLAVE) && !defined(I2C0_HW_AS_MASTER)
#include <avr/io.h>

// Initialize the IO-lines and the bus.
// Bit0 of 'slaveAddress' defines if answer to general call is enabled!
#ifdef I2C0_HW_NO_SLAVE_ADDRESS_MASK
void twi0_setup_slave(uint8_t slaveAddress)
{
    I2C0_HW_SLAVE_ADDRESS_REG = slaveAddress;
    slaveAckControl = (1 << TWEA);
    I2C0_ENABLE;
}
#else
void twi0_setup_slave(uint8_t slaveAddress, uint8_t slaveMask)
{
    I2C0_HW_SLAVE_ADDRESS_REG = slaveAddress;
    I2C0_HW_SLAVE_ADDRESS_MASK = slaveMask;
    slaveAckControl = (1 << TWEA);
    I2C0_ENABLE;
}
#endif

// Über dieses ifdef mit den zwei Interfaces muss ich nochmal nachdenken!
// Irgendwie schlechter Stil, der Anwender muss sich sonst mit den Interna
// des µC.TWI doch noch auseinandersetzen. Dann lieber was mit ignorieren?
// Andererseits suchen unbedarfte Leute sich den Wolf wenn die Lib die
// Maske 'ignoriert' weil die HW es nicht kann ...

#endif /* defined(I2C_HW_TWI_H_INCLUDED) && defined(I2C0_HW_AS_SLAVE) && !defined(I2C0_HW_AS_MASTER) */
