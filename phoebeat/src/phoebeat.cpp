#include <ArduinoFFT.h>
#include <Arduino.h>

const int NUM_SAMPLES = 256;
const int SAMPLING_RATE = 8000;

float vReal[NUM_SAMPLES];
float vImag[NUM_SAMPLES];

// Create FFT object
ArduinoFFT<float> FFT = ArduinoFFT<float>(vReal, vImag, NUM_SAMPLES, SAMPLING_RATE);

void setup() {
    Serial.begin(115200);
}

void loop() {

    // Read the raw audio data into an array
    double samples[NUM_SAMPLES];
    for (int i = 0; i < NUM_SAMPLES; i++) {
        samples[i] = analogRead(20);
    }

    // Perform the FFT on the raw audio data
    double vReal[NUM_SAMPLES];
    double vImag[NUM_SAMPLES];
    for (int i = 0; i < NUM_SAMPLES; i++) {
        vReal[i] = samples[i];
        vImag[i] = 0;
    }
    FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward);
    FFT.compute(FFTDirection::Forward);
    FFT.complexToMagnitude();

    // Extract the volume and frequency pairs
    for (int i = 2; i < (NUM_SAMPLES / 2); i++) {
        double freq = (i * 1.0 * SAMPLING_RATE) / NUM_SAMPLES;
        double volume = vReal[i];
        Serial.print(volume);
        Serial.print(",");
        Serial.println(freq);
    }

  delay(100);
}

