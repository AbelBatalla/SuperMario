#ifndef _FLAG_INCLUDE
#define _FLAG_INCLUDE


#include "Sprite.h"


class Flag
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int ftype);
	void update(int deltatime);
	void render(int offset) const;
	void setPosition(const glm::vec2& pos);
	void activate();
	bool getActivate();

private:
	glm::ivec2 tileMapDispl, posFlag;
	Texture spritesheet;
	Sprite* sprite;
	int type;
	bool activated;
};
#endif

