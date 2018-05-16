#ifndef TB67H420FTG_H
#define TB67H420FTG_H

#include "Arduino.h"

/*
  Copyright 2018 Chris Albertson 
  
  This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License (LGPL) 
    as published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#define MAXPWM 255  // The AVR can only do 8-bits

#define CW    1
#define CCW  -1 

#define MOTOR_A 0
#define MOTOR_B 1


#define ERR_OK            0
#define ERR_OPEN_LOAD     1
#define ERR_OVER_CURRENT  2
#define ERR_OVER_TEMP     3


class Tb67h420ftg
{
  public:
    Tb67h420ftg() {
      Tb67h420ftg(CW, CW);
    }
    
    Tb67h420ftg(byte directA, byte directB) {

      for (int motor=0; motor<=1; motor++){
        pinMode(in1pin[motor], OUTPUT);
        pinMode(in2pin[motor], OUTPUT);
        brake(motor);
 
        pinMode(pwmpin[motor], OUTPUT);
        analogWrite(pwmpin[motor], 0);
      }
      pinMode(lo1pin, INPUT);
      pinMode(lo2pin, INPUT);
      
      setInstalledDirection(MOTOR_A, directA);
      setInstalledDirection(MOTOR_B, directB);
    }
    
    void go(byte motor, byte direct, int pwm) {
      
      if ((InstalledDirection[motor] * CW) == direct) {
        digitalWrite(in1pin[motor], HIGH);
        digitalWrite(in2pin[motor], LOW);
      }
      else {
        digitalWrite(in1pin[motor], LOW);
        digitalWrite(in2pin[motor], HIGH);
      }
      
      analogWrite(pwmpin[motor], pwm);
    }

    void clearFault(byte motor) {
      
    }
    
    void brake(byte motor) {
      digitalWrite(in1pin[motor], HIGH);
      digitalWrite(in2pin[motor], HIGH);
    }
    
    void coast(byte motor) {
      digitalWrite(in1pin[motor], LOW);
      digitalWrite(in2pin[motor], LOW);
    }
    
    int  getStatus() {
      
      byte lo1 = digitalRead(lo1pin);
      byte lo2 = digitalRead(lo2pin);
      Serial.print(lo1);
      Serial.print(lo2);
      Serial.print(" ");
      int  rtn;

      if (HIGH == lo1) {
        if (HIGH == lo2) {
          // lo1 1, lo2 1
          rtn = ERR_OK;             // Normal Status, no error
        }
        else {
          // lo1 HIGH, lo2 LOW
          rtn = ERR_OPEN_LOAD;      // open load (OPD)
        }
      }
      else {
        if (HIGH == lo2) {
          // lo1 LOW, lo2 HIGH
          rtn = ERR_OVER_CURRENT;   // over current (ISD)
        }
        else {
          // lo1 LOW, lo2 LOW
          rtn = ERR_OVER_TEMP;      // over thermal (TSD)
        }
      }
      return rtn;
    }
    void setInstalledDirection(byte motor, byte direct) {
      InstalledDirection[motor] = direct;
    }

   
  private:
    /*  pin definitions
        xxx[0] controls motor A outputs
        xxx[1] controls motor B outputs */

    /* STM32 and TB67 are conected as follows:
     *  PB0 -- INA1
     *  PA7 -- INA2
     *  PA6 -- PWMA
     *  PA5 -- INB1
     *  PA4 -- INB2
     *  PA3 -- PWMB
     *  PA2 -- LO1
     *  PA1 -- LO2
     */
        
    const byte in1pin[2] = {PB0, PA5};
    const byte in2pin[2] = {PA7, PA4};
    const byte pwmpin[2] = {PA6, PA3};
    const byte lo1pin    = PA2;
    const byte lo2pin    = PA1;

    int InstalledDirection[2];
};


#endif /* TB67H420FTG_H */






