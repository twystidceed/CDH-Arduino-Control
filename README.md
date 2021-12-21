# CDH-Arduino-Control
Basic Auto Start/Stop for Chinese Diesel Heaters


Works with Most Atmega Arduino chipsets.

You will need to add the library included to your arduino . This can be done in seconds with the Arduino IDE - once you unzip the downloaded code file. you will find the half duplex library is included . 


This code Requires you to maintain a connection and use the factory supplied display and control. DO NOT CUT THE CONNECTOR - only splice into the wire. 

This controller only works with the same 3 wire diesel heater digital control units as the Afterburner by Ray Jones . you can see compatability list here https://gitlab.com/mrjones.id.au/bluetoothheater/-/wikis/home#compatibility

Create a splice, NO CUT - use a razor blade gently and add an extension wire. Location does not matter as long as it is between the control and the heater, into the data wire, light blue usually on compatable heaters and another into the black (ground) wire. Some heaters allow you to also splice into the red (5v) to power the arduino, but this does not work with all so a separate 5v usb power source may be needed . 

Connect the data splice to D2 on the ardiuno and the Black (GND) splice to the GND on the same side as the D2 port. 

Power the Arduino (by heater or USB) if using the red wire to the controller. please make sure that you use the 5v power pin and not the 3.3 v.. you will damage the arduino if you apply power to any other pin. 

upload the CDH.ino code to the new arduino. and restart arduino. 

An initial start command is needed from the existing controller to enable the arduino monitor. Once started the Arduion will monitor the controller data for the set temp and current temp from.  If the indicated room temp reaches +2 of your set temp, the arduino will initaite a shutdown. The Arduino will monitor the actual temperature from the controller at this point and issue a start command to the heater once the room temperature falls -1 from your set temp. 

The 2 degree on the top is to allow the heater a chance to run on low and maintain temperature prolonging the life of the glowplug a little longer. While in the temp range, the heater will adjust its speed accordingly to meet the heat demands. If you do not want this, you can change the code on line 132 to read as posted below and the heater will shut down at 1 degree above set temp. 

"if (int(settemp) < (int(currtemp))) {"

I do not suggest modifying this to be == to the current temp.. this will cause the heater to cycle too much , shortening the glowplug life. I have added a snipet of code to make sure the heater is running normally (up to temperature) to prevent cold shutdowns that contrubute to soot buildup. 

There is still a random bug that every now and then causes the arduino to ignore the data - to fix.. simply restart arduino and tap the power (start) button on the heater control. I have found this mainly happens when powering from the controller power. 

I highly suggest properly tuning your htz and rpm settings prior to using this . There are many resources available for this . 


