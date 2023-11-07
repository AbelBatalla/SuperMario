#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "ItemBlock.h"
#include "Game.h"

ItemBlock::ItemBlock(int px, int py) {
	x = px;
	y = py;
}

void ItemBlock::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
	spritesheet.loadFromFile("images/itembricktiles.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625, 0.0625), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 4);
	sprite->addKeyframe(0, glm::vec2(0.0f, 0.f));
	sprite->addKeyframe(0, glm::vec2(0.0625f, 0.f));
	sprite->addKeyframe(0, glm::vec2(0.125f, 0.f));
	sprite->changeAnimation(0, 0);


	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + 20), float(tileMapDispl.y + posItemBlock.y)));
}

void ItemBlock::setPosition(const glm::vec2& pos)
{
	posItemBlock = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posItemBlock.x), float(tileMapDispl.y + posItemBlock.y)));
}


bool ItemBlock::isHit(const glm::vec2& playerPosition) const
{
	int xm0, xm1, ym;

	xm0 = (playerPosition.x + 3) / 16;
	xm1 = (playerPosition.x + 16 - 4) / 16;
	ym = (playerPosition.y - 3) / 16;

	if (ym == y) {
		for (int xi = xm0; xi <= xm1; xi++) {
			if (xi == x) return true;
		}
	}
	return false;
}



void ItemBlock::render(int offset) const
{
	sprite->render(offset);
}

void ItemBlock::update(int deltatime) {

	sprite->update(deltatime, false);
}