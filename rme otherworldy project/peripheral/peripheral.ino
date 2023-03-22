#include <ArduinoBLE.h>
      
enum {
  ARIES = 0,
  TAURUS = 1,
  GEMINI = 2,
  CANCER = 3,
  LEO = 4,
  VIRGO = 5,
  LIBRA = 6,
  SCORPIUS = 7,
  SAGITTARIUS = 8,
  CAPRICORNUS = 9,
  AQUARIUS = 10,
  PISCES = 11
};

const char* deviceServiceUuid = "19b10000-e8f2-537e-4f6c-d104768a1214";
const char* deviceServiceCharacteristicUuid = "19b10001-e8f2-537e-4f6c-d104768a1214";

int hs = 0;
int myhs = 6; // nicole is a libra

BLEService horoscopeService(deviceServiceUuid);
BLEByteCharacteristic horoscopeCharacteristic(deviceServiceCharacteristicUuid, BLERead | BLEWrite);


void setup() {
  Serial.begin(9600);
  while (!Serial);  
  
  if (!BLE.begin()) {
    Serial.println("- Starting Bluetooth® Low Energy module failed!");
    while (1);
  }

  BLE.setLocalName("Arduino Nano 33 BLE (Peripheral)");
  BLE.setAdvertisedService(horoscopeService);
  horoscopeService.addCharacteristic(horoscopeCharacteristic);
  BLE.addService(horoscopeService);
  horoscopeCharacteristic.writeValue(0);
  BLE.advertise();

  Serial.println("Nano 33 BLE (Peripheral Device)");
  Serial.println(" ");
}

void loop() {
  BLEDevice central = BLE.central();
  Serial.println("- Discovering central device...");
  delay(500);

  if (central) {
    Serial.println("* Connected to central device!");
    Serial.print("* Device MAC address: ");
    Serial.println(central.address());
    Serial.println(" ");

    while (central.connected()) {
      if (horoscopeCharacteristic.written()) {
         hs = horoscopeCharacteristic.value();
         Serial.println("- Characteristic <horoscope> has changed!");
         Serial.print("* Actual value: "); Serial.println(hs);
         Serial.println(" ");


        drawPartnerHoroscope(hs);
        int gesture = classifyGesture();
        int compatability = checkCompatability(hs);
        drawResult(gesture, compatability);

       }
    }
    
    Serial.println("* Disconnected to central device!");
  }
}


// classifies as idle (0), high five (1), or punch (2)
int classifyGesture() {
  return 0;
}

// checks if the received horoscope is compatible with the default of this device
int checkCompatability(int horoscope) {
  // "Libra Natives Are Most Compatible With: Aquarius, Gemini, Sagittarius"
  return (horoscope == 10 || horoscope == 2 || horoscope == 8) ? 1 : 0;
}

// TODO @nicole
void drawPartnerHoroscope(int horoscope) {
  // based on the above enum, write `horoscope` to the LED display
  // also add a message saying to try to interact with the partner
}

// TODO @nicole
void drawResult(int gesture, int compatability) {
  // gesture is 0 for idle, 1 for high five, 2 for punch
  // compatability is 0 for not compatible, 1 for yes
  // draw result accordingly
}