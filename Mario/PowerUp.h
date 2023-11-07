#ifndef _POWERUP_INCLUDE
#define _POWERUP_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "DeadBlock.h"

class PowerUp {
public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap);
	virtual void update(int deltatime);
	void render(int offset) const;
	void setPosition(const glm::vec2& pos);
	bool checkCollision(const glm::vec2& pos, bool super);

protected:
	bool moveRight, startAnimation;
	glm::ivec2 tileMapDispl, pos;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	int stAnim;
	DeadBlock* db;
};

#endif
