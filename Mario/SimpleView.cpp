#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "SimpleView.h"
#include "Game.h"
#include <GL/glut.h>




SimpleView::SimpleView(TypeMenu type) {
	if(type == CREDITS)	menu.loadFromFile("images/credits.png", TEXTURE_PIXEL_FORMAT_RGBA);
	else if(type == INSTRUCTIONS) menu.loadFromFile("images/instructions.png", TEXTURE_PIXEL_FORMAT_RGBA);
}

SimpleView::~SimpleView() {
}

void SimpleView::initShaders()
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

void SimpleView::init() {
	initShaders();
	bground_f = Sprite::createSprite(glm::ivec2(640, 480), glm::vec2(1.0, 1.0), &menu, &texProgram);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);

}


int SimpleView::update() {

	if (Game::instance().getKey('B') || Game::instance().getKey('b')) return false;
	return true;
		
}



void SimpleView::render() {
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	bground_f->render();
}