////////////////////////////////////////////////////////////////////////////////
// File        : usi0_addressed_as_slave_multi_master.c
// Description : Check if this is addressed as a slave. Report address received.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: Sep / 24 / 2012
// Copyright   : © 2006-2020 R. Trapp / H.A.R.R.Y.
// License     : https://mit-license.org/
//
//               The MIT License (MIT)
//               ---------------------
//
//               Permission is hereby granted, free of charge, to
//               any person obtaining a copy of this software and
//               associated documentation files (the “Software”),
//               to deal in the Software without restriction,
//               including without limitation the rights to use,
//               copy, modify, merge, publish, distribute,
//               sublicense, and/or sell copies of the Software,
//               and to permit persons to whom the Software is
//               furnished to do so, subject to the following
//               conditions:
//
//               The above copyright notice and this permission
//               notice shall be included in all copies or
//               substantial portions of the Software.
//
//               THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT
//               WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
//               INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//               MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
//               AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//               AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//               CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
//               ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//               FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
//               OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
////////////////////////////////////////////////////////////////////////////////
//  --- Doxygen ---
/// \file   usi0_addressed_as_slave_multi_master.c
/// \brief  USI slave address scan - multi-master-slave mode
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined (I2C_HW_USI_H_INCLUDED) && defined (I2C0_HW_AS_SLAVE) && defined (I2C0_HW_AS_MASTER) || defined DOXYGEN_DOCU_IS_GENERATED
#include "i2c_lib_private.h"
#include <avr/io.h>
#include <util/delay.h>

/// \brief
/// Check if addressed as a slave device when in multimaster mode.
/// \details
/// Dedicated to USI equipped devices.
/// The next byte received will be interpreted as an address field
/// if
///
/// * the USISIF just is set or
/// * the bus master arbitration cycle has just been lost while
///   sending the addressfield.
///
/// To keep track of bus changes the `i2c0_failure_info` is updated
/// when a STOP is detected.
///
/// \param addressReceived contains the assigned (base) address of
/// slave response(s). Used to report the complete address received
/// in case the slave responds to an address range.
/// \param slaveMask excludes all marked bits as invalid (if
/// respective mask bit is *set*)
/// \returns selection state, ~0 if selected, 0 if not.
///
/// Updates the i2c0_failure_info:
///
/// * Clears `I2C_RESTARTED` and also clears `I2C_ARBITRATION_LOST`
///   if USIPF is set.
/// * Clears `I2C_ARBITRATION_LOST` when arbitration loss occured
///   during address field.
/// * Sets `i2c0_failure_info` to `I2C_SUCCESS` when addressfield
///   has been received.
/// * Sets `i2c0_failure_info` to `I2C_STOPPED` when STOP occured
///   inside the byte.
/// * Sets `i2c0_failure_info` to `I2C_RESTARTED` when START occured
///   inside the byte.
///
/// \sa usi0_open_device_multi_master()

uint8_t usi0_addressed_as_slave_multi_master(uint8_t *addressReceived, uint8_t slaveMask)
{
   if (i2c0_failure_info == (I2C_RESTARTED | I2C_ARBITRATION_LOST))
   { // bus arbitration lost while sending addressfield, so (re)start already
      // has been received.
      // take away the special 'arbitration lost in address field' but keep
      // the bus blocked. The remote master will send a stop condition.
      i2c0_failure_info &= ~I2C_ARBITRATION_LOST;
      // wait for addressfield to get complete
      while (!(I2C0_HW_STATUS_REG & (1 << USIOIF)))
      {
         if (I2C0_HW_STATUS_REG & (1 << USIPF))
         {
            i2c0_failure_info &= ~I2C_RESTARTED;
            I2C0_HW_CONTROL_REG = USI_HOLD_ON_START | USI_SAMPLE_ON_FALLING_EDGE;
            return(0);
         }
         if (I2C0_HW_STATUS_REG & (1 << USISIF))
         {
            I2C0_HW_CONTROL_REG = USI_HOLD_ON_ALL | USI_SAMPLE_ON_FALLING_EDGE;
            return(0);
         }
      }
      return(usi0_check_4_slave_address_match_multi_master(addressReceived, slaveMask));
   }
   I2C0_HW_CONTROL_REG = USI_HOLD_ON_START | USI_SAMPLE_ON_FALLING_EDGE;
   // hunt for bus release
   if (I2C0_HW_STATUS_REG & (1 << USIPF))
   {
      i2c0_failure_info &= ~(I2C_RESTARTED | I2C_ARBITRATION_LOST);
// ???  I2C0_HW_STATUS_REG &= (1 << USIPF) | 0x0F;
      I2C0_HW_STATUS_REG = (1 << USIPF);
   }
   // Check for and react upon START condition.
   if (I2C0_HW_STATUS_REG & (1 << USISIF))
   {
      // Since USISIF might occur while SCL still is '1' we have to wait until
      // SCL is '0'!
      while (I2C0_SCL_IS_HIGH) {}
      // Change SCL to block also until USIOIF is cleared
      I2C0_HW_CONTROL_REG = USI_HOLD_ON_ALL | USI_SAMPLE_ON_FALLING_EDGE;
      _delay_us(1);
      i2c0_failure_info = usi0_wait_until_transfer_done_as_slave(USI_COUNT_FOR_A_BYTE_SLAVE);
      if (!i2c0_failure_info)
      {
         return(usi0_check_4_slave_address_match_multi_master(addressReceived, slaveMask));
      }
   }
   return(0);
}

#endif /* defined (I2C_HW_USI_H_INCLUDED) && defined (I2C0_HW_AS_SLAVE) && defined (I2C0_HW_AS_MASTER) */
