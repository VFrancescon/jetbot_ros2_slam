#include <iostream>

int main(){
	int bus;

	/* Open i2c bus /dev/i2c-0 */
	if ((bus = i2c_open("/dev/i2c-0")) == -1) {

	/* Error process */
	}		

	I2CDevice device;
	memset(&device, 0, sizeof(device));

	/* 60, either motor 1 or 2 */
	device.bus = bus;	/* Bus 0 */
	device.addr = 0x60;	/* Slave address is 0x60, 7-bit */
	device.iaddr_bytes = 1;	/* Device internal address is 1 byte */
	device.page_bytes = 16; /* Device are capable of 16 bytes per page */

	unsigned char buffer[1] = {0.5};
	ssize_t size = sizeof(buffer);
	memset(buffer, 0, sizeof(buffer));

	//ssize_t i2c_read(const I2CDevice *device, unsigned int iaddr, void *buf, size_t len);
	i2c_write(&device, device.addr, buffer, size);

	i2c_close(bus);

	return 0;
}
