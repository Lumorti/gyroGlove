/*
    GyroGlove.h - Library for interfacing with a gyro glove
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

    THUMBTAP,
    INDEXTAP,
    MIDDLETAP,
    RINGTAP,
    LITTLETAP,

    WAVE,
    ROCKANDROLL,
    GUN,
    SHOCKER,

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
        void setScalingFactor(bool factor);
        void setFingerThresh(bool threshold);

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
        bool shouldOutput;
        int baudRate;
        int timeoutIterations;
        bool ledConnected;
        int scalingFactor;
        int fingerCloseThreshold;

        // Fixed params
        static const int chipAddress = 0x57;
        static const int gyroAddress = 0x69;
        static const int maxGestures = 100;

        // ledPins = {red, green, blue};
        int ledPins[];

        // Processed values
        int acc[];
        int rot[];
        Gestures gestureList[maxGestures];

        // Finger values = {thumbOpen, indexOpen, middleOpen, ringOpen, littleOpen}
        bool fingersClosed[];
        bool fingersClosedOld[];
        int fingerAccels[];

        // Raw values
        int accRaw[];
        int rotRaw[];
        int temperature;

        // Keep track of how many updates have occured since the last gesture
        int sinceLastGesture;

        // For keeping track of the gesture array
        int nextGestureIndex;
        int oldGestureIndex;

        // Add a gesture to the gesture list and update the counter
        void addGest(Gestures toAdd);

};

#endif
