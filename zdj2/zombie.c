#include "zombie.h"
#include "hero.h"
#include "helpers.h"
#include <stdlib.h>
#include <stdbool.h>

bool collided( double, double, int, int, double, double, int, int );

// Zombie state
#define MAX_ZOMBIES 1000000
#define ZOMBIE_SPEED 0.1
#define ZOMBIE_WIDTH 7
#define ZOMBIE_HEIGHT 5

static int num_zombies = 1;

static double zx[MAX_ZOMBIES], zy[MAX_ZOMBIES], zdx[MAX_ZOMBIES], zdy[MAX_ZOMBIES];


/**
 *  A dialogue which allows the position and velocity of a designated zombie to
 *  be manually overridden.
 */
void do_zombie_cheat() {
    int zombie_id = get_int( "Zombie Id?" );
    int x = get_int( "x?" );
    int y = get_int( "y?" );
    int angle = get_int( "degrees?" );

    if ( zombie_id >= 0 && zombie_id < num_zombies ) {
        zx[zombie_id] = x;
        zy[zombie_id] = y;
        zdx[zombie_id] = ZOMBIE_SPEED * cos( angle * M_PI / 180 );
        zdy[zombie_id] = ZOMBIE_SPEED * sin( angle * M_PI / 180 );
    }
}


/**
 *  Draw one zombie, indicated by zombie ID i. The object is centred on
 *  (zx[i],zy[i]).
 *
 *  Parameters:
 *      i: An integer which satisfiess 0 <= i < num_zombies.
 */
void draw_zombie( int i ) {
    int left = round( zx[i] ) - ZOMBIE_WIDTH / 2;
    int right = left + ZOMBIE_WIDTH - 1;

    int top = round( zy[i] ) - ZOMBIE_HEIGHT / 2;
    int bottom = top + ZOMBIE_HEIGHT - 1;

    draw_line( left, top, right, top, 'Z' );
    draw_line( right, top, left, bottom, 'Z' );
    draw_line( left, bottom, right, bottom, 'Z' );
}


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
void setup_zombie( int i ) {
    int w, h;
    get_screen_size( &w, &h );

    zx[i] = 1 + ZOMBIE_WIDTH / 2 + rand() % ( w - 1 - ZOMBIE_WIDTH );
    zy[i] = 1 + ZOMBIE_HEIGHT / 2 + rand() % ( h - 1 - ZOMBIE_HEIGHT );

    double zombie_dir = rand() * 2 * M_PI / RAND_MAX;
    const double step = ZOMBIE_SPEED;

    zdx[i] = step * cos( zombie_dir );
    zdy[i] = step * sin( zombie_dir );
}

/**
 *  Sets up all zombies (0 <= i < num_zombies) so that they are all
 *  wholly contained within the border, and none overlap with the hero.
 */
void setup_zombies() {
    num_zombies = get_int( "How many zombies would you like?" );

    if ( num_zombies < 1 || num_zombies > MAX_ZOMBIES ) {
        num_zombies = 1;
    }

    for ( int i = 0; i < num_zombies; i++ ) {
        do {
            setup_zombie( i );
        }
        while ( collided( get_hero_x(), get_hero_y(), get_hero_width(), get_hero_height(), zx[i], zy[i], ZOMBIE_WIDTH, ZOMBIE_HEIGHT ) );
    }
}

/**
 *  Draws all zombies.
 */
void draw_zombies() {
    for ( int i = 0; i < num_zombies; i++ ) {
        draw_zombie( i );
    }
}

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
void update_zombie( int i ) {
    // Assume that we have not already collided with the wall.
    // Predict the next screen position of the zombie.
    int new_x = round( zx[i] + zdx[i] ) - ZOMBIE_WIDTH / 2;
    int new_y = round( zy[i] + zdy[i] ) - ZOMBIE_HEIGHT / 2;

    bool bounced = false;

    if ( new_x == 0 || new_x + ZOMBIE_WIDTH == screen_width() ) {
        // horizontal bounce
        zdx[i] = -zdx[i];
        bounced = true;
    }

    if ( new_y == 0 || new_y + ZOMBIE_HEIGHT == screen_height() ) {
        // horizontal bounce
        zdy[i] = -zdy[i];
        bounced = true;
    }

    if ( !bounced ) {
        zx[i] += zdx[i];
        zy[i] += zdy[i];
    }
}

/**
 *  Updates the locatiosn of all zombies based on their stored velocities.
 */
void update_zombies() {
    for ( int i = 0; i < num_zombies; i++ ) {
        update_zombie( i );
    }
}

double get_zombie_x( int i ) {
    return zx[i];
}

double get_zombie_y( int i ) {
    return zy[i];
}

int get_zombie_width( int i ) {
    return ZOMBIE_WIDTH;
    
}

int get_zombie_height( int i ) {
    return ZOMBIE_HEIGHT;
}

void set_zombie_x( int i, double x ) {
    zx[i] = x;
}

void set_zombie_y( int i, double y ) {
    zy[i] = y;
}

int get_num_zombies( ){
    return num_zombies;
}
