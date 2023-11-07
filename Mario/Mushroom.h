#ifndef _MUSHROOM_INCLUDE
#define _MUSHROOM_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "DeadBlock.h"
#include "PowerUp.h"

class Mushroom : public PowerUp {
	public:
		void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap) override;
		bool update(int deltatime) override;
		int type() override;
};

#endif