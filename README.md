# :space_invader: :trackball: :space_invader:	:keyboard: :space_invader: :joystick: :space_invader: :computer_mouse: :space_invader:
# UnoArcade
#### by D. Scott Williamson

UnoArcade contains two projects in four arduino sketches that can turn an Arduino UNO into a USB mouse for trackball control or a 16 key USB keyboard configured for single player MAME arcade game control.

### 16 key USB keyboard controller (configured for MAME)
* UnoArcade_Keyboard_16u2/UnoArcade_Keyboard_16u2.ino
* UnoArcade_Keyboard_Uno/UnoArcade_Keyboard_Uno.ino

### USB Mouse for trackball control
* UnoArcade_Mouse_16u2/UnoArcade_Mouse_16u2.ino
* UnoArcade_Mouse_Uno/UnoArcade_Mouse_Uno.ino

See source files for pin usage and specific instructions.

:space_invader: :trackball: :space_invader:	:keyboard: :space_invader: :joystick: :space_invader: :computer_mouse: :space_invader:   :trackball: :space_invader:	:keyboard: :space_invader: :joystick: :space_invader: :computer_mouse: :space_invader:

-----

The projects rely on [HoodLoader2](https://github.com/NicoHood/HoodLoader2) to give the Arduino UNO the ability to be a USB HID device.

### Arduino UNO HoodLoader2 setup Instructions:
* [HoodLoader2 Hardware Installation instructions](https://github.com/NicoHood/HoodLoader2/wiki/Hardware-Installation)
 * I followed [this option](https://github.com/NicoHood/HoodLoader2/wiki/Installation-sketch-%28standalone-Arduino-Uno-Mega%29) to self program the Arduino UNO
* [Install Serial drivers and Board Definitions](https://github.com/NicoHood/HoodLoader2/wiki/Software-Installation)
* [Install HID project](https://github.com/NicoHood/HID/wiki/Installation)

To learn more about USB HID (Human Interface Device) descriptors, check out [*ELECCELERATOR*'s excellent tutorial page]( https://eleccelerator.com/tutorial-about-usb-hid-report-descriptors/)

-----

![My arcade cabinets](https://github.com/Workshop88/UnoArcade/blob/master/images/IMG_0115.JPG) The finished console working!

| Parts & Assembly | |
:-:|:-:
|![My arcade cabinets](https://github.com/Workshop88/UnoArcade/blob/master/images/IMG_0039.JPG) Decker helping install buttons | ![My arcade cabinets](https://github.com/Workshop88/UnoArcade/blob/master/images/IMG_0040.JPG) Switches|
|![My arcade cabinets](https://github.com/Workshop88/UnoArcade/blob/master/images/IMG_0041.JPG) Joysticks | ![My arcade cabinets](https://github.com/Workshop88/UnoArcade/blob/master/images/IMG_0042.JPG) Trackball (with spare ball) |
|![My arcade cabinets](https://github.com/Workshop88/UnoArcade/blob/master/images/IMG_0038.JPG) Testing the Happ trackball wiring on the scope | ![My arcade cabinets](https://github.com/Workshop88/UnoArcade/blob/master/images/IMG_0089.JPG) Two Arduino UNO's installed and wired, one for the trackball, and one for the keyboard control.|


![My arcade cabinets](https://github.com/Workshop88/UnoArcade/blob/master/images/IMG_1940.JPG) My coin op cabinets (left to right: custom cocktail cabinet, Happy Fun Box MK3 rebuild, working 1/3 scale replica of Happy Fun Box

![My arcade cabinets](https://github.com/Workshop88/UnoArcade/blob/master/images/IMG_0044.JPG) Happy Fun Box, my first and favorite


