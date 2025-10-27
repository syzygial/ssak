#include <linux/module.h>
#include <linux/printk.h>

int init_module() {
  pr_info("Hello world\n");
  return 0;
}

void cleanup_module() {
  pr_info("Goodbye world\n");
}

MODULE_LICENSE("GPL");
