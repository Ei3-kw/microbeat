#define sensorPin 20

#include "Arduino.h"

void setup() {
	pinMode(sensorPin, INPUT);
	Serial.begin(9600);
}

void loop() {
	int soundValue = analogRead(sensorPin);
	Serial.println(soundValue);

}
