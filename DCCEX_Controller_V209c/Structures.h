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
#ifndef STRUCTURES_H
  #define STRUCTURES_H

  struct locomotive 
  {
    uint8_t dir = 1;
    uint8_t speed;
  };
  typedef struct locomotive loco;

  #define NUM_FSLOTS 10
  #define PULSED 128

  // Structure used for the "Hard_Coded_Values" feature when loading pre-configured Locos
  
  struct HCLoco 
  {
//    char* RoadName;          //Max 8 Characters
//    char* LocoType;          //Max 8 Characters
    String RoadName;          //Max 8 Characters
    String LocoType;          //Max 8 Characters
    uint16_t RoadNumber;    
    uint16_t LocoAddress;
    String FavSlot;                         //Slot (0-9) for selectedIDs[thNum][] if present
    uint8_t FSlot[NUM_FSLOTS][4];         //10 Functions, then Function Slot number, Function Number, Function Image for each function
  };

  // Structure used for the "Hard_Coded_Values" feature when loading pre-configured Accessories

  struct HCAcc
  {
    String AccName;
//    char *AccName;
    uint16_t AccAddress;
    uint8_t AccImage;
    uint8_t AccType;
  };

  // Structure used for the "Hard_Coded_Values" feature when loading pre-configured Routes

  struct HCRoute
  {
    uint8_t AccIDs[6][2];
  };  
#endif
