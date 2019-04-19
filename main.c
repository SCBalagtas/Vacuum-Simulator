/*

Student name: Steven Balagtas
Student number: n9998250
Unit: CAB202 Semester 1, 2019

This file contains the main function for the Robot Vacuum simulator.

*/

// Import .h files
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <cab202_graphics.h>
#include <cab202_timers.h>
#include "draw.h"

// Global variables

// Setup all objects in the simulation.
void setup( void ) {
    draw_all();
}

// A reset function to reset the simulation.
void reset() {
    setup();
}

// The main loop function, runs the simulation.
void loop() {
    // Implement loop here...
}

// Run the program.
int main( void ) {
    setup_screen();
    setup();

    while (1) {
        loop();
    }

    return 0;
}