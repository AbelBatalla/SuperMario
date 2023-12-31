#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 7
#define MIN_JUMP_HEIGHT 24
#define MAX_JUMP_HEIGHT 70 //maximum jump height depends on jump angle step and jump agreggate
#define JUMP_AGREGATE 2
#define FALL_STEP 3
#define MAX_WALK_SPEED 36  // :DIVISOR //Es pot incrementar inèrcia incrementant aquests parametres sense que passin un modul del divisor
#define MAX_RUN_SPEED 50  // :DIVISOR
#define DIVISOR 16
#define NO_BUTTON_DIVISOR 2 // Augment d'inercia quan no es clica cap key.
#define MARIO_SIZE 16

enum PlayerAnims
{
	STAND_RIGHT, MOVE_RIGHT, SPRINT_RIGHT, JUMP_RIGHT, DRIFT_TO_RIGHT, FALL_RIGHT1, FALL_RIGHT2, FALL_RIGHT3, TREPAR, CROUCH, TRANSITION, DETRANSITION
};

void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	lives = 3;
	timeLife = 0;
	super = false;
	superTransition = false;
	superDetransition = false;
	loseSuperTimer = 0;
	loseSuper = false;
	loseSuperCounter = 0;
	star = false;
	starColorSpeed = 2;
	starOffset = 0;
	starCounter = 0;
	starTime = 0;
	bJumping = false;
	speedX = 0;
	jumpAcu = 0;
	jumpPress = false;
	accel = 2;
	killed = false;
	killedWithSuper = false;
	killJump = false;
	oldY = 0;
	collectedCoins = 0;
	deathAnim = false;
	flagAnim = false;
	finished = false;
	reverseFlag = false;
	reverseTimer = 0;
	flagBottom = false;

	engine = SoundManager::instance().getSoundEngine();


	spritesheet.loadFromFile("images/marioSpritesheet3.png", TEXTURE_PIXEL_FORMAT_RGBA);

	//SUPER
	sprite = Sprite::createSprite(glm::ivec2(MARIO_SIZE, MARIO_SIZE * 2), glm::vec2(0.03125, 0.0625), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(12);

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0f, 0.46875f));
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0f, 0.28125f));
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0f, 0.375f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 12);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0625f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0625f, 0.46875f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0625f, 0.28125f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0625f, 0.375f));

	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.03125f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.03125f, 0.46875f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.03125f, 0.28125f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.03125f, 0.375f));

	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.09375f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.09375f, 0.46875f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.09375f, 0.28125f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.09375f, 0.375f));


	sprite->setAnimationSpeed(FALL_RIGHT1, 8);
	sprite->addKeyframe(FALL_RIGHT1, glm::vec2(0.0625f, 0.0f));
	sprite->addKeyframe(FALL_RIGHT1, glm::vec2(0.0625f, 0.46875f));
	sprite->addKeyframe(FALL_RIGHT1, glm::vec2(0.0625f, 0.28125f));
	sprite->addKeyframe(FALL_RIGHT1, glm::vec2(0.0625f, 0.375f));

	sprite->setAnimationSpeed(FALL_RIGHT2, 8);
	sprite->addKeyframe(FALL_RIGHT2, glm::vec2(0.03125f, 0.0f));
	sprite->addKeyframe(FALL_RIGHT2, glm::vec2(0.03125f, 0.46875f));
	sprite->addKeyframe(FALL_RIGHT2, glm::vec2(0.03125f, 0.28125f));
	sprite->addKeyframe(FALL_RIGHT2, glm::vec2(0.03125f, 0.375f));

	sprite->setAnimationSpeed(FALL_RIGHT3, 8);
	sprite->addKeyframe(FALL_RIGHT3, glm::vec2(0.09375f, 0.0f));
	sprite->addKeyframe(FALL_RIGHT3, glm::vec2(0.09375f, 0.46875f));
	sprite->addKeyframe(FALL_RIGHT3, glm::vec2(0.09375f, 0.28125f));
	sprite->addKeyframe(FALL_RIGHT3, glm::vec2(0.09375f, 0.375f));

	sprite->setAnimationSpeed(SPRINT_RIGHT, 32);
	sprite->addKeyframe(SPRINT_RIGHT, glm::vec2(0.0625f, 0.0f));
	sprite->addKeyframe(SPRINT_RIGHT, glm::vec2(0.0625f, 0.46875f));
	sprite->addKeyframe(SPRINT_RIGHT, glm::vec2(0.0625f, 0.28125f));
	sprite->addKeyframe(SPRINT_RIGHT, glm::vec2(0.0625f, 0.375f));

	sprite->addKeyframe(SPRINT_RIGHT, glm::vec2(0.03125f, 0.0f));
	sprite->addKeyframe(SPRINT_RIGHT, glm::vec2(0.03125f, 0.46875f));
	sprite->addKeyframe(SPRINT_RIGHT, glm::vec2(0.03125f, 0.28125f));
	sprite->addKeyframe(SPRINT_RIGHT, glm::vec2(0.03125f, 0.375f));

	sprite->addKeyframe(SPRINT_RIGHT, glm::vec2(0.09375f, 0.0f));
	sprite->addKeyframe(SPRINT_RIGHT, glm::vec2(0.09375f, 0.46875f));
	sprite->addKeyframe(SPRINT_RIGHT, glm::vec2(0.09375f, 0.28125f));
	sprite->addKeyframe(SPRINT_RIGHT, glm::vec2(0.09375f, 0.375f));

	sprite->setAnimationSpeed(JUMP_RIGHT, 8);
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.15625f, 0.0f));
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.15625f, 0.46875f));
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.15625f, 0.28125f));
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.15625f, 0.375f));

	sprite->setAnimationSpeed(DRIFT_TO_RIGHT, 8);
	sprite->addKeyframe(DRIFT_TO_RIGHT, glm::vec2(0.125f, 0.0f));
	sprite->addKeyframe(DRIFT_TO_RIGHT, glm::vec2(0.125f, 0.46875f));
	sprite->addKeyframe(DRIFT_TO_RIGHT, glm::vec2(0.125f, 0.28125f));
	sprite->addKeyframe(DRIFT_TO_RIGHT, glm::vec2(0.125f, 0.375f));

	sprite->setAnimationSpeed(CROUCH, 8);
	sprite->addKeyframe(CROUCH, glm::vec2(0.1875f, 0.0f));
	sprite->addKeyframe(CROUCH, glm::vec2(0.1875f, 0.46875f));
	sprite->addKeyframe(CROUCH, glm::vec2(0.1875f, 0.28125f));
	sprite->addKeyframe(CROUCH, glm::vec2(0.1875f, 0.375f));

	sprite->setAnimationSpeed(TRANSITION, 12);
	sprite->addKeyframe(TRANSITION, glm::vec2(0.65625f, 0.0f));
	sprite->addKeyframe(TRANSITION, glm::vec2(0.65625f, 0.46875f));
	sprite->addKeyframe(TRANSITION, glm::vec2(0.65625f, 0.28125f));
	sprite->addKeyframe(TRANSITION, glm::vec2(0.65625f, 0.375f));

	sprite->addKeyframe(TRANSITION, glm::vec2(0.46875f, 0.0f));
	sprite->addKeyframe(TRANSITION, glm::vec2(0.46875f, 0.46875f));
	sprite->addKeyframe(TRANSITION, glm::vec2(0.46875f, 0.28125f));
	sprite->addKeyframe(TRANSITION, glm::vec2(0.46875f, 0.375f));

	sprite->addKeyframe(TRANSITION, glm::vec2(0.65625f, 0.0f));
	sprite->addKeyframe(TRANSITION, glm::vec2(0.65625f, 0.46875f));
	sprite->addKeyframe(TRANSITION, glm::vec2(0.65625f, 0.28125f));
	sprite->addKeyframe(TRANSITION, glm::vec2(0.65625f, 0.375f));

	sprite->addKeyframe(TRANSITION, glm::vec2(0.46875f, 0.0f));
	sprite->addKeyframe(TRANSITION, glm::vec2(0.46875f, 0.46875f));
	sprite->addKeyframe(TRANSITION, glm::vec2(0.46875f, 0.28125f));
	sprite->addKeyframe(TRANSITION, glm::vec2(0.46875f, 0.375f));

	sprite->addKeyframe(TRANSITION, glm::vec2(0.65625f, 0.0f));
	sprite->addKeyframe(TRANSITION, glm::vec2(0.65625f, 0.46875f));
	sprite->addKeyframe(TRANSITION, glm::vec2(0.65625f, 0.28125f));
	sprite->addKeyframe(TRANSITION, glm::vec2(0.65625f, 0.375f));

	sprite->addKeyframe(TRANSITION, glm::vec2(0.46875f, 0.0f));
	sprite->addKeyframe(TRANSITION, glm::vec2(0.46875f, 0.46875f));
	sprite->addKeyframe(TRANSITION, glm::vec2(0.46875f, 0.28125f));
	sprite->addKeyframe(TRANSITION, glm::vec2(0.46875f, 0.375f));

	sprite->addKeyframe(TRANSITION, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(TRANSITION, glm::vec2(0.0f, 0.46875f));
	sprite->addKeyframe(TRANSITION, glm::vec2(0.0f, 0.28125f));
	sprite->addKeyframe(TRANSITION, glm::vec2(0.0f, 0.375f));

	sprite->addKeyframe(TRANSITION, glm::vec2(0.65625f, 0.0f));
	sprite->addKeyframe(TRANSITION, glm::vec2(0.65625f, 0.46875f));
	sprite->addKeyframe(TRANSITION, glm::vec2(0.65625f, 0.28125f));
	sprite->addKeyframe(TRANSITION, glm::vec2(0.65625f, 0.375f));

	sprite->addKeyframe(TRANSITION, glm::vec2(0.46875f, 0.0f));
	sprite->addKeyframe(TRANSITION, glm::vec2(0.46875f, 0.46875f));
	sprite->addKeyframe(TRANSITION, glm::vec2(0.46875f, 0.28125f));
	sprite->addKeyframe(TRANSITION, glm::vec2(0.46875f, 0.375f));

	sprite->addKeyframe(TRANSITION, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(TRANSITION, glm::vec2(0.0f, 0.46875f));
	sprite->addKeyframe(TRANSITION, glm::vec2(0.0f, 0.28125f));
	sprite->addKeyframe(TRANSITION, glm::vec2(0.0f, 0.375f));

	sprite->addKeyframe(TRANSITION, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(TRANSITION, glm::vec2(0.0f, 0.46875f));
	sprite->addKeyframe(TRANSITION, glm::vec2(0.0f, 0.28125f));
	sprite->addKeyframe(TRANSITION, glm::vec2(0.0f, 0.375f));


	sprite->setAnimationSpeed(DETRANSITION, 12);
	sprite->addKeyframe(DETRANSITION, glm::vec2(0.15625f, 0.0f)); //Jump
	sprite->addKeyframe(DETRANSITION, glm::vec2(0.15625f, 0.0f)); //Jump
	sprite->addKeyframe(DETRANSITION, glm::vec2(0.6875f, 0.0f)); //"swimming" tiny mario
	sprite->addKeyframe(DETRANSITION, glm::vec2(0.3125f, 0.0f)); //"swimming" big mario
	sprite->addKeyframe(DETRANSITION, glm::vec2(0.6875f, 0.0f)); //"swimming" tiny mario
	sprite->addKeyframe(DETRANSITION, glm::vec2(0.3125f, 0.0f)); //"swimming" big mario
	sprite->addKeyframe(DETRANSITION, glm::vec2(0.6875f, 0.0f)); //"swimming" tiny mario
	sprite->addKeyframe(DETRANSITION, glm::vec2(0.3125f, 0.0f)); //"swimming" big mario
	sprite->addKeyframe(DETRANSITION, glm::vec2(0.6875f, 0.0f)); //"swimming" tiny mario
	sprite->addKeyframe(DETRANSITION, glm::vec2(0.3125f, 0.0f)); //"swimming" big mario
	sprite->addKeyframe(DETRANSITION, glm::vec2(0.6875f, 0.0f)); //"swimming" tiny mario
	sprite->addKeyframe(DETRANSITION, glm::vec2(0.6875f, 0.0f)); //"swimming" tiny mario
	sprite->addKeyframe(DETRANSITION, glm::vec2(0.6875f, 0.0f)); //"swimming" tiny mario

	sprite->setAnimationSpeed(TREPAR, 6);
	sprite->addKeyframe(TREPAR, glm::vec2(0.25f, 0.0f));
	sprite->addKeyframe(TREPAR, glm::vec2(0.25f, 0.46875f));
	sprite->addKeyframe(TREPAR, glm::vec2(0.25f, 0.28125f));
	sprite->addKeyframe(TREPAR, glm::vec2(0.25f, 0.375f));

	sprite->addKeyframe(TREPAR, glm::vec2(0.21875f, 0.0f));
	sprite->addKeyframe(TREPAR, glm::vec2(0.21875f, 0.46875f));
	sprite->addKeyframe(TREPAR, glm::vec2(0.21875f, 0.28125f));
	sprite->addKeyframe(TREPAR, glm::vec2(0.21875f, 0.375f));

	sprite->changeAnimation(0, 0);
	sprite->setMirrored(false);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	//TINY
	spriteT = Sprite::createSprite(glm::ivec2(MARIO_SIZE, MARIO_SIZE), glm::vec2(0.03125, 0.03125), &spritesheet, &shaderProgram);
	spriteT->setNumberAnimations(10);

	spriteT->setAnimationSpeed(STAND_RIGHT, 8);
	spriteT->addKeyframe(STAND_RIGHT, glm::vec2(0.0f, 0.0625f));
	spriteT->addKeyframe(STAND_RIGHT, glm::vec2(0.0f, 0.53125f));
	spriteT->addKeyframe(STAND_RIGHT, glm::vec2(0.0f, 0.34375f));
	spriteT->addKeyframe(STAND_RIGHT, glm::vec2(0.0f, 0.4375f));

	spriteT->setAnimationSpeed(MOVE_RIGHT, 12);
	spriteT->addKeyframe(MOVE_RIGHT, glm::vec2(0.0625f, 0.0625f));
	spriteT->addKeyframe(MOVE_RIGHT, glm::vec2(0.0625f, 0.53125f));
	spriteT->addKeyframe(MOVE_RIGHT, glm::vec2(0.0625f, 0.34375f));
	spriteT->addKeyframe(MOVE_RIGHT, glm::vec2(0.0625f, 0.4375f));

	spriteT->addKeyframe(MOVE_RIGHT, glm::vec2(0.03125f, 0.0625f));
	spriteT->addKeyframe(MOVE_RIGHT, glm::vec2(0.03125f, 0.53125f));
	spriteT->addKeyframe(MOVE_RIGHT, glm::vec2(0.03125f, 0.34375f));
	spriteT->addKeyframe(MOVE_RIGHT, glm::vec2(0.03125f, 0.4375f));

	spriteT->addKeyframe(MOVE_RIGHT, glm::vec2(0.09375f, 0.0625f));
	spriteT->addKeyframe(MOVE_RIGHT, glm::vec2(0.09375f, 0.53125f));
	spriteT->addKeyframe(MOVE_RIGHT, glm::vec2(0.09375f, 0.34375f));
	spriteT->addKeyframe(MOVE_RIGHT, glm::vec2(0.09375f, 0.4375f));

	spriteT->setAnimationSpeed(FALL_RIGHT1, 8);
	spriteT->addKeyframe(FALL_RIGHT1, glm::vec2(0.0625f, 0.0625f));
	spriteT->addKeyframe(FALL_RIGHT1, glm::vec2(0.0625f, 0.53125f));
	spriteT->addKeyframe(FALL_RIGHT1, glm::vec2(0.0625f, 0.34375f));
	spriteT->addKeyframe(FALL_RIGHT1, glm::vec2(0.0625f, 0.4375f));

	spriteT->setAnimationSpeed(FALL_RIGHT2, 8);
	spriteT->addKeyframe(FALL_RIGHT2, glm::vec2(0.03125f, 0.0625f));
	spriteT->addKeyframe(FALL_RIGHT2, glm::vec2(0.03125f, 0.53125f));
	spriteT->addKeyframe(FALL_RIGHT2, glm::vec2(0.03125f, 0.34375f));
	spriteT->addKeyframe(FALL_RIGHT2, glm::vec2(0.03125f, 0.4375f));

	spriteT->setAnimationSpeed(FALL_RIGHT3, 8);
	spriteT->addKeyframe(FALL_RIGHT3, glm::vec2(0.09375f, 0.0625f));
	spriteT->addKeyframe(FALL_RIGHT3, glm::vec2(0.09375f, 0.53125f));
	spriteT->addKeyframe(FALL_RIGHT3, glm::vec2(0.09375f, 0.34375f));
	spriteT->addKeyframe(FALL_RIGHT3, glm::vec2(0.09375f, 0.4375f));

	spriteT->setAnimationSpeed(SPRINT_RIGHT, 32);
	spriteT->addKeyframe(SPRINT_RIGHT, glm::vec2(0.0625f, 0.0625f));
	spriteT->addKeyframe(SPRINT_RIGHT, glm::vec2(0.0625f, 0.53125f));
	spriteT->addKeyframe(SPRINT_RIGHT, glm::vec2(0.0625f, 0.34375f));
	spriteT->addKeyframe(SPRINT_RIGHT, glm::vec2(0.0625f, 0.4375f));

	spriteT->addKeyframe(SPRINT_RIGHT, glm::vec2(0.03125f, 0.0625f));
	spriteT->addKeyframe(SPRINT_RIGHT, glm::vec2(0.03125f, 0.53125f));
	spriteT->addKeyframe(SPRINT_RIGHT, glm::vec2(0.03125f, 0.34375f));
	spriteT->addKeyframe(SPRINT_RIGHT, glm::vec2(0.03125f, 0.4375f));

	spriteT->addKeyframe(SPRINT_RIGHT, glm::vec2(0.09375f, 0.0625f));
	spriteT->addKeyframe(SPRINT_RIGHT, glm::vec2(0.09375f, 0.53125f));
	spriteT->addKeyframe(SPRINT_RIGHT, glm::vec2(0.09375f, 0.34375f));
	spriteT->addKeyframe(SPRINT_RIGHT, glm::vec2(0.09375f, 0.4375f));

	spriteT->setAnimationSpeed(JUMP_RIGHT, 8);
	spriteT->addKeyframe(JUMP_RIGHT, glm::vec2(0.15625f, 0.0625f));
	spriteT->addKeyframe(JUMP_RIGHT, glm::vec2(0.15625f, 0.53125f));
	spriteT->addKeyframe(JUMP_RIGHT, glm::vec2(0.15625f, 0.34375f));
	spriteT->addKeyframe(JUMP_RIGHT, glm::vec2(0.15625f, 0.4375f));

	spriteT->setAnimationSpeed(DRIFT_TO_RIGHT, 8);
	spriteT->addKeyframe(DRIFT_TO_RIGHT, glm::vec2(0.125f, 0.0625f));
	spriteT->addKeyframe(DRIFT_TO_RIGHT, glm::vec2(0.125f, 0.53125f));
	spriteT->addKeyframe(DRIFT_TO_RIGHT, glm::vec2(0.125f, 0.34375f));
	spriteT->addKeyframe(DRIFT_TO_RIGHT, glm::vec2(0.125f, 0.4375f));

	spriteT->setAnimationSpeed(TREPAR, 6);
	spriteT->addKeyframe(TREPAR, glm::vec2(0.25f, 0.0625f));
	spriteT->addKeyframe(TREPAR, glm::vec2(0.25f, 0.53125f));
	spriteT->addKeyframe(TREPAR, glm::vec2(0.25f, 0.34375f));
	spriteT->addKeyframe(TREPAR, glm::vec2(0.25f, 0.4375f));

	spriteT->addKeyframe(TREPAR, glm::vec2(0.21875f, 0.0625f));
	spriteT->addKeyframe(TREPAR, glm::vec2(0.21875f, 0.53125f));
	spriteT->addKeyframe(TREPAR, glm::vec2(0.21875f, 0.34375f));
	spriteT->addKeyframe(TREPAR, glm::vec2(0.21875f, 0.4375f));

	spriteT->setAnimationSpeed(9, 8);
	spriteT->addKeyframe(9, glm::vec2(0.1875f, 0.0625f));

	spriteT->changeAnimation(0, 0);
	spriteT->setMirrored(false);
	tileMapDispl = tileMapPos;
	spriteT->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

bool Player::update(int deltaTime, int camx)
{
	if (finished) {
		timeLife += deltaTime * 100;
		if (timeLife < 0) timeLife = 0;
		return false;
	}
	bool updateStar = false;
	if (star) {
		starTime += deltaTime;
		if (starTime >= 12000) star = false;
		else {
			starCounter += 1;
			if (starTime >= 8500) starColorSpeed = 10;
			if (starCounter >= starColorSpeed) { //Indica velocitat del canvide color del star, ha de ser 2 o 3
				starCounter = 0;
				updateStar = true;
				starOffset = (starOffset + 1) % 4;
			}
		}
	}
	if (flagAnim) {
		if (posPlayer.y <= (super ? 239 : 255)) {
			if (super) {
				if (sprite->animation() != TREPAR) {
					sprite->changeAnimation(TREPAR, star ? starOffset : 0);
				}
				sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
				sprite->update(deltaTime, updateStar, 4);
			}
			else {
				if (spriteT->animation() != TREPAR) {
					spriteT->changeAnimation(TREPAR, star ? starOffset : 0);
				}
				spriteT->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
				spriteT->update(deltaTime, updateStar, 4);
			}
			posPlayer.y += 2;
			return false;
		}
		else if (!flagBottom) return false;
		else if (!reverseFlag) {
			posPlayer.x += 13;
			if (super) {
				sprite->setMirrored(true);
				//sprite->changeAnimation(TREPAR, star ? starOffset : 0);
				sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
				sprite->update(deltaTime, updateStar, 4);
			}
			else {
				spriteT->setMirrored(true);
				//spriteT->changeAnimation(TREPAR, star ? starOffset : 0);
				spriteT->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
				spriteT->update(deltaTime, updateStar, 4);
			}
			reverseFlag = true;
			reverseTimer = 0;
			return false;
		}
		else if (reverseTimer <= 300) {
			reverseTimer += deltaTime;
			return false;
		}
	}

	if (!flagAnim) timeLife += deltaTime;
	if ((killed and not killedWithSuper or timeLife >= 200000) and not deathAnim) {
		deathAnim = true;
		jumpAngle = 0;
		startY = posPlayer.y;
		sY = posPlayer.y;
		deathAnimTimer = 0;
		spriteT->changeAnimation(9, star ? starOffset : 0);
	}

	if (deathAnim) {
		if (sY >= (map->getMapHeight()) * 16) {
			deathAnim = false;
			jumpAngle = 0;
			startY = 0;

		}
		else {
			if (deathAnimTimer <= 500) deathAnimTimer += deltaTime;
			else if (jumpAngle <= 180) {
				sY = int(startY - 48 * sin(3.14159f * jumpAngle / 180.f));
				jumpAngle += 4;
			}
			else sY += 3;
			spriteT->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + sY)));
			spriteT->update(deltaTime, false, 1);
			return false;
		}
	}


	if (posPlayer.y >= (map->getMapHeight() - 2) * 16 or (timeLife >= 200000 or killed) and not deathAnim) { //200000 -> 200s which are represented in units of 0.5s, so the "timer" starts at 400
		if (not killedWithSuper) {
			lives -= 1;
			killed = false;
		}
		else {
			killedWithSuper = false;
			killed = false;
			return false;
		}
		if (lives == 0) { //death
			irrklang::ISound* sound = engine->play2D("sounds/death.wav", false, false, true);
			sound->setVolume(0.7f);
			Game::instance().init(Game::instance().getActualMap(), false, true, false);
		}
		else {
			speedX = 0;
			super = false;
			star = false;
			starOffset = 0;
			starCounter = 0;
			starTime = 0;
			timeLife = 0;
			return true;
		}
	}

	if (superTransition) {
		if (sprite->getFrame() >= 40) superTransition = false;
	}
	if (superDetransition) {
		if (sprite->getFrame() >= 11) {
			superDetransition = false;
			irrklang::ISound* sound = engine->play2D("sounds/shrink.wav", false, false, true);
			sound->setVolume(0.5f);
		}
	}
	if (loseSuper) {
		loseSuperTimer += deltaTime;
		if (loseSuperTimer >= 4000) {
			loseSuper = false;
			superDetransition = false;
			loseSuperCounter = 0;
			loseSuperTimer = 0;
		}
	}

	if (Game::instance().getKey('q') and !super and !flagAnim) {
		super = true;
		posPlayer.y -= MARIO_SIZE;
		oldY -= MARIO_SIZE;
		superTransition = true;
		superDetransition = false;
		loseSuper = false;
		sprite->changeAnimation(TRANSITION, star ? starOffset : 0);
	}
	if (Game::instance().getKey('w') and super and !flagAnim) {
		super = false;
		superTransition = false;
		superDetransition = true;
		loseSuper = true;
		loseSuperCounter = 0;
		loseSuperTimer = 0;
		sprite->changeAnimation(DETRANSITION, star ? starOffset : 0);
	}
	if (Game::instance().getKey('e') and !star) star = true;
	if ((Game::instance().getKey('r') and star) or starTime >= 12000) {
		if (super) sprite->removeStar(starOffset);
		else spriteT->removeStar(starOffset);
		star = false;
		starOffset = 0;
		starCounter = 0;
		starTime = 0;
		starColorSpeed = 2;
	}
	if (super or superDetransition) sprite->update(deltaTime, updateStar, (superDetransition ? 1 : 4));
	else spriteT->update(deltaTime, updateStar, 4);

	if (!superTransition and !superDetransition) {
		bool in_the_air = false;
		bool derrape = ((speedX > 0 and Game::instance().getSpecialKey(GLUT_KEY_LEFT) or speedX < 0 and Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) and !flagAnim);

		//JUMP
		if (bJumping)
		{

			jumpAngle += JUMP_ANGLE_STEP;
			if (jumpAngle >= 180)
			{
				bJumping = false;
				killJump = false;
				posPlayer.y = startY;
			}
			else
			{
				in_the_air = true;
				if (jumpAngle > 90) {
					bJumping = false;
					//killJump = false;

					Game::instance().setSpace(false);
				}
				else if (jumpPress) {
					if (Game::instance().getKey(' ')) {
						jumpAcu += JUMP_AGREGATE;
						jumpAngle -= 4;
					}
					else jumpPress = false;
				}
				if (!map->collisionMoveUp(posPlayer, glm::ivec2(MARIO_SIZE, MARIO_SIZE * (super ? 2 : 1)))) {
					oldY = posPlayer.y;
					posPlayer.y = int(startY - (killJump ? 20 : min(MIN_JUMP_HEIGHT + jumpAcu, MAX_JUMP_HEIGHT)) * sin(3.14159f * jumpAngle / 180.f));
				}
				else {
					bJumping = false;
					//killJump = false;

					Game::instance().setSpace(false);
				}
				if (super) {
					if (sprite->animation() != JUMP_RIGHT and sprite->animation() != CROUCH) {
						irrklang::ISound* sound = engine->play2D("sounds/jump.wav", false, false, true);
						sound->setVolume(0.5f);
						sprite->changeAnimation(JUMP_RIGHT, star ? starOffset : 0);
					}
				}
				else {
					if (spriteT->animation() != JUMP_RIGHT) {
						irrklang::ISound* sound = engine->play2D("sounds/jump.wav", false, false, true);
						sound->setVolume(0.5f);
						spriteT->changeAnimation(JUMP_RIGHT, star ? starOffset : 0);
					}
				}
			}
		}
		else
		{
			oldY = posPlayer.y;
			posPlayer.y += FALL_STEP;
			if (map->collisionMoveDown(posPlayer, glm::ivec2(MARIO_SIZE, MARIO_SIZE * (super ? 2 : 1)), &posPlayer.y)) {
				killJump = false;
				in_the_air = false;
				if (Game::instance().getKey(' ') and !flagAnim)
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
				if (super and (sprite->animation() != JUMP_RIGHT and !(sprite->animation() == FALL_RIGHT3 or sprite->animation() == FALL_RIGHT2 or sprite->animation() == FALL_RIGHT1))
					or !super and (spriteT->animation() != JUMP_RIGHT and !(spriteT->animation() == FALL_RIGHT3 or spriteT->animation() == FALL_RIGHT2 or spriteT->animation() == FALL_RIGHT1))) {
					int frame = super ? sprite->getFrame() / 4 : spriteT->getFrame() / 4;
					if (frame == 2) {
						if (super) {
							if (sprite->animation() != FALL_RIGHT3) {
								sprite->changeAnimation(FALL_RIGHT3, star ? starOffset : 0);
							}
						}
						else {
							if (spriteT->animation() != FALL_RIGHT3) {
								spriteT->changeAnimation(FALL_RIGHT3, star ? starOffset : 0);
							}
						}
					}
					else if (frame == 1 or (super and sprite->animation() == CROUCH)) {
						if (super) {
							if (sprite->animation() != FALL_RIGHT2) {
								sprite->changeAnimation(FALL_RIGHT2, star ? starOffset : 0);
							}
						}
						else {
							if (spriteT->animation() != FALL_RIGHT2) {
								spriteT->changeAnimation(FALL_RIGHT2, star ? starOffset : 0);
							}
						}
					}
					else if (frame == 0) {
						if (super) {
							if (sprite->animation() != FALL_RIGHT1) {
								sprite->changeAnimation(FALL_RIGHT1, star ? starOffset : 0);
							}
						}
						else {
							if (spriteT->animation() != FALL_RIGHT1) {
								spriteT->changeAnimation(FALL_RIGHT1, star ? starOffset : 0);
							}
						}
					}
				}
			}
		}

		//CALCULATE SPEEDS & ESTABLISH ANIMATIONS
		if (((Game::instance().getSpecialKey(GLUT_KEY_LEFT) xor Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) and (in_the_air or !Game::instance().getSpecialKey(GLUT_KEY_DOWN))) or flagAnim) {
			if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) and !flagAnim) {
				if (!in_the_air) {
					sprite->setMirrored(true);
					spriteT->setMirrored(true);
				}
				if (Game::instance().getSpecialKey(112)) {
					if (speedX > -MAX_RUN_SPEED) speedX -= accel;
				}
				else {
					if (speedX > -MAX_WALK_SPEED) speedX -= accel;
					if (speedX < -MAX_WALK_SPEED) speedX += accel;
				}
			}
			else {
				if (!in_the_air) {
					sprite->setMirrored(false);
					spriteT->setMirrored(false);
				}
				if (Game::instance().getSpecialKey(112) and !flagAnim) {
					if (speedX < MAX_RUN_SPEED) speedX += accel;
				}
				else {
					if (speedX < MAX_WALK_SPEED) speedX += accel;
					if (speedX > MAX_WALK_SPEED) speedX -= accel;
				}
			}

			if (!in_the_air) {
				if (derrape) {
					if (super) {
						if (sprite->animation() != DRIFT_TO_RIGHT) {
							sprite->changeAnimation(DRIFT_TO_RIGHT, star ? starOffset : 0);
						}
					}
					else {
						if (spriteT->animation() != DRIFT_TO_RIGHT) {
							spriteT->changeAnimation(DRIFT_TO_RIGHT, star ? starOffset : 0);
						}
					}
				}
				else if (speedX < -MAX_WALK_SPEED or speedX > MAX_WALK_SPEED) {
					if (super) {
						if (sprite->animation() != SPRINT_RIGHT) {
							sprite->changeAnimation(SPRINT_RIGHT, star ? starOffset : 0);
						}
					}
					else {
						if (spriteT->animation() != SPRINT_RIGHT) {
							spriteT->changeAnimation(SPRINT_RIGHT, star ? starOffset : 0);
						}
					}

				}
				else {
					if (super) {
						if (sprite->animation() != MOVE_RIGHT) {
							sprite->changeAnimation(MOVE_RIGHT, star ? starOffset : 0);
						}
					}
					else {
						if (spriteT->animation() != MOVE_RIGHT) {
							spriteT->changeAnimation(MOVE_RIGHT, star ? starOffset : 0);
						}
					}
				}
			}
		}
		else {
			if (speedX / DIVISOR == 0) {
				if (!in_the_air) {
					if (super) {
						if (Game::instance().getSpecialKey(GLUT_KEY_DOWN) and !(Game::instance().getSpecialKey(GLUT_KEY_LEFT) or Game::instance().getSpecialKey(GLUT_KEY_RIGHT))) {
							if (sprite->animation() != CROUCH) {
								sprite->changeAnimation(CROUCH, star ? starOffset : 0);
							}
						}
						else {
							if (sprite->animation() != STAND_RIGHT) {
								sprite->changeAnimation(STAND_RIGHT, star ? starOffset : 0);
							}
						}
					}
					else {
						if (spriteT->animation() != STAND_RIGHT) {
							spriteT->changeAnimation(STAND_RIGHT, star ? starOffset : 0);
						}
					}
					speedX = 0;
				}
			}
			else if (speedX < 0) {
				if (!in_the_air) {
					speedX += accel / NO_BUTTON_DIVISOR;
					if (super) {
						if (Game::instance().getSpecialKey(GLUT_KEY_DOWN) and (!(Game::instance().getSpecialKey(GLUT_KEY_LEFT) or Game::instance().getSpecialKey(GLUT_KEY_RIGHT)))) {
							if (sprite->animation() != CROUCH) {
								sprite->changeAnimation(CROUCH, star ? starOffset : 0);
							}
						}
						else {
							if (sprite->animation() != MOVE_RIGHT) {
								sprite->changeAnimation(MOVE_RIGHT, star ? starOffset : 0);
							}
						}
					}
					else {
						if (spriteT->animation() != MOVE_RIGHT) {
							spriteT->changeAnimation(MOVE_RIGHT, star ? starOffset : 0);
						}
					}
					sprite->setMirrored(true);
					spriteT->setMirrored(true);
				}
			}
			else {
				if (!in_the_air) {
					speedX -= accel / NO_BUTTON_DIVISOR;
					if (super) {
						if (Game::instance().getSpecialKey(GLUT_KEY_DOWN) and (!(Game::instance().getSpecialKey(GLUT_KEY_LEFT) or Game::instance().getSpecialKey(GLUT_KEY_RIGHT)))) {
							if (sprite->animation() != CROUCH) {
								sprite->changeAnimation(CROUCH, star ? starOffset : 0);
							}
						}
						else {
							if (sprite->animation() != MOVE_RIGHT) {
								sprite->changeAnimation(MOVE_RIGHT, star ? starOffset : 0);
							}
						}
					}
					else {
						if (spriteT->animation() != MOVE_RIGHT) {
							spriteT->changeAnimation(MOVE_RIGHT, star ? starOffset : 0);
						}
					}
					sprite->setMirrored(false);
					spriteT->setMirrored(false);
				}
			}
		}

		posPlayer.x += speedX / DIVISOR;
		//UPDATE POSITIONS
		if (speedX < 0) {
			if (posPlayer.x - camx <= 0) {
				posPlayer.x = camx;
				speedX = 0;
			}
			if (map->collisionMoveLeftPlayer(posPlayer, glm::ivec2(MARIO_SIZE, MARIO_SIZE * (super ? 2 : 1)), &posPlayer.x)) speedX = 0;
		}
		else if (speedX > 0) {
			if (map->collisionMoveRightPlayer(posPlayer, glm::ivec2(MARIO_SIZE, MARIO_SIZE * (super ? 2 : 1)), &posPlayer.x)) speedX = 0;
		}
	}
	if (super) {
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	}
	else {
		spriteT->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	}
	return false;
}

