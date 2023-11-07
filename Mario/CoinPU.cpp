#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "CoinPU.h"
#include "Game.h"

void CoinPU::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap) {
	spritesheet.loadFromFile("images/tilesheetv3.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625, 0.0625), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 32);
	sprite->addKeyframe(0, glm::vec2(0.8125f, 0.0625f));
	sprite->addKeyframe(0, glm::vec2(0.75f, 0.0625f));
	sprite->addKeyframe(0, glm::vec2(0.6875f, 0.0625f));
	sprite->addKeyframe(0, glm::vec2(0.625f, 0.0625f));
	sprite->changeAnimation(0, 0);

	startAnimation = false;
	up = true;
	acum = 0;
	tileMapDispl = tileMapPos;
}


bool CoinPU::update(int deltatime) {
	if (up) {
		pos.y -= 3;
		acum -= 3;
		if (acum <= -48) {
			up = false;
		}
	}
	else {
		pos.y += 3;
		acum += 3;
		if (acum >= -8) {
			return false;
		}
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
	sprite->update(deltatime, false, 1);
	return true;
}

int CoinPU::type() {
	return 2;
}

void CoinPU::setPosition(const glm::vec2& position)
{
	pos = position;
	pos.y -= 16;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}