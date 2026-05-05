
/* gamatet.cpp
 *
 * Autor: Marko Gaćeša
 * Datum: 05.06.2007.
 * Datum: 05.05.2026.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <GL/glut.h>
#include <GL/gl.h>

#include "timer.h"
#include "tet3d.h"
#include "menu.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const int TIMER_PERIOD = 20; // milisekundi

GLfloat LightAmbient[]  =	{ 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[]  =	{ 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[] =	{ 0.0f, 0.0f, 0.0f, 1.0f };

GLfloat LightAmbientMenu[]  =	{ 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat LightDiffuseMenu[]  =	{ 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPositionMenu[] =	{ 0.0f, 0.0f, 2.0f, 1.0f };

GLfloat cameraH = 0.0f; // horizontalni ugao (rad)
GLfloat cameraV = 0.0f; // vertikalni ugao (rad)
GLfloat cameraD = 1.0f;

int viewW = 0;
int viewH = 0;
bool viewFullScreen = true;

//------//
// game //
//------//

class Game
{
private:
	Game();
public:

	enum Modes { MODE_GAME, MODE_MENU, MODE_HELP };

	static Tet3D* tet;
	static Menu* menu;

	enum MenuCode { START, EXIT, HELP, MAIN, LEVEL, QUIT, SETUP, CUSTOM, WIDTH, HEIGHT, DEPTH, BLOCKSET };

	static Menu* menuMain;
	static Menu* menuLevel;
	static Menu* menuQuit;
	static Menu* menuSetup;
	static Menu* menuCustom;
	static Menu* menuWidth;
	static Menu* menuHeight;
	static Menu* menuDepth;
	static Menu* menuBlockSet;

	static Modes mode;
	static int width;
	static int height;
	static int depth;

	static int startLevel;

	static Tet3D::BlockSet blockSet;

	static void init();

	static void menuSelect(MenuCode code);
	static void select();
	static void abort();

	static void help();

	static void startMenu();
	static void startGame();

	static void flatAct() { width = 5; height = 5; depth = 12; blockSet = Tet3D::FLAT; }
	static void basicAct() { width = 3; height = 3; depth = 10; blockSet = Tet3D::BASIC; }
	static void extAct() { width = 5; height = 5; depth = 10; blockSet = Tet3D::EXTENDED; }

	static bool flatSel() { return width == 5 && height == 5 && depth == 12 && blockSet == Tet3D::FLAT; }
	static bool basicSel() { return width == 3 && height == 3 && depth == 10 && blockSet == Tet3D::BASIC; }
	static bool extSel() { return width == 5 && height == 5 && depth == 10 && blockSet == Tet3D::EXTENDED; }
};

Tet3D* Game::tet = nullptr;
Menu* Game::menu = nullptr;

Menu* Game::menuMain     = nullptr;
Menu* Game::menuLevel    = nullptr;
Menu* Game::menuQuit     = nullptr;
Menu* Game::menuSetup    = nullptr;
Menu* Game::menuCustom   = nullptr;
Menu* Game::menuWidth    = nullptr;
Menu* Game::menuHeight   = nullptr;
Menu* Game::menuDepth    = nullptr;
Menu* Game::menuBlockSet = nullptr;

Game::Modes Game::mode = MODE_MENU;

int Game::width  = 5;
int Game::height = 5;
int Game::depth  = 12;

int Game::startLevel = 0;

Tet3D::BlockSet Game::blockSet = Tet3D::FLAT;

void Game::init()
{
	menuMain = new Menu(QUIT);
	menuMain->add(new MenuItemWHDB(LEVEL, "start", &width, &height, &depth, &blockSet));
	menuMain->add(new MenuItem(SETUP, "change setup"));
	menuMain->add(new MenuItem(HELP, "keyboard help"));
	menuMain->add(new MenuItem(EXIT, "quit gamatet"));

	menuLevel = new Menu(MAIN);
	menuLevel->add(new MenuItemSet(START, "level 0", &startLevel, 0));
	menuLevel->add(new MenuItemSet(START, "level 1", &startLevel, 1));
	menuLevel->add(new MenuItemSet(START, "level 2", &startLevel, 2));
	menuLevel->add(new MenuItemSet(START, "level 3", &startLevel, 3));
	menuLevel->add(new MenuItemSet(START, "level 4", &startLevel, 4));
	menuLevel->add(new MenuItemSet(START, "level 5", &startLevel, 5));
	menuLevel->add(new MenuItemSet(START, "level 6", &startLevel, 6));
	menuLevel->add(new MenuItemSet(START, "level 7", &startLevel, 7));
	menuLevel->add(new MenuItemSet(START, "level 8", &startLevel, 8));
	menuLevel->add(new MenuItemSet(START, "level 9", &startLevel, 9));

	menuQuit = new Menu(EXIT);
	menuQuit->add(new MenuItem(EXIT, "quit"));
	menuQuit->add(new MenuItem(MAIN, "cancel"));

	menuSetup = new Menu(MAIN);
	menuSetup->add(new MenuItemAdv(MAIN, "flat fun 5x5x12f", flatAct, flatSel));
	menuSetup->add(new MenuItemAdv(MAIN, "3d mania 3x3x10b", basicAct, basicSel));
	menuSetup->add(new MenuItemAdv(MAIN, "out of control 5x5x10e", extAct, extSel));
	menuSetup->add(new MenuItemCustom(CUSTOM, "custom setup", &width, &height, &depth, &blockSet));
	menuSetup->add(new MenuItem(MAIN, "back to main menu"));

	menuCustom = new Menu(SETUP);
	menuCustom->add(new MenuItemX(WIDTH, "width", &width));
	menuCustom->add(new MenuItemX(HEIGHT, "height", &height));
	menuCustom->add(new MenuItemX(DEPTH, "depth", &depth));
	menuCustom->add(new MenuItemXBlock(BLOCKSET, "blockset", &blockSet));
	menuCustom->add(new MenuItem(SETUP, "back"));

	menuWidth = new Menu(CUSTOM);
	menuWidth->add(new MenuItemSet(CUSTOM, "width 3", &width, 3));
	menuWidth->add(new MenuItemSet(CUSTOM, "width 4", &width, 4));
	menuWidth->add(new MenuItemSet(CUSTOM, "width 5", &width, 5));
	menuWidth->add(new MenuItemSet(CUSTOM, "width 6", &width, 6));
	menuWidth->add(new MenuItemSet(CUSTOM, "width 7", &width, 7));
	menuWidth->add(new MenuItemSet(CUSTOM, "width 8", &width, 8));
	menuWidth->add(new MenuItemSet(CUSTOM, "width 9", &width, 9));

	menuHeight = new Menu(CUSTOM);
	menuHeight->add(new MenuItemSet(CUSTOM, "height 3", &height, 3));
	menuHeight->add(new MenuItemSet(CUSTOM, "height 4", &height, 4));
	menuHeight->add(new MenuItemSet(CUSTOM, "height 5", &height, 5));
	menuHeight->add(new MenuItemSet(CUSTOM, "height 6", &height, 6));
	menuHeight->add(new MenuItemSet(CUSTOM, "height 7", &height, 7));
	menuHeight->add(new MenuItemSet(CUSTOM, "height 8", &height, 8));
	menuHeight->add(new MenuItemSet(CUSTOM, "height 9", &height, 9));

	menuDepth = new Menu(CUSTOM);
	menuDepth->add(new MenuItemSet(CUSTOM, "depth  6", &depth,  6));
	menuDepth->add(new MenuItemSet(CUSTOM, "depth  7", &depth,  7));
	menuDepth->add(new MenuItemSet(CUSTOM, "depth  8", &depth,  8));
	menuDepth->add(new MenuItemSet(CUSTOM, "depth  9", &depth,  9));
	menuDepth->add(new MenuItemSet(CUSTOM, "depth 10", &depth, 10));
	menuDepth->add(new MenuItemSet(CUSTOM, "depth 12", &depth, 12));
	menuDepth->add(new MenuItemSet(CUSTOM, "depth 14", &depth, 14));
	menuDepth->add(new MenuItemSet(CUSTOM, "depth 16", &depth, 16));
	menuDepth->add(new MenuItemSet(CUSTOM, "depth 18", &depth, 18));
	menuDepth->add(new MenuItemSet(CUSTOM, "depth 20", &depth, 20));

	menuBlockSet = new Menu(CUSTOM);
	menuBlockSet->add(new MenuItemSetBlock(CUSTOM, "flat", &blockSet, Tet3D::FLAT));
	menuBlockSet->add(new MenuItemSetBlock(CUSTOM, "basic", &blockSet, Tet3D::BASIC));
	menuBlockSet->add(new MenuItemSetBlock(CUSTOM, "extended", &blockSet, Tet3D::EXTENDED));

	menu = menuMain;
	menu->start();

	mode = MODE_MENU;
}

void Game::menuSelect(const MenuCode code)
{
	switch (code)
	{
	default:
	case EXIT: exit(0); break;
	case START: startGame(); return;
	case HELP: help(); return;
	case MAIN: menu = menuMain; break;
	case LEVEL: menu = menuLevel; break;
	case QUIT: menu = menuQuit; break;
	case SETUP: menu = menuSetup; break;
	case CUSTOM: menu = menuCustom; break;
	case WIDTH: menu = menuWidth; break;
	case HEIGHT: menu = menuHeight; break;
	case DEPTH: menu = menuDepth; break;
	case BLOCKSET: menu = menuBlockSet; break;
	}
	menu->start();
}

void Game::select()
{
	menuSelect(static_cast<MenuCode>(menu->activate()));
}

void Game::abort()
{
	menuSelect(static_cast<MenuCode>(menu->cancel()));
}

void Game::help()
{
	mode = MODE_HELP;
}

void Game::startGame()
{
	delete tet;

	if (blockSet == Tet3D::EXTENDED && width  < 5) width  = 5;
	if (blockSet == Tet3D::EXTENDED && height < 5) height = 5;

	tet = new Tet3D();
	tet->generateField(width, height, depth, blockSet);
	tet->setLevel(startLevel);
	tet->gameStart();

	LightPosition[0] = -tet->width() / 3.0f;
	LightPosition[1] = tet->height() / 3.0f;
	LightPosition[2] = tet->depth() / 2.0f * 1.1f;

	cameraH = 0.0f;
	cameraV = 0.0f;

	// proracuni za ugao gledanja 45 stepeni
	const float cDh = 0.5f * (tet->depth() + (tet->height() + 0.1f) * 2.4142f);
	const float cDw = 0.5f * (tet->depth() + (tet->width()  + 0.6f)  * 2.4142f);
	cameraD = cDh > cDw ? cDh : cDw;

	mode = MODE_GAME;
}

void Game::startMenu()
{
	if (tet != nullptr)
	{
		delete tet;
		tet = nullptr;
	}

	cameraH = 0.0f;
	cameraV = 0.0f;
	cameraD = 30.0f;

	menu = menuMain;

	mode = MODE_MENU;
}

//----------//
// textures //
//----------//

void LoadGLTexture(const char* textureName, GLuint *texID)
{
	int width, height, channels;
	unsigned char* data = stbi_load(textureName, &width, &height, &channels, 0);
	if (data == nullptr) return;

	glGenTextures(1, texID);
	glBindTexture(GL_TEXTURE_2D, *texID);

	switch (channels) {
	case 1:
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_INTENSITY8, width, height, GL_RED, GL_UNSIGNED_BYTE, data);
		break;
	case 3:
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
		break;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	stbi_image_free(data);
}

void LoadTextures()
{
	LoadGLTexture("texture/brick.png", &Brick::texture);
	LoadGLTexture("texture/wall.png", &WalledBrickField::texture);
	MenuChar::loadTextures();
	Menu::loadTexture();
}

void clean()
{
	if (Brick::texture != 0) glDeleteTextures(1, &Brick::texture);
	if (WalledBrickField::texture != 0) glDeleteTextures(1, &WalledBrickField::texture);
}

//-----------------------//
// OpenGL initialization //
//-----------------------//

void init_all()
{
	//*** init OpenGL ***//

	glClearColor(0.0, 0.0, 0.0, 0.0);
	LoadTextures();

	// shading

	glShadeModel(GL_SMOOTH);

	// faces

	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);

	// textures

	glEnable(GL_TEXTURE_2D);

	// z-buffer

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// alpha blending

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// svetla

	glLightfv(GL_LIGHT1, GL_AMBIENT,  LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE,  LightDiffuse);

	glLightfv(GL_LIGHT2, GL_AMBIENT,  LightAmbientMenu);
	glLightfv(GL_LIGHT2, GL_DIFFUSE,  LightDiffuseMenu);

	glEnable(GL_LIGHTING);

	// materijal

	glEnable(GL_COLOR_MATERIAL);

	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// liste

	Brick::compileList();
}

