#include "camera.h"
ArduCAM myCAM(OV5642, CS);

ArduCAM* getCamera(){
    return &myCAM;
}

void start_capture() {
  myCAM.clear_fifo_flag();
  myCAM.start_capture();
}


void cameraInit() {
  uint8_t vid, pid;
  uint8_t temp;
  Wire.begin();
  pinMode(CS, OUTPUT);

  // initialize SPI:
  SPI.begin();
  SPI.setFrequency(4000000); //4MHz

  //Check if the ArduCAM SPI bus is OK
  myCAM.write_reg(ARDUCHIP_TEST1, 0x55);
  temp = myCAM.read_reg(ARDUCHIP_TEST1);
  if (temp != 0x55) {
    Serial.println("SPI1 interface Error!");
    while (1);
  }
  //Check if the camera module type is OV5642
  myCAM.wrSensorReg16_8(0xff, 0x01);
  myCAM.rdSensorReg16_8(OV5642_CHIPID_HIGH, &vid);
  myCAM.rdSensorReg16_8(OV5642_CHIPID_LOW, &pid);
  if ((vid != 0x56) || (pid != 0x42)) {
    Serial.println("Can't find OV5642 module!");
    while (1);
  }
  else
    Serial.println("OV5642 detected.");


  //Change to JPEG capture mode and initialize the OV5642 module
  myCAM.set_format(JPEG);
  myCAM.InitCAM();
  myCAM.write_reg(ARDUCHIP_TIM, VSYNC_LEVEL_MASK);   //VSYNC is active HIGH
  myCAM.OV5642_set_JPEG_size(OV5642_320x240); delay(1000);

}