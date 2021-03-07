////////////////////////////////////////////////////////////////////////////////
// File        : i2c_def.h
// Description : Definitions for ease of I2C subroutines programming and code
//               readability.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: Aug /  2 / 2012
// Copyright   : © 2006-2021 R. Trapp / H.A.R.R.Y.
// License     : GNU General Public License V3 (GPLv3)
//               See project root LICENSE file or
//               https://www.gnu.org/licenses/gpl-3.0.html
////////////////////////////////////////////////////////////////////////////////
//  --- Doxygen ---
/// \file   i2c_def.h
/// \brief
/// Tidbits to simplify usage of the I<SUP>2</SUP>C library.
/// \details
/// Based on an older version of 2006 the I<SUP>2</SUP>C library had been rewritten.
/// This file contains definitions for ease of I<SUP>2</SUP>C library subroutines
/// programming, usage and code readability.
/// The definitions herein are used in coding the library routines but also by
/// the application. To avoid trouble when copies are handled in several places
/// these definitions are made once and made visible to all parties. The application
/// gains this through i2c_hw.h, so you do not need to include i2c_def.h directly.
////////////////////////////////////////////////////////////////////////////////


#ifndef I2C_DEF_H_INCLUDED
#define I2C_DEF_H_INCLUDED


/// \brief
/// The definition of the read/write access bit state for writing.
#define I2C_WRITE_ACCESS      0


/// \brief
/// The definition of the read/write access bit state for reading.
#define I2C_READ_ACCESS       1


/// \brief
/// The definition for sending an 'ACK' response.
#define I2C_SEND_ACK          ~0


/// \brief
/// The definition for not sending an 'ACK' response.
#define I2C_SEND_NO_ACK       0


// \brief
// The definition of the acknowledge bit state for 'ACK'.
//#define I2C_ACKNOWLEDGE       0


// \brief
// The definition of the acknowledge bit state for 'NACK'.
//#define I2C_NO_ACKNOWLEDGE    1


/// \brief
/// Failure and status codes of I<SUP>2</SUP>C library routines.
/// \note The flags `I2C_ARBITRATION_LOST` and `I2C_RESTARTED` set simultaneously
/// indicate  the case of master arbitration loss while sending out the address
/// field. This is due when  multi master mode is selected on a device having
/// USI hardware. It is necessary for the handover from master to slave mode and
/// correctly indentifying the addressfield for decoding. In case of a TWI all
/// this is handled by the TWI hardware and so the flags behave a little bit
/// different. In all cases the same application code can be used for both variants
/// if written properly.
enum I2C_FAILURE_type
{
    I2C_SUCCESS = 0,                ///< Transfer(s) are successfull.
    I2C_NO_ACK = 1 << 0,            ///< No 'ACK' from slave.
    I2C_STOPPED = 1 << 1,           ///< 'STOP' detected during transfer.
    I2C_TIMED_OUT = 1 << 2,         ///< Timeout on SCL line.
    I2C_SDA_BLOCKED = 1 << 3,       ///< Timeout on SDA line.
    I2C_PROTOCOL_FAIL = 1 << 4,     ///< Failure during access.
    I2C_ARBITRATION_LOST = 1 << 5,  ///< Bus (master) arbitration lost.
    I2C_SLAVE_COLLISION = 1 << 6,   ///< Slave address assigned multiple times; reserved to SMB enhancement(s).
    I2C_RESTARTED = 1 << 7          ///< 'START' detected during transfer.
};

#endif // I2C_DEF_H_INCLUDED
