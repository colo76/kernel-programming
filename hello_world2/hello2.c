#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/moduleparam.h>

/*
* Módulo simple que: 
*  Imprime en el log un mensaje
*  Recibe parámetros desde el boot o desde la carga por parte del usuario 
*  Exporta algunos símbolos
*
*/


/* Indicamos la licencia del módulo */
MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Martín Baez");
MODULE_DESCRIPTION("Ejemplo - SO2017");

/* Configuramos el parametro que recibe desde la carga del módulo */
static char *user_name = "";
module_param(user_name, charp, 0);  
MODULE_PARM_DESC(user_name, "El nombre del usuario");

/*
 * module_param_array(name, type, num, perm);
 * El primer parámetro es el nombre de la variable
 * El segundo parámetro es el tipo de los elementos del array 
 * El tercer argumento es un punteto a una variable que almacena el numero de elementos del arreglo inicializados
 * al momento de la carga
 * El cuarto argumento es el permiso sobre el parametro
 */
int myIntArray[2];
static int arr_argc = 0;
module_param_array(myIntArray, int, &arr_argc, 0000);
MODULE_PARM_DESC(myintArray, "Array de enteros");

/* Función que se invoca cuando se carga el módulo en el kernel */
static int hello_so_init(void){

     
    printk(KERN_INFO "Hola %s \n",user_name);

    printk(KERN_INFO "Cantidad de elementos del arreglo myintArray %u \n",arr_argc);

    int i;
    for (i = 0; i < (sizeof myIntArray / sizeof (int)); i++) {
		printk(KERN_INFO "myIntArray[%d] = %d\n", i, myIntArray[i]);
    }
  
   /* Devolver 0 para indicar una carga correcta del módulo */
   return 0;
}

/* Función que se invoca cuando se descarga el módulo del kernel */
static void hello_so_exit(void){
   printk(KERN_INFO "Chau %s",user_name);
}

//Exportamos la función printName()
char* getUserName()
{
  return user_name;
}
EXPORT_SYMBOL(getUserName);

//Declaramos las funciones de de carga y descarga
module_init(hello_so_init);
module_exit(hello_so_exit);
