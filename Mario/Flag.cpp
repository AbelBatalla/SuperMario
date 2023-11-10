#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Flag.h"
#include "Game.h"

void Flag::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int ftype) {
	spritesheet.loadFromFile("images/tilesheetv3.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625f, 0.0625f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 6);
	sprite->addKeyframe(0, glm::vec2(ftype == 0 ? 0.875f : 0.9375f, 0.25f));

	sprite->changeAnimation(0, 0);

	activated = false;
	type = ftype;
	tileMapDispl = tileMapPos;
}

void Flag::setPosition(const glm::vec2& pos)
{
	posFlag = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posFlag.x), float(tileMapDispl.y + posFlag.y)));
}

void Flag::render(int offset) const
{
	sprite->render(offset);
}

void Flag::update(int deltatime) {
	if (activated) {
		if (type == 0) {
			posFlag.y += 2;
			if (posFlag.y >= 256) activated = false;
		}
		else {
			posFlag.y -= 1;
			if (posFlag.y <= 192) activated = false;
		}
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posFlag.x), float(tileMapDispl.y + posFlag.y)));
	sprite->update(deltatime, false, 1);
}

void Flag::activate() {
	activated = true;
}

bool Flag::getActivate() {
	return activated;
}