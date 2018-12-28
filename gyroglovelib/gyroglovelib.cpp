/*
    gyroglovelib.cpp - Library for interfacing with a gyro glove
    https://github.com/Lumorti/gyroGlove
*/

#include "Arduino.h"
#include "gyroglovelib.h"

// Class constructor
GyroGlove::GyroGlove() {

    // Settings
    bool shouldOutput;
    int baudRate;
    int thumbPin, indexPin, middlePin, ringPin, littlePin;
    int timeoutIterations;
    bool ledConnected;

    // Processed values
    int accX, accY, accZ;
    int rotX, rotY, rotZ;
    Gestures gestureList[];

    // Raw values
    bool indexOpen, middleOpen, thumbOpen, ringOpen, littleOpen;
    int accXRaw, accYRaw, accZRaw;
    int rotXRaw, rotYRaw, rotYRaw;

}

// Main update function
void GyroGlove::update() {





}

// Check for a gesture (main array form)
bool GyroGlove::did(Gestures gestures[]) {



}

// Check for a gesture (non-array redirect)
bool GyroGlove::did(Gestures gesture) { this.did({gesture}); }

// Setters for settings
void GyroGlove::setThumb(int pin)                   { this.thumbPin = pin; }
void GyroGlove::setIndex(int pin)                   { this.indexPin = pin; }
void GyroGlove::setMiddle(int pin)                  { this.middlePin = pin; }
void GyroGlove::setRing(int pin)                    { this.ringPin = pin; }
void GyroGlove::setLittle(int pin)                  { this.littlePin = pin; }
void GyroGlove::setOutput(bool output)              { this.shouldOutput = output; }
void GyroGlove::setRate(int rate)                   { this.baudRate = rate; }
void GyroGlove::setTimeout(int timeout)             { this.timeoutIterations = timeout; }
void GyroGlove::setLEDConnected(bool connected)     { this.ledConnected = connected; }

// Getters for the finger values
bool GyroGlove::getIndexOpen()  { return indexOpen; }
bool GyroGlove::getThumbOpen()  { return thumbOpen; }
bool GyroGlove::getMiddleOpen() { return middleOpen; }
bool GyroGlove::getRingOpen()   { return ringOpen; }
bool GyroGlove::getLittleOpen() { return littleOpen; }

// Getters for the scaled values
int GyroGlove::getXAccel()  { return accX; }
int GyroGlove::getYAccel()  { return accY; }
int GyroGlove::getZAccel()  { return accZ; }
int GyroGlove::getXRot()    { return rotX; }
int GyroGlove::getYRot()    { return rotY; }
int GyroGlove::getZRot()    { return rotZ; }

// Getters for the raw values
int GyroGlove::getXAccelRaw() { return accXRaw; }
int GyroGlove::getYAccelRaw() { return accYRaw; }
int GyroGlove::getZAccelRaw() { return accZRaw; }
int GyroGlove::getXRotRaw()   { return rotXRaw; }
int GyroGlove::getYRotRaw()   { return rotYRaw; }
int GyroGlove::getZRotRaw()   { return rotZRaw; }
