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
#include "Score.h"
#include "Goomba.h"
#include "Koopa.h"
#include "Brick.h"



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
	void newScore(int s, glm::vec2 posScore);
	void updateGoombas(int deltaTime);
	void updateKoopas(int deltaTime);
	void initGoombas();
	void initKoopas();
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
	int playerLives, timeLife, hitLast, camx, oldPosx, numCoins, playerScore;
	glm::mat4 projection;
	std::vector<Coin*> coins;
	std::vector<Goomba*> goombas;
	std::vector<Koopa*> koopas;
	std::vector<ItemBlock*> itemBlocks;
	std::vector<PowerUp*> powerUps;
	std::vector<Score*> scores;
	std::vector<Brick*> bricks;
	
};
#endif // _SCENE_INCLUDE

