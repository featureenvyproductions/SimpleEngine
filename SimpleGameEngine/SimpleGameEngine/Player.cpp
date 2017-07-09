#include "stdafx.h"
#include "Player.h"

CPlayer::CPlayer(int x0, int y0, std::string id, int speed0, std::string filename, int initialDir)
	:CObject(x0, y0, id, speed0, filename)
{
	x = x0;
	y = y0;
	vx = 0;
	vy = 0;
	speed = speed0;
	m_strTexture = filename;
	m_strID = id;
	m_currentSprite.reset(new CSprite(x, y));
	m_bSolid = true;// false;
}

CPlayer::CPlayer()
{
}

CPlayer::~CPlayer()
{
}

void CPlayer::HandleInput(SDL_Event event)
{
	CObject::HandleInput(event);
}