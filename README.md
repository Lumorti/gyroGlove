# gyroGlove

gyroGlove is a data glove for controlling Arduino based projects

Features:

- can detect hand movements/rotation
- can detect finger movements
- can provide visual feedback via an RGB LED
- the parts only cost around £20 total
- easy to use Arduino library

![GIF of the glove](https://github.com/Lumorti/gyroGlove/raw/master/images/example.gif "GIF of the glove")

Arduino code used above:

```c++
#include <GyroGlove.h>
GyroGlove glove;

void setup() {

    // Initialise the glove
    glove.init();
    glove.setOutput(true);

}

void loop() {

    // Get all the data from the glove
    glove.update();

    // If the thumb is open, make it blue, otherwise green
    if (glove.getThumbOpen()){glove.setLED('b');}
    else {glove.setLED('g');}

}

```

### Parts Required

- 6 MPU6050 accelerometer/gyroscope modules
- Arduino Pro Mini (or equivalent)
- a medium thickness glove
- some 24 AWG wire in whatever colours (preferable red/black/yellow/green/blue)
- male to female jumper cables
- an RGB LED
- 3 100 Ohm resistors

You'll also need another Arduino to process the data output by the Arduino Pro Mini (which is located on the back of the glove for faster processing and wiring simplifcation). This is as simple as just plugging the correct four wires from the gyroGlove into an Arduino and then using the gyroGlove library.

### Assembling the Glove

![Wiring guide](https://github.com/Lumorti/gyroGlove/raw/master/images/circuits.png "Wiring guide")

### Connecting to an Arduino

To connect the gyroGlove to an Arduino plug the wires into their respective headers:

- The power wire (red)    -> 5V
- The ground wire (black) -> GND
- The SDA wire (yellow)   -> SDA
- The SDL wire (green)    -> SDL

### Using gyroglovelib

"gyroglovelib" is the Arduino library used to control the gyro glove.

Main function list:

```c++
// Was a certain finger gesture just completed? TODO
bool didWave = glove.did(Gestures::wave)
bool didTapIndex = glove.did(2, {Gestures::indexClose, Gestures::indexOpen})

// Get the acceleration/rotation values (array: {x, y, z}), scaled from 1 -> 10
int[] acc = glove.getAccel();
int[] rot = glove.getRot();

// Change the LED colour (chars representing the colours)
glove.setLED('r');

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

### Debugging Using ggmonitor

"ggmonitor" is a cross-platform Python GUI tool allowing the calibration and testing of a gyro glove.

There are some dependencies, however most should already be installed if Python is:

- python3
- pyserial
- tkinter
- PIL plus the tkinter imagetk package
- numpy
- matplotlib

On Ubuntu/Debian these can be installed with apt as follows:

```sh
sudo apt update
sudo apt install python3 python3-matplotlib python3-numpy python3-serial python3-tk python3-pil python3-pil.imagetk
```

On Windows these can be installed with pip after installing python3:

```sh
pip3 install matplotlib numpy serial tkinter pil
```

An example code snippet which will allow debugging using ggmonitor:

```c++
#include <GyroGlove.h>
GyroGlove glove;

void setup() {

    // Initialise the glove
    glove.init();

    // Tell it to output the data to serial for ggmonitor
    glove.setOutput(true);

}

void loop() {

    // Get all the data from the glove
    glove.update();

}
```

Run the ggmonitor executable ("./ggmonitor.py") on your computer, which will create a ggmonitor window, then connect the Arduino via a USB cable and ggmonitor should detect it and start displaying data. If it doesn't detect it, try plugging it in before starting the program instead.
