#pragma once

//collision handling
//basics. for now. will become more complex
namespace Collisions
{
	//check for a single collision between two things
	//eventually these will be solid things but right now they're just objects
	//may not need to specify solid actually
	//returns the side of the collider that made contact with the obstruction
	//well....since we're doing the square bounding box thing for now anyway
	DIRECTION CheckCollision(CObject *collider, CObject *obstruction)
	{
		//right now just uses square bounding boxes
		//might be more complicated eventually but...
		return collider->CheckCollision(obstruction);
	}

	void CopyCollidableObjects(std::vector<CObject *> objects, std::vector<CObject *> &copiedObjects)
	{
		for (std::vector<CObject *>::iterator it = objects.begin(); it != objects.end(); ++it)
		{
			if ((*it)->IsSolid())
			{
				copiedObjects.push_back((*it));
			}
		}

	}

	//return an int representing the direction away from the collision
	//actually let's get rid of that i don't know why i did that
	//i can see stuff being a problem if we get trapped between an enemy and a wall....oh well...sucks to be that person...
	int CheckCollisions(std::vector<CObject *> gameObjects)
	{
		DIRECTION dir = DIR_EAST;
		//look at the current object
		//go through the list and look for any objects it's colliding with

		//make a copy of the game objects
		std::vector<CObject *> copiedObjects;
		CopyCollidableObjects(gameObjects, copiedObjects);

		//loop through game objects
		//for (int i = 0; i < copiedObjects.size(); i++)
		while (copiedObjects.size() > 1) //no point in doing this if there are no objects or only one object
		{
			for (std::vector<CObject *>::iterator it = copiedObjects.begin(); it != copiedObjects.end(); ++it)
			{
				//if theres a collision i guess just execute the relative collision handlers
				if ((*it) != copiedObjects.at(0))
				{
					DIRECTION d = CheckCollision(copiedObjects.at(0), (*it));
					if (d != DIR_NONE)
					{
						copiedObjects.at(0)->HandleCollision((*it), d);
						//also make sure the other guy doesnt want to do something
						(*it)->HandleCollision(copiedObjects.at(0), d);
					}
				}
			}
			//we checked all possible collisions for this object, so it can go away from our copy
			//note....this is not gonna be efficient if i have a ton of objects but we'll cross that bridge when we get to it.
			//thinking a list might be better but for now this is fine
			copiedObjects.erase(copiedObjects.begin());
		}

		return dir;
	}
};