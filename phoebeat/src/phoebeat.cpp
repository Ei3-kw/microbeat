// #include <iostream>
// #include <cstdlib>
// #include <vector>
// #include <RtMidi.h>

// // Function to be called when a MIDI message is received
// void midiCallback(double deltaTime, std::vector<unsigned char>* message, void* userData) {
//     // Extract MIDI message data
//     unsigned int nBytes = message->size();
//     for (unsigned int i = 0; i < nBytes; i++) {
//         std::cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
//     }
//     if (nBytes > 0)
//         std::cout << "deltaTime = " << deltaTime << std::endl;
// }

// int main() {
//     RtMidiIn midiIn;

//     try {
//         // Open the first available MIDI input port
//         midiIn.openPort();

//         // Set the callback function for MIDI input
//         midiIn.setCallback(&midiCallback);

//         // Ignore sysex, timing, and active sensing messages.
//         midiIn.ignoreTypes(true, true, true);

//         std::cout << "Reading MIDI input. Press Enter to quit.\n";
//         char input;
//         std::cin.get(input);
//     } catch (RtMidiError &error) {
//         error.printMessage();
//     }

//     return 0;
// }

void setup() {}
void loop() {}

