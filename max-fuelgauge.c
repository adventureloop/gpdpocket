#include <err.h>                   
#include <errno.h>                 
#include <sysexits.h>              
#include <fcntl.h>                 
#include <stdio.h>                 
#include <stdlib.h>                
#include <string.h>                
#include <stdarg.h>                
#include <unistd.h>                
#include <sys/ioctl.h>             
                                   
#include <dev/iicbus/iic.h>        

#define MAX170xx_ADDR 		(0x36 << 1)
#define MAX170xx_REG_SOCVF  0xFF
#define I2C_DEV         	"/dev/iic0"

static int                                             
max170xx_read(int fd, uint8_t reg, uint16_t *val)
{                                                      
    struct iic_msg msg[2];                             
	struct iic_rdwr_data rdwrdata;

    msg[0].slave = MAX170xx_ADDR;                        
    msg[0].flags = IIC_M_WR | IIC_M_NOSTOP;            
    msg[0].len = 1;                                    
    msg[0].buf = &reg;                                 
                                                       
    msg[1].slave = MAX170xx_ADDR;                        
    msg[1].flags = IIC_M_RD;                           
    msg[1].len = sizeof(uint16_t);                     
    msg[1].buf = (uint8_t *)val;                       

	rdwrdata.msgs = msg;
	rdwrdata.nmsgs = 2;   
     
	return ioctl(fd, I2CRDWR, &rdwrdata);
}

int
main()
{
	int fd;
	uint16_t socvf = 0;

	fd = open(I2C_DEV, O_RDWR);
	if (fd == -1) {                                             
		printf("Error opening I2C controller (%s)" I2C_DEV, strerror(errno));
		return (EX_NOINPUT);                                    
	}                                                           

	if (max170xx_read(fd, MAX170xx_REG_SOCVF, &socvf) == -1) {
		perror("reading MAX170xx_REG_SOCVF");
		return (errno);
	} else {
		printf("%d %%\n", ( ((socvf >> 8) * 100) + (((socvf & 0x00FF) *100)/256) )/100); 
	}
	close(fd);
	return (0);
}
