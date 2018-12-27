# gyroGlove

gyroGlove is a data glove for controlling Arduino based projects

Features:

- can detect hand motion/rotation
- can detect finger movements
- can provide feedback via an RGB LED
- the parts only cost around £20 total

[GIF of the glove]

### Parts required

- 6 MPU6050 accelerometer/gyroscope modules
- 2 pairs of medium thickness gloves
- some 24 AWG wire in whatever colours
- male to male jumper cables
- an RGB LED (common anode)
- 3 resistors (roughly 221 ohm)
- a sewing kit
- some tape of some kind

Also in order to actually use it you'll obviously need an Arduino (or equivalent)

### Assembling the glove

Assembling the gyroGlove requires soldering, if you are unable to solder I recommend learning to since it's a really useful skill! Alternatively you could attempt to hold the wire connections together with tape, but that's gonna be real flimsy and I really don't recommend it

1. Cut the wires to the right sizes and strip:

 [PIC]

1. Solder the wires to the gyroscopes:

 [PIC]

1. Solder the wires to the connectors:

 [PIC]

1. Solder certain wires together:

 [PIC]

1. Wrap the wires together and then tape at the ends:

 (other cable management methods are available)

  [PIC]

1. Sew the accelerometers into one of the gloves:

 [PIC]

### Connecting to an Arduino

To connect the data glove to an Arduino plug the pins into the correct headers:

- The power wire -> 5V
- The ground wire -> GND
- The SDA wire -> SDA
- The SDL wire -> SDL
- The thumb wire -> 1
- The index finger wire -> 2
- The middle finger wire -> 3
- The ring finger wire -> 4
- The little finger wire -> 5

### Using gyroglovelib

gyroglovelib is the Arduino library used to control the gyro glove

Basic usage:

```c++
#include <gyroglovelib.h>

GyroGlove glove;

void setup() {}

void loop() {

    // Get and process all the data from the gyros (ALWAYS REQUIRED)
    glove.update();

    // If the user just closed their index finger, set the LED to blue
    if (glove.didIndexClose()) {glove.setLED(0, 0, 255)}

    // If the user just opened their index finger, set the LED to red
    else if (glove.didIndexOpen()) {glove.setLED(255, 0, 0)}

    // If the user is waving, set the LED to green
    else if (glove.getXRot() >= 5 || glove.getXRot() <= -5) {glove.setLED(0, 255, 0)}

}
```

List of main functions:

```c++
// Get the scaled acceleration/rotation value in a direction (x/y/z)
// 1 is a very tiny move, 10 is a massive gesture
int accX = glove.getXAccel();
int rotX = glove.getXRot();

// Get the open or close state of a finger (thumb/index/middle/ring/little)
bool isIndexOpen = glove.getIndexOpen()

// Did a certain finger just close? (thumb/index/middle/ring/little)
bool indexWasClosed = glove.didIndexClose()
bool indexWasOpened = glove.didIndexOpen()

// Change the LED colour (RGB values from 0 -> 255)
glove.setLED(0, 0, 255)

// Get the raw accelerometer/gyroscope values in a direction (x/y/z)
int accXRaw = glove.getXAccelRaw();
int rotXRaw = glove.getXRotRaw();

```

Optional settings, can be changed in either setup() or loop(), defaults are shown:

```c++
// Whether to output a data string to Serial for use with ggmonitor
glove.setOutput(false);

// Set the baud rate of Serial output
glove.setRate(9600);

// Change which pins are for which gyro, setting to -1 disables
glove.setThumb(1);
glove.setIndex(2);
glove.setMiddle(3);
glove.setRing(4);
glove.setLittle(5);

// Whether an LED is connected
glove.setLEDConnected(true);
```


### Debugging using ggmonitor

ggmonitor is a cross-platform Python GUI tool allowing the calibration and testing of a gyro glove

To use, first ensure gyroglovelib is initialised correctly and that the following line is in the setup code of the Arduino:

```c++
glove.setOutput(true)
```

Then run the ggmonitor executable (the ".exe" if on Windows, the ".sh" if on Linux), which will bring up a window

Then connect the Arduino via a USB cable and ggmonitor should realise and start displaying data
