#pragma once


class Game
{
protected:
	GAMESTATE m_nState;
	std::unique_ptr<CScreen> m_screen;
	std::vector<CObject *> objects; //stuff that needs to be drawn. obviously will change. will mostly be used for permanent objects like player
	std::string m_strStateContainerID;
	std::map<std::string, CStateContainer*> m_stateContainers;
public:
	Game();
	
	~Game();

	int GetState();
	
	int Init();

	//do the right stuff depending on the game state
	//we'll pretty much always be in the running state except for some specific circumstances though
	GAMESTATE GameLoop();
	
	GAMESTATE HandleInput();

	GAMESTATE Update();

	void Draw();
};
