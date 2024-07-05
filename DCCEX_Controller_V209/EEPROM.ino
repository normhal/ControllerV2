/***********************************************************************
*                  
* COPYRIGHT (c) 2024 Norman Halland (NormHal@gmail.com)
*
*  This program and all its associated modules is free software: 
*  you can redistribute it and/or modify it under the terms of the 
*  GNU General Public License as published by the Free Software 
*  Foundation, either version 3 of the License, or (at your option) 
*  any later version.
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see http://www.gnu.org/licenses
*
************************************************************************/
/* 
 * ESP32/8266/PICO EMULATED EEPROM MEMORY MAP
 * ------------------------------------------
 * - 4095 bytes in Total size (max possible)
 * 
 * LOCOMOTIVES
 * 50 Locomotives each using: 
 * - 2 bytes for Address              =  2    (100) 
 * - A Road Name of 9 Characters      =  9    (450)
 * - A Loco Type of 9 Characters      =  9    (450)
 * - A Road Number of 2 Bytes         =  2    (100)   (0 - 65535)
 * Total bytes per Loco description   = 22    (1100)
 * - Function Block for 10 functions
 *   - Each function uses:   
 *     - 1 byte for Function Number   = 10
 *     - 1 byte for Image Number      = 10           (High Order bit indicates momentary)
 *     Total bytes used for functions = 20    (1000)
 * Total bytes needed for each Loco     = 42   
 * Total for 50 Locos     (50 x 42)     2100  (2100)               
 * 
 * ACCESSORIES
 * 96 Accessories each using:          
 * - 2 bytes for Address              =  2
 * - 9 bytes for Name                 =  9
 * - 1 byte for Type                  =  1
 * - 1 byte for Image                 =  1
 * Total Bytes used by each Accessory   = 13
 * Total for 96 Accessories (96 x 13)   1248
 * 
 * Routes
 * 48 Routes each using:
 * - 6 x acc/state pairs (2 bytes)    = 12
 * Total bytes for each Route           = 12
 * Total for 48 Routes                   576
 * 
 * Total Bytes for all devices          2100
 *                                      1248
 *                                       576
 *                                      ----
 *                                      3924
 * 
 ***********************************************************************************************************************************
 * Routine to Initialize Emulated EEPROM
 ***********************************************************************************************************************************
  #define eeUseExRoster (eepromEnd - 119)     //4095 - (10  119 = 3976
  #define eeSelIDsD     (eepromEnd - 118)     //4095 - (10) 118 = 3977  10 Selected IDs
  #define eeSelIDsC     (eepromEnd - 108)     //4095 - (10) 108 = 3987  10 Selected IDs
  #define eeSelIDsB     (eepromEnd - 98)      //4095 - (10) 98 = 3997  10 Selected IDs
  #define eeSelIDsA     (eepromEnd - 88)      //4095 - (10) 88 = 4007  10 Selected IDs
  #define eeJoinMode    (eepromEnd - 78)      //4095 - (1)  78 = 4017
  #define eeSSID        (eepromEnd - 77)      //4095 - (24) 77 = 4018  24 Max 24 characters //This overlaps the UNO's Route locations
  #define eePWD         (eepromEnd - 53)      //4095 - (24) 53 = 4042  24 Max 24 characters
  #define eeIPAddr      (eepromEnd - 29)      //4095 - (16) 29 = 4066  16 Max 16 characters xxx.xxx.xxx.xxx
  #define eePort        (eepromEnd - 13)      //4095 - (2)  13 = 4082  2 Max - 1 uint16_t
  #define eeLocoStopAll (eepromEnd - 11)      //4095 - (1)  11 = 4084
  #define eeWiFiSeconds (eepromEnd - 10)      //4095 - (1)  10 = 4085
  #define eeRNumEnabled (eepromEnd - 9)      //4095 - (1)   9 = 4086
  #define eeAccDelay    (eepromEnd - 8)      //4095 - (1)   8 = 4087  Delay between Accessory Route Commands
  #define eeWiFiEnabled (eepromEnd - 7)      //4095 - (1)   7 = 4088
  #define eeThreshold   (eepromEnd - 6)       //4095 - (1)  6  = 4089
  #define eeREIncrement (eepromEnd - 5)       //4095 - (1)  5  = 4090
  #define eePUState     (eepromEnd - 4)       //4095 - (1)  4  = 4091
  #define eeDCCRefresh  (eepromEnd - 3)       //4095 - (1)  3  = 4092
  #define eeActiveSlot  (eepromEnd - 2)       //1023 - (1)  2  = 4093
  #define eeWiFiRetries (eepromEnd - 1)       //4095 - (1)  1  = 4094
  #define eeCheckByte   eepromEnd             //4095 - (1)  0  = 4095

  writeEEPROMByte(eeJoinMode, JoinButton);
  writeEEPROMByte(eeRNumEnabled, RNumEnabled);
  writeEEPROMByte(eeWiFiRetries, WiFiRetries);
  writeEEPROMByte(eeActiveSlot, activeSlot);
  writeEEPROMByte(eeThreshold, ReverseThreshold);
  writeEEPROMByte(eeREIncrement, REAccAmount);
  writeEEPROMByte(eePUState, PowerUpState);
  writeEEPROMByte(eeDCCRefresh, DCCRefresh);
  writeEEPROMByte(eeWiFiEnabled, WiFiEnabled);                //WiFi default
  writeEEPROMByte(eeAccDelay, AccDelay);
  writeEEPROMByte(eeWiFiSeconds, wifiSeconds);
  writeEEPROMByte(eeLocoStopAll, LocoStopAll);
 ***********************************************************************************************************************************
*/
void initEEPROM()
{
  uint8_t j = 255;
  for (uint16_t i = eeSelIDsA; i < (eeSelIDsA + numLocoSlots); i++)
  {
    writeEEPROMByte(i ,j);  //Initialize selectedIDs with no IDs
  }
  writeEEPROMByte(eeJoinMode, JOIN_OPTION);
  writeEEPROMByte(eeRNumEnabled, RNUM_ENABLED);
  writeEEPROMByte(eeWiFiRetries, WiFiRetries);
  writeEEPROMByte(eeActiveSlot, activeSlot);
  writeEEPROMByte(eeThreshold, ReverseThreshold);
  writeEEPROMByte(eeREIncrement, REAccAmount);
  writeEEPROMByte(eePUState, PowerUpState);
  writeEEPROMByte(eeDCCRefresh, DCCRefresh);
  writeEEPROMByte(eeWiFiEnabled, WiFiEnabled);                //WiFi default
  writeEEPROMByte(eeAccDelay, AccDelay);
  writeEEPROMByte(eeWiFiSeconds, wifiSeconds);
  writeEEPROMByte(eeLocoStopAll, LocoStopAll);
 
  //Initialize every Road Number to 0
  for (int i = locoRNumBase; i < (locoRNumBase + (numLocos * 2)); i++) writeEEPROMByte(i, 0);  //the *2 is  because of a two byte address

  //Initialize every Loco Address to 0
  for (int i = locoAddrBase; i < (locoAddrBase + (numLocos * 2)); i++) writeEEPROMAddr(i, 0);  //the *2 is  because of a two byte address

  //Initialize all functions to 127 and images to "Blank" (10)
  for(int i = locoFuncBase; i < accAddrBase; i = i+2){
    writeEEPROMByte(i, 127);
    writeEEPROMByte(i+1, BLANK);
  }

  //Now for the Accessory Addresses to 0 and Images to "Blank" (10)
  for(int i = accAddrBase; i < (accImageBase); i++) writeEEPROMByte(i, 0);
  for(int i = accImageBase; i < (routeListBase); i++) writeEEPROMByte(i, BLANK);
  // Routes?
  saveCredentials();
  setupSelected();

  writeEEPROMByte(eepromEnd, EEPROMCODE);   //Indicate that EEPROM has now been initialized
}
/*
 **************************************************************************************************************************************
 * Save WiFi Credentials
 **************************************************************************************************************************************
*/
void saveCredentials()
{
  #if defined WIFI
    writeEEPROMByte(eeWiFiEnabled, WiFiEnabled);                //WiFi default
    writeEEPROMByte(eeWiFiRetries, WiFiRetries);
    writeEEPROMName(eeSSID, ssid.c_str());
    writeEEPROMName(eePWD, password.c_str());
    writeEEPROMName(eeIPAddr, host.c_str());
    writeEEPROMAddr(eePort, port);                              //2 byte Int
  #endif
  if(!EEPROM.commit()) Console.println("EEPROM.commit Failed");
}
/*
 **************************************************************************************************************************************
 * Read WiFi Credentials
 **************************************************************************************************************************************
*/
void readCredentials()
{
  #if defined WIFI
    WiFiEnabled = readEEPROMByte(eeWiFiEnabled);                //WiFi default
    WiFiRetries = readEEPROMByte(eeWiFiRetries);
    ssid = readEEPROMName(eeSSID);
    password = readEEPROMName(eePWD);
    host = readEEPROMName(eeIPAddr);
    port = readEEPROMAddr(eePort);                              //2 byte Int
  #endif
}
/*
 **************************************************************************************************************************************
 * EEPROM Read Routines - for non-Nextion EEPROM
 *  Address of Item to be read is calculated as follows:
 *  locoAddrBase + (locoID * addrLen) eg. 7th Loco will have an ID of 6 (       = 6 * 2  = 12 + locoAddrBase   = 0+12     =   12
 *  locoNameBase + (locoID * (locoNameLen)) ESP Only
 *  accAddrBase  + (accID * addrLen) eg. 20th Accessory will have an ID of 19  = 19 * 2 = 38 + accAddrBase    = 440+38   =  478
 *  accNameBase  + (accID * (accNameLen)  ESP Only 
 *  routeAddrBase + (routeID * routeLen)eg. 5th Route will have an ID of 4       = 4 * 12 = 48 + routeAddrBase  = ...+48   =  648
 * 
 **************************************************************************************************************************************
*/
uint16_t readLocoAddress(uint8_t locoID)
{
  if(readEEPROMByte(eeUseExRoster) == 0)
  {
    uint16_t eeAddress = (locoAddrBase + (locoID * addrLen));
    uint8_t uint8_t1 = EEPROM.read(eeAddress);
    uint8_t uint8_t2 = EEPROM.read(eeAddress + 1);
    return (uint8_t1 << 8) + uint8_t2;
  }else
  {
    return (rosterList[locoID]);
  }
}
/*
 **************************************************************************************************************************************
*/
uint16_t readLocoRNum(uint8_t locoID)
{
  uint16_t eeAddress = (locoRNumBase + (locoID * addrLen));
  uint8_t uint8_t1 = EEPROM.read(eeAddress);
  uint8_t uint8_t2 = EEPROM.read(eeAddress + 1);
  return (uint8_t1 << 8) + uint8_t2;
}
/*
 **************************************************************************************************************************************
 * Read Accessory Address from EEPROM
 * Accessory Addresses are stored in Arduino/ESP EEPROM from address 440(accAddrBase) to 536 - 2 bytes each
 **************************************************************************************************************************************
*/
uint16_t readAccAddress(uint8_t accID)
{
  uint16_t eeAddress = (accAddrBase + (accID * 2));
  uint8_t uint8_t1 = EEPROM.read(eeAddress);
  uint8_t uint8_t2 = EEPROM.read(eeAddress + 1);
  return (uint8_t1 << 8) + uint8_t2;
}
/*
 **************************************************************************************************************************************
 * Read Accessory Image number form EEPROM
 * Accessory Images are stored in Arduino/ESP EEPROM from address 536 to 584 - 1 byte each
 **************************************************************************************************************************************
*/
uint8_t readAccImage(uint8_t accID)
{
  uint16_t eeAddress = (accImageBase + accID);
  uint8_t uint8_t1 = EEPROM.read(eeAddress);
  return (uint8_t1);
}
/*
 ***********************************************************************************************************************************
 * Read a String from Arduino or ESP EEPROM
 * Last Byte is null character
 * Some Names are stored at random locations
 ***********************************************************************************************************************************
*/
String readEEPROMName(uint16_t eeAddress)
{
  char data [21];
  uint8_t i;
  for (i = 0; i < 21; i++)
  {
    data[i] = EEPROM.read(eeAddress + i);
    if(data[i] == '\0') break;
  }
  return String(data);
}
/*
 ***********************************************************************************************************************************
 * Read 2 bytes from a specific EEPROM location
 ***********************************************************************************************************************************
*/
uint16_t readEEPROMAddr(uint16_t eeAddress)
{
  uint8_t uint8_t1 = EEPROM.read(eeAddress);
  uint8_t uint8_t2 = EEPROM.read(eeAddress + 1);
  return (uint8_t1 << 8) + uint8_t2;
}
/*
 ***********************************************************************************************************************************
 * Read 1 byte from a specific EEPROM location
 ***********************************************************************************************************************************
*/
uint8_t readEEPROMByte(uint16_t eeAddress)
{
  uint8_t eeData = EEPROM.read(eeAddress);
  return eeData;
}
/*
 ***********************************************************************************************************************************
 * EEPROM Read Routines - for Nextion integrated EEPROM
 * 
 * readAccName
 * 
 *  - The passed ID is used to calculate the address to be read
 *  - the passed Slot is used to determine which slot the Name must be moved into. (0 to 11)
 *  - for the sake of consistency, slot numbers or Names, Addresses and Images will always be 0 to 11
 * 
 * Nextion Address Slots range from t0 to t11
 * Nextion Name Slots range from t20 to t31
 * Nextion Image Slots range from t40 to t51
 ***********************************************************************************************************************************
*/
void writeAccAddress(uint8_t ID, uint16_t Address)
{ 
  uint16_t eeAddress = (accAddrBase+(ID * 2));
  uint8_t byte1 = Address >> 8;
  uint8_t byte2 = Address & 0xFF;
  EEPROM.write(eeAddress, byte1);
  EEPROM.write(eeAddress + 1, byte2);
}
/*
 ***********************************************************************************************************************************
 * EEPROM Write Routines - for Arduino Integrated EEPROM
 * Loco Addresses are stored in Arduino/ESP EEPROM from address 0 to 39 - 2 bytes each
 ***********************************************************************************************************************************
*/
void writeLocoAddress(uint8_t locoID, uint16_t locoAddress)
{ 
  uint16_t eeAddress = (locoAddrBase+(locoID * 2));
  uint8_t byte1 = locoAddress >> 8;
  uint8_t byte2 = locoAddress & 0xFF;
  EEPROM.write(eeAddress, byte1);
  EEPROM.write(eeAddress + 1, byte2);
}
void writeLocoRNum(uint8_t locoID, uint16_t locoRNum)
{ 
  uint16_t eeAddress = (locoRNumBase+(locoID * 2));
  uint8_t byte1 = locoRNum >> 8;
  uint8_t byte2 = locoRNum & 0xFF;
  EEPROM.write(eeAddress, byte1);
  EEPROM.write(eeAddress + 1, byte2);
}
/*
 ***********************************************************************************************************************************
*/
void writeEEPROMByte(uint16_t eeAddress, uint8_t eeData)
{
  EEPROM.write(eeAddress, eeData);
}
/*
 ***********************************************************************************************************************************
 * Write a String to Arduino or ESP EEPROM
 ***********************************************************************************************************************************
*/

