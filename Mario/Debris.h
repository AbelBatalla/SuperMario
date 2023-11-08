#ifndef _DEBRIS_INCLUDE
#define _DEBRIS_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "PowerUp.h"

class Debris : public PowerUp {
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap) override;
	bool update(int deltatime) override;
	void setPosition(const glm::vec2& pos) override;
	void render(int offset) const override;
	int type() override;

private:
	int distX, distY, xCount, jumpAngle, startY;
};

#endif