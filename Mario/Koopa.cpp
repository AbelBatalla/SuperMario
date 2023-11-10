#include "Koopa.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

void Koopa::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap) {
	map = tileMap;
	tileMapDispl = tileMapPos;
	deathTime = 0;
	time = 0;
	move = 0;
	shell = false;
	moveShell = false;
	spritesheet.loadFromFile("images/tilesheetv2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 32), glm::vec2(0.0625, 0.125), &spritesheet, &shaderProgram);
	spriteT = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625, 0.0625), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 4);
	sprite->addKeyframe(0, glm::vec2(0.0f, 0.0625f));
	sprite->addKeyframe(0, glm::vec2(0.0625f, 0.0625f));
	sprite->addKeyframe(0, glm::vec2(0.0f, 0.0625f));
	sprite->addKeyframe(0, glm::vec2(0.0625f, 0.0625f));
	sprite->changeAnimation(0, 0);
	sprite->setMirrored(true);


	spriteT->setNumberAnimations(2);
	spriteT->setAnimationSpeed(0, 1);
	spriteT->addKeyframe(1, glm::vec2(0.125f, 0.0625f));
	spriteT->setAnimationSpeed(1, 1);
	spriteT->addKeyframe(0, glm::vec2(0.125f, 0.125f));
	spriteT->changeAnimation(0, 0);
}


bool Koopa::getMoveRight() {
	return moveRight;
}

void Koopa::toggleMoveRight() {
	moveRight = !moveRight;
	if(moveRight) sprite->setMirrored(false);
	else sprite->setMirrored(true);
}


bool Koopa::update(int deltatime) {
	time += deltatime;
	//if (timeout >= 16000) return false;
	if (deathTime == 0 or moveShell) {
		pos.y += 0;
		map->collisionMoveDown(pos, glm::ivec2(16, 16), &pos.y);
		float velocity = moveShell ? 4 : 1;
		++move;
		move = move % 3;
		if (move > 0) {
			if (moveRight) {
				glm::ivec2 posaux = pos;
				if (!shell) posaux.y += 1;
				else posaux.y -= 2;

				if (map->collisionMoveRight(posaux, glm::ivec2(16, 25))) {
					moveRight = false;
					if (!shell) {
						sprite->setMirrored(true);
					}
					
				}
				else {
					pos.x += velocity;
				}
			}
			else {
				glm::ivec2 posaux = pos;
				if (!shell) posaux.y += 1;
				else posaux.y -= 2;

				if (map->collisionMoveLeft(posaux, glm::ivec2(16, 25))) {
					moveRight = true;
					if (!shell) {
					 sprite->setMirrored(false);
					}
				}
				else {
					pos.x -= velocity;
				}
			}
		}
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
	spriteT->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y + 16)));
	sprite->update(deltatime, false, 1);
	spriteT->update(deltatime, false, 1);
	return true;
}

void Koopa::setDeathTime(int t, bool changeAnimation) {
	deathTime = t;
	if(changeAnimation) spriteT->changeAnimation(1, 0);
}

int Koopa::type() {
	return 4;
}

void Koopa::setPosition(const glm::vec2& position)
{
	pos = position;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
	spriteT->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y + 16)));
}


void Koopa::render(int offset) const
{
	if (!shell) sprite->render(offset);
	else spriteT->render(offset);
}

int Koopa::getDeathTime() {
	return deathTime;
}

void Koopa::toggleShell() {
	shell = !shell;
	
}

void Koopa::toggleMoveShell() {
	
	moveShell = !moveShell;
}

bool Koopa::getShell() {
	return shell;
}

bool Koopa::getMoveShell() {
	return moveShell;
}

glm::vec2 Koopa::getPos()
{
	return pos;
}

int Koopa::checkCollision(const glm::vec2& posPlayer, bool super, bool star, bool down)
{	

	if (super and !shell) {
		if (abs(posPlayer.x - pos.x) <= 14 and abs(posPlayer.y - pos.y + 16) <= 24 - 16 and abs(posPlayer.y - pos.y + 16) >= 14 - 16) {
			shell = true;
			deathTime = time;
			spriteT->changeAnimation(0, 0);
			return 2;
		}
	}
	else if(!shell) {
		if (abs(posPlayer.x - pos.x) <= 14 and abs(posPlayer.y - pos.y - 16) <= 23 and abs(posPlayer.y - pos.y - 16) >= 18 ) {
			shell = true;
			deathTime = time;
			spriteT->changeAnimation(0, 0);
			return 2;
		}
	}

	int state = (abs(posPlayer.x - pos.x) <= 14 and ((!super and abs(posPlayer.y - 16 - pos.y) <= 14) or (super and pos.y >= posPlayer.y - 16 - 14 and pos.y  <= posPlayer.y - 16 + 30)));
	if (state == 1 and star) {
		shell = true;
		deathTime = time;
		spriteT->changeAnimation(0, 0);
		return 2;
	}
	return state;
}

