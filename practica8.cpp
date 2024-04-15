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
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
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
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture Dado;

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;


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
SpotLight spotLights2[MAX_SPOT_LIGHTS];

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

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

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
		0.0f, 0.0f,  3.0f,		0.23f,  0.82f,		1.0f,	0.0f,	0.0f,	// E (0)
		0.0f,  2.5f,  0.0f,		0.115f,  0.99f,		1.0f,	0.0f,	0.0f,	// J (2)
		-3.0f, 0.0f,  0.5f,		0.0f,  0.82f,		1.0f,	0.0f,	0.0f,	// G (1)
		-2.0f, -0.7f, 2.0f,		0.115f,  0.70f,		1.0f,	0.0f,	0.0f,	// B 																

		//x		y		z		S		T
		-3.0f, 0.0f,  0.5f,	    0.48f,  0.82f,		1.0f,	0.0f,	0.0f,	// G 
		0.0f,  2.5f,  0.0f,		0.375f,  0.99f,		1.0f,	0.0f,	0.0f,	// J (5)
		-1.5f, 0.0f,  -3.0f,	0.27f,  0.82f,		1.0f,	0.0f,	0.0f,	// K (6)
		-3.0f, -0.7f,  -2.0f,	0.375f,  0.70f,		1.0f,	0.0f,	0.0f,	// D 

		//x		y		z		S		T
		-1.5f, 0.0f,  -3.0f,	0.72f,  0.82f,		1.0f,	0.0f,	0.0f,	// K 
		0.0f,  2.5f,  0.0f,		0.625f,  0.99f,		1.0f,	0.0f,	0.0f,	// J (9)
		1.5f,  0.0f, -3.0f,		0.52f,  0.82f,		1.0f,	0.0f,	0.0f,	// F (10)
		0.0f, -0.7f,  -4.0f,	0.625f,  0.70f,		1.0f,	0.0f,	0.0f,	// C 

		//x		y		z		S		T
		1.5f,  0.0f, -3.0f,		0.98f,  0.82f,		-1.0f,	0.0f,	0.0f,	// F 
		0.0f,  2.5f,  0.0f,		0.875f,  0.99f,		-1.0f,	0.0f,	0.0f,	// J (13)
		3.0f,  0.0f,  0.5f,		0.77f,  0.82f,		-1.0f,	0.0f,	0.0f,	// H 
		3.0f, -0.7f,  -2.0f,	0.875f,  0.70f,		-1.0f,	0.0f,	0.0f,	// O 

		//x		y		z		S		T
		3.0f,  0.0f,  0.5f,		0.23f,  0.47f,		-1.0f,	0.0f,	0.0f,	// H (16)
		0.0f,  2.5f,  0.0f,		0.115f,  0.62f,		-1.0f,	0.0f,	0.0f,	// J (17)
		0.0f, 0.0f,  3.0f,		0.0f,  0.47f,		-1.0f,	0.0f,	0.0f,	// E 
		2.0f, -0.7f, 2.0f,		0.115f,  0.35f,		-1.0f,	0.0f,	0.0f,	// A 

		//x		y		z		S		T			NX		NY		NZ
		-2.0f, -0.7f, 2.0f,		0.27f,  0.53f,		1.0f,	0.0f,	0.0f,	// B
		0.0f, 0.0f,  3.0f,		0.375f,  0.62f,		1.0f,	0.0f,	0.0f,	// E
		2.0f, -0.7f, 2.0f,		0.48f,  0.53f,		1.0f,	0.0f,	0.0f,	// A
		0.0f,  -2.5f,  0.0f,	0.375f,  0.35f,		1.0f,	0.0f,	0.0f,	// P

		//x		y		z		S		T
		-3.0f, -0.7f,  -2.0f,	0.52f,  0.55f,		1.0f,	0.0f,	0.0f,	// D
		-3.0f, 0.0f,  0.5f,	    0.625f,  0.62f,		1.0f,	0.0f,	0.0f,	// G
		-2.0f, -0.7f, 2.0f,		0.72f,  0.55f,		1.0f,	0.0f,	0.0f,	// B
		0.0f,  -2.5f,  0.0f,	0.625f,  0.35f,		1.0f,	0.0f,	0.0f,	// P

		//x		y		z		S		T
		0.0f, -0.7f,  -4.0f,	0.77f,  0.57f,		1.0f,	0.0f,	0.0f,	// C
		-1.5f, 0.0f,  -3.0f,	0.875f,  0.62f,		1.0f,	0.0f,	0.0f,	// K
		-3.0f, -0.7f,  -2.0f,	0.98f,  0.57f,		1.0f,	0.0f,	0.0f,	// D
		0.0f,  -2.5f,  0.0f,	0.875f,  0.35f,		1.0f,	0.0f,	0.0f,	// P

		//x		y		z		S		T
		3.0f, -0.7f,  -2.0f,	0.0f,  0.22f,		1.0f,	0.0f,	0.0f,	// O
		1.5f,  0.0f, -3.0f,		0.115f,  0.30f,		1.0f,	0.0f,	0.0f,	// F 
		0.0f, -0.7f,  -4.0f,	0.23f,  0.22f,		1.0f,	0.0f,	0.0f,	// C
		0.0f,  -2.5f,  0.0f,	0.115f,  0.0f,		1.0f,	0.0f,	0.0f,	// P

		//x		y		z		S		T
		2.0f, -0.7f, 2.0f,		0.25f,  0.22f,		-1.0f,	0.0f,	0.0f,	// A
		3.0f,  0.0f,  0.5f,		0.375f,  0.30f,		-1.0f,	0.0f,	0.0f,	// H
		3.0f, -0.7f,  -2.0f,	0.50f,  0.22f,		-1.0f,	0.0f,	0.0f,	// O
		0.0f,  -2.5f,  0.0f,	0.375f,  0.0f,		-1.0f,	0.0f,	0.0f	// P
	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 320, 60);
	meshList.push_back(dado);
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	CrearDado();

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

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	Dado = Texture("Textures/DadoTen.tga");
	Dado.LoadTexture();

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.4, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		1.0f, 1.0f,
		-6.0f, 1.5f, 1.5f,
		0.0f, 0.0f, 0.99f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		12.0f, 10.0f, 1.0f,
		0.0f, -5.0f, 0.0f,
		0.3f, 0.05f, 0.05f,
		50.0f);
	spotLightCount++;

	/// SEGUNDO ARREGLO
	spotLights2[0] = spotLights[1];
	spotLights2[1] = spotLights[0];
	
	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana
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
			glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		spotLights[1].SetPos(glm::vec3(12.0f,10.0f+mainWindow.getmuevex(), 1.0f));

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		//if (mainWindow.getArregloLuz() != 1.0f) {
		//	if (mainWindow.getencenderLuz() == 1.0f) {
		//		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		//	}
		//	else {
		//		shaderList[0].SetSpotLights(spotLights, spotLightCount - 1);
		//	}
		//}
		//else {

		//	// luz ligada a la cámara de tipo flash
		////sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		//	glm::vec3 lowerLight = camera.getCameraPosition();
		//	lowerLight.y -= 0.3f;
		//	spotLights2[1].SetFlash(lowerLight, camera.getCameraDirection());

		//	if (mainWindow.getencenderLuz() == 1.0f) {
		//		shaderList[0].SetSpotLights(spotLights2, spotLightCount);

		//	}
		//	else {
		//		shaderList[0].SetSpotLights(spotLights2, spotLightCount - 1);
		//	}
		//}

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		
		//***********************************************************************************
				// DADO 10 Caras
		//***********************************************************************************

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 5.0f, -2.0f));
		//model = glm::rotate(model, glm::radians(grad), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dado.UseTexture();
		meshList[4]->RenderMesh();

		
		//blending: transparencia o traslucidez
		/*glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);*/

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
