#include "saprobeat.h"

// keyboard playing
NoteState noteStates[MAX_NOTE];
int noteCount = 0;
unsigned long lastKeyPressTime = 0;

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

void setup() {
    Serial.begin(9600);


    // Initialise MIDI
    MIDI.begin(MIDI_CHANNEL_OMNI);
    MIDI.setHandleNoteOn(noteOnHandler);
    MIDI.setHandleNoteOff(noteOffHandler);
}

void loop() {
    // if (Serial.available()) {
    //     char serial_input = Serial.read();

    //     // Convert the ASCII char (0-9) to a MIDI note number
    //     int note = serial_input - '0' + 60;

    //     // Send the MIDI note
    //     MIDI.sendNoteOn(note, 127, 1); // MIDI channel 1, velocity 127
    //     delay(100); // Adjust as needed for your application

    //     // Turn off the note after a delay
    //     MIDI.sendNoteOff(note, 0, 1); // MIDI channel 1
    // }

    if (Serial.available()) {
        char serial_input = Serial.read();

        // Convert the ASCII character to a MIDI note number
        int note = serial_input - '0' + 60; // Assuming the characters represent MIDI note numbers 0-9

        // Find an empty slot in the note states array
        int empty = -1;
        for (int i = 0; i < MAX_NOTE; i++) {
            if (noteStates[i].note == 0) {
                empty = i;
                break;
            }
        }

        // If an empty slot is found
        if (empty != -1) {
            // Store the note and channel in the empty slot
            noteStates[empty].note = note;
            noteStates[empty].channel = noteCount % 16 + 1; // Cycle through MIDI channels 1-16
            noteStates[empty].startTime = millis();

            // Send the MIDI note-on message
            MIDI.sendNoteOn(note, 127, noteStates[empty].channel); // Send on the determined channel
            noteCount++;
        }
    }

    // Check for note-off events
    for (int i = 0; i < MAX_NOTE; i++) {
        if (noteStates[i].note != 0) {
            // Calculate the duration the key has been held
            unsigned long currentMillis = millis();
            unsigned long duration = currentMillis - noteStates[i].startTime;

            // Check if the key has been held for longer than the debounce delay
            if (duration >= DEBOUCE_DELAY) {
                // Calculate the remaining time until the debounce delay
                unsigned long remainingTime = DEBOUCE_DELAY - (duration - DEBOUCE_DELAY);

                // Send the MIDI note-off message after the remaining time
                if (remainingTime > 0) {
                    delay(remainingTime);
                }
                MIDI.sendNoteOff(noteStates[i].note, 0, noteStates[i].channel);

                // Reset the note state
                noteStates[i].note = 0;
                noteCount--;
            }
        }
  }

    // // Read MIDI events
    // if (MIDI.read()) {
    //     byte command = MIDI.getType();
    //     byte note = MIDI.getData1();
    //     byte velocity = MIDI.getData2();
    //     handleMidiMessage(command, note, velocity);
    // }

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

    // TESTING
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

    // TESTING
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