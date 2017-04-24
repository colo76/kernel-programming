/*
* Modulo que transfiere datos desde y hacia el espacio de usuario
* a través de una entrada del /proc
*
*/

#include <linux/module.h> /* Requerido por todos los modulos */ 
#include <linux/kernel.h> /* Definición de KERN_INFO */
#include <linux/proc_fs.h>  /* Para acceder al /proc */
#include <linux/vmalloc.h>  /* Para manejo de memoria */
#include <linux/string.h>   
#include <asm/uaccess.h>  /* Para las funciones copy_to_user y copy_from_user   */

MODULE_LICENSE("GPL");   /* Licencia del Módulo */
MODULE_DESCRIPTION("Explicación Pŕactica Nro 3 - SO 2017"); /* Desripción del módulo" */
MODULE_AUTHOR("Martin Baez"); /* Autor del módulo */


/* Inicializamos el buffer con el tamaño de una página */
#define BUFFER_LENGTH       PAGE_SIZE

/* El buffer de datos que se leen y escriben al /proc/modulo_so */
static char *modulo_so_buffer;

/* Para el /proc  */
static struct proc_dir *proc_entry;

/* El nombre de nuestro modulo */
static char *modulo_so_name ="module_so";


/* Para leer el contenido del /proc/modulo_so, recordemos que la idea que idea es tranferir datos hacia el espacio de usuario,
desde espacio de usuario están accediendo al contenido de /proc/modulo_so */
static ssize_t modulo_so_read(struct file *filp, char __user *buf, size_t len, loff_t *off){
    int nr_bytes;
  
  if ((*off) > 0) /* No hay nada que leer */
      return 0;
  
  /* Cantidad de bytes que vamos a transferir al espacio de usuario */
  nr_bytes=strlen(modulo_so_buffer);
 
  printk(KERN_INFO "modulo_so se tranfieren al espacio de usuario %d bytes \n", nr_bytes);
   
  if (len<nr_bytes)
    return -ENOSPC;
  
    /* Transferimos el buffer desde espacio de kernel a espacio de usuario */  
  if (copy_to_user(buf, modulo_so_buffer,nr_bytes))
    return -EINVAL;
    
  (*off)+=len;  /* Actualizamos la posición en el archivo(la ultima, leemos todo de un saque */

  return nr_bytes;  


}

/* Para escribir contenido al /proc/modulo_so, recordemos que la idea es tranferir desde el
espacio de usuario, desde el espacio de usuario están escribiendo hacien el /proc/modulo_so */
static ssize_t modulo_so_write(struct file *filp, const char __user *buf, size_t len, loff_t *off) {
  
  int available_space = BUFFER_LENGTH-1;

   printk(KERN_INFO "modulo_so, se tranfiere desde el espacio de usuario %zu bytes \n",len);

  if ((*off) > 0) /* Siempre leemos y escribimos todo de un saque !! */
    return 0;
  
  if (len > available_space) {
    printk(KERN_INFO "modulo_so, no hay espacio para el buffer!! \n");
    return -ENOSPC;
  }
  
  /* Transferimos datos desde el espacio de usuario al espacio de kernel */
  if (copy_from_user( &modulo_so_buffer[0], buf, len ))  
    return -EFAULT;

  modulo_so_buffer[len] = '\0'; /* Add the `\0' */  
  *off+=len;            /* se actualiza el puntero del archivo */
  
  return len;
 
}

/* Declaramos las operaciones que leeran y escribiran datos al /proc/modulo_so */
static const struct file_operations proc_entry_fops = {
   .read = modulo_so_read,
   .write = modulo_so_write,
};


/* Función que se invoca cuando se carga el módulo en el kernel */	
int modulo_so_init(void){

   int ret = 0;

   printk(KERN_INFO "Comienza la carga de modulo_so \n");
  
   /* Reservamos memoria para el buffer */
   modulo_so_buffer = (char *)vmalloc( BUFFER_LENGTH );

   printk(KERN_INFO "modulo_so, espacio total en el Buffer: %lu bytes",BUFFER_LENGTH);

    if (!modulo_so_buffer) {
       ret = -ENOMEM;
     } else {

        /* Hacemos uso efectivo de la memoria para el buffer */
        memset( modulo_so_buffer, 0, BUFFER_LENGTH );

        printk(KERN_INFO "modulo_so, se crea la entrada /proc/modulo_so \n");

       /* Inicializamos la entrada del /proc */
       proc_entry = proc_create( modulo_so_name, 0666, NULL, &proc_entry_fops);
  }

   printk(KERN_INFO "Finaliza la carga de modulo_so \n");

   /* Devolver 0 para indocar una carga correcta del módulo */
   return ret;
}

/* Función que se invoca cuando se descarga el módulo del kernel */
void modulo_so_clean(void){
   printk(KERN_INFO "modulo_so descargado \n");

    /* Borramos la entrada del /proc */
   remove_proc_entry(modulo_so_name, NULL);
}



/* Declaración de las funciones init y cleanup */
module_init(modulo_so_init);
module_exit(modulo_so_clean);
