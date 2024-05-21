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
 *************************************************************************************************************************
 * Process WiFi Page
 *************************************************************************************************************************
*/
void wifiPage(uint8_t button)
{
#if defined WIFI
  if(message.startsWith("SS"))
  { 
    WiFiEnabled = 0;
    writeEEPROMByte(eeWiFiEnabled, WiFiEnabled);                //WiFi default
    writeEEPROMName(eeSSID, message.substring(2));
    ssid = message.substring(2);
    updateWiFi();
    return;
  }
  if(message.startsWith("PA"))
  {
    WiFiEnabled = 0;
    writeEEPROMByte(eeWiFiEnabled, WiFiEnabled);                //WiFi default
    writeEEPROMName(eePWD, message.substring(2));
    password = message.substring(2);
    return;
  }
  if(message.startsWith("IP"))
  {
    WiFiEnabled = 0;
    writeEEPROMByte(eeWiFiEnabled, WiFiEnabled);                //WiFi default
    writeEEPROMName(eeIPAddr, message.substring(2));
    host = message.substring(2);
    return;
  }
  if(message.startsWith("PO"))
  {
    WiFiEnabled = 0;
    writeEEPROMByte(eeWiFiEnabled, WiFiEnabled);                //WiFi default
    writeEEPROMAddr(eePort, message.substring(2).toInt());
    port = message.substring(2).toInt();
    return;
  }
  if(message.startsWith("RC"))
  {
    writeEEPROMByte(eeWiFiRetries, message.substring(2).toInt());
    retries = message.substring(2).toInt(); 
    return;
  }
  switch (button)
  {
    case ConfigButton:
      if(!EEPROM.commit()) console.println("EEPROM.commit Failed"); 
      initPage(ConfigPage);
      break;
    case Done_Press:
      saveCredentials();
      if(!EEPROM.commit()) console.println("EEPROM.commit Failed");
      initPage(MenuPage);
      break;
    case Scan_Press:
      WiFiEnabled = 0;
      writeEEPROMByte(eeWiFiEnabled, WiFiEnabled);
      nextionSetValue("WiFi",0);
      listName = SSIDs;
      returnPage = WiFiPage;
      initPage(SelectionPage);
      break;
    case  RetryButton:
      WiFiEnabled = 1;
      writeEEPROMByte(eeWiFiEnabled, WiFiEnabled);
      nextionSetText("Progress", "Retrying...");
      if(!EEPROM.commit()) console.println("EEPROM.commit Failed");
      updateWiFi();
      break;
    case WiFiEnabledON:
      WiFiEnabled = 1;
      writeEEPROMByte(eeWiFiEnabled, WiFiEnabled);
      nextionSetText("Progress", "Enabling WiFi...");
      updateWiFi();
      break;
    case WiFiEnabledOFF:
      WiFiEnabled = 0;
      writeEEPROMByte(eeWiFiEnabled, WiFiEnabled);                //WiFi default
      nextionSetText("Progress", "Disabling WiFi...");
      updateWiFi();
      break;
    default:
      break;
    }
  #endif
}
