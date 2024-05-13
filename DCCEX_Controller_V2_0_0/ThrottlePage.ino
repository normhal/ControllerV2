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
 * Process Throttle Page
 *************************************************************************************************************************
*/
void throttlePage(uint8_t button)
{
  if(message.startsWith("Th"))
  {
//    Serial.println(message);
    uint8_t newSpeed = (message.substring(2)).toInt();
//    Serial.printf("New Speed: %d", newSpeed);
//    Serial.println();
    auto th = throttles[activeSlot];
    Loco *activeLoco = th->getLoco();
    if(selectedIDs[activeSlot] != 255)        
    encoderPos = newSpeed;
    activeLoco->setSpeed(newSpeed);
    dccexProtocol.setThrottle(activeLoco, newSpeed, activeLoco->getDirection());
    return;
  }
  switch(button)                  
  {
    case AccButton:
      nextionCommand("AD.bco=" + String(GREY));
      guestActive = false;        
      AccReturnPage = ThrottlePage;
      initPage(AccPage);
      break;
    case LocosButton:
      nextionCommand("AD.bco=" + String(GREY));
      guestActive = false;        
      LocosReturnPage = ThrottlePage;
      initPage(LocosPage);
      break;
    case ProgramButton:
      nextionCommand("AD.bco=" + String(GREY));
      guestActive = false;  
      ProgReturnPage = ThrottlePage;      
      initPage(ProgramPage);
      break;
    case EditButtonOn:                          //This is Actually the "Address" text
    {
      if(selectedIDs[activeSlot] != 255)        
      {
        nextionCommand("AD.bco=" + String(GREY));
        guestActive = false;
        editingID = selectedIDs[activeSlot];
        LocoEditReturnPage = ThrottlePage;
        initPage(LocoEditPage);
      }
      break;
    }
    case ForwardButton:
    {
      auto th = throttles[activeSlot];
      Loco *activeLoco = th->getLoco();
      if(selectedIDs[activeSlot] != 255)        
      {
//        if(guestActive == false)
//        {
          if(readLocoAddress(selectedIDs[activeSlot]) == 0) return;
//          dir = 1;
          activeLoco->setDirection(Direction::Forward);
          Serial.println("Direction set to Forward");
          nextionSetValue("FR", 1);
          Serial.printf("Threshold Value: %d", readEEPROMByte(eeThreshold));
          Serial.println();
          Serial.printf("Speed Value: %d", activeLoco->getSpeed());
          Serial.println();
          if(activeLoco->getSpeed() >= readEEPROMByte(eeThreshold)) 
          {
            activeLoco->setSpeed(0);
            nextionSetValue("S1",0);
          }
          dccexProtocol.setThrottle(activeLoco, activeLoco->getSpeed(), activeLoco->getDirection());
//        }else{
//          guestDir = 1;
//          checkThreshold();
//          setGuest();
//        }
      }
      break;
    }
    case ReverseButton:
    {
      auto th = throttles[activeSlot];
      Loco *activeLoco = th->getLoco();
      if(selectedIDs[activeSlot] != 255)        
      {
//        if(guestActive == false)
//        {
          if(readLocoAddress(selectedIDs[activeSlot]) == 0) return;
//          dir = 0;
          activeLoco->setDirection(Direction::Reverse);
          Serial.println("Direction set to Reverse");
          Serial.println();
          Serial.printf("Speed Value: %d", activeLoco->getSpeed());
          Serial.println();
          nextionSetValue("FR", 0);
          Serial.printf("Threshold Value: %d", readEEPROMByte(eeThreshold));
          if(activeLoco->getSpeed() >= readEEPROMByte(eeThreshold)) 
          {
            activeLoco->setSpeed(0);
            nextionSetValue("S1",0);
          }
          dccexProtocol.setThrottle(activeLoco, activeLoco->getSpeed(), activeLoco->getDirection());
//        }else{
//          guestDir = 0;
//          checkThreshold();
//          setGuest();
//       }
      }
      break;
    }
 //   case SliderEvent:
 //   {
 //      break;
 //   }
    case GuestButton:
    {
      nextionDataType = GUEST_ADDRESS;   // Next data expected from the Nextion will be Guest Address 
      nextionSetText("Na","Guest");
      nextionSetText("Nb","Loco");
      nextionSetText("Nc","");
      nextionCommand("AD.bco=" + String(YELLOW));
      guestActive = true;
      encoderPos = 0;
      break;
    }
    default:                                                  
    {
      auto th = throttles[activeSlot];
      Loco *activeLoco = th->getLoco();
      if((button >= TabSlotStart) && (button < (TabSlotStart + locosPerPage)))      //Process the Pressed Tab
      {
        if(guestActive == true)
        {
          nextionCommand("AD.bco=" + String(GREY));
          guestActive = false;
          initPage(ThrottlePage);
          break;
        }
        if(activeSlot == (button - TabSlotStart)) break;
        deActivateSlot(activeSlot);
        activeSlot = (button - TabSlotStart);        //Determine Activated Tab 0 to 9
        activateSlot(activeSlot);
        break;
      }
      if(selectedIDs[activeSlot] != 255)        
      {
        if(button >= FunctionSlotStart && button < (FunctionSlotStart + functionsPerPage))         //Process the Function Slots
        {
          if(guestActive == true)
          {
            nextionCommand("AD.bco=" + String(GREY));
            guestActive = false;
            initPage(ThrottlePage);
            break;
          }
          g_fSlot = button - FunctionSlotStart;
          uint8_t funcNum = readEEPROMByte(locoFuncBase + (selectedIDs[activeSlot]*fBlockSize) +(g_fSlot*2));    //retrieve the actual function number from its EEPROM slot
          uint8_t actualFunc = funcNum & 0x7F;
          Serial.printf("actualFunc: %d\n\r", actualFunc);
          if(actualFunc == 127) break;       //Inactive/unassigned function
          uint8_t funcImg = readEEPROMByte(locoFuncBase + (selectedIDs[activeSlot]*fBlockSize) +(g_fSlot*2)+1);
          if(activeLoco->isFunctionOn(actualFunc))
          {
            Serial.println("Setting Function Off");
            nextionCommand(("s" + String(g_fSlot) + ".pic=22"));  // + String(funcImg)));
            dccexProtocol.functionOff(activeLoco, actualFunc);
          }else
//          if(activeLoco->isFunctionOff(funcNum))
          {
            Serial.println("Setting Function On");
            nextionCommand(("s" + String(g_fSlot) + ".pic=22"));  // + String(funcImg+1)));
            dccexProtocol.functionOn(activeLoco, actualFunc);
          }
          break;
        }
        if(button >= FunctionReleaseStart && button < (FunctionReleaseStart + functionsPerPage))
        {
          uint8_t funcNum = readEEPROMByte(locoFuncBase + (selectedIDs[activeSlot]*fBlockSize) +(g_fSlot*2));    //retrieve the actual function number from its EEPROM slot
          if((funcNum & 0x80) != 0)     //fType == PULSE)
          {
            uint8_t funcImg = readEEPROMByte(locoFuncBase + (selectedIDs[activeSlot]*fBlockSize) +(g_fSlot*2)+1);
            nextionCommand(("s" + String(g_fSlot) + ".pic=" + String(funcImg)));
            dccexProtocol.functionOff(activeLoco, (funcNum & 0x7f));
          }
        }
        break;
      }
    }
  }
}
/*
 ***************************************************************************************************************
 * Send the New Guest Speed to the Command Station
 ****************************************************************************************************************
*/
void setGuest()
{
  String dccppCMD = "<t 0 " + String(guestAddress) + " " + String(encoderPos) + " " + String(guestDir) + ">"; 
  sendCMD(dccppCMD);
}
/*
 ***************************************************************************************************************
 * Reset Speed to 0 if above Threshold
 ****************************************************************************************************************
*/
void checkThreshold()
{
  if (encoderPos >= readEEPROMByte(eeThreshold)) encoderPos = 0;
  updateNextionThrottle(encoderPos);
}
/*
 ***************************************************************************************************************
 * Populate Throttle Page Loco fields with Current Settings (and Functions) 
 ***************************************************************************************************************
*/
void populateSlots()
{  
  for(uint8_t i = 0; i <numLocoSlots ; i++)
  {
    if(i != activeSlot)
    {
      if(selectedIDs[i] != 255)
      {
        nextionSetText("n" + String(i),"");                                                                                   //Blank out if unused
        nextionSetText("t" + String(i), readEEPROMName(locoTypeBase + (selectedIDs[i]* (locoNameLen))));                      //Loco Type
        nextionSetText("n" + String(i), readEEPROMName(locoNameBase + (selectedIDs[i] * (locoNameLen))));                     //Road Name
        if((readLocoAddress(selectedIDs[i])) == 0)
        {
          nextionSetText("v" + String(i),"");
          nextionSetText("t" + String(i),"");
          nextionSetText("n" + String(i),"");
        }else
        {
          if(readEEPROMByte(eeRNumEnabled) == 0)
          {
            nextionSetText("v" + String(i), String(readLocoAddress(selectedIDs[i])));
          }else
          {
            nextionSetText("v" + String(i), String(readLocoRNum(selectedIDs[i])));
          }
          nextionCommand("v" + String(i) + ".pco=" + String(WHITE));
        }
      }else
      {
        nextionSetText("v" + String(i),"");
        nextionSetText("t" + String(i),"");
        nextionSetText("n" + String(i),"");
      }
    }
  }
}
/*
 ***************************************************************************************************************
 * Set Loco active
 ***************************************************************************************************************
*/
void activateSlot(uint8_t slot)
{
  if(nextionPage == ThrottlePage)
  {
    #if defined DISPLAY_TAB_DETAILS_GREY_BG
      nextionCommand("n" + String(slot) + ".pco=" + String(BLACK));  //Font Colour
      nextionCommand("t" + String(slot) + ".pco=" + String(BLACK));  //Font Colour
      nextionCommand("n" + String(slot) + ".bco=" + String(GREY));   //Road Name Field background Colour
      nextionCommand("t" + String(slot) + ".bco=" + String(GREY));   //Loco Type Field background Colour
      nextionCommand("v" + String(slot) + ".bco=" + String(GREY));   //Address/Road Number Field background Colour
    #elif defined DISPLAY_TAB_DETAILS_YELLOW_BG
      nextionCommand("n" + String(slot) + ".pco=" + String(BLACK));  //Font Colour
      nextionCommand("t" + String(slot) + ".pco=" + String(BLACK));  //Font Colour
      nextionCommand("n" + String(slot) + ".bco=" + String(YELLOW)); //Road Name Field background Colour
      nextionCommand("t" + String(slot) + ".bco=" + String(YELLOW)); //Loco Type Field background Colour
      nextionCommand("v" + String(slot) + ".bco=" + String(YELLOW)); //Address/Road Number Field background Colour
    #else
      nextionCommand("n" + String(slot) + ".pco=" + String(GREY));   //Font Colour
      nextionCommand("t" + String(slot) + ".pco=" + String(GREY));   //Font Colour
      nextionCommand("n" + String(slot) + ".bco=" + String(GREY));   //Road Name Field background Colour
      nextionCommand("t" + String(slot) + ".bco=" + String(GREY));   //Loco Type Field background Colour
      nextionCommand("v" + String(slot) + ".bco=" + String(GREY));   //Address/Road Number Field background Colour
    #endif
    if((readLocoAddress(selectedIDs[slot])) == 0 || selectedIDs[slot] == 255)
    {
      nextionSetText("v" + String(slot),"");
      nextionSetText("t" + String(slot),"");
      nextionSetText("n" + String(slot),"");
      for(uint8_t l_fSlot=0; l_fSlot <numFSlots; l_fSlot++) 
      {
        nextionCommand(("s" + String(l_fSlot) + ".pic=" + String(BLANK)).c_str());   //load blank Function Image
      }
      auto th = throttles[activeSlot];
      Loco *loco = th->getLoco();
      dccexProtocol.setThrottle(loco, loco->getSpeed(), loco->getDirection());
    }else
    { 
//      setHeadingDetails(slot);                                             //Update the Page Heading Info
      if(selectedIDs[slot] != 255)
      {
        nextionSetText("LName", longLocoNames[selectedIDs[slot]]);
        nextionSetText("n" + String(slot), readEEPROMName(locoNameBase + (selectedIDs[slot] * (locoNameLen))));         //Road Name
        nextionSetText("t" + String(slot), readEEPROMName(locoTypeBase + (selectedIDs[slot]* (locoNameLen))));       //Loco Type  
        if(readEEPROMByte(eeRNumEnabled) == 0)
        {
          nextionSetText("v" + String(slot), String(readLocoAddress(selectedIDs[slot])));
        }else{
          nextionSetText("v" + String(slot), String(readLocoRNum(selectedIDs[slot])));
        }
        nextionCommand("v" + String(slot) + ".pco=" + String(BLACK));   //Font Colour
        auto th = throttles[activeSlot];
        Loco *loco = th->getLoco();
        dccexProtocol.setThrottle(loco, loco->getSpeed(), loco->getDirection());
        updateNextionThrottle(loco->getSpeed());
        nextionSetValue(F("FR"), (loco->getDirection()));
        nextionSetText("AD", String(loco->getAddress()));
        loadFunctions(ThrottlePage, selectedIDs[slot]);
      }    
    }
  }
}
/*
 ***************************************************************************************************************
 * Deactivate Loco Slot after having been Active
 ***************************************************************************************************************
*/
void deActivateSlot(uint8_t slot)
{
    nextionCommand("n" + String(slot) + ".bco=" + String(DARK_GREY));
    nextionCommand("t" + String(slot) + ".bco=" + String(DARK_GREY)); 
    nextionCommand("v" + String(slot) + ".bco=" + String(DARK_GREY)); 
    nextionCommand("n" + String(slot) + ".pco=" + String(BLACK)); 
    nextionCommand("t" + String(slot) + ".pco=" + String(BLACK));
    nextionCommand("v" + String(slot) + ".pco=" + String(WHITE));
}

