/****************矩阵键盘模块*************/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <plat/gpio-cfg.h>
#include <asm/irq.h>
#include <mach/gpio.h>

#define DEVICE_NAME "keys"	//定义一个“keys”的设备名
#define KEYS_MAJOR 	245		//主设备号为245
#define KEYS_MINOR  0		//次设备号为0

struct cdev *mycdev;		//设备指针
struct class *keys_class;	
dev_t devno;				

/*按键数组GPIO列表*/
static unsigned long keys_table[]={
        S5PV210_GPC1(4),S5PV210_GPH0(2),
        S5PV210_GPH2(0),S5PV210_GPH1(4),
        S5PV210_GPC1(2),S5PV210_GPC1(0),
        S5PV210_GPC1(1),S5PV210_GPC1(3),
};
/*设置GPIO输入或输出*/
static unsigned int keys_cfg_table[]={
        S3C_GPIO_INPUT,S3C_GPIO_INPUT,
        S3C_GPIO_INPUT,S3C_GPIO_INPUT,
        S3C_GPIO_OUTPUT,S3C_GPIO_OUTPUT,
        S3C_GPIO_OUTPUT,S3C_GPIO_OUTPUT,
};
/*打开设备文件函数*/
static ssize_t keys_open(struct inode *inode,struct file *filp)
{
	int i;
    for(i=0;i<8;i++)          					
	{
		s3c_gpio_cfgpin(keys_table[i],keys_cfg_table[i]);//设置GPIO的工作模式
	}	
	return 0;
}
/*用按键读取设备文件*/
static ssize_t keys_read(struct file *filp,char __user *buffer,
			   size_t count, loff_t *f_pos)
{
	unsigned char n;
	unsigned char sccode;
	unsigned char data;
	unsigned long temp4;
	unsigned int keys_value,ret;
	// gpio init 
	gpio_set_value(S5PV210_GPC1(2),0);
	gpio_set_value(S5PV210_GPC1(0),0);
	gpio_set_value(S5PV210_GPC1(1),0);
	gpio_set_value(S5PV210_GPC1(3),0);
	keys_value=0;
	if(!(gpio_get_value(S5PV210_GPC1(4)))||!(gpio_get_value(S5PV210_GPH0(2)))||!(gpio_get_value(S5PV210_GPH2(0)))||!(gpio_get_value(S5PV210_GPH1(4))))// any key pressed
	{
		mdelay(100);
		if(!(gpio_get_value(S5PV210_GPC1(4)))||!(gpio_get_value(S5PV210_GPH0(2)))||!(gpio_get_value(S5PV210_GPH2(0)))||!(gpio_get_value(S5PV210_GPH1(4))))// fliter
		{
			sccode=0xfe;data=0xe;
			for(n=0;n<4;n++){	//for scan key value
				if(n==0){
					//printk("n==0\n");
					gpio_set_value(S5PV210_GPC1(2),0);			
					gpio_set_value(S5PV210_GPC1(0),1);
					gpio_set_value(S5PV210_GPC1(1),1);
					gpio_set_value(S5PV210_GPC1(3),1);
					if(!gpio_get_value(S5PV210_GPC1(4))){	//S1
						temp4=0xe0;
						keys_value=temp4+data;
						break;
					}
          				else if(!gpio_get_value(S5PV210_GPH0(2))){	// S2
        					temp4=0xd0;
          					keys_value=temp4+data;
						break;
          				}
          				else if(!gpio_get_value(S5PV210_GPH2(0))){	// S3
          					temp4=0xb0;
						keys_value=temp4+data;
						break;
         				}
          				else if(!gpio_get_value(S5PV210_GPH1(4))){	// S4
						temp4=0x70;
						keys_value=temp4+data;
        					break;
         				}
					
				}
				else if(n==1){
					//printk("n==1\n");
					gpio_set_value(S5PV210_GPC1(2),1);
        			gpio_set_value(S5PV210_GPC1(0),0);
          			gpio_set_value(S5PV210_GPC1(1),1);
          			gpio_set_value(S5PV210_GPC1(3),1);
         			if(!gpio_get_value(S5PV210_GPC1(4))){	// S5
          				temp4=0xe0;
            			keys_value=temp4+data;
          				break;
          			}
					else if(!gpio_get_value(S5PV210_GPH0(2))){	// S6
         				temp4=0xd0;
            			keys_value=temp4+data;
            			break;
         			}
          			else if(!gpio_get_value(S5PV210_GPH2(0))){	// S7
          				temp4=0xb0;
           				keys_value=temp4+data;
            			break;
          			}
          			else if(!gpio_get_value(S5PV210_GPH1(4))){	// S8
						temp4=0x70;
         				keys_value=temp4+data;
           				break;
					}
				}
				else if(n==2){
					//printk("n==2\n");
        			gpio_set_value(S5PV210_GPC1(2),1);
          			gpio_set_value(S5PV210_GPC1(0),1);
          			gpio_set_value(S5PV210_GPC1(1),0);
         			gpio_set_value(S5PV210_GPC1(3),1);
        			if(!gpio_get_value(S5PV210_GPC1(4))){	// S9
          				temp4=0xe0;
            			keys_value=temp4+data;
            			break;
        			}
         			else if(!gpio_get_value(S5PV210_GPH0(2))){	// S10
						temp4=0xd0;
          				keys_value=temp4+data;
            			break;
          			}
         			else if(!gpio_get_value(S5PV210_GPH2(0))){	// S11
         				temp4=0xb0;
            			keys_value=temp4+data;
            			break;
          			}
          			else if(!gpio_get_value(S5PV210_GPH1(4))){	// S12
            			temp4=0x70;
           				keys_value=temp4+data;
           				break;
         			}
				}
				else if(n==3){
					//printk("n==3\n");
        			gpio_set_value(S5PV210_GPC1(2),1);
         			gpio_set_value(S5PV210_GPC1(0),1);
          			gpio_set_value(S5PV210_GPC1(1),1);
        			gpio_set_value(S5PV210_GPC1(3),0);
          			if(!gpio_get_value(S5PV210_GPC1(4))){	// S13
          				temp4=0xe0;
            			keys_value=temp4+data;
            			break;
          			}
         			else if(!gpio_get_value(S5PV210_GPH0(2))){	// S14
						temp4=0xd0;
            			keys_value=temp4+data;
						break;
         			}
          			else if(!gpio_get_value(S5PV210_GPH2(0))){	// S15
          				temp4=0xb0;
            			keys_value=temp4+data;
            			break;
        			}
         	 		else if(!gpio_get_value(S5PV210_GPH1(4))){	// S16
          				temp4=0x70;
            			keys_value=temp4+data;
            			break;
         			}
				}
				data=((data<<1)|0x01)&0x0f;// 0x0e
			}	// for loop	
			
      			ret=copy_to_user(buffer,&keys_value,sizeof(keys_value));
			return sizeof(keys_value);
		}
	}
	return 0;
}

