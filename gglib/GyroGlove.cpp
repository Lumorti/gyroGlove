/*
    GyroGlove.cpp - Library for interfacing with a gyro glove
    https://github.com/Lumorti/gyroGlove
*/

#include <Arduino.h>
#include <Wire.h>

#include "GyroGlove.h"

// Class constructor
GyroGlove::GyroGlove() {



}

// Wake up all the gyros
void GyroGlove::init() {

    Wire.begin();

    // Go through all the different gyroscopes and wake them up
    for (int i = 0; i < 1; i++) {

        // Tell the helper chip to enable the gyro for that finger
        Wire.beginTransmission(chipAddress);
        Wire.write(i+1);
        Wire.endTransmission(true);

        delay(timeBetween);

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
    for (int i = 0; i < 5; i++) {fingersClosedOld[i] = fingersClosed[i];}

    // Tell the helder chip to enable the gyro on the back of the hand
    Wire.beginTransmission(chipAddress);
    Wire.write(1);
    Wire.endTransmission(true);

    delay(timeBetween);

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
    for (int i = 1; i < 5; i++) {

        // Tell the helper chip to enable the gyro for that finger
        Wire.beginTransmission(chipAddress);
        Wire.write(i+1);
        Wire.endTransmission(true);

        delay(timeBetween);

        // Just ask for the y axis rotation
        Wire.beginTransmission(gyroAddress);
        Wire.write(0x45);
        Wire.endTransmission(false);
        Wire.requestFrom(gyroAddress, 2, true);

        // Get the pair of bytes, then combine them
        fingerAccRaw[i] = (Wire.read() << 8 | Wire.read());

    }

    // Scale the values
    acc[0] = int(float(accRaw[0]) * scalingFactor);
    acc[1] = int(float(accRaw[1]) * scalingFactor);
    acc[2] = int(float(accRaw[2]) * scalingFactor);
    rot[0] = int(float(rotRaw[0]) * scalingFactor);
    rot[1] = int(float(rotRaw[1]) * scalingFactor);
    rot[2] = int(float(rotRaw[2]) * scalingFactor);

    oldGestureIndex = nextGestureIndex;

    // See if any gestures were performed
    if (fingerAccRaw[0] > rotThreshold && !fingersClosed[0]) {addGest(Gestures::THUMBCLOSE); fingersClosed[0] = true;}
    else if (fingerAccRaw[0] < -rotThreshold && fingersClosed[0]) {addGest(Gestures::THUMBOPEN); fingersClosed[0] = false;}
    if (fingerAccRaw[1] > rotThreshold && !fingersClosed[1]) {addGest(Gestures::INDEXCLOSE); fingersClosed[1] = true;}
    else if (fingerAccRaw[1] < -rotThreshold && fingersClosed[1]) {addGest(Gestures::INDEXOPEN); fingersClosed[1] = false;}
    if (fingerAccRaw[2] > rotThreshold && !fingersClosed[2]) {addGest(Gestures::MIDDLECLOSE); fingersClosed[2] = true;}
    else if (fingerAccRaw[2] < -rotThreshold && fingersClosed[2]) {addGest(Gestures::MIDDLEOPEN); fingersClosed[2] = false;}
    if (fingerAccRaw[3] > rotThreshold && !fingersClosed[3]) {addGest(Gestures::RINGCLOSE); fingersClosed[3] = true;}
    else if (fingerAccRaw[3] < -rotThreshold && fingersClosed[3]) {addGest(Gestures::RINGOPEN); fingersClosed[3] = false;}
    if (fingerAccRaw[4] > rotThreshold && !fingersClosed[4]) {addGest(Gestures::LITTLECLOSE); fingersClosed[4] = true;}
    else if (fingerAccRaw[4] < -rotThreshold && fingersClosed[4]) {addGest(Gestures::LITTLEOPEN); fingersClosed[4] = false;}

    // Did the gesture list change? If so, don't increase the counter
    if (nextGestureIndex == oldGestureIndex) {sinceLastGesture += 1;}
    else {sinceLastGesture = 0;}

    // If it's been long enough since a gesture, reset the gesture list
    if (sinceLastGesture > timeoutIterations) {nextGestureIndex = 0;}

    // If told to output the data to serial, do so
    if (shouldOutput) {

        Serial.print(accRaw[0]); Serial.print(",");
        Serial.print(accRaw[1]); Serial.print(",");
        Serial.print(accRaw[2]); Serial.print(",");
        Serial.print(rotRaw[0]); Serial.print(",");
        Serial.print(rotRaw[1]); Serial.print(",");
        Serial.print(rotRaw[2]); Serial.print(",");
        Serial.print(fingersClosed[0]); Serial.print(",");
        Serial.print(fingersClosed[1]); Serial.print(",");
        Serial.print(fingersClosed[2]); Serial.print(",");
        Serial.print(fingersClosed[3]); Serial.print(",");
        Serial.print(fingersClosed[4]); Serial.print(",");
        Serial.println(ledCol);

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

    ledCol = colour;

}

// Check for a gesture (main array form)
bool GyroGlove::did(Gestures gestures[]) {


    // Reset temp finger state to start of gestures array
    for (int i = 1; i < 5; i++) {fingersClosedOld[i] = fingersClosed[i];}
    for (int i = nextGestureIndex-1; i >= 0; i--){

        switch(gestureList[i]) {

            case Gestures::THUMBCLOSE: fingersClosedOld[0] = false; break;
            case Gestures::THUMBOPEN: fingersClosedOld[0] = true; break;
            default: break;
        }

    }


    // For first item in the given array

    // Go through main array

    // Evaluating finger changes

    // Until match

    // Then check next item in array

    return false;

}

// Check for a gesture (non-array redirect)
bool GyroGlove::did(Gestures gesture) { return did({gesture}); }

// Setters for settings
void GyroGlove::setRate(int rate)                   { baudRate = rate; }
void GyroGlove::setTimeout(int timeout)             { timeoutIterations = timeout; }
void GyroGlove::setLEDConnected(bool connected)     { ledConnected = connected; }
void GyroGlove::setScalingFactor(float factor)      { scalingFactor = factor; }
void GyroGlove::seRotationThresh(int threshold)     { rotThreshold = threshold; }
void GyroGlove::seAccelerationThresh(int threshold) { accThreshold = threshold; }

void GyroGlove::setOutput(bool output) {

    shouldOutput = output;
    if (shouldOutput) {

        Serial.begin(baudRate);
        Serial.println("connected");

    }

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
