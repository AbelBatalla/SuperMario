#include "Counter.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

void Counter::init(ShaderProgram& shaderProgram, int posX, int posY, int initValue, int n) {
	size = n;
	std::vector<int> number(size, 0);
	int stride = 0;
	for (int i = 0; i < size; ++i) {
		counter.push_back(new Number(0,0));
		counter[i]->init(glm::ivec2(0, -80), shaderProgram);
		counter[i]->setPosition(glm::vec2(posX * 16 + stride, posY * 16));
		stride += 8;
	}
	set(initValue);
}

void Counter::set(int num) {
	
	if (size > 1) {
		std::vector<int> number = transformNumber(num);
		int digitsToFill = size - number.size();

		if (digitsToFill < 0) {
			// Si el número es más grande que el tamaño, redimensiona number
			number.resize(size);
		}
		else {
			// Rellena los dígitos no utilizados con ceros
			std::vector<int> zeros(digitsToFill, 0);
			number.insert(number.begin(), zeros.begin(), zeros.end());
		}

		for (int i = 0; i < size; ++i) {
			counter[i]->setNumber(number[i]);
		}
	}
	else counter[0]->setNumber(num);
}

void Counter::render() {
	for (int i = 0; i < size; ++i) {
		counter[i]->render();
	}
}

void Counter::update(int deltaTime) {
	for (int i = 0; i < size; ++i) {
		counter[i]->update(deltaTime);
	}
}

std::vector<int> Counter::transformNumber(int num) {
	std::vector<int> result; // Inicializa un vector vacío
	if (num == 0) {
		result.push_back(0); // Agrega un 0 si el número es 0
	}
	else {
		while (num > 0) {
			int digit = num % 10; // Obtiene el último dígito
			result.insert(result.begin(), digit); // Agrega el dígito al inicio del vector
			num /= 10; // Elimina el último dígito
		}
	}
	return result;
}
