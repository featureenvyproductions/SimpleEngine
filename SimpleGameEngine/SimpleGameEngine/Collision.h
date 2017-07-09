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
	DIRECTION CheckCollision(CObject *collider, CObject *obstruction);

	void CopyCollidableObjects(std::vector<CObject *> objects, std::vector<CObject *> &copiedObjects);

	//return an int representing the direction away from the collision
	//actually let's get rid of that i don't know why i did that
	//i can see stuff being a problem if we get trapped between an enemy and a wall....oh well...sucks to be that person...
	int CheckCollisions(std::vector<CObject *> gameObjects);

};