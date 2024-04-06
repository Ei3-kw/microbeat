// // this file exists for testing purpose
// // handling file system on Mac
// #include <stdio.h>
// #include <stdlib.h>
// #include "saprobeat.h"

// // MIDI file
// FILE* midiFile;
// unsigned long filePos = 0;
// unsigned long lastEventTime = 0;

// unsigned long readVarLen(FILE* file);
// void processMIDIFile();


// void setup() {
//     // TESTING
//     // Open the MIDI file
//     midiFile = fopen("../test/midi/Bladerunner Synth Lead.mid", "r");
//     while (!midiFile) {
//         Serial.println("Failed to open MIDI file");
//         midiFile = fopen("../test/midi/Bladerunner Synth Lead.mid", "r");
//     }
// }

// void loop() {
//     // TESTING
//     // Process MIDI events from the file
//     processMIDIFile();

// }

// // Read a variable-length number from the MIDI file
// unsigned long readVarLen(FILE* file) {
//     unsigned long value = 0;
//     byte b;
//     do {
//         b = fgetc(file);
//         value = (value << 7) + (b & 0x7F);
//     } while (b & 0x80);
//     return value;
// }

// // TESTING
// void processMIDIFile() {
//     while (fread(&filePos, sizeof(unsigned long), 1, midiFile) == 1) {
//         // Read the MIDI event
//         unsigned long deltaTime = readVarLen(midiFile);
//         lastEventTime += deltaTime;

//         byte type = fgetc(midiFile);
//         byte channel = type & 0x0F;
//         type &= 0xF0;

//         if (type == 0x90) {  // Note on event
//             byte note = fgetc(midiFile);
//             byte velocity = fgetc(midiFile);
//             noteOnHandler(channel, note, velocity, lastEventTime);
//         } else if (type == 0x80) {  // Note off event
//             byte note = fgetc(midiFile);
//             byte velocity = fgetc(midiFile);
//             noteOffHandler(channel, note, velocity, lastEventTime);
//         } else {
//             // Ignore other MIDI events
//             fgetc(midiFile);
//         }
//     }
// }


#include <stdio.h>
#include <stdlib.h>

FILE* midiFile;

unsigned long readVarLen(FILE* file);
void serializeMIDIFile();

int main() {
    // Open the MIDI file
    midiFile = fopen("../test/midi/Bladerunner Synth Lead.mid", "rb");
    if (!midiFile) {
        exit(EXIT_FAILURE);
    }

    // Serialise MIDI file
    serializeMIDIFile();

    return 0;
}

unsigned long readVarLen(FILE* file) {
    unsigned long value = 0;
    unsigned char b;
    do {
        b = fgetc(file);
        value = (value << 7) + (b & 0x7F);
    } while (b & 0x80);
    return value;
}

void serializeMIDIFile() {
    unsigned long deltaTime;
    unsigned char buffer[4];

    while (fread(&deltaTime, sizeof(unsigned long), 1, midiFile) == 1) {
        // Serialize delta time
        buffer[0] = (deltaTime >> 24) & 0xFF;
        buffer[1] = (deltaTime >> 16) & 0xFF;
        buffer[2] = (deltaTime >> 8) & 0xFF;
        buffer[3] = deltaTime & 0xFF;
        fwrite(buffer, sizeof(unsigned char), 4, stdout);

        // Read the MIDI event
        unsigned long eventLength = readVarLen(midiFile);
        for (unsigned long i = 0; i < eventLength; i++) {
            unsigned char byte = fgetc(midiFile);
            fwrite(&byte, sizeof(unsigned char), 1, stdout);
        }
    }

    fclose(midiFile);
    exit(EXIT_SUCCESS);
}