//--------------------//
// funkcija reshape() //
//--------------------//

void reshape(int width, int height)
{
	viewW = width;
	viewH = height;

	glViewport(0, 0, (GLint) width, (GLint) height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (GLfloat)width/(GLfloat)height, 1.0f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
}

//-----------------//
// funkcija draw() //
//-----------------//

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	gluLookAt(
		cameraD * cos(cameraV) * sin(cameraH), cameraD * sin(cameraV), cameraD * cos(cameraV) * cos(cameraH),
		0, 0, 0,
		0.0f, 1.0f, 0.0f
		);

	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
	glLightfv(GL_LIGHT2, GL_POSITION, LightPositionMenu);

	/* draw light source
	glPushMatrix();
	glTranslatef(LightPosition[0], LightPosition[1], LightPosition[2]);
	//glTranslatef(LightPositionMenu[0], LightPositionMenu[1], LightPositionMenu[2]);
	glColor4f(1.0, 1.0, 0, 1.0);
	glDisable(GL_LIGHTING);
	glutSolidSphere(0.1, 10, 10);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	//*/

	if (Game::mode == Game::MODE_GAME)
	{
		// draw field...

		glEnable(GL_LIGHT1);
		Game::tet->render();
		glDisable(GL_LIGHT1);

		// HUD

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		Game::tet->renderHUD();

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
	}
	if (Game::mode == Game::MODE_MENU)
	{
		glEnable(GL_LIGHT2);
		Game::menu->draw();
		glDisable(GL_LIGHT2);
	}
	if (Game::mode == Game::MODE_HELP)
	{
		Menu::drawHelp();
	}

	// render

	//glFlush();
	glutSwapBuffers();
}

