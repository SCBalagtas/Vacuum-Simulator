/*

Student name: Steven Balagtas
Student number: n9998250
Unit: CAB202 Semester 1, 2019

This is the main file for the Robot Vacuum Cleaner simulator.

*/

// Import .h files
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <cab202_graphics.h>
#include <cab202_timers.h>
#include "terminal.h"

// Global variables

// Draw all objects in the simulation.
void draw_all() {
    clear_screen();
    draw_terminal();
    show_screen();
}

// Setup all objects in the simulation.
void setup( void ) {
    draw_all();
}

// Run the program.
int main( void ) {
    setup_screen();
    setup();

    while (1) {

    }

    return 0;
}