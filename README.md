# P1_SIDI_V2

## Descripcion
  Este es un proyecto desarrollado para la asignatura Sistemas Distribuidos (4ºIDCD, U-TAD) Esta práctica consiste en simular una terminal de Linux en la que se pueda crear directorios, cargar ficheros desde la maquina local y aceder a ellos.
  En la version 2.0 se ha incluido un metodo para guardar el arbol y cargarlo al iniciar el programa y un sistema de auto guardado

## Comandos
- ls : Lista los hijos del directorio actual
- pwd : Muestra la ruta actual
- cd <ruta_dir> : Mueve el puntero al directorio actual. Se acepta "cd .." para retroceder directorio padre, "cd /" para volver al directorio root y se puede introducir el nombre de un directorio hijo, o la ruta completa hasta el directorio
- mv <nombre_org> <nombre_dest> : cambia al nombre del fichero o directorio origen 
- cp <ruta_org> <ruta_dest> : copia el nodo del origen al destino, se permite copiar entre directorios, entre ficheros, y de un fichero a un directorio. 
- mkdir <nombre_dir> : Crea un nuevo directorio, se premite pasar solo el nombre o una ruta
- rmdir <nombre_dir> : Elimina un directorio, se premite pasar solo el nombre o una ruta
- rm <nombre_file> :  Elimina un fichero, se permite pasar solo el nombre o la ruta
- lls : Muestra el contenido del directorio local
- lcd <nombre_dir>:  Mueve el puntero al directorio actual en local.  Se acepta "cd .." para retroceder directorio padre, "cd /" para volver al directorio root y se puede introducir el nombre de un directorio hijo, o la ruta completa hasta el directorio
- lpwd : Muestra la ruta al directorio actual en local
- upload : Permite subir un archivo o directorio ya existente en local 

## Version
  V 2.1
