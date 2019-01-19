/*
    GyroGlove.h - Library for interfacing with a gyro glove
    https://github.com/Lumorti/gyroGlove
*/

#ifndef gglib_h
#define gglib_h

#include "Arduino.h"

enum Gestures {

    // Close gestures
    THUMBCLOSE,
    INDEXCLOSE,
    MIDDLECLOSE,
    RINGCLOSE,
    LITTLECLOSE,

    // Open gestures
    THUMBOPEN,
    INDEXOPEN,
    MIDDLEOPEN,
    RINGOPEN,
    LITTLEOPEN,

    // Rotations
    ROTNEGZ,
    ROTPOSZ,
    ROTNEGX,
    ROTPOSX,
    ROTNEGY,
    ROTPOSY,

    // Accelerations
    ACCNEGZ,
    ACCPOSZ,
    ACCNEGX,
    ACCPOSX,
    ACCNEGY,
    ACCPOSY,

    // Tap = close + open
    THUMBTAP,
    INDEXTAP,
    MIDDLETAP,
    RINGTAP,
    LITTLETAP,

    // WAVE = ROTNEGZ, ROTPOSZ
    WAVE,

    // ROCKANDROLL = (LITTLEOPEN, INDXOPEN) || (MIDDLECLOSE, RINGCLOSE)
    ROCKANDROLL,

    // GUN = (LITTLECLOSE, RINGCLOSE) || (INDXOPEN, MIDDLEOPEN)
    GUN,

    // MIDDLEFINGER = MIDDLEOPEN
    MIDDLEFINGER,

    // HANDCLOSE = INDEXCLOSE, MIDDLECLOSE, RINGCLOSE, LITTLECLOSE
    HANDCLOSE,

    // HANDOPEN = INDEXOPEN, MIDDLEOPEN, RINGOPEN, LITTLEOPEN
    HANDOPEN,

};

class GyroGlove {

    public:

        // Class constructor
        GyroGlove();

        // Initliasing function
        void init();

        // Main update function
        void update();

        // Check for a gesture
        bool did(Gestures gestures[]);
        bool did(Gestures gesture);

        // Change the LED
        void setLED(char colour);

        // Setters for settings
        void setRate(int rate);
        void setTimeout(int timeout);
        void setLEDConnected(bool connected);
        void setScalingFactor(float factor);
        void setFingerThresh(int threshold);

        void setOutput(bool output);

        // Getters for the scaled values
        int * getAccel();
        int * getRot();

        // Getters for the raw values
        int * getAccelRaw();
        int * getRotRaw();

        // Getters for the finger values
        bool * getFingerState();
        bool getIndexOpen();
        bool getThumbOpen();
        bool getMiddleOpen();
        bool getRingOpen();
        bool getLittleOpen();

    private:

        // Settings
        bool shouldOutput = false;
        int baudRate = 9600;
        int timeoutIterations = 10;
        bool ledConnected = true;
        int rotThreshold = 10000;
        int accThreshold = 3000;
        float scalingFactor = 1.0;

        // Fixed params
        static const int chipAddress = 0x57;
        static const int gyroAddress = 0x69;
        static const int maxGestures = 10;
        static const int timeBetween = 20;

        // Keep track of the LED colour char
        char ledCol = 'l';

        // Processed values
        int acc[3] = {0, 0, 0};
        int rot[3] = {0, 0, 0};

        // Finger values = {thumbOpen, indexOpen, middleOpen, ringOpen, littleOpen}
        bool fingersClosed[5] = {false, false, false, false, false};
        bool fingersClosedOld[5] = {false, false, false, false, false};
        int16_t fingerAccRaw[5] = {0, 0, 0, 0, 0};

        // Raw values
        int16_t accRaw[3] = {0, 0, 0};
        int16_t rotRaw[3] = {0, 0, 0};
        int16_t temperature = 0;

        // Keep track of how many updates have occured since the last gesture
        int sinceLastGesture = 0;

        // For keeping track of the gesture array
        Gestures gestureList[maxGestures];
        int nextGestureIndex = 0;
        int oldGestureIndex = 0;

        // Add a gesture to the gesture list and update the counter
        void addGest(Gestures toAdd);

};

#endif
