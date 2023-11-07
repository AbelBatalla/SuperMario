#ifndef _ITEMBLOCK_INCLUDE
#define _ITEMBLOCK_INCLUDE


#include "Sprite.h"
#include "PowerUp.h"

class ItemBlock
{
public:
	ItemBlock(int px, int py, int ptype);
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap);
	void update(int deltatime);
	void render(int offset) const;
	void setPosition(const glm::vec2& pos);
	bool isHit(const glm::vec2& playerPosition) const;
	PowerUp* getPowerUp();
private:
	int x, y, type;
	glm::ivec2 tileMapDispl, posItemBlock;
	Texture spritesheet;
	Sprite* sprite;
	PowerUp* powerUp;

};
#endif

