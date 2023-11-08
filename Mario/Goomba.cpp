#include "Goomba.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

void Goomba::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap) {
	map = tileMap;
	tileMapDispl = tileMapPos;
	deathTime = 0;
	time = 0;
	spritesheet.loadFromFile("images/tilesheetv2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625, 0.0625), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);
	sprite->setAnimationSpeed(0, 4);
	sprite->addKeyframe(0, glm::vec2(0.4375f, 0.0f));
	sprite->addKeyframe(0, glm::vec2(0.5, 0.0f));
	sprite->setAnimationSpeed(1, 6);
	sprite->addKeyframe(1, glm::vec2(0.5625f, 0.0f));
	sprite->changeAnimation(0, 0);
	//sprite->setPosition(tileMapPos);
}


bool Goomba::update(int deltatime) {
	time += deltatime;
	//if (timeout >= 16000) return false;
	pos.y += 1;
	map->collisionMoveDown(pos, glm::ivec2(16, 16), &pos.y);

	if (moveRight) {
		if (map->collisionMoveRight(pos, glm::ivec2(16, 16))) {
			moveRight = false;
		}
		else {
			pos.x += 1.0f;
		}
	}
	else {
		if (map->collisionMoveLeft(pos, glm::ivec2(16, 16))) {
			moveRight = true;
		}
		else {
			pos.x -= 1.0f;
		}
	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
	sprite->update(deltatime, false, 1);
	return true;
}

int Goomba::type() {
	return 4;
}

void Goomba::setPosition(const glm::vec2& position)
{
	pos = position;
	//pos.y -= 16;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
	//if (startAnimation) db->setPosition(position);
}


void Goomba::render(int offset) const
{
	sprite->render(offset);
	//if (startAnimation) db->render(offset);
}

int Goomba::getDeathTime() {
	return deathTime;
}

int Goomba::checkCollision(const glm::vec2& posPlayer, bool super)
{
	int playerHeight = super ? 16 : 0;
	if (abs(posPlayer.x - pos.x) <= 14 and abs(posPlayer.y - pos.y) <= 25 + playerHeight and abs(posPlayer.y - pos.y) >= 14 + playerHeight) {
		sprite->changeAnimation(1, 0);
		deathTime = time;
		return 2;
	}
	return (abs(posPlayer.x - pos.x) <= 14 and ((!super and abs(posPlayer.y - pos.y) <= 14) or (super and pos.y >= posPlayer.y - 14 and pos.y <= posPlayer.y + 30)));
}