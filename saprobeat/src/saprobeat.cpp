#include <MIDI.h>
#include <arduinoFFT.h>

// MIDI instance
MIDI_CREATE_DEFAULT_INSTANCE();

// FFT instance
ArduinoFFT<float> FFT;

// 2D array to store volume and pitch
const int ARRAY_SIZE = 128;
float volume[ARRAY_SIZE][2];
float pitch[ARRAY_SIZE][2];

// Function prototypes
void noteOnHandler(byte channel, byte note, byte velocity);
void noteOffHandler(byte channel, byte note, byte velocity);
float midi2freq(byte note);

void setup() {
  // Initialize MIDI
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleNoteOn(noteOnHandler);
  MIDI.setHandleNoteOff(noteOffHandler);
}

void loop() {
  // Read MIDI events
  MIDI.read();
}

// MIDI note on event handler
void noteOnHandler(byte channel, byte note, byte velocity) {
  // Calculate volume and pitch
  float vol = velocity / 127.0;
  float freq = midi2freq(note);

  // Store volume and pitch in the 2D array
  volume[note][0] = vol;
  volume[note][1] = millis();
  pitch[note][0] = freq;
  pitch[note][1] = millis();
}

// MIDI note off event handler
void noteOffHandler(byte channel, byte note, byte velocity) {
  // Clear the volume and pitch data for the note
  volume[note][0] = 0.0;
  volume[note][1] = 0.0;
  pitch[note][0] = 0.0;
  pitch[note][1] = 0.0;
}

// MIDI note to frequency conversion
// ref: https://newt.phys.unsw.edu.au/jw/notes.html
float midi2freq(byte note) {
  return pow(2, (note - 69) / 12.0) * 440;
}