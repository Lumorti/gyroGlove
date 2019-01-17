#include <GyroGlove.h>
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
