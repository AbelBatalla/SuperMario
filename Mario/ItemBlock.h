#ifndef _ItemBlock_INCLUDE
#define _ItemBlock_INCLUDE


#include "Sprite.h"


class ItemBlock
{
public:
	ItemBlock(int px, int py);
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltatime);
	void render(int offset) const;
	void setPosition(const glm::vec2& pos);
	bool isHit(const glm::vec2& playerPosition) const;
private:
	int x;
	int y;
	glm::ivec2 tileMapDispl, posItemBlock;
	Texture spritesheet;
	Sprite* sprite;
};
#endif

