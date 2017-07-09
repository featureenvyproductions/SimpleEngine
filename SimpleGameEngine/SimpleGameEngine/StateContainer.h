#pragma once


//a room or menu with its own rules for input handling and stuff to draw.
//Basically is its own game state.
//I call it a room everywhere out of habit
class CStateContainer
{
protected:
	std::string m_strID; //unique id for the room or menu so we can find it in a map later
	std::vector<CObject *> objects; //objects associated with this room
public:

	CStateContainer()
	{
	}

	/*! Gives room specified id*/
	CStateContainer(std::string id)
	{
		m_strID = id;
	}

	/*! Currently just deletes created objects. Called when room is left. Really should save the state of persistent objects though.*/
	~CStateContainer()
	{
		for (int i = 0; i < objects.size(); i++)
		{
			if (objects.at(i) != NULL)
				delete objects.at(i);
		}
	}

	/*! Room objects loaded here. Called when room is entered. Eventually needs to look at persistent state of objects*/
	virtual bool Init()
	{
		//these are for getting started....objects won't always be hard coded
		objects.push_back(new CNPC(200, 200, "testNPC", 0, "object.png"));
		objects.push_back(new CNPC(100, 100, "testNPCmovable", 1, "object.png"));
		for (std::vector<CObject *>::iterator it = objects.begin(); it != objects.end(); ++it)
		{
			(*it)->Init();
		}
		return (objects.size() > 0);
	}

	/*! Called by main game code to do its own input handling. Handles input before main input handler*/
	virtual GAMESTATE HandleInput(std::vector<CObject *> permObjects)
	{
		SDL_Event event;
		GAMESTATE nextState = STATE_RUNNING;

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
					//nextState = STATE_INVENTORY;
				}
				break;
				case SDLK_LCTRL:
				case SDLK_RCTRL:
				{
					printf("\nhere if we're in the right state, we'd bring up the save menu\n");
					//	nextState = STATE_MENU_SAVELOAD;
				}
				break;
				default:
				{
				}
				break;
				}
			}
			break;
			//note....this should probably still be handled by game object
			case SDL_QUIT:
			{
				printf("exiting");

				nextState = STATE_EXITING;
			}
			break;
			default:
			{
			}
			break;
			}

			//pass the event onto our objects.
			//note....i think eventually we'll call this on two things
			//global objects
			//and room objects.
			//instead of loading EVERY object at the same time.
			//of course global objects aren't really GLOBAL global just they live in the game object instead of
			//in just one room.
			for (std::vector<CObject *>::iterator it = objects.begin(); it != objects.end(); ++it)
			{
				(*it)->HandleInput(event);
			}
			for (std::vector<CObject *>::iterator it = permObjects.begin(); it != permObjects.end(); ++it)
			{
				(*it)->HandleInput(event);
			}
		}
		return nextState;
	}

	//i guess room is responsible for doing like everything itself
	virtual GAMESTATE Update(std::vector<CObject *> permObjects)//pass in the game object objects so we can check for collisions etc
	{
		//note...this should include all objects present in the room as well as game objects
		//not sure what the best way to go about this is but i'll think of something
		//guess we're just appending vectors or whatever.
		//you know i COULD pass it just two vectors and have it do the thing i want.
		std::vector<CObject *> allObjects;
		//there's probably a less stupid way to do this.
		for (std::vector<CObject *>::iterator it = objects.begin(); it != objects.end(); ++it)
		{
			allObjects.push_back(*it);
		}
		for (std::vector<CObject *>::iterator it = permObjects.begin(); it != permObjects.end(); ++it)
		{
			allObjects.push_back(*it);
		}

		Collisions::CheckCollisions(allObjects);
		for (std::vector<CObject *>::iterator it = allObjects.begin(); it != allObjects.end(); ++it)
		{
			(*it)->Update("default"); //tbr because we need to make sure this doesn't result in a room change.
		}
		return STATE_RUNNING;
	}

	virtual void Draw()
	{
		for (std::vector<CObject *>::iterator it = objects.begin(); it != objects.end(); ++it)
		{
			(*it)->Draw();
		}
	}

	//calls handle input and update both
	virtual GAMESTATE ProcessInput(std::vector<CObject *> permObjects)
	{
		return STATE_RUNNING;
	}

	virtual std::string GetTransitionData()
	{
		//set whenever we set the transition state.
		//currently just returns the name of the same room but eventually will return
		//data about the transition.
		return "TestRoom";
	}
};
