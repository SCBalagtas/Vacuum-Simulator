#include "collision.h"
#include "hero.h"
#include "zombie.h"
#include "cab202_graphics.h"
#include "cab202_timers.h"

bool timed_out( void );
void reset( void );

/**
 *  Tests to see if any zombie has collided with the hero. If so,
 *  do_collided is called to restart the simulation.
 */
void check_collision() {
    for ( int i = 0; i < get_num_zombies(); i++ ) {
        if ( collided( 
                get_hero_x(), get_hero_y(), get_hero_width(), get_hero_height(), 
                get_zombie_x(i), get_zombie_y(i), get_zombie_width(i), get_zombie_height(i) ) ) {
            do_collided();
            return;
        }
    }
}

/**
 *  Returns true if and only if two supplied bounding boxes overlap.
 *
 *  Parameters:
 *      (x0,y0) - real-valued coordinates of the centre of the first bounding box.
 *      (w0,h0) - integer-valued dimensions of the first bounding box.
 *      (x1,y1) - real-valued coordinates of the centre of the second bounding box.
 *      (w1,h1) - integer-valued dimensions of the second bounding box.
 */
bool collided(
    double x0, double y0, int w0, int h0,
    double x1, double y1, int w1, int h1
) {
    int left0 = round( x0 ) - w0 / 2, right0 = left0 + w0 - 1;
    int left1 = round( x1 ) - w1 / 2, right1 = left1 + w1 - 1;

    int top0 = round( y0 ) - h0 / 2, bottom0 = top0 + h0 - 1;
    int top1 = round( y1 ) - h1 / 2, bottom1 = top1 + h1 - 1;

    if ( right1 < left0 || right0 < left1 ) return false;
    if ( bottom1 < top0 || bottom0 < top1 ) return false;

    return true;
}

/**
 *  Action taken when a zombie has collided with the hero.
 *  Displays a message, waits for a response from standard input
 *  (or simulation time-out), then resets the simulation.
 */
void do_collided() {
    clear_screen();

    const char *message[] = {
        "You're zombie food!",
        "Press any key to restart..."
    };

    const int rows = 2;

    for ( int i = 0; i < rows; i++ ) {
        int len = strlen( message[i] );
        int x = ( screen_width() - len ) / 2;
        int y = i + ( screen_height() - rows ) / 2;
        draw_formatted( x, y, message[i] );
    }

    show_screen();

    timer_pause( 1000 );
    while ( get_char() > 0 ) {}
    while ( !timed_out() && wait_char() <= 0 ) {}

    reset();
}
