/*
Práctica 7: Iluminación 1 
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh_tn.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
//#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<MeshModel*> meshListModel; // recibe xyz uv nx ny nz
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
// Añadir Texturas del dado y del holocron
Texture dadoTexture;
Texture holocronTexture;

//Texture dado;////// anadir texturas del dado

Model Dado_M;
Model Holocron_M;

//Rover = Model();
//Rover_Mod.LoadModel("Models/cubo_starsw.obj");

Model Blackhawk_M;
Model Rover_M; //Sustituir por sus propios Rovers Con jerarquía
//Añadir modelo del avión.
//Model Avion_M;




Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};
	//agavee
	GLfloat vegetacionVertices[] = {
															//nz
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		1.0f, 0.0f, -1.0f, //nz esta cero por eso no tiene iluminacion ni colro
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		1.0f, 0.0f, -1.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		1.0f, 0.0f, -1.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		1.0f, 0.0f, -1.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		1.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		1.0f, 0.0f, 0.0f,


	};
	
	MeshModel *obj1 = new MeshModel();
	obj1->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj1);

	MeshModel *obj2 = new MeshModel();
	obj2->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj2);

	MeshModel *obj3 = new MeshModel();
	obj3->CreateMeshModel(floorVertices, floorIndices, 32, 6);
	meshListModel.push_back(obj3);

	MeshModel *obj4 = new MeshModel();
	obj4->CreateMeshModel(vegetacionVertices, vegetacionIndices, 64, 12);
	meshListModel.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


void CrearDado() //sustituir con su propio dado, para que tenga la textura de la práctica 6
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,

		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};	
	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.68f,  0.4f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.84f,	0.4f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.84f,	0.59f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.68f,	0.59f,		0.0f,	0.0f,	-1.0f,	//3

		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.34f,  0.59f,		-1.0f,	0.0f,	0.0f,// abizq
		0.5f, -0.5f,  -0.5f,	0.54f,	0.59f,		-1.0f,	0.0f,	0.0f,// abder
		0.5f,  0.5f,  -0.5f,	0.54f,	0.79f,		-1.0f,	0.0f,	0.0f,// arrder
		0.5f,  0.5f,  0.5f,	    0.34f,	0.79f,		-1.0f,	0.0f,	0.0f,// arrizq
		// back black sun
		-0.5f, -0.5f, -0.5f,	1.0f,  0.8f,		0.0f,	0.0f,	1.0f,//abder
		0.5f, -0.5f, -0.5f,		0.85f,	0.8f,		0.0f,	0.0f,	1.0f,//abizq
		0.5f,  0.5f, -0.5f,		0.85f,	0.97f,		0.0f,	0.0f,	1.0f,//arrizq
		-0.5f,  0.5f, -0.5f,	1.0f,	0.97f,		0.0f,	0.0f,	1.0f,//arrder

		// left //raziel clan
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.8f,  0.2f,		1.0f,	0.0f,	0.0f,//abizq
		-0.5f, -0.5f,  0.5f,	1.0f,	0.2f,		1.0f,	0.0f,	0.0f,//abder
		-0.5f,  0.5f,  0.5f,	1.0f,	0.4f,		1.0f,	0.0f,	0.0f,//arrder
		-0.5f,  0.5f,  -0.5f,	0.8f,	0.4f,		1.0f,	0.0f,	0.0f,//arrizq

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.16f,  0.6f,		0.0f,	1.0f,	0.0f,//abiz
		0.5f,  -0.5f,  0.5f,	0.16f,	0.4f,		0.0f,	1.0f,	0.0f,//abder
		 0.5f,  -0.5f,  -0.5f,	0.0f,	0.4f,		0.0f,	1.0f,	0.0f,//arrder
		-0.5f, -0.5f,  -0.5f,	0.0f,	0.6f,		0.0f,	1.0f,	0.0f,//arrizq

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.48f,  0.19f,		0.0f,	-1.0f,	0.0f,//arrizq
		 0.5f,  0.5f,  0.5f,	0.48f,	0.4f,		0.0f,	-1.0f,	0.0f,//arrder
		  0.5f, 0.5f,  -0.5f,	0.33f,	0.4f,		0.0f,	-1.0f,	0.0f,//abder
		 -0.5f, 0.5f,  -0.5f,	0.33f,	0.19f,		0.0f,	-1.0f,	0.0f,//abizq

	};

	MeshModel* dado = new MeshModel();
	dado->CreateMeshModel(cubo_vertices, cubo_indices, 192, 36);
	meshListModel.push_back(dado);

}


void CrearHolocron()
{

	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
	// Vértices y normales extraídos directamente de holocron_simple.obj
// Formato por fila: X, Y, Z,  S, T,  NX, NY, NZ

	GLfloat holocron_vertices[] = {
		// front First Order
		//x      y       z       S       T       NX    NY    NZ
		 0.0f,  3.0f,  3.0f,   0.75f,  0.59f,  0.0f, 0.0f, -1.0f,  //0  a
		 3.0f,  0.0f,  3.0f,   0.84f,  0.50f,  0.0f, 0.0f, -1.0f,  //1  b
		 0.0f, -3.0f,  3.0f,   0.75f,  0.41f,  0.0f, 0.0f, -1.0f,  //2  c
		-3.0f,  0.0f,  3.0f,   0.66f,  0.50f,  0.0f, 0.0f, -1.0f,  //3  d

		//der Galactic Republic
		//x      y       z       S       T       NX    NY    NZ
		 3.0f,  3.0f,  0.0f,   0.44f,  0.81f, - 1.0f, 0.0f, 0.0f,  //4  e
		 3.0f,  0.0f, -3.0f,   0.53f,  0.72f,  -1.0f, 0.0f, 0.0f,  //5  f
		 3.0f, -3.0f,  0.0f,   0.44f,  0.63f,  -1.0f, 0.0f, 0.0f,  //6  g
		 3.0f,  0.0f,  3.0f,   0.35f,  0.72f,  -1.0f, 0.0f, 0.0f,  //7  b

		 //izq Raziel Clan
		 //x      y       z       S       T       NX    NY    NZ
		 -3.0f,  3.0f,  0.0f,   0.89f,  0.38f, 1.0f, 0.0f, 0.0f,  //8  h
		 -3.0f,  0.0f,  3.0f,   0.98f,  0.29f, 1.0f, 0.0f, 0.0f,  //9  d
		 -3.0f, -3.0f,  0.0f,   0.89f,  0.20f, 1.0f, 0.0f, 0.0f,  //10 i
		 -3.0f,  0.0f, -3.0f,   0.80f,  0.29f, 1.0f, 0.0f, 0.0f,  //11 j

		 //inf SITH Empire
		 //x      y       z       S       T       NX    NY    NZ
		  0.0f, -3.0f,  3.0f,   0.09f,  0.60f,  0.0f,1.0f, 0.0f,  //12 c
		  3.0f, -3.0f,  0.0f,   0.18f,  0.51f,  0.0f,1.0f, 0.0f,  //13 g
		  0.0f, -3.0f, -3.0f,   0.09f,  0.42f,  0.0f,1.0f, 0.0f,  //14 l
		 -3.0f, -3.0f,  0.0f,   0.00f,  0.51f,  0.0f,1.0f, 0.0f,  //15 i

		 //sup Dark Lord
		 //x      y       z       S       T       NX    NY    NZ
		  0.0f,  3.0f, -3.0f,   0.41f,  0.41f,  0.0f, -1.0f, 0.0f,  //16 k
		  3.0f,  3.0f,  0.0f,   0.50f,  0.32f,  0.0f, -1.0f, 0.0f,  //17 e
		  0.0f,  3.0f,  3.0f,   0.41f,  0.23f,  0.0f, -1.0f, 0.0f,  //18 a
		 -3.0f,  3.0f,  0.0f,   0.32f,  0.32f,  0.0f, -1.0f, 0.0f,  //19 h

		 //atras Black Sun
		 //x      y       z       S       T       NX    NY    NZ
		  0.0f,  3.0f, -3.0f,   0.905f, 0.995f, 0.0f, 0.0f,1.0f,  //20 k
		  3.0f,  0.0f, -3.0f,   0.815f, 0.905f, 0.0f, 0.0f,1.0f,  //21 f  
		  0.0f, -3.0f, -3.0f,   0.905f, 0.815f, 0.0f, 0.0f,1.0f,  //22 l
		 -3.0f,  0.0f, -3.0f,   0.995f, 0.905f, 0.0f, 0.0f,1.0f,  //23 j  

		 //triang supizq
		 //x      y       z       S       T       NX    NY    NZ
		  0.0f,  3.0f,  3.0f,   0.02f,  0.02f, 0.6f, -0.6f, 1.0f,  //24 a
		 -3.0f,  0.0f,  3.0f,   0.02f,  0.02f, 0.6f, -0.6f, 1.0f,  //25 d
		 -3.0f,  3.0f,  0.0f,   0.02f,  0.02f, 0.6f, -0.6f, 1.0f,  //26 h  

		 //triang sup der
		 //x      y       z       S       T       NX    NY    NZ
		 0.0f,  3.0f,  3.0f,   0.02f,  0.02f,  -0.6f, 0.6f, 1.0f,  //27 a
		 3.0f,  3.0f,  0.0f,   0.02f,  0.02f,  -0.6f, 0.6f, 1.0f,  //28 e
		 3.0f,  0.0f,  3.0f,   0.02f,  0.02f,  -0.6f, 0.6f, 1.0f,  //29 b

		  //triang inf izq
		  //x      y       z       S       T       NX    NY    NZ
		  0.0f, -3.0f,  3.0f,   0.02f,  0.02f, 0.6f,0.6f, 1.0f,  //30 c
		  -3.0f, -3.0f,  0.0f,   0.02f,  0.02f, 0.6f,0.6f, 1.0f,  //31 i
		  -3.0f,  0.0f,  3.0f,   0.02f,  0.02f, 0.6f,0.6f, 1.0f,  //32 d

		  //triang inf der
		  //x      y       z       S       T       NX    NY    NZ
		  0.0f, -3.0f,  3.0f,   0.02f,  0.02f,  -0.6f,0.6f, 1.0f,  //33 c
		  3.0f, -3.0f,  0.0f,   0.02f,  0.02f, -0.6f,0.6f, 1.0f,  //34 g
		  3.0f,  0.0f,  3.0f,   0.02f,  0.02f,  -0.6f,0.6f, 1.0f,  //35 b

		   //triang supizq2
		   //x      y       z       S       T       NX    NY    NZ
		  0.0f,  3.0f, -3.0f,   0.02f,  0.02f, 0.6f, -0.6f,-1.0f,  //36 k
		  -3.0f,  3.0f,  0.0f,   0.02f,  0.02f,0.6f, -0.6f,-1.0f,  //37 h
		  -3.0f,  0.0f, -3.0f,   0.02f,  0.02f, 0.6f, -0.6f,-1.0f,  //38 j

		   //triang sup detr2
		   //x      y       z       S       T       NX    NY    NZ
		  0.0f,  3.0f, -3.0f,   0.02f,  0.02f,  -0.6f, -0.6f,-1.0f,  //39 k
		  3.0f,  3.0f,  0.0f,   0.02f,  0.02f, -0.6f, -0.6f,-1.0f,  //40 e
		  3.0f,  0.0f, -3.0f,   0.02f,  0.02f, -0.6f, -0.6f,-1.0f,  //41 f

			//triang inf izq2
			//x      y       z       S       T       NX    NY    NZ
		  0.0f, -3.0f, -3.0f,   0.02f,  0.02f, 0.6f,0.6f,-1.0f,  //42 l
		  -3.0f, -3.0f,  0.0f,   0.02f,  0.02f, 0.6f,0.6f,-1.0f,  //43 i
		  -3.0f,  0.0f, -3.0f,   0.02f,  0.02f, 0.6f,0.6f,-1.0f,  //44 j

			//triang inf der2
			//x      y       z       S       T       NX    NY    NZ
		  0.0f, -3.0f, -3.0f,   0.02f,  0.02f,  -0.6f,0.6f,-1.0f,  //45 l
		  3.0f, -3.0f,  0.0f,   0.02f,  0.02f,  -0.6f,0.6f,-1.0f,  //46 g
		  3.0f,  0.0f, -3.0f,   0.02f,  0.02f,  -0.6f,0.6f,-1.0f,  //47 f
	};

	GLuint holocron_indices[] = {
		// front
		0,  1,  2,
		2,  3,  0,

		// der
		4,  5,  6,
		6,  7,  4,

		// izq
		8,  9, 10,
		10, 11,  8,

		// inf
		12, 13, 14,
		14, 15, 12,

		// sup
		16, 17, 18,
		18, 19, 16,

		// atras
		20, 21, 22,
		22, 23, 20,

		// triangulos
		24, 25, 26,
		27, 28, 29,
		30, 31, 32,
		33, 34, 35,
		36, 37, 38,
		39, 40, 41,
		42, 43, 44,
		45, 46, 47,
	};

	MeshModel* holocron = new MeshModel();

	holocron->CreateMeshModel(holocron_vertices, holocron_indices, 384, 60);
	meshListModel.push_back(holocron);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();
	CreateObjects();
	CrearDado();
	CrearHolocron();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();

	dadoTexture = Texture("Textures/star wars logos edit.png");
	dadoTexture.LoadTextureA();


	holocronTexture = Texture("Textures/star wars logos edit.png");
	holocronTexture.LoadTextureA();


	Holocron_M = Model();
	Holocron_M.LoadModel("Models/holocron_simple.obj");

	Dado_M = Model();
	Dado_M.LoadModel("Models/cubo_starsw.obj");



	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");

	Rover_M = Model();
	Rover_M.LoadModel("Models/rovermodificado.obj");

	//Añadir modelo del avión
	//Avion_M = Model();
	//Avion_M.LoadModel("Models/");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	//lu direccional es nuestro sol
	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,//color blanca
		0.2f, 0.3f,//0.3 es la cantidad de iluminacion, mayor numero mayr iluminacion
		0.0f, 0.0f, -1.0f);///ilumina en z negativo, cambiar x,y,z
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,//luz roja r=1
		0.0f, 1.0f,// raiacion   color  
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);//jugar con estos valores
	pointLightCount++;


	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,//blanca
		0.0f, 6.0f,//
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);//ancho de la luz
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,//color verde luz
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	//se crean mas luces puntuales y spotlight 
	//rover
	//
	spotLights[2] = SpotLight(
		1.0f, 1.0f, 0.8f,  // color  
		0.0f,               // sin ambiente
		6.0f,               // intensidad difusa
		1.0f, -1.0f, 6.0f, // pos
		0.0f, 0.0f, -1.0f, // dir
		1.0f, 0.0f, 0.0f, // atenu
		20.0f               //
	);
	spotLightCount++;



	//avion ejex
	spotLights[3] = SpotLight(
		0.0f, 0.0f, 1.0f,  // color  
		0.0f,               // sin ambiente
		8.0f,               // intensidad difusa
		0.0f, 5.0f, 6.0f, // posición inicial 
		0.0f, 0.0f, -1.0f, // dirección inicial 
		1.0f, 0.0f, 0.0f, // atenua 
		20.0f               // 
	);
	spotLightCount++; // 4

	//avion ejey
	spotLights[4] = SpotLight(
		0.5f, 0.0f, 0.8f,  // color  
		0.0f,               // sin ambiente
		8.0f,               // intensidad difusa
		0.0f, 5.0f, 6.0f, // pos
		0.0f, 0.0f, -1.0f, // dir
		1.0f, 0.0f, 0.0f, // atenuac
		20.0f               // 
	);
	spotLightCount++; // 4



	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana
	//crear  var lowerlight
	glm::vec3 lowerLight;
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color;


	// 
	glm::vec3 roverPosicionA = glm::vec3(1.0f, -1.0f, 6.0f); // posición inicial del rover
	float roverVel = 0.05f; // velocidad de movimiento

	glm::vec3 avionPosicionA = glm::vec3(0.0f, 5.0f, 6.0f); // posición inicial del avion
	float avionVel = 0.05f; // velocidad de movimiento

	// posiscion y direccion
	glm::vec3 faroRoverOffset = glm::vec3(0.5f, 0.6f, 0.0f);  // frente del rover 
	glm::vec3 faroRoverDir = glm::vec3(1.0f, 0.0f, 0.0f);     // hacia adelante

	glm::vec3 faroAvionOffset = glm::vec3(-1.4f, 0.0f, 0.0f);   // frente del avion
	glm::vec3 faroAvionDir = glm::vec3(-1.0f, 0.0f, 0.0f);      // hacia adelante
	//
	glm::vec3 faroAvionAbajoOffset = glm::vec3(0.0f, -0.2f, 0.0f); // abajo del avion
	glm::vec3 faroAvionAbajoDir = glm::vec3(0.0f, -1.0f, 0.0f);    // hacia abajo



	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());


		// Se actualiza cada frame con la tecla
		roverPosicionA.x = mainWindow.getRoverX();
		//ara avion
		avionPosicionA.x = mainWindow.getAvionX();
		avionPosicionA.y = mainWindow.getAvionY();

		//
		glm::mat4 roverBase = glm::translate(glm::mat4(1.0f), roverPosicionA);
		glm::mat4 avionBase = glm::translate(glm::mat4(1.0f), avionPosicionA);


		// 
		// Posición del faro  posición del 
		glm::vec3 faroPosRov = glm::vec3(roverBase * glm::vec4(faroRoverOffset, 1.0f));

		// Dirección del faro hacia adelante X
		glm::vec3 faroDirRov = glm::normalize(glm::mat3(roverBase) * faroRoverDir);

		// Actualizar el spotlight del faro cada frame
		spotLights[2].SetFlash(faroPosRov, faroDirRov);

		//

		// 
		// Posición del faro  posición del avion 
		glm::vec3 faroPosA = glm::vec3(avionBase * glm::vec4(faroAvionOffset, 1.0f));

		// Dirección del faro hacia adelante X
		glm::vec3 faroDirA = glm::normalize(glm::mat3(avionBase) * faroAvionDir);

		// Actualizar el spotlight del faro cada frame
		spotLights[3].SetFlash(faroPosA, faroDirA);

		// 
		// Posición del faro de abajo del avion
		glm::vec3 faroPosA2 = glm::vec3(avionBase * glm::vec4(faroAvionAbajoOffset, 1.0f));

		// Dirección del faro hacia abajo Y
		glm::vec3 faroDirA2 = glm::normalize(glm::mat3(avionBase) * faroAvionAbajoDir);

		// Actualizar el spotlight del faro cada frame
		spotLights[4].SetFlash(faroPosA2, faroDirA2);


		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);



		color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshListModel[2]->RenderMeshModel();




		//Rover: Sustituir por su propio Rover con jerarquía
		//rover importado
		model = roverBase;
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rover_M.RenderModel();


		///================================================
		////	
		//avion importado
		//BLACKHAWK
		model = avionBase;
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();


		// DEBUG: cubito en cada faro (borrar al terminar)
		model = glm::mat4(1.0);
		model = glm::translate(model, faroPosA);
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		meshListModel[4]->RenderMeshModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, faroPosA2);
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		meshListModel[4]->RenderMeshModel();




		//Dado texturizado por codigo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, 5.0f, 6.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		meshListModel[4]->RenderMeshModel();

		//Holocron
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-7.0f, 7.0f, 6.0));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		holocronTexture.UseTexture();
		meshListModel[5]->RenderMeshModel();



		//
		//
		//
		//
		// 
		// 
		// 
		// 
		// 
		//Añadir modelo del avión



		//Agave ¿qué sucede si lo renderizan antes del helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshListModel[3]->RenderMeshModel();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}


