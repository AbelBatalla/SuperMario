#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "Coin.h"
#include "Number.h"


#define SCREEN_X 0
#define SCREEN_Y 32 //-80

#define ZOOM 2

#define INIT_PLAYER_X_TILES 7
#define INIT_PLAYER_Y_TILES 9


Scene::Scene()
{
	map = NULL;
	player = NULL;
	coinCounter = NULL;
	liveCounter = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	if (coinCounter != NULL)
		delete coinCounter;
	if (liveCounter != NULL)
		delete liveCounter;
}


void Scene::init()
{
	initShaders();
	numCoins =0;
	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);


	hud = new SimpleView(SimpleView::TypeMenu::HUD);
	hud->init();


	coinCounter = new Counter();
	liveCounter = new Counter();
	timeCounter = new Counter();
	pointsCounter = new Counter();
	worldCounter = new Counter();
	coinCounter->init(texProgram, 6, 6, 0, 2);
	liveCounter->init(texProgram, 18, 6, 3, 1);
	timeCounter->init(texProgram, 14, 6, 200, 3);
	pointsCounter->init(texProgram, 1, 6, 0, 4);
	worldCounter->init(texProgram, 10, 6, 1, 1);


	std::vector<glm::ivec2> coinPositions = map->getCoinPositions();
	for (const glm::ivec2& coinPos : coinPositions) {
		Coin* coin = new Coin();
		coin->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram); // Inicializa una moneda en la posici�n del mapa
		coin->setPosition(glm::vec2(coinPos.x * map->getTileSize(), coinPos.y * map->getTileSize()));
		coins.push_back(coin); // Agrega la moneda al vector de monedas
	}

	std::vector<glm::ivec3> itemProperties = map->getItemPositions();
	for (const glm::ivec3& itemProp : itemProperties) {
		ItemBlock* item = new ItemBlock(itemProp.x, itemProp.y, itemProp.z);
		item->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, map);
		item->setPosition(glm::vec2(itemProp.x * map->getTileSize(), itemProp.y * map->getTileSize()));
		itemBlocks.push_back(item);
	}


	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	playerLives = player->getLives();
	//liveCounter->set(player->getLives());
	projection = glm::ortho(0.f, float(SCREEN_WIDTH / ZOOM - 1), float(SCREEN_HEIGHT / ZOOM - 1), 0.f);
	currentTime = 0.0f;
	camx = 0; //Posicio x al mon de l'inici de la pantalla
	oldPosx = INIT_PLAYER_X_TILES;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	for (int i = 0; i < coins.size(); i++) {
		if (coins[i] != nullptr) {
			if (coins[i]->isCollected(player->getPos(), 16, player->getMarioState())) {
				delete coins[i]; // Elimina la moneda actual
				coins[i] = nullptr;
				++numCoins;
				coinCounter->set(numCoins);
			}
			else coins[i]->update(deltaTime);
		}
	}

	for (int i = 0; i < itemBlocks.size(); i++) {
		if (itemBlocks[i] != nullptr) {
			if (itemBlocks[i]->isHit(player->getPos())) {
				powerUps.push_back(itemBlocks[i]->getPowerUp());
				delete itemBlocks[i];
				itemBlocks[i] = nullptr;
			}
			else itemBlocks[i]->update(deltaTime);
		}
	}
	
	for (int i = 0; i < powerUps.size(); i++) {
		if (powerUps[i] != nullptr) {
			if (powerUps[i]->update(deltaTime)) { //T or F segons si delete o no (timeout? death?)
				if (powerUps[i]->checkCollision(player->getPos(), player->getMarioState())) {
					if (powerUps[i]->type() == 0) player->turnSuper();
					else if (powerUps[i]->type() == 1) player->turnStar();
					delete powerUps[i];
					powerUps[i] = nullptr;
				}
			}
			else {
				delete powerUps[i];
				powerUps[i] = nullptr;
			}
		}
	}



	if (player->getLives() != playerLives) liveCounter->set(player->getLives());
	timeCounter->set(200 - player->getTimeLife()/1000);
	coinCounter->update(deltaTime);
	liveCounter->update(deltaTime);
	timeCounter->update(deltaTime);
	pointsCounter->update(deltaTime);
	worldCounter->update(deltaTime);

	// Limpia las monedas nulas del vector (opcional)
	//coins.erase(std::remove(coins.begin(), coins.end(), nullptr), coins.end());

	
	if (player->update(deltaTime, camx)) {
		player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
		camx = 0;
		oldPosx = INIT_PLAYER_X_TILES;
	}
}

void Scene::render()
{
	glm::mat4 modelview;


	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	int posx = player->getPosX();
	float scrollMax = (0.93 / 2.0) * SCREEN_WIDTH / ZOOM;
	float scrollStart = (1.75 / 5.0) * SCREEN_WIDTH / ZOOM;
	if (posx >= camx + scrollMax)
	{
		camx = posx - scrollMax;
	}
	else if (posx > camx + scrollStart and oldPosx < posx)
	{
		camx = camx + 1; //posx - scrollMax;
	}
	oldPosx= posx;
	modelview = glm::translate(modelview, glm::vec3(-camx, 0.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	for (const Coin* coin : coins) {
		if (coin != nullptr) {
			coin->render(camx);
		}
	}
	for (const ItemBlock* itemBlock : itemBlocks) {
		if (itemBlock != nullptr) {
			itemBlock->render(camx);
		}
	}
	for (const PowerUp* pu : powerUps) {
		if (pu != nullptr) {
			pu->render(camx);
		}
	}

	player->render(camx);
	coinCounter->render();
	liveCounter->render();
	timeCounter->render();
	pointsCounter->render();
	worldCounter->render();
	hud->render();
	
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}