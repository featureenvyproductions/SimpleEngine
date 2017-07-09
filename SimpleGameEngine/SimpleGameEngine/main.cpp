#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h> 
#include <stdio.h>
#include <memory>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <time.h>
#include <map>
#include "Constants.h"
#include "Utils.h"
#include "Sprite.h"
#include "Object.h"
#include "NPC.h"
#include "Player.h"
#include "Collision.h"
#undef main

//reminder:also put sdl dll in the windows folder

/*! Represents a coordinate in R3*/
struct coord
{
	int x;
	int y;
	int z;
};

/*!Represents the viewport - aka. the area the user sees*/
struct viewport
{
	int x;
	int y;
	int h;
	int w;
	void Reset(int x_origin, int y_origin, int height, int width, bool bSetGlViewport = false)
	{
		x = x_origin;
		y = y_origin;
		h = height;
		w = width;
		if (bSetGlViewport)
		{
			glViewport(x, y, w, h);
		}
	}
};

/*! The object that controls drawing things to the screen and takes care of surface and SDL init things.*/
class CScreen
{
protected:
	std::unique_ptr<SDL_Surface> m_surface; /*!< SDL surface to draw on*/
	struct viewport m_stViewport;			/*!< view user sees*/
	std::unique_ptr<CSprite> m_bg;			/*!< the background sprite - what gets drawn to the screen initially*/
public:
	/*! Inits the SDL surface, background sprite, and viewport*/
	CScreen()
	{
		m_surface.reset();
		m_bg.reset(new CSprite(0, 0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));
		m_stViewport.Reset(0, 0, DEFAULT_SCREEN_HEIGHT, DEFAULT_SCREEN_WIDTH);
	}

	/*! Destroys SDL surface and background sprite*/
	~CScreen()
	{
		m_surface.release();
		m_bg.release();
	}

	/*! Init SDL, set open GL attributes, set up viewport, set up surface, set window title*/
	void Init()
	{
		SDL_Init(SDL_INIT_EVERYTHING);

		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, DEFAULT_RGB_BITS);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, DEFAULT_RGB_BITS);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, DEFAULT_RGB_BITS);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, DEFAULT_RGB_BITS);
		SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, DEFAULT_BUFFER_SIZE);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, DEFAULT_DEPTH);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, TRUE);

		SDL_WM_SetCaption("Test", NULL);
		m_surface.reset(SDL_SetVideoMode(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, 32, SDL_OPENGL));
		glClearColor(1.0, 1.0, 1.0, 1.0);
		m_stViewport.Reset(0, 0, DEFAULT_SCREEN_HEIGHT, DEFAULT_SCREEN_WIDTH, true);

		glShadeModel(GL_SMOOTH);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glDisable(GL_DEPTH_TEST); //for now

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	/*! Currently just loads the background sprite.*/
	void Create()
	{
		m_bg->Load("background.png");//tbr
	}

	/*! Draws the default screen image - aka. the background image*/
	void Draw()
	{
		m_bg->Draw();
	}
};

enum GAMESTATE
{
	STATE_STARTING = 0,	//game object has just been created
	STATE_LOAD_FAIL = 1,	//stuff fails to initialize
	STATE_RUNNING = 2,	//initialization successful. normal typical wandering around doing stuff
	STATE_TRANSITION = 3, //switching rooms, suspend input temporarily, some reinitialization needed
	STATE_EXITING = 4		//stuff is shutting down and getting cleaned up.
};

//a room or menu with its own rules for input handling and stuff to draw
//this base class is for a normal, wandering around doing whatever room
//i mean really this isn't a "room", it's like...a state container or something. but I felt like 
//i would lose my fucking mind less if i had to type out CRoom a thousand times
//as opposed to CStateContainer or some shit
//tbr...eventually "rooms" will be loaded from somewhere like an xml file or something
class CRoom
{
protected:
	std::string m_strID; //unique id for the room or menu so we can find it in a map later
	std::vector<CObject *> objects; //objects associated with this room
public:
	
	CRoom()
	{
	}

	/*! Gives room specified id*/
	CRoom(std::string id)
	{
		m_strID = id;
	}

	/*! Currently just deletes created objects. Called when room is left. Really should save the state of persistent objects though.*/
	~CRoom()
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
					//nextRoom = "Inventory";
				}
				break;
				case SDLK_LCTRL:
				case SDLK_RCTRL:
				{
					printf("\nhere if we're in the right state, we'd bring up the save menu\n");
					//	nextRoom = "SaveLoad";
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

class Game
{
protected:
	GAMESTATE m_nState;
	std::unique_ptr<CScreen> m_screen;
	std::vector<CObject *> objects; //stuff that needs to be drawn. obviously will change. will mostly be used for permanent objects like player
	std::string m_strRoomID;
	std::map<std::string, CRoom*> m_rooms;
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
		for (std::map<std::string, CRoom *>::iterator it = m_rooms.begin(); it != m_rooms.end(); ++it)
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
		m_strRoomID = "TestRoom";
		m_rooms[m_strRoomID] = new CRoom(m_strRoomID); //add a test room

													   //for when we're loading the objects from config files in the future
		if ((objects.size() == 0) || (m_rooms.size() == 0))
			m_nState = STATE_LOAD_FAIL;
		else
		{
			m_nState = STATE_RUNNING;
			m_rooms[m_strRoomID]->Init();
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
			m_strRoomID = m_rooms[m_strRoomID]->GetTransitionData();
			if (m_rooms.find(m_strRoomID) == m_rooms.end())
			{
				//either we're exiting or a thing happened so....
				//eventually prompt the user to save though of course
				m_nState = STATE_EXITING;
			}
			else
			{
				//tbr...save the previous room persistent items first
				//also check if room is already initialized
				m_rooms[m_strRoomID]->Init();
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
			m_nState = m_rooms[m_strRoomID]->HandleInput(objects);
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
		return m_rooms[m_strRoomID]->Update(objects);
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
		m_rooms[m_strRoomID]->Draw(); //tbr...i actually want room to be responsible for drawing
		Utils::EndDraw();
	}
};

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