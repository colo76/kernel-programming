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

/* Función que se invoca cuando se carga el módulo en el kernel */
static int hello_so_init(void){


    printk(KERN_INFO "Hola %s \n",user_name);
  
   /* Devolver 0 para indicar una carga correcta del módulo */
   return 0;
}

/* Función que se invoca cuando se descarga el módulo del kernel */
static void hello_so_exit(void){
   printk(KERN_INFO "Chau %s",user_name);
}


//Declaramos las funciones de de carga y descarga
module_init(hello_so_init);
module_exit(hello_so_exit);
