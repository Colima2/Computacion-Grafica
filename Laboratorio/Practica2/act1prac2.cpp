//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>

//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>

//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"

//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;

//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
//shaders nuevos se crearían acá

float angulo = 0.0f;

//color café/marrón en RGB : 0.478, 0.255, 0.067

using std::vector;

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = {
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3

	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* piramidetriangular = new Mesh();
	piramidetriangular->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(piramidetriangular);
}

//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	unsigned int piramidecuadrangular_indices[] = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4

	};
	GLfloat piramidecuadrangular_vertices[] = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh* piramide = new Mesh();
	piramide->CreateMesh(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}

//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{
	GLfloat vertices_letras[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		1.0f,	1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		1.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		-1.0f,  1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,

	};
	MeshColor* letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras, 36);
	meshColorList.push_back(letras);

	GLfloat vertices_triangulomagenta[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	1.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	1.0f,
		0.0f,	1.0f,		0.5f,			1.0f,	0.0f,	1.0f,

	};

	//triangulo amarillo
	GLfloat vertices_trianguloamarillo[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	1.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	1.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			1.0f,	1.0f,	0.0f,

	};

	//triangulo verde
	GLfloat vertices_trianguloverde[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	1.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			0.0f,	1.0f,	0.0f,

	};

	//triangulo rojo
	GLfloat vertices_triangulorojo[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,

	};

	MeshColor* triangulomagenta = new MeshColor();
	triangulomagenta->CreateMeshColor(vertices_triangulomagenta, 18);
	meshColorList.push_back(triangulomagenta);

	MeshColor* trianguloamarillo = new MeshColor();
	trianguloamarillo->CreateMeshColor(vertices_trianguloamarillo, 18);
	meshColorList.push_back(trianguloamarillo);

	MeshColor* trianguloverde = new MeshColor();
	trianguloverde->CreateMeshColor(vertices_trianguloverde, 18);
	meshColorList.push_back(trianguloverde);

	MeshColor* triangulorojo = new MeshColor();
	triangulorojo->CreateMeshColor(vertices_triangulorojo, 18);
	meshColorList.push_back(triangulorojo);


	//letra Y
	GLfloat vertices_letraylila[] = {
		//X			Y			Z			R		G		B

		-0.6f, 0.4f, 0.0f,					0.29f, 0.00f, 0.51f,
		-0.6f, 0.0f, 0.0f,					0.29f, 0.00f, 0.51f,
		-0.5f, 0.0f, 0.0f,					0.29f, 0.00f, 0.51f,

		-0.6f, 0.4f, 0.0f,					0.29f, 0.00f, 0.51f,
		-0.5f, 0.4f, 0.0f,					0.29f, 0.00f, 0.51f,
		-0.5f, 0.0f, 0.0f,					0.29f, 0.00f, 0.51f,

		-0.5f, 0.1f, 0.0f,					0.29f, 0.00f, 0.51f,
		-0.3f, 0.1f, 0.0f,					0.29f, 0.00f, 0.51f,
		-0.5f, 0.0f, 0.0f,					0.29f, 0.00f, 0.51f,

		-0.3f, 0.0f, 0.0f,					0.29f, 0.00f, 0.51f,
		-0.3f, 0.1f, 0.0f,					0.29f, 0.00f, 0.51f,
		-0.5f, 0.0f, 0.0f,					0.29f, 0.00f, 0.51f,

		-0.3f, 0.4f, 0.0f,					0.29f, 0.00f, 0.51f,
		-0.3f, -0.3f, 0.0f,					0.29f, 0.00f, 0.51f,
		-0.2f, -0.3f, 0.0f,					0.29f, 0.00f, 0.51f,

		-0.3f, 0.4f, 0.0f,					0.29f, 0.00f, 0.51f,
		-0.2f, 0.4f, 0.0f,					0.29f, 0.00f, 0.51f,
		-0.2f, -0.3f, 0.0f,					0.29f, 0.00f, 0.51f,

		-0.6f, -0.3f, 0.0f,					0.29f, 0.00f, 0.51f,
		-0.6f, -0.4f, 0.0f,					0.29f, 0.00f, 0.51f,
		-0.2f, -0.3f, 0.0f,					0.29f, 0.00f, 0.51f,

		-0.2f, -0.3f, 0.0f,					0.29f, 0.00f, 0.51f,
		-0.6f, -0.4f, 0.0f,					0.29f, 0.00f, 0.51f,
		-0.2f, -0.4f, 0.0f,					0.29f, 0.00f, 0.51f,
	};


	//letra C
	GLfloat vertices_letraCazul[] = {

		-0.1f, 0.4f, 0.0f,					0.68f, 0.85f, 0.90f,
		-0.1f, -0.4f, 0.0f,					0.68f, 0.85f, 0.90f,
		0.0f, -0.4f, 0.0f,					0.68f, 0.85f, 0.90f,

		-0.1f, 0.4f, 0.0f,					0.68f, 0.85f, 0.90f,
		0.0f, 0.4f, 0.0f,					0.68f, 0.85f, 0.90f,
		0.0f, -0.4f, 0.0f,					0.68f, 0.85f, 0.90f,

		0.0f, 0.4f, 0.0f,					0.68f, 0.85f, 0.90f,
		0.0f, 0.3f, 0.0f,					0.68f, 0.85f, 0.90f,
		0.3f, 0.3f, 0.0f,					0.68f, 0.85f, 0.90f,

		0.0f, 0.4f, 0.0f,					0.68f, 0.85f, 0.90f,
		0.3f, 0.4f, 0.0f,					0.68f, 0.85f, 0.90f,
		0.3f, 0.3f, 0.0f,					0.68f, 0.85f, 0.90f,

		0.0f, -0.4f, 0.0f,					0.68f, 0.85f, 0.90f,
		0.3f, -0.4f, 0.0f,					0.68f, 0.85f, 0.90f,
		0.3f, -0.3f, 0.0f,					0.68f, 0.85f, 0.90f,

		0.0f, -0.4f, 0.0f,					0.68f, 0.85f, 0.90f,
		0.0f, -0.3f, 0.0f,					0.68f, 0.85f, 0.90f,
		0.3f, -0.3f, 0.0f,					0.68f, 0.85f, 0.90f,


	};

	GLfloat vertices_letraErosa[] = {
		//letra E

		0.4f, 0.4f, 0.0f,					1.00f, 0.73f, 0.85f,
		0.4f, -0.4f, 0.0f,					1.00f, 0.73f, 0.85f,
		0.5f, -0.4, 0.0f,					1.00f, 0.73f, 0.85f,

		0.4f, 0.4f, 0.0f,					1.00f, 0.73f, 0.85f,
		0.5f, 0.4f, 0.0f,					1.00f, 0.73f, 0.85f,
		0.5f, -0.4, 0.0f,					1.00f, 0.73f, 0.85f,

		0.5f, 0.4f, 0.0f,					1.00f, 0.73f, 0.85f,
		0.5f, 0.3f, 0.0f,					1.00f, 0.73f, 0.85f,
		0.7f, 0.4f, 0.0f,					1.00f, 0.73f, 0.85f,

		0.7f, 0.3f, 0.0f,					1.00f, 0.73f, 0.85f,
		0.5f, 0.3f, 0.0f,					1.00f, 0.73f, 0.85f,
		0.7f, 0.4f, 0.0f,					1.00f, 0.73f, 0.85f,

		0.5f, 0.1f, 0.0f,					1.00f, 0.73f, 0.85f,
		0.5f, 0.0f, 0.0f,					1.00f, 0.73f, 0.85f,
		0.7f, 0.0f, 0.0f,					1.00f, 0.73f, 0.85f,

		0.5f, 0.1f, 0.0f,					1.00f, 0.73f, 0.85f,
		0.7f, 0.1f, 0.0f,					1.00f, 0.73f, 0.85f,
		0.7f, 0.0f, 0.0f,					1.00f, 0.73f, 0.85f,

		0.5f, -0.4f, 0.0f,					1.00f, 0.73f, 0.85f,
		0.7f, -0.4f, 0.0f,					1.00f, 0.73f, 0.85f,
		0.7f, -0.3f, 0.0f,					1.00f, 0.73f, 0.85f,

		0.5f, -0.4f, 0.0f,					1.00f, 0.73f, 0.85f,
		0.5f, -0.3f, 0.0f,					1.00f, 0.73f, 0.85f,
		0.7f, -0.3f, 0.0f,					1.00f, 0.73f, 0.85f,

	};



	MeshColor* letraylila = new MeshColor();
	letraylila->CreateMeshColor(vertices_letraylila, (24 * 6));
	meshColorList.push_back(letraylila);

	MeshColor* letracazul = new MeshColor();
	letracazul->CreateMeshColor(vertices_letraCazul, (18 * 6));
	meshColorList.push_back(letracazul);

	MeshColor* letraerosa = new MeshColor();
	letraerosa->CreateMeshColor(vertices_letraErosa, (24 * 6));
	meshColorList.push_back(letraerosa);


	GLfloat vertices_cuadradoazul[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
		0.5f,	-0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
		-0.5f,	0.5f,		0.5f,			0.0f,	0.0f,	1.0f,

	};

	//crear cuadrado cafe

	GLfloat vertices_cuadradocafe[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.478, 0.255, 0.067,
		0.5f,	-0.5f,		0.5f,			0.478, 0.255, 0.067,
		0.5f,	0.5f,		0.5f,			0.478, 0.255, 0.067,
		-0.5f,	-0.5f,		0.5f,			0.478, 0.255, 0.067,
		0.5f,	0.5f,		0.5f,			0.478, 0.255, 0.067,
		-0.5f,	0.5f,		0.5f,			0.478, 0.255, 0.067,

	};

	MeshColor* cuadradoazul = new MeshColor();
	cuadradoazul->CreateMeshColor(vertices_cuadradoazul, 36);
	meshColorList.push_back(cuadradoazul);

	MeshColor* cuadradocafe = new MeshColor();
	cuadradocafe->CreateMeshColor(vertices_cuadradocafe, 36);
	meshColorList.push_back(cuadradocafe);

}


