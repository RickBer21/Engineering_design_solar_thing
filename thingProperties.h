// Code generated by Arduino IoT Cloud, DO NOT EDIT.

#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

const char DEVICE_LOGIN_NAME[]  = "2f8f956b-e75e-4ca1-bb64-39bfcc653f8e";

const char SSID[]               = "secret";    // Network SSID (name)
const char PASS[]               = "secret";    // Network password (use for WPA, or use as key for WEP)
const char DEVICE_KEY[]  = "Secret";    // Secret device password

void onDataChange();
void onManualShadesChange();
void onManualControlChange();

String data;
int manualShades;
bool manualControl;

void initProperties(){

  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
  ArduinoCloud.addProperty(data, READWRITE, ON_CHANGE, onDataChange);
  ArduinoCloud.addProperty(manualShades, READWRITE, ON_CHANGE, onManualShadesChange);
  ArduinoCloud.addProperty(manualControl, READWRITE, ON_CHANGE, onManualControlChange);

}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);
