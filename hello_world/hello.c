#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
/*
* Módulo simple que imprime en el log un mensaje
*
*
*/

static int hello_so_init(void){
   printk(KERN_INFO "Hola SO2017 \n");
   return 0;
}

static void hello_so_exit(void){
   printk(KERN_INFO "Chau SO2017 \n");
}


//Inicializamos las funciones de de carga y descarga
module_init(hello_so_init);
module_exit(hello_so_exit);
