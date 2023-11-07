#ifndef _COINPU_INCLUDE
#define _COINPU_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "PowerUp.h"

class CoinPU : public PowerUp {
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap) override;
	bool update(int deltatime) override;
	void setPosition(const glm::vec2& pos) override;
	int type() override;

private:
	bool up;
	int acum;
};

#endif