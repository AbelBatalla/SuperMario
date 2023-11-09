#ifndef _KOOPA_INCLUDE
#define _KOOPA_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Koopa {
public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap);
	virtual bool update(int deltatime);
	virtual int type();
	void render(int offset) const;
	void setPosition(const glm::vec2& pos);
	int checkCollision(const glm::vec2& pos, bool super);
	int getDeathTime();
	void setDeathTime(int t);
	void toggleMoveShell();
	void toggleShell();
	bool getShell();
	bool getMoveShell();
	bool getMoveRight();
	void toggleMoveRight();
	glm::vec2 getPos();

protected:
	bool moveRight, shell, moveShell;
	glm::ivec2 tileMapDispl, pos;
	Texture spritesheet;
	Sprite* sprite;
	Sprite* spriteT;
	TileMap* map;
	int stAnim, time, deathTime, move;
};

#endif
