#include <ArduinoBLE.h>
#include <Arduino_APDS9960.h>

const char* deviceServiceUuid = "19b10000-e8f2-537e-4f6c-d104768a1214";
const char* deviceServiceCharacteristicUuid = "19b10001-e8f2-537e-4f6c-d104768a1214";

int hs = 1;
int oldhs = 1;

/* 
0 - aries
1 - taurus
2 - gemini
3 - cancer
4 - leo
5 - virgo
6 - libra
7 - scorpius
8 - sagittarius
9 - capricornus
10 - aquarius
11 - pisces
*/

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!BLE.begin()) {
    Serial.println("* Starting Bluetooth® Low Energy module failed!");
    while (1);
  }
  
  BLE.setLocalName("Nano 33 BLE (Central)"); 
  BLE.advertise();

  Serial.println("Arduino Nano 33 BLE Sense (Central Device)");
  Serial.println(" ");
}

void loop() {
  connectToPeripheral();
}

void connectToPeripheral(){
  BLEDevice peripheral;
  
  Serial.println("- Discovering peripheral device...");

  do
  {
    BLE.scanForUuid(deviceServiceUuid);
    peripheral = BLE.available();
  } while (!peripheral);
  
  if (peripheral) {
    Serial.println("* Peripheral device found!");
    Serial.print("* Device MAC address: ");
    Serial.println(peripheral.address());
    Serial.print("* Device name: ");
    Serial.println(peripheral.localName());
    Serial.print("* Advertised service UUID: ");
    Serial.println(peripheral.advertisedServiceUuid());
    Serial.println(" ");
    BLE.stopScan();
    controlPeripheral(peripheral);
  }
}

void controlPeripheral(BLEDevice peripheral) {
  Serial.println("- Connecting to peripheral device...");

  if (peripheral.connect()) {
    Serial.println("* Connected to peripheral device!");
    Serial.println(" ");
  } else {
    Serial.println("* Connection to peripheral device failed!");
    Serial.println(" ");
    return;
  }

  Serial.println("- Discovering peripheral device attributes...");
  if (peripheral.discoverAttributes()) {
    Serial.println("* Peripheral device attributes discovered!");
    Serial.println(" ");
  } else {
    Serial.println("* Peripheral device attributes discovery failed!");
    Serial.println(" ");
    peripheral.disconnect();
    return;
  }

  BLECharacteristic horoscopeCharacteristic = peripheral.characteristic(deviceServiceCharacteristicUuid);
    
  if (!horoscopeCharacteristic) {
    Serial.println("* Peripheral device does not have horoscope characteristic!");
    peripheral.disconnect();
    return;
  } else if (!horoscopeCharacteristic.canWrite()) {
    Serial.println("* Peripheral does not have a writable horoscope characteristic!");
    peripheral.disconnect();
    return;
  }
  
  while (peripheral.connected()) {
    if (oldhs != hs) {  
      oldhs = hs;
      Serial.print("* Writing value to horoscope characteristic: ");
      Serial.println(hs);
      horoscopeCharacteristic.writeValue((byte)hs);
      Serial.println("* Writing value to horoscope characteristic done!");
      Serial.println(" ");
    }
  
  }
  Serial.println("- Peripheral device disconnected!");
}