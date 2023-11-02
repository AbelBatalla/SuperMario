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
	
private:
	bool bJumping, jumpPress, super, star, superTransition;
	glm::ivec2 tileMapDispl, posPlayer;
	int speedX, accel, jumpAngle, startY, jumpAcu, starOffset, starCounter, starTime, lives, timeLife, starColorSpeed, superTransTimer;
	Texture spritesheet;
	Sprite *sprite, *spriteT;
	TileMap *map;
	int collectedCoins;
};


#endif // _PLAYER_INCLUDE


