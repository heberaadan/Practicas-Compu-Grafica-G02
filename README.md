### P03-318122131

**Descripción:** Este código genera una pirámide Rubik (pyraminx) en un entorno 3D, compuesta por 9 pirámides en cada cara. Cada cara de la pirámide tiene un color distinto y las separaciones entre las pirámides pequeñas se visualizan con líneas oscuras, lo que permite distinguir cada instancia claramente.

**Ejecución:** Para ejecutar este código, asegúrate de tener las bibliotecas necesarias de OpenGL y GLM instaladas. Compila el archivo fuente utilizando un compilador compatible con C++ y OpenGL. Por ejemplo, con `g++` puedes usar el siguiente comando:

```bash
g++ -o pyraminx P03-318122131.cpp -lGL -lGLU -lglfw -lGLEW
```

Luego, ejecuta el programa resultante con:

```bash
./pyraminx
```

---

### E03-318122131

**Descripción:** Este código instancia varias primitivas geométricas en un entorno 3D para recrear un dibujo de una práctica anterior. El entorno incluye un piso, una casa con una ventana azul circular en la pared trasera, dos ventanas verdes en cada pared lateral (idénticas a las de la pared frontal), y una puerta en la pared frontal.

**Ejecución:** Asegúrate de tener las bibliotecas necesarias de OpenGL y GLM instaladas. Compila el archivo fuente utilizando un compilador compatible con C++ y OpenGL. Por ejemplo, con `g++` puedes usar el siguiente comando:

```bash
g++ -o house E03-318122131.cpp -lGL -lGLU -lglfw -lGLEW
```

Luego, ejecuta el programa resultante con:

```bash
./house
```