//-----------//
// timerFunc //
//-----------//

void timerFunc(int value)
{
	if (value == 1)
	{
		Timer::progress();
		if (Game::mode == Game::MODE_GAME) Game::tet->animate();
		glutTimerFunc(TIMER_PERIOD, timerFunc, value);
	}
}

//-----------------------------//
// keyboard and mouse handlers //
//-----------------------------//

int oldMouseX = 0;
int oldMouseY = 0;
int diffMouseX = 0;
int diffMouseY = 0;
bool mouseLButton = false;
bool mouseRButton = false;

void mouse_motion(const int x, const int y)
{
	diffMouseX = x - oldMouseX;
	diffMouseY = y - oldMouseY;

	if (mouseLButton && Game::mode == Game::MODE_GAME)
	{
		cameraD += diffMouseY * 0.01f;

		if (cameraD > 40.0f) cameraD = 40.0f;
		if (cameraD <  0.5f) cameraD =  0.5f;
	}

	if (mouseRButton && Game::mode == Game::MODE_GAME)
	{
		cameraH -= diffMouseX * 0.005f;
		cameraV += diffMouseY * 0.005f;

		/* limit camera movement
		const float maxAng = 2.355f; // 3/4 * pi
		if (cameraH >  maxAng) cameraH =  maxAng;
		if (cameraH < -maxAng) cameraH = -maxAng;
		if (cameraV >  maxAng) cameraV =  maxAng;
		if (cameraV < -maxAng) cameraV = -maxAng;
		//*/
	}

	oldMouseX = x;
	oldMouseY = y;
}

