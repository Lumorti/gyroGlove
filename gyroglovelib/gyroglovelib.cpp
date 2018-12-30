/*
    gyroglovelib.cpp - Library for interfacing with a gyro glove
    https://github.com/Lumorti/gyroGlove
*/

#include "Arduino.h"
#include "gyroglovelib.h"

// Class constructor
GyroGlove::GyroGlove() {

    // Settings
    bool shouldOutput = false;
    int baudRate = 9600;
    int timeoutIterations = 10;
    bool ledConnected = true;
    int scalingFactor = 1;
    int fingerCloseThreshold = 5;

    // fingerPins = {handPin, thumbPin, indexPin, middlePin, ringPin, littlePin};
    int fingerPins[] = {1, 2, 3, 4, 5, 6};

    // ledPins = {red, green, blue};
    int ledPins[] = {7, 8, 9};

    // Processed values
    int acc[] = {0, 0, 0}
    int rot[] = {0, 0, 0}
    Gestures gestureList[100];

    // Finger values = {thumbOpen, indexOpen, middleOpen, ringOpen, littleOpen}
    bool fingersClosed[] = {false, false, false, false, false}
    bool fingersClosedOld[] = {false, false, false, false, false}
    int fingerAccels[] = {0, 0, 0, 0, 0};

    // Raw values
    int accRaw[] = {0, 0, 0}
    int rotRaw[] = {0, 0, 0}
    int temperature = 0;

    // Keep track of how many updates have occured since the last gesture
    int sinceLastGesture = 0;

    // For keeping track of the gesture array
    int nextGestureIndex = 0;
    int oldGestureIndex = 0;

}

// Wake up all the gyros
void GyroGlove::init() {

    // Go through all the different gyroscopes and wake them up
    for (int i = 0; i < 6; i++) {

        digitalWrite(fingerPins[i], HIGH)
        Wire.beginTransmission(0x69);
        Wire.write(0x6B);
        Wire.write(0);
        Wire.endTransmission(true);

    }

}

// Main update function
void GyroGlove::update() {

    // The current state is now the old state
    for (int i = 0; i < 6; i++) {fingersClosedOld[i] = fingersClosed[i];}

    // Ask for the full enchilada from the gyro on the back of the hand
    digitalWrite(fingerPins[0], HIGH)
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr, 14, true);

    // Get the pairs of bytes, then combine them
    accRaw[0] = Wire.read() << 8 | Wire.read();
    accRaw[1] = Wire.read() << 8 | Wire.read();
    accRaw[2] = Wire.read() << 8 | Wire.read();
    temperature = Wire.read() << 8 | Wire.read();
    rotRaw[0] = Wire.read() << 8 | Wire.read();
    rotRaw[1] = Wire.read() << 8 | Wire.read();
    rotRaw[2] = Wire.read() << 8 | Wire.read();

    // Go through all the different finger gyroscopes and get just the Z values
    for (int i = 1; i < 6; i++) {

        // Just ask for the z axis rotation
        digitalWrite(fingerPins[i], HIGH)
        Wire.beginTransmission(MPU_addr);
        Wire.write(0x47);
        Wire.endTransmission(false);
        Wire.requestFrom(MPU_addr, 2, true);

        // Get the pair of bytes, then combine them
        fingerAccels[i] = (Wire.read() << 8 | Wire.read());

    }

    // Scale the values
    acc[0] = accRaw[0] * scalingFactor;
    acc[1] = accRaw[1] * scalingFactor;
    acc[2] = accRaw[2] * scalingFactor;
    rot[0] = rotRaw[0] * scalingFactor;
    rot[1] = rotRaw[1] * scalingFactor;
    rot[2] = rotRaw[2] * scalingFactor;

    oldGestureIndex = nextGestureIndex;

    // See if any gestures were performed
    if (fingerAccels[0] > fingerCloseThreshold && !fingersClosed[i]) {addGest(Gestures::THUMBCLOSE); fingersClosed[i] = true;}
    else if (fingerAccels[0] < -fingerCloseThreshold && fingersClosed[i]) {addGest(Gestures::THUMBOPEN); fingersClosed[i] = false;}

    // Did the gesture list change? If so, don't increase the counter
    if (nextGestureIndex == oldGestureIndex) {sinceLastGesture += 1;}
    else {sinceLastGesture = 0;}

    // If it's been long enough since a gesture, reset the gesture list
    if (sinceLastGesture > timeoutIterations) {nextGestureIndex = 0;}

}

// Add a gesture to the gesture list
void GyroGlove::addGest(Gesture toAdd) {

    gestureList[nextGestureIndex] = toAdd;
    nextGestureIndex += 1
    if (nextGestureIndex > 100) {nextGestureIndex = 0;}

}

// Check for a gesture (main array form)
bool GyroGlove::did(Gestures gestures[]) {



}

// Set the LED to a certain RGB value
void GyroGlove::setLED(int r, int g. int b) {

    analogWrite(ledPins[0], r);
    analogWrite(ledPins[1], g);
    analogWrite(ledPins[2], b);

}

// Check for a gesture (non-array redirect)
bool GyroGlove::did(Gestures gesture) { return did({gesture}); }

// Setters for settings
void GyroGlove::setHand(int pin)                    { fingerPins[0] = pin; }
void GyroGlove::setThumb(int pin)                   { fingerPins[1] = pin; }
void GyroGlove::setIndex(int pin)                   { fingerPins[2] = pin; }
void GyroGlove::setMiddle(int pin)                  { fingerPins[3] = pin; }
void GyroGlove::setRing(int pin)                    { fingerPins[4] = pin; }
void GyroGlove::setLittle(int pin)                  { fingerPins[5] = pin; }
void GyroGlove::setOutput(bool output)              { shouldOutput = output; }
void GyroGlove::setRate(int rate)                   { baudRate = rate; }
void GyroGlove::setTimeout(int timeout)             { timeoutIterations = timeout; }
void GyroGlove::setLEDConnected(bool connected)     { ledConnected = connected; }
void GyroGlove::setScalingFactor(bool factor)       { scalingFactor = factor; }
void GyroGlove::setFingerThresh(bool threshold)     { fingerCloseThreshold = threshold; }

void GyroGlove::setRed(int pin)                     { ledPins[0] = pin; }
void GyroGlove::setGreen(int pin)                   { ledPins[1] = pin; }
void GyroGlove::setBlue(int pin)                    { ledPins[2] = pin; }

// Getters for the finger values
bool[] GyroGlove::getFingerState()  { return fingersClosed; }
bool GyroGlove::getIndexOpen()      { return fingersClosed[0]; }
bool GyroGlove::getThumbOpen()      { return fingersClosed[1]; }
bool GyroGlove::getMiddleOpen()     { return fingersClosed[2]; }
bool GyroGlove::getRingOpen()       { return fingersClosed[3]; }
bool GyroGlove::getLittleOpen()     { return fingersClosed[4]; }

// Getters for the scaled values
int[] GyroGlove::getAccel()         { return acc; }
int[] GyroGlove::getRot()           { return rot; }

// Getters for the raw values
int[] GyroGlove::getAccelRaw()      { return accRaw; }
int[] GyroGlove::getRotRaw()        { return rotRaw; }
