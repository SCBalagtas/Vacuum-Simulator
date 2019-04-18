#pragma once
#include <stdbool.h>

double get_hero_x( void );
double get_hero_y( void );
int get_hero_width( void );
int get_hero_height( void );
void set_hero_x( double x );
void set_hero_y( double y); 

/**
 *  A dialogue which allows the position of the hero to be specified directly.
 */
void do_hero_cheat();

/**
 *  Draw the hero, centred on (hx, hy).
 */
void draw_hero();

/**
 *  Returns true iff and only if the supplied argument is a hero navigation control.
 */
bool is_hero_ctrl( int ch );

/**
 *  Set up the hero at the centre of the terminal.
 */
void setup_hero();


/**
 *  Update the hero location based on the supplied argument.
 *
 *  Parameters:
 *      ch: a character code which is expected to be one of [asdw].
 *          'a' -> move left
 *          's' -> move down
 *          'd' -> move right
 *          'w' -> move up
 *
 *  Notes:
 *      If the designated move would cause the bounding box of the hero
 *  icon to overlap the border then the move is ignored.
 */
void update_hero( int ch );
