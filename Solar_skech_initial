//including all the library's
#include <Arduino.h>
#include <Servo.h>
#include "thingProperties.h"
//the pins of the four solar panels
#define solarPanelUR A0;
#define solarPanelDR A1;
#define solarPanelUL A2;
#define solarPanelDL A3;

// Setup code, will run only once
void setup() {

  // start Serial monitor and wait for it to finish[delay is to give the serial monitor time to start]
  // serial monitor can later be used to debug the code
  Serial.begin(9600);
  delay(1500);

  initProperties();
  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  // connect the servo motors
  ServoRight.attach(8);
  ServoLeft.attach(9);
  //setting servos to default positions [currently middle, can be changed]
  ServoRight.write(90);
  ServoLeft.write(90);

  // define some key variables
  int solarDataUR = 0;
  int solarDataDR = 0;
  int solarDataUL = 0;
  int solarDataDL = 0;
  int rotationLevel = 0;
}
// all of the functions below [except the loop function] are for the loop function to call
// these need to be defined above the loop function itself as to avoid errors
void solarData() {
  /*
   all of these will be a value from 0-1023, with 0 being no volatge at pin and 1023 being max [5V].
   prefferably we will not reach anything over 1023, or something close to it,
   as anything above 1023 could burn the arduino.
  */
  solarDataUR = analogRead(solarPanelUR);
  solarDataDR = analogRead(solarPanelDR);
  solarDataUL = analogRead(solarPanelUL);
  solarDataDL = analogRead(solarPanelDL);
}

void adjustLouvers() {
  //rotating the servos[assuming they point towards the same direction]
  ServoRight.write(rotationLevel);
  ServoLeft.write(rotationLevel);
}


void loop() {
  // First, check if anything happened in app
  ArduinoCloud.update();
  // then check if the user has taken control
  if (manualControl = true) {
    /*
      Here, the user has taken manual control over the blinds.
      Since the user enters the level they want the louvers to be at,
      only adjusting is enough
    */
    rotationLevel = manualShades;
    adjustLouvers();
  } else {
    /*
      if the user has not taken manual control, then we are in the automatic mode
      here we first gather the data of the solar panels
    */
    solarData();

    //Then there are some calculations
    /*
    This is taken from Piotr's SSA. For more information read his SSA
    In my case panel A is the upper panel on both sides and panel B the lower panel.
    because the first calculation to get R1 is a ratio calculation, the analogRead()
    values do not need to be altered
  */
    float ratioRight = solarDataUR / (solarDataUR + solardataDR);
    float ratioLeft = solarDataUL / (solarDataUL + solardataDL);
    /*
    Then we calculate the angle.
    for PI the predefined number is used which is in the arduino.h library.
  */
    float angleRight = (PI * ratioRight) / 2;
    float angleLeft = (PI * ratioLeft) / 2;
    /*
    As a final calculation, I chose to take the average of 
    the two sides to get to one average for the entire window
  */
    float angleSun = (angleRight + angleLeft) / 2;
    /*
     since the angle we get is a float value and the write function only accepts int,
     it will need to be rounded first to the nearest interger.
     In addition to this, the angle of the sun [following our calculations] can only be between 0 and 90
     this means that we can directly write this to the rotationLevel variable
  */
    rotationLevel = round(angleSun);
    //Then, adjust the louvers
    adjustLouvers();
    //wait one minute before starting again to save battery power[Can be altered if need be]
    delay(60000);
  }
}
