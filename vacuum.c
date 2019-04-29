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
#define DEFAULT_LOAD 0
#define MAX_LOAD 65 // in grams.
#define RTB_LOAD_TRIGGER 45 // Return to base trigger in grams.
#define RTB_BATTERY_TRIGGER 25 // Return to base trigger.

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
static int load;
static char load_status[30];

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
static bool docked;

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

    // Initialise vacuum load to 0g.
    load = DEFAULT_LOAD;

    // Initialise docked to false.
    docked = false;
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
    return pixel_collision(vac_left, vac_top, VACUUM_WIDTH, VACUUM_HEIGHT, vacuum, charger_left, charger_top, CHARGER_WIDTH, CHARGER_HEIGHT, charger);
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
    // If it has been 1 second or more.
    if (calc_battery_use() - battery_temp_time >= 1) {
        battery -= (calc_battery_use() - battery_temp_time);
        battery_temp_time = calc_battery_use();
    }
}

// A function to calculate an return the angle (in degrees) needed for the vacuum to head straight home.
int calc_home() {
    double adjacent, opposite;

    // Check if vacuum is on the left side or right side of the charger.
    if (vac_x < charger_x) {
        adjacent = charger_x - vac_x;
        opposite = vac_y - 9; // This is the heighest point the vacuum can go before hitting the north wall.
    }
    else if (vac_x > charger_x) {
        adjacent = vac_x - charger_x;
        opposite = vac_y - 9; // This is the heighest point the vacuum can go before hitting the north wall.
    }
     
     // Calculate the elevation angle the vacuum needs to go home.
     return rad_to_deg((atan(opposite/adjacent)));
}

// A function to change the vacuum's direction to head straight home.
void go_home() {
    // If the vacuum is on the left side subtract calc_home() from 360, else add calc_home() to 180.
    // Check if vacuum is on the left side or right side of the charger.
    if (vac_x < charger_x) {
        // Vacuum is on the left of charger.
        angle = deg_to_rad(360 - calc_home());
    }
    else if (vac_x > charger_x) {
        // Vacuum is on the right of charger.
        angle = deg_to_rad(180 + calc_home());
    }
    // If the vacuum is in line with the charger, head straight up.
    else {
        angle = deg_to_rad(270);
    }

    // Assign the new vac_dx and vac_dy values.
    double vac_speed = VACUUM_SPEED;
    vac_dx = vac_speed * cos(angle);
    vac_dy = vac_speed * sin(angle);
}

// A function to make the vacuum go to the charging station and dock.
void return_to_base() {
    // Predict the new x and y coordinates of the vacuum and check if it will overlap the charging station.
    int new_x = round(vac_x + vac_dx);
    int new_y = round(vac_y + vac_dy);
    // go_home() until vacuum is about to collide with the charging station.
    if (!vacuum_hit_charger(new_x, new_y)) {
        go_home();
    }
    else {
        // Turn on docked mode.
        docked = true;
    }
}

// A function which does everything that needs to be done when vacuum is docked.
void docked_mode() {
    // Unload current payload.
    if (load != DEFAULT_LOAD) {
        load = DEFAULT_LOAD;
    }
    // Charge battery.
    battery += 1;
}

// Returns true iff docked is set to true;
bool is_docked() {
    return docked;
}

// A function to toggle docked.
void toggle_docked() {
    docked = !docked;
}

// A vacuum hack function. Moves the vacuum to specified (x, y) coordinate and sets a new heading.
void vacuum_hack() {
    int x, y, new_heading, width, height;
    x = get_int( "New x-coordinate for the vacuum?" );
    y = get_int( "New y-coordinate for the vacuum?" );
    new_heading = get_int( "New heading (in degrees)?" );
    get_screen_size( &width, &height );

    // Check if the given (x, y) coordinate is in the room. If it is, continue. Else just ignore moving the vacuum.
    if (x - (VACUUM_WIDTH/ 2) < 1 || x + (VACUUM_WIDTH/ 2) > (width - 2) || y - (VACUUM_HEIGHT/ 2) < 5 || y + (VACUUM_HEIGHT/ 2) > (height - 4)) return;

    // Check if the new vacuum position will overlap the charging station. If it does, ignore moving the vacuum.
    if (!vacuum_hit_charger(x, y)) {
        // Move the vacuum and set the new heading.
        vac_x = x;
        vac_y = y;
        angle = deg_to_rad(new_heading);
        // Assign the new vac_dx and vac_dy values.
        double vac_speed = VACUUM_SPEED;
        vac_dx = vac_speed * cos(angle);
        vac_dy = vac_speed * sin(angle);
    }
}

// A load hack function. Changes the current payload of the vacuum based of the user's input.
void load_hack() {
    int new_load;
    new_load = get_int( "New load (in grams)?" );
    // Check if input is appropriate.
    if (new_load < 0) {
        new_load = 0;
    }
    else if (new_load >= MAX_LOAD) {
        new_load = RTB_LOAD_TRIGGER;
    }
    load = new_load;
}

// Function that adds to the load of the vacuum.
void add_load(int rubbish_weight) {
    load += rubbish_weight;
}

// Return the vacuums heading direction in a format suitable for the status display.
char * get_heading() {
    sprintf(heading, "Heading: %3d", rad_to_deg(angle));
    return heading;
}

// Return the vacuums current battery % in a format suitable for the status display.
char * get_battery_status() {
    sprintf(battery_status, "Battery: %3d%%", battery);
    return battery_status;
}

// Return the vacuum's current load(g) in a format suitable for the status display.
char * get_load_status() {
    sprintf(load_status, "Load weight (g): %2d", load);
    return load_status;
}

// Return the vacuum's current load(g).
int get_current_load() {
    return load;
}

// Return the return to base trigger.
int get_rtb_load_trigger() {
    return RTB_LOAD_TRIGGER;
}

// Return the vacuum's current battery.
int get_battery() {
    return battery;
}

// Return the return to base trigger.
int get_rtb_battery_trigger() {
    return RTB_BATTERY_TRIGGER;
}

// Return MAX_BATTERY.
int get_max_battery() {
    return MAX_BATTERY;
}

// Return the vacuum's current x-coord.
double get_vac_x() {
    return vac_x;
}

// Return the vacuum's current y-coord.
double get_vac_y() {
    return vac_y;
}

// Return the vacuum's width.
int get_vac_width() {
    return VACUUM_WIDTH;
}

// Return the vacuum's height.
int get_vac_height() {
    return VACUUM_HEIGHT;
}

// Return the vacuum's bitmap.
char * get_vac_bitmap() {
    return vacuum;
}

// Return the charger's current x-coord.
double get_charger_x() {
    return charger_x;
}

// Return the charger's current y-coord.
double get_charger_y() {
    return charger_y;
}

// Return the charger's width.
int get_charger_width() {
    return CHARGER_WIDTH;
}

// Return the charger's height.
int get_charger_height() {
    return CHARGER_HEIGHT;
}

// Return the charger's bitmap.
char * get_charger_bitmap() {
    return charger;
}