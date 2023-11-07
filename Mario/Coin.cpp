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
	sprite->setAnimationSpeed(0, 4);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(0, glm::vec2(0.0f, 0.5f));
	sprite->addKeyframe(0, glm::vec2(0.5f, 0.f));
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
	// Calcula el centro de la moneda
	glm::vec2 coinCenter = glm::vec2(tileMapDispl.x + posCoin.x, tileMapDispl.y + posCoin.y) + glm::vec2(8, 8);
	glm::vec2 playerCenter;
	// Calcula el centro del jugador
	if(super) playerCenter = glm::vec2(playerPosition.x  + tileMapDispl.x + playerSize / 2, playerPosition.y + tileMapDispl.y + 16 + playerSize / 2);
	else  playerCenter = glm::vec2(playerPosition.x + tileMapDispl.x + playerSize / 2, playerPosition.y + tileMapDispl.y + playerSize / 2);
	// Verifica si el jugador está exactamente en el mismo centro que la moneda

	int toleranceX = 8;
	return (playerCenter.y == coinCenter.y && abs(playerCenter.x - coinCenter.x) <= toleranceX);
}



void Coin::render(int offset) const
{
	sprite->render(offset);
}

void Coin::update(int deltatime) {
	
	sprite->update(deltatime, false);
}