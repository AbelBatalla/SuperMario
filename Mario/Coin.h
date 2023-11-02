#ifndef _COIN_INCLUDE
#define _COIN_INCLUDE


#include "Sprite.h"


class Coin
{
public:
	Coin(int px, int py);
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltatime);
	void render(int offset) const;
	void setPosition(const glm::vec2& pos);
	bool isCollected(const glm::vec2& playerPosition, int playerSize, bool super) const;
private:
	int x;
	int y;
	glm::ivec2 tileMapDispl, posCoin;
	bool collected;
	Texture spritesheet;
	Sprite* sprite;
};
#endif

