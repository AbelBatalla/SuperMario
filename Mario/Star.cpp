#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Star.h"
#include "Game.h"

void Star::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap) {
	spritesheet.loadFromFile("images/tilesheetv3.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625, 0.0625), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 16);
	sprite->addKeyframe(0, glm::vec2(0.5f, 0.1875f));
	sprite->addKeyframe(0, glm::vec2(0.9375f, 0.5f));
	sprite->changeAnimation(0, 0);

	moveRight = true;
	startAnimation = true;
	stAnim = 0;
	timeout = 0;
	jumpAngle = 0;
	startY = 0;
	bJumping = false;
	tileMapDispl = tileMapPos;
	map = tileMap;
	db = new DeadBlock();
	db->init(tileMapPos, shaderProgram);

}


bool Star::update(int deltatime) {

	if (startAnimation) {
		stAnim += 1;
		if (stAnim % 4 == 0) pos.y -= 1;
		if (stAnim >= 64) startAnimation = false;
	}
	else {
		timeout += deltatime;
		if (timeout >= 12000) return false;

		if (bJumping) {
			jumpAngle += 4;
			if (jumpAngle >= 180)
			{
				bJumping = false;
				pos.y = startY;
			}
			else {
				if (jumpAngle > 90) {
					bJumping = false;
				}
				if (!map->collisionMoveUp(pos, glm::ivec2(16, 16), &pos.y)) {
					pos.y = int(startY - 48 * sin(3.14159f * jumpAngle / 180.f));
				}
				else {
					bJumping = false;
				}
			}
		}
		else {
			pos.y += 2;
			if (map->collisionMoveDown(pos, glm::ivec2(16, 16), &pos.y)) {
				bJumping = true;
				jumpAngle = 0;
				startY = pos.y;

			}
		}

		if (moveRight) {
			if (map->collisionMoveRight(pos, glm::ivec2(16, 16))) {
				moveRight = false;
			}
			else {
				pos.x += 1;
			}
		}
		else {
			if (map->collisionMoveLeft(pos, glm::ivec2(16, 16))) {
				moveRight = true;
			}
			else {
				pos.x -= 1;
			}
		}
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
	sprite->update(deltatime, false, 1);
	if (startAnimation) db->update(deltatime);
	return true;
}

int Star::type() {
	return 1;
}