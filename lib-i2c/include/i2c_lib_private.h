////////////////////////////////////////////////////////////////////////////////
// File        : i2c_lib_private.h
// Description : Definitions for ease of I2C subroutines programming and code
//               readability, but not visible to the application.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: Oct / 23 / 2012
// Copyright   : © 2006-2021 R. Trapp / H.A.R.R.Y.
// License     : GNU General Public License V3 (GPLv3)
//               See project root LICENSE file or
//               https://www.gnu.org/licenses/gpl-3.0.html
////////////////////////////////////////////////////////////////////////////////
//  --- Doxygen ---
/// \file   i2c_lib_private.h
/// \brief
/// Aids to simplify usage of the I<SUP>2</SUP>C library.
/// \details
/// Based on an older version of 2006 the I<SUP>2</SUP>C library had been rewritten.
/// This file contains definitions for ease of I<SUP>2</SUP>C library subroutines
/// programming, usage and code readability. This contents are not visible to
/// the application (project).
////////////////////////////////////////////////////////////////////////////////


#ifndef I2C_LIB_PRIVATE_H_INCLUDED
#define I2C_LIB_PRIVATE_H_INCLUDED


#include "i2c_def.h"
#include <inttypes.h>


/// \brief
/// Internal place to store the failure code(s).
/// So far the original intention.
/// To save RAM cells this has been redefined to serve as a status
/// indication. So some codes are errors but others remain
/// informative - or get errors in some but not all situations.
/// See `i2c_def.h` for details about status codes.
enum I2C_FAILURE_type i2c0_failure_info;


#if defined I2C0_HW_AS_SLAVE || defined DOXYGEN_DOCU_IS_GENERATED
/// \brief
/// Internal place to store slave acknowledge behaviour.
/// \note
/// This RAM byte is occupied only when a slave mode is enabled.
/// This variable is void if solely a master mode is selected.
uint8_t slaveAckControl;
#else
   #define slaveAckControl 0
#endif

#endif // I2C_LIB_PRIVATE_H_INCLUDED
