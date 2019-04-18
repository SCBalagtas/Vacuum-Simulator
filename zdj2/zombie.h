#pragma once
#include "helpers.h"

double get_zombie_x( int i );
double get_zombie_y( int i );
int get_zombie_width( int i );
int get_zombie_height( int i );
void set_zombie_x( int i, double x );
void set_zombie_y( int i, double y );
int get_num_zombies( void );

/**
 *  A dialogue which allows the position and velocity of a designated zombie to
 *  be manually overridden.
 */
void do_zombie_cheat();

/**
 *  Draw one zombie, indicated by zombie ID i. The object is centred on
 *  (zx[i],zy[i]).
 *
 *  Parameters:
 *      i: An integer which satisfies 0 <= i < num_zombies.
 */
void draw_zombie( int i ) ;

/**
 *  Set up a single zombie at a random location.
 *
 *  Parameters:
 *      i: An integer satisfying 0 <= i < num_zombies which indicates the
 *          object to be placed.
 *
 *  Notes:
 *      This function assigns random values to zx[i] and zy[i] so that the
 *  object is guaranteed to be wholly contained within the border. A random
 *  velocity vector is set up in zdx[i] and zdy[i]. The zombie will take
 *  approximately (1.0 / ZOMBIE_SPEED) time slices to travel one screen unit in
 *  the direction specified by the velocity vector.
 */
void setup_zombie( int i );

/**
 *  Sets up all zombies (0 <= i < num_zombies) so that they are all
 *  wholly contained within the border, and none overlap with the hero.
 */
void setup_zombies();
/**
 *  Draws all zombies.
 */
void draw_zombies();

/**
 *  Update the location of a zombie based on its stored velocity.
 *
 *  Parameters:
 *      i: the ID of a zombie, satisfying 0 <= i < num_zombies.
 *
 *  Notes:
 *      If the designated move would cause the bounding box of the zombie
 *  icon to overlap the border then one or both of the velocity components
 *  is negated, as appropriate to cause the zombie to reflect off the wall.
 *  When that happens the zombie does not move.
 */
void update_zombie( int i );

/**
 *  Updates the locatiosn of all zombies based on their stored velocities.
 */
void update_zombies();

