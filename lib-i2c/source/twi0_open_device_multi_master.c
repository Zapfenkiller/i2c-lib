////////////////////////////////////////////////////////////////////////////////
// File        : twi0_open_device_multi_master.c
// Project     : Roboter 2006
// Author      : R. Trapp
// Initial date: 9. Aug. 2012
// Release rev.: 
// Copyright   : (c) 2006-2013 R. Trapp / H.A.R.R.Y.
// License     : 
// Description : Address one slave using multi master mode - TWI.
////////////////////////////////////////////////////////////////////////////////


#include "i2c_hw.h"
#if defined(I2C_HW_TWI_H_INCLUDED) && defined(I2C0_HW_AS_MASTER) && !defined(I2C0_HW_SINGLE_MASTER)
#include "i2c_lib_private.h"
#include <avr/io.h>

// Address a certain slave. Return its status. This also occupies the bus.
enum I2C_FAILURE_type twi0_open_device_multi_master(uint8_t deviceAddress)
{
//uint8_t twistat = I2C0_HW_STATUS_REG & (0b11111<<TWS3);
    switch (I2C0_HW_STATUS_REG & (0b11111<<TWS3))
    {
        case 0x08:  // 00001 START already successfully sent
        case 0x10:  // 00010 repeated START already successfully sent
            break;
        default:
            // Assert start condition - TWI-HW will wait for the next opportunity
            I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | (1 << TWSTA) | slaveAckControl;
            // Wait until done.
            while (!(I2C0_HW_CONTROL_REG & (1 << TWINT))) {}
    }
    // Datasheet requires to clear TWSTA after START has been sent!
    I2C0_HW_CONTROL_REG &= ~((1 << TWINT) | (1 << TWSTA));
    switch (I2C0_HW_STATUS_REG & (0b11111<<TWS3))
    {
        case 0x08:  // 00001 START successfully sent
        case 0x10:  // 00010 repeated START successfully sent
            // Try to send out the addressfield
            twi0_send_byte(deviceAddress);
            switch (I2C0_HW_STATUS_REG & (0b11111<<TWS3))
            {
                case 0x18:  // 00011 address + W successfully sent
                case 0x40:  // 01000 address + R successfully sent
                    i2c0_failure_info = I2C_SUCCESS;
                    break;
                case 0x20:  // 00100 address + W sent, but no ACK
                case 0x48:  // 01001 address + R sent, but no ACK
                    i2c0_failure_info = I2C_NO_ACK;
                    break;
                case 0x38:  // 00111 arbitration lost, not addressed by remote master
                case 0x68:  // 01101 arbitration lost, own address + W received while acting as master
                case 0x78:  // 01111 arbitration lost, general call address received while acting as master
                case 0xB0:  // 10110 arbitration lost, own address + R received while acting as master
                    i2c0_failure_info = I2C_RESTARTED | I2C_ARBITRATION_LOST;
                    break;
                case 0x00:
                    I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | (1 << TWSTO) | slaveAckControl;
                default:
                    i2c0_failure_info = I2C_PROTOCOL_FAIL;
            }
            return(i2c0_failure_info);
        case 0x60:  // 01100 own address + W received
        case 0x70:  // 01110 general call address received
        case 0xA8:  // 10101 own address + R received
            i2c0_failure_info = I2C_RESTARTED | I2C_ARBITRATION_LOST;
            return(i2c0_failure_info);
        case 0x00:
            I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | (1 << TWSTO) | slaveAckControl;
        default:
            i2c0_failure_info = I2C_PROTOCOL_FAIL;
    }
    return(i2c0_failure_info);
}

