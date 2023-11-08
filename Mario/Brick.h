#ifndef _BRICK_INCLUDE
#define _BRICK_INCLUDE


#include "Sprite.h"
#include "PowerUp.h"
#include "CoinPU.h"
#include "Debris.h"

class Brick
{
public:
	Brick(int px, int py, int ptype);
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap);
	void update(int deltatime);
	void render(int offset) const;
	void setPosition(const glm::vec2& pos);
	bool isHit(const glm::vec2& playerPosition);
	PowerUp* getPowerUp();
	int getState();
	glm::ivec2 getPos();

private:
	int x, y, type, state, coinCounter;
	glm::ivec2 tileMapDispl, posBrick;
	Texture spritesheet;
	Sprite* sprite;
	PowerUp* powerUp;
	bool hitLast;

};
#endif

