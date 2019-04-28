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

static int num_dust, dust_status;
static double dust_x[MAX_DUST], dust_y[MAX_DUST];
static bool dust_collected[MAX_DUST];

static char * dust =
"."
;

// Slime variables.
#define MAX_SLIME 10
#define SLIME_WEIGHT 5 // in grams.
#define SLIME_WIDTH 5
#define SLIME_HEIGHT 5

static int num_slime, slime_status;
static double slime_x[MAX_SLIME], slime_y[MAX_SLIME];
static bool slime_collected[MAX_SLIME];

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

static int num_trash, trash_status;
static double trash_x[MAX_TRASH], trash_y[MAX_TRASH];
static bool trash_collected[MAX_SLIME];

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

// Return true iff new dust will overlap any existing dust.
bool new_dust_hit_dust(int x, int y) {
    for (int d = 0; d < num_dust; d++) {
        if (pixel_collision(x, y, DUST_WIDTH, DUST_HEIGHT, dust,
                            dust_x[d], dust_y[d], DUST_WIDTH, DUST_HEIGHT, dust)) {
            return true;
        }
    }
    // If we reach here, new dust does not overlap any existing dust.
    return false;
}

// Return true iff new dust will overlap any existing slime.
bool new_dust_hit_slime(int x, int y) {
    for (int s = 0; s < num_slime; s++) {
        if (pixel_collision(x, y, DUST_WIDTH, DUST_HEIGHT, dust,
                            slime_x[s] - (SLIME_WIDTH/ 2), slime_y[s] - (SLIME_HEIGHT/ 2), SLIME_WIDTH, SLIME_HEIGHT, slime)) {
            return true;
        }
    }
    // If we reach here, new dust does not overlap any existing slime.
    return false;
}

