/* 
 *  UnoArcade_Mouse_16u2
 *  UNO 16u2 Hoodloader Arcade Mouse
 *  by D. Scott Williamson
 *  March 2020
 *  (c) 2020 D. Scott Williamson, All rights reserved.
 *  
 *  Receives mouse movements and buttons from ATMega328 via Serial1, sends USB HID Mouse events
 *  
 *  Setup Instructions:
 *    - HoodLoader2 Hardware Installation instructions: https://github.com/NicoHood/HoodLoader2/wiki/Hardware-Installation
 *        I followed this option: https://github.com/NicoHood/HoodLoader2/wiki/Installation-sketch-%28standalone-Arduino-Uno-Mega%29
 *    - Install Serial drivers and Board Definitions: https://github.com/NicoHood/HoodLoader2/wiki/Software-Installation
 *    - Install HID project https://github.com/NicoHood/HID/wiki/Installation
 *    
 *  Instructions for this sketch:
 *    - Select Board: HoodLoader2 16u2
 *    - Compile and upload this sketch (either follow 16u2 double reset instructions or just upload twice)
 *    - You will also need to program the UNO with UnoArcade_Mouse_Uno sketch
 *    - See keys table below for keycodes for each button.
 */

#include "HID-Project.h"

char dx, dy;              // Mouse dx and dy

int unsigned mouseState;  // State machine state

// State machine states
enum state {
  state_start,
  state_dx,
  state_dy,
  state_buttons
};

void setup() {
  // Start the Serial1 which is connected with the IO MCU.
  // Make sure both baud rates are the same
  // you can go up to 2000000 for very fast data transmission.
  Serial1.begin(115200);
  
  // Sends a clean report to the host. This is important on any Arduino type.
  Mouse.begin();
  //BootKeyboard.begin();

  // Start the USB Serial for debugging
  Serial.begin(115200);

  // set the mouse serial state machine
  mouseState=state_start;
}

// Union needed to conveniently re-cast values sent over serial
union vals {
  int i;
  char c;
  unsigned char uc;
};

void loop() {
  /* Mouse encoding:
   *  0xa5            start signal (for synchronization)
   *  signed byte     dx
   *  signed byte     dy
   *  1111 er r el l  end signal 0xf0, buttons (edge right, right, edge left, left)
   */

  vals data;
 
  // receive data from serial into int
  data.i = Serial1.read();
  
  // Check if any Serial data from the IO MCU was received 
  if (data.i>=0)
  {
    //Serial.print(data.i,DEC);             // serial output for debugging
    
    switch (mouseState)
    {
        case state_start:
          // wait for start value
          if (data.uc==0xa5) mouseState=state_dx;
          //Serial.println(); // serial output for debugging
          //Serial.print("state_start:");   // serial output for debugging
          //Serial.print(data.uc,HEX);      // serial output for debugging
          break;
          
        case state_dx:
          // record dx
          //Serial.print("state_dx:");      // serial output for debugging
          //Serial.print(data.c,DEC);       // serial output for debugging
          dx=data.c;
          mouseState=state_dy;
          break;

        case state_dy:
          // record dy
          //Serial.print("state_dy:");      // serial output for debugging
          //Serial.print(data.c,DEC);       // serial output for debugging
          dy=data.c;
          mouseState=state_buttons;
          break;
          
        case state_buttons:
          // Check to see if ending is valid
          // 1111 er r el l
          //Serial.print("state_buttons:"); // serial output for debugging
          //Serial.print(data.uc,BIN);      // serial output for debugging
          // validate correct packet
          if ((data.uc&0xf0) == 0xf0)
          {
            Mouse.move(-dx,dy); // NOTE: Negating dx
            
            // Left button
            if (data.uc&B00000100)    // check if edge
            {
              if (data.uc&B00000001)  // check state
              {
                // released
                Mouse.release(MOUSE_LEFT);
              }
              else
              {
                // pressed
                Mouse.press(MOUSE_LEFT);
              }
            }

            // Right buttom
            if (data.uc&B00001000)    // check if edge
            {
              // Edge on right button
              if (data.uc&B00000010)  // check state
              {
                // released
                Mouse.release(MOUSE_RIGHT);
              }
              else
              {
                // pressed
                Mouse.press(MOUSE_RIGHT);
              }
            }
          }
          // reset state machine
          mouseState=state_start;
          break;

        default:
          // unknown state, reset state machine
          mouseState=state_start;
          break;
    }
  }
} 
