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
#ifndef HCVALUES_H
  #define HCVALUES_H
  #include "Structures.h"
  #include "ImageDefs.h"

  /*
   * Hard Coded Values for LOCOMOTIVES
   *  
   * Modify the value for NUM_HCLOCOS (Number of Hard Coded Locos) as required
   * A value between 1 and 50 is possible
   * The first 10 Loco Definitions populate the Throttle Page by default
   * A Loco Definition comprises:
   * - A Loco Road Name - Maximum 8 characters
   * - A Loco Type - Maximum 8 characters
   * - A Loco Road Number - up to 4 digits
   * - A Loco Address - up to 4 digits
   * - Up to 10 Function Definitions
   *   - Each Definition comprises 3 values
   *     1 - The slot position on the Throttle page the Function "Button" must occupy
   *     2 - The Function's Number
   *         Possible values for Function numbers:-
   *          f0 = Function 0
   *          f1 = Function 1
   *            etc f2..f28 = Functions F2 to F28
   *     3 - The Function's Image
   *         Possible Names for Images:-
   *           F0 = Full Button with F0 text
   *           F1 = Full Button with F1 text
   *             etc F2 to F28 for Functions 2 to 28 
   *           HEADLIGHT = Headlight without a button image
   *           TAILLIGHT = Taillight without a button image
   *           CABLIGHT = Cab light without a button image
   *           ENGINE = Sound without a button Image
   *           SMOKE = Smoker without a button image
   *           BELL = Bell without a button Image
   *           HORN = Horn without a button image
   *          FL_B = FL on a Button Image
   *          HEADLIGHT_B = Headlight image on a Button
   *          TAILLIGHT_B = Taillight on a button image
   *          CABLIGHT_B = Cab light on a button image
   *          ENGINE_B = Sound on a button image
   *          SMOKE_B = Smoker on a button image
   *          BELL_B = Bell on a button image
   *          HORN_B = Horn on a button image
   *          
   *  An Example Loco Definition for a Santa Fe SD40, Road Number 5100 and 
   *  Address 10, (to Occupy slot 5 on the Throttle Page - NO LONGER USED), 
   *  with F0 (Lights) and F4 for Horn, with F0 located in 
   *  the top left Function position (or Slot) and F4 for the Horn 
   *  located in the bottom right position or Slot could look like:-
   *  
   *  {"Santa Fe", "SD40", 5100, 10, ""{{0, f0, HEADLIGHT}, {7, f4, HORN}}}, 
   *  
   *  Note that the number of Loco Definition Rows you provide MUST NOT 
   *  exceed the value you give for NUM_HCLOCOS!
   *  
   *  Use the samples below as a template for your own definitions
	 *  In this example, if the "#define HCL_ALL10" in "Config.h" is commented out
	 *  the "Load" button on the Config Page will only populate slots 0, 1, 3, 4, 5, 7, 8, and 9
  */

// The following two Dimensional Array represents the Four (tCount) individual Throttles each
// having 10(numLocoSlots) slots which can each contain a loco ID. The ID numbers are the order in which the 
// individual loco records appear the the "HCLoco" array where they are defined below. an ID number
// of "255" will represent an empty slot 

uint8_t selectedIDs[tCount][numLocoSlots] = {{0,1,7,10,9,23,4,2,33,24},
                                            {9,11,12,13,14,15,16,17,18,19},
                                            {20,21,22,23,24,255,255,255,255,255},
                                            {30,31,32,33,34,35,36,37,38,39}};

#define NUM_HCLOCOS 40

