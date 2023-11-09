#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "Coin.h"
#include "Number.h"
#include <windows.h>


#define SCREEN_X 0
#define SCREEN_Y -80

#define ZOOM 2

#define INIT_PLAYER_X_TILES 7
#define INIT_PLAYER_Y_TILES 9
#define PLAYER_GOAL_x 197


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
	for (int i = 0; i < goombas.size(); ++i) {
		delete goombas[i];
		goombas[i] = nullptr;
	}
	goombas.erase(std::remove(goombas.begin(), goombas.end(), nullptr), goombas.end());

	for (int i = 0; i < koopas.size(); ++i) {
		delete koopas[i];
		koopas[i] = nullptr;
	}
	koopas.erase(std::remove(koopas.begin(), koopas.end(), nullptr), koopas.end());
	
}

void Scene::initGoombas() {
	std::vector<glm::ivec2> goombaPositions = map->getGoombaPositions();
	for (const glm::ivec2& goombaPos : goombaPositions) {
		Goomba* g = new Goomba();
		g->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, map); // Inicializa una moneda en la posici�n del mapa
		g->setPosition(glm::vec2(goombaPos.x * map->getTileSize(), goombaPos.y * map->getTileSize()));
		goombas.push_back(g); // Agrega la moneda al vector de monedas
	}
}

void Scene::initKoopas() {
	std::vector<glm::ivec2> koopaPositions = map->getKoopaPositions();
	for (const glm::ivec2& koopaPos : koopaPositions) {
		Koopa* k = new Koopa();
		k->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, map); // Inicializa una moneda en la posici�n del mapa
		k->setPosition(glm::vec2(koopaPos.x * map->getTileSize(), (koopaPos.y - 1) * map->getTileSize()));
		koopas.push_back(k); // Agrega la moneda al vector de monedas
	}
}

void Scene::init(string level)
{
	initShaders();
	numCoins = 0;
	playerScore = 0;
	hitLast = 0;
	goombas.erase(goombas.begin(), goombas.end());
  
	koopas.erase(koopas.begin(), koopas.end());
	map = TileMap::createTileMap(level, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);



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
	if(level == "levels/mapa4.txt") worldCounter->init(texProgram, 10, 6, 1, 1);
	else worldCounter->init(texProgram, 10, 6, 2, 1);

	initGoombas();
	initKoopas();
	

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

	std::vector<glm::ivec3> brickProperties = map->getBrickPositions();
	for (const glm::ivec3& brickProp : brickProperties) {
		Brick* brick = new Brick(brickProp.x, brickProp.y, brickProp.z);
		brick->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, map);
		brick->setPosition(glm::vec2(brickProp.x * map->getTileSize(), brickProp.y * map->getTileSize()));
		bricks.push_back(brick);
	}

	
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	playerLives = player->getLives();
	
	projection = glm::ortho(0.f, float(SCREEN_WIDTH / ZOOM - 1), float(SCREEN_HEIGHT / ZOOM - 1), 0.f);
	currentTime = 0.0f;
	camx = 0; //Posicio x al mon de l'inici de la pantalla
	oldPosx = INIT_PLAYER_X_TILES;
}

void Scene::updateGoombas(int deltaTime) {
	for (int i = 0; i < goombas.size(); i++) {
		if (goombas[i] != nullptr) {
			if (goombas[i]->getDeathTime() != 0 and (currentTime - goombas[i]->getDeathTime()) > 400) {
				delete goombas[i]; // Elimina la moneda actual
				goombas[i] = nullptr;
			}
			if (goombas[i] != nullptr) {
				goombas[i]->update(deltaTime);
				if (not player->isKilled() and goombas[i]->getDeathTime() == 0) {
					int state = 0;
					state = goombas[i]->checkCollision(player->getPos(), player->getMarioState(), player->getMarioStar(), player->goingDown());
					if (state == 3) {
						newScore(100, player->getPos());
						playerScore += 100;
						pointsCounter->set(playerScore);
					}
					else if (state == 2) {
						if (player->getKillJump()) {
							newScore(200, player->getPos());
							playerScore += 200;
						}
						else {
							newScore(100, player->getPos());
							playerScore += 100;
						}
						pointsCounter->set(playerScore);
						player->setKillJump();
					}
					else if (state == 1) {
						if (!player->getMarioInvincible()) player->kill();
					}
				}
				for (int j = 0; j < goombas.size(); j++) {
					if (goombas[j] != nullptr) {
						int state = goombas[j]->checkCollisionEnemy(goombas[i]->getPos(), true);
						if (state) {
							goombas[i]->toggleMoveRight();
							goombas[j]->toggleMoveRight();

						}
					}
				}
			}
		}
	}
}

