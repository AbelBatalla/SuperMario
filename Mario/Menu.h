#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TexturedQuad.h"
#include "Sprite.h"


class Menu
{
public:
	Menu();
	~Menu();

	void init();
	void render();
	void update();
	void initShaders();

private:
	glm::mat4 projection;
	float currentTime;
	Sprite* bground_f;
	Texture scene;
	ShaderProgram texProgram;
	TexturedQuad* bground;


};

#endif

