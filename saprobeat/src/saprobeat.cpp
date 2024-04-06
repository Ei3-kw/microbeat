#include <MIDI.h>
#include <arduinoFFT.h>
#include <Arduino.h>

#define ARRAY_SIZE 128
#define KICK_VOL_THRESHOLD 0.7071
#define KICK_MAX_FREQ 250
#define KICK_MIN_FREQ 50


// MIDI instance
MIDI_CREATE_DEFAULT_INSTANCE();

// should FFT be in audio -> midi instead
// FFT instance
// ArduinoFFT<float> FFT;

// Vars to track note events
unsigned long lastNoteOnTime = 0;
unsigned long lastNoteOffTime = 0;
bool isNoteOn = false;

// Vars to calculate BPM
unsigned long beatTimeDiff = 0;
unsigned long bpm = 0;

// 4D array to store duration, volume, pitch & channel
float duration[ARRAY_SIZE];
float volume[ARRAY_SIZE];
float pitch[ARRAY_SIZE];
int channel[ARRAY_SIZE];

// Function prototypes
void noteOnHandler(byte ch, byte note, byte velocity);
void noteOffHandler(byte ch, byte note, byte velocity);
float midi2freq(byte note);
void calculateBPM();

void setup() {
    Serial.begin(9600);

    // Initialise MIDI
    MIDI.begin(MIDI_CHANNEL_OMNI);
    MIDI.setHandleNoteOn(noteOnHandler);
    MIDI.setHandleNoteOff(noteOffHandler);
}

void loop() {
    // Read MIDI events
    MIDI.read();

    // Update BPM every 1s
    if (millis() - lastNoteOffTime >= 1000) {
        calculateBPM();
    }
}

// MIDI note on event handler
void noteOnHandler(byte ch, byte note, byte velocity) {
    // Calculate volume and pitch
    float vol = velocity / 127.0; // 0 = no sound, 127 = max sound
    float freq = midi2freq(note);

    // Store in a 4D array
    duration[note] = millis();
    volume[note] = vol;
    pitch[note] = freq;
    channel[note] = (int)ch;

    // testing
    Serial.print("Note: ");
    Serial.print(note);
    Serial.print(", Channel: ");
    Serial.print(channel[note]);
    Serial.print(", Duration: ");
    Serial.print(duration[note]);
    Serial.print(", Pitch: ");
    Serial.println(pitch[note]);
    Serial.print(", Volume: ");
    Serial.print(volume[note]);

    // Only consider notes between 50-250 Hz and louder than 90 dB
    if (freq >= KICK_MIN_FREQ && freq <= KICK_MAX_FREQ && vol >= KICK_VOL_THRESHOLD) {
        lastNoteOnTime = millis();
        isNoteOn = true;
    }
}

// MIDI note off event handler
void noteOffHandler(byte ch, byte note, byte velocity) {
    // reset 4D array
    duration[note] = 0.0;
    volume[note] = 0.0;
    pitch[note] = 0.0;
    channel[note] = 0;

    // testing
    Serial.print("Note: ");
    Serial.print(note);
    Serial.print(", Channel: ");
    Serial.print(channel[note]);
    Serial.print(", Duration: ");
    Serial.print(duration[note]);
    Serial.print(", Pitch: ");
    Serial.println(pitch[note]);
    Serial.print(", Volume: ");
    Serial.print(volume[note]);

    // Calculate volume and pitch
    float vol = velocity / 127.0; // 0 = no sound, 127 = max sound
    float freq = midi2freq(note);

    // Only consider notes between 50-250 Hz and louder than 90 dB
    if (freq >= KICK_MIN_FREQ && freq <= KICK_MAX_FREQ && vol >= KICK_VOL_THRESHOLD) {
        lastNoteOnTime = millis();
        isNoteOn = false;
    }

    beatTimeDiff = lastNoteOffTime - lastNoteOnTime;
}

// MIDI note to frequency conversion
// ref: https://newt.phys.unsw.edu.au/jw/notes.html
float midi2freq(byte note) {
    return pow(2, (note - 69) / 12.0) * 440;
}


// Calculate the BPM based on the time between note on and note off events
void calculateBPM() {
    if (beatTimeDiff > 0) {
        bpm = (unsigned long)(60000.0 / beatTimeDiff);
        Serial.print("BPM: ");
        Serial.println(bpm);
    }
}
