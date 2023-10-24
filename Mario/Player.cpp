#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 6
#define MIN_JUMP_HEIGHT 50
#define MAX_JUMP_HEIGHT 150
#define JUMP_AGREGATE 4
#define FALL_STEP 6
#define ACCEL 1
#define MAX_WALK_SPEED 12  // :4
#define MAX_RUN_SPEED 24  // :4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	speedX = 0;
	jumpAcu = 0;
	jumpPress = false;
	spritesheet.loadFromFile("images/bub.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);
	
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));
		
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.25f, 0.f));
		
		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.25f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.5f));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	//TOCAR SPEEDS i ANIMATIONS
	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT)) { //if the speed is above walk speed the animaton should be running
		if (sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		if (Game::instance().getKey('x')) { //(glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
			if (speedX > -MAX_RUN_SPEED) speedX -= ACCEL;
		}
		else {
			if (speedX > -MAX_WALK_SPEED) speedX -= ACCEL;
			if (speedX < -MAX_WALK_SPEED) speedX += ACCEL;
		}
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
		if (sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		if (Game::instance().getKey('x')) { //(glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
			if (speedX < MAX_RUN_SPEED) speedX += ACCEL;
		}
		else {
			if (speedX < MAX_WALK_SPEED) speedX += ACCEL;
			if (speedX > MAX_WALK_SPEED) speedX -= ACCEL;
		}	
	}
	else {
		if (speedX == 0) {
			if (sprite->animation() == MOVE_LEFT)
				sprite->changeAnimation(STAND_LEFT);
			else if (sprite->animation() == MOVE_RIGHT)
				sprite->changeAnimation(STAND_RIGHT);
		}
		else {
			speedX > 0 ? speedX -= ACCEL : speedX += ACCEL;
		}
	}
	
	//Actualitzar pos
	if (!(speedX < 0 and map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32))) and !(speedX > 0 and map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))) {
		posPlayer.x += speedX / 4;
	}
	else speedX = 0;

	//JUMP
	if(bJumping)
	{
		jumpAngle += JUMP_ANGLE_STEP;
		if(jumpAngle == 180)
		{
			bJumping = false;
			posPlayer.y = startY;
		}
		else
		{
			posPlayer.y = int(startY - min(MIN_JUMP_HEIGHT + jumpAcu, MAX_JUMP_HEIGHT) * sin(3.14159f * jumpAngle / 180.f));
			if (jumpAngle > 90)
				bJumping = false; //!map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
			else if (jumpPress){
				if (Game::instance().getKey(' '))
					jumpAcu += JUMP_AGREGATE;
				else jumpPress = false;
			}
		}
	}
	else
	{
		posPlayer.y += FALL_STEP;
		if(map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y))
		{
			if(Game::instance().getKey(' '))
			{
				jumpPress = true;
				jumpAcu = 0;
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}
	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}




