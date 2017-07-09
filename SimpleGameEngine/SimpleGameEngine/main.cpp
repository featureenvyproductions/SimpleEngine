#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h> 
#include <stdio.h>
#include <memory>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <time.h>
#include <map>
#include "Constants.h"
#include "Utils.h"
#include "Sprite.h"
#include "Object.h"
#include "NPC.h"
#include "Player.h"
#include "Collision.h"

#undef main

#include "Screen.h"
#include "StateContainer.h"
#include "Game.h"
//reminder:also put sdl dll in the windows folder

int main()
{
	clock_t t0;
	t0 = clock();

	Game mygame;
	if (mygame.Init() == STATE_LOAD_FAIL)
		return -1;

	while (mygame.GetState() != STATE_EXITING)
	{

		if ((clock() - t0) > (CLOCKS_PER_SEC / FRAME_RATE))
		{
			t0 = clock();
			mygame.GameLoop();
		}
	}
	system("PAUSE");
	return 0;
}