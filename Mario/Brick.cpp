#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Brick.h"
#include "Game.h"


Brick::Brick(int px, int py, int ptype) {
	x = px;
	y = py;
	type = ptype;
}

void Brick::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap) {
	spritesheet.loadFromFile("images/itembricktiles.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625, 0.0625), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);
	sprite->setAnimationSpeed(0, 6);
	sprite->addKeyframe(0, glm::vec2(0.75f, 0.3125f));
	sprite->setAnimationSpeed(1, 6);
	sprite->addKeyframe(1, glm::vec2(0.9375f, 0.3125f));
	sprite->changeAnimation(0, 0);

	bumpAnim = false;
	up = true;
	bumpMov = 0;
	state = type+2;
	coinCounter = 10;
	tileMapDispl = tileMapPos;
	if (type == 0) powerUp = new Debris();
	else if (type == 1) powerUp = new Star();
	else powerUp = new CoinPU();

	powerUp->init(tileMapDispl, shaderProgram, tileMap);

}

void Brick::setPosition(const glm::vec2& pos)
{
	posBrick = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBrick.x), float(tileMapDispl.y + posBrick.y)));
	powerUp->setPosition(pos);

}

bool Brick::isHit(const glm::vec2& playerPosition, bool super, bool superTrans, int block)
{
	if ((block != -1 and posBrick.x/16 != block) or state == 1) {
		return false;
	}

	int xm0, xm1, ym;

	xm0 = (playerPosition.x + 3) / 16;
	xm1 = (playerPosition.x + 12) / 16;
	ym = (playerPosition.y - 3) / 16;

	if (ym == y) {
		for (int xi = xm0; xi <= xm1; xi++) {
			if (xi == x) {
				if (type == 0) {
					if (!super) {
						bumpAnim = true;
					} 
					else state = 0;
				}
				else if (type == 1) {
					state = 1;
					sprite->changeAnimation(1, 0);
				}
				else if (type == 2) {
					if (superTrans) return false;
					--coinCounter;
					bumpAnim = true;
					if (coinCounter <= 0) { //it is possible to delete powerUp here
						state = 1;
						sprite->changeAnimation(1, 0);
					}
				}
				return true;
			}
		}
	}
	return false;
}

void Brick::render(int offset) const
{
	sprite->render(offset);
}

void Brick::update(int deltatime) {
	if (bumpAnim) {
		if (up) {
			bumpMov += 1;
			if (bumpMov >= 5) up = false;
		}
		else {
			bumpMov -= 1;
			if (bumpMov < -2) {
				bumpAnim = false;
				up = true;
				bumpMov = 0;
			}
		}
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBrick.x), float(tileMapDispl.y + posBrick.y - bumpMov)));
	}
	sprite->update(deltatime, false, 1);
}

PowerUp* Brick::getPowerUp() {
	return powerUp;
}

int Brick::getState() {
	return state;
}

glm::ivec2 Brick::getPos() {
	return posBrick;
}