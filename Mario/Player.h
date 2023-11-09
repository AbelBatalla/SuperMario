#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	bool update(int deltaTime, int camx);
	void render(int offset);
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	glm::vec2 getPos();
	int getPosX();
	bool getMarioState();
	bool getMarioStar();
	bool getMarioInvincible();
	void setMarioState(bool state);
	int getLives();
	int getTimeLife();
	void turnSuper();
	void turnStar();
	void kill();
	bool isKilled();
	void collisionUp();
	bool getMarioTransitionState();
	void setKillJump();
	bool goingDown();
	bool getKillJump();
	bool inTransition();
	void setFlagAnim();
	bool getFlagAnim();
	
private:
	bool bJumping, jumpPress, super, star, superTransition, superDetransition, killed, loseSuper, killedWithSuper, killJump, deathAnim, flagAnim;
	glm::ivec2 tileMapDispl, posPlayer;
	int speedX, accel, jumpAngle, startY, jumpAcu, starOffset, starCounter, starTime, lives, timeLife, starColorSpeed, loseSuperCounter, loseSuperTimer;
	Texture spritesheet;
	Sprite *sprite, *spriteT;
	TileMap *map;
	int collectedCoins, oldY, sY, deathAnimTimer;
};


#endif // _PLAYER_INCLUDE