void writeEEPROMName(uint16_t eeAddress, const String &Name)
{
  uint8_t i;
  uint8_t len = Name.length();
  for (i = 0; i < len; i++) EEPROM.write(eeAddress + i, Name[i]);
  EEPROM.write(eeAddress + i, '\0');
}
/*
 ***********************************************************************************************************************************
 * Write a 2 uint8_t "Address" to EEPROM
 ***********************************************************************************************************************************
*/
void writeEEPROMAddr(uint16_t eeAddress, uint16_t wordToWrite)
{ 
  uint8_t uint8_t1 = wordToWrite >> 8;
  uint8_t uint8_t2 = wordToWrite & 0xFF;
  EEPROM.write(eeAddress, uint8_t1);
  EEPROM.write(eeAddress + 1, uint8_t2);
}
/*
 ***********************************************************************************************************************************
 * Initialize Selected IDS as 0 to 9 and Active Loco Slot to 0
 ***********************************************************************************************************************************
*/
void setupSelected()
{
  uint8_t c = 0;
  for (uint16_t s = eeSelIDsA; s < (eeSelIDsA+numLocoSlots); s++)
  {
    writeEEPROMByte(s, c);
    c++;
  }
  writeEEPROMByte(eeActiveSlot,0);
  if(!EEPROM.commit()) Console.println("EEPROM.commit Failed");
}
/*
 ***********************************************************************************************************************************
 * Save Selected IDS and Active Loco 
 ***********************************************************************************************************************************
*/
void saveSelected()
{
  uint8_t c = 0;
  for (uint16_t s = eeSelIDsA; s < (eeSelIDsA+numLocoSlots); s++)
  {
    writeEEPROMByte(s, selectedIDs[thNum][c]);
    c++;
  }
  writeEEPROMByte(eeActiveSlot,activeSlot);
  if(!EEPROM.commit()) Console.println("EEPROM.commit Failed");
}
/*
 ***********************************************************************************************************************************
 * Restore Selected IDS and Active Loco 
 ***********************************************************************************************************************************
*/
void restoreSelected()
{
  uint8_t r = 0;
  for (uint16_t s = eeSelIDsA; s < (eeSelIDsA + numLocoSlots); s++)
  {
    selectedIDs[thNum][r] = readEEPROMByte(s);
    r++;
  }
}
