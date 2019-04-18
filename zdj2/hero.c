#include "hero.h"
#include "helpers.h"

// Hero state
#define HERO_WIDTH 9
#define HERO_HEIGHT 3

static double hx, hy;


/**
 *  A dialogue which allows the position of the hero to be specified directly.
 */
void do_hero_cheat() {
    hx = get_int( "Hero x?" );
    hy = get_int( "Hero y?" );
}

/**
 *  Draw the hero, centred on (hx, hy).
 */
void draw_hero() {
    int left = round( hx ) - HERO_WIDTH / 2;
    int right = left + HERO_WIDTH - 1;

    int top = round( hy ) - HERO_HEIGHT / 2;
    int bottom = top + HERO_HEIGHT - 1;

    draw_line( left, top, left, bottom, 'H' );
    draw_line( right, top, right, bottom, 'H' );
    draw_line( left, round( hy ), right, round( hy ), 'H' );
}

/**
 *  Returns true iff and only if the supplied argument is a hero navigation control.
 */
bool is_hero_ctrl( int ch ) {
    return ( ch == 'a' || ch == 's' || ch == 'd' || ch == 'w' );
}

/**
 *  Set up the hero at the centre of the terminal.
 */
void setup_hero() {
    int w, h;
    get_screen_size( &w, &h );
    hx = w / 2;
    hy = h / 2;
}


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
void update_hero( int ch ) {
    if ( ch == 'a' && hx - HERO_WIDTH / 2 > 1 ) {
        hx--;
    }
    else if ( ch == 'd' && hx + HERO_WIDTH / 2 < screen_width() - 2 ) {
        hx++;
    }
    else if ( ch == 's' && hy + HERO_HEIGHT / 2 < screen_height() - 2 ) {
        hy++;
    }
    else if ( ch == 'w' && hy - HERO_HEIGHT / 2 > 1 ) {
        hy--;
    }
}

double get_hero_x() {
    return hx;
}

double get_hero_y() {
    return hy;
}

int get_hero_width() {
    return HERO_WIDTH;
}

int get_hero_height() {
    return HERO_HEIGHT;
}

void set_hero_x( double x ) {
    hx = x;
}

void set_hero_y( double y ) {
    hy = y;
}
