#include "LED.h"
void LED_Setup(){
    pinMode(LED_PIN, OUTPUT);
}
void LED_On(){
    digitalWrite(LED_PIN, HIGH);
}
void LED_Off(){
    digitalWrite(LED_PIN, LOW);
}