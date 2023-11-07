#ifndef _STAR_INCLUDE
#define _STAR_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "DeadBlock.h"
#include "PowerUp.h"

class Star : public PowerUp {
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap);
	void update(int deltatime);
};

#endif