/*
 ***************************************************************************************************************
 * Define the 10 Function details and images on the displayed Page - either Throttle or Edit for Loco id
 * Current States of functions are NOT restored in EEPROM but in functions[][] array
 * loadFunctions is called by PageInits and only applies to activeLoco
 * Sequence of events:
 * - Retrieve activeLoco function states from DCCEX (if not already retrieved)
 * - using the EEPROM stored function slot function numbers and Images
 * - create the 28 function states
 ***************************************************************************************************************
*/
void loadFunctions(uint8_t Page, uint8_t locoID)
{
  auto th = throttles[activeSlot];
  Loco *activeLoco = th->getLoco();
  uint32_t functionStates = activeLoco->getFunctionStates();
  for(uint8_t l_fSlot=0; l_fSlot<numFSlots; l_fSlot++) 
  {
    uint8_t fNum = readEEPROMByte((locoFuncBase + (locoID * 20)) + (l_fSlot*2));         // 20 bytes needed for 10 slots on a page
    fNum = fNum & 0x7F;                                                                  //Remove the pulse indicator bit
    uint8_t iNum = readEEPROMByte((locoFuncBase + (locoID * 20)) + (l_fSlot*2)+1);       // retrieve the image number
    if(fNum <=68)
    {
      if (Page == LocoEditPage)
      {
        nextionSetValue(("n" + String(l_fSlot)), fNum);               //Function Number into Function Slot
        nextionCommand(("n" + String(l_fSlot)+".pco=0").c_str());           //Set colour to Black
        nextionCommand(("c" + String(l_fSlot)+".pco=0").c_str());           //Set colour to Black
        nextionSetText("c" + String(l_fSlot), "F");                    //Ensure "F" is displayed
      } //Both Pages
      if(bitRead(functionStates, fNum) == 0) 
      {
        nextionCommand("s" + String(l_fSlot) + ".pic=" + String(iNum));     //Image = Off
      }
      else 
      {
        nextionCommand("s" + String(l_fSlot) + ".pic=" + String(iNum+1));                                    //Image = On
      }
    }else
    {
      if (Page == LocoEditPage)
      {
        nextionCommand(("c" + String(l_fSlot)+".pco=" + WHITE).c_str());      //Colour to White
        nextionCommand(("n" + String(l_fSlot)+".pco="+ GREY).c_str());      //Colour to Grey
        nextionSetText("c" + String(l_fSlot), "F"); 
        nextionCommand(("s" + String(l_fSlot) + ".pic=" + String(GREYED_BUTTON)).c_str());   //load greyed Function Image
      }else
      nextionCommand(("s" + String(l_fSlot) + ".pic=" + String(BLANK)).c_str());   //load blank Function Image
    }
  }  
}
/*
 ***************************************************************************************************************
 * Function to Toggle a Function's state
 * For the active LocoID
 * Returns the toggled image number and toggles the function state
 ***************************************************************************************************************
 */
