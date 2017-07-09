#pragma once

class Utils
{
public:
	static void BeginDraw();

	static void EndDraw();

	//vectors and points and whatever garbage

	//if you imagine an arrow pointing from object A's center to object B's center,
	//this is the angle (from 0 to 360 degrees) that arrow makes with respect to the horizontal
	//generally used to figure out quadrant (with respect to center of colliding object) of an obstruction
	static float GetDirectionalAngle(POINT a, POINT b);

	//basically does the same thing as get directional angle but returns a cardinal direction indicating the
	//general location of the obstruction
	static DIRECTION GetDirection(POINT a, POINT b);

};