HCLoco hcLocos[NUM_HCLOCOS] = 
{
  {"GN","S2",1,1,"6",{{0, f0, F0},{1, f1,BELL},{2,f2,HORN,PULSED},{3, f8, ENGINE}}},                      //0
  {"PRR","SD 9",58,7,"13",{{0, f0, F0},{1, f1,BELL},{2,f2,HORN,PULSED},{3, f8, ENGINE}}},                 //1
  {"CN Strip","H16",818,16,"2",{{0, f0, F0},{1, f1,BELL},{2,f2,HORN,PULSED},{3, f8, ENGINE}}},            //2
  {"NH","44Ton NH",8662,18,"9",{{0, f0, F0},{1, f1,BELL},{2,f2,HORN,PULSED},{3, f8, ENGINE}}},            //3
  {"Conrail","S2",5123,23,"4",{{0, f0, F0},{1, f1,BELL},{2,f2,HORN,PULSED},{3, f8, ENGINE}}},             //4
  {"Conrail","SD60-M",58,23,"17",{{0, f0, F0},{1, f1,BELL},{2,f2,HORN,PULSED},{3, f8, ENGINE}}},          //5
  {"Seaboard","GMD1A",1430,30,"3",{{0, f0, F0},{1, f1,BELL},{2,f2,HORN,PULSED},{3, f8, ENGINE}}},         //6
  {"B&O","DoodleBg",3100,31,"32",{{0, f0, F0},{1, f1,BELL},{2,f2,HORN},{3, f8, ENGINE}}},                 //7
  {"CN BCOL","Dash 8 40CW",58,46,"12",{{0, f0, F0},{1, f1,BELL},{2,f2,HORN,PULSED},{3, f8, ENGINE}}},     //8
  {"Union P","GP20 47",58,47,"16",{{0, f0, F0},{1, f1,BELL},{2,f2,HORN,PULSED},{3, f8, ENGINE}}},         //9
  {"Rio Grnd","SD60-M",5200,52,"0",{{0, f0, F0},{1, f1,BELL},{2,f2,HORN,PULSED},{3, f8, ENGINE}}},        //10
  {"GN","VO1000",58,53,"14",{{0, f0, F0},{1, f1,BELL},{2,f2,HORN,PULSED},{3, f8, ENGINE}}},               //11
  {"Cleaner","GN BNDY",58,54,"19",{{0, f0, F0},{1, f1,BELL},{2,f2,HORN,PULSED},{3, f8, ENGINE}}},         //12
  {"CN Rd","SD40-2M",5600,56,"21",{{0, f0, F0},{1, f1,BELL},{2,f2,HORN,PULSED},{3, f8, ENGINE}}},         //13  
  {"Pennsy","NW2",5900,59,"22",{{0, f0, F0},{1, f1,BELL},{2,f2,HORN,PULSED},{3, f8, ENGINE}}},            //14
  {"Erie Lak","SD45 EMD",3619,61,"5",{{0, f0, F0},{1, f1,BELL},{2,f2,HORN,PULSED},{3, f8, ENGINE}}},      //15
  {"Erie Lak","SD45 EMD",3619,61,"23",{{0, f0, F0},{1, f1,BELL},{2,f2,HORN,PULSED},{3, f8, ENGINE}}},     //16
  {"NYC P&LE","S4 NY Central",752,62,"8",{{0, f0, F0},{1, f1,BELL},{2,f2,HORN,PULSED},{3, f8, ENGINE}}},  //17
  {"NYC P&LE","S4 NY Central",752,62,"24",{{0, f0, F0},{1, f1,BELL},{2,f2,HORN,PULSED},{3, f8, ENGINE}}}, //18
  {"Nor Sthr","GEVO ET 44AC",58,63,"11",{{0, f0, F0},{1, f1,BELL},{2,f2,HORN,PULSED},{3, f8, ENGINE}}},   //19
  {"Nor Sthr","GEVO ET 44AC",58,63,"25",{{0, f0, F0},{1, f1,BELL},{2,f2,HORN,PULSED},{3, f8, ENGINE}}},   //20
  {"UnionPac","SD40-2 Sn",6700,67,"26",{{0, f0, F0},{1, f1,BELL},{2,f2,HORN,PULSED},{3, f8, ENGINE}}},    //21
  {"NY Cent","RS3",7003,70,"1",{{0, f0, F0},{1, f1,BELL},{2,f2,HORN,PULSED},{3, f8, ENGINE}}},            //22
  {"C&P RD","GP9 TT",58,75,"18",{{0, f0, F0},{1, f1,BELL},{2,f2,HORN,PULSED},{3, f8, ENGINE}}},           //23
  {"C&P RD","GP9 TT",58,75,"27",{{0, f0, F0},{1, f1,BELL},{2,f2,HORN,PULSED},{3, f8, ENGINE}}},           //24
  {"BNSF","Dash8 40",8500,85,"31",{{0, f0, F0},{1, f1,BELL},{2,f2,HORN},{3, f8, ENGINE}}},                //25
  {"BNSF","Dash 8B",58,86,"15",{{0, f0, F0},{1, f1,BELL},{2,f2,HORN,PULSED},{3, f8, ENGINE}}},            //26
  {"BNSF","Dash 8B",58,86,"28",{{0, f0, F0},{1, f1,BELL},{2,f2,HORN,PULSED},{3, f8, ENGINE}}},            //27
  {"NYC","NW2",8700,87,"29",{{0, f0, F0},{1, f1,BELL},{2,f2,HORN,PULSED},{3, f8, ENGINE}}},               //28
  {"Montana","F45 EMD",58,91,"10",{{0, f0, F0},{1, f1,BELL},{2,f2,HORN},{3, f8, ENGINE}}},                //29
  {"Montana","F45 EMD",58,91,"30",{{0, f0, F0},{1, f1,BELL},{2,f2,HORN},{3, f8, ENGINE}}},                //30
  {"Cleaner","Cleaner",126,126,"7",{{0, f0, F0},{1, f1,BELL},{2,f2,HORN,PULSED},{3, f8, ENGINE}}},        //31
  {"B&O","CLEANER",127,1270,"33",{{0, f0, F0},{1, f1,BELL},{2,f2,HORN},{3, f8, ENGINE}}}                  //32
};

