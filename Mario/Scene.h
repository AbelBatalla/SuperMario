#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Number.h"
#include "Counter.h"
#include "SimpleView.h"
#include "ItemBlock.h"
#include "Mushroom.h"
#include "Star.h"


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

private:
	TileMap *map;
	Player *player;
	ShaderProgram texProgram;
	Counter *coinCounter;
	Counter *liveCounter;
	Counter *timeCounter;
	Counter *pointsCounter;
	Counter *worldCounter;
	SimpleView* hud;
	float currentTime;
	int playerLives, timeLife;
	glm::mat4 projection;
	int camx, oldPosx, numCoins;
	std::vector<Coin*> coins;
	std::vector<ItemBlock*> itemBlocks;
	std::vector<PowerUp*> powerUps;
	
};
#endif // _SCENE_INCLUDE

