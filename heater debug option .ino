#include <SoftwareSerialWithHalfDuplex.h>

SoftwareSerialWithHalfDuplex sOne(2, 2); // NOTE TX & RX are set to same pin for half duplex operation

int optionalEnableDisableSwitch = 8;
unsigned long flasherLastTime;
unsigned long rxLastTime;
unsigned long writerLastTime;
String heaterState[] = {"Off", "Starting", "Pre-Heat", "Failed Start - Retrying", "Ignition - Now heating up", "Running Normally", "Stop heaterCommand Received", "Stopping", "Cooldown"};
int heaterStateNum = 0;
//int set_voltage = 0; //12 or 24  - this does nothing as the heater is either 12v or 24 volt factory.. unable to change 
//int fan_sensor = 0; // this doesnt appear to ever change from a value of 1 
//int mode = 0; // Seems to always read 50 ???
int heaterCommand = 0;
//int min_temp = 0;
//int max_temp = 0;
//int pump_mode = 0;
//int crc_msb_a = 0;
//int crc_lsb_a = 0;
//below is rx 
//int error_state = 0;
int error_code = 0;
//int crc_msb_rx = 0;
//int crc_lsb_rx  = 0;

// Default enable thermostat mode at start up
int controlEnable = 1;

//below are added to allow advanced users a chance to tinker,, i have commented many out that seem to have no use 
float currentTemperature = 0;
float setTemperature = 0;
//float glowplugv = 0;
//float min_pump_freq = 0;
//float max_pump_freq = 0;
//float min_fan_speed_msb = 0;
//float min_fan_speed_lsb = 0;
//float max_fan_speed_msb = 0;
//float max_fan_speed_lsb = 0;
//float glow_plug_power = 0;
//float unknown_18 = 0;
//float unknown_19 = 0;
//float elevation_msb = 0; // always 0
//float elevation_lsb = 0; 
//below is rx

//float supply_voltage_msb = 0; //always 0
//float supply_voltage_lsb = 0;
//float fan_speed_msb = 0;
//float fan_speed_lsb = 0;
//float fan_voltage_msb = 0;
//float fan_voltage_lsb = 0;
//float body_temp_msb = 0;
//float body_temp_lsb = 0;
//float glow_plug_voltage_msb = 0;  //always 0
//float glow_plug_voltage_lsb = 0;
//float glow_plug_current_msb = 0;
//float glow_plug_current_lsb = 0;
//float pump_frequency = 0;
//float unknown_42 = 0;  // always 0 at time of testing 
//float requested_hz = 0;
//float unknown_44 = 0;
//float unknown_45 = 0;



void setup()
{
  pinMode(optionalEnableDisableSwitch, INPUT_PULLUP);
  // initialize listening serial port
  // 25000 baud, Tx and Rx channels of Chinese heater comms interface:
  // Tx/Rx data to/from heater, special baud rate for Chinese heater controllers
  sOne.begin(25000);
  // initialise serial monitor on serial port 0 - use 115200
  // lower baud rates for debugging can slow down the diesel heater tx/rx
  Serial.begin(115200);
  // prepare for detecting a long delay
  flasherLastTime = millis();
  rxLastTime = millis();
  writerLastTime = millis();
    // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);      
}

