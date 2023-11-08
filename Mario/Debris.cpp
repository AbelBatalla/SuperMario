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
}


bool Debris::update(int deltatime) {
	if (up) {
		pos.y -= 1;
		acum -= 1;
		if (acum <= -48) {
			up = false;
		}
	}
	else {
		pos.y += 1;
		acum += 1;
		if (acum >= -8) {
			up = true;
			pos.y -= acum;
			acum = 0;
			return false;
		}
	}
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
	pos.y -= 16;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}