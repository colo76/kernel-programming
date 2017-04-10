#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/tty.h> 
#include <linux/kd.h>
#include <linux/vt_kern.h>


struct tty_driver* kbd_driver = NULL;


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Ejemplo para acceder a la struct tty_driver");
MODULE_AUTHOR("Martín Baez");

int tty_inspector_init(void) {
   
  int ret=0;
  printk(KERN_INFO "Se carga el driver tty_inspector");
  return ret;
}

void tty_inspector_exit(void) {
  printk(KERN_INFO "Se descarga el módulo tty_inspector");
}

module_init(tty_inspector_init);	
module_exit(tty_inspector_exit);
