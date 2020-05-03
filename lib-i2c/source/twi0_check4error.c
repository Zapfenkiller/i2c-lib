////////////////////////////////////////////////////////////////////////////////
// File        : twi0_check4error.c
// Project     : Roboter 2006
// Author      : R. Trapp
// Initial date: 14. Aug. 2012
// Release rev.: 
// Copyright   : (c) 2006-2012 R. Trapp / H.A.R.R.Y.
// License     : 
// Description : Return error code of last access.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined (I2C_HW_TWI_H_INCLUDED)
#include "i2c_lib_private.h"

// Retrieve errors occuring during last access.
enum I2C_FAILURE_type twi0_check4Error(void)
{
    return(i2c0_failure_info);
}


#endif /* defined (I2C_HW_TWI_H_INCLUDED) */
