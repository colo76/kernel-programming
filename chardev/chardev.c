/*
Crea un dispositvo de entrada salida Implementa la función ioctl para comunicar
señales de control con el proceso de usuario
*/

#include <linux/kernel.h>  /* Funciones del kernel */
#include <linux/fs.h>        /*  get_user y put_user, struct inode  */
#include <asm/uaccess.h>
#include <linux/module.h>   /* Funciones de módulos  */

#include "chardev.h"

#define SUCCESS 0
#define DEVICE_NAME "char_dev_so"
#define BUF_LEN 80

/* Para prevenir acceso concurrente al mismo dispositivo */
static int Device_Open = 0;

/* El mensaje que el dispoisitivo dará cuando se solicite */
static char Message[BUF_LEN];

/*
Posición actual de lectura. Util si el tamaño del mensaje
excede al tamaño del buffer que hay que llenar en device_read 
*/
static char *Message_Ptr;

/*
Llamada cuando cuando un proceso intenta abrir el dispositivo
*/
static int device_open(struct inode *inode, struct file *file) {
// #ifdef DEBUG
      printk(KERN_INFO "device_open(%p)\n",file);
// #endif 

 /* Solo un proceso a la vez */
 if (Device_Open) {
    return -EBUSY;
 }

 Device_Open++;
  
 /* Se inicializa el mensaje */
 Message_Ptr = Message;

 try_module_get(THIS_MODULE);

 return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file) {
//#ifdef DEBUG
	printk(KERN_INFO "device_release(%p,%p)\n", inode, file);
//#endif
/*
* Listo para el siguiente invocador
*/
Device_Open--;
module_put(THIS_MODULE);
return SUCCESS;
}

/*
Esta función es invocada cuando un proceso que abrió el dispositivo 
intenta leer de el
*/

static ssize_t device_read(struct file *file, char __user * buffer, size_t length,loff_t * offset){

int bytes_read = 0;

//#ifdef DEBUG
  printk(KERN_INFO "device_read(%p,%p,%ld)\n", file, buffer, length);
//#endif

 if (*Message_Ptr == 0)
    return 0;

/*
* Escribe datos en el buffer
*/
 while (length && *Message_Ptr) {
 	put_user(*(Message_Ptr++), buffer++);
        length--;
	bytes_read++;
 }

//#ifdef DEBUG
   printk(KERN_INFO "Read %d bytes \n", bytes_read);
//#endif

 return bytes_read;
}

/* Función que intenta escribir al dispositivo */
static ssize_t device_write(struct file *file, const char __user * buffer, size_t length, loff_t * offset) {
 int i;
 //#ifdef DEBUG
   printk(KERN_INFO "device_write(%p,%s,%ld)", file, buffer, length);
 //#endif

 for (i = 0; i < length && i < BUF_LEN; i++)
     get_user(Message[i], buffer + i);

 Message_Ptr = Message;
 return i;
}

/*
Esta función es invocada siempre que un proceso intenta invocar ioctl en nuestro dispositivo
Tenemos los siguientes parámetros:
  El inodo
  La struct del archivo
  El número de ioctl invocada
  El parámetro para la función ioctl 

  Si la ioctl es leer  o leer/ecribir, la ioctl retorna la salida de esta función  

*/

int device_ioctl(struct inode *inode, struct file *file,unsigned int ioctl_num, unsigned long ioctl_param) {
 int i;
 char *temp;
 char ch;

 printk(KERN_INFO "device_ioctl NRO: %u con param %ld \n", ioctl_num,ioctl_param);
 
  /*
   Switch según la ioctl invocada
  */
 switch (ioctl_num) {
 case IOCTL_SET_MSG:
         printk(KERN_INFO "  En device_ioctl IOCTL_SET_MSG ");  
        /* Recibe un puntero al mensaje(espacio de usuario) */  
	temp = (char *)ioctl_param;
	get_user(ch, temp);
	for (i = 0; ch && i < BUF_LEN; i++, temp++)
		device_write(file, (char *)ioctl_param, i, 0);
	break;

 case IOCTL_GET_MSG:
        printk(KERN_INFO "  En device_ioctl IOCTL_GET_MSG ");  
	i = device_read(file, (char *)ioctl_param, 99, 0);
	put_user('\0', (char *)ioctl_param + i);
	break;

 case IOCTL_GET_NTH_BYTE:
        printk(KERN_INFO "  En device_ioctl IOCTL_GET_NTH_BYTE ");  
        return Message[ioctl_param];
	break;
 }
 return SUCCESS;
}



struct file_operations Fops = {
 .read = device_read,
 .write = device_write,
 .unlocked_ioctl = device_ioctl,
 .open = device_open,
 .release = device_release,
 /* a.k.a. close */
};

int init_module() {

 int ret_val;
 
 ret_val = register_chrdev(MAJOR_NUM, DEVICE_NAME, &Fops);

 if (ret_val < 0) {
     printk(KERN_ALERT "%s failed with %d\n","Sorry, registering the character device ", ret_val);
     return ret_val;
 }

 printk(KERN_INFO "%s The major device number is %d.\n","Registeration is a success", MAJOR_NUM);
 printk(KERN_INFO "If you want to talk to the device driver,\n");
 printk(KERN_INFO "you'll have to create a device file. \n");
 printk(KERN_INFO "We suggest you use:\n");
 printk(KERN_INFO "mknod %s c %d 0\n", DEVICE_FILE_NAME, MAJOR_NUM);
 printk(KERN_INFO "The device file name is important, because\n");
 printk(KERN_INFO "the ioctl program assumes that's the\n");
 printk(KERN_INFO "file you'll use.\n");

 printk(KERN_INFO "IOCTL_SET_MSG: %ld \n",IOCTL_SET_MSG);
 printk(KERN_INFO "IOCTL_GET_MSG: %ld \n",IOCTL_GET_MSG);
 printk("IOCTL_GET_NTH_BYTE: %ld \n",IOCTL_GET_NTH_BYTE);

 return 0;
}



/*
* Cleanup − unregister the appropriate file from /proc
*/
void cleanup_module() {
 //int ret;
 //ret = 
 unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
// if (ret < 0)
//  printk(KERN_ALERT "Error: unregister_chrdev: %d\n", ret);
}
