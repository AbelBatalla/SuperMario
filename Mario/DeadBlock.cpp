#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "DeadBlock.h"
#include "Game.h"

void DeadBlock::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
	spritesheet.loadFromFile("images/tilesheetv2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625, 0.0625), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 4);
	sprite->addKeyframe(0, glm::vec2(0.9375f, 0.4375f));
	sprite->changeAnimation(0, 0);


	tileMapDispl = tileMapPos;

}

void DeadBlock::setPosition(const glm::vec2& pos)
{
	posDeadBlock = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posDeadBlock.x), float(tileMapDispl.y + posDeadBlock.y)));

}

void DeadBlock::render(int offset) const
{
	sprite->render(offset);
}

void DeadBlock::update(int deltatime) {

	sprite->update(deltatime, false, 1);
}