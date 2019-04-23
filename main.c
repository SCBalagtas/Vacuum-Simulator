/*

Student name: Steven Balagtas
Student number: n9998250
Unit: CAB202 Semester 1, 2019

This file contains the main function for the Robot Vacuum simulator.

*/

// Import .h files.
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <cab202_graphics.h>
#include <cab202_timers.h>
#include "draw.h"
#include "vacuum.h"
#include "timer.h"

// Global variables.
bool simulation_over = false;
bool paused = true;
const int DELAY = 10; // in milliseconds

// Setup all objects in the simulation.
void setup( void ) {
    paused = true;
    start_timer();
    setup_vacuum();
    draw_all();
}

// A reset function to reset the simulation.
void reset() {
    setup();
}

// A pause function which flips the paused variable.
void pause () {
    paused = !paused;
    start_battery_timer();
}

/**
 *  Jump table which chooses the action (if any) which corresponds to a
 *  supplied command character.
 *
 *  Parameters:
 *      ch: a command character. Currently recognised comands are listed in
 *          the do_help function.
 */
void do_operation( int ch ) {
    if ( ch == 'r' ) {
        reset();
    }
    else if ( is_vacuum_ctrl( ch ) ) {
        manual_update_vacuum( ch );
    }
    else if ( ch == 'p' ) {
        pause();
    }
    else if ( ch == 'y' ) {
        battery_hack(get_int( "New battery level (0 - 100)?" ));
    }
}

// The main loop function, runs the simulation.
void loop() {
    // Implement loop here...
    int ch = get_char();
    if (ch == 'q') {
        simulation_over = true;
        return;
    }

    if ( ch >= ' ' ) {
        do_operation( ch );
    }

    if (!paused) {
        if (is_battery()) {
            update_vacuum();
        }
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