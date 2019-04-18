#pragma once

#include <stdbool.h>

/**
 *  Tests to see if any zombie has collided with the hero. If so,
 *  do_collided is called to restart the simulation.
 */
void check_collision();

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
);

/**
 *  Action taken when a zombie has collided with the hero.
 *  Displays a message, waits for a response from standard input
 *  (or simulation time-out), then resets the simulation.
 */ 
void do_collided( void );

