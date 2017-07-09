#pragma once

/* base class for an image. despite its name it doesn't just have to be for sprites.
it's also used for the background object for example.
*/
class CSprite
{
protected:
	unsigned int m_uiCurrentTexture;
	//i don't know what x and y are for. i guess if this is a background, it's the location of the background
	//if not, i guess it's the origin of the sprite (upper left corner) relative to the local coordinate system
	//(attached to the object)
	int x;
	int y;
	int w;
	int h;
public:
	int GetWidth()
	{
		return w;
	}
	int GetHeight()
	{
		return h;
	}
public:
	CSprite();
	CSprite(int x0, int y0);
	CSprite(int x0, int y0, int width, int height);

	~CSprite();

	GLuint Load(std::string filename);

	//tbr...add tracking ability later ...i think i'll put it in update code though not draw code

	//draws at the specified location when currx & curry are specified.
	//basically you want to call with object coords when the sprite represents an object.
	//if it's a background though then just leave it.
	void Draw(int currx = -1, int curry = -1);

};

