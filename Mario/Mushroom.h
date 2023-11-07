#ifndef _MUSHROOM_INCLUDE
#define _MUSHROOM_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "DeadBlock.h"
#include "PowerUp.h"

class Mushroom : public PowerUp {
	public:
		void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap) override;
		void update(int deltatime) override;
		void render(int offset) const;
		void setPosition(const glm::vec2& pos);
		bool checkCollision(const glm::vec2& pos, bool super);
};

#endif