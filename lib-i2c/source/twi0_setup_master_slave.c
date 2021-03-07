////////////////////////////////////////////////////////////////////////////////
// File        : twi0_setup_master_slave.c
// Description : TWI hardware setup for master + slave operation.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: Aug /  9 / 2012
// Copyright   : © 2006-2021 R. Trapp / H.A.R.R.Y.
// License     : GNU General Public License V3 (GPLv3)
//               See project root LICENSE file or
//               https://www.gnu.org/licenses/gpl-3.0.html
////////////////////////////////////////////////////////////////////////////////
//  --- Doxygen ---
/// \file   twi0_setup_master_slave.c
/// \brief  Initializes the bus master.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_TWI_H_INCLUDED) && defined(I2C0_HW_AS_SLAVE) && defined(I2C0_HW_AS_MASTER) || defined DOXYGEN_DOCU_IS_GENERATED
#include "i2c_lib_private.h"
#include <avr/io.h>

/// \brief
/// Initializes a master with slave response.
/// \details
/// The hardware is set up for the predefined operation mode and
/// parameters.
/// Dedicated to TWI equipped devices.
///
/// \param slaveAddress defines the address to respond to when
/// another master accesses slave devices.
/// If bit 0 is set the slave also answers to the 'general call'
/// address (0).
/// \param slaveMask excludes all bits *set* as invalid for the
/// address comparison. `slaveMask` is omitted on devices without
/// adequate hardware support (e. g. ATmega8). Intentionally this
/// difference will lead to compile issues if the application code
/// gets moved between different TWI revisions (e. g. ATmega8 <=>
/// ATmega168). Using the `slaveMask` the response spans more than
/// just one single slave address.
/// \param divider used to derive the bus frequency from prescaled
/// MCU clock frequency.
/// \param prescaler reduces the MCU clock frequency into a range
/// `divider` can handle.
/// \todo Check if this can get combined with the master-only init
/// and the slave init. I think it can be done (prepared a little).
void twi0_setup_master_slave(uint8_t slaveAddress,
#ifndef I2C0_HW_NO_SLAVE_ADDRESS_MASK
                              uint8_t slaveMask,
#endif
                              uint8_t divider,
                              uint8_t prescaler)
{
//#if defined(I2C0_HW_AS_MASTER)
    // This is for master mode(s).
    I2C0_HW_BITRATE_REG = divider;
    I2C0_HW_STATUS_REG = prescaler;
//#endif
//#if defined(I2C0_HW_AS_SLAVE)
    // This is for slave mode(s).
    I2C0_HW_SLAVE_ADDRESS_REG = slaveAddress;
#ifndef I2C0_HW_NO_SLAVE_ADDRESS_MASK
    I2C0_HW_SLAVE_ADDRESS_MASK = slaveMask;
#endif
    slaveAckControl = (1 << TWEA);
//#endif
    // This is for all modes.
    I2C0_ENABLE;
}


#endif /* defined(I2C_HW_TWI_H_INCLUDED) && defined(I2C0_HW_AS_SLAVE) && defined(I2C0_HW_AS_MASTER) */
