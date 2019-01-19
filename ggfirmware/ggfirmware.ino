/*
    ggfirmware.ino - Firmware for the gyroglove helper chip
    https://github.com/Lumorti/gyroGlove
*/

#include <Wire.h>

// Hand, thumb, index, middle, ring, little
int handPins[6] = {12, 8, 7, 6, 5, 4};
int lastPin = -1;
int newPin = 12;

// r, g, b
int ledPins[3] = {9, 10, 11};


void setup() {
    
    // Start the i2c comms
    Wire.begin(0x57);
    Wire.onReceive(receiveEvent);
    
    // Set all the pins to output
    for (int i=0; i<6; i++) {pinMode(handPins[i], OUTPUT);}
    for (int i=0; i<3; i++) {pinMode(ledPins[i], OUTPUT);}

}

void loop() {}

void receiveEvent(int howMany) {

    while(Wire.available()){

        // Get the byte from i2c
        char data = Wire.read();
        
        // Set a certain pins to be on, whilst turning the last pin off
        if (int(data) >= 1 && int(data) <= 6) {

            newPin = handPins[int(data)-1];
            if (lastPin != -1 && lastPin != newPin) {digitalWrite(lastPin, LOW);}
            digitalWrite(newPin, HIGH);
            lastPin = newPin;
      
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
