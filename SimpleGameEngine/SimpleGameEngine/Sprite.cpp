#include "stdafx.h"
#include "Sprite.h"

CSprite::CSprite()
{
	m_uiCurrentTexture = 0;
	x = 0;
	y = 0;
	w = DEFAULT_TILESIZE;
	h = DEFAULT_TILESIZE;
}

CSprite::CSprite(int x0, int y0)
{
	m_uiCurrentTexture = 0;
	x = x0;
	y = y0;
	w = DEFAULT_TILESIZE;
	h = DEFAULT_TILESIZE;
}

CSprite::CSprite(int x0, int y0, int width, int height)
{
	m_uiCurrentTexture = 0;
	x = x0;
	y = y0;
	w = width;
	h = height;
}

CSprite::~CSprite()
{

}

GLuint CSprite::Load(std::string filename)
{
	//TBR...need to add a member for filename and also not have one thing only and handle when we don't have a thing.
	SDL_Surface * surf = IMG_Load(filename.c_str());
	//surf = SDL_LoadBMP("C:\\Users\\nopassword\\Documents\\Visual Studio 2015\\Projects\\BasicEngine\\BasicEngine\\um.bmp");
	if (!surf)
	{
		m_uiCurrentTexture = 0;
		return 0;
	}
	//SDL_DisplayFormatAlpha(surf);
	//unsigned object(0);

	glGenTextures(1, &m_uiCurrentTexture);
	glBindTexture(GL_TEXTURE_2D, m_uiCurrentTexture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surf->w, surf->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surf->pixels);

	SDL_FreeSurface(surf);
	return m_uiCurrentTexture;
}

//tbr...add tracking ability later ...i think i'll put it in update code though not draw code

//draws at the specified location when currx & curry are specified.
//basically you want to call with object coords when the sprite represents an object.
//if it's a background though then just leave it.
void CSprite::Draw(int currx, int curry)
{
	if (currx < 0 || curry < 0)
	{
		currx = x;
		curry = y;
	}
	glColor4ub(255, 255, 255, 255);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_uiCurrentTexture);

	//draw bg over whole screen
	glBegin(GL_QUADS);

	glTexCoord2d(0, 0); //coord in texture range from 0 to 1
	glVertex2f(currx, curry);
	glTexCoord2d(1, 0);
	glVertex2f(currx + w, curry);
	glTexCoord2d(1, 1);
	glVertex2f(currx + w, curry + h);
	glTexCoord2d(0, 1);
	glVertex2f(currx, curry + h);

	glEnd();
	glDisable(GL_TEXTURE_2D);
}
