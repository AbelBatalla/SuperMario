#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Score.h"
#include "Game.h"

void Score::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int s) {
	spritesheet.loadFromFile("images/tilesheetv3.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625, 0.0625), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 6);
	if (s == 100) sprite->addKeyframe(0, glm::vec2(0.f, 0.3125f));
	else if (s == 200) sprite->addKeyframe(0, glm::vec2(0.0625f, 0.3125f));
	else if (s == 500) sprite->addKeyframe(0, glm::vec2(0.f, 0.375f));
	else sprite->addKeyframe(0, glm::vec2(0.0625f, 0.375f));
	sprite->changeAnimation(0, 0);

	tileMapDispl = tileMapPos;
}

void Score::setPosition(const glm::vec2& posScore)
{
	pos = posScore;
	initialY = pos.y;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posScore.x), float(tileMapDispl.y + posScore.y)));
}


void Score::render(int offset) const
{
	sprite->render(offset);
}

bool Score::update(int deltatime) {
	pos.y -= 1;
	if (initialY - pos.y >= 64) return false;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
	sprite->update(deltatime, false, 1);
	return true;
}