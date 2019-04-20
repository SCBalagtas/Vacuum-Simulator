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
#include "vacuum.h"

// Global variables
bool simulation_over = false;
bool paused = true;
const int DELAY = 10; // in milliseconds

// Setup all objects in the simulation.
void setup( void ) {
    setup_vacuum();
    draw_all();
}

// A reset function to reset the simulation.
void reset() {
    setup();
}

// The main loop function, runs the simulation.
void loop() {
    // Implement loop here...
    int ch = get_char();
    if (ch == 'q') {
        simulation_over = true;
        return;
    }
    if (!paused) {
        // Update vacuum here...
    }

    draw_all();
}

// Run the program.
int main( void ) {    
    setup_screen();
    setup();

    while (!simulation_over) {
        loop();
        timer_pause(DELAY);
    }

    return 0;
}