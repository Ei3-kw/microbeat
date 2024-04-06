#include <arduinoFFT.h>
#include <Audio.h>

// Audio configuration
#define SAMPLE_RATE 44100
#define FFT_SIZE 256
#define SIZE 8

// FFT object
ArduinoFFT fft(FFT_SIZE);

// Audio input
AudioInputAnalog audioIn(A0);

// 2D spectrum data
double spectrum2D[SIZE][SIZE];

void setup() {
    // Initialise audio input
    AudioMemory(12);
    audioIn.begin();

    // Clear the 2D spectrum data
    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
          spectrum2D[y][x] = 0.0;
        }
    }
}

void loop() {
    // Capture audio samples
    double samples[FFT_SIZE];
    for (int i = 0; i < FFT_SIZE; i++) {
      samples[i] = audioIn.read();
    }

    // Perform FFT
    fft.compute(samples, FFT_SIZE, FFT_FORWARD);

    // Compute magnitude spectrum
    for (int i = 0; i < FFT_SIZE / 2; i++) {
        double magnitude = fft.real(i) * fft.real(i) + fft.imag(i) * fft.imag(i);
        magnitude = sqrt(magnitude) / FFT_SIZE;

        // Map the magnitude to the 2D spectrum
        int x = map(i, 0, FFT_SIZE / 2 - 1, 0, SIZE - 1);
        int y = map(magnitude * 100, 0, 100, SIZE - 1, 0);
        spectrum2D[y][x] = magnitude;
    }

    // Display the 2D spectrum
    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
            // Your display logic here, e.g., using an LED matrix
            // or sending the data to a connected display
            displayPixel(x, y, spectrum2D[y][x] * 255);
        }
    }
}