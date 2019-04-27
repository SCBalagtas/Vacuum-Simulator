/*

Student name: Steven Balagtas
Student number: n9998250
Unit: CAB202 Semester 1, 2019

This file contains the function definitions for everything related to rubbish.

*/

// Import .h files.
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <cab202_graphics.h>
#include <cab202_timers.h>
#include "vacuum.h"
#include "helpers.h"

// Global variables.
static char rubbish_status[40];

// Dust variables.
#define MAX_DUST 1000
#define DUST_WEIGHT 1 // in grams.
#define DUST_WIDTH 1
#define DUST_HEIGHT 1

static int num_dust;
static double dust_x[MAX_DUST], dust_y[MAX_DUST];

static char * dust =
"."
;

// Slime variables.
#define MAX_SLIME 10
#define SLIME_WEIGHT 5 // in grams.
#define SLIME_WIDTH 5
#define SLIME_HEIGHT 5

static int num_slime;
static double slime_x[MAX_SLIME], slime_y[MAX_SLIME];

static char * slime = 
" ~~~ "
"~~~~~"
"~~~~~"
"~~~~~"
" ~~~ "
;

// Trash variables.
#define MAX_TRASH 5
#define TRASH_WEIGHT 20 // in grams.
#define TRASH_WIDTH 11
#define TRASH_HEIGHT 6

static int num_trash;
static double trash_x[MAX_TRASH], trash_y[MAX_TRASH];

static char * trash =
"     &     "
"    &&&    "
"   &&&&&   "
"  &&&&&&&  "
" &&&&&&&&& "
"&&&&&&&&&&&"
;

/* The following are bool functions which return true if pixel collision is detected between two objects.
*  The objects that are being checked are documented.      
*/
// Return true iff dust will overlap vacuum.
bool dust_hit_vacuum(int dust_ID) {
    return pixel_collision(dust_x[dust_ID], dust_y[dust_ID], DUST_WIDTH, DUST_HEIGHT, dust, 
    get_vac_x() - (get_vac_width()/ 2), get_vac_y() - (get_vac_height()/ 2), get_vac_width(), get_vac_height(), get_vac_bitmap());
}

// Return true iff dust will overlap charger.
bool dust_hit_charger(int dust_ID) {
    return pixel_collision(dust_x[dust_ID], dust_y[dust_ID], DUST_WIDTH, DUST_HEIGHT, dust, 
    get_charger_x() - (get_charger_width()/ 2), get_charger_y() - (get_charger_height()/ 2), get_charger_width(), get_charger_height(), get_charger_bitmap());
}

// Return true iff this dust will overlap the last dust.
bool dust_hit_dust(int dust_ID) {
    // If this is the first piece of dust (ID = 0), dont check, can't overlap with dust that doesn't exist.
    if (dust_ID == 0) {
        return false;
    }
    else {
        return pixel_collision(dust_x[dust_ID], dust_y[dust_ID], DUST_WIDTH, DUST_HEIGHT, dust,
                                dust_x[dust_ID - 1], dust_y[dust_ID - 1], DUST_WIDTH, DUST_HEIGHT, dust);
    }
}

// Return true iff slime will overlap vacuum.
bool slime_hit_vacuum(int slime_ID) {
    return pixel_collision(slime_x[slime_ID] - (SLIME_WIDTH/ 2), slime_y[slime_ID] - (SLIME_HEIGHT/ 2), SLIME_WIDTH, SLIME_HEIGHT, slime, 
    get_vac_x() - (get_vac_width()/ 2), get_vac_y() - (get_vac_height()/ 2), get_vac_width(), get_vac_height(), get_vac_bitmap());
}

// Return true iff slime will overlap charger.
bool slime_hit_charger(int slime_ID) {
    return pixel_collision(slime_x[slime_ID] - (SLIME_WIDTH/ 2), slime_y[slime_ID] - (SLIME_HEIGHT/ 2), SLIME_WIDTH, SLIME_HEIGHT, slime, 
    get_charger_x() - (get_charger_width()/ 2), get_charger_y() - (get_charger_height()/ 2), get_charger_width(), get_charger_height(), get_charger_bitmap());
}

// Return true iff slime will overlap any dust in the room.
bool slime_hit_dust(int slime_ID) {
    // If there are no dust in the room, don't even check.
    if (num_dust == 0) {
        return false;
    }
    // Check every dust if it collides with the current slime.
    for (int d = 0; d < num_dust; d++) {
        if (pixel_collision(slime_x[slime_ID] - (SLIME_WIDTH/ 2), slime_y[slime_ID] - (SLIME_HEIGHT/ 2), SLIME_WIDTH, SLIME_HEIGHT, slime,
            dust_x[d], dust_y[d], DUST_WIDTH, DUST_HEIGHT, dust)) {
                return true;
        }
    }
    // If we get here no slime overlaps any dust.
    return false;
}

