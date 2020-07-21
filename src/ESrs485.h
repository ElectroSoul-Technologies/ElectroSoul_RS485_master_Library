
/**************************************************************************/
/*!
  @file ESrs485.h
  @mainpage ElectroSoul RS485 master liberary
  @section intro Introduction
  This is the ElectroSoul RS485 master liberary which supports RS485 modbus communication.
  Tested and works great with the ElectroSoul RS485 4CH Relay card  @model: ES-RIO-0400-R & ElectroSoul ESP32-RS485 modbus gateway
  
  ElectroSoul invests time and resources providing this open source code,
  please support ElectroSoul and open-source hardware by purchasing
  products from ElectroSoul!
  @section author Author
  Written by Dharmik for ElectroSoul Technologies.
  @section license License
  
   Copyright (C) 2020 ElectroSoul Technologies (https://electrosoul.in/)
 
   This software is released under the GNU General Public License version 3,
   which covers the main part of arduino-cli.
   The terms of this license can be found at:
   https://www.gnu.org/licenses/gpl-3.0.en.html
   You can be released from the requirements of the above licenses by purchasing
   a commercial license. Buying such a license is mandatory if you want to modify or
   otherwise use the software for commercial activities involving the Arduino
   software without disclosing the source code of your own applications. To purchase
   a commercial license, send an email to github@electrosoul.in.
*/
/**************************************************************************/

#ifndef ESrs485_h
#define ESrs485_h

#include "Arduino.h"
	
#define USE_SW_SERIAL  // coment this if not using software serial library
/*
#if (defined(__AVR__) || defined(ESP8266)) && defined(USE_SW_SERIAL)
#include <SoftwareSerial.h>
#endif
*/

#include <SoftwareSerial.h>

//#define read_holding_reg 3
//#define read_input_reg 4
//#define write_single_coil 5
//#define write_single_reg 6

// data order
#define big_endian 1           //ABCD
#define mid_little_endian 2    //CDAB
#define mid_big_endian 3       //BADC
#define little_endian 4        //DCAB

//data types
#define floats 3
#define uint32 2
#define int32  4
#define uint16 1
#define int16  5

// for Function code 5
#define ON 0xFF00
#define OFF 0x0000
#define TOGGLE 0x5500



class ESrs485 {

  public:
    //constructor
#if (defined(__AVR__) || defined(ESP8266)) && defined(USE_SW_SERIAL)
    ESrs485(SoftwareSerial *ser); // Constructor when using SoftwareSerial
    ESrs485(SoftwareSerial *ser, int8_t enPin);
#endif
    ESrs485(HardwareSerial *ser); // Constructor when using HardwareSerial
    ESrs485(HardwareSerial *ser , int8_t enPin);
    // Method /functions




    void begin(uint32_t baud);
    float read_input_reg(byte slave_id, unsigned int address, byte data_type, byte data_order, float gain); // uses FC = 4
    float read_holding_reg(byte slave_id, unsigned int address, byte data_type, byte data_order, float gain);  // uses FC = 3
    boolean write_single_reg(byte slave_id, unsigned int address, int16_t writeValue); // uses FC=6
    boolean write_single_coil(byte slave_id, unsigned int address, unsigned int writeValue); //uses FC=5
    unsigned int CRC(byte* buf, byte len, byte chk);
  private:
  
/*
#if (defined(__AVR__) || defined(ESP8266)) && defined(USE_SW_SERIAL)
    SoftwareSerial *SwSerial;
#endif
*/
    SoftwareSerial *SwSerial;
    HardwareSerial *HwSerial;

    void common_init(void);
    void read_reg(byte slave_id, byte function_code, unsigned int address, int16_t readUpto, byte data_order, byte data_type, float gain);

    float received_data;
    boolean _write_data;

    byte _request_data[8]; // request packet
    boolean _en_pin;// to know wheather enable pin is used or not
    int8_t _enablePin;  // which pin is in use as enable

    unsigned int _crc; // variable used for crc function
    unsigned int _crcPos;// variable used for crc function
    #define _timeout_time 1000  // change this if you want more timeout	
    union
    {
      uint32_t ui32;
      int32_t i32;
      uint16_t ui16;
      int16_t i16;
      float f;
    } Data;
	
};












#endif
