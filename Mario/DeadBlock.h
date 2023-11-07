#ifndef _DEADBLOCK_INCLUDE
#define _DEADBLOCK_INCLUDE

#include "Sprite.h"

class DeadBlock
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltatime);
	void render(int offset) const;
	void setPosition(const glm::vec2& pos);
private:
	glm::ivec2 tileMapDispl, posDeadBlock;
	Texture spritesheet;
	Sprite* sprite;

};
#endif

