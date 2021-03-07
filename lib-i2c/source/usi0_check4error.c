////////////////////////////////////////////////////////////////////////////////
// File        : usi0_check4error.c
// Description : Return error code of last access.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: Aug / 14 / 2012
// Copyright   : © 2006-2021 R. Trapp / H.A.R.R.Y.
// License     : GNU General Public License V3 (GPLv3)
//               See project root LICENSE file or
//               https://www.gnu.org/licenses/gpl-3.0.html
////////////////////////////////////////////////////////////////////////////////
//  --- Doxygen ---
/// \file   usi0_check4error.c
/// \brief  USI version of bus failure reporting.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined (I2C_HW_USI_H_INCLUDED) || defined DOXYGEN_DOCU_IS_GENERATED
#include "i2c_lib_private.h"

/// \brief
/// Report the last bus transaction status.
/// \details
/// Dedicated to USI equipped devices.
/// \returns failure code according to `I2C_FAILURE_type`.
/// See `i2c_def.h`
enum I2C_FAILURE_type usi0_check4Error(void)
{
    return(i2c0_failure_info);
}


#endif /* defined (I2C_HW_TWI_H_INCLUDED) */