void mouse_action(const int button, const int state, const int x, const int y)
{
	//int diffX = x - oldMouseX;
	//int diffY = y - oldMouseY;

	switch(button)
	{
		case GLUT_LEFT_BUTTON:
			switch(state)
			{
				case GLUT_DOWN:
					mouseLButton = true;
					break;

				case GLUT_UP:
					mouseLButton = false;
					break;
			}
			break;

		case GLUT_MIDDLE_BUTTON:
			switch(state)
			{
				case GLUT_DOWN:
					cameraV = 0.0f;
					cameraH = 0.0f;
					break;

				case GLUT_UP:
					break;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			switch(state)
			{
				case GLUT_DOWN:
					mouseRButton = true;
					break;

				case GLUT_UP:
					mouseRButton = false;
					break;
			}
			break;
	}
}

void special_key_down(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_UP:
			if (Game::mode == Game::MODE_GAME) Game::tet->moveY(1);
			else if (Game::mode == Game::MODE_MENU) Game::menu->up();
			break;

		case GLUT_KEY_DOWN:
			if (Game::mode == Game::MODE_GAME) Game::tet->moveY(-1);
			else if (Game::mode == Game::MODE_MENU) Game::menu->down();
			break;

		case GLUT_KEY_LEFT:
			if (Game::mode == Game::MODE_GAME) Game::tet->moveX(-1);
			break;

		case GLUT_KEY_RIGHT:
			if (Game::mode == Game::MODE_GAME) Game::tet->moveX(1);
			break;

		case GLUT_KEY_F1:
			viewFullScreen = !viewFullScreen;
			if (viewFullScreen)
				glutFullScreen();
			else
				glutReshapeWindow(640, 480);
			break;
	}
}

