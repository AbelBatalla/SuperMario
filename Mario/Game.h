#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include "Scene.h"
#include "Menu.h"
#include "SimpleView.h"
#include "SoundManager.h"


#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480


// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

private:
	~Game();
	Game(){}
	
public:
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init(string level, bool nextLevel, bool death, bool firstTime);
	bool update(int deltaTime);
	void render();
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);
	
	bool getKey(int key) const;
	bool getSpecialKey(int key) const;
	void setSpace(bool set);
	string getActualMap();
	string getNextMap();


private:
	bool bPlay, goGame, goInstructions, goCredits, goGameOver, mute;                       // Continue to play game?
	Scene scene;                      // Scene to render
	bool keys[256], specialKeys[256], spaceRelease; // Store key states so that                                  // we can have access at any time
	Menu* menu;
	SimpleView* instructions;
	SimpleView* credits;
	SimpleView* game_over;
	ISoundEngine* engine;
	irrklang::ISound* sound;
	int idMenu;
	string actualMap, nextMap;

};


#endif // _GAME_INCLUDE


