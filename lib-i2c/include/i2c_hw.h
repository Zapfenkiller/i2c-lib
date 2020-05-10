////////////////////////////////////////////////////////////////////////////////
// File        : i2c_hw.h
// Description : Definitions for using I²C hardware. Uniform interface to
//               application code, independent of underlying hardware.
//               Based on older version of 2006.
//               These identifiers need to be defined already for the library
//               routines to work if master mode is selected. They get adjusted
//               to default values if missing:
//                  F_I2C0_HW
//                  I2C0_HW_EMERGENCY_TIMEOUT_us
//               These identifiers are required to define the operation mode.
//               At least one of both has to be set (=defined) for proper usage.
//               If both are set then the device is capable to act as master and
//               as slave simultanously.
//                  I2C0_HW_AS_SLAVE
//                  I2C0_HW_AS_MASTER
//               To further improve code size the flag
//                  I2C0_HW_SINGLE_MASTER
//               can be set to rule out multi master related parts.
// Author      : R. Trapp
// Project     : I²C-Lib
// Initial date: Aug /  2 / 2012
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
/// \mainpage
/// When working with microcontroller projects quite often the need arises to
/// interface to existing standards.
/// Depending on the elected microcontroller's capabilities the software has
/// to be adapted or completely rewritten.
/// Also the API (Application Programming Interface) has to be reinvented with
/// every new project.
/// Not to mention that every change of the software needs time consuming
/// (re)qualification and evaluation to proof its desired functionality and
/// its reliability.
///
/// The aim of the I<SUP>2</SUP>C library project is to give the software engineer
/// a set of already tested routines masking the microcontroller's hardware
/// differences by using a standardized API.
/// Thus it is possible to reuse code already written (and tested) for a specific
/// microcontroller on a different model without great effort.
/// For example it makes no difference using an ATtiny2313 offering USI for an
/// implementation of a certain I<SUP>2</SUP>C slave functionality and then change
/// over to an ATmega168. Of course, some code sections need to be adapted to
/// fulfill this task but the section dealing with the two wire bus interface can
/// be reused without changes.
///
/// While development of this library took place - in fact it is based on a couple
/// of projects using TWI, USI or even pure software emulation - it turned out
/// that due to the similarity of the physical base layers most of the routines
/// could be used on both I<SUP>2</SUP>C bus or SMBus. But all the stuff is
/// currently only tested to comply with I<SUP>2</SUP>C standards.
///
/// See i2c_hw.h on how to get using this library for your project(s).
/// \todo Document the properties of the bus masters and slaves.
/// Especially emphasize the clock stretching possibilities that
/// the I²C spec just sets to be _optional_ for all devices.
/// \todo Extract the doxygen `mainpage` section into a separate
/// file.
/// \todo Try to use the wording of the I²C-spec throughout this
/// documentation.
///
/// \file   i2c_hw.h
/// \brief
/// Include this header file for implementing a hardware supported I<SUP>2</SUP>C
/// bus master or slave.
/// \details
/// Based on an older version of 2006 the I<SUP>2</SUP>C library had been rewritten.
/// To use the I<SUP>2</SUP>C functions provided you have to include this header
/// file into your sources.
///
/// In fact the I<SUP>2</SUP>C hardware support library consists of two major
/// sub parts:
///  + Devices having TWI hardware support,
///  + Devices having USI hardware support.
///
/// To facilitate programming and code reuse both sub parts define an identical
/// API (Application Programming Interface). So - hopefully - a working project
/// can be copied over to another microcontroller without massive effort rewriting
/// already tested I<SUP>2</SUP>C parts.
///
/// The library is adjusted automatically to the hardware support facilities of
/// the microcontroller selected.
///
/// Although the library keeps the programmer from worrying about the underlying
/// hardware support there still is the need to properly define the operation
/// mode.
/// Senseful modes supported by this library are:
///  + Multi master plus slave response
///  + Multi master without slave response
///  + Single master (always without slave response)
///  + Slave response only
///
/// Selection of the desired operation mode is controlled by a couple of
/// compiler flags:
///  + `I2C0_HW_AS_SLAVE`
///  + `I2C0_HW_AS_MASTER`
///  + `I2C0_HW_SINGLE_MASTER`
///
/// Using any master operation mode requires some further prerequisites to
/// adjust its properties. If any of the following definitions is omitted a
/// default value will be assumed and a warning message written:
///  + `I2C0_HW_EMERGENCY_TIMEOUT_us`
///  + `F_I2C0_HW`
///
/// Of course, for the timing to be as desired, `F_CPU` has to be defined 
/// to the oscillator frequency effectively used.
/// All I<SUP>2</SUP>C timings depend on this definition.
///
/// The following table shows the valid mapping and some examples, where the
/// values shown in the 'Example' column are literally to be taken as examples
/// only - you substitue your own required values:
/// <TABLE>
/// <TR>
/// <TD><b>Operating Mode</b></TD>
/// <TD><b>Necessary Defines</b></TD>
/// <TD><b>Example</b></TD>
/// </TR>
/// <TR>
/// <TD>Single Master</TD>
/// <TD>`I2C0_HW_AS_MASTER`\n`I2C0_HW_SINGLE_MASTER`</TD>
/// <TD>\code
/// #define I2C0_HW_EMERGENCY_TIMEOUT_us 250
/// #define F_I2C0_HW 100000UL
/// #define F_CPU 2000000UL
///
/// #define I2C0_HW_AS_MASTER
/// #define I2C0_HW_SINGLE_MASTER
///
/// #include "i2c_hw.h"
/// \endcode</TD>
/// </TR>
/// <TR>
/// <TD>Multiple Master\n(without slave!)</TD>
/// <TD>`I2C0_HW_AS_MASTER`</TD>
/// <TD>\code
/// #define I2C0_HW_EMERGENCY_TIMEOUT_us 250
/// #define F_I2C0_HW 100000UL
/// #define F_CPU 2000000UL
///
/// #define I2C0_HW_AS_MASTER
///
/// #include "i2c_hw.h"
/// \endcode</TD>
/// </TR>
/// <TR>
/// <TD>Multiple Master + Slave</TD>
/// <TD>`I2C0_HW_AS_MASTER`\n`I2C0_HW_AS_SLAVE`</TD>
/// <TD>\code
/// #define I2C0_HW_EMERGENCY_TIMEOUT_us 250
/// #define F_I2C0_HW 100000UL
/// #define F_CPU 2000000UL
///
/// #define I2C0_HW_AS_MASTER
/// #define I2C0_HW_AS_SLAVE
///
/// #include "i2c_hw.h"
/// \endcode</TD>
/// </TR>
/// <TR>
/// <TD>Slave</TD>
/// <TD>`I2C0_HW_AS_SLAVE`</TD>
/// <TD>\code
/// #define I2C0_HW_AS_SLAVE
///
/// #include "i2c_hw.h"
/// \endcode</TD>
/// </TR>
/// </TABLE>
///
/// All of the library routines report their status but only few return it directly.
/// A dedicated function is available to report the failure code(s) of the last
/// access.
/// This scheme has been implemented after several trials with the more official
/// way of having the status returned by each funtion.
/// It turned out that having each function returning its status requires both
/// more code FLASH size and more RAM cells.
/// Keeping in mind the small amount of RAM of some controllers the current
/// implementation seems to be the best compromise of coding style and resource
/// requirements.
/// <HR>
/// \par TWI
/// Using this library on a controller featuring TWI hardware you simply have to
/// include this header file and have your project linked against the already 
/// prepared `libi2ctwi.a` library.
/// Do not forget to tell the linker about it.
/// You use the routines and functions already defined by this header and are
/// done.
/// If the need arises `libi2ctwi.a` can be recompiled from scratch using the
/// makefile provided.
/// \todo
/// File i2c_hw.h - doxygen part:
/// Improve this description and prepare a makefile for building the library.
/// Is it really valid to prepare the .a file? This means all TWI modules need
/// to share the same register set, the same bit locations and the same addresses,
/// of course!
///
/// <HR>
/// \par USI
/// Using this library on a controller featuring USI hardware needs more
/// attention than dealing with TWI.
/// Although USI gives some features important considering the I<SUP>2</SUP>C
/// bus protocol it leaves major work to the software.
/// In addition the different controllers have their SCL and SDA wires assigned
/// on different ports.
/// So there is no way having an already  prepared library without code size and
/// processing speed penalty.
/// Considering the low FLASH memory size resources of some controllers typically
/// employing USI it has been decided to compile the library individually as a
/// part of the project.
/// This puts more effort on the programmer but frees some of the microcontroller's
/// resources.
/// You have to tell the make tool to compile the library in addition to your
/// own project source files. Since this library is already prepared and checked
/// to work, this step typically is necessary only once when setting up the
/// project.
/// \todo
/// Give an explanation of the intended flow (also give an example makefile - 
/// the testcase, maybe?):
/// the user has to recompile the library
/// after applying all the necessary settings for his project - similar to the
/// way you install wxWidgets to your machine. But here it is done on a "per
/// project" base. Benefit should be much better acceptance of the library by
/// it's users - avoiding complicated compilation instructions frustrating
/// the user when nearly always failing due to wrong files selected. Proper
/// preparation of the lib build will always give the opportunity to have all
/// opmodes available and leave it to the linker to take out only the functions
/// really needed. => Easy usage, easy handling, least possible code size.
/// \par
/// This workflow has been selected here because of its flexibility to adapt to
/// assignments of SCL and SDA of the microcontroller's USI-IO-lines
/// without code size penalty.
/// \par
/// These identifiers need to be defined already for the library routines to
/// compile if the selected microcontroller supports USI:
///  + `I2C0_HW_SDA_PORT` defines the port SDA is connected to (e.g. PORTB).
///  + `I2C0_HW_SDA_DDR` defines the data direction control of the port SDA is
///    connected to (e.g. DDRB).
///  + `I2C0_HW_SDA_READBACK` defines the IO status readback of the port SDA is
///     connected to (e.g. PINB).
///  + `I2C0_HW_SDA_BIT` defines the bit number of the IO-line SDA is connected
///    to (e.g. 4).
///  + `I2C0_HW_SDA_BITPOS` defines the bitposition to use when manipulating SDA
///    (e.g. (1 << I2C0_SW_SDA_BIT)).
///  + `I2C0_HW_SCL_PORT` defines the port SCL is connected to (e.g. PORTD).
///  + `I2C0_HW_SCL_DDR` defines the data direction control of the port SCL is
///    connected to (e.g. DDRD).
///  + `I2C0_HW_SCL_READBACK` defines the IO status readback of the port SCL is
///     connected to (e.g. PIND).
///  + `I2C0_HW_SCL_BIT` defines the bit number of the IO-line SCL is connected
///    to (e.g. 2).
///  + `I2C0_HW_SCL_BITPOS` defines the bitposition to use when manipulating SCL
///    (e.g. (1 << I2C0_SW_SCL_BIT)).
/// \par
/// These definitions can be done in a dedicated project header or in the
/// makefile to define your project.
/// Having them in the makfile also used to build the library avoids having to
/// edit a second file (i2c_hw.h for example). The centralized approach by the
/// makefile is expected to be more robust against forgetting to adjust the
/// second place unintendedly.
/// \par
/// As soon as the library is compiled according to your project needs you 
/// simply have to include this header file and have your project linked against
/// the just prepared `libi2cusi.a` library.
/// Do not forget to tell the linker about it.
/// You use the routines and functions already defined by this header and are
/// done.
/// <HR>
/////////////////////////////////////////////////////////////////////////////


