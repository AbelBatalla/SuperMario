#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "PowerUp.h"
#include "Game.h"

void PowerUp::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap) {
}


void PowerUp::update(int deltatime) {
}

void PowerUp::setPosition(const glm::vec2& position)
{
	pos = position;
	//pos.y -= 16;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
	if (startAnimation) db->setPosition(position);
}


void PowerUp::render(int offset) const
{
	sprite->render(offset);
	if (startAnimation) db->render(offset);
}

bool PowerUp::checkCollision(const glm::vec2& posPlayer, bool super)
{
	return (abs(posPlayer.x - pos.x) <= 15 and ((!super and abs(posPlayer.y - pos.y) <= 15) or (super and pos.y >= posPlayer.y - 15 and pos.y <= posPlayer.y + 31)));
}