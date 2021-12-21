# CDH-Arduino-Control
Auto Start/Stop for Chinese Diesel Heaters - See Readme

Works with Most Atmega Arduino chipsets.
this will only work with 3 wire diesel heater control setups. Will not work with heaters that have the basic control board and rotary dials. Use at own risk 

Create a splice into the data wire, light blue usually on compatable heaters and another into the black (ground) wire. Some heaters allow you to also splice into the red (5v) to power the arduino, but this does not work with all so a separate 5v usb power source may be needed . 

Connect the data splice to D2 on the ardiuno and the Black (GND) splice to the GND on the same side as the D2 port. 

Power the Arduino (by heater or USB) if using the red wire to the controller. please make sure that you use the 5v power pin and not the 3.3 v.. you will damage the arduino if you apply power to any other pin. 

upload the code to the new arduino. and restart arduino. 

An initial start command is needed from the existing controller to enable the arduino monitor. Once started the Arduion will monitor the controller data for the set temp and current temp from.  If the indicated room temp reaches +2 of your set temp, the arduino will initaite a shutdown. The Arduino will monitor the actual temperature from the controller at this point and issue a start command to the heater once the room temperature falls -1 from your set temp. 

The 2 degree on the top is to allow the heater a chance to run on low and maintain temperature prolonging the life of the glowplug a little longer. While in the temp range, the heater will adjust its speed accordingly to meet the heat demands. If you do not want this, you can change the code on line 132 to read as posted below and the heater will shut down at 1 degree above set temp. 

"if (int(settemp) < (int(currtemp))) {"

I do not suggest modifying this to be == to the current temp.. this will cause the heater to cycle too much , shortening the glowplug life. I have added a snipet of code to make sure the heater is running normally (up to temperature) to prevent cold shutdowns that contrubute to soot buildup. 

There is still a random bug that every now and then causes the arduino to ignore the data - to fix.. simply restart arduino and tap the power (start) button on the heater control. I have found this mainly happens when powering from the controller power. 


