#include "stdafx.h"
#include "Modelo.h"

Modelo::Modelo() {
	posicion = vec3(0, 0, 0);
	escala = vec3(1, 1, 1);
	angulo = 0.0f;
	ejesRotacion = vec3(0, 0, 1);
}

mat4 Modelo::obtenerMatrizModelo() {
	modelo = mat4(1.0f);
	modelo = translate(modelo, posicion);
	modelo = rotate(modelo, angulo, ejesRotacion);
	modelo = scale(modelo, escala);
	return modelo;
}

void Modelo::cargarVertices(vector<Vertice> vertices) {
	this->vertices = vertices;
}

void Modelo::cargarVertexArrayBuffer() {
	//Crear vertex array
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	//Crear vertex buffer
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(Vertice)*vertices.size(),
		vertices.data(), GL_STATIC_DRAW);
}

void Modelo::dibujar(GLuint modeloID) {
	glUniformMatrix4fv(modeloID, 1, GL_FALSE,
		&(obtenerMatrizModelo()[0][0]));

	//Cargar el vertex array
	glBindVertexArray(vertexArrayID);
	//Dibujar arrays
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}