/*

Student name: Steven Balagtas
Student number: n9998250
Unit: CAB202 Semester 1, 2019

This file contains function definitions for the timer that's not dependent on the main loop.

*/

// Import .h files.
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <cab202_timers.h>

// Global variables.
static int start_time;
static int elapsed_time;
static char timer[30];

// Start timer function that intiates start_time with get_current_time().
void start_timer() {
    start_time = round(get_current_time());
}

// Calculate elapsed time function that return an int of the elapsed time since last setup or reset.
int calc_elapsed_time() {
    elapsed_time = round(get_current_time()) - start_time;
    return elapsed_time;
}

// Format time function which formats a string to display the requirement for the elapsed time in the status
// display. Requires an int input which it will format into mm:ss.
void format_time(int time) {
    int minutes, seconds;

    minutes = time/ 60;
    seconds = time % 60;

    sprintf(timer, "Elapsed: %02d:%02d", minutes, seconds);
}

// Get elapsed time function which returns a string of the elapsed time in mm:ss format.
char * get_elapsed_time() {
    format_time(calc_elapsed_time());
    return timer;
}

// Return start_time.
int get_start_time() {
    return start_time;
}