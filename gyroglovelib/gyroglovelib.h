/*
    gyroglovelib.h - Library for interfacing with a gyro glove
    https://github.com/Lumorti/gyroGlove
*/

#ifndef gglib_h
#define gglib_h

#include "Arduino.h"

enum Gestures {

    thumbClose,
    indexClose,
    middleClose,
    ringClose,
    littleClose,

    thumbOpen,
    indexOpen,
    middleOpen,
    ringOpen,
    littleOpen,

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
        void setTimeout(int timeout);
        void setLEDConnected(bool connected);

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

};

#endif
