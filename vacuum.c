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

// Returns true iff the supplied argument is a vacuum navigation control.
bool is_vacuum_ctrl( int ch ) {
    return ( ch == 'j' || ch == 'k' || ch == 'l' || ch == 'i' );
}

/**
 *  Update the vacuum location based on the supplied argument.
 *
 *  Parameters:
 *      ch: a character code which is expected to be one of [ijkl].
 *          'j' -> move left
 *          'k' -> move down
 *          'l' -> move right
 *          'i' -> move up
 *
 *  Notes:
 *      If the designated move would cause the bounding box of the vacuum
 *  icon to overlap the border then the move is ignored.
 */
void manual_update_vacuum( int ch ) {
    if ( ch == 'j' && vac_x - VACUUM_WIDTH / 2 > 1 ) {
        vac_x--;
    }
    else if ( ch == 'l' && vac_x + VACUUM_WIDTH / 2 < screen_width() - 2 ) {
        vac_x++;
    }
    else if ( ch == 'k' && vac_y + VACUUM_HEIGHT / 2 < screen_height() - 4 ) {
        vac_y++;
    }
    else if ( ch == 'i' && vac_y - VACUUM_HEIGHT / 2 > 5 ) {
        vac_y--;
    }
}