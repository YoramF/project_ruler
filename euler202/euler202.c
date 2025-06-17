/*
Three mirrors are arranged in the shape of an equilateral triangle, with their reflective
surfaces pointing inwards. There is an infinitesimal gap at each vertex of the triangle 
through which a laser beam may pass.
Label the vertices A, B and C. There are 2 ways in which a laser beam may enter vertex C,
bounce off 11 surfaces, then exit through the same vertex: one way is shown below; the other
is the reverse of that.

There are 80840 ways in which a laser beam may enter vertex C, bounce off 1000001 surfaces,
then exit through the same vertex.

In how many ways can a laser beam enter at vertex C, bounce off 12017639147 surfaces, 
then exit through the same vertex?

solution:
Initial assumtions:
- For calculation I assume height of the equilateral triangle is 1.
- Each edge length is E = 1/sin(60)
- Rules for beam reflection
-- Hitting angle = reflection angle
-- The only case where the beam is being reflected back to its origin side is when the following happens:
--- The beam hits before the middle of the triangle side, and the reflection angle is bigger than
the angle between a line between the oposit vertex and the hitting point on the triangle side
--- If the hitting angle is equal to the angle between a line between the oposit vertex and the triangle side,
the beam is DEAD (the beam has been swallowed by the triangle)
- The beam will leave the equilateral triangle only when it is reflected from the side opposit 
to the entrance vertex and its hitting angle is equal to 180-alfa (alfa - is the enters angle)

*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "../../timer/timer.h"


#define SIDES 3
// #define PI 3.14159265359
#define DEG(r) ((180/M_PI)*(r))
#define RAD(d) (M_PI*(d)/180)
#define R_A(a) (RAD(120)-(a))		//Calculate reflecting angle based on the hitting angle
#define TIMER	30

double half_side;
double side;
static unsigned sec = 0;
static volatile long long int cnt;
static volatile double h_p, l_a;
static volatile int ts, ss;

void timer_f (const void *arg) {
	unsigned h, m, s;
	sec += TIMER;
	
	h = sec / 3600;
	m = (sec % 3600) / 60;
	s = sec % 60;
	fprintf(stdout, "Runtime %02d:%02d:%02d, ss %d, ts: %d, hitting_p: %10.4f, c: %lld\n", h, m, s, ss, ts, h_p, cnt);
}

// calculate new hitting point (distance from virtual triangle head vertex) based on
// beam reflection angle (radian) and reflection point distance from virtual head vertex
double h_point (double a, double d) {
	double delta = RAD(120) - a;
	return d * sin(a) / sin(delta);
}

// calculate the angle (radians) between the line from opposit vertex to d point on side
// this function is called only if d < half_side;
double l_angle (double d) {
	return atan(100.0 / (half_side-d));
}

// run a beam
long long int beam (double a) {
	double r_a = 0, h_a = 0;
	int direction = 1;

	cnt = 1L;
	ts = 1;
	ss = 0;

	printf("entering bean: %f\n", DEG(a));

	r_a = a;
	h_p = side * sin(a-RAD(60)) / sin(RAD(180)-a);	// calculating initial heatting point

	// do the first beam reflection calculation outside the main loop
	// h_a = R_A(r_a);
	// h_p = h_point(r_a, side - h_p);
	// r_a = h_a;

	do {
		if ((h_p < half_side) && ((l_a = l_angle(h_p)) < r_a)) {
			direction = -1*direction;
			r_a = r_a-RAD(60);
			h_p = h_point((RAD(180)-h_a), h_p);
		}
		else {
			h_a = R_A(r_a);
			h_p = h_point(r_a, side - h_p);
			r_a = h_a;			
		}

		ss = ts;
		if (direction > 0)
			ts = (ts + 1) % SIDES;
		else
			ts = (ts + SIDES - 1) % SIDES;
		cnt++;
	} while (!((ss == 0) && (r_a == l_a)));

	return cnt;
}

int main (int argc, char *argv[]) {
	long long int c;
	int i;
	timer_thr timer;

	if (argc < 2) {
		printf("Missing argument - angle\n");
		return 1;
	}

	i = atoi(argv[1]);

	// calculate half_edge assuming triangle heigh is 1
	side = 100.0 / sin(RAD(60.0));
	half_side = side / 2;

    if (setTimer(&timer, TIMER, &timer_f, NULL, true) != 0)
        fprintf(stdout, "failed to set timer1\n");
    else
       fprintf(stdout, "timer1 was set to %ds\n", TIMER);

	c = beam(RAD(i*1.0));

}

