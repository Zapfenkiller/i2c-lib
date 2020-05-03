////////////////////////////////////////////////////////////////////////////////
// File        : i2c_sw.h
// Project     : Roboter 2006
// Author      : R. Trapp
// Initial date: 2. Aug. 2012
// Release rev.: 
// Copyright   : (c) 2006-2012 R. Trapp / H.A.R.R.Y.
// License     : 
// Description : Definitions for using software emulated single master I2C bus.
//               Based on older version of 2006.
//               These identifiers need to be defined already for the library
//               routines to work:
//                  I2C_SW_SDA_PORT
//                  I2C_SW_SDA_DDR
//                  I2C_SW_SDA_READBACK
//                  I2C_SW_SDA_BIT
//                  I2C_SW_SDA_BITPOS
//                  I2C_SW_SCL_PORT
//                  I2C_SW_SCL_DDR
//                  I2C_SW_SCL_READBACK
//                  I2C_SW_SCL_BIT
//                  I2C_SW_SCL_BITPOS
//               These identifiers are also recommended to be set already but
//               they get adjusted to default values if missing:
//                  I2C_SW_EMERGENCY_TIMEOUT_us
//                  F_I2C_SW
////////////////////////////////////////////////////////////////////////////////
//  --- Doxygen ---
/// \file   i2c_sw.h
/// \brief
/// Include this for implementing a software emulated single I<SUP>2</SUP>C bus master.
/// \details
/// Based on an older version of 2006 the I<SUP>2</SUP>C library had been rewritten.
/// To use the software emulated single bus master functions you have to include
/// this header file into your sources.
/// You also must tell the make tool to compile the `i2c_sw.c` file in addition
/// to your own project source files.
/// This workflow has been selected here because of its flexibility to use
/// any of the microcontroller's IO-lines as SCL or SDA line without code size
/// penalty.
///
/// These identifiers need to be defined already for the library routines to
/// compile:
///  + `I2C_SW_SDA_PORT` defines the port SDA is connected to (e.g. PORTB).
///  + `I2C_SW_SDA_DDR` defines the data direction control of the port SDA is
///    connected to (e.g. DDRB).
///  + `I2C_SW_SDA_READBACK` defines the IO status readback of the port SDA is
///     connected to (e.g. PINB).
///  + `I2C_SW_SDA_BIT` defines the bit number of the IO-line SDA is connected
///    to (e.g. 4).
///  + `I2C_SW_SDA_BITPOS` defines the bitposition to use when manipulating SDA
///    (e.g. (1 << I2C_SW_SDA_BIT)).
///  + `I2C_SW_SCL_PORT` defines the port SCL is connected to (e.g. PORTD).
///  + `I2C_SW_SCL_DDR` defines the data direction control of the port SDA is
///    connected to (e.g. DDRD).
///  + `I2C_SW_SCL_READBACK` defines the IO status readback of the port SDA is
///     connected to (e.g. PIND).
///  + `I2C_SW_SCL_BIT` defines the bit number of the IO-line SDA is connected
///    to (e.g. 2).
///  + `I2C_SW_SCL_BITPOS` defines the bitposition to use when manipulating SDA
///    (e.g. (1 << I2C_SW_SCL_BIT)).
///  + `F_CPU` defines the operating frequency of the CPU and must be set to its
///    corrected value. The bus speed depends on this definition.
///
/// These identifiers are also recommended to be set but they get adjusted to
/// default values if missing. A warning message is asserted in this case:
///  + `I2C_SW_EMERGENCY_TIMEOUT_us` is used to return in case the bus blocks.
///  + `F_I2C_SW` defines the maximum bus frequency.
///
/// Those definitions can be done in a dedicated project header or in the
/// makefile.
/// The latter one is prefered for its more centralized status and higher
/// flexibility of processing definitions automatically.
///
/// All of the library routines report their status but only few return it directly.
/// A dedicated function is available to report the failure code(s) of the last
/// access.
/// This scheme has been implemented after several trials with the more official
/// way of having the status returned by each funtion.
/// It turned out that having each function returning its status requires both
/// more code FLASH and more RAM.
/// Keeping in mind the small amount of RAM of some controllers the current
/// implementation seems to be the best compromise of coding style and resource
/// requirements.
/////////////////////////////////////////////////////////////////////////////


#ifndef I2C_SW_H_INCLUDED
#define I2C_SW_H_INCLUDED


#include <inttypes.h>
#include "i2c_def.h"


// =============================================================================
// Check for bus frequency definition.
// =============================================================================

/// \brief
/// Default bus operating frequency. The unit is Hz (Hertz).
/// \details
/// This prerequisite defines the default maximum bus speed to be used - if not
/// defined already.
///
/// If your application calls for another setting simply override this
/// definition by applying your own definition line before invocation of this
/// header and compilation of file `i2c_sw.c`.
#ifndef F_I2C_SW
#   define F_I2C_SW 100000UL
#   warning "'F_I2C_SW' found undefined. Now defaults to 100kHz."
#endif


