/*
   This library can be used with both Software serial & hardware serial.
   This example shows you how to Read Input registors(FC=04) from slave device with slave ID 1 and baud 9600, Data bit 8, parity none, stop bit 1

   This example uses Hardware serial port
   You can directly use this code in our ElectroSoul ESP32-RS485 modbus gateway as it uses hardware Serial2 as UART - RS485 communication.
*/

/***********************************************************************
  Written by Dharmik for ElectroSoul Technologies.
  Copyright (C) 2020 ElectroSoul Technologies (https://electrosoul.in/)
  gpl-3.0, all text above must be included in any redistribution
 ***********************************************************************/
 
#include <ESrs485.h>

#define RS485_slave1_port Serial2       //you can use any hardware serial port just define here

//use below function like this if not using an enable pin for RS485, else comment it
ESrs485 ES_RS485 (&RS485_slave1_port);  //calling defined port to ESrs485 lib

//if using enable pin for RS485 uncoment below code and comment above line
/*
  #define RS485_enable_pin 1      // define your digital enable pin over here
  ESrs485 ES_RS485 (&RS485_slave1_port,RS485_enable_pin);
*/

float received_value;  // variable for storing Received value over RS485
unsigned int address1 = 100;  // address you want to read for this example we will read Holding Registe address 100
float gain = 1;   // gain value will be multiplied with the orignal value recived from slave and after multiplying it will return
void setup() {
  Serial.begin(9600);
  ES_RS485.begin(9600);                 //define baud here for your RS485
  // Note: Make sure that slave device is working on same baud rate.
  //       right now library only supports data bit 8, parity none & stop bit 1

}

void loop() {

  //read_holding_reg(slave_ID,address,Data_type,Data_order,gain);

  // here 1 is slave ID
  // address1 = 100

  //Data_types formate which slave will give response we can use following
  // floats
  // uint32
  // int32
  // uint16
  // int16

  //Data_order order in which slave will send bytes
  //let say slave gives 32-bit data then there will be four byte data let say A B C D  & if 16-bit data will be two byte A B
  // So Data orders can be classified as
  // big_endian             (32-bit AB CD)(16-bit AB) High byte first   &   High word first
  // mid_big_endian         (32-bit BA CD)            Low  byte  firt   &   High word first
  // mid_little_endian      (32-bit CD AB)            High byte first   &   Low  word first
  // little_endian          (32-bit DC BA)(16-bit BA) Low  byte first   &   Low  word first

  // gain value will be multiplied with the orignal value recived from slave and after multiplying it will return

  received_value = ES_RS485.read_input_reg(1, address1, floats, big_endian, gain); // uses Function Code 04
  if (received_value == -99.99)
  {
    Serial.println("TIME OUT ERROR.....");
  }
  else if (received_value == -88.88)
  {
    Serial.println("CRC FAIL.....");
  }
  else
  {
    Serial.print("Value: ");
    Serial.println(received_value);
  }

  delay(1000);
}
