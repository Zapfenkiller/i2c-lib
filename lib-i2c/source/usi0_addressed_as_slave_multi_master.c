////////////////////////////////////////////////////////////////////////////////
// File        : usi0_addressed_as_slave_multi_master.c
// Project     : Roboter 2006
// Author      : R. Trapp
// Initial date: 24. Sep. 2012
// Release rev.: 
// Copyright   : (c) 2006-2013 R. Trapp / H.A.R.R.Y.
// License     : 
// Description : Check if device is addressed as a slave. Report address
//               received.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined (I2C_HW_USI_H_INCLUDED) && defined (I2C0_HW_AS_SLAVE) && defined (I2C0_HW_AS_MASTER)
#include "i2c_lib_private.h"
#include <avr/io.h>
#include <util/delay.h>


/// \brief
/// Check if addressed as a slave device.
/// \details
/// The next byte recevied will be interpreted as an address field if:
/// * the USISIF just is set.
/// * the bus master arbitration cycle has just been lost while sending the addressfield.
///   (see usi0_open_device_multi_master)
/// To keep track of bus changes the i2c0_failure_info is updated when a STOP is
/// detected.
/// Parameters:
/// addressReceived contains the assigned (base) address of slave response(s)
/// slaveMask excludes all marked bits as invalid (if respective mask bit is *set*)
/// returns selection state, ~0 if selected
/// also updates the i2c0_failure_info:
/// clears I2C_RESTARTED and also clears I2C_ARBITRATION_LOST if USIPF is set
/// clears I2C_ARBITRATION_LOST when arbitration loss occured during address field
/// sets i2c0_failure_info to I2C_SUCCESS when addressfield has been received
/// sets i2c0_failure_info to I2C_STOPPED when STOP occured inside the byte
/// sets i2c0_failure_info to I2C_RESTARTED when START occured inside the byte
// Check if USI detected its assigned address space. Return status of check:
// 0 = not addressed, !0 = addressed.
// Report address effectively received.
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
