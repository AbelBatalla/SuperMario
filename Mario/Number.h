#ifndef _NUMBER_INCLUDE
#define _NUMBER_INCLUDE


#include "Sprite.h"


class Number
{
public:
	Number(int px, int py);
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltatime);
	void render() const;
	void setPosition(const glm::vec2& pos);
	void setNumber(int num);
private:
	glm::ivec2 tileMapDispl;
	Texture spritesheet;
	std::vector<Sprite*> numbers;
	Sprite* sprite;
};
#endif

