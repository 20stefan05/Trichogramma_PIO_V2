#include "Arduino.h"
#include "config.h"
#include "camera.h"
#include "LED.h"
#include "wifi_connection.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  LED_Setup();
  WIFIInit();
  cameraInit();
  uint8_t img = IMAGES_NR_OF;
  while(img--){
    LED_On();
    delay(1000);
    postImage();
    delay(1000);
    LED_Off();
    delay(1000);
  }
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
  Serial.println("Going to sleep now");
  Serial.flush(); 
  esp_deep_sleep_start();
}

void loop() {
  // put your main code here, to run repeatedly:
}