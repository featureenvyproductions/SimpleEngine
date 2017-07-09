#pragma once
class CNPC : public CObject
{
public:
	CNPC(int x0, int y0, std::string id, int speed0 = 0, std::string filename = "object.png", int initialDir = -1);
	CNPC();
	~CNPC();

	void HandleInput(SDL_Event event);
}; 
