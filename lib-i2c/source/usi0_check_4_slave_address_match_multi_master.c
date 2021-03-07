////////////////////////////////////////////////////////////////////////////////
// File        : usi0_check_4_slave_address_match_multi_master.c
// Description : Check if device address received matches asigned address space.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: May /  1 / 2013
// Copyright   : © 2006-2021 R. Trapp / H.A.R.R.Y.
// License     : GNU General Public License V3 (GPLv3)
//               See project root LICENSE file or
//               https://www.gnu.org/licenses/gpl-3.0.html
////////////////////////////////////////////////////////////////////////////////
//  --- Doxygen ---
/// \file   usi0_check_4_slave_address_match_multi_master.c
/// \brief  Check if received address matches the assigned one.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_USI_H_INCLUDED) && defined(I2C0_HW_AS_MASTER) && !defined(I2C0_HW_SINGLE_MASTER) || defined DOXYGEN_DOCU_IS_GENERATED
#include "i2c_lib_private.h"
#include <avr/io.h>


/// \brief
/// Performs address matching check when being a master addressed
/// as slave.
/// \details
/// Checks the address received and if it matches the assigned own
/// address (address range) it sends `ACK` to the bus. Otherwise
/// the access is `NACK`ed.
/// To keep track of bus changes the `i2c0_failure_info` is updated
/// when a STOP is detected.
/// Dedicated to USI equipped devices.
/// \param addressReceived contains the assigned (base) address of
/// slave response(s). Used to report the complete address received
/// in case the slave responds to an address range.
/// If bit 0 is set the slave also answers to the 'general call'
/// address (0).
/// \param slaveMask excludes all bits *set* as invalid for the
/// address comparison.
/// \returns selection state, ~0 if selected, 0 if not.
// Assume the byte in the receiving register to be a slave address.
// Check if this address matches the assigned address space.
// Return status of check:
// 0 = not addressed, ~0 = addressed.
// Report address effectively received.
// Handles ACK / NACK to the bus!
// ==============================
uint8_t usi0_check_4_slave_address_match_multi_master(uint8_t* addressReceived, uint8_t slaveMask)
{
    // Prepare pattern for address comparison later on.
    uint8_t addr = *addressReceived & ~slaveMask & 0xFE;
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
    i2c0_failure_info |= I2C_RESTARTED; // here means "bus is occupied" by another master
    I2C0_HW_CONTROL_REG = USI_HOLD_ON_START | USI_SAMPLE_ON_FALLING_EDGE;
    return(0);
}


#endif /* defined(I2C_HW_USI_H_INCLUDED) && defined(I2C0_HW_AS_MASTER) && !defined(I2C0_HW_SINGLE_MASTER) */