int Player::getLives() {
	return lives;
}

void Player::kill() {
	if (super) {
		super = false;
		superTransition = false;
		superDetransition = true;
		loseSuper = true;
		sprite->changeAnimation(DETRANSITION, star ? starOffset : 0);

		killedWithSuper = true;
	}
	killed = true;
}

bool Player::isKilled() {
	return killed;
}

int Player::getTimeLife() {
	return timeLife;
}

void Player::render(int offset)
{
	if (super) {
		sprite->render(offset);
	}
	else {
		if (loseSuper) {
			++loseSuperCounter;
			loseSuperCounter = loseSuperCounter % 3;
			if (loseSuperCounter != 0) {
				if (superDetransition) sprite->render(offset);
				else spriteT->render(offset);
			}
		}
		else spriteT->render(offset);
	}
}

bool Player::getMarioState() {
	return super;
}


bool Player::getMarioStar() {
	return star;

}

bool Player::getMarioInvincible() {
	return loseSuper;
}

bool Player::getMarioTransitionState() {
	return superTransition;
}

void Player::setKillJump() {
	killJump = true;
	bJumping = true;
	jumpPress = false;
	jumpAcu = 0;
	bJumping = true;
	jumpAngle = 0;
	startY = posPlayer.y;
}


void Player::setMarioState(bool state) {
	super = state;
}

