//#include <Joystick2.h>
#include "HID-Project.h"
#include "SNESPad/SNESpad.h"
/* (c) Stuart Teasdale 2020 
 *  See LICENSE for distribution terms
 *  
 *  SNES access code derived from https://github.com/burks10/Arduino-SNES-Controller
 Creator: Anthony Burkholder
*/

/** PINS **/
int DATA_CLOCK    = 2;
int DATA_LATCH    = 4;
int DATA_SERIAL   = 6;



enum snes_controller {
  SNES_B,
  SNES_Y,
  SNES_SELECT,
  SNES_START,
  SNES_UP,
  SNES_DOWN,
  SNES_LEFT,
  SNES_RIGHT,
  SNES_A,
  SNES_X,
  SNES_L,
  SNES_R
};
int button_order[8] = {SNES_A,SNES_B,SNES_X,SNES_Y,SNES_L,SNES_R,SNES_SELECT,SNES_START}; 
void setup () {
//  Joystick[0].begin();
//Serial.begin(115200);
  SNESpad.begin();
  SNESpad.releaseAll();
  setupPins ();
}

void loop () {
  
  RXTXControllerData ();
}

void setupPins () {
  /** Set DATA_CLOCK normally HIGH **/
  pinMode (DATA_CLOCK, OUTPUT);
  digitalWrite (DATA_CLOCK, HIGH);
  
  /** Set DATA_LATCH normally LOW **/
  pinMode (DATA_LATCH, OUTPUT);
  digitalWrite (DATA_LATCH, LOW);

  /** Set DATA_SERIAL normally HIGH **/
  pinMode (DATA_SERIAL, OUTPUT);
  digitalWrite (DATA_SERIAL, HIGH);
  pinMode (DATA_SERIAL, INPUT);  
}

void RXTXControllerData () {

  /** Data store for current state of buttons **/
  int buttons[12];
  uint8_t dpad;
  uint32_t b;
  /** Latch for 12us **/
  digitalWrite(DATA_LATCH, HIGH);
  delayMicroseconds(12);
  digitalWrite(DATA_LATCH, LOW);
  delayMicroseconds(6);

  /** Read data bit by bit from SR **/
  for (int i = 0; i < 16; i++) {
    digitalWrite (DATA_CLOCK, LOW);
    delayMicroseconds (6);
    if (i <= 11) {
      buttons[i] = !digitalRead (DATA_SERIAL);
    }
    digitalWrite (DATA_CLOCK, HIGH);
    delayMicroseconds (6);
  }
  
  //Code the dpad
  dpad = codeDPad(buttons[SNES_UP],buttons[SNES_DOWN],buttons[SNES_LEFT],buttons[SNES_RIGHT]);
  //Code the Buttons
  b=0;
  for (int i=0; i < 8; i++) {
    b |= (buttons[button_order[i]] << i);
  }
  /** Set Joystick state based on SNES input **/
  SNESpad.dPad1(dpad);
  SNESpad.buttons(b);
  SNESpad.write();

/*      
    Serial.print(dpad);
    Serial.print('|');
    Serial.print(b);
    Serial.print('|');  
  for (int i = 0; i <= 11; i++) {

    Serial.print(buttons[i]);
    Serial.print(' ');
  }
  Serial.println(); */
}

uint8_t codeDPad( int u, int d, int l, int r ) {
  if (u){
    if (l){ return GAMEPAD_DPAD_UP_LEFT;}
    else if (r){ return GAMEPAD_DPAD_UP_RIGHT;}
    else { return GAMEPAD_DPAD_UP;}
  } else if (d) {
    if (l){ return GAMEPAD_DPAD_DOWN_LEFT;}
    else if (r) { return GAMEPAD_DPAD_DOWN_RIGHT;}
    else {return GAMEPAD_DPAD_DOWN;}
  } else if (l) {
    return GAMEPAD_DPAD_LEFT;
  } else if (r) {
    return GAMEPAD_DPAD_RIGHT;
  }
  
  return GAMEPAD_DPAD_CENTERED;
}
