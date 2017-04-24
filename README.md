# kernel-programming

Ejemplos de Programación en el Kernel de GNU Linux:

Módulos:

hello_world: Hola mundo de módulos

hello_world2: Hola mundo de módulos + otras cositas básicas

modulo_so: Ejemplo simple de módulo que se comunica con el espacio de usuario a través del /proc

char_dev: Ejemplo de módulo que se comunica con el espacio de usuario a través de IOCTL


En todos los casos para instalar el módulo:

$ make

$ sudo insmod <nombre_modulo>.ko
