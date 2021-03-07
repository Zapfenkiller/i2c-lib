////////////////////////////////////////////////////////////////////////////////
// File        : usi0_receive_byte_any_master.c
// Description : Receive one byte in multi or single master mode - USI.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: Sep / 24 / 2012
// Copyright   : © 2006-2021 R. Trapp / H.A.R.R.Y.
// License     : GNU General Public License V3 (GPLv3)
//               See project root LICENSE file or
//               https://www.gnu.org/licenses/gpl-3.0.html
////////////////////////////////////////////////////////////////////////////////
//  --- Doxygen ---
/// \file   usi0_receive_byte_any_master.c
/// \brief  Byte reception of any master mode.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_USI_H_INCLUDED) && defined(I2C0_HW_AS_MASTER) || defined DOXYGEN_DOCU_IS_GENERATED
#include "i2c_lib_private.h"
#include <avr/io.h>


/// \brief
/// Receive one byte as a bus master (multi master or single master).
/// \details
/// Access the already opened slave to read one byte.
/// Dedicated to USI equipped devices.
///
/// According to the bus protocol the slave expects an 'ACK'
/// to indicate a subsequent transfer will follow. A slave
/// receiving a NACK response will assume that the actual data
/// transfer is the final one (aka last byte).
/// \param transferFollows sends an 'ACK' to the bus if set (!=0).
/// \returns Byte read from bus.
uint8_t usi0_receive_byte_any_master(uint8_t transferFollows)
{
    USI0_SDA_DRIVER_DISABLE;
    I2C0_HW_CONTROL_REG = USI_HOLD_ON_ALL | USI_SAMPLE_ON_FALLING_EDGE;
    for (uint8_t bitcount=0; bitcount<8; bitcount++)
    {
        usi0_wait_until_bit_done_as_multimaster();
        if (i2c0_failure_info & I2C_PROTOCOL_FAIL)
            return(0);
    }
    uint8_t dataByte = I2C0_HW_DATA_REG;
    // Prepare ACK or NACK reply and send it.
    if (transferFollows)
    {
        USI0_PULL_SDA;
        USI0_SDA_DRIVER_ENABLE;
    }
    usi0_wait_until_bit_done_as_multimaster();
    USI0_SDA_DRIVER_DISABLE;
    if (!transferFollows && (!(I2C0_HW_DATA_REG & 0x01)))
        i2c0_failure_info = I2C_ARBITRATION_LOST | I2C_NO_ACK;
    return(dataByte);
}


#endif /* defined(I2C_HW_USI_H_INCLUDED) && defined(I2C0_HW_AS_MASTER) */