#ifndef I2C_HW_H_INCLUDED
#define I2C_HW_H_INCLUDED


#include <inttypes.h>
#include "i2c_def.h"


// All operation modes:
// --------------------

/// \brief
/// Initializes the hardware in accordance to the operation mode.
/// \details
/// Initializes the supporting hardware (TWI or USI) to operate as intended.
/// The preassigned IO-lines are set up to form the open-drain drivers of the
/// SCL and SDA bus lines.
///
/// Depending on the mode of operation and depending on the underlying hardware
/// support, some parameters are invalid and can be replaced by dummy values.
///
/// Example when using Multi Master + Slave mode on a TWI enabled micro:
/// \code
/// #define I2C0_HW_AS_MASTER
/// #define I2C0_HW_AS_SLAVE
///
/// #include "i2c_hw.h"
///
/// #define SLAVE_ADDRESS       0xB8
/// #define SLAVE_ADDRESS_MASK  0xFC
///
/// int main (void)
/// {
///     ...
///     hardI2c0_init(SLAVE_ADDRESS, SLAVE_ADDRESS_MASK, I2C0_BITRATE, I2C0_PRESCALER);
///     ...
/// }
/// \endcode
/// \note
/// The `I2C0_BITRATE` and `I2C0_PRESCALER` definitions are predefined by the
/// I²C library. You can override them by simply substituting your preferred
/// settings.
///
/// Example when using Single Master mode:
/// \code
/// #define I2C0_HW_AS_MASTER
/// #define I2C0_HW_SINGLE_MASTER
///
/// #include "i2c_hw.h"
///
/// int main (void)
/// {
///     ...
///     hardI2c0_init(dummy, dummy, I2C0_BITRATE, I2C0_PRESCALER);
///     ...
/// }
/// \endcode
/// Example when using Slave mode, responding also to a general call:
/// \code
/// #define I2C0_HW_AS_SLAVE
///
/// #include "i2c_hw.h"
///
/// #define SLAVE_ADDRESS       0xB9
/// #define SLAVE_ADDRESS_MASK  0xFC
///
/// int main (void)
/// {
///     ...
///     hardI2c0_init(SLAVE_ADDRESS, SLAVE_ADDRESS_MASK, dummy, dummy);
///     ...
/// }
/// \endcode
/// Finally an example when using Slave mode on a device without mask support:
/// \code
/// #define I2C0_HW_AS_SLAVE
///
/// #include "i2c_hw.h"
///
/// #define SLAVE_ADDRESS       0xB8
///
/// int main (void)
/// {
///     ...
///     hardI2c0_init(SLAVE_ADDRESS, dummy, dummy, dummy);
///     ...
/// }
/// \endcode
/// \param slaveAddress assignes the address to react on as slave.
/// According to the datasheets bit 0 decides if also listening to the general
/// call address.
/// Valid only if `I2C0_HW_AS_SLAVE` is defined, ignored otherwise.
/// \param slaveMask assignes a mask to set some address bits to 'don't care' thus
/// getting an address range instead of a single address.
/// This parameter might be not implemented on some controllers (e.g. ATmega8).
/// Check the datasheet on TWI properties of your selected device.
/// Valid only if `I2C0_HW_AS_SLAVE` is defined, ignored otherwise.
/// \param bitrate defines the fine prescaling to achieve the desired bus
/// frequency.
/// To aid programming `I2C0_BITRATE` is derived automatically from
/// `F_I2C0_HW` and `F_CPU` definition.
/// Works only if `I2C0_HW_AS_MASTER` is defined on a TWI micro, ignored otherwise.
/// \param prescaler defines the coarse prescaling to achieve the desired bus
/// frequency.
/// To aid programming `I2C0_PRESCALER` is derived automatically from
/// `F_I2C0_HW` and `F_CPU` definition.
/// Works only if `I2C0_HW_AS_MASTER` is defined on a TWI micro, ignored otherwise.
/// \note
/// Please be aware that setting the bitrate on the fly is only possible when TWI
/// hardware support is used. Using USI the bitrate is hardcoded when the lib is
/// build and thus fixed when running the bus.
/// \todo
/// bitrate and prescaler eventually should get omitted completely
/// from the init. USI does not support it at all.
/// (see also ToDo @ F_I2C0_HW)
void hardI2c0_init(uint8_t slaveAddress, uint8_t slaveMask, uint8_t bitrate, uint8_t prescaler);


