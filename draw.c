/*

Student name: Steven Balagtas
Student number: n9998250
Unit: CAB202 Semester 1, 2019

This file contains the function definitions for drawing all objects in the simulation.

*/

// Import .h files.
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <cab202_graphics.h>
#include "helpers.h"
#include "vacuum.h"
#include "timer.h"
#include "rubbish.h"

// Global variables.
static int width, height;
static int vertical = '|';
static int horizontal = '-';
static int corner = '+';

#define STUD_NUM "n9998250"

// Simulation over message.
static char * message_box =
"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
"!!                                    !!"
"!!          SIMULATION OVER!          !!"
"!!                                    !!"
"!!       Press 'r' to restart...      !!"
"!!                 or                 !!"
"!!        Press 'q' to quit...        !!"
"!!                                    !!"
"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
;

#define MESSAGE_WIDTH 40
#define MESSAGE_HEIGHT 9

// Farewell message.
static char * farewell_message =
"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
"!!                                    !!"
"!!       THANK YOU FOR PLAYING!       !!"
"!!                                    !!"
"!!                                    !!"
"!!                                    !!"
"!!      Press any key to quit...      !!"
"!!                                    !!"
"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
;

#define FAREWELL_WIDTH 40
#define FAREWELL_HEIGHT 9

// Help screen.
static char * help_screen =
"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
"!!     VacuSim 1.0                                                 !!"
"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
"                                                                     "
"                                                                     "
"b: toggle return to base mode                                        "
"                                                                     "
"i, j, k, l: push device one unit North, West, South, or East         "
"                                                                     "
"d, s, t: rubbish cheat - drop rubbish                                "
"                                                                     "
"m: change the millisecond loop delay                                 "
"                                                                     "
"r: reset the simulation                                              "
"                                                                     "
"o: specify time-out (seconds) after which the program terminates     "
"                                                                     "
"p: pause or resume vacuum movement                                   "
"                                                                     "
"q: quit the simulation                                               "
"                                                                     "
"v: vacuum cheat 1 - override the position of the vacuum              "
"                                                                     "
"w: vacuum cheat 2 - override the load of the vacuum (grams)          "
"                                                                     "
"y: vacuum cheat 3 - override battery level of the vacuum             "
"                                                                     "
"?: display this help screen                                          "
"                                                                     "
"                                                                     "
"                      Press any key to return...                     "
;

#define HELPSCREEN_WIDTH 69
#define HELPSCREEN_HEIGHT 31

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
    draw_string(((width - 1)/ 2) - (strlen(get_heading())/ 2), 1, get_heading());
    // Draw device battery percentage in the top right cell.
    draw_string((((width - 1)/ 6) * 5) - (strlen(get_battery_status())/ 2), 1, get_battery_status());
    // Draw elapsed time in the bottom left cell.
    draw_string(((width - 1)/ 6) - (strlen(get_elapsed_time())/ 2), 3, get_elapsed_time()); 
    // Draw current load of device (in g) in the bottom middle cell.
    draw_string(((width - 1)/ 2) - (strlen(get_load_status())/ 2), 3, get_load_status());
    // Draw rubbish available in the bottom right cell.
    draw_string((((width - 1)/ 6) * 5) - (strlen(get_rubbish_status())/ 2), 3, get_rubbish_status());
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

// Draw the battery depleted message (simulation over message) in the middle of the room.
void draw_simulation_over() {
    get_screen_size( &width, &height );
    int x = (width - 1)/ 2;
    int y = ((height - 7)/ 2) + 4; // This accounts for the non-room area.
    int left = round(x) - MESSAGE_WIDTH/ 2;
    int top = round(y) - MESSAGE_HEIGHT/ 2;

    // Draw the simulation over message in the middle of the room.
    // Use draw_pixels with space_is_transparent mode off so we can't see rubbish or vaccum under
    // the message.
    draw_pixels(left, top, MESSAGE_WIDTH, MESSAGE_HEIGHT, message_box, false);
    show_screen();
}

// Draw the farewell message in the middle of the room.
void draw_farewell_message() {
    get_screen_size( &width, &height );
    int x = (width - 1)/ 2;
    int y = ((height - 7)/ 2) + 4; // This accounts for the non-room area.
    int left = round(x) - FAREWELL_WIDTH/ 2;
    int top = round(y) - FAREWELL_HEIGHT/ 2;

    // Draw the farewell message in the middle of the room.
    // Use draw_pixels with space_is_transparent mode off so we can't see rubbish or vaccum under
    // the message.
    draw_pixels(left, top, FAREWELL_WIDTH, FAREWELL_HEIGHT, farewell_message, false);
    show_screen();
}

// Draw the help screen in the middle of the room.
void draw_help_screen() {
    get_screen_size( &width, &height );
    int x = (width - 1)/ 2;
    int y = ((height - 7)/ 2) + 4; // This accounts for the non-room area.
    int left = round(x) - HELPSCREEN_WIDTH/ 2;
    int top = round(y) - HELPSCREEN_HEIGHT/ 2;

    // Draw the help screen in the middle of the room.
    // Use draw_pixels with space_is_transparent mode off. (Doesn't matter gonna clear screen anyway).
    draw_pixels(left, top, HELPSCREEN_WIDTH, HELPSCREEN_HEIGHT, help_screen, false);
    show_screen();
}



// Draw all objects in the simulation.
void draw_all() {
    clear_screen();
    draw_room();
    draw_status_display();
    draw_rubbish();
    draw_charger();
    draw_vacuum();
    show_screen();
}