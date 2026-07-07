#include <Arduino.h>
#include "OneWireHub.h"
#include "owi_pressure_sensor.hpp"
#include "owi_temperature_sensor.hpp"
#include "owi_photo_sensor.hpp"
#include "TH_definitions.h"

#ifndef __AVR_ATmega328P__
#error This firmware is only intended for AVR ATmega328P (Arduino Nano)
#endif

// Createh the OneWireHub, the apropriate hub objects  and the I2C objects.
auto hub = OneWireHub(ONE_WIRE_PIN);
// Create OWI and I2C Temperature/Humidity sensor objects
OWI_Temperature_Sensor* owi_temp_sensor_ptr = nullptr;

OWI_Pressure_Sensor* owi_press_sensor = nullptr;

OWI_Photo_Sensor* owi_photo_sensor = nullptr;

bool has_temperature_sensor = false;
bool has_pressure_sensor = false;
bool has_photo_sensor = true;


// Setup all modules and attach the OWI objects to the hub.
void setup() {
	// Serial.begin(9600);
	pinMode(LED_PIN, OUTPUT);
	pinMode(PHOTO_PIN, INPUT);

	owi_photo_sensor = new OWI_Photo_Sensor(OWI_Photo_Sensor::family_code, OWI_Photo_Sensor::device_type, 0x00, 0x00, 0x00, 0x00, 0x50);
	hub.attach(*owi_photo_sensor);

	Serial.begin(9600);

	// Setup Blink
	digitalWrite(LED_PIN, HIGH);
	delay(500);
	digitalWrite(LED_PIN, LOW);
}


void loop() {
	static unsigned long last_tick = 0;
	unsigned long currentTick = millis();
	if (currentTick - last_tick >= 1000) {	// Take sensor readings every 1 Second
		last_tick = currentTick;

		if(has_temperature_sensor){}

		if(has_pressure_sensor){}

		if(has_photo_sensor){
			int raw = analogRead(PHOTO_PIN);
			owi_photo_sensor->setLux( (raw / 1023.0) * 100.0 );
			digitalWrite(LED_PIN, HIGH);
			delay(200);
			digitalWrite(LED_PIN, LOW);
		}
	}
	hub.poll();	// Poll the OWI bus every cycle to service any incoming requests.
}