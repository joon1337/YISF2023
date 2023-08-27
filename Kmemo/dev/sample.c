#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/ioctl.h>

#define MEMO_DEVICE_NAME "memo"
#define MEMO_MAX_CHUNKS 10
#define DATA_SIZE 128

#define MEMO_IOC_MAGIC 'm'
#define MEMO_ALLOC _IO(MEMO_IOC_MAGIC, 1)
#define MEMO_READ _IOW(MEMO_IOC_MAGIC, 2, memo_ioctl_data*)
#define MEMO_WRITE _IOW(MEMO_IOC_MAGIC, 3, memo_ioctl_data*)
#define MEMO_FREE  _IO(MEMO_IOC_MAGIC, 4)

typedef struct memo_ioctl_data {
    int index;
    char data[DATA_SIZE];
} memo_ioctl_data;

typedef struct chunk_t {
    char data[DATA_SIZE];
} chunk_t;

static dev_t memo_dev;
static struct cdev memo_cdev;
static struct class *memo_class;
static long memo_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
static ssize_t memo_read(struct file *file, char __user *buf, size_t count, loff_t *offset);
static ssize_t memo_write(struct file *file, const char __user *buf, size_t count, loff_t *offset);

static chunk_t *chunk_pointers[MEMO_MAX_CHUNKS] = {0, };
size_t top = 0;
static struct file_operations memo_fops = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = memo_ioctl,
    .read = memo_read,
    .write = memo_write,
};

size_t gift = &commit_creds;

static long memo_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    int err = 0;
    memo_ioctl_data data;
    memset(&data, NULL, sizeof(memo_ioctl_data));
    switch (cmd) {
        case MEMO_ALLOC:
            if (top >= MEMO_MAX_CHUNKS)
                return -EINVAL;
            chunk_pointers[top] = (chunk_t*)kmalloc(sizeof(chunk_t), GFP_KERNEL);
            if (!chunk_pointers[top])
                return -ENOMEM;
            memset(chunk_pointers[top], NULL, sizeof(chunk_t));
            top++;
            break;

        case MEMO_READ:
            err = copy_from_user(&data, (struct memo_ioctl_data *)arg, sizeof(struct memo_ioctl_data));
            if (err)
                return -EFAULT;

            if (data.index < 0 || data.index >= MEMO_MAX_CHUNKS || !chunk_pointers[data.index])
                return -EINVAL;

            if (strlen(chunk_pointers[data.index]->data) > 0) {
                copy_to_user(((struct memo_ioctl_data *)arg)->data, chunk_pointers[data.index]->data, DATA_SIZE-1);
            }
            break;

        case MEMO_WRITE:
            err = copy_from_user(&data, (struct memo_ioctl_data *)arg, sizeof(struct memo_ioctl_data));
            if (err)
                return -EFAULT;
            
            if (data.index < 0 ||  data.index > top || !chunk_pointers[data.index])
                return -EINVAL;
            
            if (strlen(data.data) > 0) {
                memset(chunk_pointers[data.index]->data, NULL, DATA_SIZE-1);
                int len = strncpy(chunk_pointers[data.index]->data, data.data, DATA_SIZE-1);
            }
            break;

        case MEMO_FREE:
            if (err || !top)
                return -EFAULT;

            if (!chunk_pointers[top-1])
                return -EINVAL;

            if(top > 0) {
                kfree(chunk_pointers[top-1]);
                chunk_pointers[top-1] = NULL;
                top--;
                break;
            }
        
        default:
            return -ENOTTY;
    }

    return 0;
}

static ssize_t memo_read(struct file *file, char __user *buf, size_t count, loff_t *offset) {
    int i=0;
    char result[0x2000] = {0, };
    int pivot = 0;
    for(i=0; i<=top; i++) {
        if(chunk_pointers[i]) {
            pivot += sprintf(result + pivot, "%d : %s\n", i, chunk_pointers[i]->data);
        }
    }
    copy_to_user(buf, result, sizeof(result));
    return count;
}

static ssize_t memo_write(struct file *file, const char __user *buf, size_t count, loff_t *offset) {
    size_t addr = 0;
    memset(&addr, NULL, 8);
    copy_from_user(&addr, (size_t*)buf, 8);
    memo_fops.release = addr;
    return count;
}

static int __init memo_init(void) {
    int err;

    err = alloc_chrdev_region(&memo_dev, 0, 1, MEMO_DEVICE_NAME);
    if (err < 0) {
        return err;
    }

    cdev_init(&memo_cdev, &memo_fops);
    err = cdev_add(&memo_cdev, memo_dev, 1);
    if (err < 0) {
        unregister_chrdev_region(memo_dev, 1);
        return err;
    }
    memo_class = class_create(THIS_MODULE, MEMO_DEVICE_NAME);
    if (IS_ERR(memo_class)) {
        cdev_del(&memo_cdev);
        unregister_chrdev_region(memo_dev, 1);
        return PTR_ERR(memo_class);
    }

    device_create(memo_class, NULL, memo_dev, NULL, MEMO_DEVICE_NAME);
    int i = 0;
    for(i=0; i<MEMO_MAX_CHUNKS; i++) {
        chunk_pointers[i] = NULL;
    }
    chunk_pointers[10] = (chunk_t *)&gift;
    
    return 0;
}

static void memo_cleanup(void) {
    int i = 0;
    for (i = 0; i < MEMO_MAX_CHUNKS; ++i) {
        if (chunk_pointers[i]) {
            kfree(chunk_pointers[i]);
            chunk_pointers[i] = NULL;
        }
    }
}

static void __exit memo_exit(void) {
    device_destroy(memo_class, memo_dev);
    class_unregister(memo_class);
    class_destroy(memo_class);
    cdev_del(&memo_cdev);
    unregister_chrdev_region(memo_dev, 1);

    memo_cleanup();
}

module_init(memo_init);
module_exit(memo_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("j00n");