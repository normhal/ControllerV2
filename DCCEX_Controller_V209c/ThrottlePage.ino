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
//  Console.printf("Throttle Page: %d\n\r", thNum);
  if(message.startsWith("TH"))
  {
    rotaryEncoderPos = (message.substring(2)).toInt();
    updateSpeed();
    return;
  }
  if(message.startsWith("GL"))          //Guest Loco
  {
    Console.println("Guest Loco Pressed");
    uint16_t guestAddr = (message.substring(2)).toInt();
    if(readEEPROMByte(eeUseExRoster) == 0)
    {
      guestActive = true;
      Console.print("Guest Loco Requested: ");
      Console.println(guestAddr);
      auto th = throttles[numLocoSlots];        //open Throttle 11
      Loco *activeLoco = th->getLoco();
      throttles[numLocoSlots] = new Throttle(&dccexProtocol);
      throttles[numLocoSlots]->setLoco(new Loco(guestAddr, LocoSource::LocoSourceRoster));
      nextionSetText("LName", "Guest Loco");
      nextionCommand("LName.bco=" + String(YELLOW));
    }else
    {
      guestActive = false;
      Console.println("DCCEX Roster Loco requested");
  
//      auto *activeLoco = throttles[activeSlot[thNum]];
      throttles[activeSlot[thNum]]->setLoco(new Loco(guestAddr, LocoSource::LocoSourceEntry));
//      dccexProtocol.requestLocoUpdate(activeLoco->getAddress());

      auto th = throttles[activeSlot[thNum]];
      Loco *activeLoco = th->getLoco();
      dccexProtocol.requestLocoUpdate(activeLoco->getAddress());
//      selectedIDs[thNum][activeSlot[thNum]] = 
      activateSlot(activeSlot[thNum]);
      populateSlots();
      Console.printf("Loco: %d Recorded\n\r", activeLoco->getAddress());
    }
    return;
  }
  switch(button)                  
  {
    case AccButton:
      if(guestActive == false)
      {
        AccReturnPage = ThrottlePage;
        initPage(AccPage);
      }
      break;
    case LocosButton:
      if(guestActive == false)
      {
        LocosReturnPage = ThrottlePage;
        initPage(RosterPage);
      }
      break;
    case ProgramButton:
      if(guestActive == false)
      {
        ProgReturnPage = ThrottlePage;      
        initPage(ProgramPage);
      }
      break;
    case EditButtonOn:                          //This is Actually the "Address" text
    {
      if(guestActive == false)                //Edit not available for Guest Loco
      {
        if(selectedIDs[thNum][activeSlot[thNum]] != 255)        
        {
          editingID = selectedIDs[thNum][activeSlot[thNum]];
          LocoEditReturnPage = ThrottlePage;
          initPage(LocoEditPage);
        }
      }
      break;
    }
    case DirectionButton:
    {
      changeDirection();
      break;
    }
    default:                                                  
    {
//************************************************************************
//Process the Pressed Tab
//************************************************************************
      auto th = throttles[activeSlot[thNum]];
      Loco *activeLoco = th->getLoco();
      if((button >= TabSlotStart) && (button < (TabSlotStart + locosPerPage)))      
      {
        if(guestActive == true)
        {
          nextionCommand("LName.bco=" + String(GREY));
          guestActive = false;
          initPage(ThrottlePage);
          break;
        }
        if(activeSlot[thNum] == (button - TabSlotStart)) break;
        deActivateSlot(activeSlot[thNum]);
        activeSlot[thNum] = (button - TabSlotStart);        //Determine Activated Tab 0 to 9
        activateSlot(activeSlot[thNum]);
        break;
      }
//************************************************************************
// Process the Function Slots
//************************************************************************
      if(selectedIDs[thNum][activeSlot[thNum]] != 255)        
      {
        if(button >= FunctionSlotStart && button < (FunctionSlotStart + functionsPerPage))
        {
          if(guestActive == true)
          {
            nextionCommand("LName.bco=" + String(GREY));
            guestActive = false;
            initPage(ThrottlePage);
            break;
          }
          g_fSlot = button - FunctionSlotStart;
          uint8_t funcNum, actualFunc, funcImg;
          if(readEEPROMByte(eeUseExRoster) == 0)
          {
            funcNum = readEEPROMByte(locoFuncBase + (selectedIDs[thNum][activeSlot[thNum]]*fBlockSize) +(g_fSlot*2));    //retrieve the actual function number from its EEPROM slot
            actualFunc = funcNum & 0x7F;
            if(actualFunc == 127) break;       //Inactive/unassigned function
            funcImg = readEEPROMByte(locoFuncBase + (selectedIDs[thNum][activeSlot[thNum]]*fBlockSize) +(g_fSlot*2)+1);
          }else
          {
            actualFunc = g_fSlot;
            funcImg = (g_fSlot * 2) + 60;
          }
          if(activeLoco->isFunctionOn(actualFunc))
          {
            dccexProtocol.functionOff(activeLoco, actualFunc);
            nextionCommand("P2.pic=260");                                       //Waiting for CS
          }else
          {
            dccexProtocol.functionOn(activeLoco, actualFunc);
            nextionCommand("P2.pic=260");                                       //Waiting for CS
          }
          break;
        }
        if(button >= FunctionReleaseStart && button < (FunctionReleaseStart + functionsPerPage))
        {
          uint8_t funcNum = readEEPROMByte(locoFuncBase + (selectedIDs[thNum][activeSlot[thNum]]*fBlockSize) +(g_fSlot*2));    //retrieve the actual function number from its EEPROM slot
          if((funcNum & 0x80) != 0)     //fType == PULSE)
          {
            uint8_t funcImg = readEEPROMByte(locoFuncBase + (selectedIDs[thNum][activeSlot[thNum]]*fBlockSize) +(g_fSlot*2)+1);
            dccexProtocol.functionOff(activeLoco, (funcNum & 0x7f));
            nextionCommand("P2.pic=260");                                   //Waiting for CS
          }
        }
        break;
      }
    }
  }
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
    if(i != activeSlot[thNum])
    {
      if(selectedIDs[thNum][i] != 255)
      {
        nextionSetText("n" + String(i),"");                                                                                   //Blank out if unused
        nextionSetText("t" + String(i), readEEPROMName(locoTypeBase + (selectedIDs[thNum][i]* (locoNameLen))));                      //Loco Type
        nextionSetText("n" + String(i), readEEPROMName(locoNameBase + (selectedIDs[thNum][i] * (locoNameLen))));                     //Road Name
        if((readLocoAddress(selectedIDs[thNum][i])) == 0)
        {
          nextionSetText("v" + String(i),"");
          nextionSetText("t" + String(i),"");
          nextionSetText("n" + String(i),"");
        }else
        {
          if(readEEPROMByte(eeRNumEnabled) == 0)
          {
            nextionSetText("v" + String(i), String(readLocoAddress(selectedIDs[thNum][i])));
          }else
          {
            nextionSetText("v" + String(i), String(readLocoRNum(selectedIDs[thNum][i])));
          }
          nextionCommand("n" + String(i) + ".bco=" + String(THGREY));
          nextionCommand("t" + String(i) + ".bco=" + String(THGREY)); 
          nextionCommand("v" + String(i) + ".bco=" + String(THGREY)); 
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
    if((readLocoAddress(selectedIDs[thNum][slot])) == 0 || selectedIDs[thNum][slot] == 255)
    {
      nextionSetText("v" + String(slot),"");
      nextionSetText("t" + String(slot),"");
      nextionSetText("n" + String(slot),"");
      nextionSetText("AD", "");      //, String(0));
      nextionSetText("LName", "");
      for(uint8_t l_fSlot=0; l_fSlot <numFSlots; l_fSlot++) 
      {
        nextionCommand(("s" + String(l_fSlot) + ".pic=" + String(BLANK)).c_str());   //load blank Function Image
      }
    }else //loco slot number is valid
    { 
      auto th = throttles[slot];
      Loco *loco = th->getLoco();
      int address = loco->getAddress();
      dccexProtocol.requestLocoUpdate(address);
      if(selectedIDs[thNum][slot] != 255)
      {
        if(readEEPROMByte(eeUseExRoster) == 0)
        {
          nextionSetText("LName", longLocoNames[selectedIDs[thNum][slot]]);
          nextionSetText("n" + String(slot), readEEPROMName(locoNameBase + (selectedIDs[thNum][slot] * (locoNameLen))));         //Road Name
          nextionSetText("t" + String(slot), readEEPROMName(locoTypeBase + (selectedIDs[thNum][slot]* (locoNameLen))));       //Loco Type  
          if(readEEPROMByte(eeRNumEnabled) == 0)
          {
            nextionSetText("v" + String(slot), String(readLocoAddress(selectedIDs[thNum][slot])));
          }else
          {
            nextionSetText("v" + String(slot), String(readLocoRNum(selectedIDs[thNum][slot])));
          }
          nextionCommand("v" + String(slot) + ".pco=" + String(BLACK));   //Font Colour
        }else
        {
          nextionSetText("n" + String(slot), "");
          nextionSetText("t" + String(slot), String(readLocoAddress(selectedIDs[thNum][slot])));
          nextionSetText("v" + String(slot), "");
        }
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
    nextionCommand("n" + String(slot) + ".bco=" + String(THGREY));
    nextionCommand("t" + String(slot) + ".bco=" + String(THGREY)); 
    nextionCommand("v" + String(slot) + ".bco=" + String(THGREY)); 
    nextionCommand("n" + String(slot) + ".pco=" + String(BLACK)); 
    nextionCommand("t" + String(slot) + ".pco=" + String(BLACK));
    nextionCommand("v" + String(slot) + ".pco=" + String(USER1));
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
  uint8_t fNum, iNum = 0;
  auto th = throttles[activeSlot[thNum]];
  Loco *activeLoco = th->getLoco();
  uint32_t functionStates = activeLoco->getFunctionStates();
  for(uint8_t l_fSlot=0; l_fSlot<numFSlots; l_fSlot++) 
  {
    if(readEEPROMByte(eeUseExRoster) == 0)
    {
      fNum = readEEPROMByte((locoFuncBase + (locoID * 20)) + (l_fSlot*2));         // 20 bytes needed for 10 slots on a page
      fNum = fNum & 0x7F;                                                                  //Remove the pulse indicator bit
      iNum = readEEPROMByte((locoFuncBase + (locoID * 20)) + (l_fSlot*2)+1);       // retrieve the image number
    }else
    {
      fNum = l_fSlot;
      iNum = (l_fSlot * 2) + 60;              //Image Numbers start at 60 and increment by 2
    }
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
        nextionCommand(("c" + String(l_fSlot)+".pco=" + USER2).c_str());      //Colour to White
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
      if (bitRead(LocoFN0to4[selectedIDs[thNum][activeSlot[thNum]]], 4) == 0)     //if function off
      {
      funcImg = funcImg + 1;  
      bitWrite(LocoFN0to4[selectedIDs[thNum][activeSlot[thNum]]], 4, 1);        //set function on
      functions[selectedIDs[thNum][activeSlot[thNum]]][g_fSlot]=1;
    }
    else
    {
      bitWrite(LocoFN0to4[selectedIDs[thNum][activeSlot[thNum]]], 4, 0);
      functions[selectedIDs[thNum][activeSlot[thNum]]][g_fSlot]=0;
    }
    doDCCfunction04();
  }
  if (funcNum >= 1 && funcNum <= 4)
  {
    if (bitRead(LocoFN0to4[selectedIDs[thNum][activeSlot[thNum]]], funcNum-1) == 0)
    {
      funcImg = funcImg + 1;
      bitWrite(LocoFN0to4[selectedIDs[thNum][activeSlot[thNum]]], funcNum-1, 1); 
      functions[selectedIDs[thNum][activeSlot[thNum]]][g_fSlot]=1;
    }
    else
    {
      bitWrite(LocoFN0to4[selectedIDs[thNum][activeSlot[thNum]]], funcNum-1, 0);
      functions[selectedIDs[thNum][activeSlot[thNum]]][g_fSlot]=0;
    }
    doDCCfunction04();
  }
  if (funcNum >= 5 && funcNum <= 8)
  {
    if (bitRead(LocoFN5to8[selectedIDs[thNum][activeSlot[thNum]]], funcNum-5) == 0 ) 
    {
      funcImg = funcImg + 1;
      bitWrite(LocoFN5to8[selectedIDs[thNum][activeSlot[thNum]]], funcNum-5, 1);
      functions[selectedIDs[thNum][activeSlot[thNum]]][g_fSlot]=1;
    }
    else 
    {
      bitWrite(LocoFN5to8[selectedIDs[thNum][activeSlot[thNum]]], funcNum-5, 0);
      functions[selectedIDs[thNum][activeSlot[thNum]]][g_fSlot]=0;
    }
    doDCCfunction58();
  }
  if (funcNum >= 9 && funcNum <= 12)
  {
    if (bitRead(LocoFN9to12[selectedIDs[thNum][activeSlot[thNum]]], funcNum-9) == 0 ) 
    {
      funcImg = funcImg + 1;
      bitWrite(LocoFN9to12[selectedIDs[thNum][activeSlot[thNum]]], funcNum-9, 1);
      functions[selectedIDs[thNum][activeSlot[thNum]]][g_fSlot]=1;
    }
    else 
    {
      bitWrite(LocoFN9to12[selectedIDs[thNum][activeSlot[thNum]]], funcNum-9, 0);
      functions[selectedIDs[thNum][activeSlot[thNum]]][g_fSlot]=0;
    }
    doDCCfunction912();
  }
  if (funcNum >= 13 && funcNum <= 20)
  {
    if (bitRead(LocoFN13to20[selectedIDs[thNum][activeSlot[thNum]]], funcNum-13) == 0 )
    {
      funcImg = funcImg + 1;
      bitWrite(LocoFN13to20[selectedIDs[thNum][activeSlot[thNum]]], funcNum-13, 1);
      functions[selectedIDs[thNum][activeSlot[thNum]]][g_fSlot]=1;
    }
    else 
    {
      bitWrite(LocoFN13to20[selectedIDs[thNum][activeSlot[thNum]]], funcNum-13, 0);
      functions[selectedIDs[thNum][activeSlot[thNum]]][g_fSlot]=0;
    }
    doDCCfunction1320();                                                        //Set up Command to Command Station
  }
  if (funcNum >= 21 && funcNum <= 28)
  {
    if (bitRead(LocoFN21to28[selectedIDs[thNum][activeSlot[thNum]]], funcNum-21) == 0 )         //Function 21 is bit 0 of LocoFN21to28
    {
      funcImg = funcImg + 1;
      bitWrite(LocoFN21to28[selectedIDs[thNum][activeSlot[thNum]]], funcNum-21, 1);
      functions[selectedIDs[thNum][activeSlot[thNum]]][g_fSlot]=1;
    }
    else 
    {
      bitWrite(LocoFN21to28[selectedIDs[thNum][activeSlot[thNum]]], funcNum-21, 0);
      functions[selectedIDs[thNum][activeSlot[thNum]]][g_fSlot]=0;
    }
    doDCCfunction2128();                                                      //Set up Command to Command Station
    }
    return(funcImg);
  #endif

  #if !defined DCCPP
    if(functions[selectedIDs[thNum][activeSlot[thNum]]][g_fSlot] == 0)
    {
      funcImg = funcImg + 1;
      functions[selectedIDs[thNum][activeSlot[thNum]]][g_fSlot] = 1;
    }
    else
    {
      functions[selectedIDs[thNum][activeSlot[thNum]]][g_fSlot] = 0;
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
void changeDirection()
{
  uint8_t currentSlot;
  if(guestActive == false) currentSlot = activeSlot[thNum];
  else currentSlot = numLocoSlots;
  if(selectedIDs[thNum][currentSlot] != 255)
  {        
    auto th = throttles[currentSlot];
    Loco *activeLoco = th->getLoco();
    if(activeLoco->getDirection() == Forward) activeLoco->setDirection(Direction::Reverse);
    else activeLoco->setDirection(Direction::Forward);
    if(activeLoco->getSpeed() >= readEEPROMByte(eeThreshold)) 
    {
      activeLoco->setSpeed(0); 
      rotaryEncoderPos = 0;
    }
    dccexProtocol.setThrottle(activeLoco, activeLoco->getSpeed(), activeLoco->getDirection());
    nextionCommand("P2.pic=260");                                                     //Waiting for CS
  }
}
/*
 **********************************************************************************************************
 * Create Throttles for the Throttle Page
 **********************************************************************************************************
*/
void createThrottles(uint8_t thNum)
{
  for(int i =0; i<(numLocoSlots); i++)
  {
    throttles[i] = new Throttle(&dccexProtocol);
    if(readEEPROMByte(eeUseExRoster) == 1)
    {
      throttles[i]->setLoco(new Loco(readLocoAddress(selectedIDs[thNum][i]), LocoSource::LocoSourceRoster));
    }else
    {
      throttles[i]->setLoco(new Loco(readLocoAddress(selectedIDs[thNum][i]), LocoSource::LocoSourceEntry));
    }
    resumeSpeeds[i]=0;
  }
}
/*
 ***************************************************************************
 * Process the current speed setting including input from the Rotary Encoder
 ***************************************************************************
*/
void updateSpeed() 
{
  if (oldEncPos != rotaryEncoderPos)      // If activity, transmit same to Base Station and Update Nextion
  {    
    uint8_t slotToUse = activeSlot[thNum];
    if(guestActive == true) slotToUse = numLocoSlots;      //Guest uses the last Slot in the array of Locos
    oldEncPos = rotaryEncoderPos;
    auto th = throttles[slotToUse];
    Loco *activeLoco = th->getLoco();
    activeLoco->setSpeed(rotaryEncoderPos);
    dccexProtocol.setThrottle(activeLoco, rotaryEncoderPos, activeLoco->getDirection());
    nextionCommand("P2.pic=260");                                       //Waiting for CS
    resumeSpeeds[slotToUse] = rotaryEncoderPos;
  }
}
