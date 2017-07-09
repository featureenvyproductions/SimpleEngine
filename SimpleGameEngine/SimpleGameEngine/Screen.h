#pragma once

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
	CScreen();

	/*! Destroys SDL surface and background sprite*/
	~CScreen();

	/*! Init SDL, set open GL attributes, set up viewport, set up surface, set window title*/
	void Init();

	/*! Currently just loads the background sprite.*/
	void Create();

	/*! Draws the default screen image - aka. the background image*/
	void Draw();
};