#include <GyroGlove.h>
GyroGlove glove;

void setup() {

    // Initialise the glove
    glove.init();
    glove.setOutput(true);

    // Set the light to magenta initially
    glove.setLED('m');

}

void loop() {

    // Get all the data from the glove
    glove.update();

    // If the thumb is open, make it green, otherwise blue
    if(glove.getThumbOpen() == true){glove.setLED('g');}
    else{glove.setLED('b');}

}