/*
Proposal - not finished yet!

    switch (I2C0_HW_STATUS_REG & (0b11111<<TWS3))
    {
        case 0x18:  // 00011 address + W successfully sent
        case 0x20:  // 00100 address + W sent, but no ACK
        case 0x28:  // 00101 data successfully sent
        case 0x30:  // 00110 data sent, but no ACK received
        case 0x38:  // 00111 arbitration lost, not addressed by remote master
        case 0x40:  // 01000 address + R successfully sent
        case 0x48:  // 01001 address + R sent, but no ACK
        case 0x50:  // 01010 data received, ACK returned to bus
        case 0x58:  // 01011 data received, NACK returned to bus
        case 0x60:  // 01100 own address + W received
        case 0x68:  // 01101 arbitration lost, own address + W received while acting as master
        case 0x70:  // 01110 general call address received
        case 0x78:  // 01111 arbitration lost, general call address received while acting as master
        case 0x80:  // 10000 addressed, last byte received with ACK
        case 0x88:  // 10001 addressed, last byte received not ACKed
        case 0x90:  // 10010 general call, last byte received with ACK
        case 0x98:  // 10011 general call, last byte received not ACKed
        case 0xA0:  // 10100 STOP or repeated START condition
        case 0xA8:  // 10101 own address + R received
        case 0xB0:  // 10110 arbitration lost, own address + R received while acting as master
        case 0xB8:  // 10111 byte sent, ACK received
        case 0xC0:  // 11000 byte sent, no ACK received
        case 0xC8:  // 11001 last byte sent, no ACK expected but ACK received

        case 0x08:  // 00001 START successfully sent
        case 0x10:  // 00010 repeated START successfully sent
            break;

            i2c0_failure_info = I2C_RESTARTED; // here means "bus is occupied"
            I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | (1 << TWSTA) | slaveAckControl;
            return(i2c0_failure_info);
        case 0xD0:  // 11010 unused?
        case 0xD8:  // 11011 unused?
        case 0xE0:  // 11100 unused?
        case 0xE8:  // 11101 unused?
        case 0xF0:  // 11110 unused?
        case 0xF8:  // 11111 TWI idle or in transfer, TWINT is '0' here!
            // Assert start condition - TWI-HW will wait for the next opportunity
            I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | (1 << TWSTA) | slaveAckControl;
            // Wait until done.
            while (!(I2C0_HW_CONTROL_REG & (1 << TWINT))) {}
            break;
        case 0x00:
            I2C0_HW_CONTROL_REG = (1 << TWEN) | (1 << TWINT) | (1 << TWSTO) | slaveAckControl;
        default:
            i2c0_failure_info = I2C_PROTOCOL_FAIL;
            return(i2c0_failure_info);
    }
*/

/*
        case 0x00:  // 00000 Bus error (illegal START or STOP)
        case 0x08:  // 00001 START successfully sent
        case 0x10:  // 00010 repeated START successfully sent
        case 0x18:  // 00011 address + W successfully sent
        case 0x20:  // 00100 address + W sent, but no ACK
        case 0x28:  // 00101 data successfully sent
        case 0x30:  // 00110 data sent, but no ACK received
        case 0x38:  // 00111 arbitration lost, not addressed by remote master
        case 0x40:  // 01000 address + R successfully sent
        case 0x48:  // 01001 address + R sent, but no ACK
        case 0x50:  // 01010 data received, ACK returned to bus
        case 0x58:  // 01011 data received, NACK returned to bus
        case 0x60:  // 01100 own address + W received
        case 0x68:  // 01101 arbitration lost, own address + W received while acting as master
        case 0x70:  // 01110 general call address received
        case 0x78:  // 01111 arbitration lost, general call address received while acting as master
        case 0x80:  // 10000 addressed, last byte received with ACK
        case 0x88:  // 10001 addressed, last byte received not ACKed
        case 0x90:  // 10010 general call, last byte received with ACK
        case 0x98:  // 10011 general call, last byte received not ACKed
        case 0xA0:  // 10100 STOP or repeated START condition
        case 0xA8:  // 10101 own address + R received
        case 0xB0:  // 10110 arbitration lost, own address + R received while acting as master
        case 0xB8:  // 10111 byte sent, ACK received
        case 0xC0:  // 11000 byte sent, no ACK received
        case 0xC8:  // 11001 last byte sent, no ACK expected but ACK received
        case 0xD0:  // 11010 unused?
        case 0xD8:  // 11011 unused?
        case 0xE0:  // 11100 unused?
        case 0xE8:  // 11101 unused?
        case 0xF0:  // 11110 unused?
        case 0xF8:  // 11111 TWI idle or in transfer, TWINT is '0' here!
*/


#endif /* defined(I2C_HW_TWI_H_INCLUDED) && defined(I2C0_HW_AS_MASTER) && !defined(I2C0_HW_SINGLE_MASTER) */
