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
#include "rubbish.h"

// Global variables.
bool simulation_over = false;
bool paused = true;
const int DELAY = 10; // in milliseconds.

// Setup all objects in the simulation.
// Setup vacuum and charger first, so that rubbish can check whether they will overlap either
// the vacucum or the charger. 
void setup( void ) {
    paused = true;
    setup_charger();
    setup_vacuum();
    setup_rubbish();
    draw_all();
    start_timer();
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
    if (ch == 'q') {
        simulation_over = true;
    }
    else if ( ch == 'r' ) {
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

// A loop function which is called when vacuum battery is completely depleted. Give user option
// to quit or reset the simulation.
void battery_depleted() {
    bool reset_sim = false;

    while (!simulation_over && !reset_sim) {
        int ch = get_char();
        if (ch == 'q') {
            simulation_over = true; // End the simulation.
        }
        else if ( ch == 'r' ) {
            reset();
            reset_sim = true; // Don't end sim, just reset and go back to main loop.
        }
    }
}

// The main loop function, runs the simulation.
void loop() {
    // Implement loop here...
    int ch = get_char();
    if ( ch >= ' ' ) {
        do_operation( ch );
    }
    if (!paused) {
        if (is_battery()) {
            update_vacuum();
            collect_rubbish();
        }
        else {
            // Reaches here if battery is 0.
            // Draw one last frame of the simulation.
            draw_all();
            // Draw battery depleted message here.
            draw_simulation_over();
            // Run the battery depleted loop.
            battery_depleted();
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