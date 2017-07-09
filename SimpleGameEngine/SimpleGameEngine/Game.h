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
	Game()
	{
		m_nState = STATE_STARTING;
		m_screen.reset(new CScreen());
	}
	~Game()
	{
		m_screen.release();
		for (int i = 0; i < objects.size(); i++)
		{
			if (objects.at(i) != NULL)
				delete objects.at(i);
		}
		for (std::map<std::string, CStateContainer *>::iterator it = m_stateContainers.begin(); it != m_stateContainers.end(); ++it)
		{
			if ((*it).second != NULL)
				delete (*it).second;
		}
	}
	int GetState()
	{
		return m_nState;
	}
	int Init()
	{
		m_screen->Init();
		m_screen->Create();

		//load permanent objects here
		//tbr...hard-coded right now but needs to be from config files
		objects.push_back(new CPlayer(0, 0, "Player", 5, "playerimg.png")); //this is just a test object representing the "player"
		for (std::vector<CObject *>::iterator it = objects.begin(); it != objects.end(); ++it)
		{
			(*it)->Init();
		}
		m_strStateContainerID = "TestRoom";
		m_stateContainers[m_strStateContainerID] = new CStateContainer(m_strStateContainerID); //add a test room

																							   //for when we're loading the objects from config files in the future
		if ((objects.size() == 0) || (m_stateContainers.size() == 0))
			m_nState = STATE_LOAD_FAIL;
		else
		{
			m_nState = STATE_RUNNING;
			m_stateContainers[m_strStateContainerID]->Init();
		}

		return m_nState;
	}

	//do the right stuff depending on the game state
	//we'll pretty much always be in the running state except for some specific circumstances though
	virtual GAMESTATE GameLoop()
	{
		switch (m_nState)
		{
		case STATE_STARTING:
		{
			//we really shouldn't ever see a call to this when we're starting so something is weird. exit.
			m_nState = STATE_EXITING;
		}
		break;
		case STATE_LOAD_FAIL:
		{
			//we should have already exited but in case something went really really extra wrong
			m_nState = STATE_EXITING;
		}
		break;
		case STATE_TRANSITION: //we're loading a new room.
		{
			m_strStateContainerID = m_stateContainers[m_strStateContainerID]->GetTransitionData();
			if (m_stateContainers.find(m_strStateContainerID) == m_stateContainers.end())
			{
				//either we're exiting or a thing happened so....
				//eventually prompt the user to save though of course
				m_nState = STATE_EXITING;
			}
			else
			{
				//tbr...save the previous room persistent items first
				//also check if room is already initialized
				m_stateContainers[m_strStateContainerID]->Init();
			}
		}
		break;
		case STATE_RUNNING:
		{
			//this is dumb right now and doesn't update the state but eventually we'll do that.
			//just putting it together one step at a time
			HandleInput();
			m_nState = Update();
			Draw();
		}
		break;
		case STATE_EXITING:
		default:
			break;
		}
		return m_nState;
	}


	virtual GAMESTATE HandleInput()
	{
		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_KEYDOWN:
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_LALT:
				case SDLK_RALT:
				{
					printf("\nnormally we'd bring up the inventory here and pause\n");
					printf("assuming we're in normal game mode. whether we can bring up menus\n");
					printf("might depend on game state or a room check or something.\n");
					//	m_nState = STATE_MENU_INVENTORY;
				}
				break;
				case SDLK_LCTRL:
				case SDLK_RCTRL:
				{
					printf("\nhere if we're in the right state, we'd bring up the save menu\n");
					//m_nState = STATE_MENU_SAVELOAD;
				}
				break;
				default:
				{
					//note: make sure we always hit this when nothing else happens
					m_nState = STATE_RUNNING;
				}
				break;
				}
			}
			break;
			case SDL_QUIT:
			{
				printf("exiting");
				m_nState = STATE_EXITING;
			}
			break;
			default:
			{
				//make sure we always hit this when nothing else happens.
				m_nState = STATE_RUNNING;
			}
			break;
			}

			//pass the event to any other objects
			//eventually we'll get a state out of this....tbd...
			for (std::vector<CObject *>::iterator it = objects.begin(); it != objects.end(); ++it)
			{
				(*it)->HandleInput(event);
			}
			m_nState = m_stateContainers[m_strStateContainerID]->HandleInput(objects);
		}
		return m_nState;
	}

	GAMESTATE Update()
	{
		/*	Collisions::CheckCollisions(objects);
		for (std::vector<CObject *>::iterator it = objects.begin(); it != objects.end(); ++it)
		{
		(*it)->Update("Test room");
		}*/
		return m_stateContainers[m_strStateContainerID]->Update(objects);
	}

	void Draw()
	{
		Utils::BeginDraw();
		m_screen->Draw();
		//draw all the shit in the room eventually then draw the "global" objects (usually just the player)
		for (std::vector<CObject *>::iterator it = objects.begin(); it != objects.end(); ++it)
		{
			(*it)->Draw();
		}
		m_stateContainers[m_strStateContainerID]->Draw(); //tbr...i actually want room to be responsible for drawing
		Utils::EndDraw();
	}
};