void CreateShaders()
{

	Shader* shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);
}


int main()
{
	mainWindow = Window(800, 800);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	CrearPiramideCuadrangular(); //índice 2 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	glm::mat4 projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);
	//glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad

	///creados por mi
	glm::mat4 model1(1.0);
	glm::mat4 model2(1.0);
	glm::mat4 model3(1.0);


	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(0.74, 0.67, 0.82, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad


		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));


		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		//========================================================================================
		// letra Y
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		//amarillo
		model = glm::mat4(1.0);
		//MUEVE EN X 3.5 IZQ   Y ARRIBA 2.5
		model = glm::translate(model, glm::vec3(-3.0f, 0.0f, -4.0f));
		//model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		meshColorList[5]->RenderMeshColor(); //LETRA Y		


		//========================================================================================
		//	LETRA C
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model1 = glm::mat4(1.0);
		//MUEVE EN X 3.5 IZQ  Y = SE MANTIENE
		model1 = glm::translate(model1, glm::vec3(0.0f, 0.0f, -4.0f));
		//model1 = glm::rotate(model1, 180.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model1));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		meshColorList[6]->RenderMeshColor(); //

		//========================================================================================
		// LETRA E
		// primero hace todo lo de y despues se dibuja 
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model2 = glm::mat4(1.0);
		//MUEVE EN X 3.5 IZQ Y=-2.5 ABAJO
		model2 = glm::translate(model2, glm::vec3(3.0f, 0.0f, -4.0f));
		//rota hacia abajo punta trinagulo
		//model2 = glm::rotate(model2, 180.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

		//
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model2));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[7]->RenderMeshColor(); //verde






		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