void Scene::updateKoopas(int deltaTime) {
	for (int i = 0; i < koopas.size(); i++) {
		if (koopas[i] != nullptr) {
			if (koopas[i]->getDeathTime() != 0 and (currentTime - koopas[i]->getDeathTime()) > 700 and koopas[i]->getDeathTime() != 0 and (currentTime - koopas[i]->getDeathTime()) < 4000) {
				
				if (koopas[i]->checkCollision(player->getPos(), player->getMarioState(), player->getMarioStar(), false) and not koopas[i]->getMoveShell()) {
					koopas[i]->toggleMoveShell();
					koopas[i]->setDeathTime(currentTime, false);
				}
			}
			else if (((not koopas[i]->getMoveShell()) and koopas[i]->getDeathTime() != 0) and (currentTime - koopas[i]->getDeathTime()) > 4000 and koopas[i]->getDeathTime() != 0 and (currentTime - koopas[i]->getDeathTime()) < 4300) {
				koopas[i]->setDeathTime(koopas[i]->getDeathTime(), true);
			}
			else if ((not koopas[i]->getMoveShell()) and koopas[i]->getDeathTime() != 0 and (currentTime - koopas[i]->getDeathTime()) > 4300 and koopas[i]->getShell()) {
				koopas[i]->toggleShell();
				koopas[i]->setDeathTime(0, false);
			}
			else if (koopas[i]->getMoveShell() and (currentTime - koopas[i]->getDeathTime()) > 10000) {
				//koopas[i]->setDeathTime(0);
				delete koopas[i]; // Elimina la moneda actual
				koopas[i] = nullptr;
			}


			if (koopas[i] != nullptr) {
				koopas[i]->update(deltaTime);
				if (not player->isKilled() and koopas[i]->getDeathTime() == 0) {
					int state = 0;
					state = koopas[i]->checkCollision(player->getPos(), player->getMarioState(), player->getMarioStar(), player->getKillJump());
					if (state == 2) {
						if (player->getKillJump()) {
							newScore(200, player->getPos());
							playerScore += 200;
						}
						else {
							newScore(100, player->getPos());
							playerScore += 100;
						}
						pointsCounter->set(playerScore);
						player->setKillJump();
					}
					else if (state == 1) {
						if (!player->getMarioInvincible()) player->kill();
					}
				}
				if (koopas[i]->getDeathTime() != 0 and koopas[i]->getMoveShell()) {
					for (int j = 0; j < goombas.size(); j++) {
						if (goombas[j] != nullptr) {
							int state = koopas[i]->checkCollision(goombas[j]->getPos(),false, false, false);
							if (state) {
								newScore(500, goombas[j]->getPos());
								playerScore += 500;
								pointsCounter->set(playerScore);
								delete goombas[j]; // Elimina la moneda actual
								goombas[j] = nullptr;
							}
						}
					}
				} else if (koopas[i]->getDeathTime() == 0 and not koopas[i]->getMoveShell()) {
					for (int j = 0; j < goombas.size(); j++) {
						if (goombas[j] != nullptr) {
							int state = goombas[j]->checkCollisionEnemy(koopas[i]->getPos(), false);
							if (state) {
								koopas[i]->toggleMoveRight();
								goombas[j]->toggleMoveRight();
								koopas[i]->update(deltaTime);
							}
						}
					}
				}
			}
		}
	}
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
				newScore(200, player->getPos());
				playerScore += 200;
				coinCounter->set(numCoins);
				pointsCounter->set(playerScore);
			}
			else coins[i]->update(deltaTime);
		}
	}

	if (!player->inTransition()) {
		updateGoombas(deltaTime);
		updateKoopas(deltaTime);
	}

	if (player->getLives() != playerLives) liveCounter->set(player->getLives());
	timeCounter->set(200 - player->getTimeLife() / 1000);
	coinCounter->update(deltaTime);
	liveCounter->update(deltaTime);
	timeCounter->update(deltaTime);
	pointsCounter->update(deltaTime);
	worldCounter->update(deltaTime);



	if (player->update(deltaTime, camx)) {
		player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
		camx = 0;
		oldPosx = INIT_PLAYER_X_TILES;
	}

	int primaryCollisonBlock = -1;
	if (hitLast == 0) primaryCollisonBlock = map->getPrimaryCollisionBlock(player->getPos());
	bool found = false;
	if (primaryCollisonBlock == -1) {
		found = true;
	}

	for (int i = 0; i < itemBlocks.size(); i++) {
		if (itemBlocks[i] != nullptr) {
			if (!found and hitLast == 0 and itemBlocks[i]->isHit(player->getPos(), primaryCollisonBlock)) {
				hitLast++;
				found = true;
				powerUps.push_back(itemBlocks[i]->getPowerUp());
				delete itemBlocks[i];
				itemBlocks[i] = nullptr;
			}
			else itemBlocks[i]->update(deltaTime);
		}
	}

	for (int i = 0; i < bricks.size(); i++) {
		if (bricks[i] != nullptr) {
			if (!found and hitLast == 0 and bricks[i]->isHit(player->getPos(), player->getMarioState(), player->getMarioTransitionState(), primaryCollisonBlock)) {
				found = true;
				hitLast++;
				int state = bricks[i]->getState();
				if (state != 2) powerUps.push_back(bricks[i]->getPowerUp());
				if (state == 0) { //Brick Broken
					player->collisionUp();
					map->setClearBlock(bricks[i]->getPos());
					delete bricks[i];
					bricks[i] = nullptr;
				}
				else bricks[i]->update(deltaTime);
			}
			else bricks[i]->update(deltaTime);
		}
	}
	if (hitLast != 0) {
		++hitLast;
		hitLast = hitLast % 10;
	}
	
	for (int i = 0; i < powerUps.size(); i++) {
		if (powerUps[i] != nullptr) {
			if (powerUps[i]->update(deltaTime)) {
				if (powerUps[i]->type() < 2 and powerUps[i]->checkCollision(player->getPos(), player->getMarioState())) {
					newScore(1000, player->getPos());
					playerScore += 1000;
					pointsCounter->set(playerScore);
					if (powerUps[i]->type() == 0) player->turnSuper();
					else if (powerUps[i]->type() == 1) player->turnStar();
					delete powerUps[i];
					powerUps[i] = nullptr;
				}
			}
			else {
				if (powerUps[i]->type() == 2) {
					newScore(200, powerUps[i]->getPos());
					playerScore += 200;
					++numCoins;
					coinCounter->set(numCoins);
					pointsCounter->set(playerScore);
					powerUps[i] = nullptr;
					powerUps.erase(powerUps.begin() + i);
				}
				else {
					delete powerUps[i];
					powerUps[i] = nullptr;
				}
			}
		}
	}

	for (int i = 0; i < scores.size(); i++) {
		if (scores[i] != nullptr) {
			if (!(scores[i]->update(deltaTime))) {
				delete scores[i];
				scores[i] = nullptr;
			}
		}
	}
	if (player->getPosX() >= PLAYER_GOAL_x * map->getTileSize()) {
		if (Game::instance().getActualMap() == "levels/mapa3.txt") Game::instance().init("credits", true, false);
		else Game::instance().init(Game::instance().getNextMap(), true, false);
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

	for (const Brick* brick : bricks) {
		if (brick != nullptr) {
			brick->render(camx);
		}
	}

	for (const PowerUp* pu : powerUps) {
		if (pu != nullptr) {
			pu->render(camx);
		}
	}

	for (const Score* sc : scores) {
		if (sc != nullptr) {
			sc->render(camx);
		}
	}

	for (const Goomba* g : goombas) {
		if (g != nullptr) {
			g->render(camx);
		}
	}

	for (const Koopa* k : koopas) {
		if (k != nullptr) {
			k->render(camx);
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
		std::cout << "Vertex Shader Error" << endl;
		std::cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		std::cout << "Fragment Shader Error" << endl;
		std::cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		std::cout << "Shader Linking Error" << endl;
		std::cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}


void Scene::newScore(int s, glm::vec2 posScore)
{
	Score* sc = new Score();
	sc->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, s);
	sc->setPosition(posScore);
	scores.push_back(sc);
}