/*驱动层file_operations接口函数初始化*/
static struct file_operations keys_fops = {
	.owner   = 	THIS_MODULE,
	.read    = 	keys_read,
	.open    = 	keys_open,
};
/*驱动程序入口初始化函数，设置keys 向内核注册设备*/
static int __init keys_init(void)
{
	int result;
//注册keys设备
	devno = MKDEV(KEYS_MAJOR, KEYS_MINOR);//获取设备号
    mycdev = cdev_alloc();					
    cdev_init(mycdev, &keys_fops);//初始化字符设备
    result = cdev_add(mycdev, devno, 1);//向系统添加keys设备
	
	if(result<0)
	{
		printk(KERN_WARNING"KEYS:can't get major %d\n",KEYS_MAJOR);
		return result;
	}
	printk(DEVICE_NAME"keys initialized\n");
    keys_class=class_create(THIS_MODULE,"keys");//创建模块
    if(IS_ERR(keys_class))
    {
		printk("err:failed in creating class.\n");
        return -1;
    }
//建立keys设备节点
    device_create(keys_class,NULL, MKDEV(KEYS_MAJOR,0),NULL,DEVICE_NAME);
	return 0;
}
/*驱动卸载函数*/
static void __exit keys_exit(void)
{
	cdev_del(mycdev);
	device_destroy(keys_class,MKDEV(KEYS_MAJOR,0));
    class_destroy(keys_class);
}

module_init(keys_init);//声明驱动程序入口
module_exit(keys_exit);//声明驱动程序出口

MODULE_DESCRIPTION("keys driver for UP_Magic");
MODULE_LICENSE("GPL");

