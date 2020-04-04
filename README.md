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

