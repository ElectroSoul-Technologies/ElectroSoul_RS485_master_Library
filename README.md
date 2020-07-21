# ElectroSoul RS485 Modbus Master Library for Arduino IDE

This is ElectroSoul RS485 Modbus Master Library which works great and has been tested on ElectroSoul ESP32-RS485 Modbus Gateway for Monitoring as well as Controlling various Devices over RS485.

This Library make everything very simple for establishing an Industrial Standard RS485 Modbus Communication with our ESP32-RS485 Modbus Gateway or any other Arduino compatible board in which you can use TTL to RS485 modules available.
<!-- -->
## Development Status
Basic Requirenment are already set for an Industrial RS485 standards with the Library so that you can directly use the library for your Arduino + RS485 projects.

**UART Support**
  * Library supports Hardware UART ✅
  * Software Serial Library is also supported ✅
  * Can set different baud rates ✅
  * Data Bit ❌
  * Parity   ❌
  * Stop Bit ❌


**Direction Control for RS485 Transceivers**
  * Any digital pin can be used as Direction control pin ✅


**RS485 Function Code(FC) Supported By Library**
  * Read Holding Registers (FC=03)✅  
     * read single register✅
     * read multiple register❌
  * Read Input Registers (FC=04)✅
     * read single register✅
     * read multiple register❌
  * Write Single Coil (FC=05)✅
  * Write Single Register (FC=06)✅
  * Read Coil Status (FC=01)❌
  * Read Input Status (FC=02)❌
  * Write Multiple Coils (FC=15)❌
  * Write Multiple Registers (FC=16)❌


**Additional Library Features**
  * Can generate CRC ✅
  * Validate CRC in receiving packet (Note: if CRC fail you will get -88.88) ✅
  * You can set Timeout from .h file default 1 Sec (Note: if  Timeout ocures you will get -99.99) ✅
  
  
# Products that use this Library 

* [ElectroSoul ESP32-RS485 Modbus Gateway](https://www.tindie.com/products/electrosoul/esp32-rs485-modbus-gateway/)- ElectroSoul ESP32-RS485 Modbus Gateway is an small and low power easy to use Industrial standard RS485 Modebus Gateway. You can use our device and liberary to make your Arduino Based Industrial 4.0 project.
You can easily fetch data over RS485 and also write data with RS485 to any slave devices which supports RS485 such as MFM(multi function meter), AC drive, Solar Inverters etc. For more technical detail about [ElectroSoul ESP32-RS485 Gateway.](https://github.com/ElectroSoul-Technologies/ESP32-RS485_Gateway)


**Repository Contents**

* **/examples** - Example sketches for the library (.ino). Run these from the Arduino IDE. 
* **/src** - Source files for the library (.cpp, .h).
* **library.properties** - General library properties for the Arduino package manager.
* **LICENSE.md** - Library has open source license gpl-3.0
* **keywords.txt** - Library Keywords file
* **CODE_OF_CONDUCT.md** - Our code of conduct
* **CONTRIBUTING.md** - contribution info
* **README.MD**

License Information
-------------------

This Liberary is _**open source**_! 

Please review the LICENSE.md file for license information. 

If you have any questions or concerns on licensing, please contact techsupport@electrosoul.in

Distributed as-is; no warranty is given.



  
  
  
  


