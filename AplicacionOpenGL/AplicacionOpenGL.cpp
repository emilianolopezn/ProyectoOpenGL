// AplicacionOpenGL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#define GLEW_STATIC
#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <vector>
#include "Vertice.h"
#include "Shader.h"
#include "Modelo.h"

using namespace std;
using namespace glm;

Modelo *triangulo;

GLuint posicionID;
GLuint colorID;
//GLuint invertidoID;
GLuint uniformInvertidoID;
// IDs de matrices de transformacion
GLuint modeloID;



Shader * shader;

//float invertido;
bool invertido = false;



void actualizar() {

}

void dibujar() {
	//Dibujar triangulo
	//Enlazar el shader
	shader->enlazarShader();
	glUniform1i(uniformInvertidoID, invertido);
	
	triangulo->dibujar(modeloID);

	//Soltar el vertex array
	glBindVertexArray(0);
	//Desenlazar shader
	shader->desenlazarShader();
}

void callbackTeclado(GLFWwindow * window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {		
		invertido = !invertido;
	}
}

int main()
{
	//Declaracion de ventana
	GLFWwindow *window;
	//Propiedades de ventana
	GLfloat ancho = 1024;
	GLfloat alto = 768;

	triangulo = new Modelo;

	//Inicialización de GLFW
	if (!glfwInit()) {
		//Si no inició bien, terminamos la ejecución
		exit(EXIT_FAILURE);
	}

	//Inicializar la ventana
	window = glfwCreateWindow(ancho, alto, "Mi ventana", NULL, NULL);
	//Verificar la creacion de la ventana
	if (!window) {
		//Terminó con GLFW y cierro el programa
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//Establecer "window" como el contexto de OpenGL
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, callbackTeclado);

	//Se trae funciones de OpenGL moderno
	glewExperimental = true; 
	//Inicializamos GLEW y guardamos el  resultado
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	//Obtener version de OpenGL
	const GLubyte * version = glGetString(GL_VERSION);
	cout << version << endl;
	
	//Inicializar el triangulo
	Vertice v1 = { vec4(0,0.2,0,1), vec4(0.2,0.8,0.3,1)/*, 0.0f*/ };
	Vertice v2 = { vec4(-0.2,-0.2,0,1), vec4(0.45,0.72,0.8,1)/*, 0.0f*/ };
	Vertice v3 = { vec4(0.2,-0.2,0,1), vec4(0.12,0.69,0.74,1)/*, 0.0f*/ };
	
	triangulo->vertices.push_back(v1);
	triangulo->vertices.push_back(v2);
	triangulo->vertices.push_back(v3);

	triangulo->cargarVertexArrayBuffer();

	//Crear instancia de shader
	const char * rutaVertex = "ShaderSimple.vertexshader";
	const char * rutaFragment = "ShaderSimple.fragmentshader";
	shader = new Shader(rutaVertex, rutaFragment);

	//Mapeo de atributos de shader
	posicionID = glGetAttribLocation(shader->getID(), "posicion");
	colorID = glGetAttribLocation(shader->getID(), "color");
	//invertidoID = glGetAttribLocation(shader->getID(), "invertido");
	uniformInvertidoID = 
		glGetUniformLocation(shader->getID(), "invertido");
	modeloID =
		glGetUniformLocation(shader->getID(), "modelo");

	shader->desenlazarShader(); 

	//Habilitar atributos del vertex shader
	glEnableVertexAttribArray(posicionID);
	glEnableVertexAttribArray(colorID);
	//glEnableVertexAttribArray(invertidoID);


	//Especificar a OpenGL como usar la memoria con
	//ese atributo
	glVertexAttribPointer(posicionID, 4, GL_FLOAT,
		GL_FALSE, sizeof(Vertice), 0);
	glVertexAttribPointer(colorID, 4, GL_FLOAT,
		GL_FALSE, sizeof(Vertice), (void *)sizeof(vec4));

	//Soltar buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	//Modificar las transformaciones
	triangulo->posicion = vec3(0.5, -0.5, 0);
	triangulo->angulo = 45.0f;
	triangulo->ejesRotacion = vec3(0, 0, 1);
	triangulo->escala = vec3(0.5, 0.5, 0);


	//Draw Loop
	//Continua mientras la ventana se encuentre abierta
	while (!glfwWindowShouldClose(window)) {
		//Establece el area de render
		glViewport(0, 0, ancho, alto);
		//Establece el color con el que se le limpia la pantalla
		glClearColor(0, 0, 0, 1);
		//Limpiar pantalla
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Rutina de dibujo
		actualizar();
		dibujar();

		//Intercambio de buffers
		glfwSwapBuffers(window);
		//Buscar señales de entrada
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
    return 0;
}