uint8_t toggleFunction(uint8_t funcNum, uint8_t funcImg)
{
  #if defined DCCPP
    if (funcNum == 0)
    {
      if (bitRead(LocoFN0to4[selectedIDs[activeSlot]], 4) == 0)     //if function off
      {
      funcImg = funcImg + 1;  
      bitWrite(LocoFN0to4[selectedIDs[activeSlot]], 4, 1);        //set function on
      functions[selectedIDs[activeSlot]][g_fSlot]=1;
    }
    else
    {
      bitWrite(LocoFN0to4[selectedIDs[activeSlot]], 4, 0);
      functions[selectedIDs[activeSlot]][g_fSlot]=0;
    }
    doDCCfunction04();
  }
  if (funcNum >= 1 && funcNum <= 4)
  {
    if (bitRead(LocoFN0to4[selectedIDs[activeSlot]], funcNum-1) == 0)
    {
      funcImg = funcImg + 1;
      bitWrite(LocoFN0to4[selectedIDs[activeSlot]], funcNum-1, 1); 
      functions[selectedIDs[activeSlot]][g_fSlot]=1;
    }
    else
    {
      bitWrite(LocoFN0to4[selectedIDs[activeSlot]], funcNum-1, 0);
      functions[selectedIDs[activeSlot]][g_fSlot]=0;
    }
    doDCCfunction04();
  }
  if (funcNum >= 5 && funcNum <= 8)
  {
    if (bitRead(LocoFN5to8[selectedIDs[activeSlot]], funcNum-5) == 0 ) 
    {
      funcImg = funcImg + 1;
      bitWrite(LocoFN5to8[selectedIDs[activeSlot]], funcNum-5, 1);
      functions[selectedIDs[activeSlot]][g_fSlot]=1;
    }
    else 
    {
      bitWrite(LocoFN5to8[selectedIDs[activeSlot]], funcNum-5, 0);
      functions[selectedIDs[activeSlot]][g_fSlot]=0;
    }
    doDCCfunction58();
  }
  if (funcNum >= 9 && funcNum <= 12)
  {
    if (bitRead(LocoFN9to12[selectedIDs[activeSlot]], funcNum-9) == 0 ) 
    {
      funcImg = funcImg + 1;
      bitWrite(LocoFN9to12[selectedIDs[activeSlot]], funcNum-9, 1);
      functions[selectedIDs[activeSlot]][g_fSlot]=1;
    }
    else 
    {
      bitWrite(LocoFN9to12[selectedIDs[activeSlot]], funcNum-9, 0);
      functions[selectedIDs[activeSlot]][g_fSlot]=0;
    }
    doDCCfunction912();
  }
  if (funcNum >= 13 && funcNum <= 20)
  {
    if (bitRead(LocoFN13to20[selectedIDs[activeSlot]], funcNum-13) == 0 )
    {
      funcImg = funcImg + 1;
      bitWrite(LocoFN13to20[selectedIDs[activeSlot]], funcNum-13, 1);
      functions[selectedIDs[activeSlot]][g_fSlot]=1;
    }
    else 
    {
      bitWrite(LocoFN13to20[selectedIDs[activeSlot]], funcNum-13, 0);
      functions[selectedIDs[activeSlot]][g_fSlot]=0;
    }
    doDCCfunction1320();                                                        //Set up Command to Command Station
  }
  if (funcNum >= 21 && funcNum <= 28)
  {
    if (bitRead(LocoFN21to28[selectedIDs[activeSlot]], funcNum-21) == 0 )         //Function 21 is bit 0 of LocoFN21to28
    {
      funcImg = funcImg + 1;
      bitWrite(LocoFN21to28[selectedIDs[activeSlot]], funcNum-21, 1);
      functions[selectedIDs[activeSlot]][g_fSlot]=1;
    }
    else 
    {
      bitWrite(LocoFN21to28[selectedIDs[activeSlot]], funcNum-21, 0);
      functions[selectedIDs[activeSlot]][g_fSlot]=0;
    }
    doDCCfunction2128();                                                      //Set up Command to Command Station
    }
    return(funcImg);
  #endif

  #if !defined DCCPP
    if(functions[selectedIDs[activeSlot]][g_fSlot] == 0)
    {
      funcImg = funcImg + 1;
      functions[selectedIDs[activeSlot]][g_fSlot] = 1;
    }
    else
    {
      functions[selectedIDs[activeSlot]][g_fSlot] = 0;
    }
    doDCCfunctions(funcNum);
    return(funcImg);
  #endif
}
/*
 **********************************************************************************************************
 * Change Loco Direction
 **********************************************************************************************************
*/
void changeDir(uint8_t dir)
{
  locos[selectedIDs[activeSlot]].dir = dir;
  if (encoderPos >= readEEPROMByte(eeThreshold))
  {
    locos[selectedIDs[activeSlot]].speed = 0;
    encoderPos = 0;
    oldEncPos = 0;
  }
  //doDCC(activeSlot);
}
/*
 **********************************************************************************************************
 * Set Loco Details at the top of each Page
 * This routine does NOT update the Tab details on the Throttle Page except for the Address...
 **********************************************************************************************************
*/
/*
void setHeadingDetails(uint8_t activeSlot)
{
  if(selectedIDs[activeSlot] != 255)
  {
    nextionSetText("Na", readEEPROMName(locoNameBase + (selectedIDs[activeSlot] * (locoNameLen))));
    nextionSetText("Nb", readEEPROMName(locoTypeBase + (selectedIDs[activeSlot] * (locoTypeLen))));
    nextionSetText("Nc", String(readLocoRNum(selectedIDs[activeSlot])));
    nextionSetValue("S", locos[selectedIDs[activeSlot]].speed);
  }else
  {
    nextionSetText("Na", "");
    nextionSetText("Nb", "");
    nextionSetText("Nc", "");
    nextionSetValue("S", 0);
  }}
*/