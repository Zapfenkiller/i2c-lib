////////////////////////////////////////////////////////////////////////////////
// File        : usi0_setup_master_slave.c
// Description : USI hardware setup for master and slave operation mode.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: Sep / 18 / 2012
// Copyright   : © 2006-2021 R. Trapp / H.A.R.R.Y.
// License     : GNU General Public License V3 (GPLv3)
//               See project root LICENSE file or
//               https://www.gnu.org/licenses/gpl-3.0.html
////////////////////////////////////////////////////////////////////////////////
//  --- Doxygen ---
/// \file   usi0_setup_master_slave.c
/// \brief  Init USI for multi master and slave operation.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_USI_H_INCLUDED) && defined(I2C0_HW_AS_SLAVE) /*&& defined(I2C0_HW_AS_MASTER)*/ || defined DOXYGEN_DOCU_IS_GENERATED
#include "i2c_lib_private.h"
#include <avr/io.h>

/// \brief
/// Initializes a master with slave response.
/// \details
/// The hardware is set up for the predefined operation mode and
/// parameters.
/// Dedicated to USI equipped devices.
///
/// \todo Check if this can get combined with the master-only init
/// and the slave init. I think it can be done (prepared a little).
/// \todo
/// Understand and document how the difference in the slave address
/// handling (TWI stores it to a HW reg, USI does some 'magic')
/// works and check application demos for code differences that
/// shall not occur!
// Initialize the IO-lines, the bus and the USI-hardware.
void usi0_setup_master_slave(void)
{
    // setup does not disrupt any I²C transfer!
    I2C0_HW_DDR &= ~((1 << I2C0_HW_SDA_BIT) | ( 1 << I2C0_HW_SCL_BIT));
    I2C0_HW_PORT |= ((1 << I2C0_HW_SDA_BIT) | ( 1 << I2C0_HW_SCL_BIT));
    // Enable SCL to drive the bus, SDA is kept recessive.
    I2C0_HW_STATUS_REG = (1<<USIOIF) | (1<<USISIF) | (1<<USIPF);
    I2C0_HW_DDR |= (1 << I2C0_HW_SCL_BIT);
    // Enable full slave response.
    I2C0_HW_CONTROL_REG = USI_HOLD_ON_START | USI_SAMPLE_ON_FALLING_EDGE;
    slaveAckControl = ~0;
}


#endif /* defined(I2C_HW_USI_H_INCLUDED) && defined(I2C0_HW_AS_SLAVE) && defined(I2C0_HW_AS_MASTER) */
