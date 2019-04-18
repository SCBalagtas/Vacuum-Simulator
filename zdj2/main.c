#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <cab202_graphics.h>
#include <cab202_timers.h>
#include "helpers.h"
#include "hero.h"
#include "zombie.h"
#include "collision.h"

// Global variables
bool game_over = false;
int delay = 10;
int timeout = INT_MAX;
double start_time;

bool paused = false;

/**
 *  Draws a border around the terminal window using '*' symbols.
 */
void draw_border() {
    int w, h;
    get_screen_size( &w, &h );
    const int ch = '*';
    draw_line( 0, 0, 0, h - 1, ch );
    draw_line( 0, 0, w - 1, 0, ch );
    draw_line( 0, h - 1, w - 1, h - 1, ch );
    draw_line( w - 1, 0, w - 1, h - 1, ch );
}

/**
 *  Draw all objects in the simulation.
 */
void draw_all() {
    clear_screen();
    draw_border();
    draw_hero();
    draw_zombies();
    show_screen();
}

/**
 *  Set up all objects in the simulation.
 */
void setup( void ) {
    start_time = get_current_time();
    setup_hero();
    setup_zombies();
    draw_all();
}

/**
 *  Resets the simulation.
 */
void reset() {
    setup();
}

/**
 *  Obtain a time-out value from standard input and store it for future use.
 */
void do_timeout() {
    timeout = get_int( "How many seconds shall the program run?" );
}

/**
 *  Returns true iff the simulation has been running for longer than the
 *  current designated time-out value.
 */
bool timed_out() {
    return get_current_time() - start_time >= timeout;
}

/**
 *  Toggles zombie motion between paused and not-paused.
 */
void do_pause() {
    paused = !paused;
}

/**
 *  Dialogue which allows the loop delay to be manually overridden.
 */
void do_delay_cheat() {
    delay = get_int( "New delay (milliseconds)?" );

    if ( delay < 0 ) delay = 10;
}

/**
 *  Dialog which displays a help screen.
 */
void do_help() {
    clear_screen();
    draw_formatted_lines( 0, 0,
        "!!!!!!!!!!!!!!!!!!!!!!!!!\n"
        "!!! ZombieDash Junior !!!\n"
        "!!!!!!!!!!!!!!!!!!!!!!!!!\n"
        "\n"
        "a, s, d, w: Move hero left, down, right, up respectively\n"
        "r: restart\n"
        "t: time out for a user specified number of milliseconds\n"
        "z: zombie cheat - override the position and direction of a zombie\n"
        "h: hero cheat - override the position of the hero\n"
        "p: pause or resume zombie movement\n"
        "m: change the millisecond loop delay\n"
        "q: quit\n"
        "?: display this help screen\n\n\n"
        "Press a key to return to the game."
    );
    show_screen();

    while ( get_char() > 0 ) {}
    while ( !timed_out() && wait_char() <= 0 ) {}
}

/**
 *  Jump table which chooses the action (if any) which corresponds to a
 *  supplied command character.
 *
 *  Parameters:
 *      ch: a command character. Currently recognised comands are listed in
 *          the do_help function.
 */
void do_operation( int ch ) {
    if ( ch == 'r' ) {
        reset();
    }
    else if ( is_hero_ctrl( ch ) ) {
        update_hero( ch );
    }
    else if ( ch == 't' ) {
        do_timeout();
    }
    else if ( ch == 'z' ) {
        do_zombie_cheat();
    }
    else if ( ch == 'h' ) {
        do_hero_cheat();
    }
    else if ( ch == 'p' ) {
        do_pause();
    }
    else if ( ch == 'm' ) {
        do_delay_cheat();
    }
    else if ( ch == '?' ) {
        do_help();
    }
}

/**
 *  The loop function: called once per time slice to run the simulation.
 */
void loop() {
    int ch = get_char();

    if ( ch == 'q' ) {
        game_over = true;
        return;
    }
    else if ( ch >= ' ' ) {
        do_operation( ch );
    }
    else if ( !paused ) {
        update_zombies();
    }

    check_collision();

    draw_all();
}

/**
 *  Program start and event loop.
 */
int main( void ) {

    // srand(9139532); // For initial testing 
    srand( get_current_time() ); // For production

    setup_screen();
    setup();

    while ( !game_over && !timed_out() ) {
        loop();
        timer_pause( delay );
    }

    return 0;
}
