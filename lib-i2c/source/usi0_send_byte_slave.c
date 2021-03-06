////////////////////////////////////////////////////////////////////////////////
// File        : usi0_send_byte_slave.c
// Description : Send one byte in any slave mode - USI.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: Sep /  6 / 2012
// Copyright   : © 2006-2021 R. Trapp / H.A.R.R.Y.
// License     : GNU General Public License V3 (GPLv3)
//               See project root LICENSE file or
//               https://www.gnu.org/licenses/gpl-3.0.html
////////////////////////////////////////////////////////////////////////////////
//  --- Doxygen ---
/// \file   usi0_send_byte_slave.c
/// \brief  Byte submission in slave mode.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_USI_H_INCLUDED) && defined(I2C0_HW_AS_SLAVE) || defined DOXYGEN_DOCU_IS_GENERATED
#include "i2c_lib_private.h"
#include <avr/io.h>


/// \brief
/// Send one byte to the bus master.
/// \details
/// When addressed as slave one byte is returned to the master.
/// The predefined amount of bytes is checked and in case of
/// misalignements an error code is prepared.
/// Dedicated to USI equipped devices.
/// \param dataByte contains the byte to send to the master.
/// \param expectAck tells about the expected master ACK
/// submission. '0' indicates the last byte to send (from the
/// slave data set).
/// \todo Might it make more sense to not expect the ACK but just
/// report the ACK status of the master?
void usi0_send_byte_slave(uint8_t dataByte)
{
    I2C0_HW_CONTROL_REG = USI_HOLD_ON_ALL | USI_SAMPLE_ON_RISING_EDGE;
    // Put data byte to USIDR.
    I2C0_HW_DATA_REG = dataByte;
    // Enable SDA to drive the bus
    USI0_SDA_DRIVER_ENABLE;
    // Wait until data byte is 'out'.
    i2c0_failure_info |= usi0_wait_until_transfer_done_as_slave(USI_COUNT_FOR_A_BYTE_SLAVE);
    USI0_SDA_DRIVER_DISABLE;
    if (i2c0_failure_info)
    {
        return;
    }
    I2C0_HW_CONTROL_REG = USI_HOLD_ON_ALL | USI_SAMPLE_ON_FALLING_EDGE;
    // Wait until ACK bit is 'in'.
    i2c0_failure_info |= usi0_wait_until_transfer_done_as_slave(USI_COUNT_FOR_A_BIT_SLAVE);
    if (i2c0_failure_info)
    {
        return;
    }
    if (I2C0_HW_DATA_REG & 0x01)
    {
        // Always flag the NACK.
        i2c0_failure_info |= I2C_NO_ACK;
        // Only start condition detector is allowed to block SCL!
        I2C0_HW_CONTROL_REG = USI_HOLD_ON_START | USI_SAMPLE_ON_FALLING_EDGE;
        return;
    }
}


#endif /* defined(I2C_HW_USI_H_INCLUDED) && defined(I2C0_HW_AS_SLAVE) */
