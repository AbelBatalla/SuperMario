#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 5
#define MIN_JUMP_HEIGHT 50
#define MAX_JUMP_HEIGHT 200 //maximum jump height depends on jump angle step and jump agreggate
#define JUMP_AGREGATE 5
#define FALL_STEP 6
#define ACCEL 1
#define MAX_WALK_SPEED 12  // :4
#define MAX_RUN_SPEED 24  // :4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, SPRINT_LEFT, SPRINT_RIGHT, JUMP_RIGHT, DRIFT_TO_RIGHT, FALL_RIGHT1, FALL_RIGHT2, FALL_RIGHT3
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	speedX = 0;
	jumpAcu = 0;
	jumpPress = false;
	spritesheet.loadFromFile("images/tilesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.0625, 0.0625), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(11);
	
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));
		
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
		
		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0625f, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.125f, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.1875f, 0.f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0625f, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.125f, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.1875f, 0.f));

		sprite->setAnimationSpeed(FALL_RIGHT1, 8);
		sprite->addKeyframe(FALL_RIGHT1, glm::vec2(0.0625f, 0.f));

		sprite->setAnimationSpeed(FALL_RIGHT2, 8);
		sprite->addKeyframe(FALL_RIGHT2, glm::vec2(0.125f, 0.f));

		sprite->setAnimationSpeed(FALL_RIGHT3, 8);
		sprite->addKeyframe(FALL_RIGHT3, glm::vec2(0.1875f, 0.f));

		sprite->setAnimationSpeed(SPRINT_RIGHT, 32);
		sprite->addKeyframe(SPRINT_RIGHT, glm::vec2(0.0625f, 0.f));
		sprite->addKeyframe(SPRINT_RIGHT, glm::vec2(0.125f, 0.f));
		sprite->addKeyframe(SPRINT_RIGHT, glm::vec2(0.1875f, 0.f));

		sprite->setAnimationSpeed(SPRINT_LEFT, 32);
		sprite->addKeyframe(SPRINT_LEFT, glm::vec2(0.0625f, 0.f));
		sprite->addKeyframe(SPRINT_LEFT, glm::vec2(0.125f, 0.f));
		sprite->addKeyframe(SPRINT_LEFT, glm::vec2(0.1875f, 0.f));

		sprite->setAnimationSpeed(JUMP_RIGHT, 8);
		sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.3125f, 0.f));

		sprite->setAnimationSpeed(DRIFT_TO_RIGHT, 8);
		sprite->addKeyframe(DRIFT_TO_RIGHT, glm::vec2(0.25f, 0.f));
		
	sprite->changeAnimation(0);
	sprite->setMirrored(false);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime, int camx)
{
	sprite->update(deltaTime);
	bool in_the_air = false;
	bool derrape = (speedX > 0 and Game::instance().getSpecialKey(GLUT_KEY_LEFT) or speedX < 0 and Game::instance().getSpecialKey(GLUT_KEY_RIGHT));

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
			in_the_air = true;
			posPlayer.y = int(startY - min(MIN_JUMP_HEIGHT + jumpAcu, MAX_JUMP_HEIGHT) * sin(3.14159f * jumpAngle / 180.f));
			if (jumpAngle > 90) {
				bJumping = false; //!map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
				Game::instance().setSpace(false);
			}
			else if (jumpPress){
				if (Game::instance().getKey(' '))
					jumpAcu += JUMP_AGREGATE;
				else jumpPress = false;
			}
			if (sprite->animation() != JUMP_RIGHT) {
				sprite->changeAnimation(JUMP_RIGHT);
			}
		}
	}
	else
	{
		posPlayer.y += FALL_STEP;
		if(map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y))
		{
			in_the_air = false;
			if(Game::instance().getKey(' '))
			{
				if (derrape) {
					Game::instance().setSpace(false);
				}
				else {
					jumpPress = true;
					jumpAcu = 0;
					bJumping = true;
					jumpAngle = 0;
					startY = posPlayer.y;
				}
			}
		}
		else {
			in_the_air = true;
			//FALLING
			if (sprite->animation() != JUMP_RIGHT and !(sprite->animation() == FALL_RIGHT3 or sprite->animation() == FALL_RIGHT2 or sprite->animation() == FALL_RIGHT1)) {
				int frame = sprite->getFrame();
				if (frame == 2) {
					if (sprite->animation() != FALL_RIGHT3) {
						sprite->changeAnimation(FALL_RIGHT3);
					}
				}
				else if (frame == 1) {
					if (sprite->animation() != FALL_RIGHT2) {
						sprite->changeAnimation(FALL_RIGHT2);
					}
				}
				else if (frame == 0) {
					if (sprite->animation() != FALL_RIGHT1) {
						sprite->changeAnimation(FALL_RIGHT1);
					}
				}
			}
		}
	}

	if (!in_the_air) {
		//CALCULATE SPEEDS & ESTABLISH ANIMATIONS
		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) { //if the speed is above walk speed the animaton should be running
			//ANIMATIONS
			sprite->setMirrored(true);
			if (derrape) {
				if (sprite->animation() != DRIFT_TO_RIGHT) {
					sprite->changeAnimation(DRIFT_TO_RIGHT);
				}
			}
			else if (speedX < -MAX_WALK_SPEED) {
				if (sprite->animation() != SPRINT_LEFT) {
					sprite->changeAnimation(SPRINT_LEFT);
				}
			}
			else {
				if (sprite->animation() != MOVE_LEFT) {
					sprite->changeAnimation(MOVE_LEFT);
				}
			}
			//SPEEDS
			if (Game::instance().getKey('x')) {
				if (speedX > -MAX_RUN_SPEED) speedX -= ACCEL;
			}
			else {
				if (speedX > -MAX_WALK_SPEED) speedX -= ACCEL;
				if (speedX < -MAX_WALK_SPEED) speedX += ACCEL;
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
			//ANIMATIONS
			sprite->setMirrored(false);
			if (derrape) {
				if (sprite->animation() != DRIFT_TO_RIGHT) {
					sprite->changeAnimation(DRIFT_TO_RIGHT);
				}
			}
			else if (speedX > MAX_WALK_SPEED) {
				if (sprite->animation() != SPRINT_RIGHT) {
					sprite->changeAnimation(SPRINT_RIGHT);
				}
			}
			else {
				if (sprite->animation() != MOVE_RIGHT) {
					sprite->changeAnimation(MOVE_RIGHT);
				}
			}
			//SPEEDS:
			if (Game::instance().getKey('x')) {
				if (speedX < MAX_RUN_SPEED) speedX += ACCEL;
			}
			else {
				if (speedX < MAX_WALK_SPEED) speedX += ACCEL;
				if (speedX > MAX_WALK_SPEED) speedX -= ACCEL;
			}
		}
		else {
			if (speedX == 0) {
				if (sprite->getMirrored()) {//(sprite->animation() == MOVE_LEFT or (sprite->animation() == JUMP_RIGHT and sprite->getMirrored())) {
					sprite->changeAnimation(STAND_LEFT);
					sprite->setMirrored(true);
				}
				else if (!sprite->getMirrored()) { //(sprite->animation() == MOVE_RIGHT or (sprite->animation() == JUMP_RIGHT and !sprite->getMirrored())) {
					sprite->changeAnimation(STAND_RIGHT);
					sprite->setMirrored(false);
				}
			}
			else if (speedX < 0) {
				speedX += ACCEL;
				if (sprite->animation() != MOVE_LEFT) { //and speedX < -MAX_WALK_SPEED) {
					sprite->changeAnimation(MOVE_LEFT);
					sprite->setMirrored(true);
				}
			}
			else {
				speedX -= ACCEL;
				if (sprite->animation() != MOVE_RIGHT) { //and speedX > MAX_WALK_SPEED) {
					sprite->changeAnimation(MOVE_RIGHT);
					sprite->setMirrored(false);
				}
			}
		}
	}

	//UPDATE POSITIONS
	if (!(speedX < 0 and (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)) or posPlayer.x-camx <= 0)) and !(speedX > 0 and map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))) {
		posPlayer.x += speedX / 4;
	}
	else speedX = 0;

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render(int offset)
{
	sprite->render(offset);
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

int Player::getPosX()
{
	return posPlayer.x;
}



