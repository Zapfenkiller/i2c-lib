////////////////////////////////////////////////////////////////////////////////
// File        : usi0_addressed_as_slave.c
// Description : Check if this is addressed as a slave. Report address received.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: Sep / 24 / 2012
// Copyright   : © 2006-2021 R. Trapp / H.A.R.R.Y.
// License     : GNU General Public License V3 (GPLv3)
//               See project root LICENSE file or
//               https://www.gnu.org/licenses/gpl-3.0.html
////////////////////////////////////////////////////////////////////////////////
//  --- Doxygen ---
/// \file   usi0_addressed_as_slave.c
/// \brief  USI slave address scan - slave only mode
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined (I2C_HW_USI_H_INCLUDED) && defined (I2C0_HW_AS_SLAVE) && !defined (I2C0_HW_AS_MASTER) || defined DOXYGEN_DOCU_IS_GENERATED
#include "i2c_lib_private.h"
#include <avr/io.h>

/// \brief
/// Check if addressed as a slave device when just being a slave.
/// \details
/// Dedicated to USI equipped devices.
/// The next byte received will be interpreted as an address field
/// if
///
/// * the USISIF just is set.
///
/// To keep track of bus changes the `i2c0_failure_info` is updated
/// when a STOP is detected.
/// \param addressReceived contains the assigned (base) address of
/// slave response(s). Used to report the complete address received
/// in case the slave responds to an address range.
/// If bit 0 is set the slave also answers to the 'general call'
/// address (0).
/// \param slaveMask excludes all bits *set* as invalid for the
/// address comparison.
/// \returns selection state, ~0 if selected, 0 if not.

uint8_t usi0_addressed_as_slave(uint8_t *addressReceived, uint8_t slaveMask)
{
    I2C0_HW_CONTROL_REG = USI_HOLD_ON_START | USI_SAMPLE_ON_FALLING_EDGE;
    // Check for and react upon START condition.
    if (I2C0_HW_STATUS_REG & (1 << USIPF))
    {
        i2c0_failure_info &= ~I2C_RESTARTED;
    }
    if (!(I2C0_HW_STATUS_REG & (1 << USISIF)))
    {
        return(0);
    }
    i2c0_failure_info = I2C_SUCCESS;
    // Prepare pattern for address comparison later on.
    uint8_t addr = *addressReceived & ~slaveMask & 0xFE;
    // Wait for SCL to fall and then until byte is 'in'.
    while (!I2C0_SCL_IS_LOW) {}
    // Change SCL to block also until USIOIF is cleared
    I2C0_HW_CONTROL_REG = USI_HOLD_ON_ALL | USI_SAMPLE_ON_FALLING_EDGE;
    i2c0_failure_info |= usi0_wait_until_transfer_done_as_slave(USI_COUNT_FOR_A_BYTE_SLAVE);
    if (i2c0_failure_info)
    {
        return(0);
    }
    // Get complete addressfield (incl. R/W).
    *addressReceived = I2C0_HW_DATA_REG;
    // Check for coincidence.
    if (slaveAckControl &&
        (((*addressReceived & ~slaveMask & 0xFE) == addr) ||
            ((slaveMask & 0x01) && !(*addressReceived & 0xFE)))
       )
    {
        // Address matches!
        // Prepare ACK reply.
        USI0_PULL_SDA;
        USI0_SDA_DRIVER_ENABLE;
        // Wait until ACK bit is 'out'.
        i2c0_failure_info |= usi0_wait_until_transfer_done_as_slave(USI_COUNT_FOR_A_BIT_SLAVE);
        // Release SDA.
        USI0_SDA_DRIVER_DISABLE;
        USI0_RELEASE_SDA;
        return(~0);
    }
    i2c0_failure_info |= I2C_RESTARTED; // here means "bus is occupied"
    I2C0_HW_CONTROL_REG = USI_HOLD_ON_START | USI_SAMPLE_ON_FALLING_EDGE;
    return(0);
}


#endif /* defined (I2C_HW_USI_H_INCLUDED) && defined (I2C0_HW_AS_SLAVE) && !defined (I2C0_HW_AS_MASTER) */
