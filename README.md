# UVROverlay
[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=6D4FGY2RBLSFS)  

This is a ground up implementation VR overlay manager built on openVR.  Currently in development for release and developed by myself.

## Current release
You can download the current release [here](https://github.com/scudzey/UVROverlay/releases)

## Features
* Add as many overlays as want!
* Tracking to controllers, HMD and VR space
* Controller Controls
   * Touch Pad is segmented into four quadrants
   * Top Left and Right control overlay scaling
   * Bottom Left and Right control overlay opacity
   * Trigger will move the overlay
   * Grip Button will toggle tracking mode for the overlay
   * To use, place the controller into the middle of the overlay and press the corresponding button
* Adjustable update rates
   *  Having multiple overlays running at 60fps can add overhead to any VR game/application also running, lowering this value will diminish the impact
* Ground up optimized implementation
  * Low CPU utilization to keep your games running at 90fps


## Coming Soon
* Hotkey toggling
* State saving
* VR menu system
* Gamepad Support

## Known Issues
* This won't work for all windows, only those with DWM drawing
  * Solution for Chrome: disable hardware acceleration
* * Using the controller to move the overlays results in shaky angular movements.  This will be addressed in future release.
* Minimized applications will not draw
* Changing an overlay to spacial tracking does not perfectly replicate positioning
