#include <MIDI.h>
#include <arduinoFFT.h>
#include <Arduino.h>

// MIDI instance
MIDI_CREATE_DEFAULT_INSTANCE();

// FFT instance
ArduinoFFT<float> FFT;

// 3D array to store duration, volume, pitch & channel
const int ARRAY_SIZE = 128; // change me if needed
float duration[ARRAY_SIZE];
float volume[ARRAY_SIZE];
float pitch[ARRAY_SIZE];
int channel[ARRAY_SIZE];

// Function prototypes
void noteOnHandler(byte ch, byte note, byte velocity);
void noteOffHandler(byte ch, byte note, byte velocity);
float midi2freq(byte note);

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
    Serial.print(", Pitch: ");
    Serial.println(pitch[note]);
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
    Serial.print(", Pitch: ");
    Serial.println(pitch[note]);
}

// MIDI note to frequency conversion
// ref: https://newt.phys.unsw.edu.au/jw/notes.html
float midi2freq(byte note) {
    return pow(2, (note - 69) / 12.0) * 440;
}