#pragma once
class CPlayer : public CObject
{
public:
	CPlayer(int x0, int y0, std::string id, int speed0 = 0, std::string filename = "object.png", int initialDir = -1);
	CPlayer();
	~CPlayer();

	void HandleInput(SDL_Event event);
};