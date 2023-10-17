//including all the library's
#include <Arduino.h>
#include <Servo.h>
#include "thingProperties.h"

//the pins of the four solar panels
#define solarPanelUR "A0";
#define solarPanelDR "A1";
#define solarPanelUL "A2";
#define solarPanelDL "A3";

Servo servoRight;
Servo servoLeft;

// rotationLevel defined globally because it's easier
int rotationLevel = 0;

// Setup code, will run only once
void setup() {

  // start Serial monitor [delay is to give the serial monitor time to start]
  Serial.begin(9600);
  delay(1500);

  initProperties();
  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  // connect the servo motors
  servoRight.attach(8);
  
  //setting servos to default positions [currently middle, can be changed]
  servoRight.write(90);



}

// these need to be defined above the loop function itself as to avoid errors

void adjustLouvers() { //called by loop function 
  //rotating the servos[assuming they point towards the same direction]
  servoRight.write(rotationLevel);
  
}

/*
  Since ManualControl is READ_WRITE variable, onManualControlChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onManualControlChange()  {
  // Add your code here to act upon ManualControl change
  // nothing needs to be inside the function, maybe a jump to the start of the loop.
  // the funtion itself just needs to be here for the code to work :)
  delay(1000);
}
void onDataChange(){

}
/*
  Since ManualShades is READ_WRITE variable, onManualShadesChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onManualShadesChange()  {
  if(manualControl==true){
  rotationLevel = manualShades*2;
  }
}

void loop() {

  // First, check if anything happened in app
  ArduinoCloud.update();
  Serial.println(manualControl);
    //get battery voltage
    /*
    The code to measure the battery level using the internal reference did not work.
    Now my idea is to add an external voltage divider connected to the arduino to
    measure the voltage at the A6 pin. the voltage divider is needed because
    we are powering the arduino with more then 5V, which the arduino itself cannot
    handle checking. we need to lower the voltage from the batteries by a factor
    of 4 [or 5 to be safe]. after checking on the pin we can then multiply the 
    result back up by 4 [or 5] to get the battery voltage.
    */
  int analogBatteryLevel = analogRead(A5);
  //calculate voltage from analogpin and multiply by 4 to get voltage before divider
  float batteryVoltage = (analogBatteryLevel/1023)*5*4;
  /*
    battery level can be calculated when i have specifics on battery.
    the voltage a li-ion battery provides lowers as the charge level lowers.
    I would need to know how much the voltage drops over time to get the charge level
    for now I will leave it at the voltage
  */
  delay(1000);

  // then check if the user has taken control
  if (manualControl == true) {
    /*
      Here, the user has taken manual control over the blinds.
      Since the user enters the level they want the louvers to be at,
      only adjusting is enough
    */
    
    adjustLouvers();
  } else {    //if not manual
    /*
      if the user has not taken manual control, then we are in the automatic mode
      here we first gather the data of the solar panels.
      all of these will be a value from 0-1023, with 0 being no volatge at pin and 1023 being max [5V].
      prefferably we will not reach anything over 1023, or something close to it,
      as anything above 1023 could burn the arduino.
    */
    float solarDataU = analogRead(A0)-100;
    float solarDataD = analogRead(A1)-100;
    Serial.print("Upper panel:");
    Serial.println(solarDataU);
    Serial.print("Lower panel:");
    Serial.println(solarDataD);
    


    //Then there are some calculations
    /*
    This is taken from Piotr's SSA. For more information read his SSA
    In my case panel A is the upper panel on both sides and panel B the lower panel.
    because the first calculation to get R1 is a ratio calculation, the analogRead()
    values do not need to be altered
  */
    float ratio = solarDataU/(solarDataU+solarDataD);
    Serial.print("ratio: ");
    Serial.println(ratio);

    /*
    Then we calculate the angle.
    for PI the predefined number is used which is in the arduino.h library.
  */
    float angleSun = ((PI * ratio) / 2)*RAD_TO_DEG;
    Serial.print("angle: ");
    Serial.println(angleSun);
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
    //[lowered because verification of code took long]
    delay(600);
  }
}
