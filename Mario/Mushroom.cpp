#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Mushroom.h"
#include "Game.h"

void Mushroom::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap) {
	spritesheet.loadFromFile("images/tilesheetv2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625, 0.0625), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 4);
	sprite->addKeyframe(0, glm::vec2(0.1875f, 0.0625f));
	sprite->changeAnimation(0, 0);

	moveRight = true;
	startAnimation = true;
	stAnim = 0;
	tileMapDispl = tileMapPos;
	map = tileMap;
	db = new DeadBlock();
	db->init(tileMapPos, shaderProgram);

}


void Mushroom::update(int deltatime) {
	
	if (startAnimation) {
		stAnim += 1;
		if (stAnim % 4 == 0) pos.y -= 1;
		if (stAnim >= 64) startAnimation = false;
	}
	else {
		pos.y += 2;
		map->collisionMoveDown(pos, glm::ivec2(16, 16), &pos.y);

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
}

void Mushroom::setPosition(const glm::vec2& position)
{
	pos = position;
	//pos.y -= 16;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
	if (startAnimation) db->setPosition(position);
}


void Mushroom::render(int offset) const
{
	sprite->render(offset);
	if (startAnimation) db->render(offset);
}
