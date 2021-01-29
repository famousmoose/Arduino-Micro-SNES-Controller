//#include <Joystick2.h>
#include "HID-Project.h"
#include "SNESpad.h"
/* (c) Stuart Teasdale 2020 
 *  See LICENSE for distribution terms
 *  
 *  SNES access code derived from https://github.com/burks10/Arduino-SNES-Controller
 Creator: Anthony Burkholder
*/

/** PINS **/
int DATA_CLOCK    = 2; // SNES pin 2
int DATA_LATCH    = 4; // SNES pin 3
int DATA_SERIAL   = 6; // SNES pin 4
bool DEBUG = false;


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
  if (DEBUG) {Serial.begin(115200);
  } else {
    SNESpad.begin();
    SNESpad.releaseAll();
  }
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

  /** Set DATA_SERIAL normally HIGH, use pullup mode **/
  pinMode (DATA_SERIAL, INPUT_PULLUP);
}

void RXTXControllerData () {

  /** Data store for current state of buttons **/
  int buttons[12];
  uint8_t dpad;
  static uint8_t x = 127;
  static uint8_t y = 127;

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

  if (buttons[SNES_UP]) {y = 0;}
  else if (buttons[SNES_DOWN]){y = 255;}
  else {y = 127;}
  if (buttons[SNES_RIGHT]) {x = 255;}
  else if (buttons[SNES_LEFT]){x = 0;}
  else {x = 127;}

  //Code the Buttons
  b=0;
  for (int i=0; i < 8; i++) {
    b |= (buttons[button_order[i]] << i);
  }

  if (DEBUG){
    Serial.print(dpad);
    Serial.print('|');
    Serial.print(b);
    Serial.print('|');  
    for (int i = 0; i <= 11; i++) {

      Serial.print(buttons[i]);
      Serial.print(' ');
    }
    Serial.println();
  } else {
      /** Set Joystick state based on SNES input **/
    SNESpad.xAxis(x);
    SNESpad.yAxis(y);
    //SNESpad.dPad1(dpad);
    SNESpad.buttons(b);
    SNESpad.write();

  }
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
