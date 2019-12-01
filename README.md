
PROYECTO PRIMER PARCIAL
======================
Version 1.0 > 01/12/2019

1 - Descripción
---------------
* **Parte 1 - Elaborar un nuevo shell:** Programa desarrollado en lenguaje C capaz de reconocer los comandos internos exit y cd, en donde el primero permitirá salir del programa, y el segundo está diseñado para cambiar de directorios. El nuevo shell, invoca a todos los ejecutables y argumentos pasados mediante la linea de comandos. Se mostrará un prompt con el directorio actual y un id de estudiante, el cual será ingresado como argumento del programa.

* **Parte 2 - Una versión mejorada del comando wc (twc):** Programa que genera como salida el número de lineas y palabras existentes en un archivo dado. El programa desarrollado en C usa la librería pthread para maximizar el uso de los núcleos disponibles en el sistema, en cada uno de los núcleos habrá un hilo procesando un bloque del archivo, se utilizan semáforos para sincronizar el trabajo de los hilos en las regiones críticas.

El comando twc recibe los siguientes parámetros:

**-w :** El número de palabras en cada archivo de entrada se escribe en la salida estándar.
**-l :** El número de líneas en cada archivo de entrada se escribe en la salida estándar.


2 - Compilando el proyecto
----------------------------
El proyecto dispone de un archivo Makefile para compilar el código fuente.

* **Instrucciones para compilar:**
```
make shell twc
```
* **Para limpiar:**
```
make clean
```
Se generarán los ejecutables **shell** y **twc** en el directorio **/bin**

3 - Modo de uso general
------------------------

* **Programa Shell**
```
./bin/shell <student_id>

./bin/shell 201504956
```
![Nuevo Shell](https://drive.google.com/file/d/1QvgKzQ4hhCz75k3dv2bpNn1HgYUOmi_I/view?usp=sharing)
* **Comando twc**

```
./bin/twc <-l|-w> <file_name>

./bin/twc -l src/test/file.txt

```

**NOTA:** Se creó un archivo de cabecera llamado csapp.h el cual posee ciertos identificadores usados en ambos programas.

4 - Estrategia
---------------


5 - Autor
-----------

* José David Neira Martrus
=======
# OS-Project1
The Shell and twc command