void loop()
{ 
  static byte Data[48];
  static bool RxActive = false;
  static int count = 0;
  // Default heartbeat LED flash length milliseconds
  static long flashLength = 800;

  unsigned long flasherTimeNow = millis();
  unsigned long flasherDiff = flasherTimeNow - flasherLastTime;

  // LED Heartbeat
  // 1) standard heart beat when nothing is active
  // 2) fast heart beat when heater has been triggered to turn on
  // 3) slow heart beat when heater has been triggered to turn off
  // These can be used for testing if you do not want to transmit changes to the heater.
  if (controlEnable == 0) {
    flashLength = 800;
  }
  if (flasherDiff > flashLength) {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  if (flasherDiff > flashLength * 2) {
    digitalWrite(LED_BUILTIN, LOW);
    //Serial.println("Heartbeat");
    flasherLastTime = flasherTimeNow;
  }

  // read from serial on D2
  if (sOne.available()) {
    // calc elapsed time since last rx’d byte to detect start of frame sequence
    unsigned long rxTimeNow = millis();
    unsigned long rxDiff = rxTimeNow - rxLastTime;
    rxLastTime = rxTimeNow;

    if (rxDiff > 100) { // this indicates the start of a new frame sequence
      RxActive = true;
    }
    int inByte = sOne.read(); // read hex byte
    if (RxActive) {
      Data[count++] = inByte;
      if (count == 48) {
        RxActive = false;
      }
    }
  }

  if (count == 48) { // filled both frames – dump
    unsigned long writerTimeNow = millis();
    unsigned long writerDiff = writerTimeNow - writerLastTime;

    count = 0;
    //TX bits 
    heaterCommand = int(Data[2]);
    currentTemperature = Data[3];
    setTemperature = Data[4];
    //min_pump_freq = int(Data[5]);
    //max_pump_freq = int(Data[6]);
    //min_fan_speed_msb = int(Data[7]);
    //min_fan_speed_lsb = int(Data[8]);
    //max_fan_speed_msb = int(Data[9]);
    //max_fan_speed_lsb = int(Data[10]);
    //set_voltage = int(Data[11]);
    //fan_sensor = int(Data[12]);
    //mode = int(Data[13]);
	//min_temp = int(Data[14]);
	//max_temp = int(Data[15]);
	//glow_plug_power = int(Data[16]);
	//pump_mode  = int(Data[17]);
	//unknown_18 = int(Data[18]);
	//unknown_19 = int(Data[19]);
	//elevation_msb = int(Data[20]);  //always 0
  	//elevation_lsb = int(Data[21]);
  	//crc_msb_a = int(Data[22]);
  	//crc_lsb_a = int(Data[23]);
    //RX bits 
    heaterStateNum = int(Data[26]);
	//error_state = int(Data[27]);
	//supply_voltage_msb = int(Data[28]);  //always 0
	//supply_voltage_lsb = int(Data[29]);  // actual supply voltage 121 = 12.1 
	//fan_speed_msb = int(Data[30]);
	//fan_speed_lsb = int(Data[31]);
	//fan_voltage_msb = int(Data[32]);
	//fan_voltage_lsb = int(Data[33]);
	//body_temp_msb = int(Data[34]);
	//body_temp_lsb = int(Data[35]);
	//glow_plug_voltage_msb = int(Data[36]); //always 0
	//glow_plug_voltage_lsb = int(Data[37]);
	//glow_plug_current_msb = int(Data[38]);
	//glow_plug_current_lsb = int(Data[39]);
	//pump_frequency = int(Data[40]);
	error_code = int(Data[41]);
	//unknown_42 = int(Data[42]); // always 0 at time of testing 
	//requested_hz = int(Data[43]);
	//unknown_44 = int(Data[44]);
	//unknown_45 = int(Data[45]);
	//crc_msb_rx = int(Data[46]);
	//crc_lsb_rx = int(Data[47]);
	
	
	//basic heater debug info
    Serial.println();
    Serial.println("--- Heater Status ---");
    Serial.print("Command : ");
    Serial.println(int(heaterCommand));
    Serial.print("Heater Status : ");
    if (heaterStateNum >= 0 && heaterStateNum <= 8)
    {
     Serial.println(heaterState[heaterStateNum]);
    }
    Serial.print("Error Code :      ");
    Serial.println(error_code);
    Serial.print("Current Temp :    ");
    Serial.println(int(currentTemperature));
    Serial.print("Set Temp :        ");
    Serial.println(int(setTemperature));
    Serial.print("Heater State #:   ");
    Serial.println(heaterStateNum);
    Serial.print("System Enabled :  ");
    Serial.println(controlEnable);

	//below is bit by bit heater debug information for advanced users and devs 
    // TX to heater from controller 
    //Serial.print("Command :           ");Serial.println(int(heaterCommand));   // command to turn on or off the heater - 0 - off, 1 - on, 2 - prime
    //Serial.print("Current Temp :      ");Serial.println(currentTemperature);   // the current room temperature read by the controller thermistor celcius
    //Serial.print("Set Temp :          ");Serial.println(setTemperature);       // the temperature the user sets celcius
    //Serial.print("min_pump_freq:      ");Serial.println(min_pump_freq);		 //	Pump frequency min in hertz	.1
    //Serial.print("max_pump_freq:      ");Serial.println(max_pump_freq);	     // Pump frequency max in hertz .1
    //Serial.print("min_fan_speed_msb:  ");Serial.println(min_fan_speed_msb);    // Fan min speed (MSB) 
    //Serial.print("min_fan_speed_lsb:  ");Serial.println(min_fan_speed_lsb);    // Fan min speed (LSB)
    //Serial.print("max_fan_speed_msb:  ");Serial.println(max_fan_speed_msb);    // Fan max speed (MSB)
    //Serial.print("max_fan_speed_lsb:  ");Serial.println(max_fan_speed_lsb);    // Fan max speed in RPM (LSB)
    //Serial.print("set_voltage:        ");Serial.println(set_voltage/10, 1);    // Controller Voltage selection 12/24VDC
    //Serial.print("fan_sensor:         ");Serial.println(fan_sensor);		     // Fan Sensor ? The value is always constant - unsure if this is just to say fan is connected 
    //Serial.print("Mode:               ");Serial.println(mode);				 // could be display in temp or Hertz mode  - 205=Hz 50=Temp - 0 = fixed Hz, 1 = temp mode
  	//Serial.print("min_temp:           ");Serial.println(min_temp);			 // Is this just a value becauses ????
  	//Serial.print("max_temp:           ");Serial.println(max_temp);			 // Possible setting for max temp of case - safety value ???
  	//Serial.print("glow_plug_power:    ");Serial.println(glow_plug_power);      // Unsure of usage.. command to start heater and stop initiates the glow plug 
  	//Serial.print("pump_mode:          ");Serial.println(pump_mode);			 // Prime=90 Nornmal=0
  	//Serial.print("unknown_18:         ");Serial.println(unknown_18);		     // Unknown usage at this time 
	//Serial.print("unknown_19:         ");Serial.println(unknown_19);	         // Unknown usage at this time
	//Serial.print("elevation_msb:      ");Serial.println(elevation_msb);	     // The baro sensor in the controller  - Value always 0
  	//Serial.print("elevation_lsb:      ");Serial.println(elevation_lsb);		 // The baro sensor in the controller - measured in meters - used for alpine mode if heater equipped
  	//Serial.print("crc_msb_a:          ");Serial.println(crc_msb_a); 		     // CRC "cyclic redundancy check"
  	//Serial.print("crc lsb:            ");Serial.println(crc_lsb_a);			 // CRC "cyclic redundancy check"
	
	// RX from heater to controller
	//Serial.print("heaterStateNum:         ");Serial.println(heaterStateNum);          // current state of heater 1-9 (see heater state at top of code for value descriptions)
    //Serial.print("error_state:            ");Serial.println(error_state);        	  // have only seen values of 0 and 1  - maybe just no error 0 - error active 1 - test to see if this is what locks the automatic control as on press is req to clear
    //Serial.print("supply_voltage_msb:     ");Serial.println(supply_voltage_msb); 	  // Always 0
    //Serial.print("supply_voltage_lsb:     ");Serial.println(supply_voltage_lsb); 	  // Actual Supply Voltage  - must divide by ten and add decimal for VDC display eg: 121=12.1V	 
    //Serial.print("fan_speed_msb:          ");Serial.println(fan_speed_msb);	     	  //
    //Serial.print("fan_speed_lsb:          ");Serial.println(fan_speed_lsb); 		  //
    //Serial.print("fan_voltage_msb:        ");Serial.println(fan_voltage_msb); 		  //
    //Serial.print("fan_voltage_lsb:        ");Serial.println(fan_voltage_lsb);  		  //
    //Serial.print("body_temp_msb:          ");Serial.println(body_temp_msb);  		  //
    //Serial.print("body_temp_lsb:          ");Serial.println(body_temp_lsb); 		  //
    //Serial.print("glow_plug_voltage_msb:  ");Serial.println(glow_plug_voltage_msb);	  // Always 0
    //Serial.print("glow_plug_voltage_lsb:  ");Serial.println(glow_plug_voltage_lsb);	  //			 
  	//Serial.print("glow_plug_current_msb:  ");Serial.println(min_temp);				  //
  	//Serial.print("glow_plug_current_lsb:  ");Serial.println(glow_plug_current_lsb);	  //		  
  	//Serial.print("pump_frequency:         ");Serial.println(pump_frequency);		  //
  	//Serial.print("error_code:             ");Serial.println(error_code);		      // Actual Error code number 1-10
	//Serial.print("unknown_42              ");Serial.println(unknown_42);  			  // Unknown value always 0 at time of testing 
	//Serial.print("requested_hz:           ");Serial.println(requested_hz);	     	  //
	//Serial.print("unknown_44:             ");Serial.println(unknown_44);	 	      // Unknown value 
  	//Serial.print("unknown_45:             ");Serial.println(unknown_45);		      // Unknown value 
  	//Serial.print("crc_msb_a:              ");Serial.println(crc_msb_a); 		      // CRC "cyclic redundancy check"
  	//Serial.print("crc lsb:                ");Serial.println(crc_lsb_a);			      // CRC "cyclic redundancy check"
	
    //Serial.println(); Serial.println();
	
	
	// On button pressed on unit or remote.
    if (int(heaterCommand) == 160) {
      Serial.println("Start command seen from controller - Enabling Auto");
      controlEnable = 1;
    }
    // Off button pressed on unit or remote.
    if (int(heaterCommand) == 5) {
      Serial.println("Stop command seen from controller - Disabling Auto");
      // disable termostat control until next on command seen.
      controlEnable = 0;
    }

    // Make sure variables are being populated by the heater
    // Sometimes the current temp takes a while to establish
    // Use writerDiff to wait 30 seconds between issuing commands
    // Optional Switch is in correct position
    if (controlEnable == 1 && int(currentTemperature) > 0 && int(setTemperature) > 0 && writerDiff > 30000 && digitalRead(optionalEnableDisableSwitch) == HIGH)
   
	{// Inside this block only gets run every 30 seconds
      
	  writerLastTime = writerTimeNow;
	  
	  
      // Start heater if set temp is 2 degree higher than current temp by more than 1 degree and heater state is "[0] stopped" ***commented out for personal use modification - trailer was getting too cold ***** 
      //if (int(setTemperature) >= (int(currentTemperature) + 2) && error_code <= 1 && heaterStateNum == 0)  {
      
      if (int(setTemperature) >= (int(currentTemperature) + 1) && error_code <= 1 && heaterStateNum == 0)  { // set temp is 1 degree higher than current temp 
        Serial.println("*** Temp Below Set Limit - Starting Heater ***");
        uint8_t data1[24] = {0x78, 0x16, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x32, 0x08, 0x23, 0x05, 0x00, 0x01, 0x2C, 0x0D, 0xAC, 0x8D, 0x82};
        delay(50);
        sOne.write(data1, 24);
        // Now use 0.1 Second LED Heart Beat
        flashLength = 100;
      }

      // shutdown if current temp is 2 degree warmer than set temp - the 2 degree gap allows the heater to throttle as it will not throttle until set temp is 1 degree colder than current temp 
      if (int(setTemperature) +2 <= (int(currentTemperature)) && heaterStateNum == 5) {
        Serial.println("*** Temperature Above Upper Limit - Stopping Heater ***");
        uint8_t data1[24] = {0x78, 0x16, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x32, 0x08, 0x23, 0x05, 0x00, 0x01, 0x2C, 0x0D, 0xAC, 0x61, 0xD6};
        delay(50);
        sOne.write(data1, 24);
		
        // Now use 3 Second LED Heart Beat
        flashLength = 3000;
      }
    }

  }
} // loop
