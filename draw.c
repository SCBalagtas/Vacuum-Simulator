/*

Student name: Steven Balagtas
Student number: n9998250
Unit: CAB202 Semester 1, 2019

This file contains the function definitions for drawing all objects in the simulation.

*/

// Import .h files
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <cab202_graphics.h>
#include "helpers.h"

// Global variables
static int width, height;
static int vertical = '|';
static int horizontal = '-';
static int corner = '+';

#define STUD_NUM "n9998250"


// Draw the status display area table as per specification.
void draw_status_table() {
    get_screen_size( &width, &height );
    // Draw top left cell.
    draw_assignment_rect(0, 0, ((width - 1)/ 3), 2, vertical, horizontal, corner);
    // Draw top middle cell.
    draw_assignment_rect(((width - 1)/ 3), 0, ((width - 1)/ 3 * 2), 2, vertical, horizontal, corner);
    // Draw top right cell.
    draw_assignment_rect(((width - 1)/ 3 * 2), 0, (width - 1), 2, vertical, horizontal, corner);
    // Draw bottom left cell.
    draw_assignment_rect(0, 2, ((width - 1)/ 3), 4, vertical, horizontal, corner);
    // Draw bottom middle cell.
    draw_assignment_rect(((width - 1)/ 3), 2, ((width - 1)/ 3 * 2), 4, vertical, horizontal, corner);
    // Draw bottom right cell.
    draw_assignment_rect(((width - 1)/ 3 * 2), 2, (width - 1), 4, vertical, horizontal, corner);
}

// Draw the status display area information as per specification.
void draw_status_information() {
    // Draw student number in the top left cell.
    draw_string(((width - 1)/ 6) - (strlen(STUD_NUM)/ 2), 1, STUD_NUM);
    // Draw device direction in the top middle cell.
    draw_string(((width - 1)/ 2) - (strlen(STUD_NUM)/ 2), 1, STUD_NUM); // REPLACE STUD_NUM WITH DEVICE DIRECTION
    // Draw device battery percentage in the top right cell.
    draw_string((((width - 1)/ 6) * 5) - (strlen(STUD_NUM)/ 2), 1, STUD_NUM); // REPLACE STUD_NUM WITH DEVICE BATTERY %
    // Draw elapsed time in the bottom left cell.
    draw_string(((width - 1)/ 6) - (strlen(STUD_NUM)/ 2), 3, STUD_NUM); // REPLACE STUD_NUM WITH ELAPSED TIME
    // Draw current load of device (in g) in the bottom middle cell.
    draw_string(((width - 1)/ 2) - (strlen(STUD_NUM)/ 2), 3, STUD_NUM); // REPLACE STUD_NUM WITH CURRENT LOAD
    // Draw rubbish available in the bottom right cell.
    draw_string((((width - 1)/ 6) * 5) - (strlen(STUD_NUM)/ 2), 3, STUD_NUM); // REPLACE STUD_NUM WITH AVAILABLE RUBBISH
}

// Draw the status display.
void draw_status_display() {
    draw_status_table();
    draw_status_information();
}

// Draw the room which will contain the device and the rubbish.
void draw_room() {
    get_screen_size( &width, &height );
    draw_assignment_rect(0, 4, (width - 1), (height - 3), vertical, horizontal, corner);
}

// Draw all objects in the simulation.
void draw_all() {
    clear_screen();
    draw_room();
    draw_status_display();
    show_screen();
}