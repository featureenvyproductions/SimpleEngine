#include "stdafx.h"
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