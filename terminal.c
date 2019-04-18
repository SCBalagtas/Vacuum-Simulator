/*

Student name: Steven Balagtas
Student number: n9998250
Unit: CAB202 Semester 1, 2019

This file contains the function definitions for drawing the terminal.

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

// Draw the status display area as per specification.
void draw_status_display() {
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

// Draw the room which will contain the device and the rubbish.
void draw_room() {
    get_screen_size( &width, &height );
    draw_assignment_rect(0, 4, (width - 1), (height - 3), vertical, horizontal, corner);
}

// Draw the terminal window.
void draw_terminal() {
    draw_room();
    draw_status_display();
}