void key_down(unsigned char key, int x, int y)
{
	if (key >= 'A' && key <= 'Z') key += ('a' - 'A');

	switch(key)
	{
		case 27: // escape
			if (Game::mode == Game::MODE_GAME) Game::startMenu();
			else if (Game::mode == Game::MODE_MENU) Game::abort();
			else if (Game::mode == Game::MODE_HELP) Game::startMenu();
			break;

		case 13: // enter
			if (Game::mode == Game::MODE_MENU) Game::select();
			else if (Game::mode == Game::MODE_HELP) Game::startMenu();
			break;

		case 32: // space
			if (Game::mode == Game::MODE_GAME) Game::tet->drop();
			else if (Game::mode == Game::MODE_MENU) Game::select();
			else if (Game::mode == Game::MODE_HELP) Game::startMenu();
			break;

		case 'q':
			if (Game::mode == Game::MODE_GAME) Game::tet->rotateX(true);
			break;
		case 'a':
			if (Game::mode == Game::MODE_GAME) Game::tet->rotateX(false);
			break;

		case 'w':
			if (Game::mode == Game::MODE_GAME) Game::tet->rotateY(true);
			break;
		case 's':
			if (Game::mode == Game::MODE_GAME) Game::tet->rotateY(false);
			break;

		case 'e':
			if (Game::mode == Game::MODE_GAME) Game::tet->rotateZ(true);
			break;
		case 'd':
			if (Game::mode == Game::MODE_GAME) Game::tet->rotateZ(false);
			break;

		case 'z':
			if (Game::mode == Game::MODE_GAME) Game::tet->moveZ(-1);
			break;

		case 'p':
			if (Game::mode == Game::MODE_GAME) Game::tet->pause();
			break;

		case 'b':
			if (Game::mode == Game::MODE_GAME) Game::tet->drawShadows = !Game::tet->drawShadows;
			break;
	}
}

//-----------------//
// funkcija main() //
//-----------------//

int main(int argc, char *argv[])
{
	printf(u8"GaMaTeT 3D by Gaćeša Marko, 2007.\n");

	viewW = 640;
	viewH = 480;

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(viewW, viewH);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("GaMaTeT 3D");

	init_all();

	glutDisplayFunc(draw);
	glutIdleFunc(draw);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key_down);
	glutSpecialFunc(special_key_down);

	glutMouseFunc(mouse_action);
	glutMotionFunc(mouse_motion);
	glutPassiveMotionFunc(mouse_motion);

	glutSetCursor(GLUT_CURSOR_FULL_CROSSHAIR);

	Timer::setPeriod(TIMER_PERIOD);
	glutTimerFunc(TIMER_PERIOD, timerFunc, 1);

	if (viewFullScreen) glutFullScreen();

	Game::init();
	Game::startMenu();

	glutMainLoop();

	return 0;
}
