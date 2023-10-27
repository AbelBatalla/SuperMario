#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "Menu.h"
#include "Game.h"
#include <GL/glut.h>


Menu::Menu() {
	currentTime = 0;
	idMenu = 1;
	menu_1.loadFromFile("images/menu_1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	menu_2.loadFromFile("images/menu_2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	menu_3.loadFromFile("images/menu_3.png", TEXTURE_PIXEL_FORMAT_RGBA);
	menu = menu_1;
}

Menu::~Menu() {
}

void Menu::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Menu::init() {
	initShaders();
	bground_f = Sprite::createSprite(glm::ivec2(640, 480), glm::vec2(1.0, 1.0), &menu, &texProgram);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	
}


int Menu::update() {
	static int cooldown = 0; // Variable para controlar el tiempo de espera
	const int cooldownLimit = 10; // Ajusta esto según tus necesidades

	if (cooldown > 0) {
		cooldown--; // Reducir el tiempo de espera
	}
	else {
		if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
			if (idMenu < 3) idMenu += 1;
			cooldown = cooldownLimit; // Establecer un tiempo de espera
		}
		if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
			if (idMenu > 1) idMenu -= 1;
			cooldown = cooldownLimit; // Establecer un tiempo de espera
		}
	}

	if (idMenu == 1) menu = menu_1;
	else if (idMenu == 2) menu = menu_2;
	else if (idMenu == 3) menu = menu_3;
	this->init();
	return idMenu;
}



void Menu::render() {
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	bground_f->render(0);
}