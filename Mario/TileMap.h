#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include "Coin.h"


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

private:
	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	~TileMap();

	void render() const;
	void free();
	
	int getTileSize() const { return tileSize; }
	int getMapHeight() const { return mapSize.y; }

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveLeftPlayer(const glm::ivec2& pos, const glm::ivec2& size, int* posX) const;
	bool collisionMoveRightPlayer(const glm::ivec2& pos, const glm::ivec2& size, int* posX) const;
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;
	bool collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size) const;
	const std::vector<glm::ivec2>& getCoinPositions() const;
	const std::vector<glm::ivec2>& getGoombaPositions() const;
	const std::vector<glm::ivec2>& getKoopaPositions() const;
	const std::vector<glm::ivec3>& getItemPositions() const;
	const std::vector<glm::ivec3>& getBrickPositions() const;
	void setClearBlock(glm::ivec2 pos);
	int getPrimaryCollisionBlock(glm::ivec2 pos);
	
private:
	bool loadLevel(const string &levelFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	int nTiles;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int *map;
	std::vector<glm::ivec2> coinPositions;
	std::vector<glm::ivec2> goombaPositions;
	std::vector<glm::ivec2> koopaPositions;
	std::vector<glm::ivec3> itemPositions;
	std::vector<glm::ivec3> brickPositions;

};


#endif // _TILE_MAP_INCLUDE


