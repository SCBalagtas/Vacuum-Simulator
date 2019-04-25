/*

Student name: Steven Balagtas
Student number: n9998250
Unit: CAB202 Semester 1, 2019

This file contains the function declarations for everything related to the Robot Vacuum cleaner.

*/

// Import .h files.
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <cab202_graphics.h>
#include "helpers.h"

void draw_vacuum();
void setup_vacuum();
void draw_charger();
void setup_charger();
bool is_vacuum_ctrl( int ch );
void manual_update_vacuum( int ch );
void update_vacuum();
char * get_heading();
void start_battery_timer();
bool is_battery();
void battery_hack(int new_battery);
char * get_battery_status();