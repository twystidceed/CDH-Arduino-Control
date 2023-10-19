# CDH-Arduino-Control
Basic Auto Start/Stop for Chinese Diesel Heaters - based on orignal design by @jess with contributions from @wshelley and other humans. 


Works with Most Atmega Arduino chipsets.

You will need to add the library included in this repo to your arduino . This can be done in seconds with the Arduino IDE - once you unzip the downloaded code file. you will find the half duplex library is included . Be sure to search for and install "OneWire" library as well. 

You must maintain the connection with the factory supplied controller. DO NOT CUT THE CONNECTOR or the wire. 

This sketch only works with the same 3 wire diesel heater digital control units as the Afterburner by Ray Jones . you can see compatability list here https://gitlab.com/mrjones.id.au/bluetoothheater/-/wikis/home#compatibility

Create a splice, NO CUT - use a razor blade gently and add an extension wire. Location does not matter as long as it is between the control and the heater, into the data wire, light blue usually on compatable heaters and another into the black (ground) wire. Some heaters allow you to also splice into the red (5v) to power the arduino, but this does not work with all so a separate 5v usb power source may be needed and recomended as in testing, the red wire is not a consistant 5v and causes hangups and restarts of the arduinos. 

Connect the data splice to D2 on the ardiuno and the Black (GND) splice to the GND on the same side as the D2 port. (advanced users can modify the pin used to fit thier needs)

upload the CDH.ino code to the new arduino. and restart arduino. wait about 30 seconds then press the power button on the controller to activate the arduino. Pressing power again will turn off the auto start\stop.. You may need to uncomment a line in the sketch and upload again to monitor the serial data and see if the arduino is in an "auto" or "off" state or simply set the temp a few degrees above the ambient temp when the heater is off or vise versa when on to see if the arduino takes action.

The arduino will start the heater when the temperature you have selected is 1 degree colder than the set temp and will shut off the heater when the ambient temp is 2 degrees warmer than your set temp. this 2 degree buffer allows the heater to throttle down and attempt to maintain the temp, prolonging the life of the glow plug. Keep this im mind when setting your desired temp. The controller temp is usually inaccurate as well in testing... many heaters read a higher ambient than actual. Users can modify this buffer to fit their needs as well. 

I highly suggest properly tuning your hertz and rpm settings prior to using this "hack" . There are many resources available for this online. Improperly tuned heaters will build up carbon in the burn chambers eventually rendering the heater useless. This buildup can be rapid during heater low speed throttling with a tuning that is too rich "too much fuel, not enough rpm on the fan" 


2023 update:

The cold weather is back and so I am back at it! i have deciphered 99% of the bits contained in the data stream.. 

For those who dont know, most of these cheap chinese diesel heaters use 3 wires to the controller with a single wire for bi-directional communications (blue wire) between the controller and the heater motherboard.. while you can extract a total of 48 bits of data, the first 24 are the programming and control bits used to send commands to the heater and adjust the heater settings..  25 to 47 are not of any actual use to the average user. With the exception of Bit 29(Voltage) and bit 41 (error code), they are simply values being gathered from the multiple sensors that the motherboard uses to control and monitor the heater performance. others are simply passive bits just being monitored by the controller to change the on screen icons such as the indicators for pump, glow plug and temp but nothing more.. utilising an oled screen i have been able to actually monitor in real time,  case temps, fan rpm, glow plug voltage, room temp, heater status and error codes, etc using these bits.. so for advanced users... 

my next sketch push (hopefully in the next week or so ) will have a large portion at the end commented out. anyone can un-comment the serial.print lines to see ALL of the data in the serial monitor in real time. Great for troubleshooting a shoddy running heater or one that keeps posting error codes.. 

