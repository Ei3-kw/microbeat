#define sensorPin 20

#include "Arduino.h"
#include <cmath>

float volumeEstimate = 0;
float alpha = 0.5   // Smoothing factor
int time = 0 //
int freq;

void setup() {
	pinMode(sensorPin, INPUT);
	Serial.begin(9600);
}

void loop() {
	// Avg volume
	int soundValue = analogRead(sensorPin);
	volumeEstimate += alpha * soundValue / (1+ alpha);
	
	// Spoof frequency using a sin curve
	// Units of frequency are Hz
	// We want freq in 100 - 300hz
	freq = 100 + sin(time) * 100;
	
	Serial.print(volumeEstimate);
	Serial.print(",");
	Serial.println(freq);
}
