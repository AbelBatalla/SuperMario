#ifndef _MUSHROOM_INCLUDE
#define _MUSHROOM_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "DeadBlock.h"

class Mushroom {
	public:
		void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap);
		void update(int deltatime);
		void render(int offset) const;
		void setPosition(const glm::vec2& pos);

	private:
		bool moveRight, startAnimation;
		glm::ivec2 tileMapDispl, pos;
		Texture spritesheet;
		Sprite* sprite;
		TileMap* map;
		int stAnim;
		DeadBlock* db;
};

#endif