/// \brief
/// Returns the failure code(s) of the last bus access(es).
/// \details
/// Calling this function will retrieve the failure code(s) of the last bus
/// access(es).
/// Please see the documentation of ::I2C_FAILURE_type in file i2c_def.h for
/// return values available.
///
/// If the returned value is not identical to `I2C_SUCCESS` then an error occured.
/// Depending on when the user code checks the status any combination (= binary
/// OR) of the resulting failure codes is possible.
/// To check the access status one can write
/// \code {.c}
/// if (hardI2c0_check4Error() == I2C_SUCCESS)
///     ... // code executed after successfull I2C transfer
/// else
///    ... // code executed when bus access not successful
/// \endcode
/// which in turn is equal to
/// \code {.c}
/// if (!hardI2c0_check4Error())
///     ... // code executed after successfull I2C transfer
/// else
///    ... // code executed when bus access not successful
/// \endcode
/// \returns Failure code(s) occured during last bus access(es).
enum I2C_FAILURE_type hardI2c0_check4Error(void);


// Dedicated to any master mode:
// -----------------------------

#if defined I2C0_HW_AS_MASTER || defined DOXYGEN_DOCU_IS_GENERATED

/// \brief
/// Puts one byte to the bus, checks for acknowledge.
/// Dedicated to any master mode usage.
/// \details
/// One byte is sent to the bus using the I<SUP>2</SUP>C protocol.
/// The slave is expected to send an 'ACK' answer.
///
/// If an error occurs on the bus a failure code will be reported.
/// This failure code can be checked by calling `hardI2c0_check4Error()`.
/// \note This function is only available when `I2C0_HW_AS_MASTER`
/// indicates any master mode usage.
/// \param dataByte to be sent to the already addressed slave.
/// \sa hardI2c0_openDevice()
/// \sa hardI2c0_getByteAsSlave()
void hardI2c0_putByteAsMaster(uint8_t dataByte);


