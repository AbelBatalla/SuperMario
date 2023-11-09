#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"



Game::~Game() {
	if (menu != NULL) delete menu;
}


void Game::init()
{
	bPlay = true;
	goGame = false;
	goInstructions = false;
	goCredits = false;
	spaceRelease = true;
	glClearColor(0.36f, 0.58f, 0.988f, 1.0f);
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	menu = new Menu();
	instructions = new SimpleView(SimpleView::TypeMenu::INSTRUCTIONS);
	credits = new SimpleView(SimpleView::TypeMenu::CREDITS);
	
	menu->init();
	instructions->init();
	credits->init();
	scene.init();
}

bool Game::update(int deltaTime)
{
	if (goGame) scene.update(deltaTime);
	else if (goInstructions) goInstructions = instructions->update();
	else if (goCredits) goCredits = credits->update();
	else idMenu = menu->update();
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if (goGame) {
		scene.render();
	}
	else if (goInstructions) {
		instructions->render();
	}
	else if (goCredits) {
		credits->render();
	}
	else menu->render();
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
	if (idMenu == 1 && key == 13) goGame = true;
	if (idMenu == 2 && key == 13) goInstructions = true;
	if (idMenu == 3 && key == 13) goCredits = true;
	if (key == 66 || key == 98) goGame = false;
	if (key == ' ') {
		if (spaceRelease) {
			keys[key] = true;
			spaceRelease = false;
		}
	} else { 
		keys[key] = true;
	}

}

void Game::keyReleased(int key)
{
	keys[key] = false;
	if (key == ' ') spaceRelease = true;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}

void Game::setSpace(bool set)
{
	keys[' '] = set;
}




