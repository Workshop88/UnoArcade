/* 
 *  UnoArcade_Keyboard_16u2
 *  UNO 16u2 HoodLoader2 HID Arcade Keyboard
 *  by D. Scott Williamson
 *  March 2020
 *  (c) 2020 D. Scott Williamson, All rights reserved.
 *
 *  Receives and decodes custom keycodes from ATMega328 via Serial1, sends USB HID keyboard events
 *  
 *  Instructions:
 *  
 *  Requires Arduino UNO flashed with HoodLoader2 firmware
 *  https://github.com/NicoHood/HoodLoader2
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
 *    - You will also need to program the UNO with UnoArcade_Keyboard_Uno sketch
 *    - See keys array below for keycodes for each button.
 *      To customize keycodes use the table in ImprovedKeylayoutsUS.h
 *      https://github.com/NicoHood/HID/blob/master/src/KeyboardLayouts/ImprovedKeylayoutsUS.h
 */

#include "HID-Project.h"

// Uncomment to use key mappings for Happy Fun Box
//#define HAPPY_FUN_BOX

#ifdef HAPPY_FUN_BOX
const char unsigned keys[16] = {
  KEY_5,KEY_1,KEY_2,KEY_F3, KEY_P, KEY_ESC, 
  KEY_UP_ARROW,KEY_DOWN_ARROW,KEY_LEFT_ARROW,KEY_RIGHT_ARROW,
  KEY_ENTER, KEY_QUOTE, KEY_PAGE_UP, KEY_SEMICOLON, KEY_Z, KEY_PAGE_DOWN
};  
#else
// Default MAME key mappings
// http://members.optusnet.com.au/spacetaxi64/MAME-Arcade/MAME-Keys.html
const char unsigned keys[16] = {
  KEY_5,            // Pin d7 Quarter    
  KEY_1,            // Pin d6 1Player Start
  KEY_2,            // Pin d5 2Player Start   
  KEY_F3,           // Pin d4 Reset      
  KEY_P,            // Pin d3 Pause      
  KEY_ESC,          // Pin d2 Escape     
  KEY_UP_ARROW,     // Pin b0 Joy_up     
  KEY_DOWN_ARROW,   // Pin b1 Joy_down   
  KEY_LEFT_ARROW,   // Pin b2 Joy_left   
  KEY_RIGHT_ARROW,  // Pin b3 Joy_right  
  KEY_LEFT_CTRL,    // Pin c0 Button1    
  KEY_LEFT_ALT,     // Pin c1 Button2    
  KEY_SPACE,        // Pin c2 Button3    
  KEY_LEFT_SHIFT,   // Pin c3 Button4    
  KEY_Z,            // Pin c4 Button5    
  KEY_X             // Pin c5 Button6  
};  
#endif

// Arduino inter-processor key codes
enum keyindex 
{
  Quarter,    // Pin d7 Quarter    
  Start1P,    // Pin d6 1Player Start
  Start2P,    // Pin d5 2Player Start   
  Reset,      // Pin d4 Reset      
  Pause,      // Pin d3 Pause      
  Escape,     // Pin d2 Escape     
  Joy_up,     // Pin b0 Joy_up     
  Joy_down,   // Pin b1 Joy_down   
  Joy_left,   // Pin b2 Joy_left   
  Joy_right,  // Pin b3 Joy_right  
  Button1,    // Pin c0 Button1    
  Button2,    // Pin c1 Button2    
  Button3,    // Pin c2 Button3    
  Button4,    // Pin c3 Button4    
  Button5,    // Pin c4 Button5    
  Button6,    // Pin c5 Button6 
};

void setup() {
  // Start the Serial1 which is connected with the IO MCU.
  // Make sure both baud rates are the same
  // you can go up to 2000000 for very fast data transmission.
  Serial1.begin(115200);
  
  // Sends a clean report to the host. This is important on any Arduino type.
  Keyboard.begin();
  //BootKeyboard.begin();

  // Start the USB Serial for debugging
  Serial.begin(115200);
}

void loop() {
  // receive data from ATmega328 via Serial1
  int r = Serial1.read();

  // Check if any Serial data from the IO MCU was received 
  if (r>=0)
  {
    // extract byte transfered
    unsigned char c= (unsigned char) r;

    // isolate keycode
    unsigned char keyindex =  c & 0x0f;
    // isolate release
    unsigned char rel  =  c & 0x80;

    uint8_t key = keys[keyindex];
   
    Serial.print(keyindex, HEX);   // serial output for debugging
    Serial.print(" ");             // serial output for debugging
    Serial.print(key,HEX);         // serial output for debugging
    Serial.print(" ");             // serial output for debugging
 
    // press or release key
    if (rel)
    {
      // button was just released
      Keyboard.release((KeyboardKeycode) key);
      Serial.print(" released");   // serial output for debugging
    }
    else
    {
      // button was just pressed
      Keyboard.press((KeyboardKeycode) key);
      Serial.print(" pressed");    // serial output for debugging
    }
    
    Serial.println();              // serial output for debugging
    
    // Simple debounce
    //delay(300);
  }
}
