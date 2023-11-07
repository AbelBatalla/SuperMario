#ifndef _POWERUP_INCLUDE
#define _POWERUP_INCLUDE


#include "Sprite.h"
#include "Game.h"


class PowerUp
{
public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap) = 0;
	virtual void update(int deltatime) = 0;
	void render(int offset) const;
	void setPosition(const glm::vec2& pos);
protected:
	glm::ivec2 tileMapDispl, pos;
	Texture spritesheet;
	Sprite* sprite;
};
#endif

