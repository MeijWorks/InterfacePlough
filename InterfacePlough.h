/*
  Interface - a library for the MeijWorks interface
 Copyright (C) 2011-2014 J.A. Woltjer.
 All rights reserved.
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef InterfacePlough_h
#define InterfacePlough_h

#include <LiquidCrystal_I2C.h>
#include <ImplementPlough.h>
#include <VehicleTractor.h>
#include <VehicleGps.h>
#include <ConfigInterface.h>
#include <language.h>

// Software version of this library
#define INTERFACE_VERSION 0.1

//#define GPS

// State variable
enum modes{
  AUTO, HOLD, ZEROISE, MANUAL, CALIBRATE
};

class InterfacePlough {
private:
  //-------------
  // data members
  //-------------
  
  // Mode
  modes mode;
  
  // Button flag and timer
  int buttons;
  boolean button1_flag;
  boolean button2_flag;
  unsigned long button1_timer;
  unsigned long button2_timer;

  // Objects
  LiquidCrystal_I2C * lcd;
  ImplementPlough * implement;
  VehicleTractor * tractor;
  VehicleGps * gps;
public:
  // ----------------------------------------------------
  // public member functions implemented in Interface.cpp
  // ----------------------------------------------------

  // Constructor
  InterfacePlough(LiquidCrystal_I2C * _lcd,
            ImplementPlough * _implement,
            VehicleTractor * _tractor,
            VehicleGps * _gps);
            
  void updateScreen(boolean _rewrite);
  int checkButtons(int _delay1, int _delay2);
  void calibrate(); 
  modes update();
  
  inline int getButtons(){
    return buttons;
  };
};
#endif