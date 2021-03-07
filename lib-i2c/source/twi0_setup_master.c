////////////////////////////////////////////////////////////////////////////////
// File        : twi0_setup_master.c
// Description : TWI hardware setup for master without slave operation.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: Aug /  9 / 2012
// Copyright   : © 2006-2021 R. Trapp / H.A.R.R.Y.
// License     : GNU General Public License V3 (GPLv3)
//               See project root LICENSE file or
//               https://www.gnu.org/licenses/gpl-3.0.html
////////////////////////////////////////////////////////////////////////////////
//  --- Doxygen ---
/// \file   twi0_setup_master.c
/// \brief  Initializes the bus master.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_TWI_H_INCLUDED) && !defined(I2C0_HW_AS_SLAVE) && defined(I2C0_HW_AS_MASTER) || defined DOXYGEN_DOCU_IS_GENERATED
#include "i2c_lib_private.h"
#include <avr/io.h>

/// \brief
/// Initializes a master without any slave response.
/// \details
/// The hardware is set up for the predefined operation mode and
/// parameters.
/// Dedicated to TWI equipped devices.
///
/// \param divider used to derive the bus frequency from prescaled
/// MCU clock frequency.
/// \param prescaler reduces the MCU clock frequency into a range
/// `divider` can handle.
/// \todo Check if this can get combined with the master+slave
/// init.
void twi0_setup_master(uint8_t divider, uint8_t prescaler)
{
    I2C0_HW_BITRATE_REG = divider;
    I2C0_HW_STATUS_REG = prescaler;
    I2C0_ENABLE;
}


#endif /* defined(I2C_HW_TWI_H_INCLUDED) && !defined(I2C0_HW_AS_SLAVE) && defined(I2C0_HW_AS_MASTER) */
