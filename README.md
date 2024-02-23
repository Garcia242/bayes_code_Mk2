code made for teensy 4.1

to start EDF:
1. compile and push the code to the teensy using Arduino IDE or alternative.
2. open serial monitor and send the message "180" to the teensy.
3. plug in batteries to the ESC.
4. within the first 4 double beeps, send the message "30" to the teensy.
5. once the rocket has made some mario music, it should cease beeping.
6. you should now send a massage of "37" to the teensy.
7. you can now choose a thrust value for the EDF from 37 - 180 and send this as a message to the teensy as you need.
8. the thrust value can be changed as needed by repeating step 7.
9. to turn of the EDF, send a message to the teensy of "0".
