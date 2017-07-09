#pragma once

typedef struct
{
	//shit so we know how to handle a collision
	//basically everything has these properties, even non-solid stuff
	//in case we need to trigger events or something
	std::string id; //a unique name in case someone does something different for a specific object
					//	std::string type; //i mean really this will probably just be about as unique as the class the thing is but you never know. keeping it here just in case
					//i don't know what else goes here.
					//map of properties?
					//whatever there will be more than just an id.
} obstructionData;

//a base class so we can do some casting magic and keep everyone in the same vector
class CObject
{
protected:
	//for now i'm just using the sprite dimensions/coordinates as the object coordinates
	//since we care about them for like mostly two reasons: seeing the thing or colliding with the thing.
	std::unique_ptr<CSprite> m_currentSprite; //the one that's on the screen.
	int x;
	int y;
	int vx;
	int vy;
	int speed;
	std::string m_strTexture;
	bool m_bSolid;
	std::string m_strID;
public:
	CObject();
	CObject(int x0, int y0, std::string id, int speed0 = 0, std::string filename = "object.png");

	~CObject();

	bool IsSolid();

	//if not solid, bounding box will be all 0s
	void GetBoundingBox(LPRECT r);

	//gets the center of the object in absolute coordinates.
	//well the center of the sprite i guess for now
	//this is kind of like us assuming the center of mass is in the center
	//because we're boring
	virtual void GetCenter(POINT * p);

	//tbr return collision type
	virtual DIRECTION CheckCollision(CObject * obstruction);

	//ok i know right now this thing is really small but like
	//one day it probably won't be.
	//so let's just do this right the first time and use a pointer.
	//caller is responsible for allocating and deleting the thing
	virtual void RequestObstructionData(obstructionData * data);

	//probably eventually this will be empty
	//except for maybe a list of generic collision event handlers that
	//i may or may not call depending on what's specified in my eventual config files.
	virtual void HandleCollision(CObject * obstruction, DIRECTION dir); //do something with collision data

	virtual void Init();

	//called by the sdl event handler
	//changes stuff
	//should we return the game state here or?
	virtual void HandleInput(SDL_Event event);

	//determines the result of the changes
	virtual std::string Update(std::string defaultRoom);
	virtual void Draw();
};