/*

Student name: Steven Balagtas
Student number: n9998250
Unit: CAB202 Semester 1, 2019

This file contains the function definitions for everything related to the Robot Vacuum cleaner.

*/

// Import .h files.
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <cab202_graphics.h>
#include "helpers.h"

// Global variables.
// Vacuum state
#define VACUUM_WIDTH 9
#define VACUUM_HEIGHT 9

static double vac_x, vac_y;

static char * vacuum =
"  @@@@@  "
" @@@@@@@ "
"@@@@@@@@@"
"@@@@@@@@@"
"@@@@@@@@@"
"@@@@@@@@@"
"@@@@@@@@@"
" @@@@@@@ "
"  @@@@@  "
;

// Draw the vacuum at the center of (vac_x, vac_y).
void draw_vacuum() {
    int left = round(vac_x) - VACUUM_WIDTH/ 2;
    int top = round(vac_y) - VACUUM_HEIGHT/ 2;
    
    draw_pixels(left, top, VACUUM_WIDTH, VACUUM_HEIGHT, vacuum, true);
}

// Setup vacuum at the center of the room.
void setup_vacuum() {
    int width, height;
    get_screen_size( &width, &height );
    vac_x = (width - 1)/ 2;
    vac_y = ((height - 7)/ 2) + 4; // This accounts for the non-room area.
}