// From https://github.com/zuernm/RGB-Sensor
#include <stdio.h>
#include <string>
#include <unistd.h>
#include "ISL29125.h"
int main(int argc,char **argv) {
  ISL29125 rgb;
  if (!rgb.isOK()) return 1;
  if (argc<=1) {
    printf("Red:   %d\n",rgb.readRed());
    printf("Green: %d\n",rgb.readGreen());
    printf("Blue:  %d\n",rgb.readBlue());
  }
  if (argc==2) {
    if (std::string(argv[1])=="loop") {
      while (true) {
        sleep(1);
        printf("RGB color: %3d %3d %3d\n",
                rgb.readRed(),rgb.readGreen(),rgb.readBlue());
      }
    }
  }
  return 0;
}
