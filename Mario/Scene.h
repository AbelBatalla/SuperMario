#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Number.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();
	void initCoinCounter();
	void setCoinCounter(int num);
	std::vector<int> transformNumber(int num);

private:
	TileMap *map;
	Player *player;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	int camx, oldPosx, numCoins;
	std::vector<Coin*> coins;
	//Number* counterCoins;
	std::vector<Number*> counterCoins;
	
};
#endif // _SCENE_INCLUDE

