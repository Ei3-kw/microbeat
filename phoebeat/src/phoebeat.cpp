#include <ArduinoFFT.h>

// Define the number of samples to read and the sampling rate
const int NUM_SAMPLES = 256;
const int SAMPLING_RATE = 8000;

float vReal[NUM_SAMPLES];
float vImag[NUM_SAMPLES];

ArduinoFFT<float> FFT = ArduinoFFT<float>(vReal, vImag, NUM_SAMPLES, SAMPLING_RATE); /* Create FFT object */

void setup() {
    Serial.begin(9600);
}

void loop() {
    // Read the raw audio data into an array
    double samples[NUM_SAMPLES];
    for (int i = 0; i < NUM_SAMPLES; i++) {
        samples[i] = analogRead(A0);
    }

    // Perform the FFT on the raw audio data
    double vReal[NUM_SAMPLES];
    double vImag[NUM_SAMPLES];
    for (int i = 0; i < NUM_SAMPLES; i++) {
        vReal[i] = samples[i];
        vImag[i] = 0;
    }
    FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward); /* Weigh data */
    FFT.compute(FFTDirection::Forward); /* Compute FFT */
    FFT.complexToMagnitude(); /* Compute magnitudes */
    // float x = FFT.majorPeak();

  // Extract the volume and frequency pairs
  for (int i = 2; i < (NUM_SAMPLES / 2); i++) {
      double freq = (i * 1.0 * SAMPLING_RATE) / NUM_SAMPLES;
      double volume = vReal[i];
      Serial.print("Frequency: ");
      Serial.print(freq);
      Serial.print(" Hz, Volume: ");
      Serial.println(volume);
  }

  delay(100);
}