#pragma once

#include <stdio.h>
#include <stdlib.h>
#define GLEW_STATIC
#include "GL\glew.h"
#include "GLFW\glfw3.h"

#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Vertice.h"
#include <vector>

using namespace glm;
using namespace std;

class Modelo {
public:
	Modelo();
	vector<Vertice> vertices;
	GLuint vertexArrayID;
	GLuint bufferID;

	vec3 posicion;
	vec3 escala;
	float angulo;
	vec3 ejesRotacion;

	mat4 obtenerMatrizModelo();

	void cargarVertices(vector<Vertice> vertices);
	void cargarVertexArrayBuffer();
	void dibujar(GLuint modeloID);
private:
	mat4 modelo;
};