#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "FlagCover.h"
#include "Game.h"

void FlagCover::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
	spritesheet.loadFromFile("images/tilesheetv3.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 32), glm::vec2(0.0625f, 0.125f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 6);
	sprite->addKeyframe(0, glm::vec2(0.0625f, 0.75f));

	sprite->changeAnimation(0, 0);

	tileMapDispl = tileMapPos;
}

void FlagCover::setPosition(const glm::vec2& pos)
{
	posCover = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCover.x), float(tileMapDispl.y + posCover.y)));
}

void FlagCover::render(int offset) const
{
	sprite->render(offset);
}

void FlagCover::update(int deltatime) {

	sprite->update(deltatime, false, 1);
}