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
