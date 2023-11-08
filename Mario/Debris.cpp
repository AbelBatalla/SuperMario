#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Debris.h"
#include "Game.h"

void Debris::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap) {
	spritesheet.loadFromFile("images/itembricktiles.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(0.03125, 0.03125), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 12);
	sprite->addKeyframe(0, glm::vec2(0.90625f, 0.3125f));
	sprite->addKeyframe(0, glm::vec2(0.875f, 0.3125f));
	sprite->addKeyframe(0, glm::vec2(0.90625f, 0.34375f));
	sprite->addKeyframe(0, glm::vec2(0.875f, 0.34375f));
	sprite->changeAnimation(0, 0);

	startAnimation = false;
	up = true;
	acum = 0;
	tileMapDispl = tileMapPos;
	xCount = 0;
	distX = 8;
	distY = 8;
	jumpAngle = 0;
}


bool Debris::update(int deltatime) {

	++xCount;
	if (xCount % 2 == 0) {
		pos.x -= 1;
		distX += 2;
	}
	if (xCount > 64) return false;

	//---------------------------------
	jumpAngle += 4;
	if (jumpAngle <= 90) {
		pos.y = int(startY - 56 * sin(3.14159f * jumpAngle / 180.f));
		distY = int(startY - 28 * sin(3.14159f * jumpAngle / 180.f));
	}
	else {
		pos.y += 2;
		distY += 2;
	}

	//---------------------------------

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
	sprite->update(deltatime, false, 1);
	return true;
}

int Debris::type() {
	return 3;
}

void Debris::setPosition(const glm::vec2& position)
{
	pos = position;
	startY = pos.y;
	distY = pos.y + 8;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

void Debris::render(int offset) const
{
	sprite->render(offset);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x + distX), float(tileMapDispl.y + pos.y)));
	sprite->render(offset);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x + distX), float(tileMapDispl.y + distY)));
	sprite->render(offset);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + distY)));
	sprite->render(offset);
}