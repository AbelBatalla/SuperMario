#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "PowerUp.h"
#include "Game.h"


void PowerUp::setPosition(const glm::vec2& pos)
{
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}


void PowerUp::render(int offset) const
{
	sprite->render(offset);
}
