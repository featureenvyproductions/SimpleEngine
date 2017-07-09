#include "stdafx.h"
#include "Utils.h"

void Utils::BeginDraw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	glOrtho(0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, 0, -1, 1);
}

void Utils::EndDraw()
{
	glPopMatrix();
	SDL_GL_SwapBuffers();
}
//vectors and points and whatever garbage

//if you imagine an arrow pointing from object A's center to object B's center,
//this is the angle (from 0 to 360 degrees) that arrow makes with respect to the horizontal
//generally used to figure out quadrant (with respect to center of colliding object) of an obstruction
float Utils::GetDirectionalAngle(POINT a, POINT b)
{
	int dx = b.x - a.x;
	int dy = b.y - a.y;
	float t = atan2f(dy, dx);
	//we'll get a number in the range of -pi to pi.
	//but i want that to be 0 to 2pi. so let's check for a negative and handle accordingly.
	if (t < 0)
	{
		t = 2 * PI + t;
	}
	//make it degrees because even 6 years of engineering school has not beat radians into my brain enough
	//for me to think in random amounts of them
	return ((180.0F * t) / PI);
}

//basically does the same thing as get directional angle but returns a cardinal direction indicating the
//general location of the obstruction
DIRECTION Utils::GetDirection(POINT a, POINT b)
{
	int dx = b.x - a.x;
	int dy = b.y - a.y;
	float t = atan2(dy, dx);
	//we'll get a number in the range of -pi to pi.
	//this is just an approximation.
	//probably the user won't notice these are fuzzy ranges.
	//gonna work with degrees because i get less shit numbers than like PI OVER 8

	//now prepare to lose your goddamn mind since the coordinate system is upside down
	int deg = ((180.0F * t) / PI);
	//so why cast to an int? because i don't really need the precision since i'm just bopping
	//little weird 2d tile people approximately into each other
	//but also because then i get values like 180 + some stupid fraction
	//and my direction checking fucking breaks.
	//and i'm not going to rewrite it because the stupid fraction is because of my rough but sufficient estimate of pi anyway
	float incr = 22.5;
	if ((deg >= -incr) && (deg < incr)) //-22.5 to 22.5
		return DIR_EAST;
	if ((deg >= incr) && (deg < 3 * incr)) //22.5 to 67.5
		return DIR_SOUTHEAST;
	if ((deg >= 3 * incr) && (deg < 5 * incr)) //67.5 to 112.5
		return DIR_SOUTH;
	if ((deg >= 5 * incr) && (deg < 7 * incr)) //112.5 to 157.5
		return DIR_SOUTHWEST;
	if (((deg >= 7 * incr) && (deg <= 8 * incr)) || ((deg < 7 * -incr) && (deg >= 8 * -incr))) //157.5 to 180 or -157.5 to -180
		return DIR_WEST;
	if ((deg < 5 * -incr) && (deg >= 7 * -incr)) //-112.5 to -157.5
		return DIR_NORTHWEST;
	if ((deg < 3 * -incr) && (deg >= 5 * -incr)) //-67.5 to -112.5
		return DIR_NORTH;
	if ((deg < -incr) && (deg >= 3 * -incr)) //-22.5 to -67.5
		return DIR_NORTHEAST;

	//and if something went totally wrong
	return DIR_NONE;
}