#include <stdio.h>
#include <string.h>
#include <glew.h>
#include <glfw3.h>
#include <Windows.h>

//Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 600;
float r, g, b;
int d;
GLuint VAO, VBO, shader;

//Vertex Shader
//recibir color, salida Vcolor
static const char* vShader = "						\n\
#version 330										\n\
layout (location =0) in vec3 pos;					\n\
void main()											\n\
{													\n\
gl_Position=vec4(pos.x,pos.y,pos.z,1.0f); 			\n\
}";
//recibir Vcolor y dar de salida color
static const char* fShader = "						\n\
#version 330										\n\
out vec4 color;										\n\
void main()											\n\
{													\n\
	color = vec4(0.5f,0.2f,0.5f,1.0f);	 			\n\
}";



void CrearTriangulo()
	{
	GLfloat vertices[] = {
		// Letra "H"
		-0.8f,0.4f,0.0f,
		-0.8f,-0.4f,0.0f,
		-0.65f,-0.4f,0.0f,

		-0.65f,0.4f,0.0f,
		-0.8f,0.4f,0.0f,
		-0.65f,-0.4f,0.0f,

		-0.65f,0.1f,0.0f,
		-0.65f,-0.1f,0.0f,
		-0.45f,-0.1f,0.0f,

		-0.65f,0.1f,0.0f,
		-0.45f,0.1f,0.0f,
		-0.45f,-0.1f,0.0f,

		-0.45f,0.4f,0.0f,
		-0.45f,-0.4f,0.0f,
		-0.3f,-0.4f,0.0f,

		-0.3f,-0.4f,0.0f,
		-0.3f, 0.4f, 0.0f,
		-0.45f,0.4f,0.0f,
		
		// Letra "A"
		0.1f,0.4f,0.0f, // D1
		0.1f, 0.1f, 0.0f, // A1
		-0.2f,-0.4f,0.0f, // E1

		0.1f,0.4f,0.0f, // D1
		0.1f, 0.1f, 0.0f, // A1
		0.4f,-0.4f,0.0f, //F1

		0.1f, 0.1f, 0.0f, // A1
		-0.2f,-0.4f,0.0f, // E1
		0.0f,-0.4f,0.0f, // V

		0.1f, 0.1f, 0.0f, // A1
		0.4f,-0.4f,0.0f, //F1
		0.2f,-0.4f,0.0f, // U

		0.06f,-0.1f,0.0f, // B1
		0.04f, -0.2f, 0.0f, // W
		0.16f,-0.2f,0.0f, // Z

		0.06f,-0.1f,0.0f, // B1
		0.16f,-0.2f,0.0f, // Z
		0.14f,-0.1f,0.0f, // C1

		// Letra P

		0.5f,0.4f,0.0f, // G1
		0.5f,-0.4f,0.0f, // H1
		0.65f,0.3f,0.0f, // R1

		0.5f,-0.4f,0.0f, // H1
		0.65f,0.3f,0.0f, // R1
		0.65f,-0.4f,0.0f, //I1

		0.65f,0.3f,0.0f, // R1
		0.5f,0.4f,0.0f, // G1
		0.78f,0.3f,0.0f, // Q1

		0.5f,0.4f,0.0f, // G1
		0.78f,0.3f,0.0f, // Q1
		0.85f,0.4f,0.0f, // M1

		0.78f,0.3f,0.0f, // Q1
		0.85f,0.4f,0.0f, // M1
		0.85f,0.22f,0.0f, // O1

		0.95f,0.25f,0.0f, // N1
		0.85f,0.4f,0.0f, // M1
		0.85f,0.22f,0.0f, // O1

		0.95f,0.25f,0.0f, // N1
		0.95f,0.1f,0.0f, // L1
		0.85f,0.22f,0.0f, // O1

		0.85f,-0.05f,0.0f, // K1
		0.95f,0.1f,0.0f, // L1
		0.85f,0.22f,0.0f, // O1

		0.85f,-0.05f,0.0f, // K1
		0.85f,0.12f,0.0f, // P1
		0.78f,0.05f,0.0f, // S1

		0.85f,-0.05f,0.0f, // K1
		0.65f,-0.05f,0.0f, // J1
		0.78f,0.05f,0.0f, // S1

		0.65f,0.05f,0.0f, // K1
		0.65f,-0.05f,0.0f, // J1
		0.78f,0.05f,0.0f // S1
	};
	glGenVertexArrays(1, &VAO); //generar 1 VAO
	glBindVertexArray(VAO);//asignar VAO

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //pasarle los datos al VBO asignando tamano, los datos y en este caso es est�tico pues no se modificar�n los valores

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);//Stride en caso de haber datos de color por ejemplo, es saltar cierta cantidad de datos
		glEnableVertexAttribArray(0);
		//agregar valores a v�rtices y luego declarar un nuevo vertexAttribPointer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

}
void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) //Funci�n para agregar los shaders a la tarjeta gr�fica
//the Program recibe los datos de theShader


{
	GLuint theShader = glCreateShader(shaderType);//theShader es un shader que se crea de acuerdo al tipo de shader: vertex o fragment
	const GLchar* theCode[1];
	theCode[0] = shaderCode;//shaderCode es el texto que se le pasa a theCode
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);//longitud del texto
	glShaderSource(theShader,1, theCode, codeLength);//Se le asigna al shader el c�digo
	glCompileShader(theShader);//Se comila el shader
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	//verificaciones y prevenci�n de errores
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al compilar el shader %d es: %s \n",shaderType, eLog);
		return;
	}
	glAttachShader(theProgram, theShader);//Si no hubo problemas se asigna el shader a theProgram el cual asigna el c�digo a la tarjeta gr�fica
}

void CompileShaders() {
	shader= glCreateProgram(); //se crea un programa
	if (!shader)
	{
		printf("Error creando el shader");
		return;
	}
	AddShader(shader, vShader, GL_VERTEX_SHADER);//Agregar vertex shader
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);//Agregar fragment shader
	//Para terminar de linkear el programa y ver que no tengamos errores
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glLinkProgram(shader);//se linkean los shaders a la tarjeta gr�fica
	 //verificaciones y prevenci�n de errores
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al linkear es: %s \n", eLog);
		return;
	}
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al validar es: %s \n", eLog);
		return;
	}



}
int main()
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
	GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Primer ventana", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tama�o de Buffer
	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Fall� inicializaci�n de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Asignar valores de la ventana y coordenadas
	//Asignar Viewport
	glViewport(0, 0, BufferWidth, BufferHeight);

 //Crear tr�angulo
	CrearTriangulo();
	CompileShaders();


	//Loop mientras no se cierra la ventana
	while (!glfwWindowShouldClose(mainWindow))
	{
		//Recibir eventos del usuario
		glfwPollEvents();

		//Limpiar la ventana
		
		r = rand() % 2;
		g = rand() % 2;
		b = rand() % 2;
		
		glClearColor(r,g,b,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(shader);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES,0,69);
		glBindVertexArray(0);

		Sleep(3000);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);

	}


	return 0;
}