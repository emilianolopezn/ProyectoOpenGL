#include "stdafx.h"
#include "Shader.h"

GLuint Shader::getID() {
	return shaderID;
}

Shader::Shader(const char * rutaVertex, 
	const char * rutaFragment) {

	//Obtener codigo del vertex shader en un string
	string codigoVertex = "";

	//Declarar input file stream (para leer archivos)
	ifstream vertexShaderStream(rutaVertex, ios::in);
	//Verificar si se pudo abrir el archivo
	if (vertexShaderStream.is_open()) {
		//Declaramos un string para representar la linea
		string linea = ""; 
		//Recorremos linea por linea del archivo
		while (getline(vertexShaderStream, linea)) {
			//Concatenemos cada linea en codigoVertex
			codigoVertex += linea + "\n";
		}
		//Ya que no lo necesitamos mas, cerramos el stream.
		vertexShaderStream.close();
	}
	else {
		cout << "No se pudo abrir el archivo: " << rutaVertex << endl;
		exit(EXIT_FAILURE);
	}

	//Todo lo mismo pero ahora para el fragment shader
	string codigoFragment;
	ifstream fragmentShaderStream(rutaFragment, ios::in);
	if (fragmentShaderStream.is_open()) {
		string linea = "";
		while (getline(fragmentShaderStream, linea)) {
			codigoFragment += linea + "\n";
		}
		fragmentShaderStream.close();
	}
	else {
		cout << "No se pudo abrir: " << rutaFragment << endl;
		exit(EXIT_FAILURE);
	}

	//Coversion de formatos 
	const char * apuntadorCodigoVertex = codigoVertex.c_str();
	const char * apuntadorCodigoFragment = codigoFragment.c_str();

	//1.- Crear programa
	shaderID = glCreateProgram();
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	//2.- Cargar shaders
	glShaderSource(vertexShaderID, 1, &apuntadorCodigoVertex, NULL);
	glShaderSource(fragmentShaderID, 1, &apuntadorCodigoFragment, NULL);

	//3.- Compilar shaders
	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	//4.- Verificar compilacion
	checarCompilacionShader(vertexShaderID);
	checarCompilacionShader(fragmentShaderID);

	//5.- Adjuntar shaders al programa
	glAttachShader(shaderID, vertexShaderID);
	glAttachShader(shaderID, fragmentShaderID);

	//6.- Vincular el programa a OpenGL
	glLinkProgram(shaderID);

	//7.- Verificar vinculacion
	checarVinculacion(shaderID);

	//8.- Usar el programa
	glUseProgram(shaderID);
}

void Shader::enlazarShader() {
	glUseProgram(shaderID);
}

void Shader::desenlazarShader() {
	glUseProgram(0);
}

void Shader::checarVinculacion(GLuint id) {
	GLint estadoVinculacion, estadoValidacion;

	glGetProgramiv(id, GL_LINK_STATUS, &estadoVinculacion);
	if (estadoVinculacion == GL_FALSE) {
		cout << "Falló la vinculación";
	}

	glGetProgramiv(id, GL_VALIDATE_STATUS, &estadoValidacion);
	if (estadoValidacion == GL_FALSE) {
		cout << "Falló la validación";
	}
}

void Shader::checarCompilacionShader(GLuint id) {
	GLint resultado = GL_FALSE;
	int longitudLog = 0;

	glGetShaderiv(id, GL_COMPILE_STATUS, &resultado);
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &longitudLog);
	if (longitudLog > 0) {
		vector<char> mensajeError(longitudLog + 1);
		glGetShaderInfoLog(id, longitudLog, NULL, &mensajeError[0]);
		for (vector<char>::const_iterator i = mensajeError.begin();
			i != mensajeError.end();
			++i) {
			cout << *i;
		}
	}
}
