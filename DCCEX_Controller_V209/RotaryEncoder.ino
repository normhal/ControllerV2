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
 **********************************************************
 * Check the Rotary Encoder Direction Change Button
 **********************************************************
*/
void checkREButton() 
{
  uint8_t dirChanged = 0;
  if (digitalRead(REButtonPin) == LOW) 
  {
    wait(200);              //a little de-bounce time...
    do 
    {
      if(dirChanged == 0)
      {
        changeDirection();
        dirChanged = 1; 
      }
    }
    while (digitalRead(REButtonPin) == LOW);           //Loop until button released
  }
  updateSpeed();
}
/*
 ***********************************************************************************************************
 * Interrupt Service Routine for PinA
 * This routine decrements rotaryEncoderPos
 * NOTE: rotaryEncoderPos is used by all locos, but always reflects the value for the currently active Loco
 ***********************************************************************************************************
*/
#if defined (ESP8266)
  ICACHE_RAM_ATTR void PinA() 
#elif defined (ESP32)
  IRAM_ATTR void PinA()
#else
  void PinA()
#endif
  {
  #if defined ENABLE_ROTARY_ENCODER
    #if !defined (ESP32)
      cli(); //stop interrupts happening before we read pin values
    #endif
    reading = 0;

    if (digitalRead(pinA)) reading = reading | 0b00000100;
    if (digitalRead(pinB)) reading = reading | 0b00001000;
    if (reading == 0b00001100 && aFlag) //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    {
      if (rotaryEncoderPos >= 1) 
      {
        elapsedTime = (millis() - lastTime);    // Debounce Time
        rotaryEncoderPos --;
        if (elapsedTime <= 100)
        {
          if (rotaryEncoderPos >= REAccAmount)
          {
            rotaryEncoderPos = (rotaryEncoderPos - (REAccAmount - 1));    //decrement the encoder's position count
          }
        }
      }
      lastTime = millis();
      aFlag = 0; //reset flags for the next turn
      bFlag = 0; //reset flags for the next turn
    }
    else if (reading == 0b00000100) bFlag = 1; //signal that we're expecting pinB to signal the transition to detent from free rotation
    #if !defined (ESP32)
      sei(); //enable interrupts again
    #endif
  #endif
  }
/*
 **********************************************************
 * Interrupt Service Routine to PinB
 * This routine increments encoderPos
 **********************************************************
*/
#if defined (ESP8266)
  ICACHE_RAM_ATTR void PinB() 
#elif defined (ESP32)
  IRAM_ATTR void PinB()
#else
  void PinB() 
#endif
  {
  #if defined ENABLE_ROTARY_ENCODER
    #if !defined (ESP32)
      cli(); //stop interrupts happening before we read pin values
    #endif
  reading = 0;
  if (digitalRead(pinA)) reading = reading | 0b00000100;
  if (digitalRead(pinB)) reading = reading | 0b00001000;
  if (reading == 0b00001100 && bFlag) //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
  {
    if(rotaryEncoderPos < highest) 
    {
      elapsedTime = (millis() - lastTime);
      rotaryEncoderPos ++;      //increment the encoder's position count
      if (elapsedTime <= 100)
      {
        if (rotaryEncoderPos <= (highest - REAccAmount))
        {
          rotaryEncoderPos = (rotaryEncoderPos + (REAccAmount - 1));
        }
      }
    } 
    lastTime = millis();
    aFlag = 0; //reset flags for the next turn
    bFlag = 0; //reset flags for the next turn
  }
  else if (reading == 0b00001000) aFlag = 1; //signal that we're expecting pinA to signal the transition to detent from free rotation
  #if !defined (ESP32)
    sei(); //enable interrupts again
  #endif
#endif
}
/***************************************************************************************************************************/
