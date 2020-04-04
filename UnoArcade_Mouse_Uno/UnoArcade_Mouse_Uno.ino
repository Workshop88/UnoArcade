/* 
 *  UnoArcade_Mouse_Uno
 *  UNO Hoodloader Arcade Mouse
 *  by D. Scott Williamson
 *  March 2020
 *  (c) 2020 D. Scott Williamson, All rights reserved.
 *  
 *  Decodes quadrature movement and sends mouse delta and buttons
 *  to 16u2 over serial on regular interrupt
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
 *    - You will also need to program the 16u2 with UnoArcade_Mouse_16u2 sketch
 *    - Each button is a normally open switch wired from the corresponding Arduino pin to Ground
 *      See keyIndex below for button to Arduino pin mapping
 */

int dx,dy;                // Mouse x and y accumulated deltas
int ddx,ddy;              // Mouse x and y previous delta's (to be used if skipping occurs)
char unsigned buttonbits; // Mouse button bits
int sendmouse;            // Flag indicating whether mouse data should be sent over USB

enum pins
{
  ya,    // Pin c0
  yb,    // Pin c1
  xa,    // Pin c2
  xb,    // Pin c3
  left,  // Pin c4
  right, // Pin c5
};

// Assuming 2 bit encoder sequence (positive to the right)
// 00 01 11 10 00 01 11 10 00 01 11 10 00 01 11 10 
const int delta[]={  0,  // 0000 0
                     1,  // 0001 +1
                    -1,  // 0010 -1
                     2,  // 0011 +-2 (both bits changed, skipped a step, apply last known ddx twice)
                    -1,  // 0100 -1
                     0,  // 0101 0
                     2,  // 0110 +2  (both bits changed, skipped a step, apply last known ddx twice)
                     1,  // 0111 +1
                     1,  // 1000 +1
                     2,  // 1001 +-2 (both bits changed, skipped a step, apply last known ddx twice)
                     0,  // 1010 0
                    -1,  // 1011 -1
                     2,  // 1100 +-2 (both bits changed, skipped a step, apply last known ddx twice)
                    -1,  // 1101 -1
                     1,  // 1110 +1
                     0}; // 1111 0

// Timer interrupt service routine
ISR(TIMER1_COMPA_vect){
  // if there is any mouse movement, flag to send it over USB
  if (dx|dy) sendmouse=1;
}

void setup() {
   dx=ddx=dy=ddy=0;
   sendmouse=0;
   buttonbits=0xf0;

  // pullups are 1
  PORTC = B00111111;
                      // D7 is disable
  PORTD = B10000000;

  // Data Direction 0 is input 1 is output
  DDRC = B00000000;
  DDRD = B00000000;

  // safety
  // Jumper PIND to ground at boot to prevent mouse moves to reprogram
  while ((PIND&0x07)==0);
  
  // Start the Serial which is connected with the USB MCU.
  // Make sure both baud rates are the same
  // you can go up to 2000000 for very fast data transmission.  
  Serial.begin(115200);

  // Initialize interrupt 
  cli();//stop interrupts

  // Mouse interrupt to send N times a second
  // 40hz example:
  //   16000000hz /1024 prescaler = 15625hz
  //   40hz update => reload at 390 so timer 1 is needed
  //set timer1 interrupt 
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  
  // set compare match register for 1hz increments
  //OCR1A = 390;  // 390.6 = ((16*10^6) / (1*1024) - 1)/40 (must be <65536)
  //OCR1A = 195;  // 195.3.6 = ((16*10^6) / (1*1024) - 1)/80 (must be <65536)
  OCR1A = 156;  // 156.24 = ((16*10^6) / (1*1024) - 1)/100 (must be <65536)
  
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  
  sei();//allow interrupts
}

unsigned char lastc; // previous port bits

void loop() {
  unsigned char c, edge;
  int d;

  c=PINC;
  // check for edge on all pins (fast exit)
  if(edge=(c^lastc)& B00111111)
  {
    // x encoder bits are lowest two
    // create nibble index that is llcc where ll is last and cc is current
    if (edge&0x03)
    {
      d=delta[((lastc& 0x03)<<2)|(c&0x03)];
      if (d==2)
      {
        // skipped a quadrature step 
        // adding the last known delta twice
        dx+=ddx<<1;  
      } else {
        // normal stepping
        // save delta
        ddx=d;     
        // apply delta
        dx+=ddx;
      }
    }
    
    // y encoder bits are next two
    if (edge & (0x03<<2))
    {
      // create nibble index that is llcc where ll is last and cc is current
      d=delta[((lastc & (0x03<<2))|((c>>2)&0x03))];
      if (d==2)
      {
        // skipped a quadrature step 
        // adding the last known delta twice
        dy+=ddy<<1;
      } else {
        // normal stepping
        // save delta
        ddy=d;
        // apply delta        
        dy+=ddy;
      }
    }   

    // Check button bits for changes
    if (edge & 0x30)
    {
      // encode button bits
      // 1111 er r el l   end signal 0xf0, buttons (edge right, right, edge left, left)
      buttonbits=0xf0 | ((edge& 0x30)>>2) | ((c&0x30)>>4);
      // A button was pressed or released, send immediately
      sendmouse=1;
    }
    
  // save previous port c bit values for next loop
  lastc=c;
  }
      
  // send mouse updates when interrupt says we should
  if (sendmouse)
  { 
    /* Mouse encoding:
     *  0xa5            start signal (for synchronization)
     *  signed byte     dx
     *  signed byte     dy
     *  1111 er r el l  end signal 0xf0, buttons (edge right, right, edge left, left)
     */
    // start signal
    Serial.write(0xa5); 

    // dx
    if(dx>=0)
    {
      // delta needs to fit in signed char
      if (dx<=127)
      {
        Serial.write(dx); 
        dx=0;
      }
      else
      {
        // overflow just send what will fit in signed char
        Serial.write(127); 
        dx-=127;
      }
    }
    else
    {
      // delta needs to fit in signed char
      if (dx>=-128)
      {
        Serial.write(dx); 
        dx=0;
      }
      else
      {
        // overflow just send what will fit in signed char
        Serial.write(-128); 
        dx-=-128;
      }
    }
    
    //dy
    if(dy>=0)
    {
      // delta needs to fit in signed char
      if (dy<=127)
      {
        Serial.write(dy); 
        dy=0;
      }
      else
      {
        // overflow just send what will fit in signed char
        Serial.write(127); 
        dy-=127;
      }
    }
    else
    {
      // delta needs to fit in signed char
      if (dy>=-128)
      {
        Serial.write(dy); 
        dy=0;
      }
      else
      {
        // overflow just send what will fit in signed char
        Serial.write(-128); 
        dy-=-128;
      }
    }

    // buttons
    Serial.write(buttonbits);

    // debounce mouse buttons if any key was pressed
    // bounce looked under 5ms on scope
    if (buttonbits!=0xf0)
    {
      delay(8); // 16ms is 1/60 of a second
    }
    
    // set buttons to stop signal with no edges for position updates
    buttonbits=0xf0;  

    // if dx and dy are 0 we can stop sending delta's
    if((dx&dy)==0)
    {
      sendmouse=0;
    }
  }
}
