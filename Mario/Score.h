#ifndef _SCORE_INCLUDE
#define _SCORE_INCLUDE


#include "Sprite.h"


class Score
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int s);
	bool update(int deltatime);
	void render(int offset) const;
	void setPosition(const glm::vec2& posScore);
private:
	glm::ivec2 tileMapDispl, pos;
	Texture spritesheet;
	Sprite* sprite;
	int initialY;
};
#endif

