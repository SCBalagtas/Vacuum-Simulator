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
#include <cab202_timers.h>
#include "helpers.h"

// Global variables.
// Vacuum variables.
#define VACUUM_WIDTH 9
#define VACUUM_HEIGHT 9
#define VACUUM_SPEED 0.2
#define DEFAULT_HEADING 90

static double vac_x, vac_y, vac_dx, vac_dy, angle;

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

static char heading[20];

// Battery variables.
#define MAX_BATTERY 100
static int battery;
static int battery_timer;
static int battery_use;
static int battery_temp_time;
static char battery_status[15];

// Charging station variables.
#define CHARGER_WIDTH 9
#define CHARGER_HEIGHT 3

static double charger_x, charger_y;

static char * charger =
"#########"
"#########"
"#########"
;

// Draw the vacuum with the center being (vac_x, vac_y).
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

    // Initialise vacuum direction 90 degrees (pi/ 2), vacuum heads straight down.
    // Speed is initialised to 0.2 as per specification.
    angle = deg_to_rad(DEFAULT_HEADING);
    double vac_speed = VACUUM_SPEED;
    vac_dx = vac_speed * cos(angle);
    vac_dy = vac_speed * sin(angle);

    // Initialise vacuum battery as 100%.
    battery = MAX_BATTERY;
}

// Draw the charger with the center being (charger_x, charger_y).
void draw_charger() {
    int left = round(charger_x) - CHARGER_WIDTH/ 2;
    int top = round(charger_y) - CHARGER_HEIGHT/ 2;

    draw_pixels(left, top, CHARGER_WIDTH, CHARGER_HEIGHT, charger, true);
}

// Setup charger at the center of the north wall of the room.
void setup_charger() {
    int width = (screen_width() - 1)/ 2;

    charger_x = width;
    charger_y = 6;
}

// Returns true iff the vaccum is going to collide with the charger at pixel level.
bool vacuum_hit_charger(int new_x, int new_y) {
    // Calculate the coordinates of the top left corner of the vacuum in it's next position.
    int vac_left = round(new_x) - VACUUM_WIDTH/ 2;
    int vac_top = round(new_y) - VACUUM_HEIGHT/ 2;

    // Calculate the coordinates of the top left corner of the charging station.
    int charger_left = round(charger_x) - CHARGER_WIDTH/ 2;
    int charger_top = round(charger_y) - CHARGER_HEIGHT/ 2;

    // Return the result of pixel_collision between the vacuum and the charging station.
    return pixel_collision(charger_left, charger_top, CHARGER_WIDTH, CHARGER_HEIGHT, charger, vac_left, vac_top, VACUUM_WIDTH, VACUUM_HEIGHT, vacuum);
}

// Start battery timer function that initiates battery_timer with get_current_time().
void start_battery_timer() {
    battery_timer = round(get_current_time());
    battery_temp_time = 0;
}

// Calculate the battery used since last unpause.
int calc_battery_use() {
    battery_use = round(get_current_time()) - battery_timer;
    return battery_use;
}

// Boolean to check if there is still battery. Returns true iff battery is greater than 0.
bool is_battery() {
    return (battery > 0);
}

// Battery hack function. Sets the battery level to whatever the input value is.
// If input is not an int or is 0, reset battery to max.
void battery_hack(int new_battery) {
    if (new_battery <= MAX_BATTERY && new_battery > 0) {
        battery = new_battery;
    }
    else if (new_battery > MAX_BATTERY || new_battery <= 0) {
        battery = MAX_BATTERY;
    }

    // Reset battery timer.
    start_battery_timer();
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
 *      icon to overlap the border then the move is ignored. Also checks if move
 *      would cause the bounding box of the vacuum to overlap the charging station.
 */
void manual_update_vacuum( int ch ) {
    if ( ch == 'j' && round(vac_x - VACUUM_WIDTH / 2) > 1 ) {
        // Check if move will cause the vacuum to overlap the charging station.
        if (vacuum_hit_charger(vac_x - 1, vac_y)) {
            return;
        }
        else {
            vac_x--;
        }
    }
    else if ( ch == 'l' && round(vac_x + VACUUM_WIDTH / 2) < screen_width() - 2 ) {
        // Check if move will cause the vacuum to overlap the charging station.
        if (vacuum_hit_charger(vac_x + 1, vac_y)) {
            return;
        }
        else {
            vac_x++;
        }
    }
    else if ( ch == 'k' && round(vac_y + VACUUM_HEIGHT / 2) < screen_height() - 4 ) {
        // This move can't result in vacuum overlapping charging station, no need to check here.
        vac_y++;
    }
    else if ( ch == 'i' && round(vac_y - VACUUM_HEIGHT / 2) > 5 ) {
        // Check if move will cause the vacuum to overlap the charging station.
        if (vacuum_hit_charger(vac_x, vac_y - 1)) {
            return;
        }
        else {
            vac_y--;
        }
    }
}

// Change the vacuum's heading direction after hitting an obstacle as required per the specification.
void change_direction() {
    // Set a new seed for rand()
    srand(get_current_time());

    // Convert the current angle into degrees.
    int curr_angle = rad_to_deg(angle);
    
    bool direction; // True = Right, False = Left.

    // Flip a coin, left or right.
    direction = rand() % 2;

    // Check if left or right and adjust angle as necessary.
    // If right add to the angle, if left subrtact from the angle.
    if (direction) {
        curr_angle += (30 + (rand() % 30 + 1)); // Random angle between 30 - 60.
    }
    else {
        curr_angle -= (30 + (rand() % 30 + 1)); // Random angle between 30 - 60.
    }

    // Assign the new angle to the global variable 'angle'.
    angle = deg_to_rad(curr_angle);

    // Assign the new vac_dx and vac_dy values.
    double vac_speed = VACUUM_SPEED;
    vac_dx = vac_speed * cos(angle);
    vac_dy = vac_speed * sin(angle);
}

// Update the vacuum position and battery automatically if !paused.
void update_vacuum() {
    // Predict the new x and y coordinates of the vacuum and check if it will overlap any obstacles.
    int new_x = round(vac_x + vac_dx);
    int new_y = round(vac_y + vac_dy);

    bool bounced = false;

    // Check if vacuum overlaps vertical walls.
    if (new_x - VACUUM_WIDTH/ 2 < 1 || new_x + VACUUM_WIDTH/ 2 > screen_width() - 2) {
        change_direction();
        bounced = true;
    }
    // Check if vacuum overplaps horizontal walls.
    if (new_y - VACUUM_HEIGHT/ 2 < 5 || new_y + VACUUM_HEIGHT/ 2 > screen_height() - 4) {
        change_direction();
        bounced = true;
    }
    // Check if vacuum overlaps the charging station.
    if (vacuum_hit_charger(new_x, new_y)) {
        change_direction();
        bounced = true;
    }

    // If not overlapping anything just keep moving in the same direction.
    if (!bounced) {
        round(vac_x += vac_dx);
        round(vac_y += vac_dy);
    }

    // Update the battery %.
    if (calc_battery_use() - battery_temp_time == 1) {
        battery -= 1;
        battery_temp_time = calc_battery_use();
    }
}

// Return the vacuums heading direction in a format suitable for the display status.
char * get_heading() {
    sprintf(heading, "Heading: %3d", rad_to_deg(angle));
    return heading;
}

// Return the vacuums current battery % in a format suitable for the display status.
char * get_battery_status() {
    sprintf(battery_status, "Battery: %3d%%", battery);
    return battery_status;
}