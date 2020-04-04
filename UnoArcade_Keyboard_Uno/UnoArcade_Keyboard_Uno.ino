/* 
 *  UnoArcade_Keyboard_Uno
 *  UNO HoodLoader2 HID Arcade Keyboard
 *  by D. Scott Williamson
 *  March 2020
 *  (c) 2020 D. Scott Williamson, All rights reserved.
 *  
 *  Sends custom key codes to 16u2 via serial on button changes with debouncing
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
 *    - Select Board: HoodLoader2 Uno
 *    - Compile and upload this sketch (either follow 16u2 double reset instructions or just upload twice)
 *    - You will also need to program the 16u2 with UnoArcade_Keyboard_16u2 sketch that contains your key codes
 *    - Each button is a normally open switch wired from Arduino pin to Ground
 *      See keyIndex below for button to Arduino pin mapping
 */

unsigned char lastb, lastc, lastd;  // previous port bits

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

  // Set port bits to enable pullups
  PORTB = B00111111;
  PORTC = B00111111;
  PORTD = B11111111;

  // Data Direction 0 is input 1 is output
  DDRB = B00000000; 
  DDRC = B00000000;
  DDRD = B00000000;
  
  // Start the Serial which is connected with the USB MCU 16u2.
  // Make sure both baud rates are the same
  // you can go up to 2000000 for very fast data transmission.  
  Serial.begin(115200);
}

void loop() {
  int dodebounce=0;
  unsigned char b,c,d, edge, temp,keyindex;

  // read all port pins
  b=PINB;
  c=PINC;
  d=PIND;

  // Port b joystick
  if(edge=(b^lastb)&B00001111)
  {
    // something was pressed or released, indicate need for debounce
    dodebounce=1; 
    
    // Send serial messages for all changed bits
    temp=b;
    keyindex=Joy_up;
    while(edge)
    {
      if (edge& 1)
      {
        // send keyindex with the current key state in the high bit (0 pressed)
        Serial.write(keyindex | ((temp&1)<<7));
      }
      edge>>=1;
      temp>>=1;
      ++keyindex;
    }
  }

  // Port c player buttons
  if(edge=(c^lastc)&B00111111)
  {
    // something was pressed or released, indicate need for debounce
    dodebounce=1; 
    
    // Send serial messages for all changed bits
    temp=c;
    keyindex=Button1;
    while(edge)
    {
      if (edge& 1)
      {
        // send keyindex with the current key state in the high bit (0 pressed)
        Serial.write(keyindex | ((temp&1)<<7));
      }
      edge>>=1;
      temp>>=1;
      ++keyindex;
    }
  }

  // Port d top row of buttons
  if(edge=(d^lastd)&B11111100)
  {
    // something was pressed or released, indicate need for debounce
    dodebounce=1; 

    // Send serial messages for all changed bits
    temp=d;
    keyindex=Quarter;
    while(edge)
    {
      if (edge& 0x80)
      {
        // send keyindex with the current key state in the high bit (0 pressed)
         Serial.write(keyindex | (temp&0x80));
      }
      edge<<=1;
      temp<<=1;
      ++keyindex;
    }
  }

  // save the current port bit state for next loop
  lastb=b;
  lastc=c;
  lastd=d;

  // debounce if any key was pressed
  // bounce looked under 5ms on scope
  if (dodebounce)
  {
    delay(8); // 16ms is 1/60 of a second
  }
}
