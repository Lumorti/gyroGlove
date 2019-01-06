/*
    ggfirmware.ino - Firmware for the gyroglove helper chip
    https://github.com/Lumorti/gyroGlove
*/

#include <Wire.h>

int address = 0x57;

// Hand, thumb, index, middle, ring, little
int handPins[] = {1, 2, 3, 4, 5, 6};
int lastPin = -1;

// r, g, b
int ledPins[] = {7, 8, 9};

void setup() {

    // Start the i2c comms
    Wire.begin(address);

    // Set all the pins to output
    for (int i=0; i<6; i++) {pinMode(handPins[i], OUTPUT);}

}

void loop() {

    while (Wire.available()) {

        // Get the byte from i2c
        int data = Wire.read();

        // Set a certain pins to be on, whilst turning the last pin off
        if (data >=1 and data <=6) {

            if (lastPin >= 0) {digitalWrite(lastPin, LOW);}
            lastPin = handPins[data1-1];
            digitalWrite(lastPin, HIGH);

        }

        // LED to red
        else if (data == 'r') {led(255, 0, 0);}

        // LED to green
        else if (data == 'g') {led(0, 255, 0);}

        // LED to blue
        else if (data == 'b') {led(0, 0, 255);}

    }

}

void led(int r, int g, int b) {

    analogWrite(ledPins[0], r);
    analogWrite(ledPins[1], g);
    analogWrite(ledPins[2], b);

}
