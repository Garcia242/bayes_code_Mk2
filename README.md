code made for teensy 4.1

do not edit on main, use a different branch for changes and then merge onto main once the functionality works and is bug-free

to start EDF:
1. compile and push the code to the teensy using Arduino IDE or an alternative.
2. open the serial monitor and send the message "180" to the teensy.
3. plug-in batteries to the ESC.
4. within the first 4 double beeps, send the message "30" to the teensy.
5. once the rocket has made some Mario music, it should cease beeping.
6. you should now send a message of "37" to the teensy.
7. you can now choose a thrust value for the EDF from 37 - 180 and send this as a message to the teensy as you need.
8. the thrust value can be changed as needed by repeating step 7.
9. to turn off the EDF, send a message to the teensy of "0".


-------

To connect the breadboard to the ESC the brown wire is the ground, the red wire is live and the signal is the yellow wire.