/// \brief
/// Returns one byte read from the bus.
/// Dedicated to any master mode usage.
/// \details
/// The slave is read using the I<SUP>2</SUP>C protocol.
/// According to the bus protocol the slave expects an 'ACK' to
/// indicate subsequent transfers.
/// A slave receiving a 'NACK' response will assume that the actual
/// data transfer is the final one (aka last byte).
///
/// If an error occurs on the bus a failure code will be reported.
/// This failure code can be checked by calling `hardI2c0_check4Error()`.
/// \note This function is only available when `I2C0_HW_AS_MASTER`
/// indicates any master mode usage.
/// \param sendAck enables sending an 'ACK' to the bus if set (!=0).
/// \returns Byte read from bus slave.
/// \sa hardI2c0_putByteAsSlave()
uint8_t hardI2c0_getByteAsMaster(uint8_t sendAck);


/// \brief
/// Addresses a certain slave. Returns access status.
/// Dedicated to any master mode usage.
/// \details
/// According to the I<SUP>2</SUP>C bus protocol a 'START' sequence
/// is given.
/// Afterwards the slave address field plus the read/write bit is
/// send as supplied by `deviceAddress`.
/// All slaves will monitor this address field and the one whose
/// address matches will send an 'ACK'.
/// As this starts accessing a dedicated slave the failure code(s)
/// of previous accesses are cleared.
/// The intended typical usage is to address a slave for subsequent
/// read or write access and check that it is available.
/// A code example might be like this:
/// \code
/// if (!hardI2c0_openDevice(I2C_EEPROM | I2C_READ_ACCESS))
///     ... // code executed when slave responds (and bus is okay)
/// else
///    ... // code executed when slave not responding (or bus not okay)
/// \endcode
/// If a multi-master mode is selected then there is additional
/// return information to consider:
/// `I2C_ARBITRATION_LOST` generally indicates that another master won the bus
/// access, `I2C_ARBITRATION_LOST && !I2C_RESTARTED` indicates the loss happens
/// during a data byte.
/// `I2C_ARBITRATION_LOST && I2C_RESTARTED` indicates that the loss
/// of arbitration happened inside the address field and thus this
/// master might be accessed as a slave when multi-master-plus-slave
/// operation is desired.
/// A more complex code example for multi-master-plus-slave mode might be like this:
/// \code
/// if (!hardI2c0_openDevice(I2C_EEPROM | I2C_READ_ACCESS))
///     ... // code executed when slave responds (and bus is okay)
/// else
///     // code executed when slave not responding (or bus not okay)
///     if ((hardI2c0_check4Error() & I2C_ARBITRATION_LOST)
///         if (hardI2c0_check4Error() & I2C_RESTARTED)
///         {
///             ... // bus master arbitration lost in address field
///             ... // check immediately if own address received and react properly
///         }
///         else
///         {
///             ... // arbitration lost in data field, other slave is accessed
///             ... // wait until STOP to try master access again - or whatever
///             ... // the application needs to perform now
///         }
/// \endcode
/// \note This function is only available when `I2C0_HW_AS_MASTER` indicates
/// any master mode usage.
/// \param deviceAddress of the desired slave including also the R/W-flag.
/// \returns Status of access attempt.
/// \sa hardI2c0_check4Error()
/// \sa hardI2c0_releaseBus()
enum I2C_FAILURE_type hardI2c0_openDevice(uint8_t deviceAddress);


