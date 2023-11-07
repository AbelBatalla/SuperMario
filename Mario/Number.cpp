#include "Number.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

Number::Number(int px, int py) {
}

void Number::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
	spritesheet.loadFromFile("images/numbers.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(0.25, 0.3333), &spritesheet, &shaderProgram);

	sprite->setNumberAnimations(10);
	sprite->setAnimationSpeed(0, 1);
	sprite->addKeyframe(0, glm::vec2(0.0f, 0.0f));

	sprite->setAnimationSpeed(1, 1);
	sprite->addKeyframe(1, glm::vec2(0.25f, 0.0f));

	sprite->setAnimationSpeed(2, 1);
	sprite->addKeyframe(2, glm::vec2(0.5f, 0.0f));

	sprite->setAnimationSpeed(3, 1);
	sprite->addKeyframe(3, glm::vec2(0.75f, 0.0f));

	sprite->setAnimationSpeed(4, 1);
	sprite->addKeyframe(4, glm::vec2(0.0f, 0.3333f));

	sprite->setAnimationSpeed(5, 1);
	sprite->addKeyframe(5, glm::vec2(0.25f, 0.3333f));

	sprite->setAnimationSpeed(6, 1);
	sprite->addKeyframe(6, glm::vec2(0.5f, 0.3333f));

	sprite->setAnimationSpeed(7, 1);
	sprite->addKeyframe(7, glm::vec2(0.75f, 0.3333f));

	sprite->setAnimationSpeed(8, 1);
	sprite->addKeyframe(8, glm::vec2(0.0f, 0.6666f));

	sprite->setAnimationSpeed(9, 1);
	sprite->addKeyframe(9, glm::vec2(0.25f, 0.6666f));

	
	sprite->changeAnimation(0, 0);

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + 20), float(tileMapDispl.y + 40)));

}

void Number::setPosition(const glm::vec2& pos)
{
	const glm::vec2 posCoin = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCoin.x), float(tileMapDispl.y + posCoin.y)));
}

void Number::setNumber(int num) {
	sprite->changeAnimation(num, 0);
}





void Number::render() const
{
	sprite->render(0);
}

void Number::update(int deltatime) {

	sprite->update(deltatime, false, 4);
}