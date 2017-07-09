#pragma once
class CNPC : public CObject
{
public:
	CNPC(int x0, int y0, std::string id, int speed0 = 0, std::string filename = "object.png", int initialDir = -1)
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
	CNPC() {}
	~CNPC() {}

	void HandleInput(SDL_Event event)
	{

	}
}; 
