#ifndef _GOOMBA_INCLUDE
#define _GOOMBA_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "DeadBlock.h"

class Goomba {
public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap);
	virtual bool update(int deltatime);
	virtual int type();
	void render(int offset) const;
	void setPosition(const glm::vec2& pos);
	int checkCollision(const glm::vec2& pos, bool super, bool star);
	int getDeathTime();
	glm::vec2 getPos();
	bool getMoveRight();
	void toggleMoveRight();
	bool checkCollisionEnemy(const glm::vec2& posEnemy, bool sameEnemy);

protected:
	bool moveRight, startAnimation;
	glm::ivec2 tileMapDispl, pos;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	int stAnim, time, deathTime, move;
	DeadBlock* db;
};

#endif
