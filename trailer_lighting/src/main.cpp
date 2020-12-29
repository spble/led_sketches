#include <Arduino.h>
#include <tcmenu_menu.h>
#include <Encoder.h>


int led_pin = 22;
int duty_cycle = 0;
int teensy_led_pin = 13;
int zone_led_pins[5] = {10,20,21,22,23};
int zone_duty_cycle[5] = {0,0,0,0,0};
int rotary_a_pin = 1;
int rotary_b_pin = 0;
int rotary_button_pin = 19;


//U8G2_SH1106_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R2, 14, 7, 9, 10, 5);

void draw(void) {
    pinMode(led_pin, OUTPUT);
    pinMode(teensy_led_pin, OUTPUT);
  //  pinMode(rotary_button_pin, INPUT);
}
void setup(void) {
    Serial.begin(115200);
    setupMenu();
    Serial.write("Starting up!");
}
void loop(void) {
    taskManager.runLoop();
    /*
    u8g2.clearBuffer();
    draw();
    u8g2.sendBuffer();
     */
    // rebuild the picture after some delay
    analogWrite(led_pin, duty_cycle);
    duty_cycle++;
    if(duty_cycle == 256){
        duty_cycle = 0;
    }
    digitalWrite(teensy_led_pin, HIGH);
    delay(50);
    digitalWrite(teensy_led_pin, LOW);
    delay(50);
}

