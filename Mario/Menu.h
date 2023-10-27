#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Sprite.h"


class Menu
{
public:
	Menu();
	~Menu();

	void init();
	void render();
	int update();
	void initShaders();

private:
	glm::mat4 projection;
	int idMenu;
	float currentTime;
	Sprite* bground_f;
	Texture menu, menu_1, menu_2, menu_3;
	ShaderProgram texProgram;

};

#endif