// Return true iff this slime will overlap the last slime.
bool slime_hit_slime(int slime_ID) {
    // If this is the first piece of slime (ID = 0), dont check, can't overlap with slime that doesn't exist.
    if (slime_ID == 0) {
        return false;
    }
    else {
        return pixel_collision(slime_x[slime_ID] - (SLIME_WIDTH/ 2), slime_y[slime_ID] - (SLIME_HEIGHT/ 2), SLIME_WIDTH, SLIME_HEIGHT, slime,
                                slime_x[slime_ID - 1] - (SLIME_WIDTH/ 2), slime_y[slime_ID - 1] - (SLIME_HEIGHT/ 2), SLIME_WIDTH, SLIME_HEIGHT, slime);
    }
}

// Return true iff trash will overlap vacuum.
bool trash_hit_vacuum(int trash_ID) {
    return pixel_collision(trash_x[trash_ID] - (TRASH_WIDTH/ 2), trash_y[trash_ID] - (TRASH_HEIGHT/ 2), TRASH_WIDTH, TRASH_HEIGHT, trash, 
    get_vac_x() - (get_vac_width()/ 2), get_vac_y() - (get_vac_height()/ 2), get_vac_width(), get_vac_height(), get_vac_bitmap());
}

// Return true iff trash will overlap charger.
bool trash_hit_charger(int trash_ID) {
    return pixel_collision(trash_x[trash_ID] - (TRASH_WIDTH/ 2), trash_y[trash_ID] - (TRASH_HEIGHT/ 2), TRASH_WIDTH, TRASH_HEIGHT, trash, 
    get_charger_x() - (get_charger_width()/ 2), get_charger_y() - (get_charger_height()/ 2), get_charger_width(), get_charger_height(), get_charger_bitmap());
}

// Return true iff trash will overlap any dust in the room.
bool trash_hit_dust(int trash_ID) {
    // If there are no dust in the room, don't even check.
    if (num_dust == 0) {
        return false;
    }
    // Check every dust if it collides with the current trash.
    for (int d = 0; d < num_dust; d++) {
        if (pixel_collision(trash_x[trash_ID] - (TRASH_WIDTH/ 2), trash_y[trash_ID] - (TRASH_HEIGHT/ 2), TRASH_WIDTH, TRASH_HEIGHT, trash,
            dust_x[d], dust_y[d], DUST_WIDTH, DUST_HEIGHT, dust)) {
                return true;
        }
    }
    // If we get here no trash overlaps any dust.
    return false;
}

// Return true iff trash will overlap any slime in the room.
bool trash_hit_slime(int trash_ID) {
    // If there are no slime in the room, don't even check.
    if (num_slime == 0) {
        return false;
    }
    // Check every slime if it collides with the current trash.
    for (int s = 0; s < num_slime; s++) {
        if (pixel_collision(trash_x[trash_ID] - (TRASH_WIDTH/ 2), trash_y[trash_ID] - (TRASH_HEIGHT/ 2), TRASH_WIDTH, TRASH_HEIGHT, trash,
            slime_x[s] - (SLIME_WIDTH/ 2), slime_y[s] - (SLIME_HEIGHT/ 2), SLIME_WIDTH, SLIME_HEIGHT, slime)) {
                return true;
        }
    }
    // If we get here no trash overlaps any slime.
    return false;
}

// Return true iff this trash will overlap the last trash.
bool trash_hit_trash(int trash_ID) {
    // If this is the first piece of trash (ID = 0), dont check, can't overlap with trash that doesn't exist.
    if (trash_ID == 0) {
        return false;
    }
    else {
        return pixel_collision(trash_x[trash_ID] - (TRASH_WIDTH/ 2), trash_y[trash_ID] - (TRASH_HEIGHT/ 2), TRASH_WIDTH, TRASH_HEIGHT, trash,
                                trash_x[trash_ID - 1] - (TRASH_WIDTH/ 2), trash_y[trash_ID - 1] - (TRASH_HEIGHT/ 2), TRASH_WIDTH, TRASH_HEIGHT, trash);
    }
}

/* The following are setup and draw functions for the rubbish objects.
*  The objects that are being setup or drawn are documented.
*/
// Draw dust with ID 'dust_ID'.
void draw_dust(int dust_ID) {
    draw_string(dust_x[dust_ID], dust_y[dust_ID], dust);
}

// Draw all dust.
void draw_all_dust() {
    for (int id = 0; id < num_dust; id++) {
        draw_dust(id);
    }
}

