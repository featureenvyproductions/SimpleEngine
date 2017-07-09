#include "stdafx.h"
#include "NPC.h"

CNPC::CNPC(int x0, int y0, std::string id, int speed0, std::string filename, int initialDir)
	:CObject(x0, y0, id, speed0, filename)
{
	x = x0;
	y = y0;
	vx = speed0;
	vy = speed0;
	speed = speed0;
	m_strTexture = filename;
	m_currentSprite.reset(new CSprite(x, y));
}

CNPC::CNPC()
{
}

CNPC::~CNPC()
{
}

void CNPC::HandleInput(SDL_Event event)
{

}