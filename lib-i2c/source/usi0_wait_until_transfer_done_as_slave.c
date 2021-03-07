////////////////////////////////////////////////////////////////////////////////
// File        : usi0_wait_until_transfer_done_as_slave.c
// Description : Wait until transfer is done when in slave mode.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: Oct / 10 / 2012
// Copyright   : © 2006-2021 R. Trapp / H.A.R.R.Y.
// License     : GNU General Public License V3 (GPLv3)
//               See project root LICENSE file or
//               https://www.gnu.org/licenses/gpl-3.0.html
////////////////////////////////////////////////////////////////////////////////
//  --- Doxygen ---
/// \file   usi0_wait_until_transfer_done_as_slave.c
/// \brief  Enables or disables a slave device to scan for its
/// address (range).
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_USI_H_INCLUDED) || defined DOXYGEN_DOCU_IS_GENERATED
#include <avr/io.h>


/// \brief
/// Wait for transfer to finish.
/// \details
/// While in slave mode the slave has to wait until a certain count of bits is
/// transferred.
/// This transfer is fully controlled by the master, so there is a chance the master
/// aborts the byte.
/// The handling of this abortion is also done.
/// Since this is a central functionality needed several times in checking for
/// addressfield (is the slave addressed?) and subsequent data read/write cycles
/// it makes sense to have this coded as helper function.
///
/// \param countOfEdges needs to be set in accordance to the amount of bits to
/// be transferred.
/// <TABLE>
/// <TR>
/// <TD><b>Count of bits</b></TD>
/// <TD><b>1</b></TD>
/// <TD>2</TD>
/// <TD>3</TD>
/// <TD>4</TD>
/// <TD>5</TD>
/// <TD>6</TD>
/// <TD>7</TD>
/// <TD><b>8</b></TD>
/// </TR>
/// <TR>
/// <TD>Preset to USICNT[3:0]\n(<b>countOfEdges</b>)</TD>
/// <TD><b>14</b></TD>
/// <TD>12</TD>
/// <TD>10</TD>
/// <TD>8</TD>
/// <TD>6</TD>
/// <TD>4</TD>
/// <TD>2</TD>
/// <TD><b>0</b></TD>
/// </TR>
/// </TABLE>
/// See also\n
/// ::USI_COUNT_FOR_A_BYTE_SLAVE or\n
/// ::USI_COUNT_FOR_A_BIT_SLAVE\n
/// for useful predefinitions.
// countOfEdges (2 per bit!) needs to be set to (16-n) & 0x0F to receive the
// desired amount of bits:
//  bits         =  1,  2,  3, ...,  8
//  n            =  2,  4,  6, ..., 16
//  countOfEdges = 14, 12, 10, ...,  0
uint8_t usi0_wait_until_transfer_done_as_slave(uint8_t countOfEdges)
{
    // Start reception by clearing the counter.
    countOfEdges &= 0x0F;
    I2C0_HW_STATUS_REG = (1 << USIOIF) | (1 << USISIF) | countOfEdges;
    // Wait until transfer finishes, at least somehow.
    while (!(I2C0_HW_STATUS_REG & (1<<USIOIF)))
    {
        // Check for and react upon exceptions.
        if (I2C0_HW_STATUS_REG & (1 << USIPF))
        {
            // Only start condition detector is allowed to block SCL!
            I2C0_HW_CONTROL_REG = USI_HOLD_ON_START | USI_SAMPLE_ON_FALLING_EDGE;
            return(I2C_STOPPED);
        }
        if (I2C0_HW_STATUS_REG & (1 << USISIF))
        {
            // Block SCL on START or address byte complete!
            I2C0_HW_CONTROL_REG = USI_HOLD_ON_ALL | USI_SAMPLE_ON_FALLING_EDGE;
            return(I2C_RESTARTED);
        }
    }
    return(I2C_SUCCESS);
}


#endif /* ... */
