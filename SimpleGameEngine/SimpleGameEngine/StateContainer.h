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

	CStateContainer();

	/*! Gives room specified id*/
	CStateContainer(std::string id);

	/*! Currently just deletes created objects. Called when room is left. Really should save the state of persistent objects though.*/
	~CStateContainer();

	/*! Room objects loaded here. Called when room is entered. Eventually needs to look at persistent state of objects*/
	virtual bool Init();

	/*! Called by main game code to do its own input handling. Handles input before main input handler*/
	virtual GAMESTATE HandleInput(std::vector<CObject *> permObjects);

	//i guess room is responsible for doing like everything itself
	virtual GAMESTATE Update(std::vector<CObject *> permObjects);//pass in the game object objects so we can check for collisions etc

	virtual void Draw();

	//calls handle input and update both
	virtual GAMESTATE ProcessInput(std::vector<CObject *> permObjects);

	virtual std::string GetTransitionData();
};