/// \brief
/// Releases the bus.
/// Dedicated to any master mode usage.
/// \details
/// The bus is released by sending the 'STOP' sequence.
///
/// If the bus can not be released a failure code will be dropped.
/// This failure code can be checked by calling `hardI2c0_check4Error()`.
/// \note This function is only available when `I2C0_HW_AS_MASTER`
/// indicates any master mode usage.
/// \sa hardI2c0_openDevice()
void hardI2c0_releaseBus(void);

#endif


// Dedicated to any slave mode:
// ----------------------------

#if defined I2C0_HW_AS_SLAVE || defined DOXYGEN_DOCU_IS_GENERATED

/// \brief
/// Indicates if addressed as slave.
/// Dedicated to slave mode usage.
/// \details
/// Checks if the bus master has sent an assigned address. This function
/// does not wait for the device to get addressed. Thus it is possible to
/// poll for access attempts of a master and resume another task if not addressed.
/// \note This function is only available when `I2C0_HW_AS_SLAVE` indicates
/// slave mode usage.
///
/// \note Depending on the hardware support the parameter `slaveMask` is mandatory
/// (USI) or just a dummy (TWI). If dummy then its contents is dropped.
///
/// Here is an example that also shows how to find out about the subaddress sent
/// if an address mask is used:
/// \code
/// #define deviceTwiBaseAddress    0xA0
/// #define addr0wr                 deviceTwiBaseAddress
/// #define addr0rd                 (deviceTwiBaseAddress | I2C_READ_ACCESS)
/// #define addr1wr                 (deviceTwiBaseAddress + (1 << 2))
///     ...
///     while (...)
///     {
///         ...
///         uint8_t addressReceived;
///         if (hardI2c0_isAddressedAsSlave(uint8_t &addressReceived, 0x0F))
///         {
///             ... // code executed when assigned address (range) encountered
///             switch (addressReceived)
///             {
///                 case addr0wr:
///                     // code executed when writing to 'addr0'
///                     break;
///                 case addr0rd:
///                     // code executed when reading from 'addr0'
///                     break;
///                 case addr1rd:
///                     // code executed when reading from 'addr1'
///                     break;
///                 case 0:
///                     // code executed at general call write access
///                     break;
///                 case 1:
///                     // code executed at general call read access
///                     break;
///                 default:
///                     // code executed when any remaining address is accessed
///                     // - remaining addresses of assigned address range!
///             }
///         }
///         else
///             ... // code executed when other address or no address at all received
///         ...
///     }
/// \endcode
/// In this particular example the application responds to device addresses in
/// the range of 0x50 to 0x57 (pattern 0xA0 to 0xAF) and also to the general
/// call address 0 (read and write response).
/// \param addressReceived contains the address field sent by the master. Thus
/// the R/W-flag is still contained also!
/// \param slaveMask assignes a mask to set some address bits to 'don't care' thus
/// getting an address range instead of a single address. A bit set to '1' identifies
/// its mating address bit position as 'dont care'. A slaveMask of 0x00 espcially
/// indicates exact match of one assigned address and no general call reponse.
/// Bit 0 enables general call recognition if set.
/// Valid only if `I2C0_HW_AS_SLAVE` is defined.
/// \returns Addressee state: 0 if not addressed, !=0 if addressed.
uint8_t hardI2c0_isAddressedAsSlave(uint8_t *addressReceived, uint8_t slaveMask);


