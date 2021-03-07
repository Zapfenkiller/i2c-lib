////////////////////////////////////////////////////////////////////////////////
// File        : usi0_receive_byte_slave.c
// Description : Receive one byte in any slave mode - USI.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: Sep /  6 / 2012
// Copyright   : © 2006-2021 R. Trapp / H.A.R.R.Y.
// License     : GNU General Public License V3 (GPLv3)
//               See project root LICENSE file or
//               https://www.gnu.org/licenses/gpl-3.0.html
////////////////////////////////////////////////////////////////////////////////
//  --- Doxygen ---
/// \file   usi0_receive_byte_slave.c
/// \brief  Byte reception of slave mode.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined (I2C_HW_USI_H_INCLUDED) && defined (I2C0_HW_AS_SLAVE) || defined DOXYGEN_DOCU_IS_GENERATED
#include "i2c_lib_private.h"
#include <avr/io.h>


/// \brief
/// Receive one byte as a slave device.
/// \details
/// Receive one byte from the master when addressed as slave device.
/// Dedicated to USI equipped devices.
///
/// \todo
/// The slave is expected to always send an 'ACK' answer. ??
/// So why is there a parameter to decide if ACK is sent back?
/// Nope.
/// Rework this completely, we have similar places already. See there
/// how to write it. The code here is right.
/// \param sendAck sends an 'ACK' to the bus if set (!=0).
/// \returns Byte read from bus.
uint8_t usi0_receive_byte_slave(uint8_t sendAck)
{
    I2C0_HW_CONTROL_REG = USI_HOLD_ON_ALL | USI_SAMPLE_ON_FALLING_EDGE;
    // Wait until data byte is 'in'.
    i2c0_failure_info |= usi0_wait_until_transfer_done_as_slave(USI_COUNT_FOR_A_BYTE_SLAVE);
    // Get byte just received.
    uint8_t dataByte = I2C0_HW_DATA_REG;
    if (!(i2c0_failure_info))
    {
        if (sendAck)
        {
            // Prepare ACK reply.
            USI0_PULL_SDA;
            USI0_SDA_DRIVER_ENABLE;
        }
        // Wait until ACK bit is 'out'.
        i2c0_failure_info |= usi0_wait_until_transfer_done_as_slave(USI_COUNT_FOR_A_BIT_SLAVE);
        USI0_SDA_DRIVER_DISABLE;
        USI0_RELEASE_SDA;
    }
    return(dataByte);
}


#endif /* defined (I2C_HW_USI_H_INCLUDED) && defined (I2C0_HW_AS_SLAVE) */
