#include "stdafx.h"
#include "Screen.h"

/*! Inits the SDL surface, background sprite, and viewport*/
CScreen::CScreen()
{
	m_surface.reset();
	m_bg.reset(new CSprite(0, 0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT));
	m_stViewport.Reset(0, 0, DEFAULT_SCREEN_HEIGHT, DEFAULT_SCREEN_WIDTH);
}

/*! Destroys SDL surface and background sprite*/
CScreen::~CScreen()
{
	m_surface.release();
	m_bg.release();
}

/*! Init SDL, set open GL attributes, set up viewport, set up surface, set window title*/
void CScreen::Init()
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
void CScreen::Create()
{
	m_bg->Load("background.png");//tbr
}

/*! Draws the default screen image - aka. the background image*/
void CScreen::Draw()
{
	m_bg->Draw();
}