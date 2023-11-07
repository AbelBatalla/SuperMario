#ifndef _COUNTER_INCLUDE
#define _COUNTER_INCLUDE


#include "Sprite.h"
#include "Number.h"
class Counter
{
public:
	void init(ShaderProgram& shaderProgram, int posX, int posY, int initValue, int n);
	void set(int num);
	void render();
	void update(int deltaTime);
private:
	std::vector<Number*> counter;
	int size;
	std::vector<int> transformNumber(int num);
};

#endif

