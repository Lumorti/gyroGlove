# gyroGlove

gyroGlove is a data glove for controlling Arduino based projects

Features:

- can detect hand movements/rotation
- can detect finger movements
- can provide visual feedback via an RGB LED
- the parts only cost around £20 total
- easy to use Arduino library

[GIF of the glove]

Arduino code used above:

```c++
#include <gyroglovelib.h>
GyroGlove glove;

void setup() {

    // Initialise the glove
    glove.init();

}

void loop() {

    // Get all the data from the glove
    glove.update();

    // If the user is waving, set the LED to green
    if (glove.did(Gestures::WAVE)) {glove.setLED("g");}

    // If the user has only their middle and ring fingers closed, set to red
    else if (glove.did(Gestures::ROCKANDROLL)) {glove.setLED("r");}

}
```

### Parts Required

- 6 MPU6050 accelerometer/gyroscope modules
- Arduino Pro Mini (or equivalent)
- 2 pairs of medium thickness gloves
- some 24 AWG wire in whatever colours
- male to male jumper cables
- an RGB LED (optional)
- 3 220 Ohm resistors (only needed if using an LED)

Although in order to actually use it you'll obviously need an Arduino (or similar)

### Assembling the Glove

Assembling the gyroGlove requires soldering, if you can't solder I recommend learning since it's a really useful skill. Alternatively you could attempt to hold the wire connections together with tape, but that's gonna be real flimsy and I don't recommend it

![Wiring guide](https://github.com/Lumorti/gyroGlove/raw/master/images/circuits.png "Wiring guide")

### Connecting to an Arduino

To connect the data glove to an Arduino plug the wires into their respective headers:

- The power wire -> 5V
- The ground wire -> GND
- The SDA wire -> SDA
- The SDL wire -> SDL

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

Optional settings, should be changed in setup(), defaults are shown:

```c++
// Set whether to output a data string to Serial for use with ggmonitor
glove.setOutput(false);

// Set the baud rate of Serial output
glove.setRate(9600);

// Set how many iterations to wait before ending a gesture
glove.setTimeout(10);
```

### Supported Gestures

### Debugging Using ggmonitor

ggmonitor is a cross-platform Python GUI tool allowing the calibration and testing of a gyro glove

There are some dependencies, however most should already be installed if python is:

- python3
- pyserial
- tkinter
- PIL plus the tkinter imagetk package
- numpy
- matplotlib

On Ubuntu/Debian these can be installed with apt:

```sh
sudo apt update
sudo apt install python3 python3-matplotlib python3-numpy python3-serial python3-tk python3-pil python3-pil.imagetk
```

On Windows these can be installed with pip after installing python3:

```sh
pip3 install --upgrade matplotlib numpy serial tkinter pil
```

Ensure that the following lines are in their respective sections in the code of the Arduino:

```c++
#include <gyroglovelib.h>
GyroGlove glove;

void setup() {

    // Initialise the glove
    glove.init();
    glove.setOutput(true)

}

void loop() {

    // Get all the data from the glove
    glove.update();

}
```

Run the ggmonitor executable ("ggmonitor.py") on your computer, which will create a ggmonitor window

Then connect the Arduino via a USB cable and ggmonitor should detect it and start displaying data
