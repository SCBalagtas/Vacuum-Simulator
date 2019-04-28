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
#include <limits.h>
#include <cab202_graphics.h>
#include <cab202_timers.h>
#include "draw.h"
#include "vacuum.h"
#include "timer.h"
#include "rubbish.h"

// Global variables.
bool simulation_over = false;
bool paused = true;
int DELAY; // in milliseconds.
int timeout = INT_MAX;
bool rtb_mode = false;

// Setup all objects in the simulation.
// Setup vacuum and charger first, so that rubbish can check whether they will overlap either
// the vacucum or the charger. 
void setup( void ) {
    DELAY = 10;
    paused = true;
    rtb_mode = false;
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
void pause() {
    paused = !paused;
    start_battery_timer();
}

// A timeout function which will ask for how long the programs runs before exiting automatically.
void do_timeout() {
    timeout = get_int( "How many seconds shall the program run?" );
}

// Returns true iff the simulation has been running for longer than the current designated time-out value.
bool timed_out() {
    return get_current_time() - get_start_time() >= timeout;
}

// A quit function which displays a farewell message then exits gracefully after user input.
void do_quit() {
    // Draw farewell message here.
    draw_farewell_message();
    while (!simulation_over) {
        int ch = get_char();
        if ( ch >= ' ' ) {
            simulation_over = true;
        }
    }
}

// A function to pause the simulation and bring up the help screen.
void do_help_screen() {
    // Bool variable to see whether simulation was moving before help screen.
    bool was_moving;
    // If we were not paused.
    if (!paused) {
        // Pause the simulation.
        pause();
        // Set was_moving to true.
        was_moving = true;
    } 
    clear_screen();
    draw_help_screen();
    bool return_to_game = false;
    while (!return_to_game) {
        int ch = get_char();
        if ( ch >= ' ' ) {
            return_to_game = true;
        }
    }
    // If we were moving before and then paused to display the help screen.
    if (paused && was_moving) {
        // Resume the simulation.
        pause();
    } 
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
        do_quit();
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
    else if ( ch == 'd' ) {
        add_dust();
        // Reset battery timer.
        start_battery_timer();
    }
    else if ( ch == 's' ) {
        add_slime();
        // Reset battery timer.
        start_battery_timer();
    }
    else if ( ch == 't' ) {
        add_trash();
        // Reset battery timer.
        start_battery_timer();
    }
    else if ( ch == 'm' ) {
        DELAY = get_int( "New delay in milliseconds?" );
        if (DELAY < 0) {
            DELAY = 10;
        }
        // Reset battery timer.
        start_battery_timer();
    }
    else if ( ch == 'o' ) {
        do_timeout();
        // Reset battery timer.
        start_battery_timer();
    }
    else if ( ch == 'v' ) {
        vacuum_hack();
        // Reset battery timer.
        start_battery_timer();
    }
    else if ( ch == 'w' ) {
        load_hack();
        // Reset battery timer.
        start_battery_timer();
    }
    else if (ch == 'b') {
        rtb_mode = true;
    }
    else if ( ch == '?' ) {
        do_help_screen();
    }
}

// A loop function which is called when vacuum battery is completely depleted. Give user option
// to quit or reset the simulation.
void battery_depleted() {
    bool reset_sim = false;

    while (!simulation_over && !reset_sim) {
        int ch = get_char();
        if (ch == 'q') {
            do_quit(); // End the simulation.
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
        // Vacuum mode.
        if (is_battery() && !rtb_mode) {
            update_vacuum();
            if (get_current_load() <= get_rtb_load_trigger() && get_battery() > get_rtb_battery_trigger() && !rtb_mode) {
                collect_rubbish();
            }
            else {
                // Go return to base mode.
                rtb_mode = true;
            }
        }
        // Return to base mode.
        else if (is_battery() && rtb_mode) {
            if (!is_docked()) {
                return_to_base();
                update_vacuum(); 
            }
            // Docked mode.
            else {
                // Once battery charges to 100% undock and turn off return to base mode.
                if (get_battery() != get_max_battery()) {
                    docked_mode();
                }
                else {
                    toggle_docked();
                    rtb_mode = false;
                }
            }
        }
        else {
            // Reaches here if battery is 0.
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

    while (!simulation_over && !timed_out()) {
        loop();
        timer_pause(DELAY);
    }

    return 0;
}