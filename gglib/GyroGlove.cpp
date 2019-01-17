/*
    GyroGlove.cpp - Library for interfacing with a gyro glove
    https://github.com/Lumorti/gyroGlove
*/

#include "Arduino.h"
#include <Wire.h>

#include "GyroGlove.h"

// Class constructor
GyroGlove::GyroGlove() {

    // Settings
    bool shouldOutput = false;
    int baudRate = 9600;
    int timeoutIterations = 10;
    bool ledConnected = true;
    int scalingFactor = 1;
    int fingerCloseThreshold = 5;

    // Fixed params
    const int chipAddress = 0x57;
    const int gyroAddress = 0x69;
    static const int maxGestures = 100;

    // ledPins = {red, green, blue};
    int ledPins[] = {7, 8, 9};

    // Processed values
    int acc[] = {0, 0, 0};
    int rot[] = {0, 0, 0};
    Gestures gestureList[maxGestures];

    // Finger values = {thumbOpen, indexOpen, middleOpen, ringOpen, littleOpen}
    bool fingersClosed[] = {false, false, false, false, false};
    bool fingersClosedOld[] = {false, false, false, false, false};
    int fingerAccels[] = {0, 0, 0, 0, 0};

    // Raw values
    int accRaw[] = {0, 0, 0};
    int rotRaw[] = {0, 0, 0};
    int temperature = 0;

    // Keep track of how many updates have occured since the last gesture
    int sinceLastGesture = 0;

    // For keeping track of the gesture array
    int nextGestureIndex = 0;
    int oldGestureIndex = 0;

}

// Wake up all the gyros
void GyroGlove::init() {

    Wire.begin();

    // Go through all the different gyroscopes and wake them up
    for (int i = 0; i < 6; i++) {

        // Tell the helper chip to enable the gyro for that finger
        Wire.beginTransmission(chipAddress);
        Wire.write(char(i+1));
        Wire.endTransmission(true);

        // Wake up that chip
        Wire.beginTransmission(gyroAddress);
        Wire.write(0x6B);
        Wire.write(0);
        Wire.endTransmission(true);

    }

}

// Main update function
void GyroGlove::update() {

    // The current state is now the old state
    for (int i = 0; i < 6; i++) {fingersClosedOld[i] = fingersClosed[i];}

    // Tell the helder chip to enable the gyro on the back of the hand
    Wire.beginTransmission(chipAddress);
    Wire.write('1');
    Wire.endTransmission(true);

    // Ask for the full enchilada from the gyro on the back of the hand
    Wire.beginTransmission(gyroAddress);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(gyroAddress, 14, true);

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

        // Tell the helper chip to enable the gyro for that finger
        Wire.beginTransmission(chipAddress);
        Wire.write(char(i+1));
        Wire.endTransmission(true);

        // Just ask for the z axis rotation
        Wire.beginTransmission(gyroAddress);
        Wire.write(0x47);
        Wire.endTransmission(false);
        Wire.requestFrom(gyroAddress, 2, true);

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
    if (fingerAccels[0] > fingerCloseThreshold && !fingersClosed[0]) {addGest(Gestures::THUMBCLOSE); fingersClosed[0] = true;}
    else if (fingerAccels[0] < -fingerCloseThreshold && fingersClosed[0]) {addGest(Gestures::THUMBOPEN); fingersClosed[0] = false;}

    // Did the gesture list change? If so, don't increase the counter
    if (nextGestureIndex == oldGestureIndex) {sinceLastGesture += 1;}
    else {sinceLastGesture = 0;}

    // If it's been long enough since a gesture, reset the gesture list
    if (sinceLastGesture > timeoutIterations) {nextGestureIndex = 0;}

    // If told to output the data to serial, do so
    if (shouldOutput) {

        Serial.print(acc[0]); Serial.print(",");
        Serial.print(acc[1]); Serial.print(",");
        Serial.print(acc[2]); Serial.print(",");
        Serial.print(rot[0]); Serial.print(",");
        Serial.print(rot[1]); Serial.print(",");
        Serial.print(rot[2]); Serial.print(",");
        Serial.print(fingersClosed[0]); Serial.print(",");
        Serial.print(fingersClosed[1]); Serial.print(",");
        Serial.print(fingersClosed[2]); Serial.print(",");
        Serial.print(fingersClosed[3]); Serial.print(",");
        Serial.print(fingersClosed[4]); Serial.print(",");
        Serial.println(",");

    }

}

// Add a gesture to the gesture list
void GyroGlove::addGest(Gestures toAdd) {

    gestureList[nextGestureIndex] = toAdd;
    nextGestureIndex += 1;
    if (nextGestureIndex > maxGestures) {nextGestureIndex = 0;}

}

// Set the LED to a certain colour
void GyroGlove::setLED(char colour) {

    Wire.beginTransmission(chipAddress);
    Wire.write(colour);
    Wire.endTransmission(true);

}

// Check for a gesture (main array form)
bool GyroGlove::did(Gestures gestures[]) {

    return true;

}

// Check for a gesture (non-array redirect)
bool GyroGlove::did(Gestures gesture) { return did({gesture}); }

// Setters for settings
void GyroGlove::setRate(int rate)                   { baudRate = rate; }
void GyroGlove::setTimeout(int timeout)             { timeoutIterations = timeout; }
void GyroGlove::setLEDConnected(bool connected)     { ledConnected = connected; }
void GyroGlove::setScalingFactor(bool factor)       { scalingFactor = factor; }
void GyroGlove::setFingerThresh(bool threshold)     { fingerCloseThreshold = threshold; }

void GyroGlove::setOutput(bool output) {

    shouldOutput = output;
    if (shouldOutput) {Serial.begin(baudRate);}

}

// Getters for the finger values
bool * GyroGlove::getFingerState()  { return fingersClosed; }
bool GyroGlove::getIndexOpen()      { return fingersClosed[0]; }
bool GyroGlove::getThumbOpen()      { return fingersClosed[1]; }
bool GyroGlove::getMiddleOpen()     { return fingersClosed[2]; }
bool GyroGlove::getRingOpen()       { return fingersClosed[3]; }
bool GyroGlove::getLittleOpen()     { return fingersClosed[4]; }

// Getters for the scaled values
int * GyroGlove::getAccel()         { return acc; }
int * GyroGlove::getRot()           { return rot; }

// Getters for the raw values
int * GyroGlove::getAccelRaw()      { return accRaw; }
int * GyroGlove::getRotRaw()        { return rotRaw; }
