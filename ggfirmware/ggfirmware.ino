/*
    ggfirmware.ino - Firmware for the gyroglove helper chip
    https://github.com/Lumorti/gyroGlove
*/

#include <Wire.h>

int address = 0x57;

// Hand, thumb, index, middle, ring, little
int handPins[] = {13, 8, 7, 6, 5, 4};
int lastPin = -1;

// r, g, b
int ledPins[] = {9, 10, 11};

char data;
int i;

void setup() {

    // Start the i2c comms
    Wire.begin(address);

    // Set all the pins to output
    for (i=0; i<6; i++) {pinMode(handPins[i], OUTPUT);}

}

void loop() {

    // If there's any data, process it
    while (Wire.available() >= 0) {

        // Get the byte from i2c
        data = Wire.read();

        // Set a certain pins to be on, whilst turning the last pin off
        if (int(data) >= 1 && int(data) <= 6) {

            if (lastPin >= 0) {digitalWrite(lastPin, LOW);}
            lastPin = handPins[int(data)-1];
            digitalWrite(lastPin, HIGH);

        }

        // LED to red
        else if (data == 'r') {led(255, 0, 0);}

        // LED to green
        else if (data == 'g') {led(0, 255, 0);}

        // LED to blue
        else if (data == 'b') {led(0, 0, 255);}

        // LED to yellow
        else if (data == 'y') {led(255, 255, 0);}

        // LED to cyan
        else if (data == 'c') {led(0, 255, 255);}

        // LED to magenta
        else if (data == 'm') {led(255, 0, 255);}

        // LED to white
        else if (data == 'w') {led(255, 255, 255);}

        // LED to orange
        else if (data == 'o') {led(255, 165, 0);}

        // LED to black
        else if (data == 'l') {led(0, 0, 0);}

    }

}

// Function to save space, sets the LED to a certain RGB value
void led(int r, int g, int b) {

    analogWrite(ledPins[0], r);
    analogWrite(ledPins[1], g);
    analogWrite(ledPins[2], b);

}
