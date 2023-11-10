#ifndef _FLAGCOVER_INCLUDE
#define _FLAGCOVER_INCLUDE

#include "Sprite.h"

class FlagCover
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltatime);
	void render(int offset) const;
	void setPosition(const glm::vec2& pos);
private:
	glm::ivec2 tileMapDispl, posCover;
	Texture spritesheet;
	Sprite* sprite;
};
#endif

