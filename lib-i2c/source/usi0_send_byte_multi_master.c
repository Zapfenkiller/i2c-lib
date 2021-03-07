////////////////////////////////////////////////////////////////////////////////
// File        : usi0_send_byte_multi_master.c
// Description : Send one byte in multi master mode - USI.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: Sep /  4 / 2012
// Copyright   : © 2006-2021 R. Trapp / H.A.R.R.Y.
// License     : GNU General Public License V3 (GPLv3)
//               See project root LICENSE file or
//               https://www.gnu.org/licenses/gpl-3.0.html
////////////////////////////////////////////////////////////////////////////////
//  --- Doxygen ---
/// \file   usi0_send_byte_multi_master.c
/// \brief  Byte submission in multi master mode.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_USI_H_INCLUDED) && defined(I2C0_HW_AS_MASTER) && !defined(I2C0_HW_SINGLE_MASTER) || defined DOXYGEN_DOCU_IS_GENERATED
#include "i2c_lib_private.h"
#include <avr/io.h>


/// \brief
/// Submit one byte as a bus master (multi master).
/// \details
/// Access the already adressed slave to write one byte.
/// Dedicated to USI equipped devices.
///
/// The slave is expected to always send an 'ACK' answer.
/// Nope. *SMBus* spec allows a slave to NACK bytes in certain
/// cases. One slave always shall ACK its own address(es).
/// *I²C bus* specification is similar and allows any receiver
/// to send a NACK in case it cannot accept the data.
/// \todo Rework the ACK documentation.
/// \param dataByte gets sent out.
// Send one byte as a bus master aware of additional master devices that might
// need to arbitrate with. The problem is the USIDC that is assumed to show
// only during the SCL high phase (the datasheet gives no evidence for sampling
// during SCL '1' and holding while SCL '0'). The high phase might be shorter
// than the time the SW takes to check it. So an alternate scheme is developped
// to keep track with any bus contention even when the SW is a little bit
// slower than the remote master!
// The trick is to send the byte the same way a software master would send it
// and have the hardware to sample the SDA and stretch SCL after every bit. SW
// thus has plenty of time to check outgoing and received bit for collision
// and react properly.
// Sending out a byte is expected to receive an acknowledge!
void usi0_send_byte_multi_master(uint8_t dataByte)
{
    // There is the old bit 7 still latched upon the next rising SCL edge.
    // Countermeasure this latch and preload it with the new value also!
    // According to the datasheet the latch is transparent when SW-CLK is
    // selected. Thus the preloading is such simple.
    I2C0_HW_CONTROL_REG = USI_HOLD_ON_ALL | USI_SHIFT_ON_SOFTWARE_COMMAND;
    I2C0_HW_DATA_REG = 0xFF;
    I2C0_HW_CONTROL_REG = USI_HOLD_ON_ALL | USI_SAMPLE_ON_FALLING_EDGE;
    USI0_SDA_DRIVER_ENABLE;
    for (uint8_t bitcount=0; bitcount<8; )
    {
        if (dataByte & 0x80)
            USI0_RELEASE_SDA;
        else
            USI0_PULL_SDA;
        usi0_wait_until_bit_done_as_multimaster();
        if (i2c0_failure_info & I2C_PROTOCOL_FAIL)
            return;
        bitcount++;
        // check for collision; if shifter[7] is '1' then SDA also must have
        // been '1' and is sampled in USI-Shifter[0]
        if ((dataByte & 0x80) && (!(I2C0_HW_DATA_REG & 0x01)))
        {   // arbitration lost: '1' sent but '0' was on SDA
            // -> immediately retire from bus
            USI0_SDA_DRIVER_DISABLE;
            if (i2c0_failure_info & I2C_RESTARTED)
            {
                uint8_t bitedges = (bitcount << 1) & 0x0F;
                if (bitedges)
                    I2C0_HW_STATUS_REG = (1 << USIOIF) | bitedges;
            }
            else
                I2C0_HW_CONTROL_REG = USI_HOLD_ON_START | USI_SAMPLE_ON_FALLING_EDGE;
            USI0_RELEASE_SCL;
            i2c0_failure_info |= I2C_ARBITRATION_LOST;
            return;
        }
        dataByte = dataByte << 1;
    }
    USI0_SDA_DRIVER_DISABLE;
    i2c0_failure_info &= ~I2C_RESTARTED;
    usi0_wait_until_bit_done_as_multimaster();
    if (i2c0_failure_info)
        return;
    if (I2C0_HW_DATA_REG & 0x01)
        i2c0_failure_info |= I2C_NO_ACK;
}


#endif /* defined(I2C_HW_USI_H_INCLUDED) && defined(I2C0_HW_AS_MASTER) && !defined(I2C0_HW_SINGLE_MASTER) */
