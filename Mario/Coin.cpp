#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Coin.h"
#include "Game.h"

Coin::Coin(int px, int py) {
	x = px;
	y = py;
}

void Coin::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
	spritesheet.loadFromFile("images/coins.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.5, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 4);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(0, glm::vec2(0.0f, 0.5f));
	sprite->addKeyframe(0, glm::vec2(0.5f, 0.f));
	sprite->changeAnimation(0);


	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + 20), float(tileMapDispl.y + posCoin.y)));
	collected = false;
}

void Coin::setPosition(const glm::vec2& pos)
{
	posCoin = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posCoin.x), float(tileMapDispl.y + posCoin.y)));
}


bool Coin::isCollected(const glm::vec2& playerPosition, int playerSize) const
{
	// Calcula el centro de la moneda
	glm::vec2 coinCenter = glm::vec2(tileMapDispl.x + posCoin.x, tileMapDispl.y + posCoin.y) + glm::vec2(8, 8);

	// Calcula el centro del jugador
	glm::vec2 playerCenter = glm::vec2(playerPosition.x  + tileMapDispl.x + playerSize / 2, playerPosition.y + tileMapDispl.y + playerSize / 2);

	// Verifica si el jugador est� exactamente en el mismo centro que la moneda

	return (playerCenter.y == coinCenter.y && playerCenter.x == coinCenter.x);
}



void Coin::render(int offset) const
{
	sprite->render(offset);
}

void Coin::update(int deltatime) {
	
	sprite->update(deltatime);
}