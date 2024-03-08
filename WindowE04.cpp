#include "Window.h"
#include <iostream>

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	rotax = 0.0f;
	rotay = 0.0f;
	rotaz = 0.0f;
	articulacion1 = 0.0f;
	articulacion2 = 0.0f;
	articulacion3 = 0.0f;
	articulacion4 = 0.0f;
	articulacion5 = 0.0f;
	articulacion6 = 0.0f;
	articulacion7 = 0.0f;
	articulacion8 = 0.0f;
	articulacion9 = 0.0f;
	articulacion10 = 0.0f;
	articulacion11 = 0.0f;
	articulacion12 = 0.0f;
	articulacion13 = 0.0f;
	articulacion14 = 0.0f;
	
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicializaci�n de GLFW
	if (!glfwInit())
	{
		printf("Fall� inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Practica 04: Modelado Jerarquico", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tama�o de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Fall� inicializaci�n de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se est� usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}

GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_R)
	{
		if (theWindow->articulacion2 == -90 || theWindow->articulacion2 == 60) {
			theWindow->articulacion1 = theWindow->articulacion1;
		}
		else if (theWindow->articulacion1 < 50) {
			theWindow->articulacion1 += 10.0;
		}
	}
	if (key == GLFW_KEY_F)
	{
		if (theWindow->articulacion2 == -90 || theWindow->articulacion2 == 60) {
			theWindow->articulacion1 = theWindow->articulacion1;
		}else if (theWindow->articulacion1 > -70) {
			theWindow->articulacion1 -= 10.0;
		}
	}
	if (key == GLFW_KEY_E)
	{
		if (theWindow->articulacion2 < 60) {
			theWindow->articulacion2 += 10.0;
		}
	}
	if (key == GLFW_KEY_Q)
	{
		if (theWindow->articulacion2 > -90) {
			theWindow->articulacion2 -= 10.0;
		}
	}
	if (key == GLFW_KEY_T)
	{
		if (theWindow->articulacion3 < 40){
			theWindow->articulacion3 += 10.0;
		}
	}

	if (key == GLFW_KEY_G)
	{
		if (theWindow->articulacion3 > 0) {
			theWindow->articulacion3 -= 10.0;
		}
	}

	if (key == GLFW_KEY_Y)
	{
		if (theWindow->articulacion4 < 20) {
			theWindow->articulacion4 += 10.0;
		}
	}

	if (key == GLFW_KEY_H)
	{
		if (theWindow->articulacion4 > -30) {
			theWindow->articulacion4 -= 10.0;
		}
	}

	if (key == GLFW_KEY_U)
	{
		if (theWindow->articulacion5 < 20) {
			theWindow->articulacion5 += 10.0;
		}
	}

	if (key == GLFW_KEY_J)
	{
		if (theWindow->articulacion5 > -30) {
			theWindow->articulacion5 -= 10.0;
		}
	}

	if (key == GLFW_KEY_I)
	{
		if (theWindow->articulacion6 < 30) {
			theWindow->articulacion6 += 10.0;
		}
	}

	if (key == GLFW_KEY_K)
	{
		if (theWindow->articulacion6 > -20) {
			theWindow->articulacion6 -= 10.0;
		}
	}

	if (key == GLFW_KEY_O)
	{
		if (theWindow->articulacion7 < 30) {
			theWindow->articulacion7 += 10.0;
		}
	}

	if (key == GLFW_KEY_L)
	{
		if (theWindow->articulacion7 > -20) {
			theWindow->articulacion7 -= 10.0;
		}
	}

	if (key == GLFW_KEY_1)
	{
		if (theWindow->articulacion8 < 30) {
			theWindow->articulacion8 += 10.0;
		}
	}

	if (key == GLFW_KEY_2)
	{
		if (theWindow->articulacion8 > -20) {
			theWindow->articulacion8 -= 10.0;
		}
	}

	if (key == GLFW_KEY_3)
	{
		if (theWindow->articulacion9 < 10) {
			theWindow->articulacion9 += 10.0;
		}
	}

	if (key == GLFW_KEY_4)
	{
		if (theWindow->articulacion9 > -20) {
			theWindow->articulacion9 -= 10.0;
		}
	}
	if (key == GLFW_KEY_5)
	{
		if (theWindow->articulacion10 < 30) {
			theWindow->articulacion10 += 10.0;
		}
	}

	if (key == GLFW_KEY_6)
	{
		if (theWindow->articulacion10 > -20) {
			theWindow->articulacion10 -= 10.0;
		}
	}
	if (key == GLFW_KEY_7)
	{
		if (theWindow->articulacion11 < 30) {
			theWindow->articulacion11 += 10.0;
		}
	}

	if (key == GLFW_KEY_8)
	{
		if (theWindow->articulacion11 > -20) {
			theWindow->articulacion11 -= 10.0;
		}
	}

	if (key == GLFW_KEY_9)
	{
		if (theWindow->articulacion12 < 30) {
			theWindow->articulacion12 += 10.0;
		}
	}

	if (key == GLFW_KEY_0)
	{
		if (theWindow->articulacion12 > -20) {
			theWindow->articulacion12 -= 10.0;
		}
	}


	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_D, 0);
		//printf("se presiono la tecla: %s\n",key_name);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
