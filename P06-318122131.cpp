/*
Pr�ctica 6: Texturizado
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <iostream>
#include <thread>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
#include <chrono>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture dadoTexture;
Texture logofiTexture;
Texture Dado;

Model llanta, coche, cofre;

Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_texture.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_texture.frag";




//c�lculo del promedio de las normales para sombreado de Phong
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

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);


	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

}



void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearDado()
{
	unsigned int cubo_indices[] = {
		// E, J, G, B
		0, 1, 2,
		0, 3, 2,

		// G, J, K, D
		4, 5, 6,
		4, 7, 6,

		// K, J, F, C
		8, 9, 10,
		8, 11, 10,

		// F, J, H, O
		12, 13, 14,
		12, 15, 14,

		// H, J, E, A
		16, 17, 18,
		16, 19, 18,

		// B, E, A, P
		20, 21, 22,
		20, 22, 23,

		// D, G, B, P
		24, 25, 26,
		24, 26, 27,

		// C, K, D, P
		28, 29, 30,
		28, 30, 31,

		// O, F, C, P
		32, 33, 34,
		32, 34, 35,

		// A, H, O, P
		36, 37, 38,
		36, 38, 39
	};

	GLfloat cubo_vertices[] = {

		//x		y		z		S		T			NX		NY		NZ
		0.0f, 0.0f,  3.0f,		0.23f,  0.82f,		-1.0f,	1.0f,	1.0f,	// E (0)
		0.0f,  2.5f,  0.0f,		0.115f,  0.99f,		-1.0f,	1.0f,	1.0f,	// J (2)
		-3.0f, 0.0f,  0.5f,		0.0f,  0.82f,		-1.0f,	1.0f,	1.0f,	// G (1)
		-2.0f, -0.7f, 2.0f,		0.115f,  0.70f,		-1.0f,	1.0f,	1.0f,	// B 																

		//x		y		z		S		T
		-3.0f, 0.0f,  0.5f,	    0.48f,  0.82f,		-1.0f,	1.0f,	-1.0f,	// G 
		0.0f,  2.5f,  0.0f,		0.375f,  0.99f,		-1.0f,	1.0f,	-1.0f,	// J (5)
		-1.5f, 0.0f,  -3.0f,	0.27f,  0.82f,		-1.0f,	1.0f,	-1.0f,	// K (6)
		-3.0f, -0.7f,  -2.0f,	0.375f,  0.70f,		-1.0f,	1.0f,	-1.0f,	// D 

		//x		y		z		S		T
		-1.5f, 0.0f,  -3.0f,	0.72f,  0.82f,		0.0f,	1.0f,	-1.0f,	// K 
		0.0f,  2.5f,  0.0f,		0.625f,  0.99f,		0.0f,	1.0f,	-1.0f,	// J (9)
		1.5f,  0.0f, -3.0f,		0.52f,  0.82f,		0.0f,	1.0f,	-1.0f,	// F (10)
		0.0f, -0.7f,  -4.0f,	0.625f,  0.70f,		0.0f,	1.0f,	-1.0f,	// C 

		//x		y		z		S		T
		1.5f,  0.0f, -3.0f,		0.98f,  0.82f,		1.0f,	1.0f,	-1.0f,	// F 
		0.0f,  2.5f,  0.0f,		0.875f,  0.99f,		1.0f,	1.0f,	-1.0f,	// J (13)
		3.0f,  0.0f,  0.5f,		0.77f,  0.82f,		1.0f,	1.0f,	-1.0f,	// H 
		3.0f, -0.7f,  -2.0f,	0.875f,  0.70f,		1.0f,	1.0f,	-1.0f,	// O 

		//x		y		z		S		T
		3.0f,  0.0f,  0.5f,		0.23f,  0.47f,		1.0f,	1.0f,	1.0f,	// H (16)
		0.0f,  2.5f,  0.0f,		0.115f,  0.62f,		1.0f,	1.0f,	1.0f,	// J (17)
		0.0f, 0.0f,  3.0f,		0.0f,  0.47f,		1.0f,	1.0f,	1.0f,	// E 
		2.0f, -0.7f, 2.0f,		0.115f,  0.35f,		1.0f,	1.0f,	1.0f,	// A 
		 
		//x		y		z		S		T			NX		NY		NZ
		-2.0f, -0.7f, 2.0f,		0.27f,  0.53f,		0.0f,	-1.0f,	1.0f,	// B
		0.0f, 0.0f,  3.0f,		0.375f,  0.62f,		0.0f,	-1.0f,	1.0f,	// E
		2.0f, -0.7f, 2.0f,		0.48f,  0.53f,		0.0f,	-1.0f,	1.0f,	// A
		0.0f,  -2.5f,  0.0f,	0.375f,  0.35f,		0.0f,	-1.0f,	1.0f,	// P

		//x		y		z		S		T
		-3.0f, -0.7f,  -2.0f,	0.52f,  0.55f,		-1.0f,	-1.0f,	0.0f,	// D
		-3.0f, 0.0f,  0.5f,	    0.625f,  0.62f,		-1.0f,	-1.0f,	0.0f,	// G
		-2.0f, -0.7f, 2.0f,		0.72f,  0.55f,		-1.0f,	-1.0f,	0.0f,	// B
		0.0f,  -2.5f,  0.0f,	0.625f,  0.35f,		-1.0f,	-1.0f,	0.0f,	// P

		//x		y		z		S		T
		0.0f, -0.7f,  -4.0f,	0.77f,  0.57f,		0.0f,	0.0f,	0.0f,	// C
		-1.5f, 0.0f,  -3.0f,	0.875f,  0.62f,		0.0f,	0.0f,	0.0f,	// K
		-3.0f, -0.7f,  -2.0f,	0.98f,  0.57f,		0.0f,	0.0f,	0.0f,	// D
		0.0f,  -2.5f,  0.0f,	0.875f,  0.35f,		0.0f,	0.0f,	0.0f,	// P

		//x		y		z		S		T
		3.0f, -0.7f,  -2.0f,	0.0f,  0.22f,		0.0f,	0.0f,	0.0f,	// O
		1.5f,  0.0f, -3.0f,		0.115f,  0.30f,		0.0f,	0.0f,	0.0f,	// F 
		0.0f, -0.7f,  -4.0f,	0.23f,  0.22f,		0.0f,	0.0f,	0.0f,	// C
		0.0f,  -2.5f,  0.0f,	0.115f,  0.0f,		0.0f,	0.0f,	0.0f,	// P

		//x		y		z		S		T
		2.0f, -0.7f, 2.0f,		0.25f,  0.22f,		0.0f,	0.0f,	0.0f,	// A
		3.0f,  0.0f,  0.5f,		0.375f,  0.30f,		0.0f,	0.0f,	0.0f,	// H
		3.0f, -0.7f,  -2.0f,	0.50f,  0.22f,		0.0f,	0.0f,	0.0f,	// O
		0.0f,  -2.5f,  0.0f,	0.375f,  0.0f,		0.0f,	0.0f,	0.0f,	// P
	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 320, 60);
	meshList.push_back(dado);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearDado();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 15.0f, -20.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	dadoTexture = Texture("Textures/dado-de-numeros.png");
	dadoTexture.LoadTextureA();
	logofiTexture = Texture("Textures/escudo_fi_color.tga");
	logofiTexture.LoadTextureA();

	Dado = Texture("Textures/DadoTen.tga");
	Dado.LoadTexture();

	llanta = Model();
	llanta.LoadModel("Models/llanta.obj");
	coche = Model();
	coche.LoadModel("Models/coche.obj");
	cofre = Model();
	cofre.LoadModel("Models/cofre.obj");
	
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	glm::mat4 model(1.0);
	glm::mat4 dado(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	////Loop mientras no se cierra la ventana
	float grad = 0.0f; // Para rotar el dado
	int seg, r1 = 1,r2 = 0,n = 0;
	// Para medir el tiempo

	while (!mainWindow.getShouldClose())
	{

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		grad += 0.3f;

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
		uniformColor = shaderList[0].getColorLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		color = glm::vec3(1.0f, 1.0f, 1.0f);//color blanco, multiplica a la informaci�n de color de la textura

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		meshList[2]->RenderMesh();

		model = modelaux;

		//Dado de Opengl
		//Ejercicio 1: Texturizar su cubo con la imagen dado_animales ya optimizada por ustedes

		//***********************************************************************************
				// DADO 10 Caras
		//***********************************************************************************

		model = glm::translate(model, glm::vec3(-1.5f, 25.0f, -2.0f));
		model = glm::rotate(model, glm::radians(grad), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dado.UseTexture();
		meshList[4]->RenderMesh();
		
		//************************************************************************
			//			COCHE
		//************************************************************************
		
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, mainWindow.getmover()));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		coche.RenderModel();

		//************************************************************************
			//			CAPO
		//************************************************************************

		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.0f, 8.2f, 10.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cofre.RenderModel();

		//************************************************************************
			//			LLANTAS DELANTERAS
		//************************************************************************

		model = modelaux;
		model = glm::translate(model, glm::vec3(5.0f, 2.7f, 13.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llanta.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-8.0f, 2.7f, 13.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llanta.RenderModel();

		//************************************************************************
			//			LLANTAS TRASERAS
		//************************************************************************

		model = modelaux;
		model = glm::translate(model, glm::vec3(5.0f, 2.7f, -9.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llanta.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-8.0f, 2.7f, -9.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llanta.RenderModel();

		//glDisable(GL_BLEND);
		
		//Ejercicio 2:Importar el cubo texturizado en el programa de modelado con 
		//la imagen dado_animales ya optimizada por ustedes
		

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
//
////blending: transparencia o traslucidez
//		glEnable(GL_BLEND);
//		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//		logofiTexture.UseTexture(); //textura con transparencia o traslucidez
//		FIGURA A RENDERIZAR de OpenGL, si es modelo importado no se declara UseTexture
//		glDisable(GL_BLEND);