// Return true iff new dust will overlap any existing trash.
bool new_dust_hit_trash(int x, int y) {
    for (int t = 0; t < num_trash; t++) {
        if (pixel_collision(x, y, DUST_WIDTH, DUST_HEIGHT, dust,
                            trash_x[t] - (TRASH_WIDTH/ 2), trash_y[t] - (TRASH_HEIGHT/ 2), TRASH_WIDTH, TRASH_HEIGHT, trash)) {
            return true;
        }
    }
    // If we reach here, new dust does not overlap any existing trash.
    return false;
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

// Return true iff new slime will overlap any existing dust.
bool new_slime_hit_dust(int x, int y) {
    for (int d = 0; d < num_dust; d++) {
        if (pixel_collision(x - (SLIME_WIDTH/ 2), y - (SLIME_HEIGHT/ 2), SLIME_WIDTH, SLIME_HEIGHT, slime,
                            dust_x[d], dust_y[d], DUST_WIDTH, DUST_HEIGHT, dust)) {
            return true;
        }
    }
    // If we reach here, new slime does not overlap any existing dust.
    return false;
}

// Return true iff new slime will overlap any existing slime.
bool new_slime_hit_slime(int x, int y) {
    for (int s = 0; s < num_slime; s++) {
        if (pixel_collision(x - (SLIME_WIDTH/ 2), y - (SLIME_HEIGHT/ 2), SLIME_WIDTH, SLIME_HEIGHT, slime,
                            slime_x[s] - (SLIME_WIDTH/ 2), slime_y[s] - (SLIME_HEIGHT/ 2), SLIME_WIDTH, SLIME_HEIGHT, slime)) {
            return true;
        }
    }
    // If we reach here, new slime does not overlap any existing slime.
    return false;
}

// Return true iff new slime will overlap any existing trash.
bool new_slime_hit_trash(int x, int y) {
    for (int t = 0; t < num_trash; t++) {
        if (pixel_collision(x - (SLIME_WIDTH/ 2), y - (SLIME_HEIGHT/ 2), SLIME_WIDTH, SLIME_HEIGHT, slime,
                            trash_x[t] - (TRASH_WIDTH/ 2), trash_y[t] - (TRASH_HEIGHT/ 2), TRASH_WIDTH, TRASH_HEIGHT, trash)) {
            return true;
        }
    }
    // If we reach here, new slime does not overlap any existing trash.
    return false;
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

// Return true iff new trash will overlap any existing dust.
bool new_trash_hit_dust(int x, int y) {
    for (int d = 0; d < num_dust; d++) {
        if (pixel_collision(x - (TRASH_WIDTH/ 2), y - (TRASH_HEIGHT/ 2), TRASH_WIDTH, TRASH_HEIGHT, trash,
                            dust_x[d], dust_y[d], DUST_WIDTH, DUST_HEIGHT, dust)) {
            return true;
        }
    }
    // If we reach here, new trash does not overlap any existing dust.
    return false;
}

// Return true iff new trash will overlap any existing slime.
bool new_trash_hit_slime(int x, int y) {
    for (int s = 0; s < num_slime; s++) {
        if (pixel_collision(x - (TRASH_WIDTH/ 2), y - (TRASH_HEIGHT/ 2), TRASH_WIDTH, TRASH_HEIGHT, trash,
                            slime_x[s] - (SLIME_WIDTH/ 2), slime_y[s] - (SLIME_HEIGHT/ 2), SLIME_WIDTH, SLIME_HEIGHT, slime)) {
            return true;
        }
    }
    // If we reach here, new trash does not overlap any existing slime.
    return false;
}

// Return true iff new trash will overlap any existing trash.
bool new_trash_hit_trash(int x, int y) {
    for (int t = 0; t < num_trash; t++) {
        if (pixel_collision(x - (TRASH_WIDTH/ 2), y - (TRASH_HEIGHT/ 2), TRASH_WIDTH, TRASH_HEIGHT, trash,
                            trash_x[t] - (TRASH_WIDTH/ 2), trash_y[t] - (TRASH_HEIGHT/ 2), TRASH_WIDTH, TRASH_HEIGHT, trash)) {
            return true;
        }
    }
    // If we reach here, new trash does not overlap any existing trash.
    return false;
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
        // Check if dust has been collected by the vacuum.
        if (dust_collected[id] == false) {
            draw_dust(id);
        } 
        // Else don't draw the dust.
        continue;
    }
}

// Draw slime with ID 'slime_ID'.
void draw_slime(int slime_ID) {
    draw_pixels(slime_x[slime_ID] - (SLIME_WIDTH/ 2), slime_y[slime_ID] - (SLIME_HEIGHT/ 2), SLIME_WIDTH, SLIME_HEIGHT, slime, true);
}

// Draw all slime.
void draw_all_slime() {
    for (int id = 0; id < num_slime; id++) {
        // Check if slime has been collected by the vacuum.
        if (slime_collected[id] == false) {
            draw_slime(id);
        } 
        // Else don't draw the slime.
        continue;
    }
}

// Draw trash with ID 'trash_ID'.
void draw_trash(int trash_ID) {
    draw_pixels(trash_x[trash_ID] - (TRASH_WIDTH/ 2), trash_y[trash_ID] - (TRASH_HEIGHT/ 2), TRASH_WIDTH, TRASH_HEIGHT, trash, true);
}

// Draw all trash.
void draw_all_trash() {
    for (int id = 0; id < num_trash; id++) {
        // Check if trash has been collected by the vacuum.
        if (trash_collected[id] == false) {
            draw_trash(id);
        } 
        // Else don't draw the trash.
        continue;
    }
}

// Setup a single piece of dust.
void setup_dust(int dust_ID) {
    int width, height;
    get_screen_size( &width, &height );

    // Assign dust with ID 'dust_ID' a random (x, y) coordinate and assign collected attribute to false.
    // Dust is only 1 x 1 so no need to add an offset to the coordinate.
    dust_x[dust_ID] = 1 + (rand() % (width - 2));
    dust_y[dust_ID] = 5 + (rand() % (height - 8));
    dust_collected[dust_ID] = false;
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

    // Set the dust status display value.
    dust_status = num_dust;

    // Setup num_dust amount of dust.
    for (int id = 0; id < num_dust; id++) {
        do {
            setup_dust(id);
        }
        // Do while dust doesn't overlap the vacuum, charger and other dust.
        while (dust_hit_vacuum(id) || dust_hit_charger(id) || dust_hit_dust(id));
    }
}

// Add a new piece of dust. This function will be used in the do_operations(int ch) in main.
void add_dust() {
    // Check if num_dust is already at MAX_DUST. If it is, just ignore add_dust().
    if (num_dust == MAX_DUST) return;

    // Variables for the coordinates of the new dust and the the room dimensions.
    int x, y, width, height; 
    x = get_int( "x-coordinate of the new dust?" );
    y = get_int( "y-coordinate of the new dust?" );
    get_screen_size( &width, &height );

    // Check if the given (x, y) coordinate is in the room. If it is, continue. Else just ignore adding this dust.
    if (x < 1 || x > (width - 2) || y < 5 || y > (height - 4)) return;

    // Check if new dust will overlap any objects in the room.
    bool hit_vacuum = pixel_collision(x, y, DUST_WIDTH, DUST_HEIGHT, dust, 
                        get_vac_x() - (get_vac_width()/ 2), get_vac_y() - (get_vac_height()/ 2), get_vac_width(), get_vac_height(), get_vac_bitmap());
    bool hit_charger = pixel_collision(x, y, DUST_WIDTH, DUST_HEIGHT, dust, 
                        get_charger_x() - (get_charger_width()/ 2), get_charger_y() - (get_charger_height()/ 2), get_charger_width(), get_charger_height(), get_charger_bitmap());
    bool hit_dust = new_dust_hit_dust(x, y);
    bool hit_slime = new_dust_hit_slime(x, y);
    bool hit_trash = new_dust_hit_trash(x, y);

    if (!hit_vacuum && !hit_charger && !hit_dust && !hit_slime && !hit_trash) {
        // Add new dust to array of dusts.
        dust_x[num_dust] = x;
        dust_y[num_dust] = y;
        dust_collected[num_dust] = false;
        num_dust += 1;
        dust_status = num_dust;
    }
    // Else just ignore adding this dust.
}

// Setup a single piece of slime.
void setup_slime(int slime_ID) {
    int width, height;
    get_screen_size( &width, &height );

    // Assign slime with ID 'slime_ID' a random (x, y) coordinate and assign collected attribute to false.
    slime_x[slime_ID] = 1 + SLIME_WIDTH/ 2 + (rand() % ((width - 2) - SLIME_WIDTH));
    slime_y[slime_ID] = 5 + SLIME_HEIGHT/ 2 + (rand() % ((height - 8) - SLIME_HEIGHT));
    slime_collected[slime_ID] = false;
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
    
    // Set the slime status display value.
    slime_status = num_slime;

    // Setup num_slime amount of slime.
    for (int id = 0; id < num_slime; id++) {
        do {
            setup_slime(id);
        }
        // Do while slime doesn't overlap the vacuum, charger, dust and other slime.
        while (slime_hit_vacuum(id) || slime_hit_charger(id) || slime_hit_dust(id) || slime_hit_slime(id));
    }
}

// Add a new piece of slime. This function will be used in the do_operations(int ch) in main.
void add_slime() {
    // Check if num_slime is already at MAX_SLIME. If it is, just ignore add_slime().
    if (num_slime == MAX_SLIME) return;

    // Variables for the coordinates of the new slime and the the room dimensions.
    int x, y, width, height; 
    x = get_int( "x-coordinate of the new slime?" );
    y = get_int( "y-coordinate of the new slime?" );
    get_screen_size( &width, &height );

    // Check if the given (x, y) coordinate is in the room. If it is, continue. Else just ignore adding this slime.
    if (x - (SLIME_WIDTH/ 2) < 1 || x + (SLIME_WIDTH/ 2) > (width - 2) || y - (SLIME_HEIGHT/ 2) < 5 || y + (SLIME_HEIGHT/ 2) > (height - 4)) return;

    // Check if new slime will overlap any objects in the room.
    bool hit_vacuum = pixel_collision(x - (SLIME_WIDTH/ 2), y - (SLIME_HEIGHT/ 2), SLIME_WIDTH, SLIME_HEIGHT, slime, 
                        get_vac_x() - (get_vac_width()/ 2), get_vac_y() - (get_vac_height()/ 2), get_vac_width(), get_vac_height(), get_vac_bitmap());
    bool hit_charger = pixel_collision(x - (SLIME_WIDTH/ 2), y - (SLIME_HEIGHT/ 2), SLIME_WIDTH, SLIME_HEIGHT, slime, 
                        get_charger_x() - (get_charger_width()/ 2), get_charger_y() - (get_charger_height()/ 2), get_charger_width(), get_charger_height(), get_charger_bitmap());
    bool hit_dust = new_slime_hit_dust(x, y);
    bool hit_slime = new_slime_hit_slime(x, y);
    bool hit_trash = new_slime_hit_trash(x, y);

    if (!hit_vacuum && !hit_charger && !hit_dust && !hit_slime && !hit_trash) { 
        // Add new slime to array of slimes.
        slime_x[num_slime] = x;
        slime_y[num_slime] = y;
        slime_collected[num_slime] = false;
        num_slime += 1;
        slime_status = num_slime;
    }
    // Else just ignore adding this slime.
}

// Setup a single piece of trash.
void setup_trash(int trash_ID) {
    int width, height;
    get_screen_size( &width, &height );

    // Assign trash with ID 'trash_ID' a random (x, y) coordinate and assign collected attribute to false.
    trash_x[trash_ID] = 1 + TRASH_WIDTH/ 2 + (rand() % ((width - 2) - TRASH_WIDTH));
    trash_y[trash_ID] = 5 + TRASH_HEIGHT/ 2 + (rand() % ((height - 8) - TRASH_HEIGHT));
    trash_collected[trash_ID] = false;
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

    // Set the trash status display value.
    trash_status = num_trash;

    // Setup num_slime amount of slime.
    for (int id = 0; id < num_trash; id++) {
        do {
            setup_trash(id);
        }
        // Do while trash doesn't overlap the vacuum, charger, dust, slime and other trash.
        while (trash_hit_vacuum(id) || trash_hit_charger(id) || trash_hit_dust(id) || trash_hit_slime(id) || trash_hit_trash(id));
    }
}

// Add a new piece of trash. This function will be used in the do_operations(int ch) in main.
void add_trash() {
    // Check if num_trash is already at MAX_TRASH. If it is, just ignore add_trash().
    if (num_trash == MAX_TRASH) return;

    // Variables for the coordinates of the new trash and the the room dimensions.
    int x, y, width, height; 
    x = get_int( "x-coordinate of the new trash?" );
    y = get_int( "y-coordinate of the new trash?" );
    get_screen_size( &width, &height );

    // Check if the given (x, y) coordinate is in the room. If it is, continue. Else just ignore adding this trash.
    if (x - (TRASH_WIDTH/ 2) < 1 || x + (TRASH_WIDTH/ 2) > (width - 2) || y - (TRASH_HEIGHT/ 2) < 5 || y + (TRASH_HEIGHT/ 2) > (height - 4)) return;

    // Check if new trash will overlap any objects in the room.
    bool hit_vacuum = pixel_collision(x - (TRASH_WIDTH/ 2), y - (TRASH_HEIGHT/ 2), TRASH_WIDTH, TRASH_HEIGHT, trash, 
                        get_vac_x() - (get_vac_width()/ 2), get_vac_y() - (get_vac_height()/ 2), get_vac_width(), get_vac_height(), get_vac_bitmap());
    bool hit_charger = pixel_collision(x - (TRASH_WIDTH/ 2), y - (TRASH_HEIGHT/ 2), TRASH_WIDTH, TRASH_HEIGHT, trash, 
                        get_charger_x() - (get_charger_width()/ 2), get_charger_y() - (get_charger_height()/ 2), get_charger_width(), get_charger_height(), get_charger_bitmap());
    bool hit_dust = new_trash_hit_dust(x, y);
    bool hit_slime = new_trash_hit_slime(x, y);
    bool hit_trash = new_trash_hit_trash(x, y);

    if (!hit_vacuum && !hit_charger && !hit_dust && !hit_slime && !hit_trash) { 
        // Add new trash to array of trash.
        trash_x[num_trash] = x;
        trash_y[num_trash] = y;
        trash_collected[num_trash] = false;
        num_trash += 1;
        trash_status = num_trash;
    }
    // Else just ignore adding this trash.
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

// Return the current status for rubbish in a format suitable for the status display.
char * get_rubbish_status() {
    sprintf(rubbish_status, "Rubbish (d, s, t): %4d, %2d, %d", dust_status, slime_status, trash_status);
    return rubbish_status;
}

// Function that lets the vacuum collect rubbish when not in return to base mode.
void collect_rubbish() {
    // Check if vacuum overlaps any dust.
    for (int d = 0; d < num_dust; d++) {
        // If dust with ID 'd' overlaps with the vacuum, set its collected attribute to true.
        // This dust should no longer be drawn in the next frame.
        if (dust_hit_vacuum(d)) {
            // Add dust weight to the vacuum's load and update the rubbish status display, 
            // iff this is the first time this dust collides with the vacuum.
            if (dust_collected[d] == false) {
                add_load(DUST_WEIGHT);
                dust_status -= 1;
            }
            dust_collected[d] = true;
        }
    }

    // Check if vacuum overlaps any slime.
    for (int s = 0; s < num_slime; s++) {
        // If slime with ID 's' overlaps with the vacuum, set its collected attribute to true.
        // This slime should no longer be drawn in the next frame.
        if (slime_hit_vacuum(s)) {
            // Add slime weight to the vacuum's load and update the rubbish status display, 
            // iff this is the first time this slime collides with the vacuum.
            if (slime_collected[s] == false) {
                add_load(SLIME_WEIGHT);
                slime_status -= 1;
            }
            slime_collected[s] = true;
        }
    }

    // Check if vacuum overlaps any trash.
    for (int t = 0; t < num_trash; t++) {
        // If trash with ID 't' overlaps with the vacuum, set its collected attribute to true.
        // This trash should no longer be drawn in the next frame.
        if (trash_hit_vacuum(t)) {
            // Add trash weight to the vacuum's load and update the rubbish status display, 
            // iff this is the first time this trash collides with the vacuum.
            if (trash_collected[t] == false) {
                add_load(TRASH_WEIGHT);
                trash_status -= 1;
            }
            trash_collected[t] = true;
        }
    }
}