String longLocoNames[40] = 
{
  "Grand National",
  "Pennsylvania Rail Road",
  "Canadian National Stripes",
  "New Haven",
  "Conrail",
  "Conrail",
  "Seaboard",
  "B&O Doodlebug",
  "Canadian National BCOL",
  "Union Pacific",
  "Rio Grande",
  "Grand National",
  "Cleaner 1",
  "Canadian National",
  "Pennsy",
  "Erie Lackawana",
  "Erie Lackawana",
  "New York Central P&LE",
  "NYC P&LE",
  "Northern Southern",
  "Northern Southern",
  "Union Pacific",
  "New York Central",
  "Canadian & Pacific Road",
  "C & P RD",
  "Burlington Northern Santa Fe",
  "BNSF",
  "BNSF",
  "New York Central",
  "Montana",
  "Montana",
  "Cleaner 2",
  "B&O Cleaner",
};
/*
 * Hard Coded Values for ACCESSORIES
 * 
 * Definitions for Accessories are similar to those for Locos, but a lot simpler:-)
 * Names are limited to 8 Characters
 * Addresses can range from 1 to 9999 (or as defined by the CS)
 * Possible Image short names are as follows:-
 *   LH0        Left Hand 0 degrees
 *   LH90 200   Left Hand 90 Degrees
 *   LH180 206  Left Hand 180 Degrees
 *   LH270 204  Left Hand 270 Degrees
 *   RH0 210    Right Hand 0 degrees
 *   RH90 208   Right Hand 90 Degrees
 *   RH180 214  Right Hand 180 Degrees
 *   RH270 212  Right Hand 270 Degrees
 * At this stage Accessory Types are not used

 * Modify NUM_HCACCS to suit the number of Hard Coded Accessories you wish to define
 * The maximum supported is 96
 * 
*/
#define NUM_HCACCS 12

HCAcc hcAcc[NUM_HCACCS] = {
  { "ACC1", 100, LH0},    //Name, Address, Image, Type
  { "ACC2", 101, RH180},
  { "ACC3", 102, RH0},
  { "ACC4", 103, LH180},
  { "ACC5", 104, RH0},
  { "ACC6", 105, LH180},
  { "ACC7", 106, LH0},
  { "ACC8", 107, RH180},
  { "ACC9", 108, LH0},
  { "ACC10", 109, RH180},
  { "ACC11", 110, LH0},
  { "ACC12", 111, RH180},
};
/*
 * Hard Coded Values for ROUTES
 * 
 * A Route is a list of Accessories which will be activated in sequence with a 250ms delay between activations
 * A route is comprised of up to six accessory and desired state pairs
 * Accessories are defined using their ID (obtainable from the Nextion Accessory Pages)
 * followed by the desired State of the Accessory.
 * The Maximum number of routes supported is 48
*/
#define NUM_HCROUTES 2
#define Thrown 1
#define Closed 0

HCRoute hcRoute[NUM_HCROUTES] = {
  {1,Thrown,2,Thrown,3,Thrown,4,Closed,5,Thrown,6,Closed},
  {1,Closed,2,Closed,3,Closed,4,Thrown,5,Closed,6,Thrown},
};
#endif
