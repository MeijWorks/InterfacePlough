/*
  Interface - a libary for the MeijWorks interface
 Copyright (C) 2011-2015 J.A. Woltjer.
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

#include "InterfacePlough.h"

// -----------
// Constructor
// -----------
InterfacePlough::InterfacePlough(LiquidCrystal_I2C * _lcd,
                     ImplementPlough * _implement,
                     VehicleTractor * _tractor,
                     VehicleGps * _gps){
  // Pin assignments and configuration
  // Schmitt triggered inputs
  pinMode(LEFT_BUTTON, INPUT);
  pinMode(RIGHT_BUTTON, INPUT);
  pinMode(MODE_PIN, INPUT);
  
  digitalWrite(LEFT_BUTTON, LOW);
  digitalWrite(RIGHT_BUTTON, LOW);
  digitalWrite(MODE_PIN, LOW);
  
  // Mode
  mode = 2;  // MANUAL
  
  // Button flag
  buttons = 0;
  button1_flag = false;
  button2_flag = false;

  // Connected classes
  lcd = _lcd;
  implement = _implement;
  tractor = _tractor;
  gps = _gps;
}

// ------------------------
// Method for updating mode
// ------------------------
void InterfacePlough::update(){
  // Check buttons
  checkButtons(255, 0);
  
  // update GPS and tractor
  gps->update();
  tractor->update();
  
  // Check for mode change

  // ---------
  // Calibrate
  // ---------
  if(buttons == 2){
    mode = 3;

#ifdef DEBUG
    Serial.println("C");
#endif 

    // Calibrate
    calibrate();
  }
  // ------
  // Manual
  // ------
  else if(!digitalRead(MODE_PIN) ||
          tractor->getHitch()){
    // set mode to manual
    mode = 2;

#ifdef DEBUG
    Serial.println("M");
#endif
  }
  else {
    // ----
    // Hold
    // ----
    if (millis() - gps->getGgaFixAge() > 2000 ||
        millis() - gps->getVtgFixAge() > 2000 ||
        millis() - gps->getXteFixAge() > 2000 ||
        gps->getQuality() != 4 ||
        !gps->minSpeed()){
      // set mode to hold
      mode = 1;
      
#ifdef DEBUG
      Serial.println("H");
#endif
    }
    // ---------
    // Automatic
    // ---------
    else {
      // set mode to automatic
      mode = 0;

#ifdef DEBUG
      Serial.println("A");
#endif
    }
  }
  
  // Update implement and adjust
  implement->update(mode, buttons);
  implement->adjust(buttons);
  
  // Update screen (no rewrite) and write one character
  updateScreen(0); 
  lcd->write_screen(1);  
}

// --------------------------
// Method for updating screen
// --------------------------
void InterfacePlough::updateScreen(boolean _rewrite){
  int temp = 0;
  int temp2 = 0;

  // Update screen
  if (_rewrite){
    // Regel 0
    lcd->write_buffer(L_POS, 0);

    // Regel 1
    lcd->write_buffer(L_A_POS, 1);

    // Regel 2
    lcd->write_buffer(L_XTE, 2);

#ifdef ROTATION
    // Regel 3
    lcd->write_buffer(L_ROTATION, 3);
#endif

    lcd->write_screen(-1);
  }

  // Regel 0
  temp2 = implement->getOffset();
  temp = abs(temp2);

  if (temp > 99){
    if (temp2 < 0){
      lcd->write_buffer('-', 0, 16);
    }
    else {
      lcd->write_buffer(' ', 0, 16);
    }
    lcd->write_buffer(temp / 100 + '0', 0, 17);
    temp = temp % 100;
    lcd->write_buffer(temp / 10 + '0', 0, 18);
    temp = temp % 10;
    lcd->write_buffer(temp + '0', 0, 19);
  }
  else if (temp > 9){
    lcd->write_buffer(' ', 0, 16);
    if (temp2 < 0){
      lcd->write_buffer('-', 0, 17);
    }
    else {
      lcd->write_buffer(' ', 0, 17);
    }
    lcd->write_buffer(temp / 10 + '0', 0, 18);
    temp = temp % 10;
    lcd->write_buffer(temp + '0', 0, 19);
  }
  else {
    lcd->write_buffer(' ', 0, 16);
    lcd->write_buffer(' ', 0, 17);
    if (temp2 < 0){
      lcd->write_buffer('-', 0, 18);
    }
    else {
      lcd->write_buffer(' ', 0, 18);
    }
    lcd->write_buffer(temp + '0', 0, 19);
  }

  // Regel 1
  temp2 = implement->getPosition();
  temp = abs(temp2);

  if (temp > 99){
    if (temp2 < 0){
      lcd->write_buffer('-', 1, 16);
    }
    else {
      lcd->write_buffer(' ', 1, 16);
    }
    lcd->write_buffer(temp / 100 + '0', 1, 17);
    temp = temp % 100;
    lcd->write_buffer(temp / 10 + '0', 1, 18);
    temp = temp % 10;
    lcd->write_buffer(temp + '0', 1, 19);
  }
  else if (temp > 9){
    lcd->write_buffer(' ', 1, 16);
    if (temp2 < 0){
      lcd->write_buffer('-', 1, 17);
    }
    else {
      lcd->write_buffer(' ', 1, 17);
    }
    lcd->write_buffer(temp / 10 + '0', 1, 18);
    temp = temp % 10;
    lcd->write_buffer(temp + '0', 1, 19);
  }
  else {
    lcd->write_buffer(' ', 1, 16);
    lcd->write_buffer(' ', 1, 17);
    if (temp2 < 0){
      lcd->write_buffer('-', 1, 18);
    }
    else {
      lcd->write_buffer(' ', 1, 18);
    }
    lcd->write_buffer(temp + '0', 1, 19);
  }


  // Regel 2
  temp2 = gps->getXte();
  temp = abs(temp2);

  if (temp > 99){
    if (temp2 < 0){
      lcd->write_buffer('-', 2, 16);
    }
    else {
      lcd->write_buffer(' ', 2, 16);
    }
    lcd->write_buffer(temp / 100 + '0', 2, 17);
    temp = temp % 100;
    lcd->write_buffer(temp / 10 + '0', 2, 18);
    temp = temp % 10;
    lcd->write_buffer(temp + '0', 2, 19);
  }
  else if (temp > 9){
    lcd->write_buffer(' ', 2, 16);
    if (temp2 < 0){
      lcd->write_buffer('-', 2, 17);
    }
    else {
      lcd->write_buffer(' ', 2, 17);
    }
    lcd->write_buffer(temp / 10 + '0', 2, 18);
    temp = temp % 10;
    lcd->write_buffer(temp + '0', 2, 19);
  }
  else {
    lcd->write_buffer(' ', 2, 16);
    lcd->write_buffer(' ', 2, 17);
    if (temp2 < 0){
      lcd->write_buffer('-', 2, 18);
    }
    else {
      lcd->write_buffer(' ', 2, 18);
    }
    lcd->write_buffer(temp + '0', 2, 19);
  }

#ifdef ROTATION
  // Regel  3
  temp2 = implement->getRotation();
  temp = abs(temp2);

  if (temp > 99){
    if (temp2 < 0){
      lcd->write_buffer('-', 3, 9);
    }
    else {
      lcd->write_buffer(' ', 3, 9);
    }
    lcd->write_buffer(temp / 100 + '0', 3, 10);
    temp = temp % 100;
    lcd->write_buffer(temp / 10 + '0', 3, 11);
    temp = temp % 10;
    lcd->write_buffer(temp + '0', 3, 12);
  }
  else if (temp > 9){
    lcd->write_buffer(' ', 3, 9);
    if (temp2 < 0){
      lcd->write_buffer('-', 3, 10);
    }
    else {
      lcd->write_buffer(' ', 3, 10);
    }
    lcd->write_buffer(temp / 10 + '0', 3, 11);
    temp = temp % 10;
    lcd->write_buffer(temp + '0', 3, 12);
  }
  else {
    lcd->write_buffer(' ', 3, 9);
    lcd->write_buffer(' ', 3, 10);
    if (temp2 < 0){
      lcd->write_buffer('-', 3, 11);
    }
    else {
      lcd->write_buffer(' ', 3, 11);
    }
    lcd->write_buffer(temp + '0', 3, 12);
  }
#endif
  
  if (implement->getSide()){
    lcd->write_buffer('L', 3, 15);
  }
  else {
    lcd->write_buffer('R', 3, 15);
  }
  
  switch (mode){
  case 0: // AUTO
    lcd->write_buffer('A', 3, 14);
    lcd->write_buffer(' ', 3, 17);
    lcd->write_buffer(' ', 3, 18);
    break;
  case 1: // HOLD
    lcd->write_buffer('H', 3, 14);
    if (gps->minSpeed()){
      lcd->write_buffer('G', 3, 17);
      lcd->write_buffer('!', 3, 18);
    }
    else{
      lcd->write_buffer('S', 3, 17);
      lcd->write_buffer('!', 3, 18);
    }
    break;
  case 2: // MANUAL
    lcd->write_buffer('M', 3, 14);

    if (buttons == -1){
      lcd->write_buffer('<', 3, 17);
      lcd->write_buffer(' ', 3, 18);
    }
    else if (buttons == 1){
      lcd->write_buffer(' ', 3, 17);
      lcd->write_buffer('>', 3, 18);
    }
    else{
      lcd->write_buffer(' ', 3, 17);
      lcd->write_buffer(' ', 3, 18);
    }
    break;
  }
}

// ---------------------------
// Method for checking buttons
// ---------------------------
int InterfacePlough::checkButtons(byte _delay1, byte _delay2){
  if (button1_flag){
    button1_timer = millis();
    button1_flag = false;
  }
  
  if (button2_flag){
    button2_timer = millis();
    button2_flag = false;
  }
  
  // Check for left/right button presses
  if(digitalRead(LEFT_BUTTON) && digitalRead(RIGHT_BUTTON)){
    if(millis() - button1_timer >= _delay1 * 4){
      button1_flag = true;
      buttons = 2;
      return 2;
    }
    else{
      button2_flag = true;
      buttons = 0;
      return 0;
    }
  }
  else if(digitalRead(LEFT_BUTTON)){
    if(millis() - button2_timer >= _delay2){
      button2_flag = true;
      buttons = -1;
      return -1;
    }
    else{
      button1_flag = true;
      buttons = 0;
      return 0;
    }
  }
  else if(digitalRead(RIGHT_BUTTON)){
    if(millis() - button2_timer >= _delay2){
      button2_flag = true;
      buttons = 1;
      return 1;
    }
    else{
      button1_flag = true;
      buttons = 0;
      return 0;
    }
  }
  else{
    button1_flag = true;
    button2_flag = true;
    buttons = 0;
    return 0;
  }
}

// --------------------------------
// Method for calibrating implement
// --------------------------------
void InterfacePlough::calibrate(){
  // Stop any adjusting
  implement->stop();
  
  // Write complete screen
  lcd->write_screen(-1);
  
  // Temporary variables
  int _temp, _temp2, _temp3;
  
  // --------------------
  // Position calibration
  // --------------------
  lcd->write_buffer(L_CAL_POS, 0);
  lcd->write_buffer(L_CAL_ACCEPT, 1);
  lcd->write_buffer(L_CAL_DECLINE, 2);
  lcd->write_buffer(L_BLANK, 3);

  lcd->write_screen(-1);

  while(checkButtons(0, 0) != 0){
  }

  while(true){
    if(checkButtons(0, 0) == -1){
      // print message to LCD
      lcd->write_buffer(L_CAL_DECLINED, 1);
      lcd->write_buffer(L_BLANK, 2);
      
      lcd->write_screen(-1);
      break;
    }
    else if(checkButtons(0, 0) == 1){
      // Width calibration
      lcd->write_buffer(L_CAL_ADJUST, 1);
      lcd->write_buffer(L_CAL_ENTER, 2);
      lcd->write_buffer(L_CAL_POS_AD, 3);
      
      lcd->write_screen(-1);

      // Loop through calibration process
      for(int i = 0; i < 3; i++){
        _temp = implement->getPositionCalibrationPoint(i);
        _temp2 = _temp / 10;
        
        if (_temp < 0){
          lcd->write_buffer('-', 3, 12);
        }
        else{
          lcd->write_buffer(' ', 3, 12);
        }
        lcd->write_buffer(abs(_temp2) + '0', 3, 13);
        lcd->write_buffer(abs(_temp) % 10 + '0', 3, 14);

        lcd->write_screen(3);

        while(checkButtons(0, 0) != 0){
        }

        // Adjust loop
        while(true){
          lcd->write_screen(1);
          checkButtons(0, 0);
          implement->adjust(buttons);
          
          if (buttons == 1){
            lcd->write_buffer('>', 3, 19);
          }
          else if (buttons == -1){
            lcd->write_buffer('<', 3, 19);
          }
          else if (buttons == 2){
            implement->adjust(0);

            implement->setPositionCalibrationData(i);

            break;
          }
          else {
            lcd->write_buffer(' ', 3, 19);
          }
        }
      }
      lcd->write_buffer(L_CAL_DONE, 1);
      lcd->write_buffer(L_BLANK, 2);
      lcd->write_buffer(L_BLANK, 3);

      lcd->write_screen(-1);

      break;
    }
  }
  delay(1000);

#ifdef ROTATION
  // Rotation calibration
  lcd->write_buffer(L_CAL_ROTATION, 0);
  lcd->write_buffer(L_CAL_ACCEPT, 1);
  lcd->write_buffer(L_CAL_DECLINE, 2);
  lcd->write_buffer(L_BLANK, 3);

  lcd->write_screen(-1);

  while(checkButtons(0, 0) != 0){
  }

  while(true){
    if(checkButtons(0, 0) == -1){
      // print message to LCD
      lcd->write_buffer(L_CAL_DECLINED, 1);
      lcd->write_buffer(L_BLANK, 2);
      
      lcd->write_screen(-1);
      break;
    }
    else if(checkButtons(0, 0) == 1){
      // Rotation calibration
      lcd->write_buffer(L_BLANK, 1);
      lcd->write_buffer(L_CAL_ENTER, 2);
      lcd->write_buffer(L_CAL_ROTATION_AD, 3);

      lcd->write_screen(-1);

      // Loop through calibration process
      for(int i = 0; i < 3; i++){
        _temp = implement->getRotationCalibrationPoint(i);
        _temp2 = _temp / 10;
        
        if (_temp < 0){
          lcd->write_buffer('-', 3, 12);
        }
        else{
          lcd->write_buffer(' ', 3, 12);
        }
        lcd->write_buffer(abs(_temp2) + '0', 3, 13);
        lcd->write_buffer(abs(_temp) % 10 + '0', 3, 14);

        lcd->write_screen(3);

        while(checkButtons(0, 0) != 0){
        }

        // Adjust loop
        while(true){ 
          lcd->write_screen(1);
          checkButtons(0, 0);
          
          if (buttons == 2){
            implement->setRotationCalibrationData(i);

            break;
          }
        }
      }
      lcd->write_buffer(L_CAL_DONE, 1);
      lcd->write_buffer(L_BLANK, 2);
      lcd->write_buffer(L_BLANK, 3);

      lcd->write_screen(-1);

      break;
    }
  }
  delay(1000);
#endif

#ifdef SPEED_L
  // ----------------
  // SPEED calibration
  // ----------------
  lcd->write_buffer(L_CAL_SPEED, 0);
  lcd->write_buffer(L_CAL_ACCEPT, 1);
  lcd->write_buffer(L_CAL_DECLINE, 2);
  lcd->write_buffer(L_BLANK, 3);

  lcd->write_screen(-1);

  while(checkButtons(0, 0) != 0){
  }

  while(true){

    if(checkButtons(0, 0) == -1){
      // print message to LCD
      lcd->write_buffer(L_CAL_DECLINED, 1);
      lcd->write_buffer(L_BLANK, 2);
      lcd->write_buffer(L_BLANK, 3);

      lcd->write_screen(-1);

      break;
    }
    else if(checkButtons(0, 0) == 1){
      // Speed calibration
      lcd->write_buffer(L_BLANK, 1);
      lcd->write_buffer(L_CAL_ENTER, 2);
      lcd->write_buffer(L_CAL_SPEED_AD, 3);

      lcd->write_screen(-1);

      // Loop through calibration process
      tractor->resetWheelspeedPulses();
      
      while(checkButtons(0, 0) != 0){
      }

      // Adjust loop
      while(true){ 
        lcd->write_screen(1);
  
        checkButtons(0, 255);
        
        _temp = tractor->calibrateSpeed(buttons);

        if (buttons == 2){
          break;
        }
        
        _temp = _temp / 100;
        
        // Calculate derived variables
        _temp2 = _temp / 10;
        _temp3 = _temp / 100;
        
        lcd->write_buffer(abs(_temp3) + '0', 3, 14);
        lcd->write_buffer(abs(_temp2) % 10 + '0', 3, 15);
        lcd->write_buffer(abs(_temp) % 10 + '0', 3, 16);
      }
      
      lcd->write_buffer(L_CAL_DONE, 1);
      lcd->write_buffer(L_BLANK, 2);

      lcd->write_screen(-1);

      break;
    }
  }
  delay(1000);
#endif

  // -----------------------
  // Adjust number of shares
  // -----------------------
  lcd->write_buffer(L_CAL_SHARES, 0);
  lcd->write_buffer(L_CAL_ACCEPT, 1);
  lcd->write_buffer(L_CAL_DECLINE, 2);
  lcd->write_buffer(L_BLANK, 3);

  lcd->write_screen(-1);

  while(checkButtons(0, 0) != 0){
  }

  while(true){
    if(checkButtons(0, 0) == -1){
      // print message to LCD
      lcd->write_buffer(L_CAL_DECLINED, 1);
      lcd->write_buffer(L_BLANK, 2);
      
      lcd->write_screen(-1);
      break;
    }
    else if(checkButtons(0, 0) == 1){
      // Adjust amount of shares
      lcd->write_buffer(L_CAL_ADJUST, 1);
      lcd->write_buffer(L_CAL_ENTER, 2);
      lcd->write_buffer(L_CAL_SHARES_AD, 3);

      lcd->write_screen(-1);
      
      _temp = implement->getShares();
      
      while(checkButtons(0, 0) != 0){
      }

      // Adjust loop
      while(true){
        lcd->write_screen(1);
        checkButtons(0, 255);
        
        if (buttons == 1){
          _temp ++;
        }
        else if (buttons == -1){
          _temp --;
        }
        else if (buttons == 2){
          break;
        }
        // Calculate derived variables
        _temp2 = _temp / 10;
        
        // Write to screen
        lcd->write_buffer(_temp2 + '0', 3, 15);
        lcd->write_buffer(_temp % 10 + '0', 3, 16);
      }
      lcd->write_buffer(L_CAL_DONE, 1);
      lcd->write_buffer(L_BLANK, 2);

      lcd->write_buffer(_temp2 + '0', 3, 15);
      lcd->write_buffer(_temp % 10 + '0', 3, 16);

      lcd->write_screen(-1);

      implement->setShares(_temp);
      break;
    }
  }
  delay(1000);

#ifdef KP
  // ---------
  // Adjust KP
  // ---------
  lcd->write_buffer(L_CAL_KP, 0);
  lcd->write_buffer(L_CAL_ACCEPT, 1);
  lcd->write_buffer(L_CAL_DECLINE, 2);
  lcd->write_buffer(L_BLANK, 3);

  lcd->write_screen(-1);

  while(checkButtons(0, 0) != 0){
  }

  while(true){
    if(checkButtons(0, 0) == -1){
      // print message to LCD
      lcd->write_buffer(L_CAL_DECLINED, 1);
      lcd->write_buffer(L_BLANK, 2);
      
      lcd->write_screen(-1);
      break;
    }
    else if(checkButtons(0, 0) == 1){
      // Adjust KP
      lcd->write_buffer(L_CAL_ADJUST, 1);
      lcd->write_buffer(L_CAL_ENTER, 2);
      lcd->write_buffer(L_CAL_KP_AD, 3);

      lcd->write_screen(-1);
      
      _temp = implement->getKP();
      
      while(checkButtons(0, 0) != 0){
      }
      
      // Adjust loop
      while(true){
        lcd->write_screen(1);
        checkButtons(0, 255);

        if (buttons == 1){
          _temp ++;
]        }
        else if (buttons == -1){
          _temp --;
        }
        else if (buttons == 2){
          break;
        }
        // Calculate derived variables
        _temp2 = _temp / 10;
        _temp3 = _temp / 100;

        // Write to screen
        lcd->write_buffer(_temp3 + '0', 3, 13);
        lcd->write_buffer('.', 3, 14);
        lcd->write_buffer(_temp2 % 10 + '0', 3, 15);
        lcd->write_buffer(_temp  % 10 + '0', 3, 16);

      }
      lcd->write_buffer(L_CAL_DONE, 1);
      lcd->write_buffer(L_BLANK, 2);

      lcd->write_buffer(_temp3 + '0', 3, 13);
      lcd->write_buffer('.', 3, 14);
      lcd->write_buffer(_temp2 % 10 + '0', 3, 15);
      lcd->write_buffer(_temp % 10 + '0', 3, 16);

      lcd->write_screen(-1);

      implement->setKP(_temp);
      break;
    }
  }
  delay(1000);
#endif

#ifdef PWM_MAN
  // -----------------
  // Adjust PWM manual
  // -----------------
  lcd->write_buffer(L_CAL_PWM_M, 0);
  lcd->write_buffer(L_CAL_ACCEPT, 1);
  lcd->write_buffer(L_CAL_DECLINE, 2);
  lcd->write_buffer(L_BLANK, 3);

  lcd->write_screen(-1);

  while(checkButtons(0, 0) != 0){
  }

  while(true){
    if(checkButtons(0, 0) == -1){
      // print message to LCD
      lcd->write_buffer(L_CAL_DECLINED, 1);
      lcd->write_buffer(L_BLANK, 2);
      
      lcd->write_screen(-1);
      break;
    }
    else if(checkButtons(0, 0) == 1){
      // Adjust PWM manual
      lcd->write_buffer(L_CAL_ADJUST, 1);
      lcd->write_buffer(L_CAL_ENTER, 2);
      lcd->write_buffer(L_CAL_PWM_M_AD, 3);

      lcd->write_screen(-1);
      
      _temp = implement->getPwmMan();
      
      while(checkButtons(0, 0) != 0){
      }

      // Adjust loop
      while(true){
        lcd->write_screen(1);
        _buttons = checkButtons(0, 255);

        if (buttons == 1){
          _temp ++;
        }
        else if (buttons == -1){
          _temp --;
        }
        else if (buttons == 2){
          break;
        }
        // Calculate derived variables
        _temp2 = _temp / 10;

        // Write to screen
        lcd->write_buffer(_temp2 + '0', 3, 15);
        lcd->write_buffer(_temp  % 10 + '0', 3, 16);
      }
      lcd->write_buffer(L_CAL_DONE, 1);
      lcd->write_buffer(L_BLANK, 2);

      lcd->write_buffer(_temp2 + '0', 3, 15);
      lcd->write_buffer(_temp % 10 + '0', 3, 16);

      lcd->write_screen(-1);

      implement->setPwmMan(byte(_temp));
      break;
    }
  }
  delay(1000);
#endif

#ifdef PWM_AUTO
  // ---------------
  // Adjust PWM auto
  // ---------------
  lcd->write_buffer(L_CAL_PWM_A, 0);
  lcd->write_buffer(L_CAL_ACCEPT, 1);
  lcd->write_buffer(L_CAL_DECLINE, 2);
  lcd->write_buffer(L_BLANK, 3);

  lcd->write_screen(-1);

  while(checkButtons(0, 0) != 0){
  }

  while(true){
    if(checkButtons(0, 0) == -1){
      // print message to LCD
      lcd->write_buffer(L_CAL_DECLINED, 1);
      lcd->write_buffer(L_BLANK, 2);
      
      lcd->write_screen(-1);
      break;
    }
    else if(checkButtons(0, 0) == 1){
      // Adjust PWM auto
      lcd->write_buffer(L_CAL_ADJUST, 1);
      lcd->write_buffer(L_CAL_ENTER, 2);
      lcd->write_buffer(L_CAL_PWM_A_AD, 3);

      lcd->write_screen(-1);
      
      _temp = implement->getPwmAuto();
      
      while(checkButtons(0, 0) != 0){
      }

      // Adjust loop
      while(true){
        lcd->write_screen(1);
        checkButtons(0, 255);

        if (buttons == 1){
          _temp ++;
        }
        else if (buttons == -1){
          _temp --;
        }
        else if (buttons == 2){
          break;
        }
        // Calculate derived variables
        _temp2 = _temp / 10;

        // Write to screen
        lcd->write_buffer(_temp2 + '0', 3, 15);
        lcd->write_buffer(_temp  % 10 + '0', 3, 16);
      }
      lcd->write_buffer(L_CAL_DONE, 1);
      lcd->write_buffer(L_BLANK, 2);

      lcd->write_buffer(_temp2 + '0', 3, 15);
      lcd->write_buffer(_temp % 10 + '0', 3, 16);

      lcd->write_screen(-1);

      implement->setPwmAuto(byte(_temp));
      break;
    }
  }
  delay(1000);
#endif

  // ------------
  // Adjust error
  // ------------
  lcd->write_buffer(L_CAL_MARGIN, 0);
  lcd->write_buffer(L_CAL_ACCEPT, 1);
  lcd->write_buffer(L_CAL_DECLINE, 2);
  lcd->write_buffer(L_BLANK, 3);

  lcd->write_screen(-1);

  while(checkButtons(0, 0) != 0){
  }

  while(true){
    if(checkButtons(0, 0) == -1){
      // print message to LCD
      lcd->write_buffer(L_CAL_DECLINED, 1);
      lcd->write_buffer(L_BLANK, 2);

      lcd->write_screen(-1);
      break;
    }
    else if(checkButtons(0, 0) == 1){
      // Adjust error
      lcd->write_buffer(L_CAL_ADJUST, 1);
      lcd->write_buffer(L_CAL_ENTER, 2);
      lcd->write_buffer(L_CAL_MARGIN_AD, 3);

      lcd->write_screen(-1);
      
      _temp = implement->getError();
      
      while(checkButtons(0, 0) != 0){
      }

      while(true){
        lcd->write_screen(1);
        checkButtons(0, 255);
        
        if (buttons == 1){
          _temp ++;
        }
        else if (buttons == -1){
          _temp --;
        }
        else if (buttons == 2){
          break;
        }
        // Calculate derived variables
        _temp2 = _temp / 10;

        lcd->write_buffer(_temp2 + '0', 3, 15);
        lcd->write_buffer(_temp % 10 + '0', 3, 16);
      }
      lcd->write_buffer(L_CAL_DONE, 1);
      lcd->write_buffer(L_BLANK, 2);
      
      lcd->write_buffer(_temp2 + '0', 3, 15);
      lcd->write_buffer(_temp % 10 + '0', 3, 16);

      lcd->write_screen(-1);

      implement->setError(byte(_temp));
      break;
    }
  }
  delay(1000);

  // -------------------------
  // Adjust maximum correction
  // -------------------------
  lcd->write_buffer(L_CAL_MAXCOR, 0);
  lcd->write_buffer(L_CAL_ACCEPT, 1);
  lcd->write_buffer(L_CAL_DECLINE, 2);
  lcd->write_buffer(L_BLANK, 3);

  lcd->write_screen(-1);

  while(checkButtons(0, 0) != 0){
  }

  while(true){
    if(checkButtons(0, 0) == -1){
      // print message to LCD
      lcd->write_buffer(L_CAL_DECLINED, 1);
      lcd->write_buffer(L_BLANK, 2);

      lcd->write_screen(-1);
      break;
    }
    else if(checkButtons(0, 0) == 1){
      // Adjust maximum correction
      lcd->write_buffer(L_CAL_ADJUST, 1);
      lcd->write_buffer(L_CAL_ENTER, 2);
      lcd->write_buffer(L_CAL_MAXCOR_AD, 3);

      lcd->write_screen(-1);
      
      _temp = implement->getMaxCorrection();

      while(checkButtons(0, 0) != 0){
      }

      while(true){
        lcd->write_screen(1);
        checkButtons(0, 255);
        
        if (buttons == 1){
          _temp ++;
        }
        else if (buttons == -1){
          _temp --;
        }
        else if (buttons == 2){
          break;
        }
        // Calculate derived variables
        _temp2 = _temp / 10;
        _temp3 = _temp / 100;

        // Write to screen
        lcd->write_buffer(_temp3 + '0', 3, 14);
        lcd->write_buffer(_temp2 % 10 + '0', 3, 15);
        lcd->write_buffer(_temp % 10 + '0', 3, 16);
        }
      lcd->write_buffer(L_CAL_DONE, 1);
      lcd->write_buffer(L_BLANK, 2);

      lcd->write_buffer(_temp3 + '0', 3, 14);
      lcd->write_buffer(_temp2 % 10 + '0', 3, 15);
      lcd->write_buffer(_temp % 10 + '0', 3, 16);

      lcd->write_screen(-1);

      implement->setMaxCorrection(_temp);
      break;
    }
  }
  delay(1000);
  
  // -----------------
  // Adjust ploughside
  // -----------------
  lcd->write_buffer(L_CAL_SWAP, 0);
  lcd->write_buffer(L_CAL_ACCEPT, 1);
  lcd->write_buffer(L_CAL_DECLINE, 2);
  lcd->write_buffer(L_BLANK, 3);

  lcd->write_screen(-1);

  while(checkButtons(0, 0) != 0){
  }

  while(true){
    if(checkButtons(0, 0) == -1){
      // print message to LCD
      lcd->write_buffer(L_CAL_DECLINED, 1);
      lcd->write_buffer(L_BLANK, 2);

      lcd->write_screen(-1);
      break;
    }
    else if(checkButtons(0, 0) == 1){
      // Adjust maximum correction
      lcd->write_buffer(L_CAL_ADJUST, 1);
      lcd->write_buffer(L_CAL_ENTER, 2);
      lcd->write_buffer(L_CAL_SWAP_AD, 3);

      if (implement->getSide()){
        lcd->write_buffer('L', 3, 16);
      }
      else {
        lcd->write_buffer('R', 3, 16);
      }

      while(checkButtons(0, 0) != 0){
      }

      while(true){
        lcd->write_screen(1);
        checkButtons(0, 255);
        
        if (buttons == 1){
          implement->setSwap(true);
        }
        else if (buttons == -1){
          implement->setSwap(false);
        }
        else if (buttons == 2){
          break;
        }
        
        if (implement->getSide()){
          lcd->write_buffer('L', 3, 16);
        }
        else {
          lcd->write_buffer('R', 3, 16);
        }
      }
      lcd->write_buffer(L_CAL_DONE, 1);
      lcd->write_buffer(L_BLANK, 2);

      if (implement->getSide()){
        lcd->write_buffer('L', 3, 16);
      }
      else {
        lcd->write_buffer('R', 3, 16);
      }

      lcd->write_screen(-1);

      break;
    }
  }
  delay(1000);
  
  // ----------
  // Deutz
  // ----------
  lcd->write_buffer(L_CAL_DEUTZ, 0);
  lcd->write_buffer(L_CAL_ACCEPT, 1);
  lcd->write_buffer(L_CAL_DECLINE, 2);
  lcd->write_buffer(L_BLANK, 3);

    lcd->write_screen(-1);

  while(checkButtons(0, 0) != 0){
  }

  while(true){
    if(checkButtons(0, 0) == -1){
      // print message to LCD
      lcd->write_buffer(L_CAL_DECLINED, 1);
      lcd->write_buffer(L_BLANK, 2);

      lcd->write_screen(-1);
      break;
    }
    else if(checkButtons(0, 0) == 1){
      // Setting sim mode
      lcd->write_buffer(L_CAL_ADJUST, 1);
      lcd->write_buffer(L_CAL_ENTER, 2);
      
      lcd->write_screen(-1);

      _temp = tractor->getDeutz();

      while(true){
        lcd->write_screen(1);
        checkButtons(0, 255);

        if (buttons == 1){
          _temp = 1;
        }
        else if (buttons == -1){
          _temp = 0;
        }
        else if (buttons == 2){
          break;
        }

        if (_temp){
          lcd->write_buffer(L_CAL_ON, 3);
        }
        else {
          lcd->write_buffer(L_CAL_OFF, 3);
        }

      }
      lcd->write_buffer(L_CAL_DONE, 1);
      lcd->write_buffer(L_BLANK, 2);
      
      if (_temp){
        tractor->enableDeutz();
      }
      else {
        tractor->disableDeutz();
      }
      
      lcd->write_screen(-1);

      break;
    }
  }
  delay(1000);
  
  // Store calibration data
  lcd->write_buffer(L_CAL_COMPLETE, 0);
  lcd->write_buffer(L_CAL_ACCEPT, 1);
  lcd->write_buffer(L_CAL_DECLINE, 2);
  lcd->write_buffer(L_BLANK, 3);

  lcd->write_screen(-1);

  while(checkButtons(0, 0) != 0){
  }

  while(true){
    if(checkButtons(0, 0) == -1){
      // print message to LCD
      lcd->write_buffer(L_CAL_DECLINED, 1);
      lcd->write_buffer(L_CAL_NOSAVE, 2);

      lcd->write_screen(-1);

      implement->resetCalibration();
      tractor->resetCalibration();

      break;
    }
    else if(checkButtons(0, 0) == 1){
      // Commit data
      implement->commitCalibration();
      tractor->commitCalibration();

      // Print message to LCD
      lcd->write_buffer(L_CAL_DDONE, 1);
      lcd->write_buffer(L_CAL_SAVE, 2);

      lcd->write_screen(-1);

      break;
    }
  }
  delay(1000);
  
  // After calibration rewrite total screen
  updateScreen(1);
  lcd->write_screen(-1);  
}