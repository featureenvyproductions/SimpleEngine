#pragma once
class CPlayer : public CObject
{
public:
	CPlayer(int x0, int y0, std::string id, int speed0 = 0, std::string filename = "object.png", int initialDir = -1)
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
	CPlayer() {}
	~CPlayer() {}

	void HandleInput(SDL_Event event)
	{
		CObject::HandleInput(event);
	}
};