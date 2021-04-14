#include <unistd.h>			//Needed for I2C port
#include <fcntl.h>			//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
#include "smbus.h"
#include "smbus.c"
#include <stdio.h>
#include <stdlib.h>


void main(){

	int file;
	int adapter_nr = 0; /* probably dynamically determined */
	char filename[20];

	snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);
	file = open(filename, O_RDWR);
	if (file < 0) {
	/* ERROR HANDLING; you can check errno to see what went wrong */
	exit(1);
	}

	int addr = 0x0c; /* The I2C address */

  	if (ioctl(file, I2C_SLAVE, addr) < 0) {
    	/* ERROR HANDLING; you can check errno to see what went wrong */
    	exit(1);
  	}

  	char buf[10];

	__u8 st1_reg = 0x10;
	__u8 hxl_reg = 0x11;
	__u8 hxh_reg = 0x12;
	__u8 st2_reg = 0x18;
	__u8 cntl2_reg = 0x31;
	__u8 cntl3_reg = 0x32;
	__u8 st2_res, xl_res, xh_res;
	int x_res;
	double flux_density;

	i2c_smbus_write_byte_data(file, cntl3_reg, 0x01); //write 1 in reset register
	i2c_smbus_write_byte_data(file, cntl2_reg, 0x02); //write 2 in control 2 reg, continuous mode

	__u8 xl_res_temp = i2c_smbus_read_byte_data(file, hxl_reg);
	__u8 xh_res_temp = i2c_smbus_read_byte_data(file, hxh_reg);
	printf("All good before the loop sonny\n");
	printf("Prelim values for x: 8msb %x 8lsb %x\n", xh_res_temp, xl_res_temp);

	while(1){
		while( i2c_smbus_read_byte_data(file, st1_reg) != 0x01){

			xl_res = i2c_smbus_read_byte_data(file, hxl_reg);
			xh_res = i2c_smbus_read_byte_data(file, hxh_reg);	
			st2_res = i2c_smbus_read_byte_data(file, st2_reg);
			if(st2_res == 0b0100){
			//	//printf("Magnetc sensor overflow (?)\n");
				break;
			}	
			if(xl_res != xl_res_temp){
				//printf("8msb: %x", xh_res);
				//printf(" 8lsb: %x\n", xl_res);
				x_res = (xh_res<<8)+xl_res;
				if(x_res > 32752){
					x_res = x_res-32752;
					//printf("reducing negative value\n");
					flux_density = -(( (double) x_res/32752)*4912);
				}
				else{ flux_density = (double) x_res/32752*4912;}
				printf("Value being read is %x, flux density of %f\n", x_res, flux_density);
				xl_res_temp = xl_res;
				xh_res_temp = xh_res;
			}
		}

	}

}