// Draw slime with ID 'slime_ID'.
void draw_slime(int slime_ID) {
    draw_pixels(slime_x[slime_ID] - (SLIME_WIDTH/ 2), slime_y[slime_ID] - (SLIME_HEIGHT/ 2), SLIME_WIDTH, SLIME_HEIGHT, slime, true);
}

// Draw all slime.
void draw_all_slime() {
    for (int id = 0; id < num_slime; id++) {
        draw_slime(id);
    }
}

// Draw trash with ID 'trash_ID'.
void draw_trash(int trash_ID) {
    draw_pixels(trash_x[trash_ID] - (TRASH_WIDTH/ 2), trash_y[trash_ID] - (TRASH_HEIGHT/ 2), TRASH_WIDTH, TRASH_HEIGHT, trash, true);
}

// Draw all trash.
void draw_all_trash() {
    for (int id = 0; id < num_trash; id++) {
        draw_trash(id);
    }
}

// Setup a single piece of dust.
void setup_dust(int dust_ID) {
    int width, height;
    get_screen_size( &width, &height );

    // Assign dust with ID 'dust_ID' a random (x, y) coordinate.
    // Dust is only 1 x 1 so no need to add an offset to the coordinate.
    dust_x[dust_ID] = 1 + (rand() % (width - 2));
    dust_y[dust_ID] = 5 + (rand() % (height - 8));
}

// Setup dust based of the user's input.
void setup_all_dust() {
    num_dust = get_int( "How many dust would you like (0 - 1000)?" );

    if (num_dust < 1) {
        num_dust = 0;
    }
    
    else if (num_dust > MAX_DUST) {
        num_dust = MAX_DUST;
    }

    // Setup num_dust amount of dust.
    for (int id = 0; id < num_dust; id++) {
        do {
            setup_dust(id);
        }
        // Do while dust doesn't overlap the vacuum, charger and other dust.
        while (dust_hit_vacuum(id) || dust_hit_charger(id) || dust_hit_dust(id));
    }
}

// Setup a single piece of slime.
void setup_slime(int slime_ID) {
    int width, height;
    get_screen_size( &width, &height );

    // Assign slime with ID 'slime_ID' a random (x, y) coordinate.
    slime_x[slime_ID] = 1 + SLIME_WIDTH/ 2 + (rand() % ((width - 2) - SLIME_WIDTH));
    slime_y[slime_ID] = 5 + SLIME_HEIGHT/ 2 + (rand() % ((height - 8) - SLIME_HEIGHT));
}

// Setup slime based of the user's input.
void setup_all_slime() {
    num_slime = get_int( "How many slime would you like (0 - 10)?" );

    if (num_slime < 1) {
        num_slime = 0;
    }
    
    else if (num_slime > MAX_SLIME) {
        num_slime = MAX_SLIME;
    }

    // Setup num_slime amount of slime.
    for (int id = 0; id < num_slime; id++) {
        do {
            setup_slime(id);
        }
        // Do while slime doesn't overlap the vacuum, charger, dust and other slime.
        while (slime_hit_vacuum(id) || slime_hit_charger(id) || slime_hit_dust(id) || slime_hit_slime(id));
    }
}

// Setup a single piece of trash.
void setup_trash(int trash_ID) {
    int width, height;
    get_screen_size( &width, &height );

    // Assign trash with ID 'trash_ID' a random (x, y) coordinate.
    trash_x[trash_ID] = 1 + TRASH_WIDTH/ 2 + (rand() % ((width - 2) - TRASH_WIDTH));
    trash_y[trash_ID] = 5 + TRASH_HEIGHT/ 2 + (rand() % ((height - 8) - TRASH_HEIGHT));
}

// Setup trash based of the user's input.
void setup_all_trash() {
    num_trash = get_int( "How many trash would you like (0 - 5)?" );

    if (num_trash < 1) {
        num_trash = 0;
    }
    
    else if (num_trash > MAX_TRASH) {
        num_trash = MAX_TRASH;
    }

    // Setup num_slime amount of slime.
    for (int id = 0; id < num_trash; id++) {
        do {
            setup_trash(id);
        }
        // Do while trash doesn't overlap the vacuum, charger, dust, slime and other trash.
        while (trash_hit_vacuum(id) || trash_hit_charger(id) || trash_hit_dust(id) || trash_hit_slime(id) || trash_hit_trash(id));
    }
}

// Draw all rubbish.
void draw_rubbish() {
    draw_all_dust();
    draw_all_slime();
    draw_all_trash();
}

// Setup all rubbish.
void setup_rubbish() {
    // Set a new seed for rand().
    srand(get_current_time());
    setup_all_dust();
    setup_all_slime();
    setup_all_trash();
}

char * get_rubbish_status() {
    sprintf(rubbish_status, "Rubbish (d, s, t): %4d, %2d, %d", num_dust, num_slime, num_trash);
    return rubbish_status;
}