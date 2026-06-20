#define dsslavepin 2

#include <OneWireSlave.h>

// 1-Wire ROM ID - kept as DS18B20 family so any existing 1-Wire master/host
// software still recognizes this device. The OneWireSlave library only
// emulates the DS18B20 protocol, so the photoresistor reading is carried
// through the "temperature" field below.
//                     {Family , <---, ----, ----, ID--, ----, --->,  CRC}
unsigned char rom[8] = {DS18B20, 0xAD, 0xDA, 0xCE, 0x0F, 0x00, 0x11, 0x00};
#define SATELIGHT_FAMILY 0x10
#define SATELIGHT_LIGHT_MODULE 0x01
//unsigned char rom[8] = {SATELIGHT_FAMILY, 0x00, 0x00, 0x00, 0x00, 0x00, SATELIGHT_LIGHT_MODULE, 0x00};

//                              {TLSB, TMSB, THRH, TLRL, Conf, 0xFF, Rese, 0x10,  CRC}
unsigned char scratchpad[9] = {0x00, 0x00, 0x4B, 0x46, 0x7F, 0xFF, 0x00, 0x10, 0x00};

//                                  {LightLSB, LightMSB}
// Still packed in the DS18B20's signed 12.4 fixed-point "temperature" format
unsigned char lightReading[2] = {0x00, 0x00};

OneWireSlave ds(dsslavepin);

// Blinking
const int ledPin = 13;
int ledState = LOW;             // ledState used to set the LED
long previousMillis = 0;        // will store last time LED was updated
long interval = 250;            // interval at which to blink (milliseconds)

float lightLevel = 0;
float oldLightLevel = -1;
boolean expose = true;

const int photoPin = A7;

void setup() {
  attachInterrupt(dsslaveassignedint, slave, CHANGE);
  Serial.begin(9600);
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for Leonardo only
//  }
  pinMode(ledPin, OUTPUT);
  ds.init(rom);
  ds.setScratchpad(scratchpad);
  ds.setPower(PARASITE);
  ds.setResolution(9);
  lightLevel = 0;
  ds.attach44h(reportLight);
}

void slave() {
  ds.MasterResetPulseDetection();
}

void loop() {
  blinking();

  readLightSensor();
  printLightLevel();
  //ds.waitForRequest(false);
}

// Called when the 1-Wire master requests a "temperature" conversion (44h).
// We pack the light level (0-100%) into the same 12.4 fixed-point format
// a real DS18B20 would use for temperature, so it transmits correctly.
void reportLight() {
  int16_t scaled = lightLevel * 16;
  lightReading[0] = scaled & 0xFF;
  lightReading[1] = scaled >> 8;
  ds.setTemperature(lightReading);
}

void blinking() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
  }
}

void printLightLevel() {
  if (expose) {
    Serial.println("Light level: ");
    expose = false;
  }
  if (lightLevel != oldLightLevel) {
    Serial.print(lightLevel);
    Serial.println(" %");
    oldLightLevel = lightLevel;
  }
}

void readLightSensor() {
  int raw = analogRead(photoPin);        // 0-1023
  // If your photoresistor is wired so darkness gives a HIGH reading,
  // flip the direction below: lightLevel = 100.0 - (raw / 1023.0) * 100.0;
  lightLevel = (raw / 1023.0) * 100.0;   // 0% (dark) .. 100% (bright)
}
