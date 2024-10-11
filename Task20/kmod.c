#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/rwlock.h>
#include <linux/string.h>
#include <linux/proc_fs.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>

#define STR_SIZE 30
#define MOD_NAME "kmod"
#define PROC_NAME "test_proc"
#define SYS_NAME "test_kobject_sysfs"

static int major = 0;
static struct proc_dir_entry* test_proc = NULL;
static struct kobject* test_kobj_sysfs = NULL; 
static char module_string[STR_SIZE] = "Hello!\0";

/* Работа с файлами устройств */
static ssize_t test_read (struct file* fd, char __user *buff, size_t size, loff_t* off) 
{
    size_t rc;
    
    rc = simple_read_from_buffer(buff, size, off, module_string, STR_SIZE);
    
    return rc;
}

static ssize_t test_write (struct file* fd, const char __user *buff, size_t size, loff_t* off) 
{
    size_t rc = 0;
    if (size > STR_SIZE)
        return -EINVAL;    
    
    rc = simple_write_to_buffer(module_string, STR_SIZE, off, buff, size);    

    return rc;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = test_read,
    .write = test_write
};

/* Работа с файловой системой proc */
static ssize_t proc_test_read (struct file* fd, char __user *buff, size_t size, loff_t* off) 
{
    size_t rc;
    
    rc = simple_read_from_buffer(buff, size, off, module_string, STR_SIZE);
    
    return rc;
}

static ssize_t proc_test_write (struct file* fd, const char __user *buff, size_t size, loff_t* off) 
{
    size_t rc = 0;
    if (size > STR_SIZE)
        return -EINVAL;    
    
    rc = simple_write_to_buffer(module_string, STR_SIZE, off, buff, size);    

    return rc;
}

/* Для версии ядра старше 5.6.0 */
static const struct proc_ops pops = {
    .proc_read = proc_test_read,
    .proc_write = proc_test_write
};

/* Работа с файловой системой sysfs */
static ssize_t string_show (struct kobject* kobj, struct kobj_attribute* attr, char* buff) 
{
    ssize_t rc = 0;
    
    memcpy(buff, module_string, STR_SIZE);
    rc = strlen(module_string);
    
    return rc;
}

static ssize_t string_store (struct kobject* kobj, struct kobj_attribute* attr, const char* buff, size_t count) 
{
    ssize_t rc = 0;

    if (count > STR_SIZE)
        return -EINVAL;
    memcpy(module_string, buff, count);
    rc = count;

    return rc;
}

static struct kobj_attribute string_attribute = 
    __ATTR(module_string, 0664, string_show, string_store);

static struct attribute* attrs[] = {
    &string_attribute.attr,
    NULL
};

static struct attribute_group attr_group = {
    .attrs = attrs
};

int init_module (void) 
{
    int retval = 0;

    pr_info("Test module %s is loaded!\n", MOD_NAME);
    
    major = register_chrdev(major, MOD_NAME, &fops);
    if (major < 0) 
    {
        pr_info("Error: Could not register %s\n", MOD_NAME);
        return major;
    }        

    pr_info("Major number for module %s is %d.\n", MOD_NAME, major);

    test_proc = proc_create(PROC_NAME, 0666, NULL, &pops);
    if (test_proc == NULL) 
    {
        remove_proc_entry(PROC_NAME, NULL);
        pr_info("Error: Could not initialize /proc/%s\n", PROC_NAME);
        return -ENOMEM;
    }

    test_kobj_sysfs = kobject_create_and_add(SYS_NAME, kernel_kobj);
    if (!test_kobj_sysfs) 
    {
        return -ENOMEM;
    }

    retval = sysfs_create_group(test_kobj_sysfs, &attr_group);
    if (retval) 
    {
        kobject_put(test_kobj_sysfs);
        pr_info("Error: Failed to create the myvariable file in /sys/kernel/%s\n", SYS_NAME);
        return retval;
    }

    return 0;
}

void cleanup_module (void) 
{
    unregister_chrdev(major, MOD_NAME);
    remove_proc_entry(PROC_NAME, NULL);
    kobject_put(test_kobj_sysfs);
    pr_info("Test module %s is unloaded!\n", MOD_NAME);
}

MODULE_LICENSE("GPL");