/// \brief
/// Puts one byte to the bus, checks for acknowledge.
/// Dedicated to slave mode usage.
/// \details
/// According to the bus protocol the slave expects an 'ACK' to indicate subsequent
/// transfers. A slave receiving a 'NACK' response will assume that the actual 
/// data transfer has been the final one (aka last byte).
///
/// If an error occurs on the bus a failure code will be reported.
/// This failure code can be checked by calling `hardI2c0_check4Error()`.
/// \note This function is only available when `I2C0_HW_AS_SLAVE` indicates
/// slave mode usage.
/// \param dataByte to be sent to the master.
/// \param expectAck sets expectation on master response. An 'ACK' is expected
/// if set (!=0).
/// \sa hardI2c0_getByteAsMaster()
void hardI2c0_putByteAsSlave(uint8_t dataByte, uint8_t expectAck);


/// \brief
/// Returns one byte read from the bus.
/// Dedicated to slave mode usage.
/// \details
/// According to the bus protocol the master expects an 'ACK' to indicate a
/// successfull transfer.
/// A slave responding 'NACK' violates the bus definition.
/// In less strict implementation responding 'NACK' to the master might
/// be exploited as an enhancement to communications, e.g. as a 'try again
/// later' or a 'slave is busy now'.
///
/// If an error occurs on the bus a failure code will be reported.
/// This failure code can be checked by calling `hardI2c0_check4Error()`.
/// Possible failure codes are
///  + `I2C_PROTOCOL_FAIL` which means that something strange appeared on the bus.
///  + `I2C_STOPPED` which is strictly not a failure condition but indicates that
///     the bus master now has released the bus and thus slave access. In any case
///     a slave should return to address field hunting mode.
///
/// \note This function is only available when `I2C0_HW_AS_SLAVE` indicates
/// slave mode usage.
/// \param sendAck enables sending an 'ACK' to the bus if set (!=0).
/// \returns Byte read from bus master.
/// \sa hardI2c0_putByteAsMaster()
uint8_t hardI2c0_getByteAsSlave(uint8_t sendAck);


/// \brief
/// Enables or disables slave response.
/// Dedicated to slave mode usage.
/// \details
/// Enables or disables slave response to its assigned address range.
/// Thus it is possible to connect or disconnect a slave from the bus by the
/// application software.
/// This connection or disconnection is done in a logical fashion; the device
/// stays physically connected to the bus all the time.
/// \note This function is only available when `I2C0_HW_AS_SLAVE` indicates
/// slave mode usage.
/// \param on enables the slave to respond to its assigned address(es) when set (!=0).
void hardI2c0_setSlaveResponse(uint8_t on);

#endif


// =============================================================================
// Check for bus frequency definition.
// =============================================================================

/// \brief
/// Default timeout to prevent hangups on bus contention. The unit is us (microseconds).
/// \details
/// According to the I<SUP>2</SUP>C bus definition a slower slave can keep SCL low to
/// synchronize the master to its slower speed.
///
/// A timeout is implemented to avoid hangups if the bus is disrupted or blocked.
/// If your application calls for another setting simply override this
/// definition by applying your own definition line before invocation of this
/// header.
///
/// Example:
/// \code
/// define I2C0_HW_EMERGENCY_TIMEOUT_us 250UL   // 250 us
///
/// #include "i2c_hw.h"
/// \endcode
#if !defined I2C0_HW_EMERGENCY_TIMEOUT_us || defined DOXYGEN_DOCU_IS_GENERATED
#   define I2C0_HW_EMERGENCY_TIMEOUT_us 20000UL
#   if defined I2C0_HW_AS_MASTER
#       warning "'I2C0_HW_EMERGENCY_TIMEOUT_us' found undefined. Now defaults to 20 ms."
#   endif
#endif


/// \brief
/// Default bus operating frequency. The unit is Hz (Hertz).
/// \details
/// This prerequisite defines the default maximum bus speed to be used - if not
/// defined already.
///
/// If your application calls for another setting simply override this
/// definition by applying your own definition line before invocation of this
/// header.
///
/// Example:
/// \code
/// define F_I2C0_HW    400000UL   // 400 kHz
///
/// #include "i2c_hw.h"
/// \endcode
/// \todo
/// When USI is used this does not work. The definition then shall
/// be given already in the Makefile! Generally the Makefile is
/// the better location for this. Since with USI the bitrate is
/// fixed by compilation, does it make sense to keep the TWI open
/// for such a change on the fly?
#if !defined F_I2C0_HW || defined DOXYGEN_DOCU_IS_GENERATED
#   define F_I2C0_HW 100000UL
#   if defined I2C0_HW_AS_MASTER
#       warning "'F_I2C0_HW' found undefined. Now defaults to 100 kHz."
#   endif
#endif


// Just for Doxygen :(
#if defined DOXYGEN_DOCU_IS_GENERATED
#include "i2c_hw_usi.h"
#include "i2c_hw_twi.h"
#endif


// =============================================================================
// All AVR devices with USI:
// =============================================================================

#if defined (__AVR_ATtiny2313__) || \
   defined (__AVR_ATtiny26__) || \
   defined (__AVR_ATmega169__)
#  include "i2c_hw_usi.h"

// =============================================================================
// All AVR devices with TWI:
// =============================================================================

#elif defined (__AVR_ATmega8__) || \
   defined (__AVR_ATmega48__) || \
   defined (__AVR_ATmega88__) || \
   defined (__AVR_ATmega168__) || \
   defined (__AVR_ATmega328__) || \
   defined (__AVR_ATmega32U4__)
#  include "i2c_hw_twi.h"

// =============================================================================
// All AVR devices without any I2C hardware support, or yet unknown devices end
// up here.
// =============================================================================

#else
#   error "MCU type not supported yet by I2C library (or no MCU type defined at all)."
#endif


#endif // I2C_HW_H_INCLUDED
