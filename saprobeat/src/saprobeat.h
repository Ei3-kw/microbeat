#include <MIDI.h>
// #include <arduinoFFT.h>
#include <Arduino.h>

#define ARRAY_SIZE 128
#define KICK_VOL_THRESHOLD 0.7071 // approx 90dB
#define KICK_MAX_FREQ 250
#define KICK_MIN_FREQ 50

// MIDI instance
MIDI_CREATE_DEFAULT_INSTANCE();

// should FFT be in audio -> midi instead
// FFT instance
// ArduinoFFT<float> FFT;

// Function prototypes
void noteOnHandler(byte ch, byte note, byte velocity,
    unsigned long lastEventTime=0);
void noteOffHandler(byte ch, byte note, byte velocity,
    unsigned long lastEventTime=0);
float midi2freq(byte note);
void calculateBPM();