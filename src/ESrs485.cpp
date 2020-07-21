
/**************************************************************************/
/*!
  @file ESrs485.cpp
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


#include "ESrs485.h"

#if (defined(__AVR__) || defined(ESP8266)) && defined(USE_SW_SERIAL)
ESrs485::ESrs485(SoftwareSerial*ser) {
  common_init();
  SwSerial = ser;
}

ESrs485::ESrs485(SoftwareSerial*ser, int8_t enPin) {       // if using enable pin for RS485 and softwareserial
  common_init();
  SwSerial = ser;
  if (enPin >= 0)
  {
    _enablePin = enPin;
    pinMode(_enablePin, OUTPUT);
    digitalWrite(_enablePin, LOW);
    _en_pin = true;
  }
}
#endif

ESrs485::ESrs485(HardwareSerial *ser) {
  common_init();
  HwSerial = ser; // ...override HwSerial with value passed.

}

ESrs485::ESrs485(HardwareSerial *ser, int8_t enPin) {     // if using enable pin for RS485 and hardwareSerial.
  common_init();
  HwSerial = ser; //  HwSerial override with value passed.
  if (enPin >= 0)
  {
    _enablePin = enPin;
    pinMode(_enablePin, OUTPUT);
    digitalWrite(_enablePin, LOW);
    _en_pin = true;
  }

}

void ESrs485::begin(uint32_t baud)
{

#if (defined(__AVR__) || defined(ESP8266)) && defined(USE_SW_SERIAL)
  if (SwSerial) {
    SwSerial->begin(baud);
  }
#endif
  if (HwSerial) {
    HwSerial->begin(baud);
  }
}

float ESrs485::read_holding_reg(byte slave_id, unsigned int address, byte data_type, byte data_order, float gain)  // uses FC = 3
{
  received_data = 0;
  if (data_type == 1 || data_type == 5)
  {
    read_reg(slave_id, 3, address, 1, data_type, data_order, gain);
  }
  else
  {
    read_reg(slave_id, 3, address, 2, data_type, data_order, gain);
  }

  return received_data;
}

float ESrs485::read_input_reg(byte slave_id, unsigned int address, byte data_type, byte data_order, float gain)  // uses FC = 4
{
  received_data = 0;
  if (data_type == 1 || data_type == 5)
  {
    read_reg(slave_id, 4, address, 1, data_type, data_order, gain);
  }
  else
  {
    read_reg(slave_id, 4, address, 2, data_type, data_order, gain);
  }
  return received_data;
}

boolean ESrs485::write_single_reg(byte slave_id, unsigned int address,  int16_t writeValue)
{
  read_reg(slave_id, 6, address, writeValue , 2, 2, 2);
  return _write_data;
}

boolean ESrs485::write_single_coil(byte slave_id, unsigned int address, unsigned int writeValue)
{
  read_reg(slave_id, 5, address, writeValue , 2, 2, 2);
  return _write_data;
}

void ESrs485::read_reg(byte slave_id, byte function_code, unsigned int address, int16_t readUpto,  byte data_type, byte data_order, float gain)
{
  char dummy_ch;  // dummy char for dumping
  _write_data = false;  // make false before every write;
  byte receiveByte;
  if (data_type == 1 || data_type == 5 )
  {
    receiveByte = 7;
  }
  else
  {
    receiveByte = 9;
  }

  //address = address - offset;   // uncomment if you want address input to automatic -1 when callinf func.
  byte rxbuf[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  if (_en_pin == true)
  {
    digitalWrite(_enablePin, HIGH);
  }

  byte data[] = {slave_id, function_code, highByte(address), lowByte(address), highByte(readUpto), lowByte(readUpto), 0x00, 0x00};

  CRC(data, 8, 0);
  if (SwSerial)
  {
    SwSerial->flush();
    while (SwSerial->available() > 0)
    {
      dummy_ch = SwSerial->read();
    }
    SwSerial->write(data, 8);
    SwSerial->flush();
  }
  else
  {
    HwSerial ->flush();
    while (HwSerial ->available() > 0)
    {
      dummy_ch = HwSerial ->read();
    }
    HwSerial->write(data, 8);
    HwSerial ->flush();
  }

  if (_en_pin == true)
  {
    digitalWrite(_enablePin, LOW);
  }

  unsigned long oldmillis = millis();
  boolean timeout_error = false;
  if (SwSerial)
  {
    while (SwSerial->available() < 7)
    {
      if (millis() - oldmillis > _timeout_time)
      {
        //Serial.println("Read Timeout");
        timeout_error = true;
        break;
      }
    }

    while (SwSerial->available() > 0 && timeout_error == false)
    {
      SwSerial->readBytes(rxbuf, receiveByte);
    }
  }
  else
  {
    while (HwSerial->available() < 7)
    {
      if (millis() - oldmillis > _timeout_time)
      {
        //Serial.println("Read Timeout");
        timeout_error = true;
        break;
      }
    }

    while (HwSerial->available() > 0 && timeout_error == false)
    {

      HwSerial->readBytes(rxbuf, receiveByte);
	  //Serial.print(rxbuf[6],HEX);
      //Serial.println(rxbuf[7],HEX);
      //Serial.println("");
    }
  }

  if (timeout_error == false)
  {

    if (CRC(rxbuf, receiveByte, 1) == 1)
    {
      if (function_code == 3 || function_code == 4)
      {
        if (data_order == 1 && data_type == 1)   // uint16 AB
        {
          received_data = gain * ( rxbuf[3] << 8 | rxbuf[4]);
        }
        else if (data_order == 4 && data_type == 1)  // uint16 BA
        {
          received_data = gain * (rxbuf[4] << 8 | rxbuf[3]);
        }
        else if (data_order == 1 && data_type == 5) // int16 AB
        {
          Data.i16 = ( rxbuf[3] << 8 | rxbuf[4]);
          received_data = gain * Data.f;
        }
        else if (data_order == 4 && data_type == 5) // int16 BA
        {
          Data.i16 = ( rxbuf[4] << 8 | rxbuf[3]);
          received_data = gain * Data.f;
        }
        else if (data_order == 1 && readUpto == 2 )  //  High byte first  High word first ABCD
        {
          if ( data_type == 2) // uint32
          {
            received_data = gain * (rxbuf[3] << 24 | rxbuf[4] << 16 | rxbuf[5] << 8 | rxbuf[6]) ;
          }
          else if (data_type == 3) // float
          {
            Data.ui32 = (rxbuf[3] << 24 | rxbuf[4] << 16 | rxbuf[5] << 8 | rxbuf[6]);
            received_data = gain * Data.f;
          }
          else if (data_type == 4) // int32
          {
            Data.i32 = (rxbuf[3] << 24 | rxbuf[4] << 16 | rxbuf[5] << 8 | rxbuf[6]) ;
            received_data = gain * Data.f;
          }
        }
        else if (data_order == 2 && readUpto == 2 )  // 32bit  High byte first  Low word first   CDAB
        {
          if (data_type == 2)
          {
            received_data = gain * (rxbuf[5] << 24 | rxbuf[6] << 16 | rxbuf[3] << 8 | rxbuf[4]);
          }
          else if (data_type == 3)
          {
            Data.ui32 = (rxbuf[5] << 24 | rxbuf[6] << 16 | rxbuf[3] << 8 | rxbuf[4]);
            received_data = gain * Data.f;
          }
          else if (data_type == 4) // int32
          {
            Data.i32 = (rxbuf[5] << 24 | rxbuf[6] << 16 | rxbuf[3] << 8 | rxbuf[4]) ;
            received_data = gain * Data.f;
          }
          //  Serial.println("Hb  Lw");
          // Serial.println(received_data);
        }
        else if (data_order == 3 && readUpto == 2 )  // 32bit  Low byte first  High word first    BADC
        {
          if (data_type == 2)
          {
            received_data = gain * (rxbuf[4] << 24 | rxbuf[3] << 16 | rxbuf[6] << 8 | rxbuf[5]);
          }
          else if (data_type == 3)
          {
            Data.ui32 = (rxbuf[4] << 24 | rxbuf[3] << 16 | rxbuf[6] << 8 | rxbuf[5]);
            received_data = gain * Data.f;
          }
          else if (data_type == 4) // int32
          {
            Data.i32 = (rxbuf[4] << 24 | rxbuf[3] << 16 | rxbuf[6] << 8 | rxbuf[5]) ;
            received_data = gain * Data.f;
          }
          // Serial.println("Lb  Hw");
          // Serial.println(received_data);
        }
        else if (data_order == 4 && readUpto == 2 )  // 32bit  Low byte first  Low word first   DCBA
        {
          if (data_type == 2)
          {
            received_data = gain * (rxbuf[6] << 24 | rxbuf[5] << 16 | rxbuf[4] << 8 | rxbuf[3]);
          }
          else if (data_type == 3)
          {
            Data.ui32 = (rxbuf[6] << 24 | rxbuf[5] << 16 | rxbuf[4] << 8 | rxbuf[3]);
            received_data = gain * Data.f ;
          }
          else if (data_type == 4) // int32
          {
            Data.i32 = (rxbuf[6] << 24 | rxbuf[5] << 16 | rxbuf[4] << 8 | rxbuf[3]) ;
            received_data = gain * Data.f;
          }
          //  Serial.println("Lb  Lw");
          //  Serial.println(received_data);
        }
      }
      else if (function_code == 6 || function_code == 5)
      {
        for (int i = 0; i < 8; i++)
        {
          if (rxbuf[i] != data[i])
          {
            _write_data = false;
            break;
          }
          else
          {
            _write_data = true;
          }
        }
      }
    }
    else
    {
      //Serial.println("CRC FAIL... ");
      received_data = -88.88;
    }
  }
  else
  {
    //Serial.println("timeout ");
    received_data = -99.99;
  }

}



unsigned int ESrs485::CRC(byte* buf, byte len, byte chk)  // for generating and validating CRC
{
  _crc = 0xffff;
  _crcPos = 0;
  for (_crcPos = 0; _crcPos < (len - 2); _crcPos++)
  {
    _crc ^= (unsigned int)buf[_crcPos];
    for (int i = 8; i != 0; i--)
    {
      if ((_crc & 0x0001) != 0)
      {
        _crc = _crc >> 1;
        _crc = _crc ^ 0xA001;
      }
      else
        _crc = _crc >> 1;
    }
  }
  if (chk == 0)
  {
    buf[_crcPos] = _crc & 0x00ff;
    buf[_crcPos + 1] = _crc >> 8;
    return 1;
  }
  else
  {
    if (buf[_crcPos] == (_crc & 0x00ff))
    {
      if (buf[_crcPos + 1] == ((_crc >> 8) & (0x00FF)))
        return 1;
    }
    else
    {
      return 0;
    }
  }
}



void ESrs485::common_init(void) {
#if (defined(__AVR__) || defined(ESP8266)) && defined(USE_SW_SERIAL)
  SwSerial = NULL; // Set both to NULL, then override correct
#endif
  HwSerial = NULL; // port pointer in corresponding constructor
}