void Player::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2& pos)
{
	posPlayer = pos;
	oldY = pos.y;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::vec2 Player::getPos()
{
	return posPlayer;
}

int Player::getPosX()
{
	return posPlayer.x;
}

void Player::turnSuper()
{
	if (!super) {
		super = true;
		posPlayer.y -= MARIO_SIZE;
		oldY -= MARIO_SIZE;
		superTransition = true;
		sprite->changeAnimation(TRANSITION, star ? starOffset : 0);
	}
}

void Player::turnStar()
{
	star = true;
	starTime = 0;
}


void Player::collisionUp()
{
	bJumping = false;
	Game::instance().setSpace(false);
}

bool Player::goingDown()
{
	return oldY < posPlayer.y; // or jumpAngle >= 90?
}

bool Player::getKillJump()
{
	return killJump;
}

bool Player::inTransition() {
	return superTransition or superDetransition or deathAnim;
}

void Player::setFlagAnim() {
	flagAnim = true;
	bJumping = false;
	speedX = 0;
	if (posPlayer.y > (super ? 239 : 255)) posPlayer.y = (super ? 239 : 255);
}

bool Player::getFlagAnim() {
	return flagAnim;
}

void Player::finish() {
	finished = true;
}

void Player::setFlagBottom() {
	flagBottom = true;
}