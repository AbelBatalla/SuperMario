#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Coin.h"
#include "Game.h"

void Coin::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
	spritesheet.loadFromFile("images/coins.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.5, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 6);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(0, glm::vec2(0.0f, 0.5f));
	sprite->addKeyframe(0, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(0, glm::vec2(0.0f, 0.5f));
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));

	sprite->changeAnimation(0, 0);


	tileMapDispl = tileMapPos;
}

void Coin::setPosition(const glm::vec2& pos)
{
	posCoin = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCoin.x), float(tileMapDispl.y + posCoin.y)));
}


bool Coin::isCollected(const glm::vec2& playerPosition, int playerSize, bool super) const
{
	return (abs(playerPosition.x - posCoin.x) <= 14 and ((!super and abs(playerPosition.y - posCoin.y) <= 14) or (super and posCoin.y >= playerPosition.y - 14 and posCoin.y <= playerPosition.y + 30)));
}



void Coin::render(int offset) const
{
	sprite->render(offset);
}

void Coin::update(int deltatime) {
	
	sprite->update(deltatime, false, 1);
}