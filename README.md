# gyroGlove

gyroGlove is a data glove for controlling Arduino based projects

Features:

- can detect hand movements/rotation
- can detect finger movements
- can provide visual feedback via an RGB LED
- the parts only cost around £20 total
- easy to use Arduino library

Example Arduino code:

```c++
#include <gyroglovelib.h>
GyroGlove glove;

void setup() {}
void loop() {

    // Get and process all the data from the glove
    glove.update();

    // If the user is waving, set the LED to green
    if (glove.did(Gestures::wave)) { glove.setLED(0, 255, 0); }

    // If the user has only their middle and ring fingers closed, set to red
    else if (glove.did(Gestures::rockRoll)) { glove.setLED(255, 0, 0); }

}
```

[GIF of the glove]

### Parts Required

- 6 MPU6050 accelerometer/gyroscope modules
- 2 pairs of medium thickness gloves
- some 24 AWG wire in whatever colours
- male to male jumper cables
- an RGB LED (optional)
- 3 220 Ohm resistors (only needed if using an LED)

Although in order to actually use it you'll obviously need an Arduino (or similar)

### Assembling the Glove

Assembling the gyroGlove requires soldering, if you can't solder I recommend learning since it's a really useful skill. Alternatively you could attempt to hold the wire connections together with tape, but that's gonna be real flimsy and I don't recommend it

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

1. Attach the accelerometers to one of the gloves:

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

Main function list:

```c++
// Was a certain finger gesture just completed?
bool didWave = glove.did(Gestures::wave)
bool didTapIndex = glove.did({Gestures::indexClose, Gestures::indexOpen})

// Get the acceleration/rotation values (array: {x, y, z}), scaled from 1 -> 10
int[] acc = glove.getAccel();
int[] rot = glove.getRot();

// Change the LED colour (RGB values from 0 -> 255)
glove.setLED(0, 0, 255)

// Get whether a finger is open (thumb/index/middle/ring/little)
bool isIndexOpen = glove.getIndexOpen()

// Get the raw accelerometer/gyroscope values (array: {x, y, z})
int[] accRaw = glove.getAccelRaw();
int[] rotRaw = glove.getRotRaw();
```

Optional settings, can be changed in either setup() or loop(), defaults are shown:

```c++
// Set whether to output a data string to Serial for use with ggmonitor
glove.setOutput(false);

// Set the baud rate of Serial output
glove.setRate(9600);

// Set which pins are for which gyro, setting to -1 disables
glove.setHand(1);
glove.setThumb(2);
glove.setIndex(3);
glove.setMiddle(4);
glove.setRing(5);
glove.setLittle(6);

// Set whether an LED is connected
glove.setLEDConnected(true);

// Set which pins are for the LED, setting to -1 disables
glove.setRed(7);
glove.setGreen(8);
glove.setBlue(9);

// Set how long to wait before ending a gesture
glove.setTimeout(50);
```

### Supported Gesture

### Debugging Using ggmonitor

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
