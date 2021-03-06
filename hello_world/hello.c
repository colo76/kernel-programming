#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
/*
* Módulo simple que: 
* Imprime en el log un mensaje
*
*
*/


/* Indicamos la licencia del módulo */
MODULE_LICENSE("GPL"); 

/* Función que se invoca cuando se carga el módulo en el kernel */
static int hello_so_init(void){
   printk(KERN_INFO "Hola SO2017 \n");
   /* Devolver 0 para indicar una carga correcta del módulo */
   return 0;
}

/* Función que se invoca cuando se descarga el módulo del kernel */
static void hello_so_exit(void){
   printk(KERN_INFO "Chau SO2017 \n");
}


//Declaramos las funciones de de carga y descarga
module_init(hello_so_init);
module_exit(hello_so_exit);
