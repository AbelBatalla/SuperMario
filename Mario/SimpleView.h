#ifndef _SIMPLEVIEW_INCLUDE
#define _SIMPLEVIEW_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Sprite.h"


class SimpleView
{
public:

	enum TypeMenu { CREDITS, INSTRUCTIONS, HUD, GAME_OVER};

	SimpleView(TypeMenu type);
	~SimpleView();

	void init();
	void render();
	int update();
	void initShaders();



private:
	glm::mat4 projection;
	Sprite* bground_f;
	Texture menu;
	ShaderProgram texProgram;
};

#endif

