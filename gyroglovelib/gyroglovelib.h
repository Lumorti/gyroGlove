/*
    gyroglovelib.h - Library for interfacing with a gyro glove
    https://github.com/Lumorti/gyroGlove
*/

#ifndef gglib_h
#define gglib_h

#include "Arduino.h"

enum Gestures {

    THUMBCLOSE,
    INDEXCLOSE,
    MIDDLECLOSE,
    RINGCLOSE,
    LITTLECLOSE,

    THUMBOPEN,
    INDEXOPEN,
    MIDDLEOPEN,
    RINGOPEN,
    LITTLEOPEN,

};

class GyroGlove {

    public:

        // Class constructor
        GyroGlove();

        // Main update function
        void update();

        // Check for a gesture
        bool did(Gestures gestures[]);
        bool did(Gestures gesture);

        // Setters for settings
        void setOutput(bool output);
        void setRate(int rate);
        void setThumb(int pin);
        void setIndex(int pin);
        void setMiddle(int pin);
        void setRing(int pin);
        void setLittle(int pin);
        void setHand(int pin);
        void setTimeout(int timeout);
        void setLEDConnected(bool connected);
        void setFingersZOnly(bool optimise);
        void setScalingFactor(bool factor);

        // Getters for the scaled values
        int getXAccel();
        int getYAccel();
        int getZAccel();
        int getXRot();
        int getYRot();
        int getZRot();

        // Getters for the raw values
        int getXAccelRaw();
        int getYAccelRaw();
        int getZAccelRaw();
        int getXRotRaw();
        int getYRotRaw();
        int getZRotRaw();
        bool getIndexOpen();
        bool getThumbOpen();
        bool getMiddleOpen();
        bool getRingOpen();
        bool getLittleOpen();

    private:

        // Settings
        bool shouldOutput;
        int baudRate;
        int handPin, thumbPin, indexPin, middlePin, ringPin, littlePin;
        int timeoutIterations;
        bool ledConnected;
        bool fingersZOnly;
        int scalingFactor;

        // Processed values
        int accX, accY, accZ;
        int rotX, rotY, rotZ;
        Gestures gestureList[];

        // Finger values
        bool indexOpen, middleOpen, thumbOpen, ringOpen, littleOpen;
        bool indexOpenOld, middleOpenOld, thumbOpenOld, ringOpenOld, littleOpenOld;

        // Raw values
        int accXRaw, accYRaw, accZRaw;
        int rotXRaw, rotYRaw, rotYRaw;

        // Add a gesture to the gesture list and update the counter
        void addGest(Gesture toAdd);

};

#endif
