# DMXReceiver
DMX receiver to control Smart LEDS and relays using Arduino

My firts repository. As stated in my profile, background in electronics so more comfortable with hardware than software..

This project uses an Arduino Nano as a DMX receiver using a 485 module interface.
Outputs to a 281x strip of LEDS. Because the strips installed span three sections of wall, the strip is divide in three sections via software

The module also support 3 12V relays which are used to turn on/off AC devices (lasers, disco ball)

This is phase one which is functional and deployed. (doing the project here after its finished..)
Phase 2 is to upgrade to ESP32 with addtional support for fixed RGB ligths (need more PWM..) and OTA.  

Using the fastled and DMX library
