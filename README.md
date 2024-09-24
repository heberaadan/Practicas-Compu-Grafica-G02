---

## Descripción del Proyecto

### `P01-318122131.cpp`

Este proyecto es parte del curso de **Computación Gráfica e Interacción Humano-Computadora** en la Facultad de Ingeniería de la Universidad Nacional Autónoma de México. El objetivo es ilustrar el uso de OpenGL para la renderización de gráficos 2D básicos mediante la creación de triángulos que forman letras.

### Funcionamiento del Programa

- **Inicialización de GLFW y GLEW**: El programa comienza inicializando GLFW, una biblioteca para crear ventanas y manejar eventos, y GLEW, una biblioteca para manejar las extensiones de OpenGL.
  
- **Configuración de la Ventana**: Se crea una ventana de 800x600 píxeles con OpenGL 4.3 y se configuran las propiedades del contexto.

- **Shaders**: El programa utiliza un Vertex Shader y un Fragment Shader. El Vertex Shader se encarga de posicionar los vértices en la pantalla, mientras que el Fragment Shader define el color de los fragmentos.

- **Datos de Vértices**: Se definen las coordenadas de los vértices para formar letras específicas ("H", "A", y "P") y se cargan en un Vertex Buffer Object (VBO).

- **Renderizado**: En el bucle principal, el programa limpia la pantalla, aplica un color de fondo aleatorio, y dibuja los triángulos en la ventana. La pantalla se actualiza cada 3 segundos.

### Ejecución

1. **Requisitos**: Asegúrate de tener instaladas las bibliotecas GLFW, GLEW y OpenGL. Además, este código está diseñado para ser ejecutado en un entorno Windows.

2. **Compilación**:
   - Usa un compilador compatible con C++11 o superior.
   - Asegúrate de que las bibliotecas GLFW y GLEW estén correctamente vinculadas.
   - Ejemplo de comando de compilación con `g++` (ajustar las rutas según sea necesario):
     ```sh
     g++ -o P01-318122131 P01-318122131.cpp -lglfw -lglew32 -lopengl32
     ```

3. **Ejecución**:
   - Ejecuta el archivo compilado:
     ```sh
     ./P01-318122131
     ```

4. **Notas**:
   - Asegúrate de tener los controladores de gráficos actualizados para evitar problemas con OpenGL.

---
