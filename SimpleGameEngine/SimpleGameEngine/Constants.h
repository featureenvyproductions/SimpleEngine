#pragma once

#define GL_CLAMP_TO_EDGE	0x812f
#define DEFAULT_SCREEN_HEIGHT	400
#define DEFAULT_SCREEN_WIDTH	600

#define	DEFAULT_RGB_BITS	8
#define DEFAULT_DEPTH	16
#define DEFAULT_BUFFER_SIZE	32

#define DEFAULT_TILESIZE	32

#define FRAME_RATE 60

#define PI	3.1415

//note....numbered like if we were going counter clockwise
enum DIRECTION
{
	DIR_NONE = -1,
	DIR_EAST = 0,
	DIR_NORTHEAST = 1,
	DIR_NORTH = 2,
	DIR_NORTHWEST = 3,
	DIR_WEST = 4,
	DIR_SOUTHWEST = 5,
	DIR_SOUTH = 6,
	DIR_SOUTHEAST = 7
};