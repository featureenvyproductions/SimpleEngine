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
	CObject()
	{
		x = 0;
		y = 0;
		vx = 0;
		vy = 0;
		speed = 2;
		m_strTexture = "object.png";
		m_currentSprite.reset(new CSprite(x, y));
		m_bSolid = true;// false;
	}
	CObject(int x0, int y0, std::string id, int speed0 = 0, std::string filename = "object.png")
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

	~CObject()
	{
		m_currentSprite.release();
	}

	bool IsSolid()
	{
		return m_bSolid;
	}

	//if not solid, bounding box will be all 0s
	void GetBoundingBox(LPRECT r)
	{
		//default bounding box
		r->left = x;
		r->right = x + m_currentSprite->GetWidth();
		r->top = y;
		r->bottom = y + m_currentSprite->GetHeight();
	}

	//gets the center of the object in absolute coordinates.
	//well the center of the sprite i guess for now
	//this is kind of like us assuming the center of mass is in the center
	//because we're boring
	virtual void GetCenter(POINT * p)
	{
		//so far i'm only having it to where our origin is in the top left corner of all the things so
		p->x = x + m_currentSprite->GetWidth() / 2;
		p->y = y + m_currentSprite->GetHeight() / 2;
	}

	//tbr return collision type
	virtual DIRECTION CheckCollision(CObject * obstruction)
	{
		RECT r;
		r.bottom = 0;
		r.left = 0;
		r.right = 0;
		r.top = 0;
		obstruction->GetBoundingBox(&r);
		POINT p;
		p.x = 0;
		p.y = 0;
		obstruction->GetCenter(&p);
		if (r.top - r.bottom <= 0)
		{
			//check if within x range
			if (((x >= r.left) && (x <= r.right)) || ((x + m_currentSprite->GetWidth() >= r.left) && (x + m_currentSprite->GetWidth() <= r.right)))
			{
				//check within y range
				if (((y >= r.top) && (y <= r.bottom)) || ((y + m_currentSprite->GetHeight() >= r.top) && (y + m_currentSprite->GetHeight() <= r.bottom)))
				{
					//return true;
					//what quadrant is the obstruction in generally?
					//transform our origin real quick
					POINT center;
					GetCenter(&center);
					//now go figure out the direction of the thing.
					//i have a function for finding out the exact angle the line between the objects
					//makes with the horizontal
					//but for the default, lets just figure out a general direction (based on the quadrant).
					return Utils::GetDirection(center, p);
				}
			}
		}
		return DIR_NONE;
	}

	//ok i know right now this thing is really small but like
	//one day it probably won't be.
	//so let's just do this right the first time and use a pointer.
	//caller is responsible for allocating and deleting the thing
	virtual void RequestObstructionData(obstructionData * data)
	{
		//does nothing right now but eventually will populate a collision data struct
		printf("\requested obstruction data for object %p\n", this);
		data->id = m_strID;
	}

	//probably eventually this will be empty
	//except for maybe a list of generic collision event handlers that
	//i may or may not call depending on what's specified in my eventual config files.
	virtual void HandleCollision(CObject * obstruction, DIRECTION dir) //do something with collision data
	{
		//probably there will be a thing where we get the collision data we need 
		//the object name and direction (relative to us) where the collision occurred
		//object type

		//then do stuff like change color or whatever it is we decide to do 
		obstructionData data;
		obstruction->RequestObstructionData(&data);

		//but really for now we're just gonna print something.
		printf("\nhi im %p and im colliding with object %p\n", this, obstruction);

		//and bounce in opposite direction depending on location of obstruction.
		//it pains me to ignore the fuck out of real physics but this is a 2d rpg about eating shadows so....
		//also may have to tweak the bounce stuff and make it so the thing doesn't like KEEP bouncing
		//before the user has time to lift off the key.
		switch (dir)
		{
		case DIR_WEST:
		case DIR_EAST:
		{
			x -= vx;
			vx = 0;
		}
		break;
		case DIR_SOUTH:
		case DIR_NORTH:
		{
			y -= vy;
			vy = 0;
		}
		break;
		case DIR_NORTHEAST:
		case DIR_NORTHWEST:
		case DIR_SOUTHWEST:
		case DIR_SOUTHEAST:
		{
			x -= vx;
			vx = 0;
			y -= vy;
			vy = 0;
		}
		break;
		}
	}

	virtual void Init()
	{
		m_currentSprite->Load(m_strTexture);
	}

	//called by the sdl event handler
	//changes stuff
	//should we return the game state here or?
	virtual void HandleInput(SDL_Event event)
	{
		switch (event.type)
		{
		case SDL_KEYDOWN: //note: doesn't handle key staying pressed.
		{
			//I do it this way to handle diagonal motion hopefully
			if (event.key.keysym.sym == SDLK_LEFT)
			{
				vx = -speed;
			}
			if (event.key.keysym.sym == SDLK_RIGHT)
			{
				vx = speed;
			}
			if (event.key.keysym.sym == SDLK_UP)
			{
				vy = -speed;
			}
			if (event.key.keysym.sym == SDLK_DOWN)
			{
				vy = speed;
			}
		}
		break;
		case SDL_KEYUP: //note: doesn't handle key staying pressed.
		{
			if ((event.key.keysym.sym == SDLK_LEFT) || (event.key.keysym.sym == SDLK_RIGHT))
			{
				vx = 0;
			}
			if ((event.key.keysym.sym == SDLK_UP) || (event.key.keysym.sym == SDLK_DOWN))
			{
				vy = 0;
			}
		}
		break;
		}
	}
	//determines the result of the changes
	virtual std::string Update(std::string defaultRoom)
	{
		std::string ret = defaultRoom;
		x += vx;
		y += vy;
		if (x > DEFAULT_SCREEN_WIDTH - m_currentSprite->GetWidth())
		{
			x = DEFAULT_SCREEN_WIDTH - m_currentSprite->GetWidth();
		}
		if (y > DEFAULT_SCREEN_HEIGHT - m_currentSprite->GetHeight())
		{
			y = DEFAULT_SCREEN_HEIGHT - m_currentSprite->GetHeight();
		}
		if (x < 0)
		{
			x = 0; //tbr...need to account for coordinate system where sprite is.
		}
		if (y < 0)
		{
			y = 0;
		}
		return ret; //note: this is here in case we want a room transition
	}
	virtual void Draw()
	{
		//draw our sprite where we are
		m_currentSprite->Draw(x, y);
	}
};