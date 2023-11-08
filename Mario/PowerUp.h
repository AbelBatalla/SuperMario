#ifndef _POWERUP_INCLUDE
#define _POWERUP_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "DeadBlock.h"

class PowerUp {
public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap);
	virtual bool update(int deltatime);
	virtual int type();
	virtual void render(int offset) const;
	virtual void setPosition(const glm::vec2& pos);
	bool checkCollision(const glm::vec2& pos, bool super);
	glm::ivec2 getPos();

protected:
	bool moveRight, startAnimation;
	glm::ivec2 tileMapDispl, pos;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	int stAnim, timeout;
	DeadBlock* db;
};

#endif
