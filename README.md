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

   [PIC]

1. Sew the accelerometers into one of the gloves:

   [PIC]

### Connecting to an Arduino

To connect the data glove to an Arduino plug the wires into their respective headers:

- The power wire -> 5V
- The ground wire -> GND
- The SDA wire -> SDA
- The SDL wire -> SDL
- The thumb wire -> 1
- The index finger wire -> 2
- The middle finger wire -> 3
- The ring finger wire -> 4
- The little finger wire -> 5

The finger wires can be moved around if need, but you'll also need to update the pins in the software

### Using gyroglovelib

gyroglovelib is the Arduino library used to control the gyro glove

Basic example:

```c++
#include <gyroglovelib.h>

GyroGlove glove;

void setup() {}

void loop() {

    // Get and process all the data from the gyros (ALWAYS REQUIRED)
    glove.update();

    // If the user just closed their index finger, set the LED to blue
    if (glove.didIndexClose()) {glove.setLED(0, 0, 255)}

    // If the user is waving, set the LED to green
    else if (glove.getXRot() >= 5 || glove.getXRot() <= -5) {glove.setLED(0, 255, 0)}

}
```

List of main functions:

```c++
// Get the acceleration/rotation value in a direction (x/y/z), scaled from 1 -> 10
int accX = glove.getXAccel();
int rotX = glove.getXRot();

// Get whether a finger is open (thumb/index/middle/ring/little)
bool isIndexOpen = glove.getIndexOpen()

// Did a certain finger just open/close? (thumb/index/middle/ring/little)
bool indexWasOpened = glove.didIndexOpen()
bool indexWasClosed = glove.didIndexClose()

// Was a certain finger gesture just completed? (see gesture section)
bool gestureCompleted = glove.didGesture([1, 1, 3])

// Change the LED colour (RGB values from 0 -> 255)
glove.setLED(0, 0, 255)

// Get the raw accelerometer/gyroscope values in a direction (x/y/z)
int accXRaw = glove.getXAccelRaw();
int rotXRaw = glove.getXRotRaw();

```

Optional settings, can be changed in either setup() or loop(), defaults are shown:

```c++
// Set whether to output a data string to Serial for use with ggmonitor
glove.setOutput(false);

// Set the baud rate of Serial output
glove.setRate(9600);

// Set which pins are for which gyro, setting to -1 disables
glove.setThumb(1);
glove.setIndex(2);
glove.setMiddle(3);
glove.setRing(4);
glove.setLittle(5);

// Set how long to wait before ending a gesture
glove.setTimeout(50);

// Set whether an LED is connected
glove.setLEDConnected(true);
```


### Debugging using ggmonitor

ggmonitor is a cross-platform Python GUI tool allowing the calibration and testing of a gyro glove

There are some dependencies, however most should already be installed if python is:

- python3
- pyserial
- tkinter
- PIL plus the tkinter imagetk package
- numpy
- matplotlib

On Ubuntu these can be installed with apt:

```sh
sudo apt update
sudo apt install python3 python3-matplotlib python3-numpy python3-serial python3-tk python3-pil python3-pil.imagetk
```

On Windows these can be installed with pip after installing python3:

```sh
pip3 install --upgrade matplotlib numpy serial tkinter pil
```

Ensure gyroglovelib is initialised correctly and that the following line is in the setup code of the Arduino:

```c++
glove.setOutput(true)
```

Then run the ggmonitor executable ("ggmonitor.py"), which will bring up a window

Then connect the Arduino via a USB cable and ggmonitor should detect it and start displaying data
