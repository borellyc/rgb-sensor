#include "ISL29125.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include "ISL29125.h"

ISL29125::ISL29125(const char* dev):i2c_dev(dev) {
  ISL29125::init();
}
ISL29125::~ISL29125() {
  if (file>0) {
    close(file);
  }
}
void ISL29125::init() {
  initOK=false;
  // OPEN BUS
  if ((file=open(i2c_dev,O_RDWR))<0) {
    fprintf(stderr,"Failed to open the i2c bus (%s): %s\n",
            i2c_dev,strerror(errno));
    return;
  }
  // Connect to device
  if (ioctl(file,I2C_SLAVE,ISL_I2C_ADDR)<0) {
    fprintf(stderr,"Failed to acquire bus access and/or talk to slave: %s\n",
            strerror(errno));
    return;
  }
  // Check if device ID is correct
  uint8_t device_id=read8(ISL_DEVICE_ID);
  if (!(device_id==ISL_DEVICE_ID_VAL)) {
    fprintf(stderr,"Device not found (%02x <> %02x) !\n");
  } else {
    initOK=true;
    write8(ISL_CONFIG_1,ISL_CFG1_MODE_RGB|ISL_CFG1_375LUX);
    write8(ISL_CONFIG_2,ISL_CFG2_IR_ADJUST_HIGH);
    write8(ISL_CONFIG_3,ISL_CFG_DEFAULT);
  }
}
int ISL29125::isOK() {return initOK==true;}
void ISL29125::write8(uint8_t reg,uint8_t data) {
  if (file<0) return;
  uint8_t buffer[2]={};
  buffer[0]=reg;
  buffer[1]=data;
  if (write(file,buffer,2)!=2) {
    fprintf(stderr,"Failed to write byte to the i2c bus: %s\n",strerror(errno));
  }
}
uint8_t ISL29125::read8(uint8_t reg) {
  uint8_t byteBuffer=0;
  if (file<0) return byteBuffer;
  if (write(file,&reg,1)!=1) {
    fprintf(stderr,"Failed to write byte to the i2c bus: %s\n",strerror(errno));
    return byteBuffer;
  }
  if (read(file,&byteBuffer,1)!=1) {
    fprintf(stderr,"Failed to read byte from i2c bus: %s\n",strerror(errno));
  }
  return byteBuffer;
}
uint8_t ISL29125::readRed() {
  uint8_t red_low=read8(ISL_RED_L);
  uint8_t red_high=read8(ISL_RED_H);
  uint8_t red=(red_high<<8)|red_low;
  return red;
}
uint8_t ISL29125::readGreen() {
  uint8_t green_low=read8(ISL_GREEN_L);
  uint8_t green_high=read8(ISL_GREEN_H);
  uint8_t green=(green_high<<8)|green_low;
  return green;
}
uint8_t ISL29125::readBlue() {
  uint8_t blue_low=read8(ISL_BLUE_L);
  uint8_t blue_high=read8(ISL_BLUE_H);
  uint8_t blue=(blue_high<<8)|blue_low;
  return blue;
}