/// \brief
/// Default timeout to prevent hangups on bus contention. The unit is us (microseconds).
/// \details
/// According to the I<SUP>2</SUP>C bus definition a slower slave can keep SCL low to
/// synchronize the master to its slower speed.
///
/// A timeout is implemented to avoid hangups if the bus is disrupted or blocked.
/// If your application calls for another setting simply override this
/// definition by applying your own definition line before invocation of this
/// header and compilation of file `i2c_sw.c`.
#ifndef I2C_SW_EMERGENCY_TIMEOUT_us
#   define I2C_SW_EMERGENCY_TIMEOUT_us 20000UL
#   warning "'I2C_SW_EMERGENCY_TIMEOUT_us' found undefined. Now defaults to 20ms."
#endif


// =============================================================================
// Now that we know the hardware prerequisites the appropriate handling can be
// invoked.
// =============================================================================

/// \brief
/// Initializes the IO-lines and the bus.
/// \details
/// The intended IO-lines are set up to form the open-drain drivers of the SCL
/// and SDA bus lines.
/// A 'STOP' condition is sent to clean up possible power up artefacts.
///
/// If the bus can not be setup properly a failure code will be dropped.
/// This failure code can be checked by calling `softI2c_check4Error()`.
void softI2c_init(void);


/// \brief
/// Puts one byte to the bus, checks for acknowledge.
/// \details
/// One byte is sent to the bus using the I<SUP>2</SUP>C protocol.
/// The slave is expected to send an 'ACK' answer.
///
/// If an error occurs on the bus a failure code will be reported.
/// This failure code can be checked by calling `softI2c_check4Error()`.
/// \param dataByte to be sent to the slave.
void softI2c_putByte(uint8_t dataByte);


/// \brief
/// Returns one byte read from the bus.
/// \details
/// The slave is read using the I<SUP>2</SUP>C protocol.
/// According to the bus protocol the slave expects an 'ACK' to indicate subsequent
/// transfers. A slave receiving a 'NACK' response will assume that the actual 
/// data transfer is the final one (aka last byte).
///
/// If an error occurs on the bus a failure code will be reported.
/// This failure code can be checked by calling `softI2c_check4Error()`.
/// \param sendAck will enable sending an 'ACK' to the bus if set (!=0).
/// \returns
/// Byte read from bus slave.
uint8_t softI2c_getByte(uint8_t sendAck);


/// \brief
/// Addresses a certain slave. Returns its status.
/// \details
/// According to the I<SUP>2</SUP>C bus protocol a 'START' sequence is given.
/// Afterwards the slave address field plus the read/write bit is send.
/// All slaves will monitor this address field and the one whose address matches
/// will send an 'ACK'.
/// As this starts accessing a dedicated slave the failure code(s) of previous
/// accesses are cleared.
/// The intended typical usage is to address a slave for read or write access and
/// check that it is available. A code example might be like this:
/// \code
/// if (!softI2c_openDevice(I2C_EEPROM | I2C_READ_ACCESS))
///     ... // code executed when slave responds (and bus is okay)
/// else
///    ... // code executed when slave not responding (or bus not okay)
/// \endcode
/// \param address field of the desired slave including also the R/W-flag.
/// \returns
/// Status of access attempt.
/// \sa softI2c_check4Error()
enum I2C_FAILURE_type softI2c_openDevice(uint8_t address);


/// \brief
/// Releases the bus.
/// \details
/// The bus is released by sending the 'STOP' sequence.
///
/// If the bus can not be released a failure code will be dropped.
/// This failure code can be checked by calling `softI2c_check4Error()`.
void softI2c_releaseBus(void);


/// \brief
/// Returns the failure code(s) of the last bus access(es).
/// \details
/// Calling this function will retrieve the failure code(s) of the last bus
/// access(es).
/// The return values of this library part are:
///  + `I2C_SUCCESS`
///  + `I2C_NO_ACK`
///  + `I2C_TIMED_OUT`
///  + `I2C_SDA_BLOCKED`
///
/// If the returned value is not identical to `I2C_SUCCESS` then an error occured.
/// Depending on when the user code checks the status any combination (= binary
/// OR) of the resulting failure codes is possible.
/// To check the access status one can write
/// \code {.c}
/// if (softI2c_check4Error() == I2C_SUCCESS)
///     ... // code executed after successfull I2C transfer
/// else
///    ... // code executed when bus access not successful
/// \endcode
/// which in turn is equal to
/// \code {.c}
/// if (!softI2c_check4Error())
///     ... // code executed after successfull I2C transfer
/// else
///    ... // code executed when bus access not successful
/// \endcode
/// \returns
/// Failure code(s) occured during last bus access(es).
enum I2C_FAILURE_type softI2c_check4Error(void);


// Higher level of communication to a device. Address it, read/write
// 'messageSize' byte to/from 'buffer_p'.
// File: i2c_sw_access.c
//I2C_FAILURE_type softI2c_accessDevice(uint8_t address, uint8_t *buffer_p, uint16_t messageSize);


#endif // I2C_SW_H_INCLUDED


////////////////////////////////////////////////////////////////////////////////
//  Revision history:
//  -----------------
//
//  $Id:$
//
//  $Log:$
//
////////////////////////////////////////////////////////////////////////////////
