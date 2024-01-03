#ifndef CONFIG_H
#define CONFIG_H
#include "Arduino.h"
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include "time.h"
#include <WiFiUdp.h>
#include <Wire.h>
#include <ArduCAM.h>
#include <SPI.h>
#include "memorysaver.h"



#define DEVICE_ID 2

//network defines
#define SSID        "MikroLAB"
#define PASSWORD    "@2wsxdr%5"

#define CS 5

#define LED_PIN 32

#define PORT 5000
#define HOST "172.17.10.22" // ip or dns


#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  3600        /* Time ESP32 will go to sleep (in seconds) */

#define IMAGES_NR_OF